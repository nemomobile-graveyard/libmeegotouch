/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSHEET_H
#define MSHEET_H

#include <mscenewindow.h>
#include <msheetmodel.h>

class MSheetPrivate;

/*!
  \class MSheet
  \brief A full screen scene window used to display complex or multi-step subtasks

  \ingroup widgets

  \section MSheetOverview Overview

  The sheet is a self-contained full screen scene window that slides up from the bottom
  of the screen. It is used to interrupt the application's main workflow temporarily and
  gather or display some key information immediately. A sheet is especially appropriate
  for isolating a potentially complex or multistep subtask.

  Use a sheet when you want to....
  \li lead the user through a sequence of different types of input controls, such as combo box,
      keyboard, pickers, search box, lists, etc. In this case it is usually also important
      that the task is completed to avoid leaving the user's data in an ambiguous state.
      E.g. creating and sending of a new email message, creating a new calendar event or
      entering some credentials.
  \li present the user some content temporarily and offer some basic options for it. E.g.
      displaying an image attached to an email, which the user can 'dismiss' or 'save'.
  \li change work mode or context temporarily. E.g. opening a browser view after clicking on
      a URL.

  A sheet is not used when...
  \li a task is very simple and requires just one tap. E.g. selecting a value from a short
      list of options. This is handled by a simple list / dialog.

  \section MSheetAnatomy Anatomy

  The sheet always...
  \li Covers the entire screen. This strengthens the user's perception of entering a
      separate, transient mode.
  \li Displays only buttons (at least 1) on its header. The buttons preferably
      use text strings and not icons. The buttons always provide an obvious and safe way to
      exit the sheet; people should always be able to predict the fate of their work when
      they dismiss a sheet.
  \li In case the sheet merely displays information and / or is non-destructive, then the
      action is 'Done'.
  \li In most other cases the header buttons provide a negative- and positive action. For the
      negative action usually 'Cancel' is used as the label. For the positive action the
      actual verb is used, e.g. Save / Send / Add.

  The sheet contains...
  \li the controls necessary to perform the task
  \li in very specific cases the sheet could spawn a query dialog, e.g. closing the sheet
      could spawn a "Save as draft? Save / Discard" dialog.

  The sheet never...
  \li uses a back icon in the top bar ('Back' is discouraged to prevent conveying the
      association of a hierarchical relationship between the sheet and the current page).
  \li is accompanied by the navigation bar thus it can never have a menu.
  \li spawns more than 1 additional sheet, designs that require a 2nd sheet should be
      reconsidered and ones requiring a 3rd require a complete redesign.

  The sheet encourages...
  \li using a title to identify the task or provide guidance
  \li using inline pickers / combo boxes (traditionally handled by dialogues)
 */
class M_CORE_EXPORT MSheet : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSheet)

    /*!
       \property MSheet::headerVisible
       \brief Defines whether the header should be visible
       By default this property is true (header is shown).
     */
    Q_PROPERTY(bool headerVisible READ isHeaderVisible WRITE setHeaderVisible)

    /*!
       \property MSheet::statusBarVisibleInSystemwide
       \brief Defines whether the status bar should be visible when sheet appears systemwide

       To get a proper animation of a systemwide appearance you should set this
       property before calling appearSystemwide().

       This property has an effect only with sheets that are displayed with appearSystemwide().

       By default this property is false (status bar not visible for systemwide appearances).
     */
    Q_PROPERTY(bool statusBarVisibleInSystemwide READ isStatusBarVisibleInSystemwide WRITE setStatusBarVisibleInSystemwide)

    /*!
      \property MSheet::systemwideModeOrientation
      \brief Orientation of the sheet when it appears systemwide.

      This property affects only sheets that are shown with appearSystemwide().
      You should set this property before calling appearSystemwide().

      The default value of this property is FollowsDeviceOrientation.

      \sa appearSystemwide()
     */
    Q_PROPERTY(SystemwideModeOrientation systemwideModeOrientation READ systemwideModeOrientation WRITE setSystemwideModeOrientation)

    // OBS: The property below exists because we want to fix an issue
    // without causing a behavioral break.
    // In a perfect world we wouldn't have it and the behavior
    // would be the same as setting it to true.
    /*!
      \property MSheet::centralWidgetSizePolicyRespected
      \brief Whether central widget's size policy should be used/respected.

      If true, MSheet will consider central widget's size policy when setting its
      geometry.

      If false, central widget will always have the size of sheet's central area
      regardless of his size policy. It effectively works as if his size policy
      was set to QSizePolicy::Expanding on both directions.

      By default this property is false.
     */
    Q_PROPERTY(bool centralWidgetSizePolicyRespected READ isCentralWidgetSizePolicyRespected WRITE setCentralWidgetSizePolicyRespected)

public:

    /*!
     * This enum describes the possible behaviors of the sheet's
       orientation when it's displayed systemwide.
       \sa appearSystemwide()
     */
    enum SystemwideModeOrientation {
        FollowsDeviceOrientation = 0, /*!< Follows the device orientation. */
        FollowsCurrentAppWindowOrientation, /*!< Follows the orientation of the
            current application window. E.g. if it's launched on top of an app. that's
            locked to portrait the sheet will also be (and remain) in portrait.*/
        LockedToPortraitOrientation, /*!< Locked to portrait orientation. */
        LockedToLandscapeOrientation /*!< Locked to landscape orientation. */
    };

    /*!
      \brief Constructs a new sheet.
     */
    MSheet();

    /*!
      \brief Destroys the sheet.
     */
    virtual ~MSheet();

    /*!
      \brief Returns the central widget of the sheet.

      By default a sheet provides a plain QGraphicsWidget on which other widgets can be
      placed. It's also possible to set a central widget for a sheet with setCentralWidget().

      \return the pointer to the central widget.

      \sa setCentralWidget()
     */
    QGraphicsWidget *centralWidget();

    /*!
      \brief Sets the given widget to be the sheet's central widget.

      \note MSheet takes ownership of \a widget and deletes it at the appropriate
            time.

      \param  widget the central widget.

      \sa centralWidget()
     */
    void setCentralWidget(QGraphicsWidget *widget);

    /*!
      \brief Returns the widget on sheet's header.

      By default a sheet provides a plain QGraphicsWidget on which other widgets can be
      placed. It's also possible to replace it with setHeaderWidget().

      Typical usage is to set a layout of MButtons to it.

      \code
      QGraphicsLayout *headerLayout = new QGraphicsLinearLayout(sheet->headerWidget());
      headerLayout->addItem(new MButton("Cancel"));
      headerLayout->addStretch();
      headerLayout->addItem(new MButton("Save"));
      \endcode

      For most common cases (such as the example above) you should consider using MBasicSheetHeader.

      \sa setHeaderWidget(), MBasicSheetHeader
     */
    QGraphicsWidget *headerWidget();

    /*!
      \brief Sets the widget to be placed on sheet's header

      \note MSheet takes ownership of \a widget and deletes it at the appropriate
            time.

      \param  widget the widget to be placed on sheet's header

      \sa headerWidget()
     */
    void setHeaderWidget(QGraphicsWidget *widget);

    /*!
      \brief Makes the sheet appear systemwide.

      The sheet will appear on its own stand-alone MWindow and separate scene.

      By default it will be fullscreen, providing no status bar. That can be
      changed with setStatusBarVisibleInSystemwide(true).

      You can change the orientation behavior of the systemwide sheet by
      calling setSystemwideModeOrientation() before this method.

      \sa MSheet::statusBarVisibleInSystemwide, MSheet::systemwideModeOrientation
     */
    void appearSystemwide(MSceneWindow::DeletionPolicy policy);

    /*!
      \brief Sets whether the header should be visible
      \param visible true to show the header and false to hide it
     */
    void setHeaderVisible(bool visible);

    /*!
      \brief Returns whether the header is visible.
     */
    bool isHeaderVisible() const;

    /*!
      \brief Sets whether the status bar will be visible for systemwide appearances.
      \sa isStatusBarVisibleInSystemwide(), appearSystemwide()
     */
    void setStatusBarVisibleInSystemwide(bool visible);

    /*!
      \brief Returns whether the status bar will be visible for systemwide appearances.
      \sa setStatusBarVisibleInSystemwide(), appearSystemwide()
     */
    bool isStatusBarVisibleInSystemwide() const;


    /*!
      \brief Returns the orientation behavior when sheet appears systemwide
      \sa appearSystemwide()
     */
    SystemwideModeOrientation systemwideModeOrientation() const;

    /*!
      \brief Sets the orientation behavior for systemwide a appearance
      \sa appearSystemwide()
     */
    void setSystemwideModeOrientation(SystemwideModeOrientation orientation);

    /*!
      \brief Returns whether central widget's size policy is being respected.
     */
    bool isCentralWidgetSizePolicyRespected() const;

    /*!
      \brief Sets whether central widget's size policy should be respected.
     */
    void setCentralWidgetSizePolicyRespected(bool enabled);

private:
    Q_DECLARE_PRIVATE(MSheet)
    Q_DISABLE_COPY(MSheet)
    Q_PRIVATE_SLOT(d_func(), void _q_onStandAloneSheetDisappeared())
    Q_PRIVATE_SLOT(d_func(), void _q_makeSystemSheetDisappearImmediately())
    Q_PRIVATE_SLOT(d_func(), void _q_dismissSystemSheetImmediately())
    Q_PRIVATE_SLOT(d_func(), void _q_onCentralWidgetDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_onHeaderWidgetDestroyed())
};

Q_DECLARE_METATYPE(MSheet::SystemwideModeOrientation)

#endif // MSHEET_H
