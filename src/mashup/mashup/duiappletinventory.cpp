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

#include <QFileSystemWatcher>
#include <QPluginLoader>
#include <DuiDebug>
#include "duiappletinventory.h"
#include "duiappletinventorymodel.h"
#include "duiappletinstancemanager.h"
#include "duiappletbutton.h"
#include "duimashupcanvas.h"
#include "duiappletinstallationsourceinterface.h"
#include "private/duiwidgetcontroller_p.h"
#include "duiappletinstantiator.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(DuiAppletInventory)

DuiAppletInventory::DuiAppletInventory(QGraphicsItem *parent) :
    DuiWidgetController(new DuiWidgetControllerPrivate, new DuiAppletInventoryModel, parent),
    mashupCanvas(NULL),
    watcher(new QFileSystemWatcher()),
    appletPath(APPLET_DATA),
    initialized(false)
{
}

DuiAppletInventory::~DuiAppletInventory()
{
    // Destroy the installation sources
    foreach(DuiWidget * w, model()->installationSources()) {
        delete w;
    }

    // Destroy the remaining applet buttons
    foreach(DuiWidget * w, model()->widgets()) {
        delete w;
    }

    delete watcher;
}

void DuiAppletInventory::initializeIfNecessary()
{
    if (!initialized) {
        // Create installation source widgets
        createInstallationSourceWidgets();

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

void DuiAppletInventory::setMashupCanvas(DuiMashupCanvas &mashupCanvas)
{
    this->mashupCanvas = &mashupCanvas;
}

void DuiAppletInventory::setCategories(const QStringList &categories)
{
    if (appletCategories != categories) {
        appletCategories = categories;

        // If the applet inventory has already been initialized re-read the installation path
        if (initialized && appletPath.exists() && appletPath.isReadable()) {
            appletPathChanged(appletPath.absolutePath());
        }
    }
}

QStringList DuiAppletInventory::categories() const
{
    return appletCategories;
}

void DuiAppletInventory::appletButtonClicked()
{
    DuiAppletButton *button = dynamic_cast<DuiAppletButton *>(sender());
    Q_ASSERT(button != NULL);

    mashupCanvas->appletInstanceManager()->instantiateApplet(button->metadataFilename());
    emit hideAppletInventory();
}

void DuiAppletInventory::appletPathChanged(const QString &path)
{
    Q_UNUSED(path);
    Q_ASSERT(appletPath.absolutePath() == path);

    // Move all widgets from the model to the old widgets list
    QList<DuiWidget *> oldWidgets(model()->widgets());

    // Go through the applets and create applet buttons of them in this container
    QList<DuiWidget *> widgets;
    QStringList filter;
    filter << "*.desktop";
    foreach(const QString & fileName, appletPath.entryList(filter, QDir::Files)) {
        const QString metadataFilename = QFileInfo(appletPath, fileName).absoluteFilePath();
        DuiAppletMetaData metadata(metadataFilename);

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
        foreach(DuiWidget * w, oldWidgets) {
            DuiAppletButton *b = dynamic_cast<DuiAppletButton *>(w);
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
            DuiAppletButton *appletButton = new DuiAppletButton(NULL);
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
    foreach(DuiWidget * w, oldWidgets) {
        DuiAppletButton *b = dynamic_cast<DuiAppletButton *>(w);
        if (b != NULL) {
            emit appletUninstalled(b->metadataFilename());
        }

        // Destroy the applet button. This MUST be done AFTER the widget has been removed from the layout.
        delete w;
    }
}

void DuiAppletInventory::setEnabled(bool enabled)
{
    if (enabled) {
        // Initialize the applet inventory if necessary
        initializeIfNecessary();
    }

    DuiWidgetController::setEnabled(enabled);

    // Show or hide the button
    model()->setCloseButtonVisible(enabled);
}

void DuiAppletInventory::createInstallationSourceWidgets()
{
    // Destroy the existing installation sources
    foreach(DuiWidget * w, model()->installationSources()) {
        delete w;
    }

    QList<DuiWidget *> sourceWidgets;

    // Load the installation sources
    QStringList sourcePaths = installationSourceBinaryPaths();
    foreach(const QString & path, sourcePaths) {
        DuiWidget *widget = loadAppletInstallationSource(path);
        if (widget) {
            sourceWidgets.append(widget);
        }
    }

    model()->setInstallationSources(sourceWidgets);
}

void DuiAppletInventory::setInstallationSources(const QStringList &installationSources)
{
    if (this->installationSources != installationSources) {
        this->installationSources = installationSources;

        if (initialized) {
            // If the applet inventory has already been initialized, create installation source widgets
            createInstallationSourceWidgets();
        }
    }
}

QStringList DuiAppletInventory::installationSourceBinaryPaths() const
{
    QStringList sourcePaths;

    foreach(const QString & sourceName, installationSources) {
        QFileInfo source(QString(APPLET_INSTALLATION_SOURCES), sourceName);
        if (source.exists() && source.isFile()) {
            sourcePaths.append(source.absoluteFilePath());
        } else {
            duiDebug("DuiAppletInventory") << "applet source" << source.absoluteFilePath() << "does not exist!";
        }
    }

    return sourcePaths;
}

DuiWidget *DuiAppletInventory::loadAppletInstallationSource(const QString &path)
{
    QPluginLoader loader(path);
    QObject *object = loader.instance();

    DuiAppletInstallationSourceInterface *source = qobject_cast<DuiAppletInstallationSourceInterface *>(object);
    DuiWidget *widget = NULL;
    if (source != NULL) {
        widget = source->constructWidget();
        if (widget) {
            const QMetaObject *mob = widget->metaObject();

            // connect signals from widget to the applet inventory
            if (mob->indexOfSignal("packageSelectedForInstallation(QString)") != -1) {
                QObject::connect(widget, SIGNAL(packageSelectedForInstallation(QString)), this, SLOT(instantiateAppletsFromPackage(QString)));
            }
        }
    }
    delete object;
    return widget;
}

void DuiAppletInventory::instantiateAppletsFromPackage(const QString &packageName)
{
    DuiAppletInstantiator::instance()->instantiateAppletsInPackage(packageName, mashupCanvas->serviceAddress());
    emit hideAppletInventory();
}
