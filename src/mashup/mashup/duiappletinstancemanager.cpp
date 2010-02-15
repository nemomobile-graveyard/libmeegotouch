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

#include "duiappletinstancemanager.h"
#include "duiappletinstancemanagerdbusadaptor.h"
#include "duiappletinstancedata.h"
#include "duidatastore.h"
#include "duiappletmetadata.h"
#include "duiapplethandle.h"
#include "duifiledatastore.h"
#include "duigconfdatastore.h"
#include "duisubdatastore.h"
#include "duiappletloader.h"
#include "duiappletsettings.h"
#include "duiappletpackagemetadata.h"
#include "duiapplication.h"
#include "duiscenemanager.h"
#include "duiaction.h"

const QString DuiAppletInstanceManager::PACKAGE_MANAGER_DBUS_SERVICE = "com.nokia.package_manager";
const QString DuiAppletInstanceManager::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";
const QString DuiAppletInstanceManager::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";

DuiAppletInstanceManager::DuiAppletInstanceManager(const QString &mashupCanvasName, DuiDataStore *dataStore) :
    dataStore(NULL),
    fileDataStore(NULL),
    lastAppletInstanceID(1)
{
    init(mashupCanvasName, dataStore);
}

DuiAppletInstanceManager::~DuiAppletInstanceManager()
{
    qDeleteAll(applets.values());
    delete fileDataStore;
}

void DuiAppletInstanceManager::init(const QString &mashupCanvasName, DuiDataStore *dataStore)
{
    this->applicationName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    this->mashupCanvasName = mashupCanvasName;
    this->dataStore = dataStore;

    if (dataStore == NULL) {
        // Create a file datastore if a data store was not provided
        createDataStore();
        this->dataStore = fileDataStore;
    }

    // Read applet data
    readAppletData();

    // Create a D-Bus service for this applet instance manager
    new DuiAppletInstanceManagerDBusAdaptor(this);

    // Connect to D-Bus and register the DBus source as an object
    QDBusConnection::sessionBus().registerObject("/DuiAppletInstanceManager/" + mashupCanvasName, this);

    // Connect to the system bus to receive the Package Manager signals
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationComplete", this, SLOT(operationComplete(QString, QString, QString)));
}

bool DuiAppletInstanceManager::restoreApplets()
{
    // Instantiate the applets
    bool result = true;
    foreach(DuiAppletInstanceData * data, applets) {
        result &= instantiateApplet(data->appletId);
    }

    return result;
}

bool DuiAppletInstanceManager::instantiateApplet(const QString &desktopFile)
{
    // Create applet instance data from a .desktop file
    DuiAppletId appletId = getUnusedAppletID();
    QMap<QString, QVariant> appletInstantiationData;
    appletInstantiationData.insert("desktopFile", desktopFile);
    DuiAppletInstanceData *data = createAppletInstanceDataFromInstantiationData(appletId, appletInstantiationData);
    applets.insert(appletId.instanceId(), data);
    storeData(data);

    return instantiateApplet(appletId);
}

void DuiAppletInstanceManager::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    // Create applet instance data from a package meta data
    DuiAppletId appletId = getUnusedAppletID();
    QMap<QString, QVariant> appletPackageMetaData(metaData);
    appletPackageMetaData.insert(DuiAppletPackageMetaData::PACKAGE_KEY, packageName);
    DuiAppletInstanceData *data = createAppletInstanceDataFromPackageMetaData(appletId, appletPackageMetaData);
    applets.insert(appletId.instanceId(), data);
    storeData(data);

    instantiateApplet(appletId);
}

bool DuiAppletInstanceManager::removeApplet(DuiAppletId appletId)
{
    // Make sure the applet instance exists in this particular applet instance manager
    if (appletId.applicationName() == applicationName &&
            appletId.mashupCanvasName() == mashupCanvasName && applets.contains(appletId.instanceId())) {
        // Get the data for the applet
        DuiAppletInstanceData *data = applets.value(appletId.instanceId());

        // Let interested parties know that the applet's widget should be removed
        emit appletRemoved(data->widget);

        // Remove the applet instance data
        removeAppletInstanceData(appletId);
        return true;
    }

    return false;
}

void DuiAppletInstanceManager::removeActionTriggered(bool)
{
    // Check which DuiAction has generated the event
    DuiAction *action = qobject_cast<DuiAction *>(sender());
    if (action != NULL) {
        // Go through the associated graphics widgets
        foreach(QGraphicsWidget * w, action->associatedGraphicsWidgets()) {
            // They're expected to be DUIWidgets
            DuiWidget *widget = qobject_cast<DuiWidget *>(w);
            if (widget != NULL) {
                // If an applet is found for the widget, remove it
                DuiAppletId id = appletIDForWidget(widget);

                if (id != DuiAppletId()) {
                    removeApplet(id);
                    break;
                }
            }
        }
    }
}

void DuiAppletInstanceManager::appletUninstalled(const QString &desktopFile)
{
    // Remove all instances of the applet that was uninstalled
    foreach(const DuiAppletInstanceData * data, applets.values()) {
        if (data->desktopFile == desktopFile) {
            removeApplet(data->appletId);
        }
    }
}

bool DuiAppletInstanceManager::instantiateApplet(DuiAppletId appletId)
{
    DuiAppletInstanceData *data = applets.value(appletId.instanceId());
    if (data != NULL) {
        // There is no applet handle for the applet yet
        QFile desktopFile(data->desktopFile);
        if ((!data->desktopFile.isEmpty() && desktopFile.exists()) || data->installationStatus == DuiAppletInstanceData::Installing) {
            // The desktop file exists, so the applet is installed or the applet was being installed: instantiate the applet
            DuiAppletMetaData metadata(data->desktopFile);
            if (metadata.isValid()) {
                // Check whether a runner is defined
                bool success;
                if (metadata.runnerBinary().isEmpty()) {
                    // Runner not defined: create an in-process applet
                    success = instantiateInProcessApplet(data, metadata);
                } else {
                    // Runner is defined: create an out-of-process applet
                    success = instantiateOutOfProcessApplet(data, metadata);
                }

                if (success) {
                    // Notify that the applet placeholder instantiation is done
                    emit appletInstantiated(data->widget, *data->mashupCanvasDataStore);
                    return true;
                }
            }
        } else if (!data->packageName.isEmpty()) {
            // The package name is known
            if (data->installationStatus == DuiAppletInstanceData::Unknown) {
                // Installation status is not known: query installation status
                queryInstallationStatus(data);
                return true;
            } else if (data->installationStatus == DuiAppletInstanceData::PlaceHolderRequired) {
                // Placeholder is required: instantiate one and notify about it
                instantiateAppletPlaceHolder(data);
                emit appletInstantiated(data->widget, *data->mashupCanvasDataStore);
                return true;
            }
        }

        // Instantiation failed: Remove related data
        removeAppletInstanceData(appletId);
    }

    return false;
}

bool DuiAppletInstanceManager::instantiateOutOfProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata)
{
    // Create an applet handle if one doesn't exist already
    DuiAppletHandle *handle = dynamic_cast<DuiAppletHandle *>(data->widget);
    if (handle == NULL) {
        handle = new DuiAppletHandle;
        data->widget = handle;
    } else if (data->installationStatus != DuiAppletInstanceData::Installing) {
        // Applet already instantiated
        return true;
    }

    // The applet is now installed
    data->installationStatus = DuiAppletInstanceData::Installed;

    // Initialize the applet handle
    handle->init(metadata.runnerBinary(), createAppletInstanceDataFileName(data->appletId), data->desktopFile, data->appletId);
    handle->setAppletTitle(data->title);
    setAppletHandleSizeHints(*handle, *data);

    // Clean up if the applet ended up in a broken state
    if (handle->state() == DuiAppletHandleModel::BROKEN) {
        delete handle;
        data->widget = NULL;
        return false;
    }


    // Listen to changes in the applet's title
    connect(handle, SIGNAL(appletTitleChanged(QString)), SLOT(setAppletTitle(QString)));

    // Listen for removal requests
    connect(handle, SIGNAL(appletRemovalRequested(DuiAppletId)), this, SLOT(removeApplet(DuiAppletId)));

    return true;
}

bool DuiAppletInstanceManager::instantiateInProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata)
{
    DuiFileDataStore *instanceDataStore = new DuiFileDataStore(data->instanceDataFilePath);
    if (instanceDataStore->isReadable() && instanceDataStore->isWritable()) {
        data->settingsDataAccess = new DuiGConfDataStore(QString());
        data->widget = DuiAppletLoader::loadApplet(metadata, *instanceDataStore, *data->settingsDataAccess);
        data->instanceDataStore = instanceDataStore;
    } else {
        qWarning() << "Couldn't create applet instance data store";
        delete instanceDataStore;
        return false;
    }

    return true;
}

void DuiAppletInstanceManager::instantiateAppletPlaceHolder(DuiAppletInstanceData *data)
{
    // Create and initialize a placeholder applet handle
    DuiAppletHandle *handle = new DuiAppletHandle;
    if (data->desktopFile.isEmpty()) {
        //~ uispec-document ??? FIXME
        //% "Could not get package meta data"
        handle->initPlaceHolder(data->appletId, data->packageName, qtTrId("duiappletinstancedata_nopackagemetadata"));
    } else {
        handle->initPlaceHolder(data->appletId, data->packageName);
    }
    handle->setAppletTitle(data->title);
    setAppletHandleSizeHints(*handle, *data);

    data->widget = handle;
    data->installationStatus = DuiAppletInstanceData::Installing;

    // Listen for removal requests
    connect(handle, SIGNAL(appletRemovalRequested(DuiAppletId)), this, SLOT(removeApplet(DuiAppletId)));
}

void DuiAppletInstanceManager::queryInstallationStatus(DuiAppletInstanceData *data)
{
    QDBusInterface interface(DuiAppletInstanceManager::PACKAGE_MANAGER_DBUS_SERVICE, DuiAppletInstanceManager::PACKAGE_MANAGER_DBUS_PATH, DuiAppletInstanceManager::PACKAGE_MANAGER_DBUS_INTERFACE, QDBusConnection::systemBus());
    QDBusPendingCall call = interface.asyncCall("Operation", data->packageName);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    watcher->setProperty("appletId", data->appletId.toString());
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), this, SLOT(receiveOperation(QDBusPendingCallWatcher *)));
}

void DuiAppletInstanceManager::createDataStore()
{
    QString dataPath = this->dataPath();
    QString dataStoreFileName = dataPath + mashupCanvasName + ".data";

    if (!QFile::exists(dataStoreFileName)) {
        // Copy the default datastore file as the user's datastore file if one doesn't exist yet
        QString defaultDataStoreFileName = DUI_XDG_DIR "/" + applicationName + '/' + mashupCanvasName + ".data";

        // Copy the default datastore only if it exists
        if (QFile::exists(defaultDataStoreFileName)) {
            // Create the user data directory if it doesn't exist yet
            if (!QDir::root().exists(dataPath)) {
                QDir::root().mkpath(dataPath);
            }

            QFile::copy(defaultDataStoreFileName, dataStoreFileName);
        }
    }

    fileDataStore = new DuiFileDataStore(dataStoreFileName);
}

void DuiAppletInstanceManager::readAppletData()
{
    // Gather all data related to each instance ID into a key-value map
    QMap<DuiAppletId::AppletInstanceID, QMap<QString, QVariant> * > appletInstantiationDataForAppletId;
    foreach(const QString & key, dataStore->allKeys()) {
        if (isValidKey(key)) {
            // Gather data only from valid keys
            DuiAppletId::AppletInstanceID instanceId = appletInstanceIDFromKey(key);

            if (instanceId > 0) {
                // Check if the key is supported
                const static QStringList supportedParameters = (QStringList() << "desktopFile" << "packageName" << "title" << "sizeLandscape" << "sizePortrait");
                QString parameterName = getParameterName(key);
                if (!supportedParameters.contains(parameterName)) {
                    qWarning() << "Invalid key" << key;
                    continue;
                }

                // Find out whether there is already some data for this instance ID
                QMap<QString, QVariant> *appletInstantiationData = appletInstantiationDataForAppletId.value(instanceId);
                if (appletInstantiationData == NULL) {
                    appletInstantiationData = new QMap<QString, QVariant>;
                    appletInstantiationDataForAppletId.insert(instanceId, appletInstantiationData);
                }

                // Update the data
                appletInstantiationData->insert(getParameterName(key), dataStore->value(key).toString());
            }
        }
    }

    foreach(DuiAppletId::AppletInstanceID appletId, appletInstantiationDataForAppletId.keys()) {
        // Create DuiAppletInstanceData objects from the key-value maps
        DuiAppletInstanceData *data = createAppletInstanceDataFromInstantiationData(createAppletId(appletId), *appletInstantiationDataForAppletId.value(appletId));
        applets.insert(appletId, data);
    }
}

DuiAppletInstanceData *DuiAppletInstanceManager::createAppletInstanceDataFromInstantiationData(const DuiAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData)
{
    DuiAppletInstanceData *data = new DuiAppletInstanceData;
    data->appletId = appletId;
    data->installationStatus = DuiAppletInstanceData::Unknown;
    data->mashupCanvasDataStore = new DuiSubDataStore(createKey(appletId, PrivateDomain), *dataStore);
    data->instanceDataFilePath = createAppletInstanceDataFileName(appletId);
    data->packageName = appletInstantiationData.value("packageName").toString();
    data->desktopFile = appletInstantiationData.value("desktopFile").toString();
    data->title = appletInstantiationData.value("title").toString();
    data->sizeLandscape = appletInstantiationData.value("sizeLandscape").toString();
    data->sizePortrait = appletInstantiationData.value("sizePortrait").toString();
    return data;
}

DuiAppletInstanceData *DuiAppletInstanceManager::createAppletInstanceDataFromPackageMetaData(const DuiAppletId &appletId, const QMap<QString, QVariant> &packageMetaData)
{
    DuiAppletInstanceData *data = new DuiAppletInstanceData;
    data->appletId = appletId;
    data->installationStatus = DuiAppletInstanceData::PlaceHolderRequired;
    data->mashupCanvasDataStore = new DuiSubDataStore(createKey(appletId, PrivateDomain), *dataStore);
    data->instanceDataFilePath = createAppletInstanceDataFileName(appletId);
    data->packageName = packageMetaData.value(DuiAppletPackageMetaData::PACKAGE_KEY).toString();
    data->desktopFile = packageMetaData.value(DuiAppletPackageMetaData::DESKTOP_FILE_KEY).toString();
    data->title = packageMetaData.value(DuiAppletPackageMetaData::APPLET_TITLE_KEY).toString();
    data->sizeLandscape = packageMetaData.value(DuiAppletPackageMetaData::APPLET_SIZE_LANDSCAPE_KEY).toString();
    data->sizePortrait = packageMetaData.value(DuiAppletPackageMetaData::APPLET_SIZE_PORTRAIT_KEY).toString();
    return data;
}

void DuiAppletInstanceManager::setAppletHandleSizeHints(DuiAppletHandle &handle, DuiAppletInstanceData &data)
{
    // Set the size hints of the applet
    DuiWindow *window = DuiApplication::activeWindow();
    if (window != NULL) {
        DuiSceneManager *sceneManager = window->sceneManager();
        if (sceneManager != NULL) {
            // Convert the size string to a QSizeF; it should be a non-zero size
            QSizeF size = qStringToQSizeF(sceneManager->orientation() == Dui::Landscape ? data.sizeLandscape : data.sizePortrait);
            if (!size.isEmpty()) {
                // Set all size hints to the given size in the current orientation
                QVector<QSizeF> sizeHints;
                sizeHints.append(size);
                sizeHints.append(size);
                sizeHints.append(size);
                sizeHints.append(size);
                handle.setSizeHints(sizeHints);
            }
        }
    }
}

QSizeF DuiAppletInstanceManager::qStringToQSizeF(const QString &string)
{
    // Use any non-digits as the separator
    QStringList list = string.split(QRegExp("\\D+"), QString::SkipEmptyParts);
    if (list.length() == 2) {
        // There should be a valid width and a valid height
        bool widthOk, heightOk;
        qreal width = list.at(0).toFloat(&widthOk);
        qreal height = list.at(1).toFloat(&heightOk);
        if (widthOk && heightOk) {
            return QSizeF(width, height);
        }
    }

    // Return an empty size on failure
    return QSizeF();
}

QString DuiAppletInstanceManager::dataPath() const
{
    return QDir::homePath() + "/.config/" + applicationName + '/';
}

QString DuiAppletInstanceManager::createAppletInstanceDataFileName(DuiAppletId id) const
{
    return dataPath() + mashupCanvasName + '/' + QString::number(id.instanceId()) + ".data";
}

void DuiAppletInstanceManager::freeAppletInstanceID(DuiAppletId id)
{
    if (id.instanceId() < lastAppletInstanceID) {
        lastAppletInstanceID = id.instanceId();
    }
}

DuiAppletId DuiAppletInstanceManager::appletIDForWidget(DuiWidget *widget) const
{
    foreach(DuiAppletInstanceData * data, applets.values()) {
        if (data->widget == widget) {
            return data->appletId;
        }
    }
    return DuiAppletId();
}

DuiAppletId DuiAppletInstanceManager::appletIDForPackageName(const QString &packageName) const
{
    foreach(DuiAppletInstanceData * data, applets.values()) {
        if (data->packageName == packageName) {
            return data->appletId;
        }
    }
    return DuiAppletId();
}

DuiAppletId::AppletInstanceID DuiAppletInstanceManager::appletInstanceIDFromKey(const QString &key)
{
    // Convert the instance ID string to an integer
    return key.left(key.indexOf('/')).toUInt();
}

void DuiAppletInstanceManager::removeAppletInstanceData(DuiAppletId appletId)
{
    // Make sure the applet instance exists
    if (applets.contains(appletId.instanceId())) {
        // Remove the applet instance from the applets map
        DuiAppletInstanceData *data = applets.take(appletId.instanceId());

        // Remove all keys from the data store that are related to the instance we are removing
        QStringList keys(dataStore->allKeys());
        foreach(const QString & key, keys) {
            if (appletInstanceIDFromKey(key) == appletId.instanceId()) {
                dataStore->remove(key);
            }
        }

        // Remove the instance data file
        QFile::remove(data->instanceDataFilePath);

        // Remove applet instance related settings
        DuiAppletSettings settings(data->desktopFile, appletId);
        settings.removeInstanceSettingValues();

        // Destroy instance data
        delete data;

        // Free the applet instance ID
        freeAppletInstanceID(appletId);
    }
}

DuiAppletId DuiAppletInstanceManager::getUnusedAppletID()
{
    // Search for an unused applet ID
    while (applets.contains(lastAppletInstanceID)) {
        lastAppletInstanceID++;
    }

    return createAppletId(lastAppletInstanceID);
}

bool DuiAppletInstanceManager::isValidKey(const QString &key)
{
    // The key should be in instanceID/parameterName format
    return key.count('/') == 1;
}

QString DuiAppletInstanceManager::getParameterName(const QString &key)
{
    // Convert the parameter name
    return key.mid(key.indexOf('/') + 1);
}

QString DuiAppletInstanceManager::createKey(const DuiAppletId &appletId, KeyType type)
{
    QString ret;

    switch (type) {
    case DesktopFile:
        ret.sprintf("%d/desktopFile", appletId.instanceId());
        break;
    case PackageName:
        ret.sprintf("%d/packageName", appletId.instanceId());
        break;
    case Title:
        ret.sprintf("%d/title", appletId.instanceId());
        break;
    case PrivateDomain:
        ret.sprintf("%d/private/", appletId.instanceId());
        break;
    }

    return ret;
}

void DuiAppletInstanceManager::storeData(DuiAppletInstanceData *data)
{
    dataStore->createValue(createKey(data->appletId, DesktopFile), data->desktopFile);
    dataStore->createValue(createKey(data->appletId, PackageName), data->packageName);
    dataStore->createValue(createKey(data->appletId, Title), data->title);
}

DuiAppletId DuiAppletInstanceManager::createAppletId(DuiAppletId::AppletInstanceID instanceId) const
{
    return DuiAppletId(applicationName, mashupCanvasName, instanceId);
}

void DuiAppletInstanceManager::setAppletTitle(const QString &title)
{
    // Find the widget whose title has changed
    DuiWidget *widget = dynamic_cast<DuiWidget *>(sender());
    if (widget != NULL) {
        foreach(DuiAppletInstanceData * data, applets.values()) {
            if (data->widget == widget) {
                // Set the title to the data and store the data
                data->title = title;
                storeData(data);
            }
        }
    }
}

void DuiAppletInstanceManager::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    // If the installation of a package being installed succeeds instantiate it; errors are shown by the applet handle itself
    if (operation == "Install" && error.isEmpty()) {
        DuiAppletId appletId = appletIDForPackageName(pkg);

        if (appletId != DuiAppletId()) {
            // Instantiate the applet based on the applet ID of the package
            instantiateApplet(appletId);
        }
    }
}

void DuiAppletInstanceManager::receiveOperation(QDBusPendingCallWatcher *watcher)
{
    if (watcher != NULL) {
        DuiAppletId appletId(watcher->property("appletId").toString());
        DuiAppletInstanceData *data = applets.value(appletId.instanceId());
        if (data != NULL) {
            // Figure out whether installation is in progress
            bool installationInProgress = false;
            QDBusPendingReply<QString> reply = *watcher;
            if (!reply.isError()) {
                installationInProgress = reply.argumentAt<0>() == "Install";
            }

            if (!installationInProgress) {
                // Installation is not in progress: cause the placeholder to go to BROKEN state
                data->desktopFile = QString();
            }

            // Instantiate the applet
            data->installationStatus = DuiAppletInstanceData::PlaceHolderRequired;
            instantiateApplet(appletId);
        }
    }
}
