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

#include <QApplication>

#include "mservicemapper.h"
#include "mservicemapperadaptor.h"
#include "mservicemapperapplication.h"

int main(int argc, char **argv)
{
    MServiceMapperApplication app(argc, argv);

    QDBusConnection connection = QDBusConnection::sessionBus();

    if ( ! connection.isConnected() )
    {
        qWarning( "connecting to dbus failed, exiting." );
        return -1;
    }

    const QString serviceFileDir(M_DBUS_SERVICES_DIR);
    MServiceMapper service(serviceFileDir);
    new MServiceMapperAdaptor(&service); // must be on the heap, must not be deleted see QDBusAbstractAdaptor man page
    connection.registerService("com.nokia.MServiceFw");
    connection.registerObject("/", &service);

    return app.exec();
}
