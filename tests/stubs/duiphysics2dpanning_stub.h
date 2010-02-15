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

#ifndef DUIPHYSICS2DPANNING_STUB
#define DUIPHYSICS2DPANNING_STUB

#include "duiphysics2dpanning.h"
#include "duiphysics2dpanning_p.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiPhysics2DPanningStub : public StubBase
{
public:
    virtual void DuiPhysics2DPanningConstructor(Qt::Orientations direction);
    virtual void DuiPhysics2DPanningDestructor();
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
void DuiPhysics2DPanningStub::DuiPhysics2DPanningConstructor(Qt::Orientations direction)
{
    Q_UNUSED(direction);
}

void DuiPhysics2DPanningStub::DuiPhysics2DPanningDestructor()
{
}

void DuiPhysics2DPanningStub::start()
{
    stubMethodEntered("start");
}

void DuiPhysics2DPanningStub::stop()
{
    stubMethodEntered("stop");
}

void DuiPhysics2DPanningStub::setRange(QSizeF range)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF >(range));
    stubMethodEntered("setRange", params);
}

QSizeF DuiPhysics2DPanningStub::range() const
{
    stubMethodEntered("range");
    return stubReturnValue<QSizeF>("range");
}

void DuiPhysics2DPanningStub::setPosition(QPointF position)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF >(position));
    stubMethodEntered("setPosition", params);
}

QPointF DuiPhysics2DPanningStub::position() const
{
    stubMethodEntered("position");
    return stubReturnValue<QPointF>("position");
}

void DuiPhysics2DPanningStub::setTimeDelta(qreal time_delta)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(time_delta));
    stubMethodEntered("setTimeDelta", params);
}

qreal DuiPhysics2DPanningStub::timeDelta() const
{
    stubMethodEntered("timeDelta");
    return stubReturnValue<qreal>("timeDelta");
}

bool DuiPhysics2DPanningStub::inMotion()
{
    stubMethodEntered("inMotion");
    return stubReturnValue<bool>("inMotion");
}

void DuiPhysics2DPanningStub::mousePress(QPointF pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF>(pos));
    stubMethodEntered("mousePress", params);
}

void DuiPhysics2DPanningStub::mouseMove(QPointF pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF>(pos));
    stubMethodEntered("mouseMove", params);
}

void DuiPhysics2DPanningStub::mouseRelease()
{
    stubMethodEntered("mouseRelease");
}

void DuiPhysics2DPanningStub::integrator(int frame)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int>(frame));
    stubMethodEntered("integrator", params);
}


// 3. CREATE A STUB INSTANCE
DuiPhysics2DPanningStub gDefaultDuiPhysics2DPanningStub;
DuiPhysics2DPanningStub *gDuiPhysics2DPanningStub = &gDefaultDuiPhysics2DPanningStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiPhysics2DPanning::DuiPhysics2DPanning(Qt::Orientations direction)
    : d_ptr(new DuiPhysics2DPanningPrivate(direction))
{
    gDuiPhysics2DPanningStub->DuiPhysics2DPanningConstructor(direction);
}

DuiPhysics2DPanning::~DuiPhysics2DPanning()
{
    gDuiPhysics2DPanningStub->DuiPhysics2DPanningDestructor();
}

void DuiPhysics2DPanning::start()
{
    gDuiPhysics2DPanningStub->start();
}

void DuiPhysics2DPanning::stop()
{
    gDuiPhysics2DPanningStub->stop();
}

void DuiPhysics2DPanning::setRange(QSizeF range)
{
    gDuiPhysics2DPanningStub->setRange(range);
}

QSizeF DuiPhysics2DPanning::range() const
{
    return gDuiPhysics2DPanningStub->range();
}

void DuiPhysics2DPanning::setPosition(QPointF position)
{
    gDuiPhysics2DPanningStub->setPosition(position);
}

QPointF DuiPhysics2DPanning::position() const
{
    return gDuiPhysics2DPanningStub->position();
}

void DuiPhysics2DPanning::setTimeDelta(qreal time_delta)
{
    gDuiPhysics2DPanningStub->setTimeDelta(time_delta);
}

qreal DuiPhysics2DPanning::timeDelta() const
{
    return gDuiPhysics2DPanningStub->timeDelta();
}

bool DuiPhysics2DPanning::inMotion() const
{
    return gDuiPhysics2DPanningStub->inMotion();
}

void DuiPhysics2DPanning::mousePress(QPointF pos)
{
    gDuiPhysics2DPanningStub->mousePress(pos);
}

void DuiPhysics2DPanning::mouseMove(QPointF pos)
{
    gDuiPhysics2DPanningStub->mouseMove(pos);
}

void DuiPhysics2DPanning::mouseRelease()
{
    gDuiPhysics2DPanningStub->mouseRelease();
}

void DuiPhysics2DPanning::integrator(int frame)
{
    gDuiPhysics2DPanningStub->integrator(frame);
}

#endif
