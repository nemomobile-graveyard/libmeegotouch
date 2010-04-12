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

#ifndef PHONEBOOKMODEL_H
#define PHONEBOOKMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QVector>
#include <QImage>

// Structure which contain data for each row
struct PhoneBookEntry {
    QString firstName; // store first and last names for searching
    QString lastName;
    QString fullName; // store fullname so that we don't need to concantenate first and last name
    QString phoneNumber;
    QImage thumbnail; // will be filled with empty image, after real image will be loaded it will replace old one
    QString thumbnailId; // thumbnail will represent an id of image to load, if it's empty, no loading will be triggered
};

class PhoneBookImageLoader : public QObject
{
    Q_OBJECT

public:
    struct Job {
        PhoneBookEntry *entry;
        QModelIndex row;
    };

    PhoneBookImageLoader();
    virtual ~PhoneBookImageLoader();

public Q_SLOTS:
    void loadPictures(const QModelIndex &firstVisibleRow, const QModelIndex &lastVisibleRow);
    void stopLoadingPictures();
    void processJobQueue();

private:
    void notifyModel(const QModelIndex &index);

private:
    QList<Job> thumbnailLoadingJobs;
};

class PhoneBookModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    // Defining roles here which will be used for sorting and filtering in PhoneBookSortedModel
    enum PhoneBookRoles {
        PhoneBookSortRole = Qt::UserRole + 1,
        PhoneBookFilterRole
    };

    PhoneBookModel();
    virtual ~PhoneBookModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void thumbnailWasLoaded(const QModelIndex &index);

private:
    QStringList loadFakeNames();
    QStringList loadFakeImageIds();
    PhoneBookEntry *generateEntry();
    QString generatePhoneNumber();

private:
    QVector<PhoneBookEntry *> phoneBookEntries;
    QStringList namesList;
    QStringList imageIdList;
    QImage defaultThumbnail;
};

class PhoneBookSortedModel : public QSortFilterProxyModel
{
public:
    PhoneBookSortedModel();
    virtual ~PhoneBookSortedModel();
    void thumbnailWasLoaded(const QModelIndex &index);

    void setShowGroups(bool showGroups);

public:
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public:
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

private:
    void populateGroupHeaderIndex();

private:
    bool showGroups;
    QHash<QChar, int> groupHeaderIndex;
};

#endif
