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

#include "duigconfdatastore.h"
#include "duigconfdatastore_p.h"
#include "duigconfitem.h"

typedef DuiGConfDataStorePrivate::KeyContainer KeyContainer;

DuiGConfDataStorePrivate::DuiGConfDataStorePrivate(const QString &keyPath) :
    keyContainer(),
    keyPath(keyPath)
{
}

DuiGConfDataStore::DuiGConfDataStore(const QString &keyPath) :
    d_ptr(new DuiGConfDataStorePrivate(keyPath))
{
    Q_D(DuiGConfDataStore);
    DuiGConfItem rootKey(keyPath, this);
    d->keyPath += keyPath.endsWith('/') ? "" : "/";

    QList<QString> entries = rootKey.listEntries();
    foreach(const QString & entry, entries) {
        DuiGConfItem *item = new DuiGConfItem(entry, this);
        d->keyContainer.insert(entry.mid(d->keyPath.length()), item);
        connect(item, SIGNAL(valueChanged()), this, SLOT(gconfValueChanged()));
    }
}

DuiGConfDataStore::~DuiGConfDataStore()
{
    delete d_ptr;
}

bool DuiGConfDataStore::setValue(const QString &key, const QVariant &value)
{
    Q_D(DuiGConfDataStore);
    // Don't allow setting keys below the key path of the datastore
    if (key.indexOf('/') != -1) {
        return false;
    }

    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = d->keyContainer.constFind(key);
    if (it != d->keyContainer.constEnd()) {
        it.value()->set(value);
        return true;
    } else {
        return false;
    }
}

bool DuiGConfDataStore::createValue(const QString &key, const QVariant &value)
{
    Q_D(DuiGConfDataStore);
    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = d->keyContainer.constFind(key);
    if (it != d->keyContainer.constEnd()) {
        it.value()->set(value);
        return true;
    } else {
        // Don't allow creating keys below the key path of the datastore
        if (key.indexOf('/') != -1) {
            return false;
        }
        DuiGConfItem *item = new DuiGConfItem(d->keyPath + key, this);
        d->keyContainer.insert(key, item);
        item->set(value);
        return true;
    }
}

QVariant DuiGConfDataStore::value(const QString &key) const
{
    Q_D(const DuiGConfDataStore);
    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = d->keyContainer.find(key);
    if (it != d->keyContainer.end()) {
        return it.value()->value();
    } else {
        return QVariant(QVariant::Invalid);
    }
}

QStringList DuiGConfDataStore::allKeys() const
{
    Q_D(const DuiGConfDataStore);
    return d->keyContainer.keys();
}

void DuiGConfDataStore::remove(const QString &key)
{
    Q_D(DuiGConfDataStore);
    DuiGConfItem *item = d->keyContainer.take(key);
    if (item != NULL) {
        item->unset();
        delete item;
    }
}

void DuiGConfDataStore::clear()
{
    Q_D(DuiGConfDataStore);
    foreach(DuiGConfItem * item, d->keyContainer.values()) {
        if (item != NULL) {
            item->unset();
            delete item;
        }
    }
    d->keyContainer.clear();
}

bool DuiGConfDataStore::contains(const QString &key) const
{
    Q_D(const DuiGConfDataStore);
    return d->keyContainer.contains(key);
}

void DuiGConfDataStore::gconfValueChanged()
{
    Q_D(DuiGConfDataStore);
    DuiGConfItem *item = qobject_cast<DuiGConfItem *>(sender());
    if (item != NULL) {
        QString  key   = item->key().mid(d->keyPath.length());
        QVariant value = item->value();
        emit valueChanged(key, value);
    }
}
