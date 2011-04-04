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
    : MPageSwitchAnimationPrivate(),
      sceneWindow(NULL),
      positionNewPageAnimation(NULL),
      positionOldPageAnimation(NULL)
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
    d->positionNewPageAnimation->setPropertyName("x");
    d->positionNewPageAnimation->setEasingCurve(style()->easingCurve());
    d->positionNewPageAnimation->setDuration(style()->duration());
    d->positionNewPageAnimation->setEndValue((qreal)0);
    addAnimation(d->positionNewPageAnimation);

    d->positionOldPageAnimation = new QPropertyAnimation;
    d->positionOldPageAnimation->setPropertyName("x");
    d->positionOldPageAnimation->setEasingCurve(style()->easingCurve());
    d->positionOldPageAnimation->setDuration(style()->duration());
    d->positionOldPageAnimation->setStartValue((qreal)0);
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
            d->positionNewPageAnimation->setStartValue(newPage()->boundingRect().width());
        else
            d->positionNewPageAnimation->setStartValue(-newPage()->boundingRect().width());
    }

    if (oldPage()) {
        if (transitionDirection() == ToParentPage)
            d->positionOldPageAnimation->setEndValue(-oldPage()->boundingRect().width());
        else
            d->positionOldPageAnimation->setEndValue(oldPage()->boundingRect().width());
    }
}

M_REGISTER_ANIMATION(MPageSwitchSlideAnimation)

