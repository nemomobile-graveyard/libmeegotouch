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

#include "duiappletloader.h"

#include "duiappletinterface.h"
#include "duiwidget.h"
#include "duiappletmetadata.h"
#include "duidataaccess.h"

#include <QPluginLoader>

DuiAppletLoader::DuiAppletLoader()
{
}

DuiWidget *DuiAppletLoader::loadApplet(const DuiAppletMetaData &metadata, DuiDataStore &dataStore, DuiDataAccess &settings)
{
    QPluginLoader loader(metadata.appletBinary());
    QObject *object = loader.instance();

    DuiAppletInterface *applet = qobject_cast<DuiAppletInterface *>(object);
    DuiWidget *widget = NULL;
    if (applet != NULL) {
        widget = applet->constructWidget(metadata, dataStore, settings);
    }
    delete object;
    return widget;
}

