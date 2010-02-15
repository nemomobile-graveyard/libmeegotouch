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

#ifndef DUILISTNAMESPACE_H
#define DUILISTNAMESPACE_H

#include <QAbstractItemModel>
#include "duiexport.h"

/*!
    \deprecated Since 0.17. There is no replacement for this. Just create your own empty model.
  */
class DuiEmptyModel : public QAbstractItemModel
{
public:
    explicit DuiEmptyModel(QObject *parent = 0) : QAbstractItemModel(parent) {}
    QModelIndex index(int, int, const QModelIndex &) const {
        return QModelIndex();
    }
    QModelIndex parent(const QModelIndex &) const {
        return QModelIndex();
    }
    int rowCount(const QModelIndex &) const {
        return 0;
    }
    int columnCount(const QModelIndex &) const {
        return 0;
    }
    bool hasChildren(const QModelIndex &) const {
        return false;
    }
    QVariant data(const QModelIndex &, int) const {
        return QVariant();
    }
};

//! \deprecated Since 0.17. There is no replacement for this. Just create your own empty model.
Q_GLOBAL_STATIC(DuiEmptyModel, duiEmptyModel)

namespace DuiListNameSpace
{
    //! \deprecated Since 0.17. Not in used.
    enum DUIListRoles {
        ItemTypeRole = Qt::UserRole + 1,
        SubtitleRole
    };

    //! \deprecated Since 0.17. Not in used.
    enum ItemType {
        GridItem = 0,
        Custom
    };
}

class DuiWidget;
typedef DuiWidget *DuiWidgetPtr;
Q_DECLARE_METATYPE(DuiWidgetPtr)

#endif
