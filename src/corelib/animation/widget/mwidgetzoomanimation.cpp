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

#include "mwidgetzoomanimation.h"
#include "mwidgetzoomanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>

void MWidgetZoomAnimationPrivate::init()
{
    Q_Q(MWidgetZoomAnimation);

    direction = MWidgetZoomAnimation::In;
    startDelay = new QPauseAnimation;

    opacityAnimationDelay = new QPauseAnimation;
    opacityAnimation = new QPropertyAnimation;
    opacityAnimation->setPropertyName("opacity");

    QSequentialAnimationGroup *delayedOpacityAnimation = new QSequentialAnimationGroup;
    delayedOpacityAnimation->addAnimation(opacityAnimationDelay);
    delayedOpacityAnimation->addAnimation(opacityAnimation);

    scaleAnimation = new QPropertyAnimation;
    scaleAnimation->setPropertyName("scale");

    positionAnimation = new QPropertyAnimation;
    positionAnimation->setPropertyName("pos");

    QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup;
    parallelGroup->addAnimation(delayedOpacityAnimation);
    parallelGroup->addAnimation(scaleAnimation);
    parallelGroup->addAnimation(positionAnimation);

    QSequentialAnimationGroup *sequentialGroup = new QSequentialAnimationGroup;
    sequentialGroup->addAnimation(startDelay);
    sequentialGroup->addAnimation(parallelGroup);

    q->addAnimation(sequentialGroup);
}

QPointF MWidgetZoomAnimationPrivate::setupPositionAnimation(const QPointF &widgetPos)
{
    Q_Q(MWidgetZoomAnimation);

    QPointF distance = origin - widgetPos;
    return widgetPos + distance * (1 - q->style()->positionAnimationDistanceFactor());
}

MWidgetZoomAnimation::MWidgetZoomAnimation(QObject *parent) :
        MAbstractWidgetAnimation(new MWidgetZoomAnimationPrivate, parent)
{
    Q_D(MWidgetZoomAnimation);

    d->init();
}

MWidgetZoomAnimation::MWidgetZoomAnimation(MWidgetZoomAnimationPrivate *dd, QObject *parent) :
        MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetZoomAnimation);

    d->init();
}

void MWidgetZoomAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetZoomAnimation);

    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->opacityAnimation->setTargetObject(widget);
    d->scaleAnimation->setTargetObject(widget);
    d->positionAnimation->setTargetObject(widget);
}

void MWidgetZoomAnimation::restoreTargetWidgetState()
{
}

void MWidgetZoomAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MWidgetZoomAnimation);

    d->direction = direction;

    if (d->direction == In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");
}

void MWidgetZoomAnimation::setOrigin(const QPointF &pos)
{
    Q_D(MWidgetZoomAnimation);

    d->origin = pos;
}

QPointF MWidgetZoomAnimation::origin() const
{
    Q_D(const MWidgetZoomAnimation);

    return d->origin;
}

void MWidgetZoomAnimation::updateState(QAbstractAnimation::State newState,
                                             QAbstractAnimation::State oldState)
{
    Q_D(MWidgetZoomAnimation);

    QAbstractAnimation::updateState(newState, oldState);

    if (!d->targetWidget)
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        if (style().objectName().isNull())
            style().setObjectName("In");

        if (d->direction == In) {
            d->targetWidget->setOpacity(0);

            d->positionAnimation->setStartValue(d->setupPositionAnimation(d->targetWidget->pos()));
            d->positionAnimation->setEndValue(d->targetWidget->pos());

            d->scaleAnimation->setStartValue(style()->scale());
            d->scaleAnimation->setEndValue(1);

            d->opacityAnimation->setStartValue(0);
            d->opacityAnimation->setEndValue(style()->opacity());
        } else {
            d->positionAnimation->setStartValue(d->targetWidget->pos());
            d->positionAnimation->setEndValue(d->setupPositionAnimation(d->targetWidget->pos()));

            d->opacityAnimation->setStartValue(style()->opacity());
            d->opacityAnimation->setEndValue(0);
            d->scaleAnimation->setStartValue(1);
            d->scaleAnimation->setEndValue(style()->scale());
        }

        d->startDelay->setDuration(style()->delay());
        d->opacityAnimationDelay->setDuration(style()->opacityAnimationDelay());
        d->opacityAnimation->setEasingCurve(style()->opacityAnimationEasingCurve());
        d->opacityAnimation->setDuration(style()->opacityAnimationDuration());

        d->scaleAnimation->setEasingCurve(style()->scaleAnimationEasingCurve());
        d->scaleAnimation->setDuration(style()->scaleAnimationDuration());

        d->positionAnimation->setEasingCurve(style()->scaleAnimationEasingCurve());
        d->positionAnimation->setDuration(style()->scaleAnimationDuration());
    }
}

#include "moc_mwidgetzoomanimation.cpp"

M_REGISTER_ANIMATION(MWidgetZoomAnimation)
