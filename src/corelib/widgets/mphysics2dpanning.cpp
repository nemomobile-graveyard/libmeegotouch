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

namespace {
    const qreal MinimumBorderSpeed = 0.1; //px. Used when physics parameters do not allow to drag the viewport to correct range.
    const int SampleCount = 5; // Used for speed calculation;
}

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
    maxVel(0.0),
    panningAnimation(new PanningAnimation),
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
    delete panningAnimation;
}

void MPhysics2DPanningPrivate::_q_integrator(const QVariant &value)
{
    Q_Q(MPhysics2DPanning);
    Q_UNUSED(value);

    qreal accX, accY;
    qreal tempPosX;
    qreal tempPosY;

    tempPosX = posX;
    tempPosY = posY;

    if (panDirection.testFlag(Qt::Horizontal)) {
        q->integrateAxis(Qt::Horizontal,
                         posX,
                         velX,
                         accX,
                         pointerSpringX,
                         pointerPressed
                         );
    } else {
        posX = 0.0f;
        velX = 0.0f;
        accX = 0.0f;
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
        posY = 0.0f;
        velY = 0.0f;
        accY = 0.0f;
    }

    // Integration stop condition.
    if (qAbs(accX) < 1 &&
        qAbs(accY) < 1 &&
        qAbs(velX) < MinimumBorderSpeed &&
        qAbs(velY) < MinimumBorderSpeed &&
        !pointerPressed) {

        bool inRangeX = (panDirection.testFlag(Qt::Horizontal) == false) ||
                        (posX >= range.left() && posX <= range.right());

        bool inRangeY = (panDirection.testFlag(Qt::Vertical) == false) ||
                        (posY >= range.top()  && posY <= range.bottom());

        if (!inRangeX)
            velX = ( posX < range.left() ? 1 : -1) * MinimumBorderSpeed;
        if (!inRangeY)
            velY = ( posY < range.top() ? 1 : -1) * MinimumBorderSpeed;

        if (inRangeX && inRangeY) {
            panningAnimation->stop();
            emit q->panningStopped();
        }
    }

    if (tempPosX != posX || tempPosY != posY) {
        emit(q->positionChanged(QPointF(posX, posY)));
    }
}


QPointF MPhysics2DPanningPrivate::getVelocity()
{
    QPointF sum;
    int validSampleCount = qMin(SampleCount, positions.count() - 1);
    for(int i=0; i<validSampleCount; i++) {
            sum += positions[i] - positions[i + 1];
    }
    return - sum / validSampleCount;
}


MPhysics2DPanning::MPhysics2DPanning(QObject *parent)
    : QObject(parent),
      d_ptr(new MPhysics2DPanningPrivate(this))
{
    Q_D(MPhysics2DPanning);
    connect(d->panningAnimation, SIGNAL(valueChanged(QVariant)), SLOT(_q_integrator(QVariant)));
}


MPhysics2DPanning::~MPhysics2DPanning()
{
    delete d_ptr;
}

qreal MPhysics2DPanning::maximumVelocity() const
{
    return d_ptr->maxVel;
}

void MPhysics2DPanning::setMaximumVelocity(qreal velocity)
{
    d_ptr->maxVel = velocity;
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
        d->velX = 0.0f;
        d->velY = 0.0f;

        // Duration does not matter as we loop until the physics termination condition is hit
        d->panningAnimation->setDuration(1000000);
        d->panningAnimation->setLoopCount(-1);

        d->panningAnimation->setStartValue(0.0f);
        d->panningAnimation->setEndValue(1.0f);

        d->panningAnimation->start();
    }
}

void MPhysics2DPanning::stop()
{
    Q_D(MPhysics2DPanning);

    // Checking if the viewport is currently dragged beyond its borders and the integration should
    // continue.
    bool inRangeX = (d->panDirection.testFlag(Qt::Horizontal) == false) ||
                    (d->posX >= d->range.left() && d->posX <= d->range.right());

    bool inRangeY = (d->panDirection.testFlag(Qt::Vertical) == false) ||
                    (d->posY >= d->range.top()  && d->posY <= d->range.bottom());

    if (inRangeX && inRangeY) {
        d->panningAnimation->stop();
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

        if (d->panDirection.testFlag(Qt::Horizontal))
            d->posX = position.x();
        if (d->panDirection.testFlag(Qt::Vertical))
            d->posY = position.y();

        emit positionChanged(position);

        if (inMotion()) {
            // If we are in the middle of the movement, we should reduce the speed
            // but not stop because we might be outside bounds.
            d->velX = 0.0;
            d->velY = 0.0;
        } else {
            // Starts the physics in case the position is set to border
            // and it needs to slide back into range
            start();
        }
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

    return (d->panningAnimation->state() == QAbstractAnimation::Running);
}


void MPhysics2DPanning::pointerPress(const QPointF &pos)
{
    Q_D(MPhysics2DPanning);

    // Enables the pointer spring, sets it to zero length and
    // starts the integrator

    d->pointerPressed = true;
    d->sceneLastPos = pos;

    d->pointerSpringX = 0.0f;
    d->pointerSpringY = 0.0f;

    d->positions.clear();
    d->positions.push_front(pos);
}


void MPhysics2DPanning::pointerMove(const QPointF &pos)
{
    Q_D(MPhysics2DPanning);

    // Tenses the pointer spring with the amount of movement of the pointer
    QPointF delta = pos - d->sceneLastPos;

    d->sceneLastPos = pos;
    d->positions.push_front(pos);

    if (d->enabled) {
        // We are going to follow the finger starting from the
        // second move request to avoid jumping of the viewport.
        // In that way, the viewport will start moving more smoothly.
        if (d->positions.count() > 2) {
            d->pointerSpringX += delta.x();
            d->pointerSpringY += delta.y();
        }
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

    // Resolve new velocity only if we have enough samples
    if(d->positions.count() > 1) {
        // Get the current velocity
        QPointF v = d->getVelocity();
        // Limit the velocity
        d->velX = qBound(-d->maxVel, v.x(), d->maxVel);
        d->velY = qBound(-d->maxVel, v.y(), d->maxVel);
        d->positions.clear();
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

    qreal rangeStart = (orientation == Qt::Vertical ? d->range.top() : d->range.left());
    qreal rangeEnd =   (orientation == Qt::Vertical ? d->range.bottom() : d->range.right());

    acceleration = 0;

    if (pointerPressed) {

        if (position < rangeStart || position > rangeEnd)
            pointerDifference /= 2;

        position          -= pointerDifference;
        pointerDifference = velocity = 0;

    } else {

        // Acceleration applies only when overshooting
        qreal a = 0;
        qreal f = d->slideFrictionC;
        if(position < rangeStart) {
            // Overshoot at the top
            a = (rangeStart - position) * d->borderSpringK;
            if (velocity > 0) f = d->borderFrictionC;

            if (position + (velocity + a)*f >= rangeStart) {
                velocity = a = f = 0;
                position = rangeStart;
            }
        } else if(position > rangeEnd) {
            // Overshoot at the bottom
            a = (rangeEnd - position) * d->borderSpringK;
            if(velocity < 0) f = d->borderFrictionC;

            if (position + (velocity + a)*f <= rangeEnd) {
                velocity = a = f = 0;
                position = rangeEnd;
            }
        }
        velocity += a;
        velocity = velocity * f;
        // Move content
        position += velocity;
        pointerDifference = 0;
    }

}

#include "moc_mphysics2dpanning.cpp"
