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
// Make doxygen skip this internal class
//! \cond

#ifndef MSERVICEMAPPERADAPTOR_H_1263907450
#define MSERVICEMAPPERADAPTOR_H_1263907450

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.nokia.MServiceFwIf
 */
class MServiceMapperAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.MServiceFwIf")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.nokia.MServiceFwIf\" >\n"
                "    <!--\n"
                "            Returns a list of service names that implement the specified interface,\n"
                "            or, if none specified, all servicenames\n"
                "        -->\n"
                "    <method name=\"serviceNames\" >\n"
                "      <arg direction=\"in\" type=\"s\" name=\"interfaceName\" />\n"
                "      <arg direction=\"out\" type=\"as\" />\n"
                "    </method>\n"
                "    <!--\n"
                "            Returns a (preferred) service name that implements the specified interface\n"
                "        -->\n"
                "    <method name=\"serviceName\" >\n"
                "      <arg direction=\"in\" type=\"s\" name=\"interfaceName\" />\n"
                "      <arg direction=\"out\" type=\"s\" />\n"
                "    </method>\n"
                "    <!--\n"
                "            Returns the service path for the specified interface\n"
                "        -->\n"
                "    <method name=\"servicePath\" >\n"
                "      <arg direction=\"in\" type=\"s\" name=\"interfaceName\" />\n"
                "      <arg direction=\"out\" type=\"s\" />\n"
                "    </method>\n"
                "    <method name=\"interfaceName\" >\n"
                "      <arg direction=\"in\" type=\"s\" name=\"serviceName\" />\n"
                "      <arg direction=\"out\" type=\"s\" />\n"
                "    </method>\n"
                "    <signal name=\"serviceAvailable\" >\n"
                "      <arg direction=\"out\" type=\"s\" name=\"service\" />\n"
                "      <arg direction=\"out\" type=\"s\" name=\"interface\" />\n"
                "    </signal>\n"
                "    <signal name=\"serviceUnavailable\" >\n"
                "      <arg direction=\"out\" type=\"s\" name=\"service\" />\n"
                "    </signal>\n"
                "  </interface>\n"
                "")
public:
    MServiceMapperAdaptor(QObject *parent);
    virtual ~MServiceMapperAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    QString serviceName(const QString &interfaceName);
    QStringList serviceNames(const QString &interfaceName);
    QString servicePath(const QString &interfaceName);
    QString interfaceName(const QString &serviceName);
Q_SIGNALS: // SIGNALS
    void serviceAvailable(const QString &service, const QString &interface);
    void serviceUnavailable(const QString &service);
};

#endif
//! \endcond
