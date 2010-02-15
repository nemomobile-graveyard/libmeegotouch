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

#ifndef DUIDEVICEPROFILE_STUB
#define DUIDEVICEPROFILE_STUB

#include "duideviceprofile.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiDeviceProfileStub : public StubBase
{
public:
    virtual DuiDeviceProfile *instance();
    virtual QString name() const;
    virtual QSize resolution() const;
    virtual QRectF screenRectF() const;
    virtual int width() const;
    virtual int height() const;
    virtual QSize pixelsPerInch() const;
    virtual Dui::Orientation orientation() const;
    virtual void setOrientationAngle(DuiDeviceProfile::DeviceOrientationAngle angle);
    virtual void DuiDeviceProfileDestructor();
    virtual void orientationChanged();
    DuiDeviceProfilePrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
DuiDeviceProfile *DuiDeviceProfileStub::instance()
{
    stubMethodEntered("instance");
    return stubReturnValue<DuiDeviceProfile *>("instance");
}

QString DuiDeviceProfileStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

QSize DuiDeviceProfileStub::resolution() const
{
    stubMethodEntered("resolution");
    return stubReturnValue<QSize>("resolution");
}

QRectF DuiDeviceProfileStub::screenRectF() const
{
    stubMethodEntered("screenRectF");
    return stubReturnValue<QRectF>("screenRectF");
}

int DuiDeviceProfileStub::width() const
{
    stubMethodEntered("width");
    return stubReturnValue<int>("width");
}

int DuiDeviceProfileStub::height() const
{
    stubMethodEntered("height");
    return stubReturnValue<int>("height");
}

QSize DuiDeviceProfileStub::pixelsPerInch() const
{
    stubMethodEntered("pixelsPerInch");
    return stubReturnValue<QSize>("pixelsPerInch");
}

Dui::Orientation DuiDeviceProfileStub::orientation() const
{
    stubMethodEntered("orientation");
    return stubReturnValue<Dui::Orientation>("orientation");
}

void DuiDeviceProfileStub::orientationChanged()
{
}


void DuiDeviceProfileStub::setOrientationAngle(DuiDeviceProfile::DeviceOrientationAngle angle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiDeviceProfile::DeviceOrientationAngle >(angle));
    stubMethodEntered("setOrientationAngle", params);
}

void DuiDeviceProfileStub::DuiDeviceProfileDestructor()
{

}


// 3. CREATE A STUB INSTANCE
DuiDeviceProfileStub gDefaultDuiDeviceProfileStub;
DuiDeviceProfileStub *gDuiDeviceProfileStub = &gDefaultDuiDeviceProfileStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiDeviceProfile *DuiDeviceProfile::instance()
{
    return gDuiDeviceProfileStub->instance();
}

QString DuiDeviceProfile::name() const
{
    return gDuiDeviceProfileStub->name();
}

QSize DuiDeviceProfile::resolution() const
{
    return gDuiDeviceProfileStub->resolution();
}

QRectF DuiDeviceProfile::screenRectF() const
{
    return gDuiDeviceProfileStub->screenRectF();
}

int DuiDeviceProfile::width() const
{
    return gDuiDeviceProfileStub->width();
}

int DuiDeviceProfile::height() const
{
    return gDuiDeviceProfileStub->height();
}

QSize DuiDeviceProfile::pixelsPerInch() const
{
    return gDuiDeviceProfileStub->pixelsPerInch();
}

Dui::Orientation DuiDeviceProfile::orientation() const
{
    return gDuiDeviceProfileStub->orientation();
}

void DuiDeviceProfile::setOrientationAngle(DeviceOrientationAngle angle)
{
    gDuiDeviceProfileStub->setOrientationAngle(angle);
}

DuiDeviceProfile::~DuiDeviceProfile()
{
    gDuiDeviceProfileStub->DuiDeviceProfileDestructor();
}

void DuiDeviceProfile::orientationChanged()
{
    gDuiDeviceProfileStub->orientationChanged();
}

#endif
