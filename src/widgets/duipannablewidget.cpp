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

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QTapAndHoldGesture>

#include "duipannablewidget.h"
#include "duipannablewidget_p.h"
#include "duitheme.h"
#include "duicancelevent.h"

#include "duiondisplaychangeevent.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiPannableWidget)

namespace
{
    //! Limiter for the resent list
    const int ResentListMaxSize = 10;
    //! Z-value of the glass
    const int ZValueGlass       =  2;
    //! Hardcoded timeout value for tap&hold gesture;
    const int TapAndHoldTimeoutValue = 500;
}

/*
 * DuiPannableWidget handles the event interception by setting up a
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


//! \cond

class DuiPannableWidgetGlass : public DuiWidget
{
public:
    DuiPannableWidgetGlass(QGraphicsItem *parent = 0);
    virtual ~DuiPannableWidgetGlass();

    virtual QRectF boundingRect() const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void ungrabMouseEvent(QEvent *event);
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);

    DuiPannableWidget *pannableWidget;
};

//! \endcond

void DuiPannableWidgetGlass::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture)
{
    if (gesture->state() == Qt::GestureFinished)
        pannableWidget->glassLongTapEvent();
    event->accept(gesture);
}

DuiPannableWidgetGlass::DuiPannableWidgetGlass(QGraphicsItem *parent) :
    DuiWidget(parent)
{
    this->pannableWidget = dynamic_cast<DuiPannableWidget *>(parent);
}


DuiPannableWidgetGlass::~DuiPannableWidgetGlass()
{
}


QRectF DuiPannableWidgetGlass::boundingRect() const
{
    return QRectF(QPointF(), pannableWidget->size());
}


void DuiPannableWidgetGlass::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->glassMousePressEvent(event);
}


void DuiPannableWidgetGlass::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->glassMouseMoveEvent(event);
}


void DuiPannableWidgetGlass::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->glassMouseReleaseEvent(event);
}

void DuiPannableWidgetGlass::ungrabMouseEvent(QEvent *event)
{
    pannableWidget->glassUngrabMouseEvent(event);
}

DuiPannableWidgetPrivate::DuiPannableWidgetPrivate() :
    state(DuiPannableWidgetPrivate::Wait),
    itemCount(0),
    pressEvent(QEvent::GraphicsSceneMousePress),
    physics(0),
    mouseGrabber(0),
    resentList()
{
}

DuiPannableWidgetPrivate::~DuiPannableWidgetPrivate()
{
    delete physics;
}

void DuiPannableWidgetPrivate::translateEventToItemCoordinates(const QGraphicsItem *srcItem, const QGraphicsItem *destItem, QGraphicsSceneMouseEvent *event)
{
    //we only handle left button here. Every other buttons are filtered out in the glassMousePressEvents();
    event->setButtonDownPos(Qt::LeftButton, destItem->mapFromItem(srcItem, event->buttonDownPos(Qt::LeftButton)));
    event->setLastPos(destItem->mapFromItem(srcItem, event->lastPos()));
    event->setPos(destItem->mapFromItem(srcItem, event->pos()));
}

void DuiPannableWidgetPrivate::resetState()
{
    physics->pointerRelease();
    physics->stop();
    state = DuiPannableWidgetPrivate::Wait;
}

void DuiPannableWidgetPrivate::deliverMouseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_Q(DuiPannableWidget);
    if (mouseGrabber && q->scene()->items().contains(mouseGrabber)) {
        translateEventToItemCoordinates(glass, mouseGrabber, event);
        q->scene()->sendEvent(mouseGrabber, event);
    }
}

DuiPannableWidget::DuiPannableWidget(DuiPannableWidgetPrivate *dd, DuiPannableWidgetModel *model,
                                     QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    init();
}


DuiPannableWidget::DuiPannableWidget(QGraphicsItem *parent) :
    DuiWidgetController(new DuiPannableWidgetPrivate(), new DuiPannableWidgetModel, parent)
{
    init();
}

void DuiPannableWidget::init()
{
    Q_D(DuiPannableWidget);

    d->physics = new DuiPhysics2DPanning(this);

    connect(d->physics, SIGNAL(updatePosition(QPointF)),
            this,       SLOT(updatePosition(QPointF)));

    connect(d->physics, SIGNAL(panningStopped()),
            this,       SIGNAL(panningStopped()));

    d->glass = new DuiPannableWidgetGlass(this);
    d->glass->setZValue(ZValueGlass);

    d->glass->setObjectName("glass");
    d->glass->grabGesture(Qt::TapAndHoldGesture);

    setPosition(QPointF());
    setRange(QRectF());
}

DuiPannableWidget::~DuiPannableWidget()
{
}


DuiPhysics2DPanning *DuiPannableWidget::physics() const
{
    Q_D(const DuiPannableWidget);
    return d->physics;
}

void DuiPannableWidget::setPhysics(DuiPhysics2DPanning *newPhysics)
{
    Q_D(DuiPannableWidget);

    if (!newPhysics) return;

    delete d->physics;
    d->physics = newPhysics;

    connect(d->physics, SIGNAL(updatePosition(QPointF)),
            this,       SLOT(updatePosition(QPointF)));

    connect(d->physics, SIGNAL(panningStopped()),
            this,       SIGNAL(panningStopped()));

    emit physicsChanged();

    setPosition(QPointF());
    setRange(QRectF());
}

void DuiPannableWidget::setEnabled(bool enabled)
{
    Q_D(DuiPannableWidget);

    model()->setEnabled(enabled);

    if (!enabled) {
        d->physics->pointerRelease();

        d->state = DuiPannableWidgetPrivate::Wait;

        d->physics->stop();
    }
}


bool DuiPannableWidget::isEnabled()
{
    return model()->enabled();
}


void DuiPannableWidget::setRange(const QRectF &r)
{
    Q_D(const DuiPannableWidget);
    d->physics->setRange(r);
}


QRectF DuiPannableWidget::range() const
{
    Q_D(const DuiPannableWidget);
    return d->physics->range();
}


void DuiPannableWidget::setPosition(const QPointF &p)
{
    Q_D(const DuiPannableWidget);
    d->physics->setPosition(p);
    emit positionChanged(p);
}


QPointF DuiPannableWidget::position() const
{
    Q_D(const DuiPannableWidget);
    return d->physics->position();
}


bool DuiPannableWidget::checkForResent(QEvent *event)
{
    Q_D(DuiPannableWidget);

    const int size = d->resentList.size();
    for (int i = 0; i < size; ++i) {
        const DuiPannableWidgetPrivate::resentItem &item = d->resentList.at(i);
        if (item.type      == (static_cast<QGraphicsSceneMouseEvent *>(event))->type()      &&
                item.screenPos == (static_cast<QGraphicsSceneMouseEvent *>(event))->screenPos() &&
                item.button    == (static_cast<QGraphicsSceneMouseEvent *>(event))->button()) {

            d->resentList.removeAt(i);

            return true;
        }
    }

    return false;
}

void DuiPannableWidget::updatePosition(const QPointF &position)
{
    Q_UNUSED(position);
}


void DuiPannableWidget::glassMousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPannableWidget);

    if (!isEnabled()) {
        // Glass: Ignoring, panning disabled

        event->ignore();
        return;
    }

    if (event->button() != Qt::LeftButton) {
        // Glass: Ignoring, not a left button

        event->ignore();
        return;
    }

    if (!(panDirection().testFlag(Qt::Horizontal) || panDirection().testFlag(Qt::Vertical))) {
        // Glass: Ignoring, no enabled panning directions

        event->ignore();
        return;
    }

    if (checkForResent(event)) {
        // Glass: Ignoring, already seen

        event->ignore();
        return;
    }

    switch (d->state) {
    case DuiPannableWidgetPrivate::Wait:
        // Saves the event so it can be passed forward if the
        // press doesn't end to be a panning action

        copyGraphicsSceneMouseEvent(d->pressEvent, *event);

        if (!d->physics->inMotion()) {
            // sending it now, we will send "cancel" if it will be needed.

            d->glass->ungrabMouse();
            this->resendEvent(&d->pressEvent);
            d->mouseGrabber = scene()->mouseGrabberItem();
            d->itemCount = scene()->items().size();
            d->glass->grabMouse();
        }

        d->physics->pointerPress(event->pos());

        d->state = DuiPannableWidgetPrivate::Evaluate;
        break;

    default:
        // Evaluate and pan states don't see press events
        break;
    }
}

void DuiPannableWidget::glassMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPannableWidget);

    if (!isEnabled()) {
        //Widget disabled in the middle of the gesture. We need to deliver
        //the events to the underlaying widget if we have it.
        d->deliverMouseEvent(event);
        d->resetState();
        //All events from this interaction has been delivered or dropped.
        //We can now safely become really disabled.
        d->mouseGrabber = 0;
        d->glass->ungrabMouse();
        return;
    }

    switch (d->state) {
    case DuiPannableWidgetPrivate::Evaluate: {

        d->glass->ungrabMouse();
        //ungrab event handler will ensure that physics is stopped
        //and pointer is released.

        QPointF velocity = d->physics->velocity();

        if (!d->physics->inMotion()) {
            // If the scene's item count has changed between mouse press and release,
            // there is a possibility that the mousegrabber pointer points to deleted
            // object which potentially leads to a crash.
            if (d->itemCount != scene()->items().size()) {
                resendEvent(&d->pressEvent); // we need to setup implicit mouse grabber
                resendEvent(event);
            } else {
                d->deliverMouseEvent(event);
            }
        } else if (qAbs(velocity.x()) < model()->panClickThreshold() &&
                   qAbs(velocity.y()) < model()->panClickThreshold()) {
            if (d->mouseGrabber) {
                resendEvent(&d->pressEvent); // we need to setup implicit mouse grabber
                resendEvent(event);
            }
        } else {
            sendCancel(&d->pressEvent);
        }

        d->state = DuiPannableWidgetPrivate::Wait;
        break;
    }
    case DuiPannableWidgetPrivate::Pan:
        d->physics->pointerRelease();

        d->state = DuiPannableWidgetPrivate::Wait;
        break;

    default:
        // Wait state sees a release event in case of a press / move
        // to a passive direction causing a stop of physics because of
        // physics being inmotion
        break;
    }
}


void DuiPannableWidget::glassMouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPannableWidget);
    QPointF delta;
    qreal distAct, distPass;

    if (!isEnabled()) {
        //Widget disabled in the middle of the gesture. We need to deliver
        //the events to the underlaying widget if we have it.
        d->deliverMouseEvent(event);
        d->resetState();
        //We don't ungrab at this point because we want to deliver
        //rest of the events from this interaction to the underlaying widget.
        return;
    }

    switch (d->state) {
    case DuiPannableWidgetPrivate::Evaluate:

        // Check if the movement is big enough to justify panning

        delta = event->pos() - d->pressEvent.pos();

        if (panDirection().testFlag(Qt::Horizontal) && !panDirection().testFlag(Qt::Vertical)) {
            distAct  = abs((int)delta.x());
            distPass = abs((int)delta.y());

        } else if (!panDirection().testFlag(Qt::Horizontal) && panDirection().testFlag(Qt::Vertical)) {
            distAct  = abs((int)delta.y());
            distPass = abs((int)delta.x());

        } else {
            // 0.7*(x + y) approximates sqrt(x^2 + y^2)

            distAct  = 0.7 * (abs((int)delta.x()) + abs((int)delta.y()));
            distPass = 0;
        }

        if (distAct > panThreshold()) {

            // This is panning, cancel the press event.
            sendCancel(&d->pressEvent);
            d->physics->pointerMove(event->pos());

            d->state = DuiPannableWidgetPrivate::Pan;
        } else if (distPass > panThreshold()) {
            d->physics->pointerRelease();

            if (!d->physics->inMotion()) {

                d->glass->ungrabMouse();

                resendEvent(&d->pressEvent); // we need to setup implicit mouse grabber
                resendEvent(event);
            } else {
                d->physics->stop();
            }

            d->state = DuiPannableWidgetPrivate::Wait;
        }

        break;

    case DuiPannableWidgetPrivate::Pan:

        d->physics->pointerMove(event->pos());

        d->state = DuiPannableWidgetPrivate::Pan;
        break;

    default:
        // Wait state sees a move event in case of a press / move to a
        // passive direction causing a stop of physics because of
        // physics being inmotion
        break;
    }
}

void DuiPannableWidget::glassUngrabMouseEvent(QEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiPannableWidget);

    //We will reset the state so that pannable widget
    //will be ready to receive next mousePress.
    switch (d->state) {
    case DuiPannableWidgetPrivate::Evaluate:
    case DuiPannableWidgetPrivate::Pan:
        d->resetState();
        break;
    default:
        break;
    }
}

void DuiPannableWidget::glassLongTapEvent()
{
    Q_D(DuiPannableWidget);

    //We will reset the state so that pannable widget
    //will be ready to receive next mousePress.
    switch (d->state) {
    case DuiPannableWidgetPrivate::Evaluate: {
        QGraphicsSceneContextMenuEvent contextEvent(QEvent::GraphicsSceneContextMenu);
        contextEvent.setPos(d->pressEvent.pos());
        contextEvent.setScenePos(d->pressEvent.scenePos());
        contextEvent.setScreenPos(d->pressEvent.screenPos());

        QApplication::sendEvent(scene(), &contextEvent);

        if (contextEvent.isAccepted()) {

            sendCancel(&d->pressEvent);
            d->resetState();

            //We will still receive mouse release, but
            //we aren't interested in it.
            d->mouseGrabber = 0;
            d->glass->ungrabMouse();
        }
        break;
    }
    default:
        break;
    }
}

void DuiPannableWidget::resendEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPannableWidget);

    QEvent::Type type;
    struct DuiPannableWidgetPrivate::resentItem resentItem;

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

    if ((this->scene() == NULL) || (this->scene()->views().size() == 0)) {

        // If this widget has been removed from the scene and/or there
        // is no view, return
        return;
    }

    QMouseEvent mouse_event(type,
                            this->scene()->views()[0]->mapFromScene(event->scenePos()),
                            event->screenPos(),
                            event->button(),
                            event->buttons(),
                            event->modifiers());

    /*
    duiDebug("DuiPannableWidget") << "Event: " << type
                                  << " " << this->scene()->views()[0]->mapFromScene(event->scenePos())
                                  << " " << event->screenPos()
                                  << " " << event->button()
                                  << " " << event->buttons()
                                  << " " << event->modifiers();
    */

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

    QApplication::sendEvent(this->scene()->views()[0]->viewport(), &mouse_event);
}

// onDisplayChangeEvent in DuiWidget handles DuiPannableWidgets in a
// wrong way: because a pannable widget fits always in viewRect of a
// window, it never passes MustBeResolved event to its children, even if
// the contents of the widget would not fit to its rectangle. This re-imp
// passes MustBeResolved in any case.
void DuiPannableWidget::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    Q_D(DuiPannableWidget);

    switch (event->state()) {
    case DuiOnDisplayChangeEvent::MustBeResolved:
    case DuiOnDisplayChangeEvent::FullyOnDisplay:
        if (!d->onDisplay || !d->onDisplaySet) {

            d->onDisplay = true;
            d->onDisplaySet = true;

            // Call the virtual handler and emit the signal
            enterDisplayEvent();
            emit displayEntered();

            DuiOnDisplayChangeEvent ev(DuiOnDisplayChangeEvent::MustBeResolved,
                                       event->viewRect());

            // Explicitly set the states of the children
            Q_FOREACH(QGraphicsItem * item, childItems()) {
                scene()->sendEvent(item, &ev);
            }
        }
        break;

    default:
        DuiWidget::onDisplayChangeEvent(event);
        break;
    }
}

void DuiPannableWidget::sendCancel(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiPannableWidget);
    Q_UNUSED(event);

    if ((this->scene() == NULL) || (this->scene()->views().size() == 0)) {

        // If this widget has been removed from the scene and/or there
        // is no view, return
        return;
    }

    if (!d->mouseGrabber || !scene()->items().contains(d->mouseGrabber)) {
        // We don't have the recipient of the cancel event, don't send it.
        return;
    }

    DuiCancelEvent cancelEvent;
    scene()->sendEvent(d->mouseGrabber, &cancelEvent);
}

void DuiPannableWidget::setPanDirection(const Qt::Orientations &panDirection)
{
    Q_D(DuiPannableWidget);
    model()->setPanDirection(panDirection);
    d->physics->setPanDirection(panDirection);
}

Qt::Orientations DuiPannableWidget::panDirection()
{
    return model()->panDirection();
}

void DuiPannableWidget::setPanThreshold(qreal value)
{
    model()->setPanThreshold(value);
}

qreal DuiPannableWidget::panThreshold()
{
    return model()->panThreshold();
}
