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

#include "duiwidgetfadeoutanimation.h"
#include "duiwidgetfadeoutanimation_p.h"
#include "duiwidgetview.h"
#include "duianimationcreator.h"

DUI_REGISTER_ANIMATION(DuiWidgetFadeOutAnimation)

DuiWidgetFadeOutAnimation::DuiWidgetFadeOutAnimation(DuiWidgetFadeOutAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
    dd->startOpacity = 255;
}

DuiWidgetFadeOutAnimation::DuiWidgetFadeOutAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetFadeOutAnimationPrivate, parent)
{
    Q_D(DuiWidgetFadeOutAnimation);
    d->startOpacity = 255;
}

DuiWidgetFadeOutAnimation::~DuiWidgetFadeOutAnimation()
{
}

void DuiWidgetFadeOutAnimation::updateCurrentTime(int currentTime)
{
    Q_D(DuiWidgetFadeOutAnimation);

    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);

    view()->setOpacity(d->startOpacity + (style()->opacity() - d->startOpacity)*value);
}

void DuiWidgetFadeOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetFadeOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->startOpacity = view()->opacity();
    } else if (oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::Stopped) {
        view()->hide();
    }
}
