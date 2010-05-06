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

#include <QTimeLine>

#include "mpannablewidget.h"
#include "mphysics2dpanning.h"
#include "mphysics2dpanning_p.h"

static const int PanningTimelineDuration      = 1000000; /* in ms */
static const int PanningTimelineInterval      =      20; /* in ms */
static const int PositionNoiseDampingDelta    =       2; /* in px */

MPhysics2DPanningPrivate::MPhysics2DPanningPrivate(MPhysics2DPanning *publicObject) :
    enabled(true),
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
    pointerPressed(false),
    pointerSpringK(0.0),
    frictionC(0.0),
    slideFrictionC(0.0),
    borderSpringK(0.0),
    borderFrictionC(0.0),
    panDirection(0),
    q_ptr(publicObject)
{
}


MPhysics2DPanningPrivate::~MPhysics2DPanningPrivate()
{
    delete timeLine;
}

void MPhysics2DPanningPrivate::_q_integrator(int frame)
{
    Q_Q(MPhysics2DPanning);

    qreal accX, accY;
    qreal tempPosX;
    qreal tempPosY;
    int i = 0;

    tempPosX = posX;
    tempPosY = posY;

    while (frame > currFrame) {
        if (panDirection.testFlag(Qt::Horizontal)) {
            q->integrateAxis(Qt::Horizontal,
                             posX,
                             velX,
                             accX,
                             pointerSpringX,
                             pointerPressed
                             );
        } else {
            posX = 0.0;
            velX = 0.0;
            accX = 0.0;
        }

        if (panDirection.testFlag(Qt::Vertical)) {
            q->integrateAxis(Qt::Vertical,
                             posY,
                             velY,
                             accY,
                             pointerSpringY,
                             pointerPressed
                             );

        } else {
            posY = 0.0;
            velY = 0.0;
            accY = 0.0;
        }

        // Checking if the viewport is currently dragged beyond it's borders and the integration should
        // continue even though the speed is low.
        bool inRangeX = (panDirection.testFlag(Qt::Horizontal) == false) ||
                        (posX >= range.left() && posX <= range.right());

        bool inRangeY = (panDirection.testFlag(Qt::Vertical) == false) ||
                        (posY >= range.top()  && posY <= range.bottom());

        // Integration stop condition.
        if (inRangeX && inRangeY &&
                qAbs(accX) < 1 &&
                qAbs(accY) < 1 &&
                qAbs(velX) < 1 &&
                qAbs(velY) < 1 &&
                !pointerPressed) {
            timeLine->stop();

            emit q->panningStopped();

            break;
        }

        currFrame++;
        i++;
    }

    if (tempPosX != posX || tempPosY != posY) {
        emit(q->positionChanged(QPointF(posX, posY)));
    }
}

MPhysics2DPanning::MPhysics2DPanning(QObject *parent)
    : QObject(parent),
      d_ptr(new MPhysics2DPanningPrivate(this))
{
    Q_D(MPhysics2DPanning);
    connect(d->timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(_q_integrator(int)));
}


MPhysics2DPanning::~MPhysics2DPanning()
{
    delete d_ptr;
}

bool MPhysics2DPanning::enabled() const
{
    return d_ptr->enabled;
}

void MPhysics2DPanning::setEnabled(bool enabled)
{
    Q_D(MPhysics2DPanning);

    d->enabled = enabled;
    if (d->enabled == false) stop();
}

Qt::Orientations MPhysics2DPanning::panDirection() const
{
    return d_ptr->panDirection;
}

void MPhysics2DPanning::setPanDirection(Qt::Orientations direction)
{
    d_ptr->panDirection = direction;
}

qreal MPhysics2DPanning::pointerSpringK() const
{
    return d_ptr->pointerSpringK;
}

void MPhysics2DPanning::setPointerSpringK(qreal value)
{
    d_ptr->pointerSpringK = value;
}

qreal MPhysics2DPanning::friction() const
{
    return d_ptr->frictionC;
}

void MPhysics2DPanning::setFriction(qreal value)
{
    d_ptr->frictionC = value;
}

qreal MPhysics2DPanning::slidingFriction() const
{
    return d_ptr->slideFrictionC;
}

void MPhysics2DPanning::setSlidingFriction(qreal value)
{
    d_ptr->slideFrictionC = value;
}

qreal MPhysics2DPanning::borderSpringK() const
{
    return d_ptr->borderSpringK;
}

void MPhysics2DPanning::setBorderSpringK(qreal value)
{
    d_ptr->borderSpringK = value;
}

qreal MPhysics2DPanning::borderFriction() const
{
    return d_ptr->borderFrictionC;
}

void MPhysics2DPanning::setBorderFriction(qreal value)
{
    d_ptr->borderFrictionC = value;
}

void MPhysics2DPanning::start()
{
    Q_D(MPhysics2DPanning);
    if (!inMotion()) {
        d->velX = 0.0;
        d->velY = 0.0;

        d->timeLine->setDuration(PanningTimelineDuration);
        d->timeLine->setUpdateInterval(PanningTimelineInterval);
        d->timeLine->setFrameRange(0, 29999);
        d->timeLine->setCurrentTime(0);
        d->timeLine->setCurveShape(QTimeLine::LinearCurve);
        d->currFrame = 0;
        d->timeLine->start();
    }
}


void MPhysics2DPanning::stop()
{
    Q_D(MPhysics2DPanning);

    // Checking if the viewport is currently dragged beyond it's borders and the integration should
    // continue.
    bool inRangeX = (d->panDirection.testFlag(Qt::Horizontal) == false) ||
                    (d->posX >= d->range.left() && d->posX <= d->range.right());

    bool inRangeY = (d->panDirection.testFlag(Qt::Vertical) == false) ||
                    (d->posY >= d->range.top()  && d->posY <= d->range.bottom());

    if (inRangeX && inRangeY) {
        d->timeLine->stop();
        emit panningStopped();
    }
}


void MPhysics2DPanning::setRange(const QRectF &range)
{
    Q_D(MPhysics2DPanning);
    
    //update the range only if it has actually changed
    if( d->range != range ) {
        d->range = range;

        // Updates position after range setting (but this is not enough to
        // drag the position inside range in case range shrinks and puts
        // position outside it)
        emit positionChanged(QPointF(d->posX, d->posY));

        // Starts physics to return the position into range (border spring
        // activates). This does not cover the former case since physics
        // emits no positionChanged signals in case the position is still
        // inside range
        start();
    }
}


QRectF MPhysics2DPanning::range() const
{
    Q_D(const MPhysics2DPanning);
    return d->range;
}


void MPhysics2DPanning::setPosition(const QPointF &position)
{
    Q_D(MPhysics2DPanning);

    if (QPointF(d->posX, d->posY) != position) {
        d->posX = position.x();
        d->posY = position.y();

        emit positionChanged(position);

        // Starts the physics in case the position is set to border
        // and it needs to slide back into range
        start();
    }
}


QPointF MPhysics2DPanning::position() const
{
    Q_D(const MPhysics2DPanning);

    return QPointF(d->posX, d->posY);
}


QPointF MPhysics2DPanning::velocity() const
{
    Q_D(const MPhysics2DPanning);

    return QPointF(d->velX, d->velY);
}


bool MPhysics2DPanning::inMotion() const
{
    Q_D(const MPhysics2DPanning);

    return (d->timeLine->state() == QTimeLine::Running);
}


void MPhysics2DPanning::pointerPress(const QPointF &pos)
{
    Q_D(MPhysics2DPanning);

    // Enables the pointer spring, sets it to zero length and
    // starts the integrator

    d->pointerPressed = true;
    d->sceneLastPos = pos;

    d->pointerSpringX = 0.0;
    d->pointerSpringY = 0.0;
}


void MPhysics2DPanning::pointerMove(const QPointF &pos)
{
    Q_D(MPhysics2DPanning);

    // Tenses the pointer spring with the amount of movement of the pointer
    QPointF delta = pos - d->sceneLastPos;

    d->sceneLastPos = pos;

    if (d->enabled) {
        d->pointerSpringX += delta.x();
        d->pointerSpringY += delta.y();
        start();
    } else {

        bool emitSignal = false;

        if (d->panDirection.testFlag(Qt::Horizontal)) {

            qreal prevPosX = d->posX;
            d->posX -= delta.x();

            if (d->posX < d->range.left()) d->posX = d->range.left();
            if (d->posX > d->range.right()) d->posX = d->range.right();

            if (prevPosX != d->posX) emitSignal = true;
        }

        if (d->panDirection.testFlag(Qt::Vertical)) {

            qreal prevPosY = d->posY;
            d->posY -= delta.y();

            if (d->posY < d->range.top()) d->posY = d->range.top();
            if (d->posY > d->range.bottom()) d->posY = d->range.bottom();

            if (prevPosY != d->posY) emitSignal = true;
        }

        if (emitSignal) emit positionChanged(QPointF(d->posX, d->posY));
    }

}


void MPhysics2DPanning::pointerRelease()
{
    Q_D(MPhysics2DPanning);

    // Disables the pointer spring
    d->pointerPressed = false;

    if (d->enabled == false) {
        //The physics engine is not working, emitting panning stopped now.
        emit panningStopped();
    }
}


void MPhysics2DPanning::integrateAxis(Qt::Orientation orientation,
                                        qreal &position,
                                        qreal &velocity,
                                        qreal &acceleration,
                                        qreal &pointerDifference,
                                        bool pointerPressed
                                        )
{
    Q_D(MPhysics2DPanning);

    qreal force;
    qreal rangeStart = (orientation == Qt::Vertical ? d->range.top() : d->range.left());
    qreal rangeEnd =   (orientation == Qt::Vertical ? d->range.bottom() : d->range.right());

    // Damping
    if (position >= rangeStart && position <= rangeEnd) {

        // Inside range
        if (d->pointerPressed) {
            force = -d->frictionC * velocity;
        } else {
            force = -d->slideFrictionC * velocity;
        }
    } else {
        // Outside range (in border)
        force = -d->borderFrictionC * velocity;
    }

    // Border springs
    if (position < rangeStart) {
        force += d->borderSpringK * (rangeStart - position);
        //Special case - when the border is crossed,
        //we don't want the view to "bounce" from the border.
        //We snap in this place to rangeStart value;
        if ((pointerPressed == false) && (position + velocity + force >= rangeStart)) {
            velocity = force = 0;
            position = rangeStart;
        }
    }

    if (position > rangeEnd) {
        force += -d->borderSpringK * (position - rangeEnd);
        //Special case - when the border is crossed
        //we don't want the view to "bounce" from the border.
        //We snap in this place to rangeEnd value;
        if ((pointerPressed == false) && (position + velocity + force <= rangeEnd)) {
            velocity = force = 0;
            position = rangeEnd;
        }
    }

    // Integration. Currently does not use time_delta or mass (assumed as 1.0)

    if (pointerPressed) {

        // Damping of acceleration with pointer spring values.
        force += d->pointerSpringK * pointerDifference;
        // Increasing the speed by the last movement of the pointer
        acceleration = force - pointerDifference;
        velocity += acceleration;

        if (abs(pointerDifference) > PositionNoiseDampingDelta) {
            position           -= pointerDifference;
            pointerDifference = 0;
        }

    } else {

        acceleration = force;

        velocity                 += acceleration;
        position                 += velocity;
        pointerDifference   += velocity;
    }
}

#include "moc_mphysics2dpanning.cpp"
