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

#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include "mwidgetmoveanimation.h"
#include "mwidgetmoveanimation_p.h"
#include "mwidgetcontroller.h"

void MWidgetMoveAnimationPrivate::init()
{
    Q_Q(MWidgetMoveAnimation);

    widget = 0;

    QSequentialAnimationGroup *delayedAnimation = new QSequentialAnimationGroup;

    delay = new QPauseAnimation;
    delay->setDuration(q->style()->delay());
    positionAnimation = new QPropertyAnimation;
    positionAnimation->setPropertyName("pos");
    positionAnimation->setDuration(q->style()->duration());
    positionAnimation->setEasingCurve(q->style()->easingCurve());

    delayedAnimation->addAnimation(delay);
    delayedAnimation->addAnimation(positionAnimation);
    q->addAnimation(delayedAnimation);
}

MWidgetMoveAnimation::MWidgetMoveAnimation(QObject *parent) :
    MParallelAnimationGroup(new MWidgetMoveAnimationPrivate, parent)
{
    Q_D(MWidgetMoveAnimation);

    d->init();
}

MWidgetMoveAnimation::MWidgetMoveAnimation(MWidgetMoveAnimationPrivate *dd, QObject *parent) :
    MParallelAnimationGroup(dd, parent)
{
    Q_D(MWidgetMoveAnimation);

    d->init();
}

void MWidgetMoveAnimation::setWidget(QGraphicsWidget *widget)
{
    Q_D(MWidgetMoveAnimation);

    d->widget = widget;
}

void MWidgetMoveAnimation::setFinalPos(const QPointF &pos)
{
    Q_D(MWidgetMoveAnimation);

    d->finalPos = pos;
}

void MWidgetMoveAnimation::updateState(QAbstractAnimation::State newState,
        QAbstractAnimation::State oldState)
{
    Q_D(MWidgetMoveAnimation);

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        d->positionAnimation->setTargetObject(d->widget);
        d->positionAnimation->setStartValue(d->widget->pos());
        d->positionAnimation->setEndValue(d->finalPos);
    }
}

#include "moc_mwidgetmoveanimation.cpp"
