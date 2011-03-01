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

#ifndef MUNIQUESTRINGCACHE_P_H
#define MUNIQUESTRINGCACHE_P_H

#include <QString>
#include <QHash>
#include <QByteArray>
#include <QVector>

class MUniqueStringCache;
class UniqueStringCacheMappedMemory;

class MUniqueStringCachePrivate {
public:
    MUniqueStringCachePrivate(MUniqueStringCache *parent, const QString& filename);
    ~MUniqueStringCachePrivate();

    void fillUniqueStringCache();
    int insertStringToCache(const QByteArray &string);
    void initiallyFillCache();
    QByteArray stringFromOffset(uchar *stringAdress);
    void fillStringToIdCache();

    MUniqueStringCache *q_ptr;

    QString filename;

    UniqueStringCacheMappedMemory *uniqueStringCacheMappedMemory;

    // mapping id => offset
    QVector<uchar*> idToPointerCache;
    // mapping string => id
    typedef QHash<QLatin1String, int> StringToIdCache;
    StringToIdCache stringToIdCache;
    int offset;

    bool cacheFilled;
    bool stringToIdCacheFilled;
};

#endif
