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

#ifndef MWIDGETMOVEANIMATION_H
#define MWIDGETMOVEANIMATION_H

#include <QtGlobal>

#include <mparallelanimationgroup.h>
#include <mwidgetmoveanimationstyle.h>

#include <QPointF>

class MWidgetMoveAnimationPrivate;
class QGraphicsWidget;

//! \internal

class MWidgetMoveAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MWidgetMoveAnimation)
    M_ANIMATION_GROUP(MWidgetMoveAnimationStyle)

public:
    MWidgetMoveAnimation(QObject *parent = NULL);
    void setWidget(QGraphicsWidget *widget);
    void setFinalPos(const QPointF &pos);

protected:
    MWidgetMoveAnimation(MWidgetMoveAnimationPrivate *dd, QObject *parent = NULL);
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
};

//! \internal_end

#endif
