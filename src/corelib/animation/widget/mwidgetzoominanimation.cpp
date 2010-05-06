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

#include "mwidgetzoominanimation.h"
#include "mwidgetzoominanimation_p.h"
#include "mwidgetview.h"
#include "manimationcreator.h"

M_REGISTER_ANIMATION(MWidgetZoomInAnimation)

MWidgetZoomInAnimation::MWidgetZoomInAnimation(MWidgetZoomInAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
}

MWidgetZoomInAnimation::MWidgetZoomInAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetZoomInAnimationPrivate, parent)
{
}

MWidgetZoomInAnimation::~MWidgetZoomInAnimation()
{
}

void MWidgetZoomInAnimation::resetToInitialState()
{
    view()->setScale(0.0);
}

void MWidgetZoomInAnimation::updateCurrentTime(int currentTime)
{
    Q_D(MWidgetZoomInAnimation);

    qreal progress = ((qreal)currentTime) / ((qreal)style()->duration());
    qreal value = style()->easingCurve().valueForProgress(progress);
    view()->setScale(d->startScale + (1 - d->startScale) * value);
}

void MWidgetZoomInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetZoomInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        view()->show();
        d->startScale = view()->scale();
    }
}
