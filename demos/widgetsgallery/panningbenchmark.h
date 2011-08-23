/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PANNINGBENCHMARK_H
#define PANNINGBENCHMARK_H

#include "timedemobenchmark.h"

#include <MPannableViewport>
#include <MNamespace>

#include <QTime>

/**
  * PanningBenchmark emulates panning a page by scrolling down until the
  * bottom of the page is reached.
  */
class PanningBenchmark : public TimedemoBenchmark
{
    Q_OBJECT
public:
    PanningBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo, M::OrientationAngle targetOrientationAngle = M::Angle0);

    QString name();
    void start();

private slots:
    void waitBeforePanning();
    void pan();
    void terminateBenchmark();

private:
    void doPan(bool panDown);

    MPannableViewport *pannableViewport;
    bool timingStarted;
    QPointF formerPosition;
    QTime timer;

private:
    M::OrientationAngle targetOrientationAngle;
};

#endif // PANNINGBENCHMARK_H
