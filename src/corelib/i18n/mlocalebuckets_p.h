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

#ifndef MLOCALEBUCKETS_P_H
#define MLOCALEBUCKETS_P_H

#include <QStringList>
#include <QVector>

#include "mlocale.h"
#ifdef HAVE_ICU
#  include "mcollator.h"
#endif

class MLocaleBuckets;

class MLocaleBucketsPrivate
{
    Q_DECLARE_PUBLIC(MLocaleBuckets)

    MLocaleBucketsPrivate();
    void copy(const MLocaleBuckets &other);

    void setItems(const QStringList &items, Qt::SortOrder sortOrder);
    void clear();
    bool removeBucketItems(int bucketIndex, int itemIndex, int count);
    void removeEmptyBucket(int bucketIndex);

    //
    // Data members
    //

    MLocale locale;
#ifdef HAVE_ICU
    MCollator collator;
#endif
    QStringList allBuckets;
    QStringList buckets; // used buckets
    QVector<QStringList> bucketItems;
    // Intentionally not using QList to avoid flattening the list
    // when trying to append another QStringList
    QVector<QList<int> > origIndices;

    MLocaleBuckets *q_ptr;
};


// Helper class to retain the original index for items during sorting
struct MLocaleBucketItem
{
    QString text;
    int origIndex;

    MLocaleBucketItem(QString text, int origIndex) :
        text(text), origIndex(origIndex) {}
};



// Functor for qStableSort() comparison
class MLocaleBucketItemComparator
{
public:
    MLocaleBucketItemComparator(Qt::SortOrder sortOrder = Qt::AscendingOrder):
#ifdef HAVE_ICU
        collator(MLocale()),
#endif
        sortOrder(sortOrder)
        {}

    bool operator()(const MLocaleBucketItem &left, const MLocaleBucketItem &right)
    {
#ifdef HAVE_ICU
        return sortOrder == Qt::DescendingOrder ?
            collator(right.text, left.text) :
            collator(left.text, right.text);
#else
        return sortOrder == Qt::DescendingOrder ?
            (right.text < left.text) :
            (left.text < right.text);
#endif
    }

private:
#ifdef HAVE_ICU
    MCollator collator;
#endif
    Qt::SortOrder sortOrder;
};


#endif // MLOCALEBUCKETS_P_H
