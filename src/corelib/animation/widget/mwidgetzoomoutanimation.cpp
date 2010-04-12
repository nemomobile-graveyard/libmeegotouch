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

#include "mwidgetzoomoutanimation.h"
#include "mwidgetzoomoutanimation_p.h"
#include "mwidgetview.h"
#include "manimationcreator.h"

M_REGISTER_ANIMATION(MWidgetZoomOutAnimation)

MWidgetZoomOutAnimation::MWidgetZoomOutAnimation(MWidgetZoomOutAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
}

MWidgetZoomOutAnimation::MWidgetZoomOutAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetZoomOutAnimationPrivate, parent)
{
}

MWidgetZoomOutAnimation::~MWidgetZoomOutAnimation()
{
}

void MWidgetZoomOutAnimation::updateCurrentTime(int currentTime)
{
    Q_D(MWidgetZoomOutAnimation);
    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);
    view()->setScale(d->startScale - (d->startScale) * value);
}

void MWidgetZoomOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetZoomOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->startScale = view()->scale();
    } else if (oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::Stopped) {
        view()->hide();
    }
}
