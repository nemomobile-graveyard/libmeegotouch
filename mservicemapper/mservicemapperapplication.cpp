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


#include "mservicemapperapplication.h"

#include <QDBusConnection>

MServiceMapperApplication::MServiceMapperApplication( int & argc, char** argv )
  : QCoreApplication( argc, argv )
{
    QDBusConnection connection = QDBusConnection::sessionBus();

    // this signal is emitted, when the connection to the
    // dbus daemon dies, e.g. when the dbus daemon gets killed.

    bool ok = connection.connect(
	    "",                            // service
	    "/org/freedesktop/DBus/Local", // path
	    "org.freedesktop.DBus.Local",  // interface
	    "Disconnected",                // (signal) name
	    this,
	    SLOT( handleDBusServerDied() ) );

    if ( ! ok )
    {
        qWarning( "dbus connect failed" );
    }
}

void MServiceMapperApplication::handleDBusServerDied()
{
    qWarning( "MServiceMapperApplication::handleDBusServerDied: exiting." );

    // quit the servicemapper gracefully
    quit();
}
