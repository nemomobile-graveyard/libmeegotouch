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

#include "mwidgetslideoutanimation.h"
#include "mwidgetslideoutanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>
#include <mwidgetcontroller.h>

MWidgetSlideOutAnimation::MWidgetSlideOutAnimation(MWidgetSlideOutAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetSlideOutAnimation);

    d->positionAnimation = new QPropertyAnimation;
    d->positionAnimation->setPropertyName("pos");
    addAnimation(d->positionAnimation);
}

MWidgetSlideOutAnimation::MWidgetSlideOutAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetSlideOutAnimationPrivate, parent)
{
    Q_D(MWidgetSlideOutAnimation);

    d->positionAnimation = new QPropertyAnimation;
    d->positionAnimation->setPropertyName("pos");
    addAnimation(d->positionAnimation);
}

MWidgetSlideOutAnimation::~MWidgetSlideOutAnimation()
{
}

void MWidgetSlideOutAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetSlideOutAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->positionAnimation->setTargetObject(targetWidget());
}

void MWidgetSlideOutAnimation::restoreTargetWidgetState()
{
    Q_D(MWidgetSlideOutAnimation);
    if (d->played)
        targetWidget()->setPos(d->originalPos);
}

void MWidgetSlideOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetSlideOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->originalPos = targetWidget()->pos();
        d->played = true;

        QPointF endPos;

        if (style()->to() == "top") {
            endPos = QPointF(0, -targetWidget()->boundingRect().height());
        } else if (style()->to() == "right") {
            endPos = QPointF(targetWidget()->boundingRect().width(), 0);
        } else if (style()->to() == "bottom") {
            endPos = QPointF(0, targetWidget()->boundingRect().height());
        } else if (style()->to() == "left") {
            endPos = QPointF(-targetWidget()->boundingRect().width(), 0);
        }

        d->positionAnimation->setStartValue(d->originalPos);
        d->positionAnimation->setEndValue(endPos);
        d->positionAnimation->setDuration(style()->duration());
    }
}
