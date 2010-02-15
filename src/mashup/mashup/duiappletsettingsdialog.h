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

#ifndef DUIAPPLETSETTINGSDIALOG_H
#define DUIAPPLETSETTINGSDIALOG_H

#include "duiexport.h"
#include "duiappletsettings.h"

class QGraphicsLayoutItem;
class QGraphicsItem;
class DuiSettingsLanguageBinary;
class DuiDataStore;

/*!
 * \class DuiAppletSettingsDialog
 * \brief DuiAppletSettingsDialog implements the applet settings dialog.
 * A dialog is constructed if an applet has any settings to show.
 */
class DUI_EXPORT DuiAppletSettingsDialog
{
public:
    /*!
     * \brief Constructs DuiAppletSettingsDialog.
     * \param appletSettings the settings object that is used for the dialog
     * \note DO NOT USE THIS CONSTRUCTOR
     */
    DuiAppletSettingsDialog(const DuiAppletSettings &appletSettings);

    /*!
     * Constructor.
     */
    DuiAppletSettingsDialog();

    /*!
     * \brief Destructor
     */
    virtual ~DuiAppletSettingsDialog();

    /*!
     * Creates the applet settings dialog showing applet instance and global settings.
     */
    void exec() const;

    /*!
     * Creates the applet settings dialog showing applet instance and global settings.
     * Uses the applet settings object received as an argument.
     * \param settings the applet settings object to use.
     */
    void exec(const DuiAppletSettings& settings) const;

private:
    /*!
     * Creates a container for showing applet instance/global settings
     * Also places the widgets into the container
     *
     * \param parent The parent of the container
     * \param title The title of the container
     * \param settingsBinaryList a list of applet settings binaries
     * \param settingsDataStore the settings data store for the settings
     */
    static QGraphicsLayoutItem *createAppletSettingsWidgets(QGraphicsItem *parent, const QString &title, const DuiSettingsLanguageBinary &settingsBinary, DuiDataStore *settingsDataStore);

    //! The applet settings object that is used to construct the dialog
    DuiAppletSettings appletSettings;
};

#endif // DUIAPPLETSETTINGSDIALOG_H
