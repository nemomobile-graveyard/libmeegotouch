/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef STUBMAP_H
#define STUBMAP_H

#include "stub.h"

// == StubMap declaration ===============================================================

template <typename K, typename T> class StubMap
{
public:
    typedef Stub<K,T> StubType;
public:
    StubMap();
    virtual ~StubMap();
    StubType *findStub(const T *proxy) const;
    StubType *findStub(const K& key) const;
    StubType *createStub(const K& key, const T *proxy = 0);
    int deleteStub(const T *proxy);
private:
    StubType Null;
    QHash<const T*, StubType*> stubs;
};

// == StubMap definition ================================================================

template <typename K, typename T>
StubMap<K, T>::StubMap()
{
}

template <typename K, typename T>
StubMap<K, T>::~StubMap()
{
}

template <typename K, typename T>
typename StubMap<K, T>::StubType *StubMap<K, T>::findStub(const T *proxy) const
{
    return stubs.value(proxy, const_cast<StubType*>(&Null));
}

template <typename K, typename T>
typename StubMap<K, T>::StubType *StubMap<K, T>::findStub(const K& key) const
{
    foreach (StubType *stub, stubs) {
        if (stub->key() == key)
            return stub;
    }

    return const_cast<StubType*>(&Null);
}

template <typename K, typename T>
typename StubMap<K, T>::StubType *StubMap<K, T>::createStub(const K& key, const T *proxy)
{
    qDebug() << "StubMap::createStub(" << key << ")";

    StubType *stub = findStub(key);

    if (stub != &Null) {
        stub->addProxy(proxy);
        return stubs.insert(proxy, stub).value();
    }

    return stubs.insertMulti(proxy, new StubType(key, proxy)).value();
}

template <typename K, typename T>
int StubMap<K, T>::deleteStub(const T *proxy)
{
    qDebug() << "StubMap::deleteStub(" << proxy << ")";

    stubs.value(proxy)->deleteProxy(proxy);

    if (!stubs.value(proxy)->hasProxies()) {
        StubType *stub = stubs.value(proxy);
        stubs.remove(proxy);
        delete stub;
    }
    return 0;
}

#endif // STUBMAP_H
