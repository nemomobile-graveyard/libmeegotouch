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

public:
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
      It will be fullscreen, providing no status bar.

      User won't be able to minimize or move it away.
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

private:
    Q_DECLARE_PRIVATE(MSheet)
    Q_DISABLE_COPY(MSheet)
    Q_PRIVATE_SLOT(d_func(), void _q_onStandAloneSheetDisappeared())
    Q_PRIVATE_SLOT(d_func(), void _q_makeSystemSheetDisappear())
    Q_PRIVATE_SLOT(d_func(), void _q_dismissSystemSheet())
    Q_PRIVATE_SLOT(d_func(), void _q_onCentralWidgetDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_onHeaderWidgetDestroyed())
};

#endif // MSHEET_H
