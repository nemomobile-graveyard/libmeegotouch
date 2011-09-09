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

#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <MCancelEvent>
#include "mcontainerheader_p.h"
#include "mviewconstants.h"
#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MContainerHeader)

/*
 * View
 */

M_REGISTER_VIEW(MContainerHeaderView, MContainerHeader)

MContainerHeaderView::MContainerHeaderView(MContainerHeader *controller)
    : MWidgetView(controller),
      expectMouseReleaseEvent(false)
{
    this->controller = controller;
    connect(this, SIGNAL(clicked()), controller, SIGNAL(clicked()));
}

void MContainerHeaderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    style().setModePressed();

    expectMouseReleaseEvent = true;
    pressScenePos = event->scenePos();

    update();
}

void MContainerHeaderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    style().setModeDefault();

    if (!expectMouseReleaseEvent) {
        return;
    }

    QPointF touch = event->scenePos();
    QRectF rect = controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    if (rect.contains(touch))
        emit clicked();

    expectMouseReleaseEvent = false;

    update();
}

void MContainerHeaderView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();

    if (!expectMouseReleaseEvent) {
        return;
    }

    QPointF touch = event->scenePos();
    qreal distanceFromPressPoint = (touch - pressScenePos).manhattanLength();

    QRectF rect = controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);

    if (distanceFromPressPoint > style()->maxDistanceForClick() || !rect.contains(touch)) {
        // Too far: cancel
        expectMouseReleaseEvent = false;
        QApplication::postEvent(controller, new MCancelEvent);
        return;
    }

    update();
}

void MContainerHeaderView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    expectMouseReleaseEvent = false;
    update();
}


/*
 * Controller
 */

MContainerHeader::MContainerHeader()
    : MWidgetController(new MWidgetModel, NULL)
{
    setView(new MContainerHeaderView(this));
}
