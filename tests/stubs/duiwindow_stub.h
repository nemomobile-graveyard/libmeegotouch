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

#ifndef DUIWINDOW_STUB
#define DUIWINDOW_STUB

#include "duiwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiWindowStub : public StubBase
{
public:
    virtual void DuiWindowConstructor(QWidget *parent);
    virtual void DuiWindowDestructor();
    virtual void setOrientationAnimationLength(DuiWindow::OrientationAnimationPhase phase, qreal lenghtInSecs);
    virtual bool orientationChanging();
    virtual qreal orientationChangePhase();
    virtual void registerForPermanentOrientationChangeAnimation(DuiWidget *widget);
    virtual void storeOrientationChangeParameters(DuiWidget *widget, DuiOrientationChangeParameters *parameters);
    virtual DuiOrientationChangeParameters *orientationChangeParameters(DuiWidget *widget);
    virtual void orientationAngleChanged(DuiDeviceProfile::DeviceOrientationAngle angle);
    virtual void orientationAnimationChanged(qreal value);
    virtual void orientationAnimationCompleted();
    virtual void prepareOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, int startAngle, int endAngle);
    virtual void executeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, qreal offset, int startAngle, int endAngle);
    virtual void finalizeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase);
    virtual bool event(QEvent *event);
};

// 2. IMPLEMENT STUB
void DuiWindowStub::DuiWindowConstructor(QWidget *parent)
{
    Q_UNUSED(parent);

}
void DuiWindowStub::DuiWindowDestructor()
{

}
void DuiWindowStub::setOrientationAnimationLength(DuiWindow::OrientationAnimationPhase phase, qreal lenghtInSecs)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<qreal >(lenghtInSecs));
    stubMethodEntered("setOrientationAnimationLength", params);
}

bool DuiWindowStub::orientationChanging()
{
    stubMethodEntered("orientationChanging");
    return stubReturnValue<bool>("orientationChanging");
}

qreal DuiWindowStub::orientationChangePhase()
{
    stubMethodEntered("orientationChangePhase");
    return stubReturnValue<qreal>("orientationChangePhase");
}

void DuiWindowStub::registerForPermanentOrientationChangeAnimation(DuiWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    stubMethodEntered("registerForPermanentOrientationChangeAnimation", params);
}

void DuiWindowStub::storeOrientationChangeParameters(DuiWidget *widget, DuiOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    params.append(new Parameter<DuiOrientationChangeParameters * >(parameters));
    stubMethodEntered("storeOrientationChangeParameters", params);
}

DuiOrientationChangeParameters *DuiWindowStub::orientationChangeParameters(DuiWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    stubMethodEntered("orientationChangeParameters", params);
    return stubReturnValue<DuiOrientationChangeParameters *>("orientationChangeParameters");
}

void DuiWindowStub::orientationAngleChanged(DuiDeviceProfile::DeviceOrientationAngle angle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiDeviceProfile::DeviceOrientationAngle >(angle));
    stubMethodEntered("orientationAngleChanged", params);
}

void DuiWindowStub::orientationAnimationChanged(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(value));
    stubMethodEntered("orientationAnimationChanged", params);
}

void DuiWindowStub::orientationAnimationCompleted()
{
    stubMethodEntered("orientationAnimationCompleted");
}

void DuiWindowStub::prepareOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, int startAngle, int endAngle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<int >(startAngle));
    params.append(new Parameter<int >(endAngle));
    stubMethodEntered("prepareOrientationAnimationPhase", params);
}

void DuiWindowStub::executeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, qreal offset, int startAngle, int endAngle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<qreal >(offset));
    params.append(new Parameter<int >(startAngle));
    params.append(new Parameter<int >(endAngle));
    stubMethodEntered("executeOrientationAnimationPhase", params);
}

void DuiWindowStub::finalizeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWindow::OrientationAnimationPhase >(phase));
    stubMethodEntered("finalizeOrientationAnimationPhase", params);
}

bool DuiWindowStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}



// 3. CREATE A STUB INSTANCE
DuiWindowStub gDefaultDuiWindowStub;
DuiWindowStub *gDuiWindowStub = &gDefaultDuiWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWindow::DuiWindow(QWidget *parent) : d_ptr(0)
{
    gDuiWindowStub->DuiWindowConstructor(parent);
}

DuiWindow::~DuiWindow()
{
    gDuiWindowStub->DuiWindowDestructor();
}

void DuiWindow::setOrientationAnimationLength(DuiWindow::OrientationAnimationPhase phase, qreal lenghtInSecs)
{
    gDuiWindowStub->setOrientationAnimationLength(phase, lenghtInSecs);
}

bool DuiWindow::orientationChanging()
{
    return gDuiWindowStub->orientationChanging();
}

qreal DuiWindow::orientationChangePhase()
{
    return gDuiWindowStub->orientationChangePhase();
}

void DuiWindow::registerForPermanentOrientationChangeAnimation(DuiWidget *widget)
{
    gDuiWindowStub->registerForPermanentOrientationChangeAnimation(widget);
}

void DuiWindow::storeOrientationChangeParameters(DuiWidget *widget, DuiOrientationChangeParameters *parameters)
{
    gDuiWindowStub->storeOrientationChangeParameters(widget, parameters);
}

DuiOrientationChangeParameters *DuiWindow::orientationChangeParameters(DuiWidget *widget)
{
    return gDuiWindowStub->orientationChangeParameters(widget);
}

void DuiWindow::orientationAngleChanged(DuiDeviceProfile::DeviceOrientationAngle angle)
{
    gDuiWindowStub->orientationAngleChanged(angle);
}

void DuiWindow::orientationAnimationChanged(qreal value)
{
    gDuiWindowStub->orientationAnimationChanged(value);
}

void DuiWindow::orientationAnimationCompleted()
{
    gDuiWindowStub->orientationAnimationCompleted();
}

void DuiWindow::prepareOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, int startAngle, int endAngle)
{
    gDuiWindowStub->prepareOrientationAnimationPhase(phase, startAngle, endAngle);
}

void DuiWindow::executeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, qreal offset, int startAngle, int endAngle)
{
    gDuiWindowStub->executeOrientationAnimationPhase(phase, offset, startAngle, endAngle);
}

void DuiWindow::finalizeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase)
{
    gDuiWindowStub->finalizeOrientationAnimationPhase(phase);
}

bool DuiWindow::event(QEvent *event)
{
    return gDuiWindowStub->event(event);
}


#endif
