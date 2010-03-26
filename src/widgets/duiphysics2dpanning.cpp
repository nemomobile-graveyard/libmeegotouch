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

#include <cstdio>
#include <QTimeLine>

#include "duipannablewidget.h"
#include "duiphysics2dpanning.h"
#include "duiphysics2dpanning_p.h"

static const int PanningTimelineDuration      = 1000000; /* in ms */
//static const int PanningTimelineInterval      =      20; /* in ms */
static const int PositionNoiseDampingDelta    =       2; /* in px */

DuiPhysics2DIntegrationStrategy::~DuiPhysics2DIntegrationStrategy()
{

}

void DuiPhysics2DIntegrationStrategy::integrate(qreal &pos,
        qreal &vel,
        qreal &pointerSpring,
        qreal &acc,
        qreal rangeStart,
        qreal rangeEnd,
        IntegrationData &data
                                               )
{
    qreal force;

    // Damping
    if (pos >= rangeStart && pos <= rangeEnd) {

        // Inside range
        if (data.pointer) {
            force = -data.frictionC * vel;
        } else {
            force = -data.slideFrictionC * vel;
        }
    } else {
        // Outside range (in border)
        force = -data.borderFrictionC * vel;
    }

    // Border springs
    if (pos < rangeStart) {
        force += data.borderSpringK * (rangeStart - pos);
        //Special case - when the border is crossed,
        //we don't want the view to "bounce" from the border.
        //We snap in this place to rangeStart value;
        if ((data.pointer == false) && (pos + vel + force >= rangeStart)) {
            vel = force = 0;
            pos = rangeStart;
        }
    }

    if (pos > rangeEnd) {
        force += -data.borderSpringK * (pos - rangeEnd);
        //Special case - when the border is crossed
        //we don't want the view to "bounce" from the border.
        //We snap in this place to rangeEnd value;
        if ((data.pointer == false) && (pos + vel + force <= rangeEnd)) {
            vel = force = 0;
            pos = rangeEnd;
        }
    }

    // Integration. Currently does not use time_delta or mass (assumed as 1.0)

    if (data.pointer) {

        // Damping of acceleration with pointer spring values.
        force += data.pointerSpringK * pointerSpring;
        // Increasing the speed by the last movement of the pointer
        acc = force - pointerSpring;
        vel += acc;

        if (abs(pointerSpring) > PositionNoiseDampingDelta) {
            pos           -= pointerSpring;
            pointerSpring = 0;
        }

    } else {

        acc = force;

        vel           += acc;
        pos           += vel;
        pointerSpring += vel;
    }
}

DuiPhysics2DPanningPrivate::DuiPhysics2DPanningPrivate(DuiPannableWidget *parentPannableWidget) :
    parentPannableWidget(parentPannableWidget),
    range(QRectF(0.0, 0.0, 0.0, 0.0)),
    posX(0.0),
    posY(0.0),
    velX(0.0),
    velY(0.0),
    pointerSpringX(0.0),
    pointerSpringY(0.0),
    sceneLastPos(QPointF()),
    timeLine(new QTimeLine()),
    currFrame(0),
    integrationStrategy(0)
{
    integrationData.pointerSpringK = 0.0;
    integrationData.frictionC = 0.0;
    integrationData.slideFrictionC = 0.0;
    integrationData.borderSpringK = 0.0;
    integrationData.borderFrictionC = 0.0;
    integrationData.pointer = false;
}


DuiPhysics2DPanningPrivate::~DuiPhysics2DPanningPrivate()
{
    delete timeLine;
}


DuiPhysics2DPanning::DuiPhysics2DPanning(DuiPannableWidget *parentPannableWidget)
    : QObject(),
      d_ptr(new DuiPhysics2DPanningPrivate(parentPannableWidget))
{
    Q_D(DuiPhysics2DPanning);
    d->integrationStrategy = new DuiPhysics2DIntegrationStrategy();
    connect(d->timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(integrator(int)));
}


DuiPhysics2DPanning::~DuiPhysics2DPanning()
{
    Q_D(DuiPhysics2DPanning);
    delete d->integrationStrategy;
    delete d_ptr;
}

void DuiPhysics2DPanning::setIntegrationStrategy(DuiPhysics2DIntegrationStrategy *strategy)
{
    if (!strategy) return;

    Q_D(DuiPhysics2DPanning);
    delete d->integrationStrategy;
    d->integrationStrategy = strategy;
}

DuiPhysics2DIntegrationStrategy *DuiPhysics2DPanning::integrationStrategy() const
{
    Q_D(const DuiPhysics2DPanning);
    return d->integrationStrategy;
}

void DuiPhysics2DPanning::start()
{
    Q_D(DuiPhysics2DPanning);
    if (!inMotion()) {
        d->velX = 0.0;
        d->velY = 0.0;

        d->timeLine->setDuration(PanningTimelineDuration);
        //d->timeLine->setUpdateInterval(PanningTimelineInterval);
        d->timeLine->setFrameRange(0, 29999);
        d->timeLine->setCurrentTime(0);
        d->timeLine->setCurveShape(QTimeLine::LinearCurve);
        d->currFrame = 0;
        d->timeLine->start();
    }
}


void DuiPhysics2DPanning::stop()
{
    Q_D(DuiPhysics2DPanning);
    d->timeLine->stop();
    emit panningStopped();
}


void DuiPhysics2DPanning::setRange(const QRectF &range)
{
    Q_D(DuiPhysics2DPanning);
    
    //update the range only if it has actually changed
    if( d->range != range ) {
        d->range = range;

        // Updates position after range setting (but this is not enough to
        // drag the position inside range in case range shrinks and puts
        // position outside it)
        emit updatePosition(QPointF(d->posX, d->posY));

        // Starts physics to return the position into range (border spring
        // activates). This does not cover the former case since physics
        // emits no updatePosition signals in case the position is still
        // inside range
        start();
    }
}


QRectF DuiPhysics2DPanning::range() const
{
    Q_D(const DuiPhysics2DPanning);
    return d->range;
}


void DuiPhysics2DPanning::setPosition(const QPointF &position)
{
    Q_D(DuiPhysics2DPanning);

    if (QPointF(d->posX, d->posY) != position) {
        d->posX = position.x();
        d->posY = position.y();

        emit updatePosition(position);

        // Starts the physics in case the position is set to border
        // and it needs to slide back into range
        start();
    }
}


QPointF DuiPhysics2DPanning::position() const
{
    Q_D(const DuiPhysics2DPanning);

    return QPointF(d->posX, d->posY);
}


QPointF DuiPhysics2DPanning::velocity() const
{
    Q_D(const DuiPhysics2DPanning);

    return QPointF(d->velX, d->velY);
}


bool DuiPhysics2DPanning::inMotion() const
{
    Q_D(const DuiPhysics2DPanning);

    return (d->timeLine->state() == QTimeLine::Running);
}


void DuiPhysics2DPanning::pointerPress(const QPointF &pos)
{
    Q_D(DuiPhysics2DPanning);

    // Enables the pointer spring, sets it to zero length and
    // starts the integrator

    d->integrationData.pointer = true;
    d->sceneLastPos = pos;

    d->pointerSpringX = 0.0;
    d->pointerSpringY = 0.0;
}


void DuiPhysics2DPanning::pointerMove(const QPointF &pos)
{
    Q_D(DuiPhysics2DPanning);

    // Tenses the pointer spring with the amount of movement of the pointer

    QPointF delta = pos - d->sceneLastPos;

    d->sceneLastPos = pos;

    d->pointerSpringX += delta.x();
    d->pointerSpringY += delta.y();
    start();
}


void DuiPhysics2DPanning::pointerRelease()
{
    Q_D(DuiPhysics2DPanning);

    // Disables the pointer spring

    d->integrationData.pointer = false;
}

void DuiPhysics2DPanning::setPointerSpringK(qreal value)
{
    d_ptr->integrationData.pointerSpringK = value;
}

void DuiPhysics2DPanning::setFriction(qreal value)
{
    d_ptr->integrationData.frictionC = value;
}

void DuiPhysics2DPanning::setSlidingFriction(qreal value)
{
    d_ptr->integrationData.slideFrictionC = value;
}

void DuiPhysics2DPanning::setBorderSpringK(qreal value)
{
    d_ptr->integrationData.borderSpringK = value;
}

void DuiPhysics2DPanning::setBorderFriction(qreal value)
{
    d_ptr->integrationData.borderFrictionC = value;
}

void DuiPhysics2DPanning::integrator(int frame)
{
    Q_D(DuiPhysics2DPanning);

    qreal accX, accY;
    qreal tempPosX;
    qreal tempPosY;
    int i = 0;

    tempPosX = d->posX;
    tempPosY = d->posY;

    printf("frame %d: %d\n", frame, d->timeLine->currentTime());
    while (frame > d->currFrame) {
        if (d->parentPannableWidget->panDirection().testFlag(Qt::Horizontal)) {
            d->integrationStrategy->integrate(d->posX,
                                              d->velX,
                                              d->pointerSpringX,
                                              accX,
                                              d->range.left(),
                                              d->range.right(),
                                              d->integrationData
                                             );
        } else {
            d->posX = 0.0;
            d->velX = 0.0;
            accX = 0.0;
        }

        if (d->parentPannableWidget->panDirection().testFlag(Qt::Vertical)) {
            d->integrationStrategy->integrate(d->posY,
                                              d->velY,
                                              d->pointerSpringY,
                                              accY,
                                              d->range.top(),
                                              d->range.bottom(),
                                              d->integrationData
                                             );

        } else {
            d->posY = 0.0;
            d->velY = 0.0;
            accY = 0.0;
        }

        // Checking if the viewport is currently dragged beyond it's borders and the integration should
        // continue even though the speed is low.
        bool inRangeX = (d->parentPannableWidget->panDirection().testFlag(Qt::Horizontal) == false) ||
                        (d->posX >= d->range.left() && d->posX <= d->range.right());

        bool inRangeY = (d->parentPannableWidget->panDirection().testFlag(Qt::Vertical) == false) ||
                        (d->posY >= d->range.top()  && d->posY <= d->range.bottom());

        // Integration stop condition.
        if (inRangeX && inRangeY &&
                qAbs(accX) < 1 &&
                qAbs(accY) < 1 &&
                qAbs(d->velX) < 1 &&
                qAbs(d->velY) < 1 &&
                !d->integrationData.pointer) {
            d->timeLine->stop();

            emit panningStopped();

            break;
        }

        d->currFrame++;
        i++;
    }

    if (tempPosX != d->posX || tempPosY != d->posY) {
        emit(updatePosition(QPointF(d->posX, d->posY)));
    }
}

