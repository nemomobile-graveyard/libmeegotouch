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

#include <QStringList>
#include "mappletid.h"

MAppletId::MAppletId() :
    applicationName_(),
    mashupCanvasName_(),
    appletInstanceId(0)
{
}

MAppletId::MAppletId(const QString &applicationName, const QString &mashupCanvasName, const AppletInstanceID &instanceId) :
    applicationName_(applicationName),
    mashupCanvasName_(mashupCanvasName),
    appletInstanceId(instanceId)
{
}

MAppletId::MAppletId(const QString &appletId) :
        appletInstanceId(0)
{
    QStringList list = appletId.split('/');
    if (list.length() > 0) {
        applicationName_ = list.at(0);
        if (list.length() > 1) {
            mashupCanvasName_ = list.at(1);
            if (list.length() > 2) {
                appletInstanceId = list.at(2).toUInt();
            }
        }
    }
}

MAppletId::~MAppletId()
{
}

QString MAppletId::toString() const
{
    QString ret = applicationName_ + '/' + mashupCanvasName_ + '/' + QString::number(appletInstanceId);
    return ret;
}

QString MAppletId::applicationName() const
{
    return applicationName_;
}

QString MAppletId::mashupCanvasName() const
{
    return mashupCanvasName_;
}

MAppletId::AppletInstanceID MAppletId::instanceId() const
{
    return appletInstanceId;
}
