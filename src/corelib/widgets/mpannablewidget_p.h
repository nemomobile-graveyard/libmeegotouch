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

#ifndef MPANNABLEWIDGET_P_H
#define MPANNABLEWIDGET_P_H

#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QTimer>

#include "mwidget.h"
#include "private/mwidgetcontroller_p.h"

class QPoint;
class QEvent;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class MPhysics2DPanning;
class MPannableWidgetGlass;

class MPannableWidgetGlass : public MWidget
{
public:
    MPannableWidgetGlass(QGraphicsItem *parent = 0);
    virtual ~MPannableWidgetGlass();

    virtual QRectF boundingRect() const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture);

protected:

    MPannableWidget *pannableWidget;
};

class MPannableWidgetPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MPannableWidget)
public:
    explicit MPannableWidgetPrivate();
    virtual ~MPannableWidgetPrivate();

    QGraphicsSceneMouseEvent  pressEvent;

    MPhysics2DPanning *physics;
    MPannableWidgetGlass *glass;

    bool panGestureCausedCancelEvent;
public:

    /*!
     * \brief Method used for resetting state of the physics engine.
     */
    void resetPhysics();
};

#endif
