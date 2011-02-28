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

#ifndef MAPPLICATIONWINDOWSTYLE_H
#define MAPPLICATIONWINDOWSTYLE_H

#include <mstyle.h>

#include <QColor>

/** \brief Defines a style for a MApplicationWindowStyle class.
 *  This class defines default style values for application window.
 */
class MApplicationWindowStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MApplicationWindowStyle)

    /*!
     \brief A time threshold which is used as a delay for
     autohide of Navigation Bar.
     */
    M_STYLE_ATTRIBUTE(int, autoHideTimeout, AutoHideTimeout)

    /*!
      \brief Whether the tool bar prefers to be displayed as a separate, independent, scene window.
    */
    M_STYLE_ATTRIBUTE(bool, floatableToolBar, FloatableToolBar)

    /*!
      \brief Whether the tab bar prefers to be displayed as a separate, independent, scene window.
    */
    M_STYLE_ATTRIBUTE(bool, floatableTabBar, FloatableTabBar)

    /*!
      \brief This allows to customize the style of navigation bar for a specific app
    */
    M_STYLE_ATTRIBUTE(QString, navigationBarStyleName, NavigationBarStyleName)

    /*!
      \brief This allows to customize the style of the toolbar for a specific app
    */
    M_STYLE_ATTRIBUTE(QString, toolBarStyleName, ToolBarStyleName)

    /*!
      \brief This allows to customize the style of the toolbar tabs for a specific app
    */
    M_STYLE_ATTRIBUTE(QString, tabBarStyleName, TabBarStyleName)

    /*!
      \brief This allows to customize the style of the status bar for a specific app
    */
    M_STYLE_ATTRIBUTE(QString, statusBarStyleName, StatusBarStyleName)

    /*!
      \brief This allows to customize application window background color.
    */
    M_STYLE_ATTRIBUTE(QColor, backgroundColor, BackgroundColor)
};

class MApplicationWindowStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MApplicationWindowStyle)
};

#endif // MAPPLICATIONWINDOWSTYLE_H
