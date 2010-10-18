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

#ifndef MLIST_P_H
#define MLIST_P_H

#include "private/mwidgetcontroller_p.h"

#include <QObject>
#include <QAbstractItemModel>
#include <QGraphicsGridLayout>
#include <QVector>
#include <QSize>
#include <QVariant>
#include <QHash>

class MPannableViewport;
class MWidget;
class MListFilter;

#include "mlist.h"

class MListPrivate : protected MWidgetControllerPrivate
{
    Q_DECLARE_PUBLIC(MList)


public:
    MListPrivate();
    virtual ~MListPrivate();

    void init();

    MList::SelectionMode selectionMode;

    MListFilter* listFilter;

    void updateLongTapConnections();
public slots:
    /*!
     * \brief This slot is called when items are changed in the model. The changed items are those
     * from topLeft to bottomRight inclusive. If just one item is changed topLeft == bottomRight.
     * Equivalent to the same method in QListView.
     */
    //void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    MList::ListOptimizationFlags optimizationFlags;
};

#endif
