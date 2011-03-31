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

#ifndef MLOCALEBUCKETS_H
#define MLOCALEBUCKETS_H

#include "mexport.h"
#include <QStringList>

class MLocaleBucketsPrivate;

/*!
 * \class MLocaleBuckets
 *
 * \brief MLocaleBuckets is a helper class to handle locale dependent "sort
 * buckets", i.e., groups of list items that start with a common character.
 *
 * In English, this is as simple as taking the first character from each list
 * item, i.e. "Smith" -> "S", "Snyder" -> "S", "Taylor" -> "T", etc. where "S"
 * and "T" are the bucket names.
 *
 * In other locales, there are more characters that might belong to the same
 * bucket, i.e. "Olechot" -> "O", "Ömer" -> "O" (not "Ö"!), maybe even more
 * than one real character long like in Czech: "Cholotov" -> "Ch", not "C", and
 * "Ch" is to be sorted after "H", not after "C". Each locale may have its own
 * set of rules, and users expect lists (contacts, music titles) to be grouped
 * and sorted accordingly. This is what this class does.
 *
 * This class is designed to integrate well with MAbstractItemModel which is
 * used in MList and MComboBox. Create one MLocaleBuckets object for all items
 * in your list.
 *
 * Example:
 *
 * Original items (they don't need to be sorted, this just makes the example
 * easier to understand):
 *
 *   - Agnetha
 *   - Anna
 *   - Bernardo
 *   - Chaim
 *   - Christopher
 *   - Claudia
 *   - Halvar
 *   - Hendrik
 *   - Olund
 *   - Yannick
 *   - Ömer
 *
 * Sorted and grouped in a Czech locale:
 *
 *   - A
 *     - Agnetha
 *     - Anna
 *   - B
 *     - Bernardo
 *   - C
 *     - Claudia
 *   - H
 *     - Halvar
 *     - Hendrik
 *   - CH
 *     - Chaim
 *     - Christopher
 *   - O
 *     - Olund
 *     - Ömer
 *   - Y
 *     - Yannick
 *
 * "A", "B", "C", "H", "CH", "O", "Y" are the bucket names in this example.
 *
 * "Chaim" has the original item index 4 ("Agnetha" being #0, "Anna" #1,
 * "Bernardo" #3). Since this was done in a Czech locale, there is a separate
 * "CH" bucket (which has bucket index #4 after "A" #0, "B" #1, "C" #2, "H" #3)
 * where "Chaim" belongs to with item index #0 in that "CH" bucket .
 *
 * There are no buckets "D", "E", "F", "G" since there were no items that would
 * belong to any of them.
 *
 * Notice that in some locales (notably Chinese, Japanese, Korean) strings might
 * be sorted into buckets with a character that doesn't even appear in that
 * word. For example, in Japanese, all of ワ,ゐ,ヰ,ヸ, ヹ, を, ヲ, ヺ, are
 * sorted into the わ bucket.
 *
 * \sa MAbstractItemModel
 * \sa MLocale::indexBucket()
 */

class M_CORE_EXPORT MLocaleBuckets
{
public:
    /*!
     * \brief Constructor: Create an empty MLocaleBuckets object with the
     * current locale.
     *
     * Remember to call setItems() at some point.
     */
    MLocaleBuckets();

    /*!
     * \brief Constructor: Copies locale buckets from the other reference.
     */
    MLocaleBuckets(const MLocaleBuckets &other);

    /*!
     * \brief Constructor: Create an MLocaleBuckets object from the specified
     * list of items with the current locale.
     *
     * This is equivalent to creating an empty MLocaleBuckets object and
     * calling setItems() with that item list.
     */
    explicit MLocaleBuckets(const QStringList &unsortedItems, Qt::SortOrder sortOrder = Qt::AscendingOrder);

    /*!
     * \brief Destructor.
     */
    virtual ~MLocaleBuckets();

    /*!
     * \brief Set the items for this MLocaleBuckets object and recalculate all
     * values.
     *
     * This sorts all items into their corresponding bucket according to locale
     * rules.
     *
     * Each bucket has an index (starting with 0), a name and a number of
     * items. Both the buckets and the items inside each bucket are sorted in
     * the specified sort order according to locale rules.
     *
     * Only buckets that actually contain items are taken into account. If
     * there was no item starting with "X" in the item list specified in the
     * constructor, there will not be a bucket "X".
     *
     * If there were any previous items or buckets, this function removes them
     * first.
     *
     * Since the items list is sorted internally anyway, there is no benefit in
     * sorting them first before passing to this function.
     */
    void setItems(const QStringList &unsortedItems, Qt::SortOrder sortOrder = Qt::AscendingOrder);

    /*!
     * \brief Return the number of buckets.
     */
    int bucketCount() const;

    /*!
     * \brief Return the name of the bucket with the specified index (starting
     * with 0).
     *
     * Notice that bucket names may be longer than one character. For example,
     * in Czech, there might be a bucket named "CH".
     */
    QString bucketName(int bucketIndex) const;

    /*!
     * \brief Return the name of the bucket for the specified item.
     */
    QString bucketName(const QString &item) const;

    /*!
     * \brief Return the index of the specified bucket name or -1 if there is
     * no bucket with that name.
     */
    int bucketIndex(const QString &bucketName) const;

    /*!
     * \brief Return the bucket content for the specified bucket index.
     */
    QStringList bucketContent(int bucketIndex) const;

    /*!
     * \brief Return the original index of an item
     *
     * This returns the original index from the list passed in the constructor
     * of an item that is now sorted into bucket no. 'bucketIndex'. See also
     * the example in the intro of this class.
     *
     * If there is no item at that position, -1 is returned.
     *
     * If during the life time of this MLocaleBuckets object any items are
     * removed with removeBucketItems(), the original index of each item that
     * came after the removed item is decremented accordingly, just as if the
     * item was removed from the original items list set with setItems() or in
     * the constructor.
     *
     * \param bucketIndex index of the bucket.
     * \param indexInBucket index within the bucket. Each bucket starts with 0.
     */
    int origItemIndex(int bucketIndex, int indexInBucket) const;

    /*!
     * \brief Return the bucket size for the specified bucket index, i.e. the
     * number of items in that bucket.
     *
     * This returns -1 if there is no bucket with that index.
     */
    int bucketSize(int bucketIndex) const;

    /*!
     * \brief Check if there are any buckets or items in this MLocaleBuckets
     * object.
     */
    bool isEmpty() const;

    /*!
     * \brief Clear this MLocaleBuckets object: Remove all items and all
     * buckets.
     *
     * This is equivalent to setting an empty items list.
     */
    void clear();

    /*!
     * \brief Remove a range of items from bucket.
     *
     * This removes 'count' items from the bucket 'bucketIndex', starting with
     * index 'indexInBucket' inside that bucket.
     *
     * This returns 'true' if the bucket is empty afterwards (i.e., it should
     * be removed), 'false' otherwise.
     *
     * The original indices of items (origItemIndex()) will be decremented
     * accordingly, just as if the items were removed from the QStringList
     * passed in the constructor or in setItems().
     */
    bool removeBucketItems(int bucketIndex, int indexInBucket, int count=1);

    /*!
     * \brief Remove a bucket if it is empty.
     *
     * This should be done after removeBucketItems() returned true. The
     * rationale why removeBucketItems() doesn't do this automatically is
     * because SortFilterProxyModels that are attached to a model that uses
     * MLocaleBuckets will get very confused if toplevel items disappear
     * because the items below them were removed.
     *
     * If the bucket is not empty, this function does nothing.
     */
    void removeEmptyBucket(int bucketIndex);

    /*!
     * \brief Copies buckets and bucket items from the other reference.
     */
    MLocaleBuckets& operator =(const MLocaleBuckets &other);

private:
    Q_DECLARE_PRIVATE(MLocaleBuckets)

    MLocaleBucketsPrivate *const d_ptr;
};

#endif // MLOCALEBUCKETS_H
