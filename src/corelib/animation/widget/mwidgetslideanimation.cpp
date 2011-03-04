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

#include "mwidgetslideanimation.h"
#include "mwidgetslideanimation_p.h"
#include "manimationcreator.h"

#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <mwidgetcontroller.h>
#include <mscenemanager.h>

void MWidgetSlideAnimationPrivate::init()
{
    Q_Q(MWidgetSlideAnimation);

    direction = MWidgetSlideAnimation::In;

    QSequentialAnimationGroup *delayedAnimation = new QSequentialAnimationGroup;
    delay = new QPauseAnimation;
    positionAnimation = new QPropertyAnimation;
    positionAnimation->setPropertyName("pos");
    delayedAnimation->addAnimation(delay);
    delayedAnimation->addAnimation(positionAnimation);
    q->addAnimation(delayedAnimation);
    q->connect(delay, SIGNAL(finished()), SLOT(_q_onDelayFinished()));
}

void MWidgetSlideAnimationPrivate::_q_onDelayFinished()
{
    targetWidget->show();
}

MWidgetSlideAnimation::MWidgetSlideAnimation(MWidgetSlideAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetSlideAnimation);

    d->init();
}

MWidgetSlideAnimation::MWidgetSlideAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetSlideAnimationPrivate, parent)
{
    Q_D(MWidgetSlideAnimation);

    d->init();
}

MWidgetSlideAnimation::~MWidgetSlideAnimation()
{
}

void MWidgetSlideAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetSlideAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->positionAnimation->setTargetObject(targetWidget());
}

void MWidgetSlideAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MWidgetSlideAnimation);

    d->direction = direction;

    if (d->direction == In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");
}

void MWidgetSlideAnimation::restoreTargetWidgetState()
{
    Q_D(MWidgetSlideAnimation);
    if (d->played)
        targetWidget()->setPos(d->originalPos);
}

void MWidgetSlideAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetSlideAnimation);

    if (!d->targetWidget)
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        d->originalPos = targetWidget()->pos();

        QPointF offscreenPos;

        if (style().objectName().isNull())
            style().setObjectName("In");

        if (style()->origin() == "top") {
            offscreenPos = QPointF(0, -targetWidget()->size().height());
        } else if (style()->origin() == "right") {
            offscreenPos = QPointF(targetWidget()->sceneManager()->visibleSceneSize().width() , 0);
        } else if (style()->origin() == "bottom") {
            offscreenPos = QPointF(0, targetWidget()->sceneManager()->visibleSceneSize().height());
        } else if (style()->origin() == "left") {
            offscreenPos = QPointF(-targetWidget()->boundingRect().width(), 0);
        }

        if (d->direction == In) {
            targetWidget()->hide();
            d->positionAnimation->setStartValue(offscreenPos);
            d->positionAnimation->setEndValue(d->originalPos);
        } else {
            d->positionAnimation->setStartValue(d->originalPos);
            d->positionAnimation->setEndValue(offscreenPos);
        }

        d->delay->setDuration(style()->delay());
        d->positionAnimation->setEasingCurve(style()->easingCurve());
        d->positionAnimation->setDuration(style()->duration());
        d->played = true;
    }
}

#include "moc_mwidgetslideanimation.cpp"

M_REGISTER_ANIMATION(MWidgetSlideAnimation)
