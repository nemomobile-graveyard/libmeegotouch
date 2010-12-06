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

#include "rotationbenchmark.h"
#include "timedemo.h"

#include <MApplication>
#include <MApplicationPage>

#include <QTimer>

RotationBenchmark::RotationBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo, M::OrientationAngle targetOrientationAngle)
    : TimedemoBenchmark(applicationPage, timedemo)
    , targetOrientationAngle(targetOrientationAngle)
{
}

QString RotationBenchmark::name()
{
    return QString("RotationBenchmark (%1)").arg(QString::number(targetOrientationAngle));
}

void RotationBenchmark::start()
{
    if (!applicationPage->isOnDisplay()) {
        connect(applicationPage, SIGNAL(appeared()), this, SLOT(stabilizeFps()));
        applicationPage->appear(MApplication::activeWindow());
        verifyAppearanceTimer->start(5000);
    } else {
        QTimer::singleShot(0, this, SLOT(stabilizeFps()));
    }
}

void RotationBenchmark::stabilizeFps() {
    verifyAppearanceTimer->stop();
    QTimer::singleShot(1000, this, SLOT(rotate()));
}

void RotationBenchmark::rotate()
{
    timedemo->startTiming(this);
    MApplication::activeWindow()->setOrientationAngle(targetOrientationAngle);
    MApplication::activeWindow()->setOrientationAngleLocked(true);
    connect(MApplication::activeWindow(), SIGNAL(orientationChangeFinished(M::Orientation)), this, SLOT(terminateBenchmark()));
}

void RotationBenchmark::terminateBenchmark()
{
    timedemo->stopTiming(this);
    disconnect(MApplication::activeWindow(), SIGNAL(orientationChangeFinished(M::Orientation)), this, SLOT(terminateBenchmark()));
    emit finished();
}
