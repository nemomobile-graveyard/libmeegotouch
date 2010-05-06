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

#ifndef MWINDOW_STUB
#define MWINDOW_STUB

#include "mwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MWindowStub : public StubBase
{
public:
    virtual void MWindowConstructor(QWidget *parent);
    virtual void MWindowDestructor();
    virtual void setOrientationAnimationLength(MWindow::OrientationAnimationPhase phase, qreal lenghtInSecs);
    virtual bool orientationChanging();
    virtual qreal orientationChangePhase();
    virtual void registerForPermanentOrientationChangeAnimation(MWidget *widget);
    virtual void storeOrientationChangeParameters(MWidget *widget, MOrientationChangeParameters *parameters);
    virtual MOrientationChangeParameters *orientationChangeParameters(MWidget *widget);
    virtual void orientationAngleChanged(MDeviceProfile::DeviceOrientationAngle angle);
    virtual void orientationAnimationChanged(qreal value);
    virtual void orientationAnimationCompleted();
    virtual void prepareOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase, int startAngle, int endAngle);
    virtual void executeOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase, qreal offset, int startAngle, int endAngle);
    virtual void finalizeOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase);
    virtual bool event(QEvent *event);
};

// 2. IMPLEMENT STUB
void MWindowStub::MWindowConstructor(QWidget *parent)
{
    Q_UNUSED(parent);

}
void MWindowStub::MWindowDestructor()
{

}
void MWindowStub::setOrientationAnimationLength(MWindow::OrientationAnimationPhase phase, qreal lenghtInSecs)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<qreal >(lenghtInSecs));
    stubMethodEntered("setOrientationAnimationLength", params);
}

bool MWindowStub::orientationChanging()
{
    stubMethodEntered("orientationChanging");
    return stubReturnValue<bool>("orientationChanging");
}

qreal MWindowStub::orientationChangePhase()
{
    stubMethodEntered("orientationChangePhase");
    return stubReturnValue<qreal>("orientationChangePhase");
}

void MWindowStub::registerForPermanentOrientationChangeAnimation(MWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    stubMethodEntered("registerForPermanentOrientationChangeAnimation", params);
}

void MWindowStub::storeOrientationChangeParameters(MWidget *widget, MOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    params.append(new Parameter<MOrientationChangeParameters * >(parameters));
    stubMethodEntered("storeOrientationChangeParameters", params);
}

MOrientationChangeParameters *MWindowStub::orientationChangeParameters(MWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    stubMethodEntered("orientationChangeParameters", params);
    return stubReturnValue<MOrientationChangeParameters *>("orientationChangeParameters");
}

void MWindowStub::orientationAngleChanged(MDeviceProfile::DeviceOrientationAngle angle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MDeviceProfile::DeviceOrientationAngle >(angle));
    stubMethodEntered("orientationAngleChanged", params);
}

void MWindowStub::orientationAnimationChanged(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(value));
    stubMethodEntered("orientationAnimationChanged", params);
}

void MWindowStub::orientationAnimationCompleted()
{
    stubMethodEntered("orientationAnimationCompleted");
}

void MWindowStub::prepareOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase, int startAngle, int endAngle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<int >(startAngle));
    params.append(new Parameter<int >(endAngle));
    stubMethodEntered("prepareOrientationAnimationPhase", params);
}

void MWindowStub::executeOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase, qreal offset, int startAngle, int endAngle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<qreal >(offset));
    params.append(new Parameter<int >(startAngle));
    params.append(new Parameter<int >(endAngle));
    stubMethodEntered("executeOrientationAnimationPhase", params);
}

void MWindowStub::finalizeOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWindow::OrientationAnimationPhase >(phase));
    stubMethodEntered("finalizeOrientationAnimationPhase", params);
}

bool MWindowStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}



// 3. CREATE A STUB INSTANCE
MWindowStub gDefaultMWindowStub;
MWindowStub *gMWindowStub = &gDefaultMWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWindow::MWindow(QWidget *parent) : d_ptr(0)
{
    gMWindowStub->MWindowConstructor(parent);
}

MWindow::~MWindow()
{
    gMWindowStub->MWindowDestructor();
}

void MWindow::setOrientationAnimationLength(MWindow::OrientationAnimationPhase phase, qreal lenghtInSecs)
{
    gMWindowStub->setOrientationAnimationLength(phase, lenghtInSecs);
}

bool MWindow::orientationChanging()
{
    return gMWindowStub->orientationChanging();
}

qreal MWindow::orientationChangePhase()
{
    return gMWindowStub->orientationChangePhase();
}

void MWindow::registerForPermanentOrientationChangeAnimation(MWidget *widget)
{
    gMWindowStub->registerForPermanentOrientationChangeAnimation(widget);
}

void MWindow::storeOrientationChangeParameters(MWidget *widget, MOrientationChangeParameters *parameters)
{
    gMWindowStub->storeOrientationChangeParameters(widget, parameters);
}

MOrientationChangeParameters *MWindow::orientationChangeParameters(MWidget *widget)
{
    return gMWindowStub->orientationChangeParameters(widget);
}

void MWindow::orientationAngleChanged(MDeviceProfile::DeviceOrientationAngle angle)
{
    gMWindowStub->orientationAngleChanged(angle);
}

void MWindow::orientationAnimationChanged(qreal value)
{
    gMWindowStub->orientationAnimationChanged(value);
}

void MWindow::orientationAnimationCompleted()
{
    gMWindowStub->orientationAnimationCompleted();
}

void MWindow::prepareOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase, int startAngle, int endAngle)
{
    gMWindowStub->prepareOrientationAnimationPhase(phase, startAngle, endAngle);
}

void MWindow::executeOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase, qreal offset, int startAngle, int endAngle)
{
    gMWindowStub->executeOrientationAnimationPhase(phase, offset, startAngle, endAngle);
}

void MWindow::finalizeOrientationAnimationPhase(MWindow::OrientationAnimationPhase phase)
{
    gMWindowStub->finalizeOrientationAnimationPhase(phase);
}

bool MWindow::event(QEvent *event)
{
    return gMWindowStub->event(event);
}


#endif
