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

#include <MWidgetCreator>

#include "mcolorcombobox.h"

M_REGISTER_WIDGET(MColorComboBox)

MColorComboBox::MColorComboBox(QGraphicsItem *parent)
    :MWidgetController(new MColorComboBoxModel(), parent)
{
}

MColorComboBox::~MColorComboBox()
{
}

QString MColorComboBox::title()
{
    return model()->title();
}

void MColorComboBox::setTitle(const QString &title)
{
    model()->setTitle(title);
}

QColor MColorComboBox::pickedColor()
{
    return model()->color();
}

void MColorComboBox::setPickedColor(const QColor &color)
{
    model()->setColor(color);
}

void MColorComboBox::updateData(const QList<const char*> &modifications)
{
    const char *member;
    foreach (member, modifications) {
        if (member == MColorComboBoxModel::Color)
            emit colorPicked(model()->color());
    }
}
