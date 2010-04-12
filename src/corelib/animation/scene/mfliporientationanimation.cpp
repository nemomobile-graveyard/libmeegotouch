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

#include "mfliporientationanimation.h"

#if QT_VERSION >= 0x040600

#include "mfliporientationanimation_p.h"

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsWidget>
#include "mwidgetanimation.h"
#include "mscenemanager.h"
#include "mscenewindow.h"
#include "mnavigationbaranimation.h"

///////////////////
// Private class //
///////////////////
MFlipOrientationAnimationPrivate::MFlipOrientationAnimationPrivate() : angle(M::Angle0)
{
}

MFlipOrientationAnimationPrivate::~MFlipOrientationAnimationPrivate()
{
}

void MFlipOrientationAnimationPrivate::finishedPhase0()
{
    Q_Q(MFlipOrientationAnimation);

    //TODO: create own class for rootElement to be able to
    //      rotate around other axes as well.

    if (angle == M::Angle0 || angle == M::Angle180) {
        q->rootElement()->setPos(0, 0);
        q->rootElement()->setTransformOriginPoint(QPointF(432, 240));
        //q->rootElement()->setXRotation(0);
        //q->rootElement()->setYRotation(0);
        //q->rootElement()->setZRotation(angle);
    } else {
        q->rootElement()->setPos(192, -192);
        q->rootElement()->setTransformOriginPoint(QPointF(240, 432));
        //q->rootElement()->setXRotation(0);
        //q->rootElement()->setYRotation(0);
        //q->rootElement()->setZRotation(angle);
    }
}

//////////////////
// Public class //
//////////////////

MFlipOrientationAnimation::MFlipOrientationAnimation(QObject *parent) :
    MOrientationAnimation(new MFlipOrientationAnimationPrivate(), parent, MGroupAnimation::Sequential)
{
    Q_D(MFlipOrientationAnimation);

    d->phase0 = new QPropertyAnimation(NULL, "xRotation", group());
    d->phase0->setDuration(500);


    d->phase1 = new QPropertyAnimation(NULL, "yRotation", group());
    d->phase1->setDuration(500);

    connect(d->phase0, SIGNAL(finished()), SIGNAL(orientationChanged()));
    connect(d->phase0, SIGNAL(finished()), SLOT(finishedPhase0()));
}

MFlipOrientationAnimation::~MFlipOrientationAnimation()
{
}

void MFlipOrientationAnimation::addSceneWindow(MSceneWindow *window)
{
    Q_UNUSED(window);
}

void MFlipOrientationAnimation::removeSceneWindow(MSceneWindow *window)
{
    Q_UNUSED(window);
}

void MFlipOrientationAnimation::setTargetRotationAngle(M::OrientationAngle start,
        M::OrientationAngle end)
{
    Q_D(MFlipOrientationAnimation);

    //TODO: create own class for rootElement to be able to
    //      rotate around other axes as well.


    //d->phase0->setStartValue(0);
    //d->phase0->setStartValue(rootElement()->xRotation());
    //d->phase1->setEndValue(0);

    int startDir = -1;
    //if(start == M::Angle180/* || start == M::Angle270*/)
    //    startDir = 1;

    int endDir = -1;
    //if(/*end == M::Angle180 || */end == M::Angle270)
    //    endDir = 1;

    d->phase0->setEndValue(90 * startDir);
    d->phase1->setStartValue(-90 * endDir);


    if (start == M::Angle0 || start == M::Angle180) {
        // Landscape to portrait
        d->phase0->setPropertyName("xRotation");
        d->phase1->setPropertyName("xRotation");
    } else {
        // Portrait to landscape
        d->phase0->setPropertyName("xRotation");
        d->phase1->setPropertyName("xRotation");
    }

    d->angle = end;
}

void MFlipOrientationAnimation::rootElementChanged()
{
    Q_D(MFlipOrientationAnimation);
    d->phase0->setTargetObject(rootElement());
    d->phase1->setTargetObject(rootElement());
}

#include "moc_mfliporientationanimation.cpp"
#endif
