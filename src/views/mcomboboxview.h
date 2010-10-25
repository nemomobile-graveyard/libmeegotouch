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

#ifndef MCOMBOBOXVIEW_H
#define MCOMBOBOXVIEW_H

#include "mwidgetview.h"
#include "mcomboboxmodel.h"
#include "mcomboboxstyle.h"

class MComboBox;
class MComboBoxViewPrivate;

/*!
    \class MComboBoxView
    \brief View class for MComboBox.

    \ingroup views

    MComboBoxView is used to visualize comboBox.

    \sa MComboBox MComboBoxStyle
 */

class M_VIEWS_EXPORT MComboBoxView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MComboBoxModel, MComboBoxStyle)

public:
    /*!
        \brief Constructor
        \param controller Pointer to the controller
     */
    MComboBoxView(MComboBox *controller);

    /*!
        \brief Destructor
     */
    virtual ~MComboBoxView();

protected Q_SLOTS:
    //! \reimp
    virtual void applyStyle();
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    //! \internal
    MComboBoxView(MComboBoxViewPrivate &dd, MComboBox *controller);
    MComboBoxViewPrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(MComboBoxView)
    Q_DECLARE_PRIVATE(MComboBoxView)

    Q_PRIVATE_SLOT(d_func(), void _q_itemModelCurrentIndexChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_showPopup())
    Q_PRIVATE_SLOT(d_func(), void _q_dismissPopup())
    Q_PRIVATE_SLOT(d_func(), void _q_popuplistAppeared())
    Q_PRIVATE_SLOT(d_func(), void _q_popuplistDisappeared())

#ifdef UNIT_TEST
    friend class Ut_MComboBox;
#endif
};

#endif
