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

#include "mappletinstancemanager.h"
#include "mappletinstancemanagerdbusadaptor.h"
#include "mappletinstancedata.h"
#include "mdatastore.h"
#include "mappletmetadata.h"
#include "mapplethandle.h"
#include "mfiledatastore.h"
#include "mgconfdatastore.h"
#include "msubdatastore.h"
#include "mappletloader.h"
#include "mappletsettings.h"
#include "mappletpackagemetadata.h"
#include "mapplication.h"
#include "mscenemanager.h"
#include "morientationtracker.h"
#include "maction.h"
#include "mdbusinterface.h"
#include "mextensionwatcher.h"
#include <algorithm>

const QString MAppletInstanceManager::PACKAGE_MANAGER_DBUS_SERVICE = "com.nokia.package_manager";
const QString MAppletInstanceManager::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";
const char *  MAppletInstanceManager::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";

MAppletInstanceManager::MAppletInstanceManager(const QString &mashupCanvasName, MDataStore *dataStore) :
    dataStore(NULL),
    fileDataStore(NULL),
    lastAppletInstanceID(1)
{
    init(mashupCanvasName, dataStore);
}

MAppletInstanceManager::~MAppletInstanceManager()
{
    qDeleteAll(applets.values());
    delete fileDataStore;
    delete watcher;
}

void MAppletInstanceManager::init(const QString &mashupCanvasName, MDataStore *dataStore)
{
    this->applicationName = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    this->mashupCanvasName = mashupCanvasName;
    this->dataStore = dataStore;
    this->watcher = new MExtensionWatcher;

    if (dataStore == NULL) {
        // Create a file datastore if a data store was not provided
        createDataStore();
        this->dataStore = fileDataStore;
    }

    // Read applet data
    readAppletData();

    // Create a D-Bus service for this applet instance manager
    new MAppletInstanceManagerDBusAdaptor(this);

    // Connect to D-Bus and register the DBus source as an object
    QDBusConnection::sessionBus().registerObject("/MAppletInstanceManager/" + mashupCanvasName, this);

    // Connect to the system bus to receive the Package Manager signals
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationComplete", this, SLOT(operationComplete(QString, QString, QString)));
    connect(watcher, SIGNAL(extensionChanged(const MDesktopEntry &)),
            this, SLOT(updateApplet(const MDesktopEntry &)));
}

bool MAppletInstanceManager::restoreApplets()
{
    // Instantiate the applets
    bool result = true;
    foreach(MAppletInstanceData * data, applets) {
        result &= instantiateApplet(data->appletId);
    }

    return result;
}

bool MAppletInstanceManager::instantiateApplet(const QString &desktopFile)
{
    // Create applet instance data from a .desktop file
    MAppletId appletId = getUnusedAppletID();
    QMap<QString, QVariant> appletInstantiationData;
    appletInstantiationData.insert("desktopFile", desktopFile);
    MAppletInstanceData *data = createAppletInstanceDataFromInstantiationData(appletId, appletInstantiationData);
    applets.insert(appletId.instanceId(), data);
    storeData(data);

    return instantiateApplet(appletId);
}

void MAppletInstanceManager::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    // Create applet instance data from a package meta data
    MAppletId appletId = getUnusedAppletID();
    QMap<QString, QVariant> appletPackageMetaData(metaData);
    appletPackageMetaData.insert(MAppletPackageMetaData::PACKAGE_KEY, packageName);
    MAppletInstanceData *data = createAppletInstanceDataFromPackageMetaData(appletId, appletPackageMetaData);
    applets.insert(appletId.instanceId(), data);
    storeData(data);

    instantiateApplet(appletId);
}

/*!
 * A helper functor class for finding remaining applet instances
 * after removing one instance
 */
struct FindAppletByFileName
{
    QString fileName_;
    /**
     * Constructs the functor
     * \param fileName File name to compare against
     */
    explicit FindAppletByFileName(const QString& fileName)
        : fileName_(fileName){
    }
    /**
     * Compares an applet instance against the wanted file name
     * \param data Instance to compare
     */
    bool operator()(const MAppletInstanceData* data) {
        return data->desktopFile == fileName_;
    }
};

bool MAppletInstanceManager::removeApplet(MAppletId appletId)
{
    // Make sure the applet instance exists in this particular applet instance manager
    if (appletId.applicationName() == applicationName &&
            appletId.mashupCanvasName() == mashupCanvasName && applets.contains(appletId.instanceId())) {
        // Get the data for the applet
        MAppletInstanceData *data = applets.value(appletId.instanceId());
        QString fileName = data->desktopFile;
        // Let interested parties know that the applet's widget should be removed
        emit appletRemoved(data->widget);

        // Remove the applet instance data
        removeAppletInstanceData(appletId);

        if (std::find_if(applets.constBegin(), applets.constEnd(),
                         FindAppletByFileName(fileName))
            == applets.constEnd()) {
            // this was the last instance, remove from watcher
            watcher->removeExtension(data->desktopFile);
        }

        return true;
    }

    return false;
}

void MAppletInstanceManager::removeActionTriggered(bool)
{
    // Check which MAction has generated the event
    MAction *action = qobject_cast<MAction *>(sender());
    if (action != NULL) {
        // Go through the associated graphics widgets
        foreach(QGraphicsWidget *widget, action->associatedGraphicsWidgets()) {
            // If an applet is found for the widget, remove it
            MAppletId id = appletIDForWidget(widget);

            if (id != MAppletId()) {
                removeApplet(id);
                break;
            }
        }
    }
}

void MAppletInstanceManager::appletUninstalled(const QString &desktopFile)
{
    // Remove all instances of the applet that was uninstalled
    foreach(const MAppletInstanceData * data, applets.values()) {
        if (data->desktopFile == desktopFile) {
            removeApplet(data->appletId);
        }
    }
}

bool MAppletInstanceManager::instantiateApplet(MAppletId appletId)
{
    MAppletInstanceData *data = applets.value(appletId.instanceId());
    if (data != NULL) {
        // There is no applet handle for the applet yet
        QFile desktopFile(data->desktopFile);
        if ((!data->desktopFile.isEmpty() && desktopFile.exists()) || data->installationStatus == MAppletInstanceData::Installing) {
            // The desktop file exists, so the applet is installed or the applet was being installed: instantiate the applet
            QSharedPointer<const MAppletMetaData>
                metadata(new MAppletMetaData(data->desktopFile));
            if (metadata->isValid()) {
                // Check whether a runner is defined
                bool success;
                if (metadata->runnerBinary().isEmpty()) {
                    // Runner not defined: create an in-process applet
                    // IN PROCESS APPLETS ARE NOT FULLY SUPPORTED AT THE MOMENT
                    success = instantiateInProcessApplet(data, *metadata);
                } else {
                    // Runner is defined: create an out-of-process applet
                    success = instantiateOutOfProcessApplet(data, *metadata);
                }

                if (success) {
                    // Notify that the applet placeholder instantiation is done
                    emit appletInstantiated(data->widget, *data->mashupCanvasDataStore);
                    watcher->addExtension(metadata);
                    return true;
                }
            }
        } else if (!data->packageName.isEmpty()) {
            // The package name is known
            if (data->installationStatus == MAppletInstanceData::Unknown) {
                // Installation status is not known: query installation status
                queryInstallationStatus(data);
                return true;
            } else if (data->installationStatus == MAppletInstanceData::PlaceHolderRequired) {
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

void MAppletInstanceManager::updateApplet(const MDesktopEntry &updatedExtension)
{
    const MAppletMetaData* metadata =
        dynamic_cast<const MAppletMetaData*>(&updatedExtension);
    if (metadata->runnerBinary().isEmpty()) {
        // Runner not defined: create an in-process applet
        // IN PROCESS APPLETS ARE NOT FULLY SUPPORTED AT THE MOMENT
        qWarning() << "In-process applet updates are not supported at the moment";
    } else {
        AppletContainerIterator appletIterator(applets);
        while (appletIterator.hasNext()) {
            appletIterator.next();
            if (appletIterator.value()->desktopFile == metadata->fileName()) {
                MAppletHandle *handle = dynamic_cast<MAppletHandle *>(appletIterator.value()->widget);
                if ( handle )
                {
                    handle->reinit();
                }
            }
        }
    }
}

bool MAppletInstanceManager::instantiateOutOfProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata)
{
    // Create an applet handle if one doesn't exist already
    MAppletHandle *handle = dynamic_cast<MAppletHandle *>(data->widget);
    if (handle == NULL) {
        handle = new MAppletHandle;
        data->widget = handle;
    } else if (data->installationStatus != MAppletInstanceData::Installing) {
        // Applet already instantiated
        return true;
    }

    // The applet is now installed
    data->installationStatus = MAppletInstanceData::Installed;

    // Initialize the applet handle
    handle->init(metadata.runnerBinary(), createAppletInstanceDataFileName(data->appletId), data->desktopFile, data->appletId);
    handle->setAppletTitle(data->title);
    setAppletHandleSizeHints(*handle, *data);

    // Clean up if the applet ended up in a broken state
    if (handle->state() == MAppletHandleModel::BROKEN) {
        delete handle;
        data->widget = NULL;
        return false;
    }


    // Listen to changes in the applet's title
    connect(handle, SIGNAL(appletTitleChanged(QString)), SLOT(setAppletTitle(QString)));

    // Listen for removal requests
    connect(handle, SIGNAL(appletRemovalRequested(MAppletId)), this, SLOT(removeApplet(MAppletId)));

    return true;
}

// IN PROCESS APPLETS ARE NOT FULLY SUPPORTED AT THE MOMENT
bool MAppletInstanceManager::instantiateInProcessApplet(MAppletInstanceData *data, const MAppletMetaData &metadata)
{
    MFileDataStore *instanceDataStore = new MFileDataStore(data->instanceDataFilePath);
    if (instanceDataStore->isReadable() && instanceDataStore->isWritable()) {
        data->settingsDataAccess = new MGConfDataStore(QString());
        data->widget = MAppletLoader::loadApplet(metadata, *instanceDataStore, *data->settingsDataAccess);
        data->instanceDataStore = instanceDataStore;
    } else {
        qWarning() << "Couldn't create applet instance data store";
        delete instanceDataStore;
        return false;
    }

    return true;
}

void MAppletInstanceManager::instantiateAppletPlaceHolder(MAppletInstanceData *data)
{
    // Create and initialize a placeholder applet handle
    MAppletHandle *handle = new MAppletHandle;
    if (data->desktopFile.isEmpty()) {
        //~ uispec-document ??? FIXME
        //% "Could not get package meta data"
        handle->initPlaceHolder(data->appletId, data->packageName, qtTrId("mappletinstancedata_nopackagemetadata"));
    } else {
        handle->initPlaceHolder(data->appletId, data->packageName);
    }
    handle->setAppletTitle(data->title);
    setAppletHandleSizeHints(*handle, *data);

    data->widget = handle;
    data->installationStatus = MAppletInstanceData::Installing;

    // Listen for removal requests
    connect(handle, SIGNAL(appletRemovalRequested(MAppletId)), this, SLOT(removeApplet(MAppletId)));
}

void MAppletInstanceManager::queryInstallationStatus(MAppletInstanceData *data)
{
    MDBusInterface interface(MAppletInstanceManager::PACKAGE_MANAGER_DBUS_SERVICE, MAppletInstanceManager::PACKAGE_MANAGER_DBUS_PATH, MAppletInstanceManager::PACKAGE_MANAGER_DBUS_INTERFACE, QDBusConnection::systemBus());
    QDBusPendingCall call = interface.asyncCall("Operation", data->packageName);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    watcher->setProperty("appletId", data->appletId.toString());
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)), this, SLOT(receiveOperation(QDBusPendingCallWatcher *)));
}

void MAppletInstanceManager::createDataStore()
{
    QString dataPath = this->dataPath();
    QString dataStoreFileName = dataPath + mashupCanvasName + ".data";

    if (!QFile::exists(dataStoreFileName)) {
        // Copy the default datastore file as the user's datastore file if one doesn't exist yet
        QString defaultDataStoreFileName = M_XDG_DIR "/" + applicationName + '/' + mashupCanvasName + ".data";

        // Copy the default datastore only if it exists
        if (QFile::exists(defaultDataStoreFileName)) {
            // Create the user data directory if it doesn't exist yet
            if (!QDir::root().exists(dataPath)) {
                QDir::root().mkpath(dataPath);
            }

            QFile::copy(defaultDataStoreFileName, dataStoreFileName);
        }
    }

    fileDataStore = new MFileDataStore(dataStoreFileName);
}

void MAppletInstanceManager::readAppletData()
{
    // Gather all data related to each instance ID into a key-value map
    QMap<MAppletId::AppletInstanceID, QMap<QString, QVariant> * > appletInstantiationDataForAppletId;
    foreach(const QString & key, dataStore->allKeys()) {
        if (isValidKey(key)) {
            // Gather data only from valid keys
            MAppletId::AppletInstanceID instanceId = appletInstanceIDFromKey(key);

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

    foreach(MAppletId::AppletInstanceID appletId, appletInstantiationDataForAppletId.keys()) {
        // Create MAppletInstanceData objects from the key-value maps
        MAppletInstanceData *data = createAppletInstanceDataFromInstantiationData(createAppletId(appletId), *appletInstantiationDataForAppletId.value(appletId));
        applets.insert(appletId, data);
    }
}

MAppletInstanceData *MAppletInstanceManager::createAppletInstanceDataFromInstantiationData(const MAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData)
{
    MAppletInstanceData *data = new MAppletInstanceData;
    data->appletId = appletId;
    data->installationStatus = MAppletInstanceData::Unknown;
    data->mashupCanvasDataStore = new MSubDataStore(createKey(appletId, PrivateDomain), *dataStore);
    data->instanceDataFilePath = createAppletInstanceDataFileName(appletId);
    data->packageName = appletInstantiationData.value("packageName").toString();
    data->desktopFile = appletInstantiationData.value("desktopFile").toString();
    data->title = appletInstantiationData.value("title").toString();
    data->sizeLandscape = appletInstantiationData.value("sizeLandscape").toString();
    data->sizePortrait = appletInstantiationData.value("sizePortrait").toString();
    return data;
}

MAppletInstanceData *MAppletInstanceManager::createAppletInstanceDataFromPackageMetaData(const MAppletId &appletId, const QMap<QString, QVariant> &packageMetaData)
{
    MAppletInstanceData *data = new MAppletInstanceData;
    data->appletId = appletId;
    data->installationStatus = MAppletInstanceData::PlaceHolderRequired;
    data->mashupCanvasDataStore = new MSubDataStore(createKey(appletId, PrivateDomain), *dataStore);
    data->instanceDataFilePath = createAppletInstanceDataFileName(appletId);
    data->packageName = packageMetaData.value(MAppletPackageMetaData::PACKAGE_KEY).toString();
    data->desktopFile = packageMetaData.value(MAppletPackageMetaData::DESKTOP_FILE_KEY).toString();
    data->title = packageMetaData.value(MAppletPackageMetaData::APPLET_TITLE_KEY).toString();
    data->sizeLandscape = packageMetaData.value(MAppletPackageMetaData::APPLET_SIZE_LANDSCAPE_KEY).toString();
    data->sizePortrait = packageMetaData.value(MAppletPackageMetaData::APPLET_SIZE_PORTRAIT_KEY).toString();
    return data;
}

void MAppletInstanceManager::setAppletHandleSizeHints(MAppletHandle &handle, MAppletInstanceData &data)
{
    // Set the size hints of the applet. Convert the size string to a QSizeF; it should be a non-zero size
    M::OrientationAngle angle = MOrientationTracker::instance()->orientationAngle();
    QSizeF size = qStringToQSizeF((angle == M::Angle0 || angle == M::Angle180) ? data.sizeLandscape : data.sizePortrait);
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

QSizeF MAppletInstanceManager::qStringToQSizeF(const QString &string)
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

QString MAppletInstanceManager::dataPath() const
{
    return QDir::homePath() + "/.config/" + applicationName + '/';
}

QString MAppletInstanceManager::createAppletInstanceDataFileName(MAppletId id) const
{
    return dataPath() + mashupCanvasName + '/' + QString::number(id.instanceId()) + ".data";
}

void MAppletInstanceManager::freeAppletInstanceID(MAppletId id)
{
    if (id.instanceId() < lastAppletInstanceID) {
        lastAppletInstanceID = id.instanceId();
    }
}

MAppletId MAppletInstanceManager::appletIDForWidget(QGraphicsWidget *widget) const
{
    foreach(MAppletInstanceData * data, applets.values()) {
        if (data->widget == widget) {
            return data->appletId;
        }
    }
    return MAppletId();
}

MAppletId MAppletInstanceManager::appletIDForPackageName(const QString &packageName) const
{
    foreach(MAppletInstanceData * data, applets.values()) {
        if (data->packageName == packageName) {
            return data->appletId;
        }
    }
    return MAppletId();
}

MAppletId::AppletInstanceID MAppletInstanceManager::appletInstanceIDFromKey(const QString &key)
{
    // Convert the instance ID string to an integer
    return key.left(key.indexOf('/')).toUInt();
}

void MAppletInstanceManager::removeAppletInstanceData(MAppletId appletId)
{
    // Make sure the applet instance exists
    if (applets.contains(appletId.instanceId())) {
        // Remove the applet instance from the applets map
        MAppletInstanceData *data = applets.take(appletId.instanceId());

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
        MAppletSettings settings(data->desktopFile, appletId);
        settings.removeInstanceSettingValues();

        // Destroy instance data
        delete data;

        // Free the applet instance ID
        freeAppletInstanceID(appletId);
    }
}

MAppletId MAppletInstanceManager::getUnusedAppletID()
{
    // Search for an unused applet ID
    while (applets.contains(lastAppletInstanceID)) {
        lastAppletInstanceID++;
    }

    return createAppletId(lastAppletInstanceID);
}

bool MAppletInstanceManager::isValidKey(const QString &key)
{
    // The key should be in instanceID/parameterName format
    return key.count('/') == 1;
}

QString MAppletInstanceManager::getParameterName(const QString &key)
{
    // Convert the parameter name
    return key.mid(key.indexOf('/') + 1);
}

QString MAppletInstanceManager::createKey(const MAppletId &appletId, KeyType type)
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

void MAppletInstanceManager::storeData(MAppletInstanceData *data)
{
    dataStore->createValue(createKey(data->appletId, DesktopFile), data->desktopFile);
    dataStore->createValue(createKey(data->appletId, PackageName), data->packageName);
    dataStore->createValue(createKey(data->appletId, Title), data->title);
}

MAppletId MAppletInstanceManager::createAppletId(MAppletId::AppletInstanceID instanceId) const
{
    return MAppletId(applicationName, mashupCanvasName, instanceId);
}

void MAppletInstanceManager::setAppletTitle(const QString &title)
{
    // Find the widget whose title has changed
    QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(sender());
    if (widget != NULL) {
        foreach(MAppletInstanceData * data, applets.values()) {
            if (data->widget == widget) {
                // Set the title to the data and store the data
                data->title = title;
                storeData(data);
            }
        }
    }
}

void MAppletInstanceManager::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    // If the installation of a package being installed succeeds instantiate it; errors are shown by the applet handle itself
    if (operation == "Install" && error.isEmpty()) {
        MAppletId appletId = appletIDForPackageName(pkg);

        if (appletId != MAppletId()) {
            // Instantiate the applet based on the applet ID of the package
            instantiateApplet(appletId);
        }
    }
}

void MAppletInstanceManager::receiveOperation(QDBusPendingCallWatcher *watcher)
{
    if (watcher != NULL) {
        MAppletId appletId(watcher->property("appletId").toString());
        MAppletInstanceData *data = applets.value(appletId.instanceId());
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
            data->installationStatus = MAppletInstanceData::PlaceHolderRequired;
            instantiateApplet(appletId);
        }
    }
}
