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

/*!
 * \class DuiAppletSettingsDialog
 * \brief DuiAppletSettingsDialog implements the applet settings dialog.
 * A dialog is constructed if an applet has any settings to show.
 */
class DUI_EXPORT DuiAppletSettingsDialog
{
public:
    /*!
     * Creates the applet settings dialog showing applet instance and global settings.
     * Uses the applet settings object received as an argument.
     * \param settings the applet settings object to use.
     */
    static void exec(const DuiAppletSettings& settings);

private:
    /*!
     * Do not instantiate this class - use the static \ref exec(const
     * DuiAppletSettings &) function instead.
     */
    DuiAppletSettingsDialog();
    ~DuiAppletSettingsDialog();
};

#endif // DUIAPPLETSETTINGSDIALOG_H
