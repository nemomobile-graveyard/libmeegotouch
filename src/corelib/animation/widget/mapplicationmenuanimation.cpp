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

#include "mapplicationmenuanimation.h"
#include "mapplicationmenuanimation_p.h"
#include "manimationcreator.h"

#include <QPropertyAnimation>

MApplicationMenuAnimationPrivate::MApplicationMenuAnimationPrivate()
    : MDialogAnimationPrivate()
{
}

QPointF MApplicationMenuAnimationPrivate::setupPositionAnimation(const QPointF &widgetPos)
{
    Q_UNUSED(widgetPos);
    Q_Q(MApplicationMenuAnimation);

    return QPointF(0, q->style()->contentsViewportPositionAnimationDistance());
}

void MApplicationMenuAnimationPrivate::setupDurations()
{
    Q_Q(MApplicationMenuAnimation);

    MDialogAnimationPrivate::setupDurations();

    contentsViewportPositionAnimation->setDuration(q->style()->contentsViewportPositionAnimationDuration());
}

void MApplicationMenuAnimationPrivate::setupEasingCurves()
{
    Q_Q(MApplicationMenuAnimation);

    MDialogAnimationPrivate::setupEasingCurves();

    if (!q->style()->titleBarHasCustomEasingCurve()) {
        titleBarOpacityAnimation->setEasingCurve(q->style()->titleBarAnimationEasingCurve());
        titleBarPositionAnimation->setEasingCurve(q->style()->titleBarAnimationEasingCurve());
    }

    contentsViewportPositionAnimation->setEasingCurve(q->style()->contentsViewportPositionAnimationEasingCurve());
}

MApplicationMenuAnimation::MApplicationMenuAnimation(QObject *parent)
    : MDialogAnimation(new MApplicationMenuAnimationPrivate, parent)
{
}

MApplicationMenuAnimation::MApplicationMenuAnimation(MApplicationMenuAnimationPrivate *dd, QObject *parent)
    : MDialogAnimation(dd, parent)
{
}

#include "moc_mapplicationmenuanimation.cpp"

M_REGISTER_ANIMATION(MApplicationMenuAnimation)

