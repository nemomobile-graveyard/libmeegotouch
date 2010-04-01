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

#include "duiwidgetzoomoutanimation.h"
#include "duiwidgetzoomoutanimation_p.h"
#include "duiwidgetview.h"
#include "duianimationcreator.h"

DUI_REGISTER_ANIMATION(DuiWidgetZoomOutAnimation)

DuiWidgetZoomOutAnimation::DuiWidgetZoomOutAnimation(DuiWidgetZoomOutAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
}

DuiWidgetZoomOutAnimation::DuiWidgetZoomOutAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetZoomOutAnimationPrivate, parent)
{
}

DuiWidgetZoomOutAnimation::~DuiWidgetZoomOutAnimation()
{
}

void DuiWidgetZoomOutAnimation::updateCurrentTime(int currentTime)
{
    Q_D(DuiWidgetZoomOutAnimation);
    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);
    view()->setScale(d->startScale - (d->startScale) * value);
}

void DuiWidgetZoomOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetZoomOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->startScale = view()->scale();
    } else if (oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::Stopped) {
        view()->hide();
    }
}
