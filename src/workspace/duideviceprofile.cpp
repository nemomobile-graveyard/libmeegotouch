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

#include "duidebug.h"
#include "duideviceprofile.h"
#include "duideviceprofile_p.h"
#include "duidevicestyle.h"
#include "duitheme.h"

#include "duiapplication.h"
#include "duicomponentdata.h"
#include "duicomponentdata_p.h"

DuiDeviceProfilePrivate::DuiDeviceProfilePrivate()
    : style(0)
{
    style = static_cast<const DuiDeviceStyle *>(DuiTheme::style("DuiDeviceStyle", DuiApplication::deviceName()));
}

DuiDeviceProfilePrivate::~DuiDeviceProfilePrivate()
{
    if (style) {
        DuiTheme::releaseStyle(style);
        style = 0;
    }
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

    if (d->style)
        return d->style->resolution();
    else
        return QSize();
}

QSize DuiDeviceProfile::pixelsPerInch() const
{
    Q_D(const DuiDeviceProfile);

    if (d->style)
        return d->style->pixelsPerInch();
    else
        return QSize();
}
