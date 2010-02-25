/****************************************************************************
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

#include "duiapplicationextensionrunner.h"
#include <duiapplicationextensionmetadata.h>
#include <duiapplicationextensionloader.h>
#include <duiapplicationextensioninterface.h>
#include <DuiWidget>
#include <QGraphicsLinearLayout>
#include <QDebug>

DuiApplicationExtensionRunner::DuiApplicationExtensionRunner() : DuiExtensionRunner()
{
}

DuiApplicationExtensionRunner::~DuiApplicationExtensionRunner()
{
}

bool DuiApplicationExtensionRunner::init(const QString &serverName, DuiApplicationExtensionMetaData &metaData)
{
    // Check whether the metadata is valid
    if (!metaData.isValid()) {
        qWarning() << "Extension metadata is not valid.";
        return false;
    }

    bool success = DuiExtensionRunner::init(serverName);

    if (success) {
        success = false;
        extension = DuiApplicationExtensionLoader::loadExtension(metaData);
        if (extension != NULL) {
            widget = extension->widget();
            if (widget != NULL) {
                QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(parentWidget->layout());
                if (layout != NULL) {
                    layout->addItem(widget);
                    success = true;
                }
            }
        }
    }

    return success;
}

void DuiApplicationExtensionRunner::teardown()
{
    DuiExtensionRunner::teardown();

    delete extension;
    extension = NULL;
}
