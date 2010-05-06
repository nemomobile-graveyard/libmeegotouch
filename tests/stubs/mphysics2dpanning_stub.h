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

#ifndef MPHYSICS2DPANNING_STUB
#define MPHYSICS2DPANNING_STUB

#include "mphysics2dpanning.h"
#include "mphysics2dpanning_p.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MPhysics2DPanningStub : public StubBase
{
public:
    virtual void MPhysics2DPanningConstructor(Qt::Orientations direction);
    virtual void MPhysics2DPanningDestructor();
    virtual void start();
    virtual void stop();
    virtual void setRange(QSizeF range);
    virtual QSizeF range() const;
    virtual void setPosition(QPointF position);
    virtual QPointF position() const;
    virtual void setTimeDelta(qreal time_delta);
    virtual qreal timeDelta() const;
    virtual bool inMotion();
    virtual void mousePress(QPointF pos);
    virtual void mouseMove(QPointF pos);
    virtual void mouseRelease();
    virtual void integrator(int frame);
};

// 2. IMPLEMENT STUB
void MPhysics2DPanningStub::MPhysics2DPanningConstructor(Qt::Orientations direction)
{
    Q_UNUSED(direction);
}

void MPhysics2DPanningStub::MPhysics2DPanningDestructor()
{
}

void MPhysics2DPanningStub::start()
{
    stubMethodEntered("start");
}

void MPhysics2DPanningStub::stop()
{
    stubMethodEntered("stop");
}

void MPhysics2DPanningStub::setRange(QSizeF range)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF >(range));
    stubMethodEntered("setRange", params);
}

QSizeF MPhysics2DPanningStub::range() const
{
    stubMethodEntered("range");
    return stubReturnValue<QSizeF>("range");
}

void MPhysics2DPanningStub::setPosition(QPointF position)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF >(position));
    stubMethodEntered("setPosition", params);
}

QPointF MPhysics2DPanningStub::position() const
{
    stubMethodEntered("position");
    return stubReturnValue<QPointF>("position");
}

void MPhysics2DPanningStub::setTimeDelta(qreal time_delta)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(time_delta));
    stubMethodEntered("setTimeDelta", params);
}

qreal MPhysics2DPanningStub::timeDelta() const
{
    stubMethodEntered("timeDelta");
    return stubReturnValue<qreal>("timeDelta");
}

bool MPhysics2DPanningStub::inMotion()
{
    stubMethodEntered("inMotion");
    return stubReturnValue<bool>("inMotion");
}

void MPhysics2DPanningStub::mousePress(QPointF pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF>(pos));
    stubMethodEntered("mousePress", params);
}

void MPhysics2DPanningStub::mouseMove(QPointF pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF>(pos));
    stubMethodEntered("mouseMove", params);
}

void MPhysics2DPanningStub::mouseRelease()
{
    stubMethodEntered("mouseRelease");
}

void MPhysics2DPanningStub::integrator(int frame)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int>(frame));
    stubMethodEntered("integrator", params);
}


// 3. CREATE A STUB INSTANCE
MPhysics2DPanningStub gDefaultMPhysics2DPanningStub;
MPhysics2DPanningStub *gMPhysics2DPanningStub = &gDefaultMPhysics2DPanningStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MPhysics2DPanning::MPhysics2DPanning(Qt::Orientations direction)
    : d_ptr(new MPhysics2DPanningPrivate(direction))
{
    gMPhysics2DPanningStub->MPhysics2DPanningConstructor(direction);
}

MPhysics2DPanning::~MPhysics2DPanning()
{
    gMPhysics2DPanningStub->MPhysics2DPanningDestructor();
}

void MPhysics2DPanning::start()
{
    gMPhysics2DPanningStub->start();
}

void MPhysics2DPanning::stop()
{
    gMPhysics2DPanningStub->stop();
}

void MPhysics2DPanning::setRange(QSizeF range)
{
    gMPhysics2DPanningStub->setRange(range);
}

QSizeF MPhysics2DPanning::range() const
{
    return gMPhysics2DPanningStub->range();
}

void MPhysics2DPanning::setPosition(QPointF position)
{
    gMPhysics2DPanningStub->setPosition(position);
}

QPointF MPhysics2DPanning::position() const
{
    return gMPhysics2DPanningStub->position();
}

void MPhysics2DPanning::setTimeDelta(qreal time_delta)
{
    gMPhysics2DPanningStub->setTimeDelta(time_delta);
}

qreal MPhysics2DPanning::timeDelta() const
{
    return gMPhysics2DPanningStub->timeDelta();
}

bool MPhysics2DPanning::inMotion() const
{
    return gMPhysics2DPanningStub->inMotion();
}

void MPhysics2DPanning::mousePress(QPointF pos)
{
    gMPhysics2DPanningStub->mousePress(pos);
}

void MPhysics2DPanning::mouseMove(QPointF pos)
{
    gMPhysics2DPanningStub->mouseMove(pos);
}

void MPhysics2DPanning::mouseRelease()
{
    gMPhysics2DPanningStub->mouseRelease();
}

void MPhysics2DPanning::integrator(int frame)
{
    gMPhysics2DPanningStub->integrator(frame);
}

#endif
