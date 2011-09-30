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
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MComponentCache>
#include <MExport>
#include <MButton>

#include "imageviewer.h"
#include "chainingexampleinterfaceadaptor.h"

#include "imageviewer.h"

M_EXPORT int main(int argc, char** argv)
{
    qCritical( "holger chainingexampleapp2" );
    MApplication& app = *MComponentCache::mApplication(argc, argv);

    ImageViewer myImageViewer;

    new ChainingExampleInterfaceAdaptor( &myImageViewer );

    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService( "com.nokia.chainingexampleapp2" );

    ret = connection.registerObject("/", &myImageViewer);

    ret = app.exec();

    return ret;
}
