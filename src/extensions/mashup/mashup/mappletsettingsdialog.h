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

#ifndef MAPPLETSETTINGSDIALOG_H
#define MAPPLETSETTINGSDIALOG_H

#include "mexport.h"
#include "mappletsettings.h"

/*!
 * \class MAppletSettingsDialog
 * \brief MAppletSettingsDialog implements the applet settings dialog.
 * A dialog is constructed if an applet has any settings to show.
 */
class M_EXTENSIONS_EXPORT MAppletSettingsDialog
{
public:
    /*!
     * Creates the applet settings dialog showing applet instance and global settings.
     * Uses the applet settings object received as an argument.
     * \param settings the applet settings object to use.
     */
    static void exec(const MAppletSettings& settings);

private:
    /*!
     * Do not instantiate this class - use the static \ref exec(const
     * MAppletSettings &) function instead.
     */
    MAppletSettingsDialog();
    ~MAppletSettingsDialog();
};

#endif // MAPPLETSETTINGSDIALOG_H
