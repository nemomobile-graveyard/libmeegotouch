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

#include "mlistitemview.h"
#include "mlistitemview_p.h"

#include "mviewconstants.h"
#include "mtapstatemachine.h"

#include <MCancelEvent>
#include <MListItem>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QTapAndHoldGesture>

MListItemViewPrivate::MListItemViewPrivate(MWidgetController *controller) 
    : MWidgetViewPrivate(),
    down(false),
    tapAndHoldStarted(false),
    expectMouseReleaseEvent(false),
    tapStateMachine(0)
{
    this->controller = dynamic_cast<MListItem *>(controller);
}

MListItemViewPrivate::~MListItemViewPrivate()
{
}

void MListItemViewPrivate::init()
{
    Q_Q(MListItemView);

    tapStateMachine = new MTapStateMachine(controller);
    q->connect(tapStateMachine, SIGNAL(delayedPress()), q, SLOT(_q_applyPressedStyle()));
    q->connect(tapStateMachine, SIGNAL(release()), q, SLOT(_q_applyReleasedStyle()));
    q->connect(controller, SIGNAL(visibleChanged()), q, SLOT(_q_releaseOnHide()));

}

void MListItemViewPrivate::_q_applyPressedStyle()
{
    Q_Q(MListItemView);

    setPressed(true);

    if (tapAndHoldStarted && !q->style()->downStateEffect().isEmpty())
        controller->setGraphicsEffect(MTheme::effect(q->style()->downStateEffect()));

    q->applyStyle();
    q->update();
}

void MListItemViewPrivate::_q_applyReleasedStyle()
{
    Q_Q(MListItemView);

    setPressed(false);

    if (!tapAndHoldStarted && controller->graphicsEffect())
        controller->setGraphicsEffect(NULL);

    q->applyStyle();
    q->update();
}

void MListItemViewPrivate::_q_releaseOnHide()
{
    Q_Q(MListItemView);

    // Cancel pressed state when the item is hidden.
    // This cancel event does only part of the job, as it  won't cause a transition
    // in the MTapStateMachine. That transition will happen when the
    // machine detects the visibleChanged() signal from the widget.
    if (!controller->isVisible() && down) {
        MCancelEvent e;
        q->cancelEvent(&e);
    }
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

    d->init();
}

void MListItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    d->expectMouseReleaseEvent = true;

    d->pressScenePos = event->scenePos();

    if (d->down)
        return;
    
    style()->pressFeedback().play();
    d->down = true;
}

void MListItemView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    d->expectMouseReleaseEvent = false;

    if (!d->down)
        return;
    
    d->down = false;
    d->tapAndHoldStarted = false;

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

    if (!d->expectMouseReleaseEvent) {
        // The usual mouse-press -> mouse-move -> mouse-release cycle
        // has been interrupted by a cancel event.
        return;
    }

    QPointF touch = event->scenePos();

    // Make sure that only regular taps can click an item.
    // If the release point is too far from the press point then it's more
    // likely a failed attempt at swiping away the app.
    qreal distanceFromPressPoint = (touch - d->pressScenePos).manhattanLength();
    if (distanceFromPressPoint > style()->maxDistanceForClick()) {
        // It's too far from the press point.
        // Let's cancel it so that a click never takes place.
        QApplication::postEvent(d->controller, new MCancelEvent);
        return;
    }

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
    }
}

void MListItemView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MListItemView);

    d->expectMouseReleaseEvent = false;

    if (!d->down)
        return;

    style()->cancelFeedback().play();

    d->down = false;
    d->tapAndHoldStarted = false;
}

void MListItemView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    Q_D(MListItemView);

    if (gesture->state() == Qt::GestureStarted)
        d->tapAndHoldStarted = true;

    if (gesture->state() == Qt::GestureCanceled ||
        gesture->state() == Qt::GestureFinished)
        d->tapAndHoldStarted = false;

    if (gesture->state() == Qt::GestureFinished && d->down)
        d->longTap(gesture->position());

    event->accept(gesture);
}

M_REGISTER_VIEW_NEW(MListItemView, MListItem)

#include "moc_mlistitemview.cpp"
