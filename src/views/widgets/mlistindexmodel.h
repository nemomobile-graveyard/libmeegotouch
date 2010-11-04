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

#ifndef MLISTINDEXMODEL_H
#define MLISTINDEXMODEL_H

#include "mwidgetmodel.h"
#include <QModelIndexList>

class MList;

class MListIndexModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MListIndexModel)

public:
    M_MODEL_PROPERTY(QStringList, shortcutLabels, ShortcutLabels, true, QStringList())
    M_MODEL_PROPERTY(QModelIndexList, shortcutIndexes, ShortcutIndexes, true, QModelIndexList())
    M_MODEL_PTR_PROPERTY(MList*, list, List, true, NULL)
    M_MODEL_PROPERTY(int, displayMode, DisplayMode, true, 0)
    M_MODEL_PROPERTY(QPointF, offset, Offset, true, QPointF())
};

#endif
