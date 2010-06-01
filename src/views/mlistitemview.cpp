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

#include "mlistitemview.h"
#include "mlistitemview_p.h"

#include <MListItem>

#include <QGraphicsLayout>
#include <QGraphicsSceneMouseEvent>

#define RELEASE_MISS_DELTA 30.0

MListItemViewPrivate::MListItemViewPrivate(MWidgetController *controller) 
    : MWidgetViewPrivate(),
    down(false)
{
    this->controller = dynamic_cast<MListItem *>(controller);
}

MListItemViewPrivate::~MListItemViewPrivate()
{
}

void MListItemViewPrivate::updateStyleMode()
{
    Q_Q(MListItemView);

    if (down) {
        q->style().setModePressed();
    } else {
        if (controller->isSelected())
            q->style().setModeSelected();
        else
            q->style().setModeDefault();
    }

    q->applyStyle();
    q->update();
}

void MListItemViewPrivate::click()
{
    controller->click();
}

MListItemView::MListItemView(MWidgetController *controller) 
    : MWidgetView(new MListItemViewPrivate(controller))
{
}

void MListItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    if (d->down)
        return;
    
    d->down = true;
    d->updateStyleMode();
}

void MListItemView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    if (!d->down)
        return;
    
    d->down = false;
    d->updateStyleMode();    

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-RELEASE_MISS_DELTA, -RELEASE_MISS_DELTA,
                RELEASE_MISS_DELTA, RELEASE_MISS_DELTA);
    bool pressed = rect.contains(touch);

    if (pressed)
        d->click();
}

void MListItemView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-RELEASE_MISS_DELTA, -RELEASE_MISS_DELTA,
                RELEASE_MISS_DELTA, RELEASE_MISS_DELTA);
    bool pressed = rect.contains(touch);

    if (pressed != d->down) {
        d->down = pressed;
        d->updateStyleMode();
    }
}

void MListItemView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    if (!d->down)
        return;

    d->down = false;
    d->updateStyleMode();
}

M_REGISTER_VIEW_NEW(MListItemView, MListItem)
