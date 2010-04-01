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

#include <QString>
#include <QSize>
#include <QSettings>
#include <QFile>

#include "duidebug.h"
#include "duideviceprofile.h"
#include "duideviceprofile_p.h"

#include "duiapplication.h"
#include "duicomponentdata.h"
#include "duicomponentdata_p.h"

DuiDeviceProfilePrivate::DuiDeviceProfilePrivate()
    : q_ptr(0)
{
    QString filename = "/etc/dui/devices.conf";

    if(!load(filename)) {
        qFatal("Failed to load device profile.");
    }
}

DuiDeviceProfilePrivate::~DuiDeviceProfilePrivate()
{
}

bool DuiDeviceProfilePrivate::load(const QString& filename)
{
    QString device = DuiApplication::deviceName();
    if(device.isEmpty())
        device = "Default";

    if(!QFile::exists(filename))
        return false;

    QSettings settings(filename, QSettings::IniFormat);
    if(settings.status() != QSettings::NoError)
        return false;

    resolution.setWidth(settings.value(device + "/resolutionX", 0).toInt());
    resolution.setHeight(settings.value(device + "/resolutionY", 0).toInt());
    pixelsPerInch.setWidth(settings.value(device + "/ppiX", 0).toInt());
    pixelsPerInch.setHeight(settings.value(device + "/ppiY", 0).toInt());
    return true;
}

/*
 * @return pointer to the singleton DuiDeviceProfile instance
 */
DuiDeviceProfile *DuiDeviceProfile::instance()
{
    DuiComponentData *data = DuiComponentData::instance();
    if (!data)
        qFatal("There is no instance of DuiDeviceProfile. Please create DuiComponentData first.");
    return data->d_ptr->deviceProfile;
}

DuiDeviceProfile::DuiDeviceProfile(QObject *parent)
    : QObject(parent),
      d_ptr(new DuiDeviceProfilePrivate)
{
    Q_D(DuiDeviceProfile);
    d->q_ptr = this;

    if (!DuiComponentData::instance())
        qFatal("There is no instance of DuiComponentData. Please create DuiApplication first.");
    if (DuiComponentData::instance()->d_ptr->deviceProfile)
        qFatal("Device profile is already created. Please use DuiDeviceProfile::instance()");
}

DuiDeviceProfile::~DuiDeviceProfile()
{
    delete d_ptr;
}

QSize DuiDeviceProfile::resolution() const
{
    Q_D(const DuiDeviceProfile);
    return d->resolution;
}

QSize DuiDeviceProfile::pixelsPerInch() const
{
    Q_D(const DuiDeviceProfile);
    return d->pixelsPerInch;
}
