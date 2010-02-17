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

#ifndef TIMEDEMO_H
#define TIMEDEMO_H

#include <QObject>
#include <QVector>
#include <QTime>
#include <QHash>

class QString;
class ListPage;
class TimingScene;
class TimedemoPage;
class TimedemoBenchmark;

struct BenchmarkResult {
    uint  runtime;
    qreal fps;

    BenchmarkResult() : runtime(0), fps(0.) {}
    BenchmarkResult(uint runtime, qreal fps) : runtime(runtime), fps(fps) {}
};

/**
  * Timedemo shows all the pages stored by the ListPage and shows them one after another.
  * FPS are measured and printed in the end.
  */
class Timedemo : public QObject
{
    Q_OBJECT
public:
    Timedemo(TimingScene *timingScene, ListPage *listPage);

    void setOutputCsv(const QString &filename);

    /**
      * Every TimedemoBenchmark must call this method once the benchmarking phase starts.
      */
    void startTiming();

    /**
      * Every TimedemoBenchmark must call this method once the benchmarking phase is finished.
      */
    void stopTiming();

private:
    void displayBenchmarkResults();

    TimingScene *timingScene;

    ListPage *m_pFrontPage;

    typedef QHash<QString, BenchmarkResult> BenchmarkResultHash;
    QVector<BenchmarkResultHash> benchmarkResults;
    QList<QString> allBenchmarks;

    int m_currentPageIndex;
    int m_currentBenchmarkIndex;
    TimedemoPage *currentPage;

    uint m_beginFrameCount;
    QTime m_beginTime;

    QString m_csvFilename;

    bool timingStarted;
    bool timingStopped;


private slots:
    void showFirstPage();
    void beginBenchmark();
    void benchmarkFinished();
    void showNextPage();
};

#endif // TIMEDEMO_H
