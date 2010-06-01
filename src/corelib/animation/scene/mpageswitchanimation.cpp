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

#include <QPropertyAnimation>

#include "mpageswitchanimation.h"
#include "mpageswitchanimation_p.h"
#include "mscenewindow.h"
#include "mscenemanager.h"

MPageSwitchAnimation::MPageSwitchAnimation(QObject *parent) :
    MParallelAnimationGroup(new MPageSwitchAnimationPrivate, parent)
{
    Q_D(MPageSwitchAnimation);

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

MPageSwitchAnimation::MPageSwitchAnimation(MPageSwitchAnimationPrivate *dd, QObject *parent) :
    MParallelAnimationGroup(dd, parent)
{
}

void MPageSwitchAnimation::setNewPage(MSceneWindow *newPage)
{
    Q_D(MPageSwitchAnimation);

    d->newPage = newPage;
}

void MPageSwitchAnimation::setOldPage(MSceneWindow *oldPage)
{
    Q_D(MPageSwitchAnimation);

    d->oldPage = oldPage;
}

void MPageSwitchAnimation::setPageTransitionDirection(PageTransitionDirection direction)
{
    Q_D(MPageSwitchAnimation);

    d->direction = direction;
}

MSceneWindow *MPageSwitchAnimation::oldPage() const
{
    Q_D(const MPageSwitchAnimation);
    return d->oldPage;
}

MSceneWindow *MPageSwitchAnimation::newPage() const
{
    Q_D(const MPageSwitchAnimation);
    return d->newPage;
}

void MPageSwitchAnimation::updateState(QAbstractAnimation::State newState,
        QAbstractAnimation::State oldState)
{
    Q_D(MPageSwitchAnimation);
    Q_UNUSED(oldState);

    if (newState != Running)
        return;

    d->positionNewPageAnimation->setTargetObject(d->newPage);
    d->positionOldPageAnimation->setTargetObject(d->oldPage);

    if (d->newPage) {
        if (d->direction == RightToLeft)
            d->positionNewPageAnimation->setStartValue(QPointF(d->newPage->boundingRect().width(), 0));
        else
            d->positionNewPageAnimation->setStartValue(QPointF(-d->newPage->boundingRect().width(), 0));
    }

    if (d->oldPage) {
        if (d->direction == RightToLeft)
            d->positionOldPageAnimation->setEndValue(QPointF(-d->oldPage->boundingRect().width(), 0));
        else
            d->positionOldPageAnimation->setEndValue(QPointF(d->oldPage->boundingRect().width(), 0));
    }
}

#include "moc_mpageswitchanimation.cpp"
