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

#ifndef DUISTATUSBAR_P_H
#define DUISTATUSBAR_P_H

#include "duiscenewindow_p.h"
#include "duistatusbar.h"

//! \internal
/*!
 * Private class for the MStatusBar.
 */
class DuiStatusBarPrivate : public DuiSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(DuiStatusBar)

public:
    /*!
     * Constructs a private class for the MStatusBar.
     */
    DuiStatusBarPrivate();

    /*!
     * Destroys the MStatusBar private class.
     */
    virtual ~DuiStatusBarPrivate();

    /*!
     * Displays the status indicator menu.
     */
    void showStatusIndicatorMenu();

    //! The name of the status indicator menu D-Bus service
    static const QString STATUS_INDICATOR_MENU_DBUS_SERVICE;

    //! The name of the status indicator menu D-Bus path
    static const QString STATUS_INDICATOR_MENU_DBUS_PATH;

    //! The name of the status indicator menu D-Bus interface
    static const QString STATUS_INDICATOR_MENU_DBUS_INTERFACE;

    //! The amount of movement required to trigger the opening
    static const int SWIPE_THRESHOLD;

    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;
};

//! \internal_end
#endif
