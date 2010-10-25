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

#ifndef MAPPLETSETTINGS_H
#define MAPPLETSETTINGS_H

#include <QString>
#include "mexport.h"

class MSettingsLanguageBinary;
class MAppletId;
class MDataAccess;
class MDataStore;
class MAppletSettingsPrivate;

/*!
 * \class MAppletSettings
 * This class handles parsing of the applet instance and global settings files
 * and creation of settings binaries.
 */
class M_EXTENSIONS_EXPORT MAppletSettings
{
public:
    /*!
     * Constructs a MAppletSettings object.
     * \param metaDataFileName Name of the applet metadata file
     * \param appletId The applet's id
     */
    MAppletSettings(const QString &metaDataFileName, const MAppletId &appletId);

    /*!
     * Constructs a MAppletSettings object.
     * \param metaDataFileName Name of the applet metadata file
     * \param appletId The applet's id as a string with the form
     *        applicationName/mashupCanvasName/instanceid, for example:
     *        myapp/canvas1/1
     */
    MAppletSettings(const QString &metaDataFileName, const QString &appletId);

    /*!
     * \brief Destructor
     */
    virtual ~MAppletSettings();

    /*!
     * Gets the MSettingsLanguageBinary object for the instance
     * settings. Returns \c NULL if the applet has no instance settings.
     * \return the applet instance settings binary.
     */
    const MSettingsLanguageBinary *instanceSettingsBinary() const;

    /*!
     * Gets the MSettingsLanguageBinary object for the global
     * settings. Returns \c NULL if the applet has no global settings.
     * \return the applet global settings binary.
     */
    const MSettingsLanguageBinary *globalSettingsBinary() const;

    /*!
      * Returns \c true if applet has settings (either instance or global or both), \c false otherwise.
      * \return a boolean telling whether applet has settings or not.
      */
    bool hasSettings() const;

    /*!
      * Returns the datastore for the applet instance settings.
      * Returns \c NULL if the applet has no settings.
      */
    MDataStore *instanceDataStore() const;

    /*!
      * Returns the datastore for the applet global settings.
      * Returns \c NULL if the applet has no settings.
      */
    MDataStore *globalDataStore() const;

    /*!
     * Returns a data access for the applet global and instance settings.
      * Returns \c NULL if the applet has no settings.
     */
    MDataAccess *dataAccess() const;

    /*!
     * Removes all applet instance settings from the storage backend. The handles
     * to the instance settings still remain in this object though. For example
     * \c instanceSettingsBinary() still returns a binary object that contains all
     * the instance settings.
     */
    void removeInstanceSettingValues() const;

protected:
    MAppletSettingsPrivate * const d_ptr;

private:
    Q_DECLARE_PRIVATE(MAppletSettings)
    Q_DISABLE_COPY(MAppletSettings)

    /*!
     * Called by the constructors to initialize the object. See the constructors for
     * the parameter descriptions.
     */
    void init(const QString &metaDataFileName, const QString &appletId);

    //! Creates the datastores if they are not created yet
    void createDataStoresIfNeeded() const;
    //! Deletes all the datastore instances
    void deleteDataStores() const;

#ifdef UNIT_TEST
    friend class Ut_MAppletSettingsDialog;
#endif // UNIT_TEST
};

#endif // MAPPLETSETTINGS_H
