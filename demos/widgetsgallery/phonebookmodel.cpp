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
#include <MSortFilterProxyModel>

PhoneBookModel::PhoneBookModel()
    : MAbstractItemModel(),
      phoneBookEntries(),
      namesList(),
      imageIdList(),
      defaultThumbnail(),
      buckets()
{
    namesList = loadFakeNames();
    imageIdList = loadFakeImageIds();
}

PhoneBookModel::~PhoneBookModel()
{
    qDeleteAll(phoneBookEntries);
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
            QString line = in.readLine().simplified();
            if (!line.isEmpty()) {
                listNames << line.split(' ');
            }
        }
    }

    // fallback
    if (listNames.isEmpty())
        listNames << "Pasi" << "Arto" << "Kiki" << "Chaim" << "Cholotov" << "Keitty" << "Kitti" << "Adam" << "Tomas" << "Jakub" << "Sergiy" << "Rocks";

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

int PhoneBookModel::groupCount() const
{
    return buckets.bucketCount();
}

int PhoneBookModel::rowCountInGroup(int group) const
{
    if (group == -1) {
        return phoneBookEntries.count();
    } else {
        return buckets.bucketSize(group);
    }
}

QString PhoneBookModel::groupTitle(int group) const
{
    return buckets.bucketName(group);
}

QVariant PhoneBookModel::itemData(int row, int group, int role) const
{
    int flatRow = row;

    if (group >= 0 && row >= 0)
        flatRow = buckets.origItemIndex(group, row);

    // This function will be called many times during fast panning, let's
    // check boundaries and validity only in debug mode
    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < phoneBookEntries.size());

    if (role == Qt::DisplayRole)
        return QVariant::fromValue(static_cast<void *>(phoneBookEntries[flatRow]));
    else if (role == PhoneBookSortRole)
        return QVariant::fromValue(phoneBookEntries[flatRow]->lastName);
    else if (role == PhoneBookFilterRole)
        return QVariant::fromValue(phoneBookEntries[flatRow]->fullName);

    return QVariant();
}

bool PhoneBookModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; // Successfully added 0 rows.

    QVector<PhoneBookEntry *> entries = phoneBookEntries;
    for (int i = entries.size(); i < count; i++) {
        PhoneBookEntry *entry = generateEntry();
        entries.append(entry);
    }

    QStringList entriesList;
    foreach (PhoneBookEntry *entry, entries) {
        Q_ASSERT(entry);
        entriesList << entry->fullName;
    }

    MLocaleBuckets entryBuckets(entriesList);

    if (entryBuckets.isEmpty()) {
        qDeleteAll(entries.begin() + phoneBookEntries.size(), entries.end());
        return false; // Something went wrong.
    }

    bool layoutHasChanged = false;
    if (entryBuckets.bucketCount() == buckets.bucketCount())
        layoutHasChanged = true;

    if (layoutHasChanged) {
        layoutAboutToBeChanged();
    }
    else {
        if (isGrouped() && !parent.isValid())
           beginInsertRows(QModelIndex(), row, row + (entryBuckets.bucketCount() - buckets.bucketCount()) - 1, false);
        else
           beginInsertRows(parent, row, row + count - 1, false);
    }


    phoneBookEntries = entries;
    buckets = entryBuckets;

    if (layoutHasChanged)
        layoutChanged();
    else
        endInsertRows();


    return true;
}

bool PhoneBookModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0)
        return true; //Successfully removed 0 rows.

    int flatRow = row;
    int group = parent.row();

    if (isGrouped() && group >= 0)
        flatRow = buckets.origItemIndex(group, row);

    Q_ASSERT(flatRow >= 0);
    Q_ASSERT(flatRow < phoneBookEntries.size());

    beginRemoveRows(parent, row, row + count - 1, count == 1);
    qDeleteAll(phoneBookEntries.begin() + flatRow, phoneBookEntries.begin() + flatRow + count - 1);
    phoneBookEntries.remove(flatRow, count);
    if (isGrouped() && group >= 0) {
        bool bucketEmpty = buckets.removeBucketItems(group, row, count);

        if (bucketEmpty)
            regenerateModel();
    }
    else
        regenerateModel();

    endRemoveRows();

    return true;
}

void PhoneBookModel::clear()
{
    if (!phoneBookEntries.isEmpty()) {
        beginResetModel();
        qDeleteAll(phoneBookEntries);
        phoneBookEntries.clear();
        buckets.clear();
        endResetModel();
    }
}

void PhoneBookModel::regenerateModel()
{
    QStringList itemList;

    foreach (PhoneBookEntry *entry, phoneBookEntries) {
        Q_ASSERT(entry);
        itemList << entry->fullName;
    }

    buckets.setItems(itemList);
}

void PhoneBookModel::thumbnailWasLoaded(const QModelIndex &index)
{
    emit dataChanged(index, index);
}

void PhoneBookModel::updateData(const QModelIndex &first, const QModelIndex &last)
{
    emit dataChanged(first, last);
}

PhoneBookImageLoader::PhoneBookImageLoader()
    : QObject(),
      thumbnailLoadingJobs()
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

    foreach (const Job& job, thumbnailLoadingJobs) {
        if (job.entry == entry)
            return;
    }

    if (entry->thumbnailId.isEmpty())
        return;

    if (!entry->thumbnail.isNull())
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
// but this is one way to achieve the same result.
void PhoneBookImageLoader::notifyModel(const QModelIndex &index)
{
    QModelIndex realIndex = index;
    QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
    Q_ASSERT(model);
    if (model) {
        MSortFilterProxyModel *sortModel = dynamic_cast<MSortFilterProxyModel *>(model);
        Q_ASSERT(sortModel);
        PhoneBookModel *phoneBookModel = dynamic_cast<PhoneBookModel *>(sortModel->sourceModel());
        if (phoneBookModel == NULL) {
            // Looks like live filtering is enabled.
            // One more cast step needed to get the correct source index.
            MSortFilterProxyModel *filterModel = dynamic_cast<MSortFilterProxyModel *>(sortModel->sourceModel());
            if (filterModel) {
                phoneBookModel = dynamic_cast<PhoneBookModel *>(filterModel->sourceModel());
                realIndex = sortModel->mapToSource(index);
                sortModel = filterModel;
            }
        }
        if (phoneBookModel != NULL && sortModel != NULL) {
            phoneBookModel->thumbnailWasLoaded(sortModel->mapToSource(realIndex));
        }
    }
}
