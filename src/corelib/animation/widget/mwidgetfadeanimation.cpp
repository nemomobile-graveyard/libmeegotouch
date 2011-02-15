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

#include "mwidgetfadeanimation.h"
#include "mwidgetfadeanimation_p.h"
#include "manimationcreator.h"

#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <mwidgetcontroller.h>

void MWidgetFadeAnimationPrivate::init()
{
    Q_Q(MWidgetFadeAnimation);

    direction = MWidgetFadeAnimation::In;
    QSequentialAnimationGroup *delayedAnimation = new QSequentialAnimationGroup;
    delay = new QPauseAnimation;
    opacityAnimation = new QPropertyAnimation;
    opacityAnimation->setPropertyName("opacity");
    delayedAnimation->addAnimation(delay);
    delayedAnimation->addAnimation(opacityAnimation);
    q->addAnimation(delayedAnimation);
}

MWidgetFadeAnimation::MWidgetFadeAnimation(MWidgetFadeAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetFadeAnimation);

    d->init();
}

MWidgetFadeAnimation::MWidgetFadeAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetFadeAnimationPrivate, parent)
{
    Q_D(MWidgetFadeAnimation);

    d->init();
}

MWidgetFadeAnimation::~MWidgetFadeAnimation()
{
}

void MWidgetFadeAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetFadeAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->opacityAnimation->setTargetObject(targetWidget());
}

void MWidgetFadeAnimation::restoreTargetWidgetState()
{
    Q_D(MWidgetFadeAnimation);
    if (d->played)
        targetWidget()->setOpacity(d->originalOpacity);
}

void MWidgetFadeAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MWidgetFadeAnimation);

    d->direction = direction;

    if (d->direction == In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");
}

void MWidgetFadeAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetFadeAnimation);

    if (!d->targetWidget)
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        if (style().objectName().isNull())
            style().setObjectName("In");

        d->originalOpacity = d->targetWidget->opacity();

        if (d->direction == In) {
            targetWidget()->setOpacity(0);
            d->opacityAnimation->setStartValue(0);
            d->opacityAnimation->setEndValue(d->originalOpacity);
        } else {
            d->opacityAnimation->setStartValue(d->originalOpacity);
            d->opacityAnimation->setEndValue(0);
        }

        d->played = true;

        d->delay->setDuration(style()->delay());
        d->opacityAnimation->setDuration(style()->duration());
    }
}

M_REGISTER_ANIMATION(MWidgetFadeAnimation)
