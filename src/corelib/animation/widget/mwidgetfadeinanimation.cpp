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

#include "mwidgetfadeinanimation.h"
#include "mwidgetfadeinanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>
#include <mwidgetcontroller.h>

MWidgetFadeInAnimation::MWidgetFadeInAnimation(MWidgetFadeInAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetFadeInAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

MWidgetFadeInAnimation::MWidgetFadeInAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetFadeInAnimationPrivate, parent)
{
    Q_D(MWidgetFadeInAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

MWidgetFadeInAnimation::~MWidgetFadeInAnimation()
{
}

void MWidgetFadeInAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetFadeInAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->opacityAnimation->setTargetObject(targetWidget());
}

void MWidgetFadeInAnimation::restoreTargetWidgetState()
{
    Q_D(MWidgetFadeInAnimation);
    if (d->played)
        targetWidget()->setOpacity(d->originalOpacity);
}

void MWidgetFadeInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetFadeInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        targetWidget()->show();

        d->originalOpacity = targetWidget()->opacity();
        d->played = true;

        d->opacityAnimation->setStartValue(0.0);
        d->opacityAnimation->setEndValue(style()->opacity());
        d->opacityAnimation->setDuration(style()->duration());
    }
}
