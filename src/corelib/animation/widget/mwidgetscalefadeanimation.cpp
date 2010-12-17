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

#include "mwidgetscalefadeanimation.h"
#include "mwidgetscalefadeanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>

void MWidgetScaleFadeAnimationPrivate::init()
{
    Q_Q(MWidgetScaleFadeAnimation);

    direction = MWidgetScaleFadeAnimation::In;
    
    opacityAnimation = new QPropertyAnimation;
    opacityAnimation->setPropertyName("opacity");

    scaleAnimation = new QPropertyAnimation;
    scaleAnimation->setPropertyName("scale");

    QParallelAnimationGroup *parallelGroup = new QParallelAnimationGroup;
    parallelGroup->addAnimation(opacityAnimation);
    parallelGroup->addAnimation(scaleAnimation);

    q->addAnimation(parallelGroup);
}

MWidgetScaleFadeAnimation::MWidgetScaleFadeAnimation(QObject *parent) :
        MAbstractWidgetAnimation(new MWidgetScaleFadeAnimationPrivate, parent)
{
    Q_D(MWidgetScaleFadeAnimation);

    d->init();
}

MWidgetScaleFadeAnimation::MWidgetScaleFadeAnimation(MWidgetScaleFadeAnimationPrivate *dd, QObject *parent) :
        MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetScaleFadeAnimation);

    d->init();
}

void MWidgetScaleFadeAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetScaleFadeAnimation);

    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->opacityAnimation->setTargetObject(widget);
    d->scaleAnimation->setTargetObject(widget);
    
    widget->setTransformOriginPoint(widget->size().width()/2.0, widget->size().height()/2.0);
}

void MWidgetScaleFadeAnimation::restoreTargetWidgetState()
{
}

void MWidgetScaleFadeAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MWidgetScaleFadeAnimation);

    d->direction = direction;

    if (d->direction == In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");
}

void MWidgetScaleFadeAnimation::updateState(QAbstractAnimation::State newState,
                                             QAbstractAnimation::State oldState)
{
    Q_D(MWidgetScaleFadeAnimation);

    QAbstractAnimation::updateState(newState, oldState);

    if (!d->targetWidget)
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        if (style().objectName().isNull())
            style().setObjectName("In");

        if (d->direction == In) {
            d->scaleAnimation->setStartValue(style()->scale());
            d->scaleAnimation->setEndValue(1);
            d->opacityAnimation->setStartValue(0);
            d->opacityAnimation->setEndValue(style()->opacity());
        } else {
            d->opacityAnimation->setStartValue(style()->opacity());
            d->opacityAnimation->setEndValue(0);
            d->scaleAnimation->setStartValue(1);
            d->scaleAnimation->setEndValue(style()->scale());
        }

        //QEasingCurve bezierEasingCurve;
        //bezierEasingCurve.setCustomType(bezierEasingFunction);

        //d->opacityAnimation->setEasingCurve(bezierEasingCurve);
        d->opacityAnimation->setEasingCurve(style()->opacityAnimationEasingCurve());
        d->opacityAnimation->setDuration(style()->opacityAnimationDuration());

        //d->scaleAnimation->setEasingCurve(bezierEasingCurve);
        d->scaleAnimation->setEasingCurve(style()->scaleAnimationEasingCurve());
        d->scaleAnimation->setDuration(style()->scaleAnimationDuration());
    }
}

#include "moc_mwidgetscalefadeanimation.cpp"

M_REGISTER_ANIMATION(MWidgetScaleFadeAnimation)
