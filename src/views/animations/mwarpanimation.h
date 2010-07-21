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

#ifndef MWARPANIMATION_H
#define MWARPANIMATION_H

#include <QtGlobal>
#include <QPointF>

#include <mwidget.h>
#include <mparallelanimationgroup.h>
#include <mwarpanimationstyle.h>

class QGraphicsWidget;
class QPropertyAnimation;

//! \internal

class MWarpAnimation : public MParallelAnimationGroup
{
    Q_OBJECT
    M_ANIMATION_GROUP(MWarpAnimationStyle)

public:

   enum WarpDirection {
        InFromLeft,
        InFromRight,
        OutFromLeft,
        OutFromRight
    };

    MWarpAnimation(MWidget *widget, WarpDirection direction = InFromLeft, QObject *parent=NULL);
    
public Q_SLOTS:
    void clearTarget();
    void pauseFinished();
protected:
    virtual void updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
    MWidget *target;
    WarpDirection warpDirection;
    QPropertyAnimation *offsetAnimation;
    QPropertyAnimation *opacityAnimation;
};

//! \internal_end

#endif
