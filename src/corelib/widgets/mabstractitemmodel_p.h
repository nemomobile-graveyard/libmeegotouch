/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MABSTRACTITEMMODEL_P_H
#define MABSTRACTITEMMODEL_P_H

#include <QObject>
#include <QModelIndex>

class MAbstractItemModel;

class MAbstractItemModelPrivate
{    
public:
    MAbstractItemModelPrivate();
    virtual ~MAbstractItemModelPrivate();
    
    void connectModel();
    
    void setGrouped(bool mode);
    bool isGrouped() const;
    
    void _q_rowsInsertAnimated(const QModelIndex &parent, int first, int last);
    void _q_rowsRemoveAnimated(const QModelIndex &parent, int first, int last);
    
    void _q_layoutChanged();

private:
    MAbstractItemModel *q_ptr;
    Q_DECLARE_PUBLIC(MAbstractItemModel)

    bool groupMode;    
    bool isAnimatedChange;
};

#endif // MABSTRACTITEMMODEL_P_H
