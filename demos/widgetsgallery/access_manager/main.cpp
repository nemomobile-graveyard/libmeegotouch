/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <MApplication>
#include <QDBusConnection>
#include "accessmanagerimpl.h"
#include "accessmanageradaptor.h"

int main(int argc, char *argv[])
{
    MApplication app(argc, argv);

    AccessManagerImpl implementation;

    new AccessManagerAdaptor(&implementation);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("com.nokia.widgetsgallery.AccessManager");
    connection.registerObject("/",&implementation);

    return app.exec();
}
