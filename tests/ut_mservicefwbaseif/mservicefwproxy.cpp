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

#include "mservicefwproxy.h"
#include "ut_mservicefwbaseif.h"

MServiceFwProxy::MServiceFwProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent) :
    QObject(parent)
{
    Q_UNUSED(service);
    Q_UNUSED(path);
    Q_UNUSED(connection);
    Q_UNUSED(parent);
}

MServiceFwProxy::~MServiceFwProxy()
{
}

QString MServiceFwProxy::serviceName(const QString &interfaceName)
{
    Q_UNUSED(interfaceName);
    return QString(Ut_MServiceFwBaseIf::serviceFwService);
}

QStringList MServiceFwProxy::serviceNames(const QString &interfaceName)
{
    Q_UNUSED(interfaceName);
    return QStringList();
}

QString MServiceFwProxy::servicePath(const QString &interfaceName)
{
    Q_UNUSED(interfaceName);
    return QString();
}

