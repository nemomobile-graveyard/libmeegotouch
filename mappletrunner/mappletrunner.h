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

#ifndef MAPPLETRUNNER_H
#define MAPPLETRUNNER_H

#include "mextensionrunner.h"

class MFileDataStore;
class MDataAccess;
class MGConfDataStore;
class MSettingsLanguageBinary;
class MAppletSettings;

/**
 * \ingroup appletrunner
 *
 * MAppletRunner is a class that is used to start and run oop (out of process)
 * applet widgets.
 */
class MAppletRunner : public MExtensionRunner
{
public:
    /*!
     * MAppletRunner constructor
     */
    MAppletRunner();

    //! MAppletRunner destructor
    virtual ~MAppletRunner();

    /*!
     * Initialize the applet and establish an IPC connection between the runner and
     * the host application.
     * \param serverName the name of the host process's server.
     * \param metaData Metadata of the applet.
     * \param appletId applet ID as a string
     * \param appletInstanceFileDataPath path to data file used to store instance specific data about this applet.
     * \return \c true if the applet and ipc connection was initialized correctly,
     *         \c false if something went wrong.
     */
    bool init(const QString &serverName, MAppletMetaData &metaData, const QString &appletId, const QString &appletInstanceFileDataPath);

protected:
    //! Data for the applet instance
    MFileDataStore *appletInstanceDataStore;

    //! An interface for applets to access their settings
    MAppletSettings *appletSettings;

protected slots:
    /*!
     * \brief Applet changed its icon, part of container communication API
     */
    void appletIconChanged(const QString &newIcon);

    /*!
     * \brief Applet changed its title, part of container communication API
     */
    void appletTitleChanged(const QString &newTitle);

    /*!
     * \brief Applet changed its additional text, part of container communication API
     */
    void appletTextChanged(const QString &newText);

protected:
    /*!
     * Sets the icon, title and text properties of an applet and connects any
     * related signals to slots so the changes in the properties are handled
     * correctly.
     */
    void setAppletProperties();

    //! \reimp
    virtual void teardown();
    //! \reimp_end

#ifdef UNIT_TEST
    friend class Ut_MAppletRunner;
#endif
};

#endif // MAPPLETRUNNER_H
