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

/*
 * automatically generated with the command line :
 * ../../../../../tools/dui-servicefwgen -p com.nokia.TextProcessorIf
 */

#include "textprocessorif.h"

QString TextProcessorIf::reverse(const QString &message)
{
    return qobject_cast<TextProcessorIfProxy *>(interfaceProxy)->reverse(message).value();
}

TextProcessorIf::TextProcessorIf(const QString &preferredService, QObject *parent)
    : DuiServiceFwBaseIf(TextProcessorIfProxy::staticInterfaceName(), parent)
{
    // Resolve the provider service name
    service = resolveServiceName(interface, preferredService);

    bool serviceNameInvalid = service.contains(" ");   // "not provided" - when the service wouldn't run
    if (serviceNameInvalid) {
        service.clear();
    }

    if (!service.isEmpty()) {
        // Construct the D-Bus proxy
        interfaceProxy = new TextProcessorIfProxy(service, "/", QDBusConnection::sessionBus(), this);
        // allConnectSignals go here (empty block if none)

    }
}

void TextProcessorIf::setService(const QString &service)
{
    if (service.isEmpty()) return;

    this->service = service;

    if (interfaceProxy) {
        delete interfaceProxy;
        interfaceProxy = 0;
    }
    interfaceProxy = new TextProcessorIfProxy(service, "/", QDBusConnection::sessionBus(), this);
    {

    }
}
