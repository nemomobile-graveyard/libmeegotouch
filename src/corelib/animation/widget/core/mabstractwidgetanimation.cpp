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

#include "mabstractwidgetanimation.h"
#include "mabstractwidgetanimation_p.h"


MAbstractWidgetAnimationPrivate::MAbstractWidgetAnimationPrivate()
{
    targetWidget = 0;
}

MAbstractWidgetAnimation::MAbstractWidgetAnimation(MAbstractWidgetAnimationPrivate *dd, QObject *parent) :
   MParallelAnimationGroup(dd, parent)
{
}

MAbstractWidgetAnimation::MAbstractWidgetAnimation(QObject *parent) :
    MParallelAnimationGroup(new MAbstractWidgetAnimationPrivate, parent)
{
}

MAbstractWidgetAnimation::~MAbstractWidgetAnimation()
{
}

const MWidgetController *MAbstractWidgetAnimation::targetWidget() const
{
    Q_D(const MAbstractWidgetAnimation);
    return d->targetWidget;
}

MWidgetController *MAbstractWidgetAnimation::targetWidget()
{
    Q_D(MAbstractWidgetAnimation);
    return d->targetWidget;
}

void MAbstractWidgetAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MAbstractWidgetAnimation);

    d->targetWidget = widget;

    style().setParent(widget);
}
