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

/*
 * automatically generated with the command line :
 * m-servicefwgen -p MUserGuideIf
 */

#include "muserguideif.h"
#include "muserguideifproxy.h"

bool MUserGuideIf::mainPage()
{
    return qobject_cast<MUserGuideIfProxy*>(interfaceProxy())->mainPage().value();
}

bool MUserGuideIf::pageByPath( const QString &path )
{
    return qobject_cast<MUserGuideIfProxy*>(interfaceProxy())->pageByPath( path ).value();
}

bool MUserGuideIf::pageByPathChained( const QString &path )
{
    return pageByPathChained( path, QString() );
}

bool MUserGuideIf::pageByPathChained( const QString &path, const QString &_taskTitle )
{
    return qobject_cast<MUserGuideIfProxy*>(interfaceProxy())->pageByPathChained( path, _taskTitle ).value();
}

MUserGuideIf::MUserGuideIf( const QString& preferredService, QObject* parent )
    : MServiceFwBaseIf( MUserGuideIfProxy::staticInterfaceName(), parent )
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
        setInterfaceProxy( new MUserGuideIfProxy( service, "/", QDBusConnection::sessionBus(), this ));
        // allConnectSignals go here (empty block if none)

    }
}

void MUserGuideIf::setService(const QString & service)
{
    if (service.isEmpty()) return;

    setServiceName( service );
    setInterfaceProxy( new MUserGuideIfProxy( service, "/", QDBusConnection::sessionBus(), this ));
    {

    }
}

