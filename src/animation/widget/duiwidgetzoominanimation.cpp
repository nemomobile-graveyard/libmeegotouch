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

#include "duiwidgetzoominanimation.h"
#include "duiwidgetzoominanimation_p.h"
#include "duiwidgetview.h"
#include "duianimationcreator.h"

DUI_REGISTER_ANIMATION(DuiWidgetZoomInAnimation)

DuiWidgetZoomInAnimation::DuiWidgetZoomInAnimation(DuiWidgetZoomInAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
}

DuiWidgetZoomInAnimation::DuiWidgetZoomInAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetZoomInAnimationPrivate, parent)
{
}

DuiWidgetZoomInAnimation::~DuiWidgetZoomInAnimation()
{
}

void DuiWidgetZoomInAnimation::resetToInitialState()
{
    view()->setScale(0.0);
}

void DuiWidgetZoomInAnimation::updateCurrentTime(int currentTime)
{
    Q_D(DuiWidgetZoomInAnimation);

    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);
    view()->setScale(d->startScale + (1 - d->startScale) * value);
}

void DuiWidgetZoomInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetZoomInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        view()->show();
        d->startScale = view()->scale();
    }
}
