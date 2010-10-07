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

#include "msceneeventeater.h"
#include "mscene.h"
#include "mscenemanager.h"

#include <QEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <float.h>

MSceneEventEater::MSceneEventEater(QGraphicsItem *parent) :
        QGraphicsWidget(parent)
{
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    setFlag(QGraphicsItem::ItemHasNoContents);
    setFlag(QGraphicsItem::ItemStopsClickFocusPropagation);

    //FLT_MAX is maximal value for single precision floating-point number
    //We want this item to be on top of everything
    setZValue(FLT_MAX);
    setVisible(false);
}

bool MSceneEventEater::event(QEvent *event)
{
    if ((event->type() > QEvent::MouseButtonPress && event->type() < QEvent::MouseMove) ||
        (event->type() > QEvent::GraphicsSceneMouseMove && event->type() < QEvent::GraphicsSceneWheel) ||
        (event->type() > QEvent::HoverEnter && event->type() < QEvent::HoverMove) ||
        (event->type() == QEvent::Gesture)) {

        event->accept();
        return true;
    }
    return false;
}
