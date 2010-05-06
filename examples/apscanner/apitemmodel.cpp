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

#include <QTime>
#include "apitemmodel.h"

APItemModel::APItemModel()
{
    QTime time;
    time.start();
    qsrand(time.hour() + time.second() + time.minute());

    theHeaderData.append("Title");
    theHeaderData.append("Subtitle");
    theHeaderData.append("Thumbnail");
}

APItemModel::~APItemModel()
{
}

int APItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return theData.size();
}

int APItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return theHeaderData.size();
}

QVariant APItemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.isValid() && index.row() < theData.size()) {
            QStringList rowData = theData[index.row()];
            return QVariant(rowData);
        }
    }

    return QVariant();
}

QVariant APItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole) {
        if (section < theHeaderData.size()) {
            return QVariant(theHeaderData.at(section));
        }
    }

    return QVariant();
}

bool APItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        theData.insert(row + i, QStringList() << accessPointInfo.stationName << accessPointInfo.encryption << QString("./pics/") + QString::number(accessPointInfo.signalStrength) + QString("signal.png"));
    }
    endInsertRows();

    return true;
}

bool APItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    theData.remove(row, count);
    endRemoveRows();

    return true;
}

int APItemModel::accessPointDataIndex(const QString &accessPointName)
{
    int index = -1;

    for (int i = 0; i < rowCount(); ++i) {
        QStringList rowData = theData.at(i);
        if (rowData.at(0) == accessPointName) {
            index = i;
            break;
        }
    }

    return index;
}

void APItemModel::updateAccessPointData(int accessPointDataIndex)
{
    theData.replace(accessPointDataIndex, QStringList() << accessPointInfo.stationName << accessPointInfo.encryption << QString("./pics/") + QString::number(accessPointInfo.signalStrength) + QString("signal.png"));
    QModelIndex index = QAbstractItemModel::createIndex(accessPointDataIndex, accessPointDataIndex);
    emit dataChanged(index, index);
}
