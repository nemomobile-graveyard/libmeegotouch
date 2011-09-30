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
 * m-servicefwgen -p com.nokia.widgetsgallery.AccessManager
 */

#include "accessmanager.h"
#include "accessmanagerproxy.h"

bool AccessManager::login( bool fullscreen, bool autofocus, int orientation )
{
    return login( fullscreen, autofocus, orientation, QString() );
}

bool AccessManager::login( bool fullscreen, bool autofocus, int orientation, const QString &_taskTitle )
{
    return qobject_cast<AccessManagerProxy*>(interfaceProxy())->login( fullscreen, autofocus, orientation, _taskTitle ).value();
}

AccessManager::AccessManager( const QString& preferredService, QObject* parent )
    : MServiceFwBaseIf( AccessManagerProxy::staticInterfaceName(), parent )
{
    // Resolve the provider service name
    QString service = resolveServiceName( interfaceName(), preferredService );

    bool serviceNameInvalid = service.contains( " " ); // "not provided" - when the service wouldn't run
    if ( serviceNameInvalid ) {
        service.clear();
    }
    setServiceName( service );

    if (!service.isEmpty()) {
        // Construct the D-Bus proxy
        setInterfaceProxy( new AccessManagerProxy( service, "/", QDBusConnection::sessionBus(), this ));
        // allConnectSignals go here (empty block if none)

    }
}

void AccessManager::setService(const QString & service)
{
    if (service.isEmpty()) return;

    setServiceName( service );
    setInterfaceProxy( new AccessManagerProxy( service, "/", QDBusConnection::sessionBus(), this ));
    {

    }
}

