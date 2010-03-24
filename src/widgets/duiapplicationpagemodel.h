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

#ifndef DUIAPPLICATIONPAGEMODEL_H
#define DUIAPPLICATIONPAGEMODEL_H

#include <duiscenewindowmodel.h>

class DUI_EXPORT DuiApplicationPageModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiApplicationPageModel)

public:
    /*!
        This enum specifies the display mode of a component.
     */
    enum ComponentDisplayMode {
        Show,
        AutoHide,
        Hide
    };
    /*!
        \var ComponentDisplayMode Show
        Component is shown

        \var ComponentDisplayMode AutoHide
        Component is automatically hidden whenever the user stays
        for longer than a few seconds without interacting with the GUI. In other
        words: It's hidden but reappears temporarily every time the user interacts
        with the GUI.
        The exact amount of idle time needed to make the component disappear
        is defined by the system.

        \var ComponentDisplayMode Hide
        Component is hidden
     */

    /*!
        This enum specifies the page's escape mode.
        \sa DuiSceneManager::pageHistoryStack()
     */
    enum PageEscapeMode {
        EscapeAuto, /*!< Default value. Escape button will automatically be set
                         to either back or close according to the page navigation
                         history.

                         If the page history is not empty, the escape button
                         will be set to back mode and its clicked signal connected
                         to the page's dismiss() slot.

                         If the page happens to be the root (page history is empty),
                         escape button will be set to close mode. */

        EscapeManualBack, /*!< Escape button will be set so back mode. All that
                             happens when it gets clicked is the emission of the
                             backButtonClicked() signal. It's up to the application
                             developer to connect that signal to some useful slot. */

        EscapeCloseWindow /*!< Escape button will be set to close mode. Clicking
                               it will cause the application window to be closed. */
    };

private:
    DUI_MODEL_PROPERTY(DuiApplicationPageModel::ComponentDisplayMode, homeButtonDisplayMode, HomeButtonDisplayMode, true, DuiApplicationPageModel::Show)

    DUI_MODEL_PROPERTY(DuiApplicationPageModel::ComponentDisplayMode, escapeButtonDisplayMode, EscapeButtonDisplayMode, true, DuiApplicationPageModel::Show)

    DUI_MODEL_PROPERTY(DuiApplicationPageModel::ComponentDisplayMode, navigationBarDisplayMode, NavigationBarDisplayMode, true, DuiApplicationPageModel::Show)

    /*!
     * \property DuiApplicationPageModel::escapeMode
     * \brief Page's escape mode.
     * \sa DuiApplicationPageMode::EscapeMode
     */
    DUI_MODEL_PROPERTY(DuiApplicationPageModel::PageEscapeMode, escapeMode, EscapeMode, true, DuiApplicationPageModel::EscapeAuto)

    /*!
        \property DuiApplicationPageModel::progressIndicatorVisible
        \brief Whether the progress indicator is visible.
    */
    DUI_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property DuiApplicationPageModel::marginsForComponentsEnabled
        \brief Whether extra margins will be automatically added around the central
               widget to avoid occlusion by other components.

        If enabled, extra margins will be automatically added around the central
        widget according to the components that are currently being displayed in
        order to avoid parts of the central widget from being inaccessible.

        When a navigation bar appears on the upper edge of the application window, for
        instance, an extra margin will be automatically added to the top of the
        central widget of roughly the height of that navigation bar. Therefore
        effectively avoiding the top part of the central widget from being
        inaccessible.

        It's enabled by default.
    */
    DUI_MODEL_PROPERTY(bool, autoMarginsForComponentsEnabled, AutoMarginsForComponentsEnabled, true, true)

    /*!
        \property DuiApplicationPageModel::title
        \brief Title for a page.

        Title appears on the navigation bar.
     */
    DUI_MODEL_PROPERTY(QString, title, Title, true, QString())
};

#endif

