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
class QTimer;

class MPannableWidgetGlass : public MWidget
{
public:
    MPannableWidgetGlass(MPannableWidget *pannableWidget = 0);

    virtual QRectF boundingRect() const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
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


    void init();
    void _q_resetPhysics();
    void glassMousePressEvent(QGraphicsSceneMouseEvent *event);

private:

    QTimer *mousePressPhysicsStopTimer;

    MPhysics2DPanning *physics;
    MPannableWidgetGlass *glass;

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_MPannableWidget;
#endif
};

#endif
