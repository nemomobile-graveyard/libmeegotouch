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

#include "private/mwidgetcontroller_p.h"

class QPoint;
class QEvent;
class QTransform;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class MPhysics2DPanning;
class MStyle;
class MPannableWidgetGlass;

class MPannableWidgetPrivate : public MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MPannableWidget)
public:
    explicit MPannableWidgetPrivate();
    virtual ~MPannableWidgetPrivate();

    enum states {Wait, Evaluate, Pan};
    int state;
    int itemCount;

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
public:

    /*!
     * \brief Internal method necessary to correctly handle event positions.
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
};

#endif
