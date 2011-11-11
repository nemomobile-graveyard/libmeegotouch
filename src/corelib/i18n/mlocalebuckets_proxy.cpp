/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mlocalebuckets.h"
#include "mlocale/mlocalebuckets.h"
#include "mlocale.h"

static bool g_mLocaleInited = false;

class MLocaleBucketsPrivate
{
public:
    MLocaleBucketsPrivate() : pBuckets( 0 )
        {
            if ( ! g_mLocaleInited )
            {
                MLocale::initMLocale();
                g_mLocaleInited = true;
            }
        }
    ~MLocaleBucketsPrivate()
        {
            delete pBuckets;
        }

    ML10N::MLocaleBuckets *pBuckets;
};


MLocaleBuckets::MLocaleBuckets():
    d_ptr(new MLocaleBucketsPrivate())
{
    d_ptr->pBuckets = new ML10N::MLocaleBuckets();
}

MLocaleBuckets::MLocaleBuckets(const MLocaleBuckets &other)
    : d_ptr(new MLocaleBucketsPrivate)
{
    d_ptr->pBuckets = new ML10N::MLocaleBuckets( *other.d_ptr->pBuckets );
}

MLocaleBuckets::MLocaleBuckets(const QStringList &unsortedItems, Qt::SortOrder sortOrder) :
    d_ptr(new MLocaleBucketsPrivate())
{
    d_ptr->pBuckets = new ML10N::MLocaleBuckets( unsortedItems, sortOrder );
}

MLocaleBuckets &MLocaleBuckets::operator=(const MLocaleBuckets &other)
{
    *d_ptr->pBuckets = *other.d_ptr->pBuckets;
    return *this;
}

MLocaleBuckets::~MLocaleBuckets()
{
    delete d_ptr;
}

void MLocaleBuckets::setItems(const QStringList &items, Qt::SortOrder sortOrder)
{
    d_ptr->pBuckets->setItems( items, sortOrder );
}

int MLocaleBuckets::bucketCount() const
{
    return d_ptr->pBuckets->bucketCount();
}

QString MLocaleBuckets::bucketName(int bucketIndex) const
{
    return d_ptr->pBuckets->bucketName( bucketIndex );
}

QString MLocaleBuckets::bucketName(const QString &item) const
{
    return d_ptr->pBuckets->bucketName( item );
}

int MLocaleBuckets::bucketIndex(const QString &bucketName) const
{
    return d_ptr->pBuckets->bucketIndex( bucketName );
}

QStringList MLocaleBuckets::bucketContent(int bucketIndex) const
{
    return d_ptr->pBuckets->bucketContent( bucketIndex );
}

int MLocaleBuckets::origItemIndex(int bucketIndex, int indexInBucket) const
{
    return d_ptr->pBuckets->origItemIndex( bucketIndex, indexInBucket );
}

int MLocaleBuckets::bucketSize(int bucketIndex) const
{
    return d_ptr->pBuckets->bucketSize( bucketIndex );
}

bool MLocaleBuckets::isEmpty() const
{
    return d_ptr->pBuckets->isEmpty();
}

void MLocaleBuckets::clear()
{
    d_ptr->pBuckets->clear();
}

bool MLocaleBuckets::removeBucketItems(int bucketIndex, int itemIndex, int count)
{
    return d_ptr->pBuckets->removeBucketItems( bucketIndex, itemIndex, count );
}

void MLocaleBuckets::removeEmptyBucket(int bucketIndex)
{
    return d_ptr->pBuckets->removeEmptyBucket( bucketIndex );
}
