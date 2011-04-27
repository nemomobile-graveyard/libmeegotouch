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

#ifndef TEXTPROCESSORINTERFACEADAPTOR_H_1267694774
#define TEXTPROCESSORINTERFACEADAPTOR_H_1267694774

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.nokia.TextProcessorInterface
 */
class TextProcessorInterfaceAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.TextProcessorInterface")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.nokia.TextProcessorInterface\">\n"
"    <method name=\"reverse\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"message\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    TextProcessorInterfaceAdaptor(QObject *parent);
    virtual ~TextProcessorInterfaceAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    QString reverse(const QString &message);
Q_SIGNALS: // SIGNALS
};

#endif
