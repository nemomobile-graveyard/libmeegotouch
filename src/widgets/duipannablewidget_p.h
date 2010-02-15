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

#ifndef DUIPANNABLEWIDGET_P_H
#define DUIPANNABLEWIDGET_P_H

#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QTimer>

#include "private/duiwidgetcontroller_p.h"

class QPoint;
class QEvent;
class QTransform;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class DuiPhysics2DPanning;
class DuiStyle;
class DuiPannableWidgetGlass;

class DuiPannableWidgetPrivate : public DuiWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(DuiPannableWidget)
public:
    explicit DuiPannableWidgetPrivate();
    virtual ~DuiPannableWidgetPrivate();

    enum states {Wait, Evaluate, Pan};
    int state;
    int itemCount;

    QGraphicsSceneMouseEvent  pressEvent;

    DuiPhysics2DPanning *physics;
    DuiPannableWidgetGlass *glass;
    QGraphicsItem *mouseGrabber;

    struct resentItem {
        QEvent::Type     type;
        QPoint           screenPos;
        Qt::MouseButton  button;
    };

    QList<struct resentItem> resentList;

    int tapAndHoldTimerId;
public:

    /*!
     * \brief Internal method neccessary to correctly handle event positions.
     */
    void translateEventToItemCoordinates(const QGraphicsItem *srcItem,
                                         const QGraphicsItem *destItem,
                                         QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method used for resetting state of the pannable widget.
     */
    void resetState();

    /*!
     * \brief Method used for delivering event to interested widget.
     */
    void deliverMouseEvent(QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method used for starting tap&hold timer.
     */
    void tapAndHoldStartTimer();

    /*!
     * \brief Method used for stopping tap&hold timer.
     */
    void tapAndHoldStopTimer();
};

#endif
