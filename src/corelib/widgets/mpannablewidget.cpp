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
#include "mscenemanager.h"

#include "mondisplaychangeevent.h"

#include "mwidgetcreator.h"
#include "mscene.h"
M_REGISTER_WIDGET(MPannableWidget)

namespace {
    const int ZValueGlass = 2;
    const int MousePressStopingMovementTimeout = 300; //ms.
}

MPannableWidgetGlass::MPannableWidgetGlass(MPannableWidget *pannableWidget) :
    MWidget(pannableWidget),
    pannableWidget(pannableWidget)
{
}

QRectF MPannableWidgetGlass::boundingRect() const
{
    return QRectF(QPointF(), pannableWidget->size());
}

void MPannableWidgetGlass::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pannableWidget->d_func()->glassMousePressEvent(event);
}

void MPannableWidgetGlass::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    pannableWidget->tapAndHoldGestureEvent(event, gesture);
}

MPannableWidgetPrivate::MPannableWidgetPrivate() :
    mousePressPhysicsStopTimer(0),
    physics(0),
    glass(0)
{
}

MPannableWidgetPrivate::~MPannableWidgetPrivate()
{
    delete physics;
}

void MPannableWidgetPrivate::init()
{
    Q_Q(MPannableWidget);

    physics = new MPhysics2DPanning(q);

    mousePressPhysicsStopTimer = new QTimer(q);
    mousePressPhysicsStopTimer->setInterval(MousePressStopingMovementTimeout);
    mousePressPhysicsStopTimer->setSingleShot(true);

    q->connect(mousePressPhysicsStopTimer, SIGNAL(timeout()),
               q,                          SLOT(_q_resetPhysics()));

    q->connect(physics, SIGNAL(positionChanged(QPointF)),
               q,       SLOT(updatePosition(QPointF)));

    q->connect(physics, SIGNAL(panningStopped()),
               q,       SIGNAL(panningStopped()));

    glass = new MPannableWidgetGlass(q);
    glass->setZValue(ZValueGlass);
    glass->setFlag(QGraphicsItem::ItemHasNoContents, true);

    glass->setObjectName("glass");
    glass->grabGesture(Qt::TapAndHoldGesture);

    q->grabGestureWithCancelPolicy(Qt::PanGesture, Qt::GestureFlags(), MWidget::MouseEventCancelOnGestureStarted);

    q->setPosition(QPointF());
    q->setRange(QRectF());
}

void MPannableWidgetPrivate::glassMousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_Q(MPannableWidget);
    // This handler is called by the glass tapAndHoldGestureEvent method.
    if (q->isEnabled() && physics->inMotion()) {
        // The viewport is still moving,
        // let's swallow this event
        mousePressPhysicsStopTimer->start();
        mouseEvent->accept();
    } else {
        mouseEvent->ignore();
    }
}

void MPannableWidgetPrivate::_q_resetPhysics()
{
    physics->pointerRelease();
    physics->stop();
}

MPannableWidget::MPannableWidget(MPannableWidgetPrivate *dd, MPannableWidgetModel *model,
                                     QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MPannableWidget);
    d->init();
}

MPannableWidget::MPannableWidget(QGraphicsItem *parent) :
    MWidgetController(new MPannableWidgetPrivate(), new MPannableWidgetModel, parent)
{
    Q_D(MPannableWidget);
    d->init();
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
    MWidgetController::setEnabled(enabled);
}

bool MPannableWidget::isEnabled()
{
    return MWidgetController::isEnabled();
}

void MPannableWidget::setVerticalPanningPolicy(PanningPolicy policy)
{
    Q_D(MPannableWidget);

    model()->setVerticalPanningPolicy(policy);
    d->physics->setPanDirection(panDirection());

    if (policy == PanningAlwaysOff)
        d->_q_resetPhysics();
}

MPannableWidget::PanningPolicy MPannableWidget::verticalPanningPolicy() const
{
    return (PanningPolicy) model()->verticalPanningPolicy();
}

void MPannableWidget::setHorizontalPanningPolicy(PanningPolicy policy)
{
    Q_D(MPannableWidget);

    model()->setHorizontalPanningPolicy(policy);
    d->physics->setPanDirection(panDirection());

    if (policy == PanningAlwaysOff)
        d->_q_resetPhysics();
}

MPannableWidget::PanningPolicy MPannableWidget::horizontalPanningPolicy() const
{
    return (PanningPolicy) model()->horizontalPanningPolicy();
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

void MPannableWidget::updatePosition(const QPointF &/*position*/)
{
}

void MPannableWidget::panGestureEvent(QGestureEvent *event, QPanGesture* panGesture)
{
    Q_D(MPannableWidget);

    d->mousePressPhysicsStopTimer->stop();

    QTransform itemTransform(sceneTransform().inverted());
    QPointF itemSpaceOffset = panGesture->offset() * itemTransform - QPointF(itemTransform.dx(),itemTransform.dy());

    //Ignoring pan gestures with directions not aligned with enabled pannign directions
    if (itemSpaceOffset.x() != 0) {
        if ( horizontalPanningPolicy() == PanningAlwaysOff ||
            (horizontalPanningPolicy() == PanningAsNeeded && range().width() == 0) ) {

            event->ignore(panGesture);
            d->_q_resetPhysics();
            return;
        }
    }

    if (itemSpaceOffset.y() != 0) {
        if ( verticalPanningPolicy() == PanningAlwaysOff ||
            (verticalPanningPolicy() == PanningAsNeeded && range().height() == 0) ) {

            event->ignore(panGesture);
            d->_q_resetPhysics();
            return;
        }
    }

    switch (panGesture->state())
    {
    case Qt::GestureStarted:
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

void MPannableWidget::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* tapAndHoldGesture)
{
    Q_D(MPannableWidget);
    // This handler is called by the glass tapAndHoldGestureEvent method.
    if (isEnabled() && d->physics->inMotion()) {
        // The viewport is still moving,
        // let's swallow this event and avoid showing
        // object menu.
        event->accept(tapAndHoldGesture);
    } else {
        event->ignore(tapAndHoldGesture);
    }
}

void MPannableWidget::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
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
        if (!d->onDisplay) {

            d->onDisplay = true;

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

void MPannableWidget::orientationChangeEvent(MOrientationChangeEvent *event)
{
    MWidgetController::orientationChangeEvent(event);
}

void MPannableWidget::setPanDirection(const Qt::Orientations &panDirection)
{
    if (panDirection.testFlag(Qt::Vertical))
        setVerticalPanningPolicy(PanningAlwaysOn);
    else
        setVerticalPanningPolicy(PanningAlwaysOff);

    if (panDirection.testFlag(Qt::Horizontal))
        setHorizontalPanningPolicy(PanningAlwaysOn);
    else
        setHorizontalPanningPolicy(PanningAlwaysOff);
}

Qt::Orientations MPannableWidget::panDirection()
{
    return const_cast<const MPannableWidget*>(this)->panDirection();
}

Qt::Orientations MPannableWidget::panDirection() const
{
    Qt::Orientations directions(0);

    if (verticalPanningPolicy() != PanningAlwaysOff)
        directions |= Qt::Vertical;

    if (horizontalPanningPolicy() != PanningAlwaysOff)
        directions |= Qt::Horizontal;

    return directions;
}

void MPannableWidget::setPanThreshold(qreal value)
{
    Q_UNUSED(value);
}

qreal MPannableWidget::panThreshold()
{
    return 0;
}

QVariant MPannableWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //we connect here because there was no scene manager before
    if(change == ItemSceneHasChanged && sceneManager()) {
        QObject::disconnect(this, SLOT(_q_resetPhysics()));
        connect(sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)),
                this, SLOT(_q_resetPhysics()));
    }
    return MWidgetController::itemChange(change, value);
}

#include "moc_mpannablewidget.cpp"
