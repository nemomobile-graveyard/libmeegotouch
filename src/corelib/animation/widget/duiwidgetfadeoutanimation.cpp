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
#include "duianimationcreator.h"

#include <QPropertyAnimation>
#include <duiwidgetcontroller.h>

DuiWidgetFadeOutAnimation::DuiWidgetFadeOutAnimation(DuiWidgetFadeOutAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
    Q_D(DuiWidgetFadeOutAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

DuiWidgetFadeOutAnimation::DuiWidgetFadeOutAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetFadeOutAnimationPrivate, parent)
{
    Q_D(DuiWidgetFadeOutAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

DuiWidgetFadeOutAnimation::~DuiWidgetFadeOutAnimation()
{
}

void DuiWidgetFadeOutAnimation::setTargetWidget(DuiWidgetController *widget)
{
    Q_D(DuiWidgetFadeOutAnimation);
    DuiAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->opacityAnimation->setTargetObject(targetWidget());
}

void DuiWidgetFadeOutAnimation::restoreTargetWidgetState()
{
    Q_D(DuiWidgetFadeOutAnimation);
    if (d->played)
        targetWidget()->setOpacity(d->originalOpacity);
}

void DuiWidgetFadeOutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetFadeOutAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        d->originalOpacity = targetWidget()->opacity();
        d->played = true;

        d->opacityAnimation->setStartValue(d->originalOpacity);
        d->opacityAnimation->setEndValue(style()->opacity());
        d->opacityAnimation->setDuration(style()->duration());
    }
}
