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

#ifndef DUICOMBOBOX_P_H
#define DUICOMBOBOX_P_H

#include "private/duiwidgetcontroller_p.h"
#include <QModelIndex>
#include <QItemSelection>

class DuiComboBoxPrivate : protected DuiWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(DuiComboBox)

public:
    DuiComboBoxPrivate();
    virtual ~DuiComboBoxPrivate();

    void init();
    void _q_modelDestroyed();
    void _q_selectionModelCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void _q_selectionModelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void _q_itemModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void _q_itemModelReset();
    void _q_itemClicked(const QModelIndex& index);
};

#endif
