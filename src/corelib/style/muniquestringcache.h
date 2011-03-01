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

#ifndef MUNIQUESTRINGCACHE_H
#define MUNIQUESTRINGCACHE_H

#include <QByteArray>
#include <QString>
#include <QSharedPointer>

class MUniqueStringCachePrivate;

/**
  * MUniqueStringCache can be used to map strings to unique ids.
  * The mapping is backed up by a file on disk which will be mmaped
  * into memory at runtime. Therefor the ids can be shared between
  * several running programs. Additionally the memory occupied by the
  * strings will be shared between the processes.
  * Proper locking will make sure that two processes cannot add strings
  * to the cache at the same time.
  * Reading values from the cache happens without locking.
  */
class MUniqueStringCache
{
public:
    MUniqueStringCache(const QString& filename);
    ~MUniqueStringCache();

    typedef int Index;
    static const Index UndefinedIndex = -1;
    static const Index EmptyStringIndex = 0;

    /**
      * Returns true when the cache is properly attached to the file on disk.
      */
    bool isAttached();

    /**
      * Converts a given index into a string. This lookup is very fast.
      */
    QLatin1String indexToString(Index id);

    /**
      * Returns the id for a given string. If the string is not in the cache yet
      * it will be added.
      * If possible use this function only to fill the cache. To work it needs
      * to read in the whole cache file to find the given string. Depending on
      * the cache size this might be costly.
      */
    Index stringToIndex(const QByteArray& string);

    /**
      * indexToString() and stringToIndex() internally automatically lock the cache
      * to ensure that no two processes are changing the cache at the same time.
      * If you know that the cache needs to be locked very often, e.g. because stringToIndex()
      * is called often with strings likely not in the cache, this method can be used to lock
      * it just once. This avoids the need to lock and unlock it seperately for every call to
      * stringToIndex().
      * Make sure to call unlock() after all strings have been processed.
      */
    bool lock();

    /**
      * Unlocks the cache formerly locked with lock()
      */
    bool unlock();

    /**
      * Return true if the cache is locked.
      */
    bool isLocked();

private:
    friend class MUniqueStringCacheLocker;
    MUniqueStringCachePrivate * const d_ptr;
};

/**
  * Class which locks the cache when created and unlocks the cache once it gets out of scope.
  */
class MUniqueStringCacheLocker
{
public:
    MUniqueStringCacheLocker(MUniqueStringCache *cache);

    ~MUniqueStringCacheLocker();

    bool isLocked() const;

private:
    MUniqueStringCache *cache;
};

#endif // MUNIQUESTRINGCACHE_H
