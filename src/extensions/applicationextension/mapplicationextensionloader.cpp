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

#include "mapplicationextensionloader.h"
#include "mapplicationextensioninterface.h"
#include "mapplicationextensionmetadata.h"
#include <MDebug>
#include <QPluginLoader>

MApplicationExtensionLoader::MApplicationExtensionLoader()
{
}

MApplicationExtensionInterface *MApplicationExtensionLoader::loadExtension(const MApplicationExtensionMetaData &metadata)
{
    QPluginLoader loader(metadata.extensionBinary());
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint);
    QObject *object = loader.instance();

    if (object != NULL) {
        if (object->inherits(metadata.interface().toUtf8().constData())) {
            MApplicationExtensionInterface *extension = qobject_cast<MApplicationExtensionInterface *>(object);
            if (extension != NULL) {
                if (extension->initialize(metadata.interface())) {
                    return extension;
                } else {
                    mWarning("MApplicationExtensionLoader") << "Application extension" << metadata.fileName() << "could not be initialized.";
                }
            } else {
                mWarning("MApplicationExtensionLoader") << "Application extension" << metadata.fileName() << "could not be instantiated. The extension does not implement MApplicationExtensionInterface.";
            }
        } else {
            mWarning("MApplicationExtensionLoader") << "Application extension" << metadata.fileName() << "could not be instantiated. The extension does not inherit" << metadata.interface();
        }
        delete object;
    } else {
        mWarning("MApplicationExtensionLoader") << "Application extension" << metadata.fileName() << "could not be loaded." << loader.errorString();
    }

    return false;
}
