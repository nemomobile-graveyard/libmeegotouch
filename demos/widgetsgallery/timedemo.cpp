/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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
#include "timingscene.h"
#include "listpage.h"
#include "timedemobenchmark.h"


#include <DuiApplicationPage>

#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QTimer>

namespace
{
    const int pageDuration = 5000;
}

Timedemo::Timedemo(TimingScene *timingScene, ListPage *listPage)
    : timingScene(timingScene)
    , m_pFrontPage(listPage)
    , m_currentPageIndex(0)
    , m_currentBenchmarkIndex(0)
    , timingStarted(false)
    , timingStopped(false)
{
    connect(m_pFrontPage, SIGNAL(windowShown()), this, SLOT(showFirstPage()));
}

void Timedemo::setOutputCsv(const QString &filename)
{
    m_csvFilename = filename;
}

void Timedemo::startTiming()
{
    if (timingStopped || timingStarted) {
        qFatal("New timing started, but old one not processed yet.");
    }

    timingStarted = true;
    m_beginFrameCount = timingScene->frameCount();
    m_beginTime = QTime::currentTime();
    timingScene->setUpdateContinuously(true);
}

void Timedemo::stopTiming()
{
    if (timingStopped || !timingStarted) {
        qFatal("Timing already processed or not running yet.");
    }

    timingScene->setUpdateContinuously(false);
    QTime endTime = QTime::currentTime();
    uint endFrameCount = timingScene->frameCount();

    BenchmarkResult currentResult;
    currentResult.runtime = m_beginTime.msecsTo(endTime);
    currentResult.fps = qreal(endFrameCount - m_beginFrameCount)
                / qMax<uint>(currentResult.runtime, 1) * 1000;

    QSharedPointer<TimedemoBenchmark> benchmark = currentPage->benchmarks()[m_currentBenchmarkIndex];
    benchmarkResults[m_currentPageIndex].insert(benchmark->name(), currentResult);

    timingStopped = true;
}

void Timedemo::showFirstPage()
{
    disconnect(m_pFrontPage, SIGNAL(windowShown()), this, SLOT(showFirstPage()));
    m_currentPageIndex = 0;
    benchmarkResults.resize(m_pFrontPage->pageCount() + 1);
    currentPage = dynamic_cast<TimedemoPage*>(m_pFrontPage->findPageByIndex(0));
    if(!currentPage) {
        qFatal("%s does not inherit from TimedemoPage", qPrintable(currentPage->title()));
    }
    currentPage->createBenchmarks(this);
    beginBenchmark();
}

void Timedemo::beginBenchmark()
{
    if (m_currentBenchmarkIndex >= currentPage->benchmarks().count()) {
        // all benchmarks have been processed, switch to the next page
        showNextPage();
        return;
    }

    QSharedPointer<TimedemoBenchmark> benchmark = currentPage->benchmarks()[m_currentBenchmarkIndex];
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

    if (m_currentPageIndex < m_pFrontPage->pageCount()) {
        currentPage = static_cast<TimedemoPage*>(m_pFrontPage->findPageByIndex(m_currentPageIndex));
        currentPage->createBenchmarks(this);
        beginBenchmark();
    } else if (m_currentPageIndex == m_pFrontPage->pageCount()) {
        currentPage = m_pFrontPage;
        // FIXME: the front page needs a special invitation.
        // otherwise it does not show up again
        currentPage->appearNow();
        currentPage->createBenchmarks(this);
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

    QFile csvFile;
    QTextStream csv;

    if (m_csvFilename.isEmpty()) {
        csvFile.setFileName("/dev/null");
    } else {
        csvFile.setFileName(m_csvFilename);
    }
        csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        csv.setDevice(&csvFile);

    const int pageTitleWidth = 30;
    const int fpsWidth = 5;
    const int fpsUnitWidth = 5;
    const int runtimeWidth = 5;
    const int runtimeUnitWidth = 2;
    const int benchmarkWidth = fpsWidth + fpsUnitWidth + runtimeWidth + runtimeUnitWidth;
    QHash<QString, int> actualWidth;

    log.setRealNumberNotation(QTextStream::FixedNotation);
    log.setRealNumberPrecision(2);
    log << left << qSetFieldWidth(pageTitleWidth) << "Page name";
    csv << "Page name";

    foreach(const QString& name, allBenchmarks) {
        int width = qMax(benchmarkWidth + 2, name.length() + 2);
        log << qSetFieldWidth(width) << name;
        csv << ", \"" << name << " [fps]\", \"" << name << " [ms]\"";

        actualWidth[name] = width;
    }
    log << qSetFieldWidth(0) << '\n';
    csv << '\n';

    for (int i = 0; i < benchmarkResults.count(); ++i) {
        DuiApplicationPage *page = (i < m_pFrontPage->pageCount()) ? m_pFrontPage->findPageByIndex(i) : m_pFrontPage;
        QString title = page->title();
        if (title.length() > pageTitleWidth) {
            title.truncate(pageTitleWidth - 5);
            title.append("...");
        }
        log << qSetFieldWidth(pageTitleWidth) << title;
        csv << "\"" << title << "\"";

        BenchmarkResultHash results = benchmarkResults[i];
        foreach(const QString& name, allBenchmarks) {
            csv << ", ";
            BenchmarkResultHash::const_iterator resultIter = results.find(name);
            if (resultIter != results.constEnd() && resultIter->fps != 0 && resultIter->runtime != 0) {
                log << right
                    << qSetFieldWidth(fpsWidth) << resultIter->fps
                    << qSetFieldWidth(fpsUnitWidth) << "fps |"
                    << qSetFieldWidth(runtimeWidth) << resultIter->runtime
                    << qSetFieldWidth(runtimeUnitWidth) << "ms"
                    << qSetFieldWidth(actualWidth[name] - benchmarkWidth) << ""
                    << left;
                csv << resultIter->fps << ", " << resultIter->runtime;
            } else {
                log << qSetFieldWidth(actualWidth[name]) << center << "n/a" << left;
                csv << "0";
            }
        }
        log << qSetFieldWidth(0) << '\n';
        csv << '\n';
    }

    log.flush();
    csv.flush();

}
