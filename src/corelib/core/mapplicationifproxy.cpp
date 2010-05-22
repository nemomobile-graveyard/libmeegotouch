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

// Command line was: qdbusxml2cpp -c MApplicationIfProxy -p mapplicationifproxy.h:mapplicationifproxy.cpp com.nokia.MApplicationIf.xml

#include "mapplicationifproxy.h"

MApplicationIfProxy::MApplicationIfProxy(const QString &service, QObject *parent) :
    QDBusAbstractInterface(service, "/org/maemo/m", "com.nokia.MApplicationIf", QDBusConnection::sessionBus(), parent)
{
}

MApplicationIfProxy::~MApplicationIfProxy()
{
}

QDBusPendingReply<> MApplicationIfProxy::close()
{
    return asyncCall(QLatin1String("close"));
}

QDBusPendingReply<> MApplicationIfProxy::exit()
{
    // here we only do the dbus call, when the interface is valid,
    // i.e. there is a program listening "on the other side" of the
    // dbus connection.
    if ( isValid() )
    {
        return asyncCall(QLatin1String("exit"));
    }
    else
    {
        return QDBusPendingReply<>();
    }
}

QDBusPendingReply<> MApplicationIfProxy::launch()
{
    return callWithArgumentList(QDBus::BlockWithGui, QLatin1String("launch"), QList<QVariant>());
}

QDBusPendingReply<> MApplicationIfProxy::launch(const QStringList &parameters)
{
    QList<QVariant> argumentList;
    argumentList << qVariantFromValue(parameters);
    return callWithArgumentList(QDBus::BlockWithGui, QLatin1String("launch"), argumentList);
}
