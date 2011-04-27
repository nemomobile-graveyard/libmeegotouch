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

#ifndef MAPPLETINSTANCEMANAGERDBUSADAPTOR_H_1265801264
#define MAPPLETINSTANCEMANAGERDBUSADAPTOR_H_1265801264

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "mappletinstancemanager.h"
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.meego.core.MAppletInstanceManager
 */
class MAppletInstanceManagerDBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meego.core.MAppletInstanceManager")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.meego.core.MAppletInstanceManager\">\n"
                "    <method name=\"instantiateAppletFromPackage\">\n"
                "      <annotation value=\"QMap&lt;QString, QVariant>\" name=\"com.trolltech.QtDBus.QtTypeName.In1\"/>\n"
                "      <arg direction=\"in\" type=\"s\" name=\"packageName\"/>\n"
                "      <arg direction=\"in\" type=\"a{sv}\" name=\"metaData\"/>\n"
                "    </method>\n"
                "  </interface>\n"
                "")
public:
    MAppletInstanceManagerDBusAdaptor(MAppletInstanceManager *parent);
    virtual ~MAppletInstanceManagerDBusAdaptor();

    inline MAppletInstanceManager *parent() const {
        return static_cast<MAppletInstanceManager *>(QObject::parent());
    }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData);
Q_SIGNALS: // SIGNALS
};

#endif
