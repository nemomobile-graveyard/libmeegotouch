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

//! \internal

// Command line was: qdbusxml2cpp -c MApplicationIfAdaptor -a mapplicationadaptor.h:mapplicationadaptor.cpp MApplicationIfAdaptor.xml

#ifndef MAPPLICATIONIFADAPTOR_H
#define MAPPLICATIONIFADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "mexport.h"

class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*!
 * \class MApplicationIfAdaptor
 * \brief Adaptor class for interface com.nokia.MApplicationIf
 *
 * This is the adaptor for the MApplication QDBus service.
 * Do not use this class directly.
 */
class M_CORE_EXPORT MApplicationIfAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.MApplicationIf")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.nokia.MApplicationIf\" >\n"
                "    <method name=\"launch\" >\n"
                "    </method>\n"
                "    <method name=\"launch\">\n"
                "      <arg direction=\"in\" type=\"as\" name=\"parameters\"/>\n"
                "    </method>\n"
                "    <method name=\"close\" >\n"
                "    </method>\n"
                "    <method name=\"exit\" >\n"
                "    </method>\n"
                "  </interface>\n"
                "")
public:
    MApplicationIfAdaptor(QObject *parent);
    virtual ~MApplicationIfAdaptor();

public Q_SLOTS: // METHODS

    //! Close the GUI.
    Q_NOREPLY void close();

    //! Exit the application.
    Q_NOREPLY void exit();

    //! Launch the application.
    Q_NOREPLY void launch();

    //! Launch the application with parameters.
    Q_NOREPLY void launch(const QStringList& parameters);
};

#endif
//! \internal_end
