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

class QString;
class ListPage;
class TimingScene;
class DuiApplicationPage;

struct PerfData {
    uint  startupTime;
    qreal startupFps;
    qreal runtimeFps;

    PerfData() : startupTime(0), startupFps(0.0), runtimeFps(0.0) {}
    PerfData(uint st, qreal sf, qreal rf) : startupTime(st), startupFps(sf), runtimeFps(rf) {}
};

/**
  * Timedemo shows all the pages stored by the ListPage and shows them one after another.
  * FPS are measured and printed in the end.
  */
class Timedemo : public QObject
{
    Q_OBJECT
public:
    Timedemo(const TimingScene *timingScene, ListPage *listPage);

    void setOutputCsv(const QString &filename);

private:
    void endBenchmark();
    void displayBenchmarkResults();

    const TimingScene *timingScene;

    ListPage *m_pFrontPage;

    QVector<PerfData> m_frameCounts;
    int m_currentPageIndex;
    DuiApplicationPage *currentPage;

    uint m_beginFrameCount;
    QTime m_beginTime;

    QString m_csvFilename;


private slots:
    void showFirstPage();
    void beginBenchmark();
    void pageLayoutFinished();
    void showNextPage();
};

#endif // TIMEDEMO_H
