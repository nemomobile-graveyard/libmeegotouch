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

#include "duifliporientationanimation.h"

#if QT_VERSION >= 0x040600

#include "duifliporientationanimation_p.h"

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsWidget>
#include "duiwidgetanimation.h"
#include "duiscenemanager.h"
#include "duiscenewindow.h"
#include "duinavigationbaranimation.h"

///////////////////
// Private class //
///////////////////
DuiFlipOrientationAnimationPrivate::DuiFlipOrientationAnimationPrivate()
{
}

DuiFlipOrientationAnimationPrivate::~DuiFlipOrientationAnimationPrivate()
{
}

void DuiFlipOrientationAnimationPrivate::finishedPhase0()
{
    Q_Q(DuiFlipOrientationAnimation);

    //TODO: create own class for rootElement to be able to
    //      rotate around other axes as well.

    if (angle == Dui::Angle0 || angle == Dui::Angle180) {
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

DuiFlipOrientationAnimation::DuiFlipOrientationAnimation(QObject *parent) :
    DuiOrientationAnimation(new DuiFlipOrientationAnimationPrivate(), parent, DuiGroupAnimation::Sequential)
{
    Q_D(DuiFlipOrientationAnimation);

    d->phase0 = new QPropertyAnimation(NULL, "xRotation", group());
    d->phase0->setDuration(500);


    d->phase1 = new QPropertyAnimation(NULL, "yRotation", group());
    d->phase1->setDuration(500);

    connect(d->phase0, SIGNAL(finished()), SIGNAL(orientationChanged()));
    connect(d->phase0, SIGNAL(finished()), SLOT(finishedPhase0()));
}

DuiFlipOrientationAnimation::~DuiFlipOrientationAnimation()
{
}

void DuiFlipOrientationAnimation::addSceneWindow(DuiSceneWindow *window)
{
    Q_UNUSED(window);
}

void DuiFlipOrientationAnimation::removeSceneWindow(DuiSceneWindow *window)
{
    Q_UNUSED(window);
}

void DuiFlipOrientationAnimation::setTargetRotationAngle(Dui::OrientationAngle start,
        Dui::OrientationAngle end)
{
    Q_D(DuiFlipOrientationAnimation);

    //TODO: create own class for rootElement to be able to
    //      rotate around other axes as well.


    //d->phase0->setStartValue(0);
    //d->phase0->setStartValue(rootElement()->xRotation());
    //d->phase1->setEndValue(0);

    int startDir = -1;
    //if(start == Dui::Angle180/* || start == Dui::Angle270*/)
    //    startDir = 1;

    int endDir = -1;
    //if(/*end == Dui::Angle180 || */end == Dui::Angle270)
    //    endDir = 1;

    d->phase0->setEndValue(90 * startDir);
    d->phase1->setStartValue(-90 * endDir);


    if (start == Dui::Angle0 || start == Dui::Angle180) {
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

void DuiFlipOrientationAnimation::rootElementChanged()
{
    Q_D(DuiFlipOrientationAnimation);
    d->phase0->setTargetObject(rootElement());
    d->phase1->setTargetObject(rootElement());
}

#include "moc_duifliporientationanimation.cpp"
#endif
