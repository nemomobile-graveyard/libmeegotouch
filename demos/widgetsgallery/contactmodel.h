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

#ifndef _CONTACTMODEL_H_
#define _CONTACTMODEL_H_

#include <QAbstractListModel>
#include <QStringList>
#include <QSize>
#include <QPixmap>

class Contact;
class ContactImageLoader;

class ContactModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ContactModel(QObject *parent=0);
    ~ContactModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    void avatarLoaded(const QModelIndex& index);

signals:
    void rowCountChanged(int rows);

private:
    class Private;
    Private *priv;

    friend class Contact;
};

class Contact
{
public:
    ~Contact();
    const QString getUID() const;
    const QString getName() const;
    const QStringList getPhoneNumbers() const;
    const QStringList getEmailAddresses() const;
    const QPixmap getAvatar() const;
    const void loadAvatar();

    static bool equals(const Contact *a, const Contact *b);
    static bool collate(Contact* a, Contact* b, Qt::SortOrder);

    class Collator
    {
    public:
        Collator(Qt::SortOrder o) : order(o) {}
        bool operator()(Contact *a, Contact *b)
        {
            return collate(a, b, order);
        }
    private:
        Qt::SortOrder order;
    };

private:
    Contact();
    class Private;
    Private  *priv;

    friend class ContactModel::Private;
};

Q_DECLARE_METATYPE(Contact*)

class ContactImageLoader : public QObject
{
    Q_OBJECT

public:
    struct Job{
        Contact *contact;
        QModelIndex row;
    };

    ContactImageLoader();
    virtual ~ContactImageLoader();

public Q_SLOTS:
    void loadPictures(const QModelIndex& firstVisibleRow, const QModelIndex& lastVisibleRow);
    void stopLoadingPictures();
    void processJobQueue();

private:
    void notifyModel(const QModelIndex& index);

private:
    QList<Job> thumbnailLoadingJobs;
};

#endif
