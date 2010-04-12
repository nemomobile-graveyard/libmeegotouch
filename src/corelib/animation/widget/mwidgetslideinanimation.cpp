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

#include "mwidgetslideinanimation.h"
#include "mwidgetslideinanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>
#include <mwidgetcontroller.h>

MWidgetSlideInAnimation::MWidgetSlideInAnimation(MWidgetSlideInAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetSlideInAnimation);

    d->positionAnimation = new QPropertyAnimation;
    d->positionAnimation->setPropertyName("pos");
    addAnimation(d->positionAnimation);
}

MWidgetSlideInAnimation::MWidgetSlideInAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetSlideInAnimationPrivate, parent)
{
    Q_D(MWidgetSlideInAnimation);

    d->positionAnimation = new QPropertyAnimation;
    d->positionAnimation->setPropertyName("pos");
    addAnimation(d->positionAnimation);
}

MWidgetSlideInAnimation::~MWidgetSlideInAnimation()
{
}

void MWidgetSlideInAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetSlideInAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->positionAnimation->setTargetObject(targetWidget());
}

void MWidgetSlideInAnimation::restoreTargetWidgetState()
{
    Q_D(MWidgetSlideInAnimation);
    if (d->played)
        targetWidget()->setPos(d->originalPos);
}

void MWidgetSlideInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetSlideInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        targetWidget()->show();
        d->originalPos = targetWidget()->pos();
        d->played = true;

        QPointF startPos;

        if (style()->from() == "top") {
            startPos = QPointF(0, -targetWidget()->boundingRect().height());
        } else if (style()->from() == "right") {
            startPos = QPointF(targetWidget()->boundingRect().width(), 0);
        } else if (style()->from() == "bottom") {
            startPos = QPointF(0, targetWidget()->boundingRect().height());
        } else if (style()->from() == "left") {
            startPos = QPointF(-targetWidget()->boundingRect().width(), 0);
        }

        d->positionAnimation->setStartValue(startPos);
        d->positionAnimation->setEndValue(d->originalPos);
        d->positionAnimation->setDuration(style()->duration());
    }
}
