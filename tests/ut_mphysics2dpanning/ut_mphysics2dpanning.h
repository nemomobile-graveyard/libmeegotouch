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

#ifndef UT_PHYSICS2DPANNING_H
#define UT_PHYSICS2DPANNING_H


#include <string>
#include <QtTest/QtTest>
#include <QTimeLine>
#include "mapplication.h"

class MPhysics2DPanning;

class Ut_MPhysics2DPanning : public QObject
{
    Q_OBJECT;

public:
    static QList<QString>   timeLineActions;
    static QTimeLine::State timeLineState;

private:
    MApplication *app;

    MPhysics2DPanning *physics;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // Init values

    void initValues();

    // QTimeLine actions in start and stop (1=QTimeLine not running, 2=running)

    void timeLineInStart1();
    void timeLineInStart2();
    void timeLineInStop();

    // Setters and getters which take QSizeF

    void setAndGetQSizeF_data();
    void setAndGetQSizeF();

    // Setters and getters which take QPointF

    void setAndGetQPointF_data();
    void setAndGetQPointF();

    // Passing the state of the TimeLine

    void timeLineStateQuery();

    // Pointer press, move and release actions

    void pointerPress();
    void pointerMove();
    void pointerRelease();

    // Integrating

    void integrating_data();
    void integrating();

    // Integration algorithm convergence and border behaviour
    void positionShouldReturnToStartRangeAfterMovingViewportBeyondStartRange();
    void positionShouldReturnToEndRangeAfterMovingViewportBeyondEndRange();
    void integrationShouldStopAfterReachingPositionInsideRange();

};


#endif
