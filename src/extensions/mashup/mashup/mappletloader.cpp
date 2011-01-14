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

#include "mappletloader.h"
#include "mappletinterface.h"
#include "mappletmetadata.h"
#include "mdataaccess.h"

#include <QPluginLoader>

MAppletLoader::MAppletLoader()
{
}

QGraphicsWidget *MAppletLoader::loadApplet(const MAppletMetaData &metadata, MDataStore &dataStore, MDataAccess &settings)
{
    QPluginLoader loader(metadata.appletBinary());
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint);
    QObject *object = loader.instance();

    MAppletInterface *applet = qobject_cast<MAppletInterface *>(object);
    QGraphicsWidget *widget = NULL;
    if (applet != NULL) {
        widget = applet->constructWidget(metadata, dataStore, settings);
    }
    delete object;
    return widget;
}

