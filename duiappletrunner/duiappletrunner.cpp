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

#include "duiappletrunner.h"
#include <duiappletmetadata.h>
#include <duiappletsettings.h>
#include <duiappleticonchangedmessage.h>
#include <duiapplettitlechangedmessage.h>
#include <duiapplettextchangedmessage.h>
#include <duiappletclient.h>
#include <duiappletloader.h>
#include <duifiledatastore.h>
#include <duiwidget.h>
#include <QMetaProperty>
#include <QGraphicsLinearLayout>
#include <QDebug>

DuiAppletRunner::DuiAppletRunner() : DuiExtensionRunner(),
    appletInstanceDataStore(NULL),
    appletSettings(NULL)
{
}

DuiAppletRunner::~DuiAppletRunner()
{
    delete appletSettings;
}

bool DuiAppletRunner::init(const QString &serverName, DuiAppletMetaData &metaData, const QString &appletId, const QString &appletInstanceFileDataPath)
{
    // Check whether the applet ID is valid
    if (appletId.isEmpty()) {
        qWarning() << "Applet ID is not valid.";
        return false;
    }

    // Check whether the metadata is valid
    if (!metaData.isValid()) {
        qWarning() << "Applet metadata is not valid.";
        return false;
    }

    // Try to construct a valid instance data store and bail out if not successful
    appletInstanceDataStore = new DuiFileDataStore(appletInstanceFileDataPath);
    if (!appletInstanceDataStore->isReadable() || !appletInstanceDataStore->isWritable()) {
        qWarning() << "Couldn't create applet instance data store";
        teardown();
        return false;
    }

    appletSettings = new DuiAppletSettings(metaData.fileName(), appletId);

    bool success = DuiExtensionRunner::init(serverName);

    if (success) {
        // Instantiate the applet and get the applet's widget
        widget = DuiAppletLoader::loadApplet(metaData, *appletInstanceDataStore, *appletSettings->dataAccess());
        success = false;

        if (widget != NULL) {
            // Put the widget into the parent widget's layout for layout request watching
            QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(parentWidget->layout());
            if (layout != NULL) {
                layout->addItem(widget);
                success = true;
            }
        }

        if (success) {
            setAppletProperties();
        }
    }

    return success;
}

void DuiAppletRunner::setAppletProperties()
{
    if (widget == NULL) {
        return;
    }

    const QMetaObject *mob = widget->metaObject();

    // use applet properties to fill header of the container
    int iconProperty = mob->indexOfProperty("appletIcon");
    int titleProperty = mob->indexOfProperty("appletTitle");
    int textProperty = mob->indexOfProperty("appletText");

    int iconSignal  = mob->indexOfSignal("appletIconChanged(QString)");
    int titleSignal = mob->indexOfSignal("appletTitleChanged(QString)");
    int textSignal  = mob->indexOfSignal("appletTextChanged(QString)");

    if (iconProperty != -1) {
        QString icon((mob->property(iconProperty).read(widget)).toString());
        if (!icon.isNull()) {
            appletIconChanged(icon);
        }
    }

    if (titleProperty != -1) {
        QString title((mob->property(titleProperty).read(widget)).toString());
        if (!title.isNull()) {
            appletTitleChanged(title);
        }
    }

    if (textProperty != -1) {
        QString text((mob->property(textProperty).read(widget)).toString());
        if (!text.isNull()) {
            appletTextChanged(text);
        }
    }

    if (iconSignal != -1) {
        connect(widget, SIGNAL(appletIconChanged(QString)), this, SLOT(appletIconChanged(QString)));
    }

    if (titleSignal != -1) {
        connect(widget, SIGNAL(appletTitleChanged(QString)), this, SLOT(appletTitleChanged(QString)));
    }

    if (textSignal != -1) {
        connect(widget, SIGNAL(appletTextChanged(QString)), this, SLOT(appletTextChanged(QString)));
    }

    // Connect the deprecated signals
    // TODO remove these after the deprecation period
    iconSignal  = mob->indexOfSignal("setAppletIcon(QString)");
    titleSignal = mob->indexOfSignal("setAppletTitle(QString)");
    textSignal  = mob->indexOfSignal("setAppletText(QString)");
    if (iconSignal != -1) {
        connect(widget, SIGNAL(setAppletIcon(QString)), this, SLOT(appletIconChanged(QString)));
    }
    if (titleSignal != -1) {
        connect(widget, SIGNAL(setAppletTitle(QString)), this, SLOT(appletTitleChanged(QString)));
    }
    if (textSignal != -1) {
        connect(widget, SIGNAL(setAppletText(QString)), this, SLOT(appletTextChanged(QString)));
    }

    connect(this, SIGNAL(visibilityChanged(bool)), widget, SIGNAL(visibilityChanged(bool)));
    connect(view, SIGNAL(orientationChanged(Dui::Orientation)), widget, SIGNAL(orientationChanged(Dui::Orientation)));
}

void DuiAppletRunner::teardown()
{
    DuiExtensionRunner::teardown();

    delete appletInstanceDataStore;
    appletInstanceDataStore = NULL;
}

void DuiAppletRunner::appletIconChanged(const QString &newIcon)
{
    DuiAppletIconChangedMessage m;
    m.setIcon(newIcon);
    communicator->sendMessage(m);
}

void DuiAppletRunner::appletTitleChanged(const QString &newTitle)
{
    DuiAppletTitleChangedMessage m;
    m.setTitle(newTitle);
    communicator->sendMessage(m);
}

void DuiAppletRunner::appletTextChanged(const QString &newText)
{
    DuiAppletTextChangedMessage m;
    m.setText(newText);
    communicator->sendMessage(m);
}
