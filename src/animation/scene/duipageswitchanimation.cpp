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

#include <QPropertyAnimation>

#include "duipageswitchanimation.h"
#include "duipageswitchanimation_p.h"
#include "duiscenewindow.h"

DuiPageSwitchAnimation::DuiPageSwitchAnimation(QObject *parent) :
    DuiParallelAnimationGroup(new DuiPageSwitchAnimationPrivate, parent)
{
    Q_D(DuiPageSwitchAnimation);

    d->newPage = 0;
    d->oldPage = 0;

    d->positionNewPageAnimation = new QPropertyAnimation;
    d->positionNewPageAnimation->setPropertyName("pos");
    d->positionNewPageAnimation->setEasingCurve(style()->easingCurve());
    d->positionNewPageAnimation->setDuration(style()->duration());
    d->positionNewPageAnimation->setEndValue(QPointF(0, 0));
    addAnimation(d->positionNewPageAnimation);

    d->positionOldPageAnimation = new QPropertyAnimation;
    d->positionOldPageAnimation->setPropertyName("pos");
    d->positionOldPageAnimation->setEasingCurve(style()->easingCurve());
    d->positionOldPageAnimation->setDuration(style()->duration());
    d->positionOldPageAnimation->setStartValue(QPointF(0, 0));
    addAnimation(d->positionOldPageAnimation);
}

DuiPageSwitchAnimation::DuiPageSwitchAnimation(DuiPageSwitchAnimationPrivate *dd, QObject *parent) :
    DuiParallelAnimationGroup(dd, parent)
{
}

void DuiPageSwitchAnimation::setNewPage(DuiSceneWindow *newPage)
{
    Q_D(DuiPageSwitchAnimation);

    d->newPage = newPage;
}

void DuiPageSwitchAnimation::setOldPage(DuiSceneWindow *oldPage)
{
    Q_D(DuiPageSwitchAnimation);

    d->oldPage = oldPage;
}

void DuiPageSwitchAnimation::setPageTransitionDirection(PageTransitionDirection direction)
{
    Q_D(DuiPageSwitchAnimation);

    d->direction = direction;
}

void DuiPageSwitchAnimation::updateState(QAbstractAnimation::State newState,
        QAbstractAnimation::State oldState)
{
    Q_D(DuiPageSwitchAnimation);
    Q_UNUSED(oldState);

    if (newState != Running)
        return;

    d->positionNewPageAnimation->setTargetObject(d->newPage);
    d->positionOldPageAnimation->setTargetObject(d->oldPage);

    if (d->newPage) {
        if (d->direction == LeftToRight)
            d->positionNewPageAnimation->setStartValue(QPointF(d->newPage->boundingRect().width(), 0));
        else
            d->positionNewPageAnimation->setStartValue(QPointF(-d->newPage->boundingRect().width(), 0));
    }

    if (d->oldPage) {
        if (d->direction == LeftToRight)
            d->positionOldPageAnimation->setEndValue(QPointF(-d->oldPage->boundingRect().width(), 0));
        else
            d->positionOldPageAnimation->setEndValue(QPointF(d->oldPage->boundingRect().width(), 0));
    }
}
