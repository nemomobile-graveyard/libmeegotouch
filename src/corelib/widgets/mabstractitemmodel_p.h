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

#ifndef MABSTRACTITEMMODEL_P_H
#define MABSTRACTITEMMODEL_P_H

#include <QObject>
#include <QModelIndex>
#include <QStack>

class MAbstractItemModel;

class MAbstractItemModelPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MAbstractItemModel)
    
public:
    MAbstractItemModelPrivate();
    virtual ~MAbstractItemModelPrivate();
    
    void connectModel();
    
    void setGrouped(bool mode);
    bool isGrouped() const;
    
    void setChangeAnimated(bool animated);
    bool changeAnimated();
    
private Q_SLOTS:
    void _q_rowsInsertAnimated(const QModelIndex &parent, int first, int last);
    void _q_rowsRemoveAnimated(const QModelIndex &parent, int first, int last);
    
private:
    MAbstractItemModel *q_ptr;
    QStack<bool> animatedChange;
    bool groupMode;    
};

#endif // MABSTRACTITEMMODEL_P_H
