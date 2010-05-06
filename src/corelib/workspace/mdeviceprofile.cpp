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

#include <QString>
#include <QSize>
#include <QSettings>
#include <QFile>

#include "mdebug.h"
#include "mdeviceprofile.h"
#include "mdeviceprofile_p.h"

#include "mapplication.h"
#include "mcomponentdata.h"
#include "mcomponentdata_p.h"

MDeviceProfilePrivate::MDeviceProfilePrivate()
    : q_ptr(0)
{
    QString filename = "/etc/meegotouch/devices.conf";

    if(!load(filename)) {
        qFatal("Failed to load device profile.");
    }
}

MDeviceProfilePrivate::~MDeviceProfilePrivate()
{
}

bool MDeviceProfilePrivate::load(const QString& filename)
{
    QString device = MApplication::deviceName();
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
 * @return pointer to the singleton MDeviceProfile instance
 */
MDeviceProfile *MDeviceProfile::instance()
{
    MComponentData *data = MComponentData::instance();
    if (!data)
        qFatal("There is no instance of MDeviceProfile. Please create MComponentData first.");
    return data->d_ptr->deviceProfile;
}

MDeviceProfile::MDeviceProfile(QObject *parent)
    : QObject(parent),
      d_ptr(new MDeviceProfilePrivate)
{
    Q_D(MDeviceProfile);
    d->q_ptr = this;

    if (!MComponentData::instance())
        qFatal("There is no instance of MComponentData. Please create MApplication first.");
    if (MComponentData::instance()->d_ptr->deviceProfile)
        qFatal("Device profile is already created. Please use MDeviceProfile::instance()");
}

MDeviceProfile::~MDeviceProfile()
{
    delete d_ptr;
}

QSize MDeviceProfile::resolution() const
{
    Q_D(const MDeviceProfile);
    return d->resolution;
}

QSize MDeviceProfile::pixelsPerInch() const
{
    Q_D(const MDeviceProfile);
    return d->pixelsPerInch;
}
