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
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void timerEvent(QTimerEvent* event);
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
    QGraphicsItem *mouseGrabber;

    struct resentItem {
        QEvent::Type     type;
        QPoint           screenPos;
        Qt::MouseButton  button;
    };

    QList<struct resentItem> resentList;

    int pressDeliveryTimerId;
    bool panGestureCausedCancelEvent;
public:

    /*!
     * \brief Internal method necessary to correctly handle event positions.
     */
    void translateEventToItemCoordinates(const QGraphicsItem *srcItem,
                                         const QGraphicsItem *destItem,
                                         QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method used for resetting state of the physics engine.
     */
    void resetPhysics();

    /*!
     * \brief Method used for resetting glass and mouse grabber.
     */
    void resetMouseGrabber();

    /*!
     * \brief Method used for delivering event to interested widget.
     */
    void deliverMouseEvent(QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method used for delivering initial mouse press. It will set
     * the mouseGrabber if it is necessary at this point.
     */
    void deliverPressEvent();

    /*!
     * \brief Method used to start a timer which will trigger the delivery
     * of initial mouse press event.
     */
    void initialPressStartTimer();

    /*!
     * \brief Method used for cancelling the mouse press delivery timer.
     */
    void initialPressStopTimer();

    /*!
     * \brief Method used for getting an enabled parent pannable widget.
     */
    MPannableWidget* parentPannableWidget();
};

#endif
