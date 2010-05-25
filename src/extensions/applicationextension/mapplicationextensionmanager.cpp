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

#include "mapplicationextensionmanager.h"
#include "mapplicationextensioninterface.h"
#include "mapplicationextensionmetadata.h"

#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <QFile>
#include "mdesktopentry.h"
#include "mextensionhandle.h"
#include "mfiledatastore.h"
#include "mdebug.h"

MApplicationExtensionManager::MApplicationExtensionManager(const QString &interface) :
        initialized(false),
        interface(interface)
{
}


MApplicationExtensionManager::~MApplicationExtensionManager()
{
    foreach (MExtensionHandle* handle, outOfProcessHandles) {
        delete handle;
    }
    foreach (InProcessExtensionData extension, inProcessExtensions) {
        delete extension.first;
    }
}

void MApplicationExtensionManager::setInProcessFilter(const QRegExp &inProcessFilter)
{
    if (!initialized) {
        this->inProcessFilter = inProcessFilter;
    }
}

void MApplicationExtensionManager::setOutOfProcessFilter(const QRegExp &outOfProcessFilter)
{
    if (!initialized) {
        this->outOfProcessFilter = outOfProcessFilter;
    }
}

bool MApplicationExtensionManager::init()
{
    if (initialized) {
        return false;
    }

    if (!createDataStore()) {
        mWarning("MApplicationExtensionManager") << "DataStore cannot be created for application extension manager. Bailing out.";
        return false;
    }

    initialized = true;
    updateAvailableExtensions(APPLICATION_EXTENSION_DATA_DIR);

    // Start watching the application extensions directory for changes
    connect(&watcher, SIGNAL(directoryChanged(const QString)), this, SLOT(updateAvailableExtensions(QString)));
    watcher.addPath(APPLICATION_EXTENSION_DATA_DIR);

    return true;
}

void MApplicationExtensionManager::updateAvailableExtensions(const QString &path)
{
    QList<QString> currentExtensionsList;

    QDir applicationExtensionsDir(path);
    if (applicationExtensionsDir.exists()) {
        QStringList filter;
        filter << "*.desktop";
        foreach(const QString &fileName, applicationExtensionsDir.entryList(filter, QDir::Files)) {
            const QString desktopFile = QFileInfo(applicationExtensionsDir, fileName).absoluteFilePath();
            QSharedPointer<MApplicationExtensionMetaData> metadata(new MApplicationExtensionMetaData(desktopFile));
            if (metadata->isValid() && metadata->type() == "X-MeeGoApplicationExtension" && metadata->interface() == interface) {
                currentExtensionsList.append(fileName);

                if (!extensionMetaDatas.contains(fileName)) {
                    // This extension is a new one. Instantiate it
                    if (instantiateExtension(*metadata.data())) {
                        extensionMetaDatas[fileName] = metadata;
                    }
                }
            }
        }

        // Find what extensions got removed
        QList<QString> toBeRemovedExtensions;
        MetaDataEntryHash::const_iterator itEnd = extensionMetaDatas.constEnd();
        for (MetaDataEntryHash::const_iterator it = extensionMetaDatas.constBegin(); it != itEnd; ++it) {
            if (!currentExtensionsList.contains(it.key())) {
                toBeRemovedExtensions.append(it.key());
            }
        }
        // Remove extensions that no longer exist
        foreach (const QString &fileName, toBeRemovedExtensions) {
            removeExtension(*extensionMetaDatas[fileName].data());
            extensionMetaDatas.remove(fileName);
        }
    }
}

bool MApplicationExtensionManager::isInProcess(const MApplicationExtensionMetaData &metaData)
{
    return metaData.runnerBinary().isEmpty();
}

bool MApplicationExtensionManager::instantiateExtension(const MApplicationExtensionMetaData &metadata)
{
    bool success = false;

    if (isInProcess(metadata)) {
        if (metadata.fileName().indexOf(inProcessFilter) >= 0) {
            success = instantiateInProcessExtension(metadata.extensionBinary());
        }
    } else {
        if (metadata.fileName().indexOf(outOfProcessFilter) >= 0) {
            success = instantiateOutOfProcessExtension(metadata);
        }
    }

    return success;
}

bool MApplicationExtensionManager::instantiateInProcessExtension(const QString &binary)
{
    QPluginLoader loader(binary);
    QObject *object = loader.instance();

    bool success = false;
    if (object != NULL) {
        if (object->inherits(interface.toUtf8().constData())) {
            MApplicationExtensionInterface *extension = qobject_cast<MApplicationExtensionInterface *>(object);
            if (extension != NULL) {
                success = extension->initialize(interface);
                if (success) {
                    QGraphicsWidget *widget = extension->widget();
                    if (widget) {
                        // Inform about the added extension widget
                        emit widgetCreated(widget, *extensionDataStore.data());
                    }
                    // Inform interested parties about the new extension
                    emit extensionInstantiated(extension);

                    // Store the instantiated extension
                    inProcessExtensions[binary] = qMakePair(extension, widget);
                }
            }
        }
    }
    if (!success) {
        delete object;
    }

    return success;
}

bool MApplicationExtensionManager::instantiateOutOfProcessExtension(const MApplicationExtensionMetaData &metadata)
{
    MExtensionHandle *handle = new MExtensionHandle;
    handle->init(metadata.runnerBinary(), metadata.fileName());
    outOfProcessHandles.insert(metadata.fileName(), handle);
    emit widgetCreated(handle, *extensionDataStore.data());
    return true;
}

QString MApplicationExtensionManager::dataPath() const
{
    QStringList interfaceNameSplit = interface.split("/");
    QString interfaceName = interfaceNameSplit.at(0);
    return QDir::homePath() + "/.config/" + QFileInfo(QCoreApplication::applicationFilePath()).fileName() + '/' +  interfaceName ;
}

void MApplicationExtensionManager::removeExtension(const MApplicationExtensionMetaData &metadata)
{
    if (isInProcess(metadata)) {
        removeInProcessExtension(metadata.extensionBinary());
    } else {
        removeOutOfProcessExtension(metadata);
    }
}

void MApplicationExtensionManager::removeInProcessExtension(const QString &binary)
{
    if (inProcessExtensions.contains(binary)) {
        InProcessExtensionData extension = inProcessExtensions.take(binary);
        emit extensionRemoved(extension.first);
        if (extension.second) {
            emit widgetRemoved(extension.second);
        }
        delete extension.first;
    }
}

void MApplicationExtensionManager::removeOutOfProcessExtension(const MApplicationExtensionMetaData &metadata)
{
    QString desktopFileName = metadata.fileName();
    if (outOfProcessHandles.contains(desktopFileName)) {
        MExtensionHandle *handle = outOfProcessHandles.take(desktopFileName);
        if (handle) {
            emit widgetRemoved(handle);
            handle->kill();
            delete handle;
        }
    }
}

QList<MApplicationExtensionInterface*> MApplicationExtensionManager::extensions()
{
    QList<MApplicationExtensionInterface*> extensions;

    foreach (InProcessExtensionData extension, inProcessExtensions) {
        extensions.append(extension.first);
    }

    return extensions;
}

bool MApplicationExtensionManager::createDataStore()
{
    if (!extensionDataStore.isNull()) {
        return true;
    }

    QString dataPath = this->dataPath();
    QString dataStoreFileName = dataPath + ".data";
    bool dataPathExists = QDir::root().exists(dataPath);

    // Create the user data directory if it doesn't exist yet
    if (!QFile::exists(dataStoreFileName) && !dataPathExists) {
        dataPathExists = QDir::root().mkpath(dataPath);
    }
    if (dataPathExists) {
        extensionDataStore = QSharedPointer<MFileDataStore>(new MFileDataStore(dataStoreFileName));
        return true;
    } else {
        // Cannot create the data store.
        return false;
    }
}
