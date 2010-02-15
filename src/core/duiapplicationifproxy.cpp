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

// Command line was: qdbusxml2cpp -c DuiApplicationIfProxy -p duiapplicationifproxy.h:duiapplicationifproxy.cpp com.nokia.DuiApplicationIf.xml

#include "duiapplicationifproxy.h"

DuiApplicationIfProxy::DuiApplicationIfProxy(const QString &service, QObject *parent) :
    QDBusAbstractInterface(service, "/org/maemo/dui", "com.nokia.DuiApplicationIf", QDBusConnection::sessionBus(), parent)
{
}

DuiApplicationIfProxy::~DuiApplicationIfProxy()
{
}

QDBusPendingReply<> DuiApplicationIfProxy::close()
{
    return asyncCall(QLatin1String("close"));
}

QDBusPendingReply<> DuiApplicationIfProxy::exit()
{
    return asyncCall(QLatin1String("exit"));
}

QDBusPendingReply<> DuiApplicationIfProxy::launch()
{
    return asyncCallWithArgumentList(QLatin1String("launch"), QList<QVariant>());
}

