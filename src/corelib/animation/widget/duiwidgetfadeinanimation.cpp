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
#include "duianimationcreator.h"

#include <QPropertyAnimation>
#include <duiwidgetcontroller.h>

DuiWidgetFadeInAnimation::DuiWidgetFadeInAnimation(DuiWidgetFadeInAnimationPrivate *dd, QObject *parent) :
    DuiAbstractWidgetAnimation(dd, parent)
{
    Q_D(DuiWidgetFadeInAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

DuiWidgetFadeInAnimation::DuiWidgetFadeInAnimation(QObject *parent) :
    DuiAbstractWidgetAnimation(new DuiWidgetFadeInAnimationPrivate, parent)
{
    Q_D(DuiWidgetFadeInAnimation);

    d->opacityAnimation = new QPropertyAnimation;
    d->opacityAnimation->setPropertyName("opacity");
    addAnimation(d->opacityAnimation);
}

DuiWidgetFadeInAnimation::~DuiWidgetFadeInAnimation()
{
}

void DuiWidgetFadeInAnimation::setTargetWidget(DuiWidgetController *widget)
{
    Q_D(DuiWidgetFadeInAnimation);
    DuiAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->opacityAnimation->setTargetObject(targetWidget());
}

void DuiWidgetFadeInAnimation::restoreTargetWidgetState()
{
    Q_D(DuiWidgetFadeInAnimation);
    if (d->played)
        targetWidget()->setOpacity(d->originalOpacity);
}

void DuiWidgetFadeInAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiWidgetFadeInAnimation);
    if (oldState == QAbstractAnimation::Stopped && newState == QAbstractAnimation::Running) {
        targetWidget()->show();

        d->originalOpacity = targetWidget()->opacity();
        d->played = true;

        d->opacityAnimation->setStartValue(0.0);
        d->opacityAnimation->setEndValue(style()->opacity());
        d->opacityAnimation->setDuration(style()->duration());
    }
}
