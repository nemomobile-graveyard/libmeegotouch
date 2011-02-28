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

#ifndef MCOLORLISTMODEL_H
#define MCOLORLISTMODEL_H

#include <MWidgetModel>
#include <QColor>
#include <QList>

typedef QList<QColor> ColorListType;

class M_CORE_EXPORT MColorListModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(MColorListModel)
    M_MODEL_PROPERTY(QColor, selectedColor, SelectedColor, true, QColor())
};

#endif
