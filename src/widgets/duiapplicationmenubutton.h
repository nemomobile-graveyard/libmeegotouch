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

#ifndef DUIAPPLICATIONMENUBUTTON_H
#define DUIAPPLICATIONMENUBUTTON_H

#include <duibutton.h>
#include <duiapplicationmenubuttonmodel.h>

class DuiApplicationMenuButtonPrivate;

//! \internal

/*!
    \class DuiApplicationMenuButton
    \brief DuiApplicationMenuButton implementation of a application menu title button.

    \ingroup widgets

    \sa DuiApplicationMenuButtonModel
*/
class  DuiApplicationMenuButton : public DuiButton
{
    Q_OBJECT
    DUI_CONTROLLER(DuiApplicationMenuButton)

    /*!
        \property DuiApplicationMenuButton::progressIndicatorVisible
        \brief See DuiApplicationMenuButtonModel::progressIndicatorVisible
    */
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible)

    /*!
        \property DuiApplicationMenuButton::arrowIconVisible
        \brief See DuiApplicationMenuButtonModel::arrowIconVisible
    */
    Q_PROPERTY(bool arrowIconVisible READ isArrowIconVisible WRITE setArrowIconVisible)

public:
    /*!
      \brief Constructs the button without text.
     */
    explicit DuiApplicationMenuButton(QGraphicsItem *parent = 0, DuiApplicationMenuButtonModel *model = 0);

    /*!
      \brief Destroys the button.
     */
    virtual ~DuiApplicationMenuButton();

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
    DuiApplicationMenuButton(DuiButtonPrivate *dd, DuiApplicationMenuButtonModel *model, QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(DuiApplicationMenuButton)
    Q_DISABLE_COPY(DuiApplicationMenuButton)
};

//! \internal_end

#endif

