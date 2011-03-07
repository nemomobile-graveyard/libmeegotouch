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

#include "muniquestringcache_p.h"
#include "muniquestringcache.h"

#include "mdebug.h"
#include "mcomponentdata.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSystemSemaphore>
#include <QHash>
#include <QByteArray>
#include <QApplication>
#include <QScopedPointer>

namespace {
const int MAX_CACHE_SIZE = 1024*1024;
#define CACHE_NAME "MTF_UNIQUE_STRING_CACHE"

// slightly adapted from the version found in Qt's corelib/tools/qhash.cpp
static uint hash(const uchar *p)
{
    uint h = 0;

    while (*p) {
        h = (h << 4) + *p++;
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}

}

uint qHash(const QLatin1String &key)
{
    return hash(reinterpret_cast<const uchar *>(key.latin1()));
}

class UniqueStringCacheMappedMemory
{
public:
    UniqueStringCacheMappedMemory(const QString& filename)
        : cacheFile(filename),
        accessSemaphore(filename + "_UNIQUE_STRING_SEMAPHORE", 1),
        rawMappedMemory(0),
        attached(false),
        locked(false)
    {
        if (!accessSemaphore.acquire()) {
            mWarning("UniqueStringCacheMappedMemory::UniqueStringCacheMappedMemory") << "Unable to aquire semaphore:" << accessSemaphore.errorString();
            return;
        }

        attachToCache();

        accessSemaphore.release();
    }

    bool isAttached() const { return attached; }

    uchar* data() { return rawMappedMemory; }
    QString errorString() { return accessSemaphore.errorString(); }

    ~UniqueStringCacheMappedMemory()
    {
    }

    bool isLocked()
    {
        return locked;
    }

    bool lock() {
        bool result = accessSemaphore.acquire();
        locked = result;
        return result;

    }
    bool unlock() {
        locked = false;
        return accessSemaphore.release();
    }

private:
    friend class MUniqueStringCacheLocker;

    void attachToCache()
    {
        if (!cacheFile.exists()) {
            if (!initializeCache()) {
                attached = false;
                return;
            }
        }

        if (!cacheFile.open(QIODevice::ReadWrite)) {
            mWarning("UniqueStringCacheMappedMemory") << "Could not open" <<
                cacheFile.fileName();
            return;
        }

        if (cacheFile.size() != MAX_CACHE_SIZE) {
            mWarning("UniqueStringCacheMappedMemory") << "Wrong cache file size" <<
                cacheFile.size() << "Expected:" << MAX_CACHE_SIZE;
            return;
        }

        rawMappedMemory = cacheFile.map(0, MAX_CACHE_SIZE);

        attached = true;

        cacheFile.close();
    }

    bool initializeCache() {
        if (!cacheFile.open(QFile::WriteOnly)) {
            //Maybe it failed because the directory doesn't exist
            QDir().mkpath(QFileInfo(cacheFile.fileName()).absolutePath());
            if (!cacheFile.open(QFile::WriteOnly)) {
                mWarning("UniqueStringCacheMappedMemory") <<
                         "Wrong permissions for cache directory. Cannot create" <<
                          cacheFile.fileName();
                return false;
            }
        }
        cacheFile.resize(MAX_CACHE_SIZE);
        cacheFile.close();
        return true;
    }

    QFile cacheFile;
    QSystemSemaphore accessSemaphore;
    uchar* rawMappedMemory;
    bool attached;
    bool locked;
};

MUniqueStringCachePrivate::MUniqueStringCachePrivate(MUniqueStringCache *parent, const QString &filename)
    : q_ptr(parent),
    filename(filename),
    uniqueStringCacheMappedMemory(new UniqueStringCacheMappedMemory(filename)),
    offset(sizeof(int)),
    cacheFilled(false),
    stringToIdCacheFilled(false)
{
}

MUniqueStringCachePrivate::~MUniqueStringCachePrivate()
{
    delete uniqueStringCacheMappedMemory;
}

// fills our datastructures from memory
// this method can be called several times to update the internal representation
// if the cache has been changed by another application
void MUniqueStringCachePrivate::fillUniqueStringCache() {
    uchar *rawCache = uniqueStringCacheMappedMemory->data();
    int elementsInCache = *reinterpret_cast<int*>(rawCache);
    int oldSize = idToPointerCache.count();
    idToPointerCache.resize(elementsInCache);

    for (int i = oldSize; i < elementsInCache; ++i) {
        uchar *stringAdress = rawCache + offset;
        int stringLength = *reinterpret_cast<int*>(stringAdress);

        idToPointerCache[i] = stringAdress;

        offset += sizeof(int) + stringLength + 1;

        if (stringToIdCacheFilled) {
            stringToIdCache.insert(QLatin1String(q_ptr->indexToString(i)), i);
        }
    }
    if (elementsInCache == 0) {
        // we want the empty string as first element
        insertStringToCache(QByteArray());
    }

    mDebug("MUniqueStringCachePrivate::fillUniqueStringCache")
            << QString("Elements in cache %1: %2, %3% filled").arg(filename).arg(elementsInCache).arg((float)offset/MAX_CACHE_SIZE*100);
}

// insert one string into the cache, makes sure to handle it gracefully when the cache has
// changed before it has been locked
int MUniqueStringCachePrivate::insertStringToCache(const QByteArray &string) {
    uchar *rawCache = uniqueStringCacheMappedMemory->data();
    int elementsInCache = *reinterpret_cast<int*>(rawCache);
    if (elementsInCache != idToPointerCache.count()) {
        // cache has changed, check if the string has been added
        // in the meanwhile
        fillUniqueStringCache();
        if (!stringToIdCacheFilled) {
            fillStringToIdCache();
        }
        StringToIdCache::const_iterator it = stringToIdCache.constFind(QLatin1String(string.constData()));
        if (it != stringToIdCache.constEnd()) {
            return *it;
        }
    }

    ++*reinterpret_cast<int*>(rawCache);

    uchar *stringAdress = rawCache + offset;
    *reinterpret_cast<int*>(stringAdress) = string.length();
    memcpy(stringAdress + sizeof(int), string.constData(), string.length() + 1);

    offset += sizeof(int) + string.length() + 1;
    if (offset >= MAX_CACHE_SIZE) {
        qFatal("unique string cache is full");
    }

    idToPointerCache.append((uchar*)stringAdress);
    stringToIdCache.insert(QLatin1String(reinterpret_cast<const char*>(stringAdress + sizeof(int))), idToPointerCache.count() - 1);

    return idToPointerCache.count() - 1;
}

// triggers initialization of our data structures from the cache if this
// did not happen yet
void MUniqueStringCachePrivate::initiallyFillCache() {
    if (!cacheFilled) {
        QScopedPointer<MUniqueStringCacheLocker> locker;
        if (!q_ptr->isLocked()) {
            locker.reset(new MUniqueStringCacheLocker(q_ptr));
            if (!locker->isLocked()) {
                return;
            }
        }
        fillUniqueStringCache();
        cacheFilled = true;
    }
}

void MUniqueStringCachePrivate::fillStringToIdCache()
{
    mDebug("MUniqueStringCachePrivate::fillStringToIdCache") << "filling stringToIdCache for" << filename;
    for (int i = 0; i < idToPointerCache.count(); ++i) {
        stringToIdCache.insert(QLatin1String(q_ptr->indexToString(i)), i);
    }
    stringToIdCacheFilled = true;
}

//////////////////////////////////////

MUniqueStringCache::MUniqueStringCache(const QString& filename)
    : d_ptr(new MUniqueStringCachePrivate(this, filename))
{
}

MUniqueStringCache::~MUniqueStringCache()
{
    delete d_ptr;
}

bool MUniqueStringCache::isAttached()
{
    return d_ptr->uniqueStringCacheMappedMemory->isAttached();
}

MUniqueStringCache::Index MUniqueStringCache::stringToIndex(const QByteArray& string)
{
    if (string.isEmpty()) {
        return EmptyStringIndex;
    }

    d_ptr->initiallyFillCache();

    if (!d_ptr->stringToIdCacheFilled) {
        d_ptr->fillStringToIdCache();
    }

    MUniqueStringCachePrivate::StringToIdCache::const_iterator it = d_ptr->stringToIdCache.constFind(QLatin1String(string.constData()));
    if (it != d_ptr->stringToIdCache.constEnd()) {
        return *it;
    }

    // string is unknown, we need to add it to the cache
    QScopedPointer<MUniqueStringCacheLocker> locker;
    if (!isLocked()) {
        locker.reset(new MUniqueStringCacheLocker(this));
        if (!locker->isLocked()) {
            return UndefinedIndex;
        }
    }
    return d_ptr->insertStringToCache(string);
}

QLatin1String MUniqueStringCache::indexToString(Index id)
{
    d_ptr->initiallyFillCache();
    if (id < 0) {
        return indexToString(EmptyStringIndex);
    } else if (id >= d_ptr->idToPointerCache.count())  {
        // it may be a new string in the cache. try to update the cache and then check again
        QScopedPointer<MUniqueStringCacheLocker> locker;
        if (!isLocked()) {
            locker.reset(new MUniqueStringCacheLocker(this));
            if (!locker->isLocked()) {
                return indexToString(EmptyStringIndex);
            }
        }
        d_ptr->fillUniqueStringCache();
        if (id >= d_ptr->idToPointerCache.count()) {
            mWarning("MUniqueStringCache::indexToString") << "Id" << id << "is unknown. Has the string cache been deleted?" << d_ptr->idToPointerCache.count();
            return indexToString(EmptyStringIndex);
        }
    }

    uchar *stringAdress = d_ptr->idToPointerCache.at(id);
    return QLatin1String(reinterpret_cast<const char*>(stringAdress + sizeof(int)));
}

bool MUniqueStringCache::lock()
{
    return d_ptr->uniqueStringCacheMappedMemory->lock();
}

bool MUniqueStringCache::unlock()
{
    return d_ptr->uniqueStringCacheMappedMemory->unlock();
}

bool MUniqueStringCache::isLocked()
{
    return d_ptr->uniqueStringCacheMappedMemory->isLocked();
}

////////////////////////

MUniqueStringCacheLocker::MUniqueStringCacheLocker(MUniqueStringCache *usCache)
    : cache(usCache->lock() ? usCache : (MUniqueStringCache *)0)
{
    if (!cache) {
        mWarning("MUniqueStringCacheLocker") << "Unable to lock unique string cache" << usCache->d_ptr->uniqueStringCacheMappedMemory->accessSemaphore.errorString();
    }
}

MUniqueStringCacheLocker::~MUniqueStringCacheLocker()
{
    if (!cache) {
        return;
    }
    if (!cache->unlock()) {
        mWarning("MUniqueStringCacheLocker") << "Unable to unlock unique string cache" << cache->d_ptr->uniqueStringCacheMappedMemory->accessSemaphore.errorString();
    }
}

bool MUniqueStringCacheLocker::isLocked() const
{
    return cache;
}
