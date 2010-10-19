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
#include "mscene.h"
M_REGISTER_WIDGET(MPannableWidget)

namespace
{
    //! Z-value of the glass
    const int ZValueGlass       =  2;
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

void MPannableWidgetGlass::cancelEvent(MCancelEvent *event)
{
    pannableWidget->cancelEvent(event);
}

void MPannableWidgetGlass::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    pannableWidget->tapAndHoldGestureEvent(event, gesture);
}

MPannableWidgetPrivate::MPannableWidgetPrivate() :
    physics(0),
    panGestureCausedCancelEvent(false)
{
}

MPannableWidgetPrivate::~MPannableWidgetPrivate()
{
    delete physics;
}

void MPannableWidgetPrivate::resetPhysics()
{
    physics->pointerRelease();
    physics->stop();
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
    d->glass->grabGesture(Qt::TapAndHoldGesture);

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

    if (policy == PanningAlwaysOff) {
        d->resetPhysics();
    }
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

    if (policy == PanningAlwaysOff) {
        d->resetPhysics();
    }
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


void MPannableWidget::updatePosition(const QPointF &position)
{
    Q_UNUSED(position);
}

void MPannableWidget::glassMousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_D(MPannableWidget);
    // This handler is called by the glass tapAndHoldGestureEvent method.
    if (isEnabled() && d->physics->inMotion()) {
        // The viewport is still moving,
        // let's swallow this event
        d->resetPhysics();
        mouseEvent->accept();
    } else {
        mouseEvent->ignore();
    }
}

void MPannableWidget::panGestureEvent(QGestureEvent *event, QPanGesture* panGesture)
{
    Q_D(MPannableWidget);

    QTransform itemTransform(sceneTransform().inverted());
    QPointF itemSpaceOffset = panGesture->offset() * itemTransform - QPointF(itemTransform.dx(),itemTransform.dy());

    //Ignoring pan gestures with directions not aligned with enabled pannign directions
    if (itemSpaceOffset.x() != 0) {
        if ( horizontalPanningPolicy() == PanningAlwaysOff ||
            (horizontalPanningPolicy() == PanningAsNeeded && range().height() == 0) ) {

            event->ignore(panGesture);
            d->panGestureCausedCancelEvent = true;
            return;
        }
    }

    if (itemSpaceOffset.y() != 0) {
        if ( verticalPanningPolicy() == PanningAlwaysOff ||
            (verticalPanningPolicy() == PanningAsNeeded && range().height() == 0) ) {

            event->ignore(panGesture);
            d->panGestureCausedCancelEvent = true;
            return;
        }
    }

    switch (panGesture->state())
    {
    case Qt::GestureStarted:

        {
            MScene *mScene = qobject_cast<MScene *>(scene());
            if (mScene)
                mScene->sendCancel();
        }

        d->physics->pointerPress(QPointF());
        //Fallthrough is intentional, we need to handle the movement that
        //is delivered with a started gesture.
    case Qt::GestureUpdated:
        d->physics->pointerMove(itemSpaceOffset);
        break;
    case Qt::GestureFinished:
        d->panGestureCausedCancelEvent = true;
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
    Q_D(MPannableWidget);

    if (d->panGestureCausedCancelEvent) {
        d->panGestureCausedCancelEvent = false;  
        return;
    }

    d->resetPhysics();
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

void MPannableWidget::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_D(MPannableWidget);

    d->resetPhysics();
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
    return static_cast<const MPannableWidget *>(this)->panDirection();
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
