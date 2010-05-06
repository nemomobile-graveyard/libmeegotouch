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

#ifndef MCOMBOBOX_P_H
#define MCOMBOBOX_P_H

#include "private/mwidgetcontroller_p.h"
#include <QModelIndex>
#include <QItemSelection>

class MComboBoxPrivate : protected MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MComboBox)

public:
    MComboBoxPrivate();
    virtual ~MComboBoxPrivate();

    void init();
    void _q_modelDestroyed();
    void _q_selectionModelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void _q_itemModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void _q_itemModelReset();
    void _q_itemClicked(const QModelIndex& index);
};

#endif
