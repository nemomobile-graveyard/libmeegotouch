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

#ifndef MBASELISTITEMDELETIONANIMATION_H
#define MBASELISTITEMDELETIONANIMATION_H

#include <QtGlobal>
#include <QPointF>
#include <QModelIndex>

#include <mwidget.h>
#include <mparallelanimationgroup.h>
#include "mbasiclistitemdeletionanimationstyle.h"

class QGraphicsWidget;
class QPropertyAnimation;

class MBasicListItemDeletionAnimationPrivate;

//! \internal

class MBasicListItemDeletionAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    M_ANIMATION_GROUP(MBasicListItemDeletionAnimationStyle)

public:
    MBasicListItemDeletionAnimation(QObject *parent = NULL);
    virtual ~MBasicListItemDeletionAnimation();

    virtual void appendBeforeTarget(MWidget *cell);
    virtual void appendDeleteTarget(MWidget *cell);
    virtual void appendAfterTarget(MWidget *cell, const QPointF &targetDestination);

private:
    MBasicListItemDeletionAnimationPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MBasicListItemDeletionAnimation)
    Q_DISABLE_COPY(MBasicListItemDeletionAnimation)

    Q_PRIVATE_SLOT(d_func(), void _q_resetAnimation())
};

//! \internal_end

#endif
