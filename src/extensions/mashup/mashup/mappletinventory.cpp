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

#include <QFileSystemWatcher>
#include <QPluginLoader>
#include <MDebug>
#include "mappletinventory.h"
#include "mappletinventorymodel.h"
#include "mappletinstancemanager.h"
#include "mappletbutton.h"
#include "mmashupcanvas.h"
#include "mappletinstallationsourceinterface.h"
#include "private/mwidgetcontroller_p.h"
#include "mappletinstantiator.h"
#include "mapplicationextensioninterface.h"
#include "mappletinstallationsourceinterface.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(MAppletInventory)

MAppletInventory::MAppletInventory(QGraphicsItem *parent) :
    MWidgetController(new MAppletInventoryModel, parent),
    mashupCanvas(NULL),
    watcher(new QFileSystemWatcher()),
    appletPath(APPLET_DATA),
    initialized(false)
{
}

MAppletInventory::~MAppletInventory()
{
    // Destroy the applet buttons
    foreach(MWidget * w, model()->widgets()) {
        delete w;
    }
    delete watcher;
}

void MAppletInventory::initializeIfNecessary()
{
    if (!initialized) {
        // Verify that the applet .desktop file path exists and is readable
        if (appletPath.exists() && appletPath.isReadable()) {
            // Start monitoring the applet desktop file path
            watcher->addPath(appletPath.absolutePath());

            if (watcher->directories().contains(appletPath.absolutePath())) {
                connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(appletPathChanged(QString)));

                // Trigger the first applet path refresh manually
                appletPathChanged(appletPath.absolutePath());
            }
        }
        // The applet inventory has now been initialized
        initialized = true;
    }
}

void MAppletInventory::setMashupCanvas(MMashupCanvas &mashupCanvas)
{
    this->mashupCanvas = &mashupCanvas;
}

void MAppletInventory::setCategories(const QStringList &categories)
{
    if (appletCategories != categories) {
        appletCategories = categories;

        // If the applet inventory has already been initialized re-read the installation path
        if (initialized && appletPath.exists() && appletPath.isReadable()) {
            appletPathChanged(appletPath.absolutePath());
        }
    }
}

QStringList MAppletInventory::categories() const
{
    return appletCategories;
}

void MAppletInventory::appletButtonClicked()
{
    MAppletButton *button = dynamic_cast<MAppletButton *>(sender());

    Q_ASSERT(button != NULL);

    if (button) {
        mashupCanvas->appletInstanceManager()->instantiateApplet(button->metadataFilename());
        emit hideAppletInventory();
    }
}

void MAppletInventory::appletPathChanged(const QString &path)
{
    Q_UNUSED(path);
    Q_ASSERT(appletPath.absolutePath() == path);

    // Move all widgets from the model to the old widgets list
    QList<MWidget *> oldWidgets(model()->widgets());

    // Go through the applets and create applet buttons of them in this container
    QList<MWidget *> widgets;
    QStringList filter;
    filter << "*.desktop";
    foreach(const QString & fileName, appletPath.entryList(filter, QDir::Files)) {
        const QString metadataFilename = QFileInfo(appletPath, fileName).absoluteFilePath();
        MAppletMetaData metadata(metadataFilename);

        // If no categories have been defined all applet buttons should be shown
        bool appletShouldBeShown = appletCategories.empty();

        if (!appletCategories.empty()) {
            // If categories have been defined only applets belonging in any of the defined categories should be shown
            foreach(const QString & category, metadata.categories()) {
                appletShouldBeShown = appletCategories.contains(category);
                if (appletShouldBeShown) {
                    break;
                }
            }
        }

        // Check if there was already a button for the applet
        bool alreadyAdded = false;
        foreach(MWidget * w, oldWidgets) {
            MAppletButton *b = dynamic_cast<MAppletButton *>(w);
            if (b != NULL && b->metadataFilename() == metadataFilename && appletShouldBeShown) {
                // Applet button already created - put it back in the widget list
                widgets.append(w);

                // Remove from the old widgets list
                oldWidgets.removeOne(w);

                // Don't create a new button
                alreadyAdded = true;
                break;
            }
        }

        if (!alreadyAdded && appletShouldBeShown) {
            // No button for this applet found in the model
            MAppletButton *appletButton = new MAppletButton(NULL);
            appletButton->setViewType("icon");
            if (appletButton->initialize(metadata)) {
                connect(appletButton, SIGNAL(clicked()), this, SLOT(appletButtonClicked()));
                widgets.append(appletButton);
            } else {
                delete appletButton;
            }
        }
    }

    // Let the view know about the new widget list
    model()->setWidgets(widgets);

    // Send a signal about each applet that is no longer installed
    foreach(MWidget * w, oldWidgets) {
        MAppletButton *b = dynamic_cast<MAppletButton *>(w);
        if (b != NULL) {
            emit appletUninstalled(b->metadataFilename());
        }

        // Destroy the applet button. This MUST be done AFTER the widget has been removed from the layout.
        delete w;
    }
}

void MAppletInventory::setEnabled(bool enabled)
{
    if (enabled) {
        // Initialize the applet inventory if necessary
        initializeIfNecessary();
    }

    MWidgetController::setEnabled(enabled);

    // Show or hide the button
    model()->setCloseButtonVisible(enabled);
}

void MAppletInventory::setMAppletInventoryInterface(MApplicationExtensionInterface *extension)
{
    MAppletInstallationSourceInterface *plugin = static_cast<MAppletInstallationSourceInterface* >(extension);
    plugin->setMAppletInventoryInterface(*this);
}

void MAppletInventory::instantiateAppletsInPackage(const QString &packageName)
{
    MAppletInstantiator::instance()->instantiateAppletsInPackage(packageName, mashupCanvas->serviceAddress());
    emit hideAppletInventory();
}
