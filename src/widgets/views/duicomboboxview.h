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

#ifndef DUICOMBOBOXVIEW_H
#define DUICOMBOBOXVIEW_H

#include "duiwidgetview.h"
#include "duicomboboxmodel.h"
#include "duicomboboxstyle.h"

class DuiComboBox;
class DuiComboBoxViewPrivate;

/*!
    \class DuiComboBoxView
    \brief View class for DuiComboBox.

    \ingroup views

    DuiComboBoxView is used to visualize comboBox.

    \sa DuiComboBox DuiComboBoxStyle
 */

class DUI_EXPORT DuiComboBoxView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiComboBoxModel, DuiComboBoxStyle)

public:
    /*!
        \brief Constructor
        \param controller Pointer to the controller
     */
    DuiComboBoxView(DuiComboBox *controller);

    /*!
        \brief Destructor
     */
    virtual ~DuiComboBoxView();

protected slots:
    //! \reimp
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

    //! \internal
    DuiComboBoxView(DuiComboBoxViewPrivate &dd, DuiComboBox *controller);
    //! \internal_end

private:
    Q_DISABLE_COPY(DuiComboBoxView)
    Q_DECLARE_PRIVATE(DuiComboBoxView)

    Q_PRIVATE_SLOT(d_func(), void _q_itemModelCurrentIndexChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_showPopup())

#ifdef UNIT_TEST
    friend class Ut_DuiComboBox;
#endif
};

#endif
