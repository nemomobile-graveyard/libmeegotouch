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

#include "phonebookmodel.h"
#include "utils.h"
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

#include <MTheme>

PhoneBookModel::PhoneBookModel()
{
    namesList = loadFakeNames();
    imageIdList = loadFakeImageIds();
    defaultThumbnail = QImage(Utils::imagesDir() + "DefaultAvatar.png");
    modelRowCount = 0;
}

PhoneBookModel::~PhoneBookModel()
{
    qDeleteAll(phoneBookEntries);
}

QVariant PhoneBookModel::data(const QModelIndex &index, int role) const
{
    // This function will be called many times during fast panning, lets
    // check boundaries and validnes only in debug mode
    Q_ASSERT(index.isValid());
    Q_ASSERT(index.row() < phoneBookEntries.size());

    if (role == Qt::DisplayRole) {
        // Let's store a pointer into QVariant, otherwise QT will make a copy but we are lazy
        // and don't want to copy stuff around
        return QVariant::fromValue(static_cast<void *>(phoneBookEntries[index.row()]));
    } else if (role == PhoneBookModel::PhoneBookSortRole) {
        return QVariant::fromValue(phoneBookEntries[index.row()]->lastName);
    } else if (role == PhoneBookModel::PhoneBookFilterRole) {
        return QVariant::fromValue(phoneBookEntries[index.row()]->fullName);
    }

    return QVariant();
}

QStringList PhoneBookModel::loadFakeNames()
{
    QStringList listNames;

    // loads the names from file, read each line splitting by space
    QFile fileNames(Utils::contactsDir().append("names.txt"));
    if (fileNames.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileNames);
        in.setCodec("UTF-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                listNames << line.split(' ');
            }
        }
    }

    // fallback
    if (listNames.isEmpty())
        listNames << "Pasi" << "Arto" << "Kiki" << "Keitty" << "Kitti" << "Adam" << "Tomas" << "Jakub" << "Sergiy" << "Rocks";

    return listNames;
}

QStringList PhoneBookModel::loadFakeImageIds()
{
    QString contactsDir = Utils::contactsDir();

    // gets all thumbnail contacts
    QDir imagesDir(contactsDir);
    imagesDir.setNameFilters(QStringList() << "*.png");

    QStringList imageContacts = imagesDir.entryList(QDir::Files);
    QStringList imageIds;
    foreach(const QString & imageName, imageContacts) {
        imageIds << QFileInfo(imageName).baseName();
    }

    return imageIds;
}

QString PhoneBookModel::generatePhoneNumber()
{
    QString phoneNumber("+358 ");

    for (int n = 0; n < 8; ++n)
        phoneNumber.append(QString::number(qrand() % 10));

    return phoneNumber;
}

PhoneBookEntry *PhoneBookModel::generateEntry()
{
    PhoneBookEntry *entry = new PhoneBookEntry;
    entry->firstName = namesList[qrand() % namesList.size()];
    entry->lastName = namesList[qrand() % namesList.size()];
    entry->fullName = QString("%1, %2").arg(entry->lastName).arg(entry->firstName);
    entry->phoneNumber = generatePhoneNumber();
    entry->thumbnail = defaultThumbnail;

    if (imageIdList.size() > 0)
        entry->thumbnailId = imageIdList[qrand() % imageIdList.size()];

    return entry;
}

int PhoneBookModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return modelRowCount;
}

int PhoneBookModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

bool PhoneBookModel::insertRows(int row, int count, const QModelIndex &parent)
{
    emit layoutAboutToBeChanged();
    beginInsertRows(parent, row, row + count - 1);

    for (int i = phoneBookEntries.size(); i < count; i++) {
        PhoneBookEntry *entry = generateEntry();
        phoneBookEntries.append(entry);
    }

    if(!parent.isValid())
        modelRowCount += count;

    endInsertRows();
    emit layoutChanged();
    return true;
}

bool PhoneBookModel::removeRows(int row, int count, const QModelIndex &parent)
{
    emit layoutAboutToBeChanged();
    if (count <= 0)
        return true; //Successfully removed 0 rows.

    beginRemoveRows(parent, row, row + count - 1);
    modelRowCount -= count;
    endRemoveRows();

    emit layoutChanged();
    return true;
}

void PhoneBookModel::thumbnailWasLoaded(const QModelIndex &index)
{
    emit dataChanged(index, index);
}

void PhoneBookModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);

    for (int i = 0; i < phoneBookEntries.size(); i++) {
        for (int j = i + 1; j < phoneBookEntries.size(); j++) {
            PhoneBookEntry *iEntry = phoneBookEntries.at(i);
            PhoneBookEntry *jEntry = phoneBookEntries.at(j);
            if((order == Qt::AscendingOrder && iEntry->fullName > jEntry->fullName) ||
               (order == Qt::DescendingOrder && iEntry->fullName < jEntry->fullName)) {
                phoneBookEntries.replace(i, jEntry);
                phoneBookEntries.replace(j, iEntry);
            }
        }
    }
}


PhoneBookImageLoader::PhoneBookImageLoader()
{

}

PhoneBookImageLoader::~PhoneBookImageLoader()
{
    stopLoadingPictures();
}

void PhoneBookImageLoader::loadPictures(const QModelIndex &firstVisibleRow, const QModelIndex &lastVisibleRow)
{
    if (!firstVisibleRow.isValid() || !lastVisibleRow.isValid())
        return;

    QModelIndex topLeftRow = firstVisibleRow;
    QModelIndex bottomRightRow = lastVisibleRow;

    if (firstVisibleRow.parent().isValid())
        topLeftRow = firstVisibleRow.parent();

    if (lastVisibleRow.parent().isValid())
        bottomRightRow = lastVisibleRow.parent();

    int rowCount = bottomRightRow.row();

    for (int i = topLeftRow.row(); i <= rowCount; i++) {
        QModelIndex index(topLeftRow.sibling(i, 0));

        if (index.isValid()) {
            if (index.model()->hasChildren(index) && !index.parent().isValid()) {
                for (int ci = 0; ci < index.model()->rowCount(index); ci++) {
                    QModelIndex cIndex(index.model()->index(ci, 0, index));
                    if (cIndex.isValid())
                        addJob(cIndex);
                }
            } else
                addJob(index);
        }
    }

    // processJobQueue will issue single shots to continue loading images
    if (thumbnailLoadingJobs.count() != 0)
        QTimer::singleShot(0, this, SLOT(processJobQueue()));
}

void PhoneBookImageLoader::addJob(const QModelIndex &index)
{
    QVariant data = index.data(Qt::DisplayRole);
    PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(data.value<void *>());

    // May happen if size of list model was changed
    if (entry == NULL)
        return;

    if (entry->thumbnailId.isEmpty())
        return;

    Job job;
    job.entry = entry;
    job.row = index;
    thumbnailLoadingJobs << job;
}

void PhoneBookImageLoader::stopLoadingPictures()
{
    // Lets not process any more requests to load image because list is panning now
    thumbnailLoadingJobs.clear();
}

void PhoneBookImageLoader::processJobQueue()
{
    if (thumbnailLoadingJobs.isEmpty())
        return;

    Job job = thumbnailLoadingJobs.takeFirst();

    PhoneBookEntry *entry = job.entry;
    entry->thumbnail = QImage(Utils::contactsDir() + entry->thumbnailId + ".png");
    // indicate that thumbnail was loaded
    entry->thumbnailId = "";

    if (job.row.parent().isValid())
        notifyModel(job.row.model()->index(job.row.row(), 0, job.row.parent()));
    else
        notifyModel(job.row);

    // Continue loading and letting UI thread do something
    if (thumbnailLoadingJobs.count() > 0)
        QTimer::singleShot(0, this, SLOT(processJobQueue()));
}

// There is no need to cast if you attach model to the image loader
// but this is one way to achieve same result.
void PhoneBookImageLoader::notifyModel(const QModelIndex &index)
{
    QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
    Q_ASSERT(model);
    if (model) {
        PhoneBookSortedModel *proxyModel = dynamic_cast<PhoneBookSortedModel *>(model);
        Q_ASSERT(proxyModel);
        proxyModel->thumbnailWasLoaded(index);
    }
}

PhoneBookSortedModel::PhoneBookSortedModel() : QSortFilterProxyModel(NULL), showGroups(false)
{
    // By default QSortFilterProxyModel uses Qt::DisplayRole for sorting and filter however we are
    // returning pointer in QVariant so it won't work, to solve that we need to set custom roles
    // and in the data() of model, we need to return corresponding QString
    setSortRole(PhoneBookModel::PhoneBookSortRole);
    setFilterRole(PhoneBookModel::PhoneBookFilterRole);
}

PhoneBookSortedModel::~PhoneBookSortedModel()
{

}

QModelIndex PhoneBookSortedModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return QSortFilterProxyModel::mapFromSource(sourceIndex);
}

QModelIndex PhoneBookSortedModel::mapToSource(const QModelIndex &proxyIndex) const
{
    return QSortFilterProxyModel::mapToSource(proxyIndex);
}

void PhoneBookSortedModel::thumbnailWasLoaded(const QModelIndex &index)
{
    emit dataChanged(index, index);
}

void PhoneBookSortedModel::setShowGroups(bool showGroups)
{
    if (this->showGroups != showGroups) {
        this->showGroups = showGroups;
        if (this->showGroups) {
            sort(0, Qt::AscendingOrder);
            createGroupedModel();
        }
        else
            createPlainModel();
    }
}


void PhoneBookSortedModel::createPlainModel()
{
    int plainRowCount = totalRowCount();
    treeHeaderIndex.clear();

    removeRows(0, rowCount());
    insertRows(0, plainRowCount);
}

void PhoneBookSortedModel::createGroupedModel()
{
    treeHeaderIndex.clear();

    QVector<PhoneBookEntry *> phoneBookEntries;
    for (int i = 0; i < sourceModel()->rowCount(); i++) {
        PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(sourceModel()->data(sourceModel()->index(i, 0), Qt::DisplayRole).value<void *>());
        phoneBookEntries.append(entry);
        QChar firstLetter = sourceModel()->data(sourceModel()->index(i, 0), PhoneBookModel::PhoneBookSortRole).toString()[0];

        treeHeaderIndex[firstLetter].push_back(entry);
    }

    removeRows(0, rowCount());

    insertRows(0, treeHeaderIndex.keys().count(), QModelIndex());
    for (int i = 0; i < treeHeaderIndex.keys().count(); i++) {
        QChar key = treeHeaderIndex.keys()[i];
        int count = treeHeaderIndex[key].count();
        insertRows(0, count, index(i, 0, QModelIndex()));
    }
}

int PhoneBookSortedModel::totalRowCount()
{
    int totalRowCount = 0;

    foreach(PhoneBookEntryVector entries, treeHeaderIndex.values()) {
        totalRowCount += entries.count();
    }

    return totalRowCount;
}

void PhoneBookSortedModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);

    if(showGroups)
        return;

    sourceModel()->sort(column, order);

    emit dataChanged(index(0, 0), index(rowCount(), 0));
}

QModelIndex PhoneBookSortedModel::parent(const QModelIndex &child) const
{
    return QSortFilterProxyModel::parent(child);
}

int PhoneBookSortedModel::rowCount(const QModelIndex &parent) const
{
    if (showGroups) {
        if (!parent.isValid()) {
            return treeHeaderIndex.keys().count();
        } else if (parent.row() < treeHeaderIndex.keys().count()) {
            QChar key = treeHeaderIndex.keys().at(parent.row());
            return treeHeaderIndex[key].count();
        }
    }
    return QSortFilterProxyModel::rowCount(parent);
}

bool PhoneBookSortedModel::hasChildren(const QModelIndex &parent) const
{
    if (showGroups && parent.isValid() && !parent.parent().isValid())
        return true;
    else
        return false;
}

QVariant PhoneBookSortedModel::data(const QModelIndex &index, int role) const
{
    if (showGroups && index.isValid()) {
        if (role == Qt::DisplayRole) {
            if (!index.parent().isValid()) {
                return QVariant::fromValue(treeHeaderIndex.keys().at(index.row()));
            } else {
                QChar key = treeHeaderIndex.uniqueKeys().at(index.parent().row());
                PhoneBookEntry *entry = treeHeaderIndex[key].at(index.row());
                return QVariant::fromValue(static_cast<void *>(entry));
            }
        }
    }
    if (index.isValid())
        return QSortFilterProxyModel::data(index, role);

    return QVariant();
}
