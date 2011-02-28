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

#ifndef MCOLORCOMBOBOXVEIW_H
#define MCOLORCOMBOBOXVEIW_H

#include <QObject>
#include <MWidgetView>

#include "mcolorcomboboxmodel.h"
#include "mcolorcomboboxstyle.h"
#include "mcolorcomboboxview_p.h"

class MButton;
class MColorComm;
class MColorComboBoxViewPrivate;

/**
 * \brief View class for MColorComboBox.
 */
class M_VIEWS_EXPORT MColorComboBoxView : public MWidgetView
{
    Q_OBJECT
    MColorComboBoxViewPrivate* d_ptr;
    Q_DISABLE_COPY(MColorComboBoxView)
    Q_DECLARE_PRIVATE(MColorComboBoxView)
    M_VIEW(MColorComboBoxModel, MColorComboBoxStyle)

public:

    MColorComboBoxView(MColorComboBox *controller);

    virtual ~MColorComboBoxView();

protected slots:

    //! \reimp

    virtual void applyStyle();

    virtual void setupModel();

    virtual void updateData(const QList<const char*> &modifications);

    //! \reimp_end

private:

    Q_PRIVATE_SLOT(d_func(), void _q_showPopup())
    Q_PRIVATE_SLOT(d_func(), void _q_colorPicked())
    Q_PRIVATE_SLOT(d_func(), void _q_popupDisappeared())
};

#endif
