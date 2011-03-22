/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "timedemo.h"
#include "mainpage.h"
#include "timedemobenchmark.h"
#include "templatepage.h"

#include <MApplication>
#include <MWindow>
#include <MSceneManager>
#include <MApplicationPage>

#include <QApplication>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QFile>
#include <QTimer>
#include <QDebug>

namespace
{
    const int pageDuration = 5000;
}

Timedemo::Timedemo(MainPage *mainPage, const QStringList& demoPageTitles)
    : QObject(),
    m_pFrontPage(mainPage),
    benchmarkResults(),
    allBenchmarks(),
    m_currentPageIndex(0),
    m_currentBenchmarkIndex(0),
    demoPages(),
    demoPageTitles(demoPageTitles),
    m_beginFrameCount(0),
    m_beginTime(),
    m_csvFilename(),
    framelogFilename(),
    timingStarted(false),
    timingStopped(false)
{
    connect(m_pFrontPage, SIGNAL(appeared()), this, SLOT(showFirstPage()));
}

void Timedemo::setOutputCsv(const QString &filename)
{
    m_csvFilename = filename;
}

void Timedemo::setFramelog(const QString &filename)
{
    framelogFilename = filename;
}

void Timedemo::startTiming(const TimedemoBenchmark *activeBenchmark)
{
    QSharedPointer<TimedemoBenchmark> benchmark = demoPages[m_currentPageIndex]->benchmarks()[m_currentBenchmarkIndex];
    if (activeBenchmark != benchmark) {
        qFatal("Only the currently running benchmark can stop the timing.");
    }
    if (timingStopped || timingStarted) {
        qFatal("New timing started, but old one not processed yet.");
    }

    timingStarted = true;
    SwapHook::instance()->startLurking();
}

void Timedemo::stopTiming(const TimedemoBenchmark *activeBenchmark)
{
    QSharedPointer<TimedemoBenchmark> benchmark = demoPages[m_currentPageIndex]->benchmarks()[m_currentBenchmarkIndex];
    if (activeBenchmark != benchmark) {
        qFatal("Only the currently running benchmark can stop the timing.");
    }
    if (timingStopped || !timingStarted) {
        qFatal("Timing already processed or not running yet.");
    }

    SwapHook::instance()->stopLurking();

    BenchmarkResult currentResult(SwapHook::instance()->timestamps(), benchmark->type());
    benchmarkResults[m_currentPageIndex].insert(benchmark->name(), currentResult);

    timingStopped = true;
}

void Timedemo::showFirstPage()
{
    if (demoPageTitles.count() == 0) {
        foreach(TimedemoPage *page, m_pFrontPage->demoPages()) {
            demoPages.append(page);
        }
    } else {
        foreach(const QString& title, demoPageTitles) {
            TimedemoPage *page;
            if (title == m_pFrontPage->timedemoTitle()) {
                page = m_pFrontPage;
            } else {
                page = m_pFrontPage->findPageByTimedemoTitle(title);
            }
            if (page) {
                demoPages.append(page);
            } else {
                qWarning() << "Did not find a page for" << title;
            }
        }
    }

    if (demoPages.count() == 0) {
        qWarning() << "No valid demopages found.";
        exit(EXIT_FAILURE);
        return;
    }

    disconnect(m_pFrontPage, SIGNAL(appeared()), this, SLOT(showFirstPage()));
    m_currentPageIndex = 0;
    benchmarkResults.resize(demoPages.count());

    demoPages[m_currentPageIndex]->createBenchmarks(this);
    beginBenchmark();
}

void Timedemo::beginBenchmark()
{
    if (m_currentBenchmarkIndex >= demoPages[m_currentPageIndex]->benchmarks().count()) {
        // all benchmarks have been processed, switch to the next page
        showNextPage();
        return;
    }

    QSharedPointer<TimedemoBenchmark> benchmark = demoPages[m_currentPageIndex]->benchmarks()[m_currentBenchmarkIndex];
    if (!allBenchmarks.contains(benchmark->name())) {
        allBenchmarks.append(benchmark->name());
    }

    connect(benchmark.data(), SIGNAL(finished()), this, SLOT(benchmarkFinished()));

    benchmark->start();
}


void Timedemo::benchmarkFinished()
{
    if (!timingStopped) {
        qFatal("Benchmark did not stop timing.");
    }

    timingStarted = false;
    timingStopped = false;
    ++m_currentBenchmarkIndex;
    beginBenchmark();
}

void Timedemo::showNextPage()
{
    ++m_currentPageIndex;
    m_currentBenchmarkIndex = 0;

    if (m_currentPageIndex < demoPages.count()) {
        TimedemoPage* currentPage = demoPages[m_currentPageIndex];
        demoPages[m_currentPageIndex]->createBenchmarks(this);

        if (currentPage == m_pFrontPage) {
            // FIXME: the front page needs a special invitation.
            // otherwise it does not show up again
            if (MApplication::activeWindow()) {
                // FIXME: why appearSceneWindowNow() and not appear()?
                MApplication::activeWindow()->sceneManager()->appearSceneWindowNow(demoPages[m_currentPageIndex]);
            }
        }
        beginBenchmark();
    } else {
        // all pages shown, display results:
        displayBenchmarkResults();

        qApp->exit(0);
    }
}

void Timedemo::displayBenchmarkResults()
{
    QTextStream log(stdout, QIODevice::WriteOnly | QIODevice::Text);

    QFile statsCsvFile;
    if (m_csvFilename.isEmpty()) {
        statsCsvFile.setFileName("/dev/null");
    } else {
        statsCsvFile.setFileName(m_csvFilename);
    }

    if ( ! statsCsvFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
    {
        qCritical( "failed to open stats file: %s", qPrintable( statsCsvFile.fileName() ) );
        exit(EXIT_FAILURE);
    }

    QTextStream statsCsv(&statsCsvFile);

    int pageTitleWidth = 0;
    const int fpsWidth = 5;
    const int fpsUnitWidth = 5;
    const int runtimeWidth = 5;
    const int runtimeUnitWidth = 2;
    const int benchmarkWidth = fpsWidth + fpsUnitWidth + runtimeWidth + runtimeUnitWidth;
    QHash<QString, int> actualWidth;

    for (int i = 0; i < benchmarkResults.count(); ++i) {
        TimedemoPage *page = demoPages[i];
        QString title = page->timedemoTitle();
        pageTitleWidth = qMax(pageTitleWidth, title.length());
    }
    pageTitleWidth += 2;

    log.setRealNumberNotation(QTextStream::FixedNotation);
    log.setRealNumberPrecision(2);
    log << left << qSetFieldWidth(pageTitleWidth) << "Page name";
    statsCsv << "Page name";

    foreach(const QString& name, allBenchmarks) {
        int width = qMax(benchmarkWidth + 2, name.length() + 2);
        log << qSetFieldWidth(width) << name;
        statsCsv << ", \"" << name << " [fps]\", \"" << name << " [ms]\"";

        actualWidth[name] = width;
    }
    log << qSetFieldWidth(0) << '\n';
    statsCsv << '\n';

    for (int i = 0; i < benchmarkResults.count(); ++i) {
        TimedemoPage *page = demoPages[i];
        QString title = page->timedemoTitle();

        log << qSetFieldWidth(pageTitleWidth) << title;
        statsCsv << "\"" << title << "\"";

        BenchmarkResultHash results = benchmarkResults[i];
        foreach(const QString& name, allBenchmarks) {
            statsCsv << ", ";
            BenchmarkResultHash::const_iterator resultIter = results.find(name);
            if (resultIter != results.constEnd() && resultIter->fps() != 0 && resultIter->runtime() != 0) {
                log << right
                    << qSetFieldWidth(fpsWidth) << resultIter->fps()
                    << qSetFieldWidth(fpsUnitWidth) << "fps |"
                    << qSetFieldWidth(runtimeWidth) << resultIter->runtime()
                    << qSetFieldWidth(runtimeUnitWidth) << "ms"
                    << qSetFieldWidth(actualWidth[name] - benchmarkWidth) << ""
                    << left;
            } else {
                log << qSetFieldWidth(actualWidth[name]) << center << "n/a" << left;
            }
            statsCsv << resultIter->fps() << ", " << resultIter->runtime();
        }
        log << qSetFieldWidth(0) << '\n';
        statsCsv << '\n';
    }

    log.flush();
    statsCsv.flush();
    saveFramelog();
}

void Timedemo::saveFramelog() {
    QFile framelogFile;
    if (framelogFilename.isEmpty()) {
        return;
    } else {
        framelogFile.setFileName(framelogFilename);
    }

    if (!framelogFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        qWarning( "failed to open frame log file: %s", qPrintable( framelogFile.fileName() ) );
    }

    QXmlStreamWriter framelog(&framelogFile);
    framelog.setAutoFormatting(true);
    framelog.writeStartDocument();
    framelog.writeDTD("<!DOCTYPE timedemo>");
    framelog.writeStartElement("timedemo");

    for (int i = 0; i < benchmarkResults.count(); ++i) {
        framelog.writeStartElement("page");
        TimedemoPage *page = demoPages[i];
        QString title = page->timedemoTitle();

        framelog.writeAttribute("id", title);

        BenchmarkResultHash results = benchmarkResults[i];
        foreach(const QString& name, allBenchmarks) {
            BenchmarkResultHash::const_iterator resultIter = results.find(name);
            if (resultIter == results.end() || resultIter->runtime() == 0 ||  resultIter->fps() == 0) {
                continue;
            }
            qint64 firstTs = resultIter->timestamps.first();
            framelog.writeStartElement("benchmark");
            framelog.writeAttribute("id", name);
            framelog.writeAttribute("start", QString::number(static_cast<qulonglong>(firstTs)));
            framelog.writeAttribute("end", QString::number(static_cast<qulonglong>(firstTs + resultIter->runtime())));
            framelog.writeTextElement("type", resultIter->type);
            framelog.writeTextElement("runtime", QString::number(resultIter->runtime()));
            QString timestamps;
            foreach(const qint64& ts, resultIter->timestamps) {
                timestamps.append(QString::number(ts - firstTs) + ',');
            }
            timestamps.truncate(timestamps.size() - 1);
            framelog.writeTextElement("frames", timestamps);

            // "page"
            framelog.writeEndElement();
        }

        // "benchmark"
        framelog.writeEndElement();
    }

    framelog.writeEndElement();
    framelog.writeEndDocument();

    framelogFile.flush();
}
