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

#ifndef MSERVICEFWPROXY_H_LOCAL
#define MSERVICEFWPROXY_H_LOCAL

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class QDBusConnection;

class MockQDBusConnection
{
public:
    bool isConnected() const {
        return true;
    };
};

/*
 * Proxy class for interface com.nokia.MServiceFwIf
 */
class MServiceFwProxy : public QObject
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName() {
        return "com.nokia.MServiceFwIf";
    }

public:
    MServiceFwProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    virtual ~MServiceFwProxy();

    MockQDBusConnection connection() {
        static MockQDBusConnection myConnection;
        return myConnection;
    };

public Q_SLOTS: // METHODS
    QString serviceName(const QString &interfaceName);

    QStringList serviceNames(const QString &interfaceName);

    QString servicePath(const QString &interfaceName);

Q_SIGNALS: // SIGNALS
    void serviceAvailable(const QString &service, const QString &interface);
    void serviceUnavailable(const QString &service);

};

namespace com
{
    namespace nokia
    {
        typedef ::MServiceFwProxy MServiceFwIf;
    }
}
#endif
