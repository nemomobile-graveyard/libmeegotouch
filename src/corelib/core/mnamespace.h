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

#ifndef MNAMESPACE_H
#define MNAMESPACE_H

#include <QMetaType>
#include <QMessageBox>
#include <QTextOption>
#include "mexport.h"

namespace M
{
    enum AssemblyType {
        Library,
        Application,
        AssemblyNone
    };

    /*!
     * setting a dynamic porperty to any QWidget or QApplication will cause the object
     * not to be styled like M does.
     */
    M_CORE_EXPORT extern const char* NoMStyle;

    /*!
     * setting a dynamic property to QApplication named NoMNavigationBar will hide the
     * navigationbar from qt maemo 6 style
     */
    M_CORE_EXPORT extern const char* NoMNavigationBar;

    /*!
     * setting a dynamic property to QApplication named NoMStatusBar will hide the
     * statusbar from qt maemo 6 style
     */
    M_CORE_EXPORT extern const char* NoMStatusBar;

    /*!
     * \brief An enumeration of different rotation angles.
     * \sa Orientation
     */
    enum OrientationAngle {
        Angle0   =   0, //!< No rotation.
        Angle90  =  90, //!< 90 degrees of rotation, clockwise.
        Angle180 = 180, //!< 180 degrees of rotation, clockwise.
        Angle270 = 270  //!< 270 degrees of rotation, clockwise.
    };

    /*!
     * \brief This enum contains values that defines a basic relationship between the width and
     *        height of windows, displays, rectangles, etc.
     *
     * \sa OrientationAngle
     */
    enum Orientation {
        Portrait, //!< Window's or display's height is larger than its width
        Landscape //!< Window's or display's width is larger than (or equal to) its height
    };

    /*!
     * This enum contains all possible positions for a tile in a layout
     * \code
           .---.          .-----------------------.
         /       \      /        |         |        \
        | Default |    |   H-L   x   H-C   x   H-R   |
         \       /      \        |         |        /
           '---'          '-----------------------'

           .---.          .-----------------------.
         /       \      /        |         |        \
        |   V-T   |    |   TL    x   TC    x    TR   |
        |         |    |         |         |         |
        |----x----|    |----x---------x---------x----|
        |         |    |         |         |         |
        |   V-C   |    |   CL    x  Center x    CR   |
        |         |    |         |         |         |
        |----x----|    |----x---------x---------x----|
        |         |    |         |         |         |
        |   V-B   |    |   BL    x   BC    x    BR   |
         \       /      \        |         |        /
           '---'          '-----------------------'
     * \endcode
     */
    enum Position {
        DefaultPosition,

        TopLeftPosition,
        TopCenterPosition,
        TopRightPosition,
        CenterLeftPosition,
        CenterPosition,
        CenterRightPosition,
        BottomLeftPosition,
        BottomCenterPosition,
        BottomRightPosition,

        HorizontalLeftPosition,
        HorizontalCenterPosition,
        HorizontalRightPosition,

        VerticalTopPosition,
        VerticalCenterPosition,
        VerticalBottomPosition
    };

    /*!
     * Defines whether directories should be traversed recursively.
     */
    enum RecursionMode {
        Recursive,
        NonRecursive
    };

    //! extensions for Qt::inputMethodQuery
    enum InputMethodQueryExtensions {
        VisualizationPriorityQuery = 10001, //!< Tells if input method widget wants to have high
        //!< priority for visualization. Input method should
        //!< honor this and stay out of widgets space.
        PreeditRectangleQuery,      //!< Retrieve bounding rectangle for current preedit text.
        ImCorrectionEnabledQuery,   //!< explicit correction enabling for text entries
        ImModeQuery,                //!< Retrieve mode: normal, direct or proxy
        InputMethodToolbarIdQuery,  //!< custom toolbar identifier for text entry. \deprecated use InputMethodAttributeExtensionIdQuery instead.
        InputMethodAttributeExtensionIdQuery = InputMethodToolbarIdQuery, //!< attribute extension identifier for text entry
        InputMethodToolbarQuery,     //!< custom toolbar file name for text entry. \deprecated use InputMethodAttributeExtensionQuery instead.
        InputMethodAttributeExtensionQuery = InputMethodToolbarQuery //!< attribute extension file name for text entry
    };

    //! \deprecated do not use
    enum PreeditFace {
        PreeditDefault,
        PreeditNoCandidates
    };

    //! Content type for text entries. Used at least with MTextEdit
    enum TextContentType {
        //! all characters allowed
        FreeTextContentType,

        //! only integer numbers allowed
        NumberContentType,

        //! allows numbers and certain other characters used in phone numbers
        PhoneNumberContentType,

        //! allows only characters permitted in email address
        EmailContentType,

        //! allows only character permitted in URL address
        UrlContentType,

        //! allows content with user defined format
        CustomContentType
    };

    enum InputMethodMode {
        //! Normal mode allows to use preedit and error correction
        InputMethodModeNormal,

        //! Virtual keyboard sends QKeyEvent for every key press or release
        InputMethodModeDirect,

        //! Used with proxy widget
        InputMethodModeProxy
    };

    enum PrestartMode {
        //! Default mode: no prestarting
        NoPrestart,

        /*! enable prestart mode and return back
         *  to the prestarted state after being launched and closed
         */
        LazyShutdown,

        /*!
         * enable prestart mode and restart automatically
         * after being launched and closed
         */
        TerminateOnClose,

        /*! enable multi-windowed prestart mode and return back
         *  to the prestarted state after being launched and closed
         */
        LazyShutdownMultiWindow,

        /*!
         * enable multi-windowed prestart mode and restart automatically
         * after being launched and closed
         */
        TerminateOnCloseMultiWindow
    };

    /*!
      * This enum describes the roles that can be used to describe buttons in the button box.
      * Currently they are used to determine primary order of buttons.
      * Positive roles like AcceptRole, ActionRole, YesRole will be placed always before buttons with negative role:
      * CancelRole, RejectRole.
      */
    enum ButtonRole {
        //! The button is invalid.
        InvalidRole = QMessageBox::InvalidRole,
        //! Clicking the button causes the dialog to be accepted (e.g. OK).
        AcceptRole = QMessageBox::AcceptRole,
        //! Clicking the button causes the dialog to be rejected (e.g. Cancel).
        RejectRole = QMessageBox::RejectRole,
        //! Clicking the button causes a destructive change (e.g. for Discarding Changes) and closes the dialog.
        DestructiveRole = QMessageBox::DestructiveRole,
        //! Clicking the button causes changes to the elements within the dialog.
        ActionRole = QMessageBox::ActionRole,
        //! The button can be clicked to request help.
        HelpRole = QMessageBox::HelpRole,
        //! The button is a "Yes"-like button.
        YesRole = QMessageBox::YesRole,
        //! The button is a "No"-like button.
        NoRole = QMessageBox::NoRole,
        //! The button resets the dialog's fields to default values.
        ResetRole = QMessageBox::ResetRole,
        //! The button applies current changes.
        ApplyRole = QMessageBox::ApplyRole,

        NRoles = QMessageBox::NRoles
    };

    /*!
     * \brief This enum describes flags for standard buttons. Each button has a defined caption.
     *
     */
    enum StandardButton {
        //! An invalid button. \b NOTE: it's not the button witn "No" caption.
        NoStandardButton = QMessageBox::NoButton,
        //! An "OK" button.
        OkButton = QMessageBox::Ok,
        //! A "Save" button.
        SaveButton = QMessageBox::Save,
        //! A "Save All" button.
        SaveAllButton = QMessageBox::SaveAll,
        //! A "Open" button.
        OpenButton = QMessageBox::Open,
        //! A "Yes" button.
        YesButton = QMessageBox::Yes,
        //! A "Yes to all" button.
        YesToAllButton = QMessageBox::YesToAll,
        //! A "No" button.
        NoButton = QMessageBox::No,
        //! A "No to all" button.
        NoToAllButton = QMessageBox::NoToAll,
        //! An "Abort" button.
        AbortButton = QMessageBox::Abort,
        //! A "Retry" button.
        RetryButton = QMessageBox::Retry,
        //! An "Ignore" button.
        IgnoreButton = QMessageBox::Ignore,
        //! A "Close" button.
        CloseButton = QMessageBox::Close,
        //! A "Cancel" button.
        CancelButton = QMessageBox::Cancel,
        //! A "Discard" button.
        DiscardButton = QMessageBox::Discard,
        //! A "Help" button.
        HelpButton = QMessageBox::Help,
        //! An "Apply" button.
        ApplyButton = QMessageBox::Apply,
        //! A "Reset" button.
        ResetButton = QMessageBox::Reset,
        //! A "Restore defaults" button.
        RestoreDefaultsButton = QMessageBox::RestoreDefaults,
        //! A "Done" button.
        DoneButton = 0x10000000,

        FirstButton = OkButton,
        LastButton = DoneButton
    };
    Q_DECLARE_FLAGS(StandardButtons, StandardButton)
}

Q_DECLARE_OPERATORS_FOR_FLAGS(M::StandardButtons)

Q_ENUMS(M::AssemblyType)

Q_DECLARE_METATYPE(M::OrientationAngle)
Q_DECLARE_METATYPE(M::Orientation)
Q_DECLARE_METATYPE(M::Position)
Q_DECLARE_METATYPE(M::RecursionMode)
Q_DECLARE_METATYPE(M::InputMethodQueryExtensions)
Q_DECLARE_METATYPE(M::PreeditFace)
Q_DECLARE_METATYPE(M::TextContentType)
Q_DECLARE_METATYPE(M::InputMethodMode)
Q_DECLARE_METATYPE(M::PrestartMode)
Q_DECLARE_METATYPE(M::StandardButton)

Q_DECLARE_METATYPE(QTextOption::WrapMode)
#endif
