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
 

#include "mwidgetrectangularclipanimation.h"
#include "mwidgetrectangularclipanimation_p.h"

#include "mgraphicsclipeffect.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>

MWidgetRectangularClipAnimationPrivate::MWidgetRectangularClipAnimationPrivate()
    : MAbstractWidgetAnimationPrivate(),
    clipAnimation(NULL),
    clipEffect(NULL),
    transitionDirection(MWidgetRectangularClipAnimation::In)
{
}

MWidgetRectangularClipAnimationPrivate::~MWidgetRectangularClipAnimationPrivate()
{
}

void MWidgetRectangularClipAnimationPrivate::init()
{
    Q_Q(MWidgetRectangularClipAnimation);

    if (!clipAnimation) {
        clipAnimation = new QPropertyAnimation(q);
        clipAnimation->setPropertyName("drawRect");
    }

    q->addAnimation(clipAnimation);

    if (!clipEffect)
        clipEffect = new MGraphicsClipEffect(q);
}

MWidgetRectangularClipAnimation::MWidgetRectangularClipAnimation(QObject *parent)
    : MAbstractWidgetAnimation(new MWidgetRectangularClipAnimationPrivate, parent)
{
    Q_D(MWidgetRectangularClipAnimation);

    d->init();
}

MWidgetRectangularClipAnimation::MWidgetRectangularClipAnimation(MWidgetRectangularClipAnimationPrivate *dd, QObject *parent)
    : MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetRectangularClipAnimation);

    d->init();
}

MWidgetRectangularClipAnimation::~MWidgetRectangularClipAnimation()
{
}

void MWidgetRectangularClipAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetRectangularClipAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    widget->setGraphicsEffect(d->clipEffect);
    d->clipAnimation->setTargetObject(d->clipEffect);
}

void MWidgetRectangularClipAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MWidgetRectangularClipAnimation);

    d->transitionDirection = direction;
}

void MWidgetRectangularClipAnimation::restoreTargetWidgetState()
{
    targetWidget()->setGraphicsEffect(NULL);
}

void MWidgetRectangularClipAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetRectangularClipAnimation);
    Q_UNUSED(oldState);

    if (newState != Running)
        return;

    if (d->transitionDirection == MWidgetRectangularClipAnimation::In)
        style().setObjectName("In");
    else if (d->transitionDirection == MWidgetRectangularClipAnimation::Out)
        style().setObjectName("Out");

    QRect startRect = QRect(0.0, targetWidget()->size().height() / 2.f, targetWidget()->size().width(), 0.0);
    QRect endRect = QRect(0.0, 0.0, targetWidget()->size().width(), targetWidget()->size().height());

    d->clipEffect->setClipMargin(style()->clipMargin());
    d->clipAnimation->setEasingCurve(style()->easingCurve());
    d->clipAnimation->setDuration(style()->duration());

    if (d->transitionDirection == MWidgetRectangularClipAnimation::In) {
        d->clipEffect->setDrawRect(startRect);

        d->clipAnimation->setStartValue(startRect);
        d->clipAnimation->setEndValue(endRect);
    } else if (d->transitionDirection == MWidgetRectangularClipAnimation::Out) {
        d->clipEffect->setDrawRect(endRect);

        d->clipAnimation->setStartValue(endRect);
        d->clipAnimation->setEndValue(startRect);
    }
}

M_REGISTER_ANIMATION(MWidgetRectangularClipAnimation)
