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

#include "panningbenchmark.h"
#include "timedemo.h"

#include <MApplication>
#include <MApplicationPage>
#include <MPannableViewport>
#include <MPhysics2DPanning>

#include <QTimer>

namespace {
    const int updateInterval = 20; // ms
    const qreal ySpeed = .5; // pixel/ms
}

PanningBenchmark::PanningBenchmark(
  MApplicationPage * applicationPage, Timedemo * timedemo, M::OrientationAngle targetOrientationAngle) :
      TimedemoBenchmark(applicationPage, timedemo),
      pannableViewport(0),
      timingStarted(false),
      formerPosition(),
      timer(),
      targetOrientationAngle(targetOrientationAngle)

{
    pannableViewport = 0;
    QList<QGraphicsItem *> childItems = applicationPage->childItems();
    foreach(QGraphicsItem * childItem, childItems) {
        if (MPannableViewport * viewport = dynamic_cast<MPannableViewport*>(childItem)) {
            pannableViewport = viewport;
            break;
        }
    }

    if (!pannableViewport) {
        qFatal("Did not find matching viewport of MApplicationWindow");
    }
}

QString PanningBenchmark::name() {
    return QString("PanningBenchmark (%1)").arg(QString::number(targetOrientationAngle));
}

void PanningBenchmark::start()
{
    MApplication::activeWindow()->setOrientationAngle(targetOrientationAngle);
    MApplication::activeWindow()->setOrientationAngleLocked(true);
    if (!applicationPage->isOnDisplay()) {
        connect(applicationPage, SIGNAL(appeared()), this, SLOT(waitBeforePanning()));
        applicationPage->appear(MApplication::activeWindow());
        verifyAppearanceTimer->start(5000);
    } else {
        waitBeforePanning();
    }
}

//HACK! if we do not wait pannableViewport->physics()->range() is (0, 0, 0, 0) because
// the widgets are not completely set up yet
void PanningBenchmark::waitBeforePanning()
{
    verifyAppearanceTimer->stop();
    QTimer::singleShot(2500, this, SLOT(pan()));
}

void PanningBenchmark::pan()
{
    if (!timingStarted) {
        timedemo->startTiming(this);
        timingStarted = true;
        formerPosition = pannableViewport->physics()->position();
        timer.start();
    }

    QPointF currentPosition = pannableViewport->physics()->position();
    QRectF range = pannableViewport->physics()->range();

    if (!timedemo->currentBenchmarkRuntimeReached()) {
        if (currentPosition.y() >= range.height())
            formerPosition = currentPosition;
        else if (currentPosition.y() <= 0)
            formerPosition = currentPosition;

        doPan(formerPosition.y() < range.height());
    }
    else {
        terminateBenchmark();
    }
}

void PanningBenchmark::doPan(bool panDown)
{
    qreal displacement = ySpeed * timer.elapsed();

    QPointF currentPosition = pannableViewport->physics()->position();
    pannableViewport->physics()->setPosition(currentPosition +
            QPointF(0., panDown ? displacement : -displacement));
    timer.start();
    QTimer::singleShot(updateInterval, this, SLOT(pan()));
}

void PanningBenchmark::terminateBenchmark()
{
    timedemo->stopTiming(this);
    pannableViewport->physics()->setPosition(formerPosition);
    emit finished();
}
