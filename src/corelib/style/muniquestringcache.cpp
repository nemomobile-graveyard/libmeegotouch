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

#include "muniquestringcache.h"

#include "mdebug.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSystemSemaphore>
#include <QHash>
#include <QByteArray>
#include <QElapsedTimer>
#include <QCoreApplication>

const int MAX_CACHE_SIZE = 1024*1024;
#define CACHE_NAME "MTF_UNIQUE_STRING_CACHE"

class UniqueStringCacheMappedMemory
{
public:
    UniqueStringCacheMappedMemory()
        : cacheFile(createCacheFileName()),
        accessSemaphore(QLatin1String(CACHE_NAME "_SEMAPHORE"), 1),
        attached(false)
    {
        if (!accessSemaphore.acquire()) {
            qWarning() << "Unable to aquire semaphore:" << accessSemaphore.errorString();
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

private:
    friend class UniqueStringCacheMappedMemoryLocker;
    bool lock() { return accessSemaphore.acquire(); }
    bool unlock() { return accessSemaphore.release(); }

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

    QString createCacheFileName() {
        QString cacheDir;
#ifdef Q_OS_WIN
	QDir appDir(QCoreApplication::applicationDirPath());
	appDir.cdUp();

	cacheDir = appDir.absolutePath()
	  + QDir::separator() + "var"
	  + QDir::separator() + "cache"
	  + QDir::separator() + "meegotouch";
#else
	cacheDir = QString(CACHEDIR);
#endif
        cacheDir = cacheDir
	  + QDir::separator() + "css"
	  + QDir::separator() + CACHE_NAME;

        return cacheDir;
    }

    QFile cacheFile;
    QSystemSemaphore accessSemaphore;
    uchar* rawMappedMemory;
    bool attached;
};

Q_GLOBAL_STATIC(UniqueStringCacheMappedMemory, uniqueStringCacheMappedMemory)


class UniqueStringCacheMappedMemoryLocker
{
public:
    UniqueStringCacheMappedMemoryLocker(UniqueStringCacheMappedMemory *cache)
        : cache(cache->lock() ? cache : (UniqueStringCacheMappedMemory *)0)
    {
        if (!cache) {
            mWarning("UniqueStringCacheMappedMemoryLocker") << "Unable to lock unique string cache" << cache->errorString();
        }
    }

    bool isLocked() const
    {
        return cache;
    }

    ~UniqueStringCacheMappedMemoryLocker()
    {
        if (!cache) {
            return;
        }
        if (!cache->unlock()) {
            mWarning("UniqueStringCacheMappedMemoryLocker") << "Unable to unlock unique string cache" << cache->errorString();
        }
    }

private:
    UniqueStringCacheMappedMemory *cache;
};

// mapping id => string
typedef QVector<QByteArray> IdToStringCache;
Q_GLOBAL_STATIC(IdToStringCache, idToStringCache)
// mapping string => id
typedef QHash<QByteArray, int> StringToIdCache;
Q_GLOBAL_STATIC(StringToIdCache, stringToIdCache)
static int offset = sizeof(int);

int insert_string_to_cache(const QByteArray &string);

// fills our datastructures from memory
// this method can be called several times to update the internal representation
// if the cache has been changed by another application
void fill_unique_string_cache() {
    uchar *rawCache = uniqueStringCacheMappedMemory()->data();
    int elementsInCache = *reinterpret_cast<int*>(rawCache);
    int oldSize = idToStringCache()->count();
    idToStringCache()->resize(elementsInCache);

    for (int i = oldSize; i < elementsInCache; ++i) {
        uchar *stringAdress = rawCache + offset;
        int stringLength = *reinterpret_cast<int*>(stringAdress);
        // TODO: figure out if we can delay creating the actual byte arrays. as long as stringForId()
        // is not called we do not need to actually create them.
        // first: check if this makes a difference at all
        QByteArray string = QByteArray::fromRawData(reinterpret_cast<const char*>(stringAdress + sizeof(int)), stringLength);
        offset += sizeof(int) + stringLength;

        (*idToStringCache())[i] = string;
        stringToIdCache()->insert(string, i);
    }
    if (elementsInCache == 0) {
        // we want the empty string as first element
        insert_string_to_cache(QByteArray());
    }

    qWarning() << "elements in cache:" << elementsInCache << ", " << (float)offset/MAX_CACHE_SIZE*100 << "% filled cache";
}

// insert one string into the cache, makes sure to handle it gracefully when the cache has
// changed before it has been locked
int insert_string_to_cache(const QByteArray &string) {
    uchar *rawCache = uniqueStringCacheMappedMemory()->data();
    int elementsInCache = *reinterpret_cast<int*>(rawCache);
    if (elementsInCache != idToStringCache()->count()) {
        // cache has changed, check if the string has been added
        // in the meanwhile
        fill_unique_string_cache();
        StringToIdCache::const_iterator it = stringToIdCache()->constFind(string);
        if (it != stringToIdCache()->constEnd()) {
            return *it;
        }
    }

    ++*reinterpret_cast<int*>(rawCache);

    uchar *stringAdress = rawCache + offset;
    *reinterpret_cast<int*>(stringAdress) = string.length();
    memcpy(stringAdress + sizeof(int), string.constData(), string.length());

    offset += sizeof(int) + string.length();
    if (offset >= MAX_CACHE_SIZE) {
        qFatal("unique string cache is full");
    }

    idToStringCache()->append(string);
    stringToIdCache()->insert(string, idToStringCache()->count() - 1);

    return idToStringCache()->count() - 1;
}

// triggers initialization of our data structures from the cache if this
// did not happen yet
void initially_fill_cache() {
    static bool cacheFilled = false;
    if (!cacheFilled) {
        QElapsedTimer timer;
        timer.start();
        UniqueStringCacheMappedMemoryLocker locker(uniqueStringCacheMappedMemory());
        if (!locker.isLocked()) {
            return;
        }
        fill_unique_string_cache();

        cacheFilled = true;
    }
}

bool MUniqueStringCache::isAttached()
{
    UniqueStringCacheMappedMemoryLocker locker(uniqueStringCacheMappedMemory());
    if (!locker.isLocked()) {
        return false;
    }
    return uniqueStringCacheMappedMemory()->isAttached();
}

MUniqueStringCache::Index MUniqueStringCache::stringToIndex(const QByteArray& string)
{
    initially_fill_cache();
    StringToIdCache::const_iterator it = stringToIdCache()->constFind(string);
    if (it != stringToIdCache()->constEnd()) {
        return *it;
    }

    // string is unknown, we need to add it to the cache
    UniqueStringCacheMappedMemoryLocker locker(uniqueStringCacheMappedMemory());
    if (!locker.isLocked()) {
        return UndefinedIndex;
    }

    return insert_string_to_cache(string);
}

QByteArray MUniqueStringCache::indexToString(Index id)
{
    initially_fill_cache();
    if (id < 0) {
        return QByteArray();
    } else if (id >= idToStringCache()->count())  {
        mWarning("MUniqueStringCache::indexToString") << "Id" << id << "is unknown. Has the string cache been deleted?";
        return QByteArray();
    }

    return idToStringCache()->at(id);
}
