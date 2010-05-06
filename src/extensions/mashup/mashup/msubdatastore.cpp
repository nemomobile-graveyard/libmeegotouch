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

#include "msubdatastore.h"

MSubDataStore::MSubDataStore(const QString &prefix, MDataStore &baseStore) :
    _prefix(prefix),
    _prefixLength(prefix.size()),
    _baseStore(baseStore)
{
    connect(&baseStore, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(baseStoreValueChanged(QString, QVariant)));
}

MSubDataStore::~MSubDataStore()
{
}

QString MSubDataStore::prefix() const
{
    return _prefix;
}

bool MSubDataStore::createValue(const QString &key, const QVariant &value)
{
    return _baseStore.createValue(_prefix + key, value);
}


bool MSubDataStore::setValue(const QString &key, const QVariant &value)
{
    return _baseStore.setValue(_prefix + key, value);
}


QVariant MSubDataStore::value(const QString &key) const
{
    return _baseStore.value(_prefix + key);
}

QStringList MSubDataStore::allKeys() const
{
    QStringList keys = _baseStore.allKeys();
    QStringList result;
    foreach(QString aKey, keys) {
        if (aKey.startsWith(_prefix)) {
            result.append(aKey.remove(0, _prefixLength));
        }
    }
    return result;
}

void MSubDataStore::remove(const QString &key)
{
    _baseStore.remove(_prefix + key);
}

void MSubDataStore::clear()
{
    QStringList keys = _baseStore.allKeys();

    foreach(const QString & aKey, keys) {
        if (aKey.startsWith(_prefix)) {
            _baseStore.remove(aKey);
        }
    }
}

bool MSubDataStore::contains(const QString &key) const
{
    return _baseStore.contains(_prefix + key);
}

void MSubDataStore::baseStoreValueChanged(const QString &key, const QVariant &value)
{
    if (key.startsWith(_prefix)) {
        emit valueChanged(key.mid(_prefixLength), value);
    }
}
