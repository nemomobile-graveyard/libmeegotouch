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

#include "duiwidgetslideoutanimation.h"
#include "duiwidgetslideoutanimation_p.h"
#include "duiwidgetview.h"
#include "duianimationcreator.h"

DUI_REGISTER_ANIMATION(DuiWidgetSlideOutAnimation)

DuiWidgetSlideOutAnimation::DuiWidgetSlideOutAnimation(DuiWidgetSlideOutAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
}

DuiWidgetSlideOutAnimation::DuiWidgetSlideOutAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetSlideOutAnimationPrivate, parent)
{
}

DuiWidgetSlideOutAnimation::~DuiWidgetSlideOutAnimation()
{
}

void DuiWidgetSlideOutAnimation::updateCurrentTime(int currentTime)
{
    Q_D(DuiWidgetSlideOutAnimation);

    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);

    view()->setContentPosition(d->startPos + (d->endPos - d->startPos)*value);
}

void DuiWidgetSlideOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetSlideOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->startPos = view()->contentPosition();
        if (style()->to() == "top") {
            d->endPos = QPointF(0, -view()->boundingRect().height());
        } else if (style()->to() == "right") {
            d->endPos = QPointF(view()->boundingRect().width(), 0);
        } else if (style()->to() == "bottom") {
            d->endPos = QPointF(0, view()->boundingRect().height());
        } else if (style()->to() == "left") {
            d->endPos = QPointF(-view()->boundingRect().width(), 0);
        }
    } else if (oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::Stopped) {
        view()->hide();
    }
}
