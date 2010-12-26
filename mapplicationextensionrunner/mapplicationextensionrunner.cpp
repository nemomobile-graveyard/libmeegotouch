/****************************************************************************
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

#include "mapplicationextensionrunner.h"
#include <mapplicationextensionmetadata.h>
#include <mapplicationextensionloader.h>
#include <mapplicationextensioninterface.h>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <MDebug>

MApplicationExtensionRunner::MApplicationExtensionRunner() :
        MExtensionRunner(),
        extension(NULL)
{
}

MApplicationExtensionRunner::~MApplicationExtensionRunner()
{
}

bool MApplicationExtensionRunner::init(const QString &serverName, MApplicationExtensionMetaData &metaData)
{
    // Check whether the metadata is valid
    if (!metaData.isValid()) {
        mWarning("MApplicationExtensionRunner") << "Application extension" << metaData.fileName() << "metadata is not valid.";
        return false;
    }

    bool success = MExtensionRunner::init(serverName);

    if (success) {
        success = false;
        extension = MApplicationExtensionLoader::loadExtension(metaData);
        if (extension != NULL) {
            widget = extension->widget();
            if (widget != NULL) {
                QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(parentWidget->layout());
                if (layout != NULL) {
                    layout->addItem(widget);
                    success = true;
                }
            }
        } else {
            mWarning("MApplicationExtensionRunner") << "Application extension" << metaData.fileName() << "could not be loaded.";
        }
    } else {
        mWarning("MApplicationExtensionRunner") << "Application extension" << metaData.fileName() << "could not be initialized.";
    }

    return success;
}

void MApplicationExtensionRunner::teardown()
{
    MExtensionRunner::teardown();

    delete extension;
    extension = NULL;
}
