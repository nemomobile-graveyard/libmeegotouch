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

#ifndef TIMEDEMO_H
#define TIMEDEMO_H

#include <QObject>
#include <QVector>
#include <QTime>
#include <QHash>
#include <QStringList>

#include "swaphook.h"

class QString;
class MainPage;
class SwapHook;
class TimedemoPage;
class TimedemoBenchmark;

class BenchmarkResult {
public:
    BenchmarkResult() {}
    BenchmarkResult(const QLinkedList<qint64>& timestamps, const QString& type)
        : timestamps(timestamps),
        type(type)
    {}

    QLinkedList<qint64> timestamps;
    QString type;

    int fps() const {
        if (timestamps.count() == 0) {
            return 0;
        } else {
            return (timestamps.count() - 1) / qMax<float>(runtime(), 1.f) * 1000;
        }
    }

    int runtime() const {
        return timestamps.isEmpty() ? 0 : timestamps.last() - timestamps.first();
    }
};

/**
  * Every TimedemoBenchmark must call this method once the benchmarking phase starts.
  * Make sure to set the orientation angle of the application to the one you need. No
  * guarantee is made about the current angle when startTiming() is called.
  */
class Timedemo : public QObject
{
    Q_OBJECT
public:
    Timedemo(MainPage *mainPage, const QStringList& demoPageTitles);

    void setOutputCsv(const QString &filename);
    void setFramelog(const QString &filename);

    /**
      * Every TimedemoBenchmark must call this method once the benchmarking phase starts.
      */
    void startTiming(const TimedemoBenchmark *activeBenchmark);

    /**
      * Every TimedemoBenchmark must call this method once the benchmarking phase is finished.
      */
    void stopTiming(const TimedemoBenchmark *activeBenchmark);

    void displayBenchmarkResults();
private:
    void saveFramelog();

    MainPage *m_pFrontPage;

    typedef QHash<QString, BenchmarkResult> BenchmarkResultHash;
    QVector<BenchmarkResultHash> benchmarkResults;
    QList<QString> allBenchmarks;

    int m_currentPageIndex;
    int m_currentBenchmarkIndex;
    QVector<TimedemoPage*> demoPages;
    QStringList demoPageTitles;

    uint m_beginFrameCount;
    QTime m_beginTime;

    QString m_csvFilename;
    QString framelogFilename;

    bool timingStarted;
    bool timingStopped;


private slots:
    void showFirstPage();
    void beginBenchmark();
    void benchmarkFinished();
    void showNextPage();
};

#endif // TIMEDEMO_H
