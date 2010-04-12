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

#include "mwidgetfadeoutanimation.h"
#include "mwidgetfadeoutanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>
#include <mwidgetcontroller.h>

MWidgetFadeOutAnimation::MWidgetFadeOutAnimation(MWidgetFadeOutAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MWidgetFadeOutAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

MWidgetFadeOutAnimation::MWidgetFadeOutAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MWidgetFadeOutAnimationPrivate, parent)
{
    Q_D(MWidgetFadeOutAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

MWidgetFadeOutAnimation::~MWidgetFadeOutAnimation()
{
}

void MWidgetFadeOutAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MWidgetFadeOutAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->opacityAnimation->setTargetObject(targetWidget());
}

void MWidgetFadeOutAnimation::restoreTargetWidgetState()
{
    Q_D(MWidgetFadeOutAnimation);
    if (d->played)
        targetWidget()->setOpacity(d->originalOpacity);
}

void MWidgetFadeOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MWidgetFadeOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->originalOpacity = targetWidget()->opacity();
        d->played = true;

        d->opacityAnimation->setStartValue(d->originalOpacity);
        d->opacityAnimation->setEndValue(style()->opacity());
        d->opacityAnimation->setDuration(style()->duration());
    }
}
