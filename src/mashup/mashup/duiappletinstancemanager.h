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

#ifndef DUIAPPLETINSTANCEMANAGER_H_
#define DUIAPPLETINSTANCEMANAGER_H_

#include "duiappletid.h"
#include <QString>
#include <QHash>
#include <QSizeF>

class DuiWidget;
class DuiDataStore;
class DuiAppletInstanceData;
class DuiAppletMetaData;
class DuiAppletHandle;
class DuiFileDataStore;
class QDBusPendingCallWatcher;

//! \internal

/*!
 * DuiAppletInstanceManager maintains currently running applets.
 *
 * DuiAppletInstanceManager ensures that data about running applet instances is stored in a permanent storage.
 * DuiAppletInstanceManager can be used to instantiate new applets and remove existing applet instances.
 * DuiAppletInstanceManager also monitors if out-of-process applets die or become unresponsive. If this occurs
 * DuiAppletInstanceManager tries to first restart them but if that fails the applet instance is removed
 * permanently from the manager.
 *
 * Other objects can observe state of the applet instance manager by subscribing to appletInstantiated
 * and appletRemoved signals.
 *
 * DuiAppletInstanceManager uses a data store to store the information about instantiated applets and their data.
 * The data store object is either given to the DuiAppletInstanceManager in the constructor or then the
 * DuiAppletInstanceManager creates its own file-based data storing mechanism. User needs to provide a unique
 * identifier to identify a DuiAppletInstanceManager object in application's context.
 *
 * \see \ref appletdevelopment
 */
class DuiAppletInstanceManager : public QObject
{
    Q_OBJECT

public:
    //! Type for the container that holds the applet data
    typedef QHash<DuiAppletId::AppletInstanceID, DuiAppletInstanceData *> AppletContainer;
    //! An iterator type for the container that holds the applet data
    typedef QHashIterator<DuiAppletId::AppletInstanceID, DuiAppletInstanceData *> AppletContainerIterator;

    /*!
     * Constructs a DuiAppletInstanceManager.
     *
     * \param identifier An identifier to identify this DuiAppletInstanceManager in this applications context.
     * \param dataStore Instance of DuiDataStore class that is to be used as an interface
     * to store information about applet instance data to and restore the data from.
     * The ownership of dataStore is maintained by the caller. If no data store is provided, the
     * DuiAppletInstanceManager will create a default file-system based data store.
     */
    explicit DuiAppletInstanceManager(const QString &identifier, DuiDataStore *dataStore = NULL);

    /*!
     * Destroys the DuiAppletInstanceManager.
     */
    virtual ~DuiAppletInstanceManager();

    /*!
     * Restores applets from the permanent data store provided in the constructor.
     * \return true if restoration succeeded, false otherwise.
     */
    bool restoreApplets();

    /*!
     * Creates a new applet instance from the given applet metadata.
     * \param metadataFile Metadata file which specificies the applet that is to be instantiated.
     * \return true if the applet instantiation succeeded, false otherwise.
     */
    bool instantiateApplet(const QString &metadataFile);

    /*!
     * Installs an applet on the canvas from a software package. If the package
     * is not yet installed, the mashup canvas starts monitoring the installation
     * progress (but this method call returns). After the package installation is
     * done an applet instantiation takes place.
     * \param packageName the name of the package that contains the applet.
     * \param metaData meta data for the applet package.
     */
    void instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData);

signals:
    /*!
     * Signals about a new instance of an applet.
     * \param widget Pointer to DuiWidget that represents the new widget.
     * \param store Reference to a DuiDataStore object given to clients listening to this signal. Through this DuiDataStore object
     * clients can store their own data into permanent store that will be bound with the instantiated applet instance.
     */
    void appletInstantiated(DuiWidget *widget, DuiDataStore &store) const;

    /*!
     * Signals about a removal of an applet instance.
     * \param widget Pointer to DuiWidget representation about the removed applet instance.
     */
    void appletRemoved(DuiWidget *widget);

public slots:
    /*!
     * removeActionTriggered is invoked when the user selects "Remove Applet"
     * from the object menu of an applet. This will remove the applet.
     *
     * \param checked Not used. (true if the action is checked, false if the action is unchecked)
     */
    void removeActionTriggered(bool checked);

    /*!
     * \brief Called when a .desktop file that used to be present is removed
     */
    void appletUninstalled(const QString &desktopFile);

    /*!
     * Removes applet instance with the given instance ID. Does nothing if applet instance with the specified instance ID is not found.
     * \param instanceId The ID of the instance to be removed.
     */
    bool removeApplet(DuiAppletId appletId);

private slots:
    /*!
     * Slot for listening to operation complete signals from the Package Manager
     *
     * \param operation Name of the operation. Possible names are: Install, Uninstall and Upgrade
     * \param pkg Name of the package that operation is performed on
     * \param error Empty if operation was successful, DBus error name otherwise
     */
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);

    /*!
     * Sets the applet's title to a new value in the sending widget's applet data
     *
     * \param title the new title
     */
    void setAppletTitle(const QString &title);

    /*!
     * Figures out whether installation is in progress from the received
     * Operation and instantiate the applet placeholder with relevant
     * data accordingly
     *
     * \param watcher the call watcher
     */
    void receiveOperation(QDBusPendingCallWatcher *watcher);

private:
    /*!
     * Initializes the instance manager.
     *
     * \param mashupCanvasName the name of the mashup canvas that this manager is part of.
     * \param dataStore the data store to be used for storing the applet instance related data.
     */
    void init(const QString &mashupCanvasName, DuiDataStore *dataStore);

    /*!
     * Instantiates the applet with the given ID.
     *
     * \param appletId the ID of the applet to be instantiated
     * \return true if the applet instantiation succeeded, false otherwise.
     */
    bool instantiateApplet(DuiAppletId appletId);

    /*!
     * Instantiates an out of process applet from the given data.
     *
     * \param data applet instance data to instantiate applet from
     * \param metadata meta data for the applet
     * \return true if the applet instantiation succeeded, false otherwise.
     */
    bool instantiateOutOfProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata);

    /*!
     * Instantiates an in process applet from the given data.
     *
     * \param data applet instance data to instantiate applet from
     * \param metadata meta data for the applet
     * \return true if the applet instantiation succeeded, false otherwise.
     */
    bool instantiateInProcessApplet(DuiAppletInstanceData *data, const DuiAppletMetaData &metadata);

    /*!
     * Instantiates a placeholder for the applet with the given data.
     *
     * \param data applet instance data to instantiate applet placeholder from
     */
    void instantiateAppletPlaceHolder(DuiAppletInstanceData *data);

    /*!
     * Queries installation status of an applet with the given data.
     * This calls the Package Manager method "Operation" with the
     * package name of the applet. The result is expected to be
     * returned to the receiveOperation() function.
     *
     * \param data applet instance data of the applet whose installation status is to be queried
     */
    void queryInstallationStatus(DuiAppletInstanceData *data);

    /*!
     * Creates a data store for the instance manager. If the data store does
     * not already exist on disk a default data store is copied if one exists.
     */
    void createDataStore();

    /*!
     * Reads applet data from the instance manager's data store.
     */
    void readAppletData();

    /*!
     * Creates DuiAppletInstanceData objects from applet instantiation data
     * read from the data file.
     *
     * \param appletId the ID of the applet instance
     * \param appletInstantiationData key-value pairs read from a data file for an applet instance
     * \return DuiAppletInstanceData object for the data
     */
    DuiAppletInstanceData *createAppletInstanceDataFromInstantiationData(const DuiAppletId &appletId, const QMap<QString, QVariant> &appletInstantiationData);

    /*!
     * Creates DuiAppletInstanceData objects from package metadata.
     *
     * \param appletId the ID of the applet instance
     * \param packageMetaData package metadata for the applet package
     * \return DuiAppletInstanceData object for the data
     */
    DuiAppletInstanceData *createAppletInstanceDataFromPackageMetaData(const DuiAppletId &appletId, const QMap<QString, QVariant> &packageMetaData);

    /*!
     * Sets the size hints of the given applet handle as defined in the given data.
     *
     * \param handle the DuiAppletHandle whose size hints should be set
     * \param data the DuiAppletInstanceData to get the size hints from
     */
    static void setAppletHandleSizeHints(DuiAppletHandle &handle, DuiAppletInstanceData &data);

    /*!
     * Converts a size string to a QSizeF. Any non-digits are used as separators between a width and a height.
     *
     * \param string the string to be converted
     * \return the given string as a QSizeF or an empty QSizeF() if the operation fails
     */
    static QSizeF qStringToQSizeF(const QString &string);

    /*!
     * Returns a common data file path that the applet instance manager should use
     * for storing data files.
     *
     * \return data file path used by the applet instance manager
     */
    QString dataPath() const;

    /*!
     * Returns a file name for an applet instance data file. Every applet instance
     * has an own data file so the applet instance id is used to separate these.
     *
     * \param id the ID of the applet to get instance data file name for
     * \return instance data file name for the applet with the given ID
     */
    QString createAppletInstanceDataFileName(DuiAppletId id) const;

    /*!
     * Frees an applet instance ID for later use.
     *
     * \param the applet instance ID to mark unused
     */
    void freeAppletInstanceID(DuiAppletId id);

    /*!
     * Returns the Applet ID for a widget
     *
     * \return the Applet ID of the widget or an empty ID if the widget does not belong to an applet
     */
    DuiAppletId appletIDForWidget(DuiWidget *widget) const;

    /*!
     * Returns the Applet ID for a package name
     *
     * \return the Applet ID for a package name or an empty ID if the package name does not match an applet
     */
    DuiAppletId appletIDForPackageName(const QString &packageName) const;

    /*!
     * Gets the applet instance ID from a settings file key
     *
     * \param key the key to get the applet instance ID from
     * \return the applet instance ID or 0 if it is not valid
     */
    static DuiAppletId::AppletInstanceID appletInstanceIDFromKey(const QString &key);

    /*!
     * Removes all temporary data related to an applet instance.
     * This includes applet instance data and applet instance related settings.
     * After a call to this method the applet instance can't basically be restored
     * any more.
     * \param appletId the ID of the applet instance which data is to be removed.
     */
    void removeAppletInstanceData(DuiAppletId appletId);

    /*!
     * Returns an unused applet ID
     *
     * \return an unused applet ID
     */
    DuiAppletId getUnusedAppletID();

    /*!
     * Checks whether a settings file key is valid (is in instanceID/parameterName format)
     *
     * \param key the key to check for validity
     * \return true if the settings file key is in instanceID/parameterName format
     */
    static bool isValidKey(const QString &key);

    /*!
     * Gets the parameter name from a settings file key
     *
     * \param key the key to get the parameter name from
     * \return the parameter name
     */
    static QString getParameterName(const QString &key);

    /*!
     * Key types for the data stored about applet instances by the applet instance manager.
     */
    enum KeyType {
        DesktopFile,   //!< The desktop file of the applet.
        PackageName,   //!< The name of the package the applet is being installed from.
        Title,         //!< The title of the applet.
        PrivateDomain  //!< Key prefix for applet instance namespece.
    };

    /*!
     * Generates a data store key from instance id and type of the key.
     */
    static QString createKey(const DuiAppletId &appletId, KeyType type);

    /*!
     * Stores the data in the given DuiAppletInstanceData object to a permanent store.
     * This will store only the data that is permanent.
     *
     * \param data the DuiAppletInstanceData to be stored to permanent data storage.
     */
    void storeData(DuiAppletInstanceData *data);

    /*!
     * \brief Creates the DuiAppetId for the applet instance
     * \param instanceId Applet instance id
     */
    DuiAppletId createAppletId(DuiAppletId::AppletInstanceID instanceId) const;

    //! The name of the application this applet instance manager belongs to
    QString applicationName;

    //! The unique name of the mashup canvas this applet instance manager is associated to in the application's context
    QString mashupCanvasName;

    //! DuiDataStore object used to store and restore applet instance data
    DuiDataStore *dataStore;

    //! Default file data store used, if user provides no DuiDataStore.
    DuiFileDataStore *fileDataStore;

    //! A map for keeping track of running applets
    AppletContainer applets;

    //! The last applet ID that used to be free
    DuiAppletId::AppletInstanceID lastAppletInstanceID;

    //! D-Bus service of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_SERVICE;

    //! D-Bus path of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_PATH;

    //! D-Bus interface of the Package Manager
    static const QString PACKAGE_MANAGER_DBUS_INTERFACE;

    Q_DISABLE_COPY(DuiAppletInstanceManager)
};

//! \internal_end

#endif /* DUIAPPLETINSTANCEMANAGER_H_ */
