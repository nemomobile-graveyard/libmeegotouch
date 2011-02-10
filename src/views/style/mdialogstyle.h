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

#ifndef MDIALOGSTYLE_H
#define MDIALOGSTYLE_H

#include <mscenewindowstyle.h>

/*!
    \class MDialogStyle
    \brief Style class for standard m dialogs.

    \code
        MDialogStyle {
            vertical-spacing: 10;
            button-spacing: 10;

            title-bar-height: 5.2mm;

            dialog-vertical-alignment: bottom;

            dialog-top-margin: 5;
            dialog-bottom-margin: 0;
            dialog-left-margin: 10;
            dialog-right-margin: 10;

            dialog-minimum-size: 1mm 1mm;
            dialog-preferred-size: 100% -1;

            button-box-orientation: horizontal;
            button-box-centered: false;

            has-close-button: true;
            has-title-bar: true;

            contents-viewport-style-name: "MDialogContentsViewport";
            button-box-style-name: "MDialogButtonBox";
            title-bar-style-name: "MDialogTItleBar";
            title-label-style-name: "CommonDialogHeader";
        }
    \endcode

    \ingroup styles
    \sa MWidgetStyle MDialog MDialogView
*/
class M_VIEWS_EXPORT MDialogStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MDialogStyle)

    /*!
        \property MDialogStyle::verticalSpacing
        \brief Vertical spacing between dialog contents and button box.
    */
    M_STYLE_ATTRIBUTE(qreal, verticalSpacing, VerticalSpacing)

    /*!
        \property MDialogStyle::titleBarHeight
        \brief Height of the titlebar.
    */
    M_STYLE_ATTRIBUTE(qreal, titleBarHeight, TitleBarHeight)

    /*!
        \property MDialogStyle::buttonSpacing
        \brief Space between buttons.
    */
    M_STYLE_ATTRIBUTE(qreal, buttonSpacing, ButtonSpacing)

    /*!
        \property MDialogStyle::alignment
        \brief Vertical alignment of the dialog.

        Only "bottom" or "center" is supported.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment, dialogVerticalAlignment, DialogVerticalAlignment)

    /*!
        \property MDialogStyle::dialogTopMargin
        \brief Top margin of the dialog.

        Empty space above dialog contents.
    */
    M_STYLE_ATTRIBUTE(qreal, dialogTopMargin, DialogTopMargin)

    /*!
        \property MDialogStyle::dialogBottomMargin
        \brief Bottom margin of the dialog.

        Empty space below dialog contents.
    */
    M_STYLE_ATTRIBUTE(qreal, dialogBottomMargin, DialogBottomMargin)

    /*!
        \property MDialogStyle::dialogLeftMargin
        \brief Left margin of the dialog.

        Empty space on the left of dialog contents.
    */
    M_STYLE_ATTRIBUTE(qreal, dialogLeftMargin, DialogLeftMargin)

    /*!
        \property MDialogStyle::dialogRightMargin
        \brief Right margin of the dialog.

        Empty space on the right of dialog contents.
    */
    M_STYLE_ATTRIBUTE(qreal, dialogRightMargin, DialogRightMargin)

    /*!
        \property MDialogStyle::dialogPreferredSize
        \brief Dialog preferred size.
    */
    M_STYLE_ATTRIBUTE(QSize, dialogPreferredSize, DialogPreferredSize)

    /*!
        \property MDialogStyle::dialogMinimumSize
        \brief Dialog minimum size.
    */
    M_STYLE_ATTRIBUTE(QSize, dialogMinimumSize,   DialogMinimumSize)

    /*!
        \property MDialogStyle::buttonBoxOrientation
        \brief Orientation of dialog button box.
    */
    M_STYLE_ATTRIBUTE(Qt::Orientation, buttonBoxOrientation, ButtonBoxOrientation)

    /*!
        \property MDialogStyle::buttonBoxCentered
        \brief Centering buttons on dialog.
    */
    M_STYLE_ATTRIBUTE(bool, buttonBoxCentered, ButtonBoxCentered)

    /*!
        \property MDialogStyle::dialogButtonFixedWidth
        \brief Fixed width of single dialog button. If set to -1 button will be sized automatically.
    */
    M_STYLE_ATTRIBUTE(qreal, dialogButtonFixedWidth, DialogButtonFixedWidth)

    /*!
        \property MDialogStyle::maximumHorizontalButtons
        \brief Maximum number of buttons aligned horizontally
    */
    M_STYLE_ATTRIBUTE(int, maximumHorizontalButtons, MaximumHorizontalButtons)

    /*!
        \property MDialogStyle::titleBarAlignment
        \brief Alignment of title bar text.
    */
    M_STYLE_ATTRIBUTE(Qt::Alignment, titleBarAlignment, TitleBarAlignment)

    /*!
        \property MDialogStyle::titleCapitalization
        \brief If true, title text will be all uppercase.
    */
    M_STYLE_ATTRIBUTE(bool, titleCapitalization, titleCapitalization)

    /*!
        \property MDialogStyle::hasTitleBar
        \brief Whether the dialog should have a title bar.

        The title bar usually sits on top of the dialog and contains a title
        text, an optional close button and an optional progress indicator.
    */
    M_STYLE_ATTRIBUTE(bool, hasTitleBar, HasTitleBar)

    /*!
        \property MDialogStyle::hasCloseButton
        \brief Whether the dialog should have a close button.

       The dialog's optional close button is provided as a convenient way of
       rejecting the dialog and discarding all changes.
    */
    M_STYLE_ATTRIBUTE(bool, hasCloseButton, HasCloseButton)
    /*!
        \property MDialogStyle::contentsViewportStyleName
        \brief Style name of the pannable viewport.
    */
    M_STYLE_ATTRIBUTE(QString, contentsViewportStyleName, ContentsViewportStyleName)
    /*!
        \property MDialogStyle::buttonBoxStyleName
        \brief Style name of the button box.
    */
    M_STYLE_ATTRIBUTE(QString, buttonBoxStyleName, ButtonBoxStyleName)
    /*!
        \property MDialogStyle::titleBarStyleName
        \brief Style name of the title bar.
    */
    M_STYLE_ATTRIBUTE(QString, titleBarStyleName, TitleBarStyleName)
    /*!
        \property MDialogStyle::titleLabelStyleName
        \brief Style name of the title bar label.
    */
    M_STYLE_ATTRIBUTE(QString, titleLabelStyleName, TitleLabelStyleName)
};

/*!
    \class MDialogStyleContainer
    \brief Style mode container class for MDialogStyle.

    \ingroup styles
    \sa MDialogStyle
*/
class M_VIEWS_EXPORT MDialogStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MDialogStyle)
};

#endif

