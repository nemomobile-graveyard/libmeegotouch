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


#ifndef MBASICSHEETHEADER_H
#define MBASICSHEETHEADER_H

#include "mexport.h"
#include "mwidgetcontroller.h"
#include "mbasicsheetheadermodel.h"

class MBasicSheetHeaderPrivate;

/*!
  \class MBasicSheetHeader
  \brief A basic header for MSheet

  MBasicSheetHeader is a basic header that you can conveniently use as the
  header widget of your MSheet.

  Its default view provides a button for a negative or neutral action in its
  left corner, a button for a positive action in its right corner and an
  optional progress indicator (of unknown duration, a.k.a a spinner) in its middle.

   Common usage:
   \code
   MySheet::MySheet() : MSheet() {
       MBasicSheetHeader *sheetHeader = new MBasicSheetHeader;

       sheetHeader->setPositiveAction(new QAction("Done", sheetHeader));
       connect(sheetHeader->positiveAction(), SIGNAL(triggered()), SLOT(done()));

       sheetHeader->setNegativeAction(new QAction("Cancel", sheetHeader));
       connect(sheetHeader->negativeAction(), SIGNAL(triggered()), SLOT(cancel()));

       setHeaderWidget(sheetHeader);
   }
   \endcode
 */
class M_CORE_EXPORT MBasicSheetHeader : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MBasicSheetHeader)

    /*!
       \property MBasicSheetHeader::progressIndicatorVisible
       \brief Defines whether a progress indicator should be shown

       It true, a progress indicator with unknown duration (such as a spinner) will be
       displayed in the center of the header (for its default view). Use it to hint the
       user that some process is ongoing or that the sheet is waiting for something to
       complete.

       By default, this property is false.

       Set it with setProgressIndicatorVisible() and get its current value with
       isProgressIndicatorVisible().
     */
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)

public:
    MBasicSheetHeader(QGraphicsItem *parent = 0);
    virtual ~MBasicSheetHeader();

    /*!
      \brief Sets the positive action

      Ownership is not automatically transfered, but you may still set MBasicSheetHeader
      as the parent of \a action (e.g. by passing MBasicSheetHeader in the constructor
      of \a action).
     */
    void setPositiveAction(QAction *action);

    /*!
      \brief Returns the positive action.
      There's not positive action by default, thus by default it returns 0 (null).
      \return the positive action.
     */
    QAction *positiveAction() const;

    /*!
      \brief Sets the negative action.

      In case your sheet has only a single, neutral, action, set it here and leave
      positiveAction() empty.

      Ownership is not automatically transfered, but you may still set MBasicSheetHeader
      as the parent of \a action (e.g. by passing MBasicSheetHeader in the constructor
      of \a action).
     */
    void setNegativeAction(QAction *action);

    /*!
      \brief Returns the negative action.
      There's not negative action by default, thus by default it returns 0 (null).
      \return the negative action.
     */
    QAction *negativeAction() const;

    /*!
      \brief Defines whether a progress indicator should be shown
      \param visible true will show it and false will hide it
     */
    void setProgressIndicatorVisible(bool visibile);

    /*!
      \brief Returns whether a progress indicator is being shown
      \return true if it's being shown or false if it's hidden
     */
    bool isProgressIndicatorVisible() const;

private:
    Q_DISABLE_COPY(MBasicSheetHeader)
    Q_DECLARE_PRIVATE(MBasicSheetHeader)
    MBasicSheetHeaderPrivate *d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_removePositiveActionFromModel())
    Q_PRIVATE_SLOT(d_func(), void _q_removeNegativeActionFromModel())
};

#endif // MBASICSHEETHEADER_H
