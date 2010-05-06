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

#ifndef MDEVICEPROFILE_STUB
#define MDEVICEPROFILE_STUB

#include "mdeviceprofile.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MDeviceProfileStub : public StubBase
{
public:
    virtual MDeviceProfile *instance();
    virtual QString name() const;
    virtual QSize resolution() const;
    virtual QRectF screenRectF() const;
    virtual int width() const;
    virtual int height() const;
    virtual QSize pixelsPerInch() const;
    virtual M::Orientation orientation() const;
    virtual void setOrientationAngle(MDeviceProfile::DeviceOrientationAngle angle);
    virtual void MDeviceProfileDestructor();
    virtual void orientationChanged();
    MDeviceProfilePrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
MDeviceProfile *MDeviceProfileStub::instance()
{
    stubMethodEntered("instance");
    return stubReturnValue<MDeviceProfile *>("instance");
}

QString MDeviceProfileStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

QSize MDeviceProfileStub::resolution() const
{
    stubMethodEntered("resolution");
    return stubReturnValue<QSize>("resolution");
}

QRectF MDeviceProfileStub::screenRectF() const
{
    stubMethodEntered("screenRectF");
    return stubReturnValue<QRectF>("screenRectF");
}

int MDeviceProfileStub::width() const
{
    stubMethodEntered("width");
    return stubReturnValue<int>("width");
}

int MDeviceProfileStub::height() const
{
    stubMethodEntered("height");
    return stubReturnValue<int>("height");
}

QSize MDeviceProfileStub::pixelsPerInch() const
{
    stubMethodEntered("pixelsPerInch");
    return stubReturnValue<QSize>("pixelsPerInch");
}

M::Orientation MDeviceProfileStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<M::Orientation>("orientation");
}

void MDeviceProfileStub::orientationChanged()
{
}


void MDeviceProfileStub::setOrientationAngle(MDeviceProfile::DeviceOrientationAngle angle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MDeviceProfile::DeviceOrientationAngle >(angle));
    stubMethodEntered("setOrientationAngle", params);
}

void MDeviceProfileStub::MDeviceProfileDestructor()
{

}


// 3. CREATE A STUB INSTANCE
MDeviceProfileStub gDefaultMDeviceProfileStub;
MDeviceProfileStub *gMDeviceProfileStub = &gDefaultMDeviceProfileStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MDeviceProfile *MDeviceProfile::instance()
{
    return gMDeviceProfileStub->instance();
}

QString MDeviceProfile::name() const
{
    return gMDeviceProfileStub->name();
}

QSize MDeviceProfile::resolution() const
{
    return gMDeviceProfileStub->resolution();
}

QRectF MDeviceProfile::screenRectF() const
{
    return gMDeviceProfileStub->screenRectF();
}

int MDeviceProfile::width() const
{
    return gMDeviceProfileStub->width();
}

int MDeviceProfile::height() const
{
    return gMDeviceProfileStub->height();
}

QSize MDeviceProfile::pixelsPerInch() const
{
    return gMDeviceProfileStub->pixelsPerInch();
}

M::Orientation MDeviceProfile::orientation() const
{
    return gMDeviceProfileStub->orientation();
}

void MDeviceProfile::setOrientationAngle(DeviceOrientationAngle angle)
{
    gMDeviceProfileStub->setOrientationAngle(angle);
}

MDeviceProfile::~MDeviceProfile()
{
    gMDeviceProfileStub->MDeviceProfileDestructor();
}

void MDeviceProfile::orientationChanged()
{
    gMDeviceProfileStub->orientationChanged();
}

#endif
