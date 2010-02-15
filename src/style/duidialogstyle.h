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

#ifndef DUIDIALOGSTYLE_H
#define DUIDIALOGSTYLE_H

#include <duiscenewindowstyle.h>

/*!
    \class DuiDialogStyle
    \brief Style class for standard dui dialogs.

    \code
        DuiDialogStyle {
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
        }
    \endcode

    \ingroup styles
    \sa DuiWidgetStyle DuiDialog DuiDialogView
*/
class DUI_EXPORT DuiDialogStyle : public DuiSceneWindowStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiDialogStyle)

    /*!
        \property DuiDialogStyle::verticalSpacing
        \brief Vertical spacing between dialog contents and button box.
    */
    DUI_STYLE_ATTRIBUTE(qreal, verticalSpacing, VerticalSpacing)

    /*!
        \property DuiDialogStyle::titleBarHeight
        \brief Height of the titlebar.
    */
    DUI_STYLE_ATTRIBUTE(qreal, titleBarHeight, TitleBarHeight)

    /*!
        \property DuiDialogStyle::buttonSpacing
        \brief Space between buttons.
    */
    DUI_STYLE_ATTRIBUTE(qreal, buttonSpacing, ButtonSpacing)

    /*!
        \property DuiDialogStyle::alignment
        \brief Vertical alignment of the dialog.

        Only "bottom" or "center" is supported.
    */
    DUI_STYLE_ATTRIBUTE(Qt::Alignment, dialogVerticalAlignment, DialogVerticalAlignment)

    /*!
        \property DuiDialogStyle::dialogTopMargin
        \brief Top margin of the dialog.

        Empty space above dialog contents.
    */
    DUI_STYLE_ATTRIBUTE(qreal, dialogTopMargin, DialogTopMargin)

    /*!
        \property DuiDialogStyle::dialogBottomMargin
        \brief Bottom margin of the dialog.

        Empty space below dialog contents.
    */
    DUI_STYLE_ATTRIBUTE(qreal, dialogBottomMargin, DialogBottomMargin)

    /*!
        \property DuiDialogStyle::dialogLeftMargin
        \brief Left margin of the dialog.

        Empty space on the left of dialog contents.
    */
    DUI_STYLE_ATTRIBUTE(qreal, dialogLeftMargin, DialogLeftMargin)

    /*!
        \property DuiDialogStyle::dialogRightMargin
        \brief Right margin of the dialog.

        Empty space on the right of dialog contents.
    */
    DUI_STYLE_ATTRIBUTE(qreal, dialogRightMargin, DialogRightMargin)

    /*!
        \property DuiDialogStyle::dialogPreferredSize
        \brief Dialog preffered size.
    */
    DUI_STYLE_ATTRIBUTE(QSize, dialogPreferredSize, DialogPreferredSize)

    /*!
        \property DuiDialogStyle::dialogMinimumSize
        \brief Dialog minimum size.
    */
    DUI_STYLE_ATTRIBUTE(QSize, dialogMinimumSize,   DialogMinimumSize)

    /*!
        \property DuiDialogStyle::buttonBoxOrientation
        \brief Orientation of dialog button box.
    */
    DUI_STYLE_ATTRIBUTE(Qt::Orientation, buttonBoxOrientation, ButtonBoxOrientation)
};

/*!
    \class DuiDialogStyleContainer
    \brief Style mode container class for DuiDialogStyle.

    \ingroup styles
    \sa DuiDialogStyle
*/
class DUI_EXPORT DuiDialogStyleContainer : public DuiSceneWindowStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiDialogStyle)
};

#endif

