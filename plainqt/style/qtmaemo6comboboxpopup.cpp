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

#include "qtmaemo6comboboxpopup.h"
#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6style_p.h"

#include <QComboBox>
#include <QModelIndex>

QtMaemo6ComboBoxPopup::QtMaemo6ComboBoxPopup(QComboBox *comboBox, QWidget *parent)
    : QListView(parent),
    m_comboBox(comboBox)
{
    if(comboBox)
        setModel(comboBox->model());

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(listItemClicked(QModelIndex)));
}

QtMaemo6ComboBoxPopup::~QtMaemo6ComboBoxPopup()
{

}

void QtMaemo6ComboBoxPopup::listItemClicked(const QModelIndex & index)
{
    m_comboBox->setCurrentIndex(index.row());
    close();
}
