/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
/*
 * automatically generated with the command line :
 * m-servicefwgen -a com.nokia.widgetsgallery.AccessManager
 *
 * m-servicefwgen is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef ACCESSMANAGERADAPTOR_H_1309946715
#define ACCESSMANAGERADAPTOR_H_1309946715

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.nokia.widgetsgallery.AccessManager
 */
class AccessManagerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.widgetsgallery.AccessManager")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.nokia.widgetsgallery.AccessManager\">\n"
"    <method name=\"login\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"_windowId\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"_taskTitle\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"fullscreen\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"autofocus\"/>\n"
"      <arg direction=\"in\" type=\"i\" name=\"orientation\"/>\n"
"      <arg direction=\"out\" type=\"b\" name=\"\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    AccessManagerAdaptor(QObject *parent);
    virtual ~AccessManagerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool login(const uint _windowId, const QString _taskTitle, bool fullscreen, bool autofocus, int orientation);
Q_SIGNALS: // SIGNALS
};

#endif
