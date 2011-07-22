/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MBASELISTITEMINSERTIONANIMATION_H
#define MBASELISTITEMINSERTIONANIMATION_H

#include <QtGlobal>
#include <QPointF>
#include <QModelIndex>

#include <mwidget.h>
#include <mparallelanimationgroup.h>
#include "mbasiclistiteminsertionanimationstyle.h"

class QGraphicsWidget;
class QPropertyAnimation;

class MBasicListItemInsertionAnimationPrivate;

//! \internal

class MBasicListItemInsertionAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    M_ANIMATION_GROUP(MBasicListItemInsertionAnimationStyle)

public:
    MBasicListItemInsertionAnimation(QObject *parent = NULL);
    virtual ~MBasicListItemInsertionAnimation();

    virtual void appendBeforeTarget(MWidget *cell);
    virtual void appendInsertTarget(MWidget *cell);
    virtual void appendAfterTarget(MWidget *cell, const QPointF &targetDestination);

private:
    MBasicListItemInsertionAnimationPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MBasicListItemInsertionAnimation)
    Q_DISABLE_COPY(MBasicListItemInsertionAnimation)

    Q_PRIVATE_SLOT(d_func(), void _q_resetAnimation())
};

//! \internal_end

#endif // MBASELISTITEMINSERTIONANIMATION_H
