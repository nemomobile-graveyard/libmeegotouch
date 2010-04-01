/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duiwidgetslideinanimation.h"
#include "duiwidgetslideinanimation_p.h"
#include "duiwidgetview.h"
#include "duianimationcreator.h"

DUI_REGISTER_ANIMATION(DuiWidgetSlideInAnimation)

DuiWidgetSlideInAnimation::DuiWidgetSlideInAnimation(DuiWidgetSlideInAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
}

DuiWidgetSlideInAnimation::DuiWidgetSlideInAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetSlideInAnimationPrivate, parent)
{
}

DuiWidgetSlideInAnimation::~DuiWidgetSlideInAnimation()
{
}

void DuiWidgetSlideInAnimation::resetToInitialState()
{
    if (style()->from() == "top") {
        view()->setContentPosition(QPointF(0, -view()->boundingRect().height()));
    } else if (style()->from() == "right") {
        view()->setContentPosition(QPointF(view()->boundingRect().width(), 0));
    } else if (style()->from() == "bottom") {
        view()->setContentPosition(QPointF(0, view()->boundingRect().height()));
    } else if (style()->from() == "left") {
        view()->setContentPosition(QPointF(-view()->boundingRect().width(), 0));
    }
}

void DuiWidgetSlideInAnimation::updateCurrentTime(int currentTime)
{
    Q_D(DuiWidgetSlideInAnimation);

    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);

    view()->setContentPosition((1.0 - value) * d->startPos);
}

void DuiWidgetSlideInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetSlideInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        view()->show();
        if (style()->from() == "top") {
            d->startPos = QPointF(0, -view()->boundingRect().height());
        } else if (style()->from() == "right") {
            d->startPos = QPointF(view()->boundingRect().width(), 0);
        } else if (style()->from() == "bottom") {
            d->startPos = QPointF(0, view()->boundingRect().height());
        } else if (style()->from() == "left") {
            d->startPos = QPointF(-view()->boundingRect().width(), 0);
        }
    }
}
