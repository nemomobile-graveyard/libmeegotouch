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

#include "mcolorlist.h"

#include <MWidgetCreator>

M_REGISTER_WIDGET(MColorList)

MColorList::MColorList(QGraphicsItem *parent)
    :MWidgetController(new MColorListModel(), parent)
{
}

MColorList::~MColorList()
{
}

QColor MColorList::selectedColor()
{
    return model()->selectedColor();
}

void MColorList::setSelectedColor(const QColor &color)
{
    model()->setSelectedColor(color);
}

void MColorList::updateData(const QList<const char*> &modifications)
{
    const char *member;
    foreach (member, modifications) {
        if (member == MColorListModel::SelectedColor)
            emit colorPicked();
    }
}
