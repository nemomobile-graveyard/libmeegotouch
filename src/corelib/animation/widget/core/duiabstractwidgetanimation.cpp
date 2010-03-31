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

#include "duiabstractwidgetanimation.h"
#include "duiabstractwidgetanimation_p.h"


DuiAbstractWidgetAnimationPrivate::DuiAbstractWidgetAnimationPrivate()
{
    targetWidget = 0;
}

DuiAbstractWidgetAnimation::DuiAbstractWidgetAnimation(DuiAbstractWidgetAnimationPrivate *dd, QObject *parent) :
   DuiParallelAnimationGroup(dd, parent)
{
}

DuiAbstractWidgetAnimation::DuiAbstractWidgetAnimation(QObject *parent) :
    DuiParallelAnimationGroup(new DuiAbstractWidgetAnimationPrivate, parent)
{
}

DuiAbstractWidgetAnimation::~DuiAbstractWidgetAnimation()
{
}

const DuiWidgetController *DuiAbstractWidgetAnimation::targetWidget() const
{
    Q_D(const DuiAbstractWidgetAnimation);
    return d->targetWidget;
}

DuiWidgetController *DuiAbstractWidgetAnimation::targetWidget()
{
    Q_D(DuiAbstractWidgetAnimation);
    return d->targetWidget;
}

void DuiAbstractWidgetAnimation::setTargetWidget(DuiWidgetController *widget)
{
    Q_D(DuiAbstractWidgetAnimation);

    d->targetWidget = widget;

    style().setParent(widget);
}
