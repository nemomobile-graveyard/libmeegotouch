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

// Make doxygen skip this internal class
//! \cond

// Command line was: qdbusxml2cpp -c DuiApplicationIfAdaptor -a duiapplicationadaptor.h:duiapplicationadaptor.cpp DuiApplicationIfAdaptor.xml

#ifndef DUIAPPLICATIONIFADAPTOR_H
#define DUIAPPLICATIONIFADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "duiexport.h"

class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*!
 * \class DuiApplicationIfAdaptor
 * \brief Adaptor class for interface com.nokia.DuiApplicationIf
 *
 * This is the adaptor for the DuiApplication QDBus service.
 * Do not use this class directly.
 */
class DUI_EXPORT DuiApplicationIfAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.DuiApplicationIf")
    Q_CLASSINFO("D-Bus Introspection", ""
                "  <interface name=\"com.nokia.DuiApplicationIf\" >\n"
                "    <method name=\"launch\" >\n"
                "    </method>\n"
                "    <method name=\"close\" >\n"
                "    </method>\n"
                "    <method name=\"exit\" >\n"
                "    </method>\n"
                "  </interface>\n"
                "")
public:
    DuiApplicationIfAdaptor(QObject *parent);
    virtual ~DuiApplicationIfAdaptor();

public Q_SLOTS: // METHODS

    //! Close the GUI.
    Q_NOREPLY void close();

    //! Exit the application.
    Q_NOREPLY void exit();

    //! Launch the application.
    Q_NOREPLY void launch();
};

#endif
//! \cond
