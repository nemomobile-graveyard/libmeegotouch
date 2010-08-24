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

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QPanGesture>

#include "mpannablewidget.h"
#include "mpannablewidget_p.h"
#include "mtheme.h"
#include "mcancelevent.h"

#include "mondisplaychangeevent.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MPannableWidget)

namespace
{
    //! Limiter for the resent list
    const int ResentListMaxSize = 10;
    //! Z-value of the glass
    const int ZValueGlass       =  2;
    //! Hardcoded timeout value for delivering initial press event;
    const int InitialPressDeliveryTimeoutValue = 140;

}

/*
 * MPannableWidget handles the event interception by setting up a
 * 'glass' widget on top of the pannable widget and its possible
 * children. Glass forwards all mouse events to pannable widget which
 * interprets them.  The press event is just stored first and in case
 * the gesture turns out to be not a panning one, the press is resend
 * to scene. It is also put to the exclude list and when seen for the
 * second time in mouse press handler, it is ignored. For this to
 * work, just before the resending the mouse is ungrabbed in the glass
 * which enables Qt to forward the event deeper after ignoring.
 * After the forwarded press, all events go directly to the possible
 * widget under the glass.
 *
 * If debugging of glass events is needed, it is recommend to override
 * sceneEvent since also the mouse grabs and ungrabs are seen on that
 * level.
 */

/**
 * Copies QGraphicsSceneMouseEvent data from one instance to another.
 * Note that this function copies only data defined in QGraphicsSceneMouseEvent.
 * Target event will keep its event type and other properties defined in QGraphicsSceneMouseEvent
 * subclasses.
 */
void copyGraphicsSceneMouseEvent(QGraphicsSceneMouseEvent &target, const QGraphicsSceneMouseEvent &source)
{
    target.setPos(source.pos());
    target.setScenePos(source.scenePos());
    target.setScreenPos(source.screenPos());

    target.setButtons(source.buttons());
    Qt::MouseButtons buttons = source.buttons();
    Qt::MouseButton buttonbit = (Qt::MouseButton)0x1;
    while (buttons != 0x0) {
        if (buttons & buttonbit) {
            // Button pressed
            target.setButtonDownPos(buttonbit, source.buttonDownPos(buttonbit));
            target.setButtonDownScenePos(buttonbit, source.buttonDownScenePos(buttonbit));
            target.setButtonDownScreenPos(buttonbit, source.buttonDownScreenPos(buttonbit));

            // Unset button
            buttons = buttons & ~buttonbit;
        }
        buttonbit = (Qt::MouseButton)(buttonbit << 1);
    }

    target.setLastPos(source.lastPos());
    target.setLastScenePos(source.lastScenePos());
    target.setLastScreenPos(source.lastScreenPos());
    target.setButton(source.button());
    target.setModifiers(source.modifiers());
}

MPannableWidgetGlass::MPannableWidgetGlass(QGraphicsItem *parent) :
    MWidget(parent)
{
    pannableWidget = dynamic_cast<MPannableWidget *>(parent);
}


MPannableWidgetGlass::~MPannableWidgetGlass()
{
}


QRectF MPannableWidgetGlass::boundingRect() const
{
    return QRectF(QPointF(), pannableWidget->size());
}


void MPannableWidgetGlass::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->glassMousePressEvent(event);
}


void MPannableWidgetGlass::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->glassMouseMoveEvent(event);
}


void MPannableWidgetGlass::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->glassMouseReleaseEvent(event);
}

void MPannableWidgetGlass::timerEvent(QTimerEvent *event)
{
    pannableWidget->glassTimerEvent(event);
}

MPannableWidgetPrivate::MPannableWidgetPrivate() :
    pressEvent(QEvent::GraphicsSceneMousePress),
    physics(0),
    mouseGrabber(0),
    resentList(),
    pressDeliveryTimerId(0)
{
}

MPannableWidgetPrivate::~MPannableWidgetPrivate()
{
    delete physics;
}

void MPannableWidgetPrivate::translateEventToItemCoordinates(const QGraphicsItem *srcItem, const QGraphicsItem *destItem, QGraphicsSceneMouseEvent *event)
{
    event->setLastPos(destItem->mapFromItem(srcItem, event->lastPos()));
    event->setPos(destItem->mapFromItem(srcItem, event->pos()));
}

void MPannableWidgetPrivate::deliverPressEvent()
{
    Q_Q(MPannableWidget);

    if (physics->inMotion())
    {
        physics->stop();
        return;
    }

    if (q->scene() == NULL) {
        // We have been removed from the scene
        // while waiting for timer to expire.
        // Reset the states and wait for next events.
        resetPhysics();
        resetMouseGrabber();
        return;
    }

    glass->ungrabMouse();
    q->resendEvent(&pressEvent);
    mouseGrabber = q->scene()->mouseGrabberItem();
    glass->grabMouse();
}

void MPannableWidgetPrivate::initialPressStartTimer()
{
    pressDeliveryTimerId = glass->startTimer(InitialPressDeliveryTimeoutValue);
}

void MPannableWidgetPrivate::initialPressStopTimer()
{
    if (pressDeliveryTimerId) {
        glass->killTimer(pressDeliveryTimerId);
        pressDeliveryTimerId = 0;
    }
}

void MPannableWidgetPrivate::resetPhysics()
{
    physics->pointerRelease();
    physics->stop();
}

void MPannableWidgetPrivate::resetMouseGrabber()
{
    Q_Q(MPannableWidget);

    mouseGrabber = 0;
    if (q->scene() && glass == q->scene()->mouseGrabberItem()) {
        glass->ungrabMouse();
    }
}

void MPannableWidgetPrivate::deliverMouseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_Q(MPannableWidget);
    if (mouseGrabber && q->scene()->items().contains(mouseGrabber)) {
        translateEventToItemCoordinates(glass, mouseGrabber, event);
        q->scene()->sendEvent(mouseGrabber, event);
    }
}

MPannableWidget* MPannableWidgetPrivate::parentPannableWidget()
{
    Q_Q(MPannableWidget);

    QGraphicsWidget* parentWidget = q->parentWidget();
    MPannableWidget* parentPannableWidget = 0;
    while(parentWidget && !parentPannableWidget) {
        parentPannableWidget = qobject_cast<MPannableWidget*>(parentWidget);
        if (parentPannableWidget && !parentPannableWidget->isEnabled())
            parentPannableWidget = 0;
        parentWidget = parentWidget->parentWidget();
    }

    return parentPannableWidget;
}

MPannableWidget::MPannableWidget(MPannableWidgetPrivate *dd, MPannableWidgetModel *model,
                                     QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    init();
}


MPannableWidget::MPannableWidget(QGraphicsItem *parent) :
    MWidgetController(new MPannableWidgetPrivate(), new MPannableWidgetModel, parent)
{
    init();
}

void MPannableWidget::init()
{
    Q_D(MPannableWidget);

    d->physics = new MPhysics2DPanning(this);

    connect(d->physics, SIGNAL(positionChanged(QPointF)),
            this,       SLOT(updatePosition(QPointF)));

    connect(d->physics, SIGNAL(panningStopped()),
            this,       SIGNAL(panningStopped()));

    d->glass = new MPannableWidgetGlass(this);
    d->glass->setZValue(ZValueGlass);

    d->glass->setObjectName("glass");
    grabGesture(Qt::PanGesture);

    setPosition(QPointF());
    setRange(QRectF());
}

MPannableWidget::~MPannableWidget()
{
}


MPhysics2DPanning *MPannableWidget::physics() const
{
    Q_D(const MPannableWidget);
    return d->physics;
}

void MPannableWidget::setPhysics(MPhysics2DPanning *newPhysics)
{
    Q_D(MPannableWidget);

    if (!newPhysics) return;

    delete d->physics;
    d->physics = newPhysics;

    connect(d->physics, SIGNAL(positionChanged(QPointF)),
            this,       SLOT(updatePosition(QPointF)));

    connect(d->physics, SIGNAL(panningStopped()),
            this,       SIGNAL(panningStopped()));

    emit physicsChanged();

    setPosition(QPointF());
    setRange(QRectF());
}

void MPannableWidget::setEnabled(bool enabled)
{
    Q_D(MPannableWidget);

    model()->setEnabled(enabled);

    if (!enabled) {
        d->resetPhysics();
    }
}


bool MPannableWidget::isEnabled()
{
    return model()->enabled();
}


void MPannableWidget::setRange(const QRectF &r)
{
    Q_D(const MPannableWidget);
    d->physics->setRange(r);
}


QRectF MPannableWidget::range() const
{
    Q_D(const MPannableWidget);
    return d->physics->range();
}


void MPannableWidget::setPosition(const QPointF &p)
{
    Q_D(const MPannableWidget);
    d->physics->setPosition(p);
    emit positionChanged(p);
}


QPointF MPannableWidget::position() const
{
    Q_D(const MPannableWidget);
    return d->physics->position();
}


bool MPannableWidget::checkForResent(QEvent *event)
{
    Q_D(MPannableWidget);

    const int size = d->resentList.size();
    for (int i = 0; i < size; ++i) {
        const MPannableWidgetPrivate::resentItem &item = d->resentList.at(i);
        if (item.type      == (static_cast<QGraphicsSceneMouseEvent *>(event))->type()      &&
                item.screenPos == (static_cast<QGraphicsSceneMouseEvent *>(event))->screenPos() &&
                item.button    == (static_cast<QGraphicsSceneMouseEvent *>(event))->button()) {

            d->resentList.removeAt(i);

            return true;
        }
    }

    return false;
}

void MPannableWidget::updatePosition(const QPointF &position)
{
    Q_UNUSED(position);
}


void MPannableWidget::glassMousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MPannableWidget);

    if (event->button() != Qt::LeftButton) {
        // Glass: Ignoring, not a left button

        event->ignore();
        return;
    }

    if (checkForResent(event)) {
        // Glass: Ignoring, already seen

        event->ignore();
        return;
    }

    if (isEnabled() == false) {
        // Glass: Ignoring, I'm disabled
        event->ignore();
        return;
    }

    MPannableWidget *parentPannableWidget = d->parentPannableWidget();
    if (parentPannableWidget) {

        //We are stopping here, because if there is a parent pannable widget
        //then the initial press timeout has already happened.
        d->physics->stop();

        //My parent will handle this mouse press.
        event->ignore();
        return;
    }

    copyGraphicsSceneMouseEvent(d->pressEvent, *event);
    d->initialPressStartTimer();
}

void MPannableWidget::glassMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MPannableWidget);

    if (d->pressDeliveryTimerId) {
        d->deliverPressEvent();
        d->initialPressStopTimer();
    }

    d->deliverMouseEvent(event);
    d->resetMouseGrabber();
}


void MPannableWidget::glassMouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MPannableWidget);
    d->deliverMouseEvent(event);
}

void MPannableWidget::panGestureEvent(QGestureEvent *event, QPanGesture* panGesture)
{
    Q_D(MPannableWidget);

    if (!isEnabled()) {
        event->ignore(panGesture);
        return;
    }

    QTransform itemTransform(sceneTransform().inverted());
    QPointF itemSpaceOffset = panGesture->offset() * itemTransform - QPointF(itemTransform.dx(),itemTransform.dy());

    if (panDirection().testFlag(Qt::Vertical) == false || panDirection().testFlag(Qt::Horizontal) == false) {
        //Ignoring gestures that aren't aligned to allowed pan direction.
        if ((itemSpaceOffset.x() != 0 && panDirection().testFlag(Qt::Vertical)) ||
            (itemSpaceOffset.y() != 0 && panDirection().testFlag(Qt::Horizontal)))
        {
            // Panning against the pannable direction, we aren't interested in it.
            event->ignore(panGesture);
            return;
        }
    }

    switch (panGesture->state())
    {
    case Qt::GestureStarted:

        //If we have parent pannable widget, then it is the parent
        //who handles the cancelling of mouse press. We need to inform
        //it that this needs to be done.
        {
            MPannableWidget *parentPannableWidget = d->parentPannableWidget();
            if (parentPannableWidget) {
                parentPannableWidget->sendCancel();
            } else {
                sendCancel();
            }
        }

        d->physics->pointerPress(QPointF());
        //Fallthrough is intentional, we need to handle the movement that
        //is delivered with a started gesture.
    case Qt::GestureUpdated:
        d->physics->pointerMove(itemSpaceOffset);
        break;
    case Qt::GestureFinished:
    case Qt::GestureCanceled:
        d->physics->pointerRelease();
        break;
    default:
        break;
    }

    event->accept(panGesture);
}

void MPannableWidget::glassTimerEvent(QTimerEvent *event)
{
    Q_D(MPannableWidget);
    if (event->timerId() == d->pressDeliveryTimerId) {
        d->deliverPressEvent();
        d->initialPressStopTimer();
    }
}

void MPannableWidget::resendEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MPannableWidget);

    QEvent::Type type;
    struct MPannableWidgetPrivate::resentItem resentItem;

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        type = QEvent::MouseButtonPress;
        break;

    case QEvent::GraphicsSceneMouseRelease:
        type = QEvent::MouseButtonRelease;
        break;

    case QEvent::GraphicsSceneMouseMove:
        type = QEvent::MouseMove;
        break;

    default:
        // Shouldn't end up here
        return;

        break;
    }

    if ((scene() == NULL) || (scene()->views().size() == 0)) {

        // If this widget has been removed from the scene and/or there
        // is no view, return
        return;
    }

    QMouseEvent mouse_event(type,
                            scene()->views().at(0)->mapFromScene(event->scenePos()),
                            event->screenPos(),
                            event->button(),
                            event->buttons(),
                            event->modifiers());

    if (type == QEvent::MouseButtonPress) {
        // Puts the event to exclude list

        resentItem.type      = event->type();
        resentItem.screenPos = event->screenPos();
        resentItem.button    = event->button();

        // Size limiter for the list. Prevents the list from growing in
        // unlimited fashion in case of an unexpected loss of events

        while (d->resentList.size() > ResentListMaxSize) {
            d->resentList.removeLast();
        }

        d->resentList.append(resentItem);
    }

    QApplication::sendEvent(scene()->views().at(0)->viewport(), &mouse_event);
}

void MPannableWidget::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MPannableWidget);

    d->resetPhysics();

    //We will still receive mouse release, but
    //we aren't interested in it.
    d->resetMouseGrabber();
}

// onDisplayChangeEvent in MWidget handles MPannableWidgets in a
// wrong way: because a pannable widget fits always in viewRect of a
// window, it never passes MustBeResolved event to its children, even if
// the contents of the widget would not fit to its rectangle. This re-imp
// passes MustBeResolved in any case.
void MPannableWidget::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    Q_D(MPannableWidget);

    switch (event->state()) {
    case MOnDisplayChangeEvent::MustBeResolved:
    case MOnDisplayChangeEvent::FullyOnDisplay:
        if (!d->onDisplay || !d->onDisplaySet) {

            d->onDisplay = true;
            d->onDisplaySet = true;

            // Call the virtual handler and emit the signal
            enterDisplayEvent();
            emit displayEntered();

            MOnDisplayChangeEvent ev(MOnDisplayChangeEvent::MustBeResolved,
                                       event->viewRect());

            // Explicitly set the states of the children
            Q_FOREACH(QGraphicsItem * item, childItems()) {
                scene()->sendEvent(item, &ev);
            }
        }
        break;

    default:
        MWidget::onDisplayChangeEvent(event);
        break;
    }
}

void MPannableWidget::sendCancel()
{
    Q_D(MPannableWidget);

    MCancelEvent cancelEvent;

    if ((scene() == NULL) || (scene()->views().size() == 0)) {

        // If this widget has been removed from the scene and/or there
        // is no view, return
        return;
    }

    if (d->pressDeliveryTimerId) {
        // The initial MousePress event hasn't been delivered yet.
        d->initialPressStopTimer();
    } else if (d->mouseGrabber && scene()->items().contains(d->mouseGrabber)) {
        scene()->sendEvent(d->mouseGrabber, &cancelEvent);
        d->mouseGrabber = 0;
    }

}

void MPannableWidget::setPanDirection(const Qt::Orientations &panDirection)
{
    Q_D(MPannableWidget);
    model()->setPanDirection(panDirection);
    d->physics->setPanDirection(panDirection);
}

Qt::Orientations MPannableWidget::panDirection()
{
    return model()->panDirection();
}

void MPannableWidget::setPanThreshold(qreal value)
{
    Q_UNUSED(value);
}

qreal MPannableWidget::panThreshold()
{
    return 0;
}
