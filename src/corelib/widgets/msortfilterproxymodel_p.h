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

#ifndef MSORTFILTERPROXYMODEL_P_H
#define MSORTFILTERPROXYMODEL_P_H

#include <QObject>
#include <QModelIndex>
#include "mlocale.h"

class QAbstractItemModel;
class MSortFilterProxyModel;
class MCollator;

class MSortFilterProxyModelPrivate
{
public:
    MSortFilterProxyModelPrivate();
    virtual ~MSortFilterProxyModelPrivate();

    void init();
    void connectToSourceModel(QAbstractItemModel *sourceModel);

    void _q_rowsInserted(const QModelIndex &parent, int first, int last);
    void _q_rowsRemoved(const QModelIndex &parent, int first, int last);
    void _q_layoutAboutToBeAnimated();
    void _q_resetAnimatedChange();

private:
    MSortFilterProxyModel *q_ptr;
    Q_DECLARE_PUBLIC(MSortFilterProxyModel)

    bool isSupportedModel;
    bool isAnimatedChange;
    bool localeAwareSortingEnabled;
    MLocale locale;
    MCollator *pCollator;
};

#endif // MSORTFILTERPROXYMODEL_P_H
