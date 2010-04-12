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

#ifndef MAPPLICATIONMENUBUTTON_H
#define MAPPLICATIONMENUBUTTON_H

#include <mbutton.h>
#include <mapplicationmenubuttonmodel.h>

class MApplicationMenuButtonPrivate;

//! \internal

/*!
    \class MApplicationMenuButton
    \brief MApplicationMenuButton implementation of a application menu title button.

    \ingroup widgets

    \sa MApplicationMenuButtonModel
*/
class  MApplicationMenuButton : public MButton
{
    Q_OBJECT
    M_CONTROLLER(MApplicationMenuButton)

    /*!
        \property MApplicationMenuButton::progressIndicatorVisible
        \brief See MApplicationMenuButtonModel::progressIndicatorVisible
    */
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)

    /*!
        \property MApplicationMenuButton::arrowIconVisible
        \brief See MApplicationMenuButtonModel::arrowIconVisible
    */
    Q_PROPERTY(bool arrowIconVisible READ isArrowIconVisible WRITE setArrowIconVisible)

public:
    /*!
      \brief Constructs the button without text.
     */
    explicit MApplicationMenuButton(QGraphicsItem *parent = 0, MApplicationMenuButtonModel *model = 0);

    /*!
      \brief Destroys the button.
     */
    virtual ~MApplicationMenuButton();

    /*!
      \brief Returns true if the progress indicator is visible.
    */
    bool isProgressIndicatorVisible() const;

    /*!
      \brief Returns true if the arrow icon is visible.
    */
    bool isArrowIconVisible() const;

public Q_SLOTS:

    /*!
      \brief Set the visibility of the progress indicator.
    */
    void setProgressIndicatorVisible(bool);

    /*!
      \brief Set the visibility of the arrow icon.
    */
    void setArrowIconVisible(bool);

protected:
    //! \internal
    MApplicationMenuButton(MButtonPrivate *dd, MApplicationMenuButtonModel *model, QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MApplicationMenuButton)
    Q_DISABLE_COPY(MApplicationMenuButton)
};

//! \internal_end

#endif

