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

#include "duiwidgetfadeinanimation.h"
#include "duiwidgetfadeinanimation_p.h"
#include "duiwidgetview.h"
#include "duianimationcreator.h"

DUI_REGISTER_ANIMATION(DuiWidgetFadeInAnimation)

DuiWidgetFadeInAnimation::DuiWidgetFadeInAnimation(DuiWidgetFadeInAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
    dd->startOpacity = 255;
}

DuiWidgetFadeInAnimation::DuiWidgetFadeInAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetFadeInAnimationPrivate, parent)
{
    Q_D(DuiWidgetFadeInAnimation);
    d->startOpacity = 255;
}

DuiWidgetFadeInAnimation::~DuiWidgetFadeInAnimation()
{
}

void DuiWidgetFadeInAnimation::resetToInitialState()
{
    view()->setOpacity(0.0);
}

void DuiWidgetFadeInAnimation::updateCurrentTime(int currentTime)
{
    Q_D(DuiWidgetFadeInAnimation);

    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);

    view()->setOpacity(d->startOpacity + (style()->opacity() - d->startOpacity)*value);
}

void DuiWidgetFadeInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetFadeInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        view()->show();
        d->startOpacity = view()->opacity();
    }
}
