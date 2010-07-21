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

#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QPauseAnimation>

#include <MDebug>
#include "mwarpanimation.h"

MWarpAnimation::MWarpAnimation(MWidget *widget, WarpDirection direction, QObject *parent) :
    MParallelAnimationGroup(parent)
{
    target = widget;
    warpDirection = direction;

    opacityAnimation = new QPropertyAnimation();
    opacityAnimation->setPropertyName("opacity");
    opacityAnimation->setDuration(style()->warpDuration());

    offsetAnimation = new QPropertyAnimation();
    offsetAnimation->setPropertyName("paintOffset");
    offsetAnimation->setDuration(style()->warpDuration());

    if (direction == InFromLeft || direction == InFromRight) {
        opacityAnimation->setStartValue(0.0f);
        opacityAnimation->setEndValue(1.0f);

        opacityAnimation->setEasingCurve(style()->warpInCurve());
        offsetAnimation->setEasingCurve(style()->warpInCurve());
    } else {
        opacityAnimation->setStartValue(1.0f);
        opacityAnimation->setEndValue(0.0f);

        opacityAnimation->setEasingCurve(style()->warpOutCurve());
        offsetAnimation->setEasingCurve(style()->warpOutCurve());
    }

    opacityAnimation->setTargetObject(widget);
    offsetAnimation->setTargetObject(widget);


    if (direction == InFromLeft || direction == InFromRight) {
        QPauseAnimation *pause = new QPauseAnimation(style()->warpInDelay());
        this->addAnimation(pause);
        connect(pause, SIGNAL(finished()), this, SLOT(pauseFinished()));
    } else {
        this->addAnimation(opacityAnimation);
        this->addAnimation(offsetAnimation);
    }

    connect(target, SIGNAL(destroyed()), this, SLOT(clearTarget()));
}

void MWarpAnimation::clearTarget()
{
    target = 0;
}

void MWarpAnimation::pauseFinished()
{
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    offsetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MWarpAnimation::updateState(QAbstractAnimation::State newState,
                                 QAbstractAnimation::State oldState)
{
    Q_UNUSED(oldState);

    if (newState == Running && target) {
        target->show();

        if (warpDirection == InFromLeft) {
            // On the first frame of the animation, the incoming widget might
            // be visible with a non-zero opacity unless explcitly set
            target->setOpacity(0.0f);
            offsetAnimation->setStartValue(QPointF(-style()->warpDistance(), 0));
            offsetAnimation->setEndValue(QPointF(0, 0));
        } else if (warpDirection == InFromRight) {
            target->setOpacity(0.0f);
            offsetAnimation->setStartValue(QPointF(style()->warpDistance(), 0));
            offsetAnimation->setEndValue(QPointF(0, 0));
        } else if (warpDirection == OutFromLeft) {
            offsetAnimation->setStartValue(QPointF(0, 0));
            offsetAnimation->setEndValue(QPointF(-style()->warpDistance(), 0));
        } else {
            offsetAnimation->setStartValue(QPointF(0, 0));
            offsetAnimation->setEndValue(QPointF(style()->warpDistance(), 0));
        }
    } else if (newState == Stopped && target) {
        if (warpDirection == OutFromLeft || warpDirection == OutFromRight) {
            target->hide();
            target->setOpacity(1);
            target->setPaintOffset(QPointF(0, 0));
        }
    }
}

#include "moc_mwarpanimation.cpp"
