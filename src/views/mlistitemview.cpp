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

#include "mviewconstants.h"

#include <MListItem>

#include <QGraphicsSceneMouseEvent>
#include <QTapAndHoldGesture>
#include <QTimer>


MListItemViewPrivate::MListItemViewPrivate(MWidgetController *controller) 
    : MWidgetViewPrivate(),
    down(false),
    queuedStyleModeChange(false),
    styleModeChangeTimer(0)
{
    this->controller = dynamic_cast<MListItem *>(controller);
}

MListItemViewPrivate::~MListItemViewPrivate()
{
}

void MListItemViewPrivate::_q_applyQueuedStyleModeChange()
{
    if (queuedStyleModeChange) {
        queuedStyleModeChange = false;
        updateStyleMode();
    }
}

void MListItemViewPrivate::updateStyleMode()
{
    Q_Q(MListItemView);

    if (down) {
        if (styleModeChangeTimer->isActive()) {
            styleModeChangeTimer->start(M_PRESS_STYLE_TIMEOUT);
            return;
        }
        styleModeChangeTimer->start(M_PRESS_STYLE_TIMEOUT);
        q->style().setModePressed();
    } else {
        if (controller->isSelected()) {
            q->style().setModeSelected();
        } else {
            if (styleModeChangeTimer->isActive()) {
                queuedStyleModeChange = true;
                return;
            }
            q->style().setModeDefault();
        }
    }

    q->applyStyle();
    q->update();
}

void MListItemViewPrivate::click()
{
    controller->click();
}

void MListItemViewPrivate::longTap(const QPointF &pos)
{
    controller->longTap(pos);
}

MListItemView::MListItemView(MWidgetController *controller) 
    : MWidgetView(new MListItemViewPrivate(controller))
{
    Q_D(MListItemView);

    d->styleModeChangeTimer = new QTimer(this);
    d->styleModeChangeTimer->setSingleShot(true);
    connect(d->styleModeChangeTimer, SIGNAL(timeout()), SLOT(_q_applyQueuedStyleModeChange()));
}

void MListItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    if (d->down)
        return;
    
    style()->pressFeedback().play();
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
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    bool pressed = rect.contains(touch);

    if (pressed) {
        style()->releaseFeedback().play();
        d->click();
    }
}

void MListItemView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    QPointF touch = event->scenePos();
    QRectF rect = d->controller->sceneBoundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    bool pressed = rect.contains(touch);

    if (pressed != d->down) {
        if (pressed) {
            style()->pressFeedback().play();
        } else {
            style()->cancelFeedback().play();
        }
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

    style()->cancelFeedback().play();
    d->down = false;
    d->updateStyleMode();
}

void MListItemView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    Q_D(MListItemView);

    if (gesture->state() == Qt::GestureFinished)
        d->longTap(gesture->position());

    event->accept(gesture);
}

M_REGISTER_VIEW_NEW(MListItemView, MListItem)

#include "moc_mlistitemview.cpp"
