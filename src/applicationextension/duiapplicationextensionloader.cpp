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

#include "duiapplicationextensionloader.h"
#include "duiapplicationextensioninterface.h"
#include "duiapplicationextensionmetadata.h"
#include <QPluginLoader>

DuiApplicationExtensionLoader::DuiApplicationExtensionLoader()
{
}

DuiApplicationExtensionInterface *DuiApplicationExtensionLoader::loadExtension(const DuiApplicationExtensionMetaData &metadata)
{
    QPluginLoader loader(metadata.extensionBinary());
    QObject *object = loader.instance();

    if (object != NULL) {
        if (object->inherits(metadata.interface().toUtf8().constData())) {
            DuiApplicationExtensionInterface *extension = qobject_cast<DuiApplicationExtensionInterface *>(object);
            if (extension != NULL) {
                if (extension->initialize()) {
                    return extension;
                }
            }
        }
        delete object;
    }

    return false;
}
