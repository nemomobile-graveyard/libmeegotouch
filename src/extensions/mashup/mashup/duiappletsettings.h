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

#ifndef DUIAPPLETSETTINGS_H
#define DUIAPPLETSETTINGS_H

#include <QString>
#include "duiexport.h"

class DuiSettingsLanguageBinary;
class DuiAppletId;
class DuiDataAccess;
class DuiDataStore;
class DuiAppletSettingsPrivate;

/*!
 * \class DuiAppletSettings
 * This class handles parsing of the applet instance and global settings files
 * and creation of settings binaries.
 */
class DUI_EXPORT DuiAppletSettings
{
public:
    /*!
     * Constructs a DuiAppletSettings object.
     * \param metaDataFileName Name of the applet metadata file
     * \param appletId The applet's id
     */
    DuiAppletSettings(const QString &metaDataFileName, const DuiAppletId &appletId);

    /*!
     * Constructs a DuiAppletSettings object.
     * \param metaDataFileName Name of the applet metadata file
     * \param appletId The applet's id as a string with the form
     *        applicationName/mashupCanvasName/instanceid, for example:
     *        myapp/canvas1/1
     */
    DuiAppletSettings(const QString &metaDataFileName, const QString &appletId);

    /*!
     * \brief Destructor
     */
    virtual ~DuiAppletSettings();

    /*!
     * Gets the DuiSettingsLanguageBinary object for the instance
     * settings. Returns \c NULL if the applet has no instance settings.
     * \return the applet instance settings binary.
     */
    const DuiSettingsLanguageBinary *instanceSettingsBinary() const;

    /*!
     * Gets the DuiSettingsLanguageBinary object for the global
     * settings. Returns \c NULL if the applet has no global settings.
     * \return the applet global settings binary.
     */
    const DuiSettingsLanguageBinary *globalSettingsBinary() const;

    /*!
      * Returns \c true if applet has settings (either instance or global or both), \c false otherwise.
      * \return a boolean telling whether applet has settings or not.
      */
    bool hasSettings() const;

    /*!
      * Returns the datastore for the applet instance settings.
      * Returns \c NULL if the applet has no settings.
      */
    DuiDataStore *instanceDataStore() const;

    /*!
      * Returns the datastore for the applet global settings.
      * Returns \c NULL if the applet has no settings.
      */
    DuiDataStore *globalDataStore() const;

    /*!
     * Returns a data access for the applet global and instance settings.
      * Returns \c NULL if the applet has no settings.
     */
    DuiDataAccess *dataAccess() const;

    /*!
     * Removes all applet instance settings from the storage backend. The handles
     * to the instance settings still remain in this object though. For example
     * \c instanceSettingsBinary() still returns a binary object that contains all
     * the instance settings.
     */
    void removeInstanceSettingValues() const;

protected:
    DuiAppletSettingsPrivate * const d_ptr;

private:
    Q_DECLARE_PRIVATE(DuiAppletSettings)
    Q_DISABLE_COPY(DuiAppletSettings)

    /*!
     * Called by the constructors to initialize the object. See the constructors for
     * the parameter descriptions.
     */
    void init(const QString &metaDataFileName, const QString &appletId);

    //! Creates the datastores if they are not created yet
    void createDataStoresIfNeeded() const;
    //! Deletes all the datastore instances
    void deleteDataStores() const;
};

#endif // DUIAPPLETSETTINGS_H
