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

#ifndef MAPPLICATIONEXTENSIONMANAGER_H
#define MAPPLICATIONEXTENSIONMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>
#include <QSharedPointer>
#include <QRegExp>
#include <QStringList>
#include "mextensionwatcher.h"

class MApplicationExtensionMetaData;
class MDataStore;
class MExtensionHandle;
class MApplicationExtensionInterface;
class MFileDataStore;
class QGraphicsWidget;

//! \internal
class MApplicationExtensionManager : public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs a MApplicationExtensionManager.
     * \param interface The interface the extensions to be instantiated by this manager should implement
     */
    MApplicationExtensionManager(const QString &interface);

    //! Destroys the MApplicationExtensionManager object
    virtual ~MApplicationExtensionManager();

    /*!
     * Sets a filter for allowing only certain extensions to be loaded in the
     * same process. The filter is a regular expression which is matched against
     * the .desktop file names of the extensions.
     *
     * Must be called before init().
     * Calling this after init() has no effect.
     *
     * \param inProcessFilter a regular expression for defining which
     *                        extensions are allowed in-process
     */
    void setInProcessFilter(const QRegExp &inProcessFilter);

    /*!
     * Sets a filter for allowing only certain extensions to be ran in separate
     * processes. The filter is a regular expression which is matched against
     * the .desktop file names of the extensions.
     *
     * Must be called before init().
     * Calling this after init() has no effect.
     *
     * \param outOfProcessFilter a regular expression for defining which
     *                           extension are allowed out-of-process
     */
    void setOutOfProcessFilter(const QRegExp &outOfProcessFilter);

    /*!
     * Defines the order in which extensions are placed if they are found. Each
     * extension is placed in the position in which it appears for the first
     * time in the list.
     *
     * Must be called before init().
     * Calling this after init() has no effect.
     *
     * \param order a list of extension .desktop file names for defining the
     *              order in which certain extensions appear
     */
    void setOrder(const QStringList &order);

    /*!
      * Initializes the application extension manager
      *
      * \return true if initialization succeeds, false otherwise
      */
    bool init();

    /*!
      * Returns a list of in process extensions loaded by this manager
      *
      * \return List of application extension interface pointers. Receving party should appropriately cast them correctly.
      */
    QList<MApplicationExtensionInterface*> extensions();

Q_SIGNALS:
    /*!
     * Signal sent when an extension has been instantiated.
     *
     * \param extension the extension that was instantiated
     */
    void extensionInstantiated(MApplicationExtensionInterface *extension);

    /*!
     * Signal that is sent when an extension has been removed.
     * \param extension the extension that was removed
     */
    void extensionRemoved(MApplicationExtensionInterface* extension);

    /*!
     * Signal sent when an extension widget is removed
     * \param widget the extension that was removed
     */
    void widgetRemoved(QGraphicsWidget* widget);

    /*!
     * Signal for instantiating an extension widget and its datastore
     * \param widget Widget to be added onto the MExtensionArea.
     * \param store This MDataStore object can be used to store permanent extension area data related
     * to this particular application extension instance.
     */
     void widgetCreated(QGraphicsWidget *widget, MDataStore &store);

private slots:
    /*!
     * \brief Slot for refreshing the list of available application extensions
     * \param path the path of the extensions
     */
    void updateAvailableExtensions(const QString &path);

    /*!
     * \brief Slot for updating an extension when the files for the extension
     * have been updated.
     * \param extensionData the extension to update
     */
    void updateExtension(const MDesktopEntry &extensionData);

private:
    //! Whether the manager has been initialized or not
    bool initialized;

    //! A file system watcher for the desktop entry file directory
    QFileSystemWatcher desktopDirectoryWatcher;

    //! An extension watcher for observing modifications to the extensions
    MExtensionWatcher extensionWatcher;
    //! The name of the interface extensions to be instantiated should implement
    QString interface;

    //! Data store for application extension manager
    QSharedPointer <MDataStore> extensionDataStore;

    //! Filter for extensions allowed to be loaded in-process
    QRegExp inProcessFilter;

    //! Filter for extensions allowed to be loaded out-of-process
    QRegExp outOfProcessFilter;

    //! The order in which the extensions should appear
    QStringList extensionOrder;

    //! The index where the extensions go when their order is unspecified
    int unorderedExtensionsIndex;

    typedef QPair<MApplicationExtensionInterface*, QGraphicsWidget*> InProcessExtensionData;
    //! In-process extensions and datastores
    QHash<const MApplicationExtensionMetaData*, InProcessExtensionData> inProcessExtensions;

    //! Keeps track how many times a given extension is in use
    QHash<MApplicationExtensionInterface*, uint> inProcessExtensionUsageCounts;

    QHash<const MApplicationExtensionMetaData*,
      QSharedPointer<MDataStore> > inProcessDataStores;

    //! Ouf-of-process extensions and datastores
    QHash<const MApplicationExtensionMetaData*, MExtensionHandle*> outOfProcessHandles;

    QHash<const MApplicationExtensionMetaData*, QSharedPointer<MDataStore> >
        outOfProcessDataStores;

    typedef QHash<QString, QSharedPointer<const MApplicationExtensionMetaData> > MetaDataEntryHash;
    /*!
     * A container for application extension desktop entries with the correct application extension interface defined.
     * This container also only contains the entries that succeeded to instantiate.
     * This maps from the desktop entry file name (without path) to the desktop entry object.
     */
    MetaDataEntryHash extensionMetaDatas;


    /*!
     * Gives the path where the application extension specific data is kept
     */
    QString dataPath() const;

    /*!
     * Determines whether the extension represented by the meta data object is an in-process
     * extension or not.
     * \param metaData the meta data object of the extension
     * \return \c true if the extension should be run in-process, \c false otherwise
     */
    static bool isInProcess(const MApplicationExtensionMetaData &metaData);

    /*!
     * Creates the datastore for application extension manager for storing the layouting information of the extensions
     */
    bool createDataStore();

    /*!
     * Creates a subdatastore for an extension
     * \param metaData Metadata for the extension
     */
    QSharedPointer<MDataStore> createSubDataStore(
        const MApplicationExtensionMetaData &metaData);
    /*!
     * Connects the signals for the directory watcher and the extension
     * watcher.
     */
    void connectSignals();
    /*!
     * Disconnects the signals for the directory watcher and the extension
     * watcher.
     */
    void disconnectSignals();

    /*!
     * Creates extension metadata and instantiates an extension
     * if it isn't already instantiated.
     * \param desktopFile Full path of the desktop file to parse.
     * \param currentExtensionsList List of current extensions.
     * Used by updateAvailableExtensions to keep track of which
     * extensions exist and which need to be removed.
     */
    void parseAndInstantiateExtension(const QString& desktopFile,
                                      QSet<QString>* currentExtensionsList);
    /*!
     * Instantiate an extension for an application extension metadata
     * \param metadata the metadata object of the extension
     * \return \c true if extension instantiated succesfully, \c false otherwise
     */
    bool instantiateExtension(
        QSharedPointer<const MApplicationExtensionMetaData> &metadata);

    /*!
     * Instantiates an in process extension.
     * \param metadata metadata for the extension
     * \return \c true if the instantiation succeeds, \c false otherwise
     */
    bool instantiateInProcessExtension(
        QSharedPointer<const MApplicationExtensionMetaData> &metadata);

    /*!
     * Instantiate out of process extensions
     * \param metadata metadata for the extension
     * \return bool True if extension instantiated, else return false
     */
    bool instantiateOutOfProcessExtension(
        QSharedPointer<const MApplicationExtensionMetaData> &metadata);

    /*!
     * Removes an extension that was defined in the desktop entry given as a parameter.
     * \param metadata metadata for the extension
     */
    void removeExtension(const MApplicationExtensionMetaData &metadata);

    /*!
     * Removes an in process extension that was loaded from the specified library.
     * \param metadata metadata for the extension
     */
    void removeInProcessExtension(const MApplicationExtensionMetaData &metadata);

    /*!
     * Removes an out of process extension that was loaded from the specified metadata
     * \param metadata for the extension
     */
    void removeOutOfProcessExtension(const MApplicationExtensionMetaData &metadata);

    /*!
     * Returns a file name for an application extension data file. Every application extension
     * has its own data file.
     *
     * \param interface The interface from which the extension needs to be instantiated by the manager
     * \return data file path of each application extension
     */
    QString createApplicationExtensionDataFileName(const QString& interface) const;

#ifdef UNIT_TEST
    friend class Ut_MApplicationExtensionManager;
#endif
};
//! \internal_end

#endif // MAPPLICATIONEXTENSIONMANAGER_H
