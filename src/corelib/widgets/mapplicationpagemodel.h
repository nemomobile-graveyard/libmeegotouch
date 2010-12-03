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

#ifndef MAPPLICATIONPAGEMODEL_H
#define MAPPLICATIONPAGEMODEL_H

#include <mscenewindowmodel.h>

class M_CORE_EXPORT MApplicationPageModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MApplicationPageModel)

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
        \sa MSceneManager::pageHistory()
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
    M_MODEL_PROPERTY(MApplicationPageModel::ComponentDisplayMode, homeButtonDisplayMode, HomeButtonDisplayMode, true, MApplicationPageModel::Show)

    M_MODEL_PROPERTY(MApplicationPageModel::ComponentDisplayMode, escapeButtonDisplayMode, EscapeButtonDisplayMode, true, MApplicationPageModel::Show)

    M_MODEL_PROPERTY(MApplicationPageModel::ComponentDisplayMode, navigationBarDisplayMode, NavigationBarDisplayMode, true, MApplicationPageModel::Show)

    /*!
     * \property MApplicationPageModel::escapeMode
     * \brief Page's escape mode.
     * \sa MApplicationPageMode::EscapeMode
     */
    M_MODEL_PROPERTY(MApplicationPageModel::PageEscapeMode, escapeMode, EscapeMode, true, MApplicationPageModel::EscapeAuto)

    /*!
        \deprecated do not use
        \property MApplicationPageModel::progressIndicatorVisible
        \brief Whether the progress indicator is visible.
    */
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property MApplicationPageModel::marginsForComponentsEnabled
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
    M_MODEL_PROPERTY(bool, autoMarginsForComponentsEnabled, AutoMarginsForComponentsEnabled, true, true)

    /*!
        \property MApplicationPageModel::rememberPosition
        \brief Whether the page should remember the pannable viewport position it had
               on its last appearance.
     */
    M_MODEL_PROPERTY(bool, rememberPosition, RememberPosition, true, true)

    /*!
        \property MApplicationPageModel::exposedContentRect
        \brief Area of the page whose content is not covered by any bar
     */
    M_MODEL_PROPERTY(QRectF, exposedContentRect, ExposedContentRect, true, QRectF())

    /*!
        \property MApplicationPageModel::title
        \brief Title for a page.

        Title appears on the navigation bar.
     */
    M_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property MApplicationPageModel::centralWidget
        \brief Central widget for the applicationPage.
    */
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, centralWidget, CentralWidget, true, new MWidget)
};

#endif

