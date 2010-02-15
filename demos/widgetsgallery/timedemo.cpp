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


#include <DuiApplicationPage>

#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QTimer>

namespace
{
    const int pageDuration = 5000;
}

Timedemo::Timedemo(const TimingScene *timingScene, ListPage *listPage)
    : timingScene(timingScene)
    , m_pFrontPage(listPage)
    , m_currentPageIndex(0)
{
    connect(m_pFrontPage, SIGNAL(windowShown()), this, SLOT(showFirstPage()));
}

void Timedemo::setOutputCsv(const QString &filename)
{
    m_csvFilename = filename;
}

void Timedemo::showFirstPage()
{
    disconnect(m_pFrontPage, SIGNAL(windowShown()), this, SLOT(showFirstPage()));
    m_currentPageIndex = 0;
    m_frameCounts.resize(m_pFrontPage->pageCount() + 1);
    currentPage = m_pFrontPage->findPageByIndex(0);
    beginBenchmark();
    m_pFrontPage->showPageByIndex(0);
}

void Timedemo::beginBenchmark()
{
    // The windowShown() signal is emitted when the show animation
    // of the scene window has finished.
    connect(currentPage, SIGNAL(windowShown()), this, SLOT(pageLayoutFinished()));

    m_beginFrameCount = timingScene->frameCount();
    m_beginTime = QTime::currentTime();
}


void Timedemo::endBenchmark()
{
    QTime endTime = QTime::currentTime();
    uint endFrameCount = timingScene->frameCount();

    qreal fps = qreal(endFrameCount - m_beginFrameCount)
                / qMax(m_beginTime.msecsTo(endTime), 1) * 1000;

    m_frameCounts[m_currentPageIndex].runtimeFps = fps;
}


void Timedemo::pageLayoutFinished()
{
    disconnect(currentPage, SIGNAL(windowShown()), this, SLOT(pageLayoutFinished()));

    QTime endTime = QTime::currentTime();
    uint endFrameCount = timingScene->frameCount();

    PerfData &perf = m_frameCounts[m_currentPageIndex];
    perf.startupTime = m_beginTime.msecsTo(endTime);
    perf.startupFps = qreal(endFrameCount - m_beginFrameCount) / qMax(perf.startupTime, 1u) * 1000;

    m_beginFrameCount = endFrameCount;
    m_beginTime = endTime;

    QTimer::singleShot(pageDuration, this, SLOT(showNextPage()));
}


void Timedemo::showNextPage()
{
    endBenchmark();

    ++m_currentPageIndex;

    // if a page with this index exists, switch to it
    if (m_currentPageIndex < m_pFrontPage->pageCount()) {
        currentPage = m_pFrontPage->findPageByIndex(m_currentPageIndex);
        beginBenchmark();
        m_pFrontPage->showPageByIndex(m_currentPageIndex);

    } else if (m_currentPageIndex == m_pFrontPage->pageCount()) {
        currentPage = m_pFrontPage;
        beginBenchmark();
        m_pFrontPage->appear();
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

    if (!m_csvFilename.isEmpty()) {
        csvFile.setFileName(m_csvFilename);
        csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        csv.setDevice(&csvFile);
    }

    log << left
        << qSetFieldWidth(20) << "Page name:"
        << qSetFieldWidth(12) << "Start time" << "Start FPS" << "Run FPS"
        << qSetFieldWidth(0)  << '\n';

    if (!m_csvFilename.isEmpty()) {
        csv << "#\"Page name\",\"Start-up time\",\"Start-up frames/s\",\"Run-time frames/s\"\n";
    }

    for (int i = 0; i < m_frameCounts.count(); ++i) {
        DuiApplicationPage *page = (i < m_pFrontPage->pageCount()) ? m_pFrontPage->findPageByIndex(i) : m_pFrontPage;
        const QString title = page->title();

        log << qSetFieldWidth(20) << (title + ':');
        if (!m_csvFilename.isEmpty()) {
            csv << '"' << title << "\",";
        }

        const PerfData &perf = m_frameCounts[i];
        const qreal startupSeconds = perf.startupTime / 1000.0;

        log << qSetFieldWidth(12) << startupSeconds << perf.startupFps << perf.runtimeFps
            << qSetFieldWidth(0)  << '\n';
        if (!m_csvFilename.isEmpty()) {
            csv << startupSeconds << ',' << perf.startupFps << ',' << perf.runtimeFps << '\n';
        }
    }

    log.flush();
    csv.flush();
}
