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

#ifndef STUB_H
#define STUB_H

#include "stubbase.h"
#include <QSet>

// == Stub declaration ==================================================================

template <typename K, typename T> class Stub : public StubBase
{
public:
    Stub(const K &key = K(), const T *proxy = 0);
    ~Stub();
    const K key() const;
    bool hasProxies() const;
    T *getProxy() const;
    T *addProxy(const T *proxy);
    bool deleteProxy(const T *proxy);
private:
    const K k;
    mutable QList<const T *> proxies;
};

// == Stub definition ===================================================================

template <typename K, typename T>
Stub<K, T>::Stub(const K &key, const T *proxy)
    : k(key)
{
    qDebug() << "Stub::Stub(" << k << ")";

    proxies.push_front(proxy);
}

template <typename K, typename T>
Stub<K, T>::~Stub()
{
    qDebug() << "Stub::~Stub(" << k << ")";
}

template <typename K, typename T>
const K Stub<K, T>::key() const
{
    return k;
}

template <typename K, typename T>
bool Stub<K, T>::hasProxies() const
{
    return !proxies.isEmpty();
}

template <typename K, typename T>
T *Stub<K, T>::getProxy() const
{
    return proxies.isEmpty() ? 0 : const_cast<T *>(proxies.front());
}

template <typename K, typename T>
T *Stub<K, T>::addProxy(const T *proxy)
{
    proxies.push_front(proxy);
    return const_cast<T *>(proxy);
}

template <typename K, typename T>
bool Stub<K, T>::deleteProxy(const T *proxy)
{
    return proxies.removeOne(proxy);
}

#endif // STUB_H
