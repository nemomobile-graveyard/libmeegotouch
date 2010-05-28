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

#include "mscenedimminganimation.h"
#include "mscenedimminganimation_p.h"
#include "mgraphicsdimeffect.h"

#include <QPropertyAnimation>

void MSceneDimmingAnimationPrivate::init()
{
//    Q_Q(MSceneDimmingAnimation);
    direction = MSceneDimmingAnimation::In;
}

MSceneDimmingAnimation::MSceneDimmingAnimation(QObject *parent) :
    MParallelAnimationGroup(new MSceneDimmingAnimationPrivate, parent)
{
    Q_D(MSceneDimmingAnimation);
    d->init();
}

MSceneDimmingAnimation::~MSceneDimmingAnimation()
{
    // fast-forward child animations to the end
    for(int i = 0; i < animationCount(); i++)
        animationAt(i)->setCurrentTime(animationAt(i)->duration());
}

void MSceneDimmingAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MSceneDimmingAnimation);
    d->direction = direction;
}

void MSceneDimmingAnimation::addGraphicsEffect(MGraphicsEffect* effect)
{
    Q_D(MSceneDimmingAnimation);

    QPropertyAnimation* animation = new QPropertyAnimation(effect, "intensity");
    animation->setDuration(style()->duration());
    animation->setEasingCurve(style()->easingCurve());

    if (d->direction == In) {
        animation->setStartValue(0);
        animation->setEndValue(1.0);
    } else {
        animation->setStartValue(1.0);
        animation->setEndValue(0);
    }

    effect->setAnimation(animation);

    addAnimation(animation);
}

void MSceneDimmingAnimation::setStyleParent(MWidgetController* widget)
{
    style().setParent(widget);
}
