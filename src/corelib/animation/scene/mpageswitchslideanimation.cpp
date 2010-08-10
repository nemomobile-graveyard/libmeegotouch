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

#include "mpageswitchslideanimation.h"
#include "mpageswitchslideanimation_p.h"

#include "mscenewindow.h"
#include "mscenemanager.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>

MPageSwitchSlideAnimationPrivate::MPageSwitchSlideAnimationPrivate()
    : MPageSwitchAnimationPrivate()
{
}

MPageSwitchSlideAnimationPrivate::~MPageSwitchSlideAnimationPrivate()
{
}

MPageSwitchSlideAnimation::MPageSwitchSlideAnimation(QObject *parent) :
    MPageSwitchAnimation(new MPageSwitchSlideAnimationPrivate, parent)
{
    Q_D(MPageSwitchSlideAnimation);

    d->positionNewPageAnimation = new QPropertyAnimation;
    d->positionNewPageAnimation->setPropertyName("pos");
    d->positionNewPageAnimation->setEasingCurve(style()->easingCurve());
    d->positionNewPageAnimation->setDuration(style()->duration());
    d->positionNewPageAnimation->setEndValue(QPointF(0, 0));
    addAnimation(d->positionNewPageAnimation);

    d->positionOldPageAnimation = new QPropertyAnimation;
    d->positionOldPageAnimation->setPropertyName("pos");
    d->positionOldPageAnimation->setEasingCurve(style()->easingCurve());
    d->positionOldPageAnimation->setDuration(style()->duration());
    d->positionOldPageAnimation->setStartValue(QPointF(0, 0));
    addAnimation(d->positionOldPageAnimation);
}

MPageSwitchSlideAnimation::MPageSwitchSlideAnimation(MPageSwitchSlideAnimationPrivate *dd, QObject *parent) :
    MPageSwitchAnimation(dd, parent)
{
}

void MPageSwitchSlideAnimation::updateState(QAbstractAnimation::State newState,
        QAbstractAnimation::State oldState)
{
    Q_D(MPageSwitchSlideAnimation);
    Q_UNUSED(oldState);

    if (newState != Running)
        return;

    d->positionNewPageAnimation->setTargetObject(newPage());
    d->positionOldPageAnimation->setTargetObject(oldPage());

    if (newPage()) {
        if (transitionDirection() == ToParentPage)
            d->positionNewPageAnimation->setStartValue(QPointF(newPage()->boundingRect().width(), 0));
        else
            d->positionNewPageAnimation->setStartValue(QPointF(-newPage()->boundingRect().width(), 0));
    }

    if (oldPage()) {
        if (transitionDirection() == ToParentPage)
            d->positionOldPageAnimation->setEndValue(QPointF(-oldPage()->boundingRect().width(), 0));
        else
            d->positionOldPageAnimation->setEndValue(QPointF(oldPage()->boundingRect().width(), 0));
    }
}

M_REGISTER_ANIMATION(MPageSwitchSlideAnimation)

