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
#include "duigconfitem.h"

DuiGConfDataStore::DuiGConfDataStore(const QString &keyPath) :
    keyContainer(),
    keyPath(keyPath)
{
    DuiGConfItem rootKey(keyPath, this);
    this->keyPath += keyPath.endsWith('/') ? "" : "/";

    QList<QString> entries = rootKey.listEntries();
    foreach(const QString & entry, entries) {
        DuiGConfItem *item = new DuiGConfItem(entry, this);
        keyContainer.insert(entry.mid(this->keyPath.length()), item);
        connect(item, SIGNAL(valueChanged()), this, SLOT(gconfValueChanged()));
    }
}

DuiGConfDataStore::~DuiGConfDataStore()
{
}

bool DuiGConfDataStore::setValue(const QString &key, const QVariant &value)
{
    // Don't allow setting keys below the key path of the datastore
    if (key.indexOf('/') != -1) {
        return false;
    }

    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = keyContainer.constFind(key);
    if (it != keyContainer.constEnd()) {
        it.value()->set(value);
        return true;
    } else {
        return false;
    }
}

bool DuiGConfDataStore::createValue(const QString &key, const QVariant &value)
{
    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = keyContainer.constFind(key);
    if (it != keyContainer.constEnd()) {
        it.value()->set(value);
        return true;
    } else {
        // Don't allow creating keys below the key path of the datastore
        if (key.indexOf('/') != -1) {
            return false;
        }
        DuiGConfItem *item = new DuiGConfItem(keyPath + key, this);
        keyContainer.insert(key, item);
        item->set(value);
        return true;
    }
}

QVariant DuiGConfDataStore::value(const QString &key) const
{
    // Use an iterator and find() in order to avoid double search in the container
    KeyContainer::const_iterator it = keyContainer.find(key);
    if (it != keyContainer.end()) {
        return it.value()->value();
    } else {
        return QVariant(QVariant::Invalid);
    }
}

QStringList DuiGConfDataStore::allKeys() const
{
    return keyContainer.keys();
}

void DuiGConfDataStore::remove(const QString &key)
{
    DuiGConfItem *item = keyContainer.take(key);
    if (item != NULL) {
        item->unset();
        delete item;
    }
}

void DuiGConfDataStore::clear()
{
    foreach(DuiGConfItem * item, keyContainer.values()) {
        if (item != NULL) {
            item->unset();
            delete item;
        }
    }
    keyContainer.clear();
}

bool DuiGConfDataStore::contains(const QString &key) const
{
    return keyContainer.contains(key);
}

void DuiGConfDataStore::gconfValueChanged()
{
    DuiGConfItem *item = qobject_cast<DuiGConfItem *>(sender());
    if (item != NULL) {
        QString  key   = item->key().mid(keyPath.length());
        QVariant value = item->value();
        emit valueChanged(key, value);
    }
}
