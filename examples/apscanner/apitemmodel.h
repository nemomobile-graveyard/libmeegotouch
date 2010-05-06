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

#ifndef APITEMMODEL_H
#define APITEMMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QStringList>
#include "apscanner.h"

class APItemModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    APItemModel();
    virtual ~APItemModel();
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public:
    APScanner::AccessPointInfo accessPointInfo;
    int accessPointDataIndex(const QString &accessPointName);
    void updateAccessPointData(int accessPointDataIndex);

private:
    QVector<QStringList> theData;
    QStringList theHeaderData;
};

#endif
