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

#include "mgconfdatastore.h"
#include "mgconfdatastore_p.h"
#include "mgconfitem.h"

typedef MGConfDataStorePrivate::KeyContainer KeyContainer;

MGConfDataStorePrivate::MGConfDataStorePrivate(const QString &keyPath) :
    keyContainer(),
    keyPath(keyPath)
{
}

MGConfDataStore::MGConfDataStore(const QString &keyPath) :
    d_ptr(new MGConfDataStorePrivate(keyPath))
{
    Q_D(MGConfDataStore);
    MGConfItem rootKey(keyPath, this);
    d->keyPath += keyPath.endsWith('/') ? "" : "/";

    QList<QString> entries = rootKey.listEntries();
    foreach(const QString & entry, entries) {
        MGConfItem *item = new MGConfItem(entry, this);
        d->keyContainer.insert(entry.mid(d->keyPath.length()), item);
        connect(item, SIGNAL(valueChanged()), this, SLOT(gconfValueChanged()));
    }
}

MGConfDataStore::~MGConfDataStore()
{
    delete d_ptr;
}

bool MGConfDataStore::setValue(const QString &key, const QVariant &value)
{
    Q_D(MGConfDataStore);
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

bool MGConfDataStore::createValue(const QString &key, const QVariant &value)
{
    Q_D(MGConfDataStore);
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
        MGConfItem *item = new MGConfItem(d->keyPath + key, this);
        d->keyContainer.insert(key, item);
        item->set(value);
        return true;
    }
}

QVariant MGConfDataStore::value(const QString &key) const
{
    Q_D(const MGConfDataStore);
    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = d->keyContainer.find(key);
    if (it != d->keyContainer.end()) {
        return it.value()->value();
    } else {
        return QVariant(QVariant::Invalid);
    }
}

QStringList MGConfDataStore::allKeys() const
{
    Q_D(const MGConfDataStore);
    return d->keyContainer.keys();
}

void MGConfDataStore::remove(const QString &key)
{
    Q_D(MGConfDataStore);
    MGConfItem *item = d->keyContainer.take(key);
    if (item != NULL) {
        item->unset();
        delete item;
    }
}

void MGConfDataStore::clear()
{
    Q_D(MGConfDataStore);
    foreach(MGConfItem * item, d->keyContainer.values()) {
        if (item != NULL) {
            item->unset();
            delete item;
        }
    }
    d->keyContainer.clear();
}

bool MGConfDataStore::contains(const QString &key) const
{
    Q_D(const MGConfDataStore);
    return d->keyContainer.contains(key);
}

void MGConfDataStore::gconfValueChanged()
{
    Q_D(MGConfDataStore);
    MGConfItem *item = qobject_cast<MGConfItem *>(sender());
    if (item != NULL) {
        QString  key   = item->key().mid(d->keyPath.length());
        QVariant value = item->value();
        emit valueChanged(key, value);
    }
}
