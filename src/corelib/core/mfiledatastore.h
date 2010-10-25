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

#ifndef MFILEDATASTORE_H
#define MFILEDATASTORE_H

#include "mdatastore.h"

class MFileDataStorePrivate;

/*!
 * Concrete implementation of \c MDataStore interface. This class stores the data to the
 * filesystem. The file name is given as a constructor parameter.
 */
class M_CORE_EXPORT MFileDataStore : public MDataStore
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     * \param filePath Absolute path to the file that the settings will be written to and read from.
     */
    explicit MFileDataStore(const QString &filePath);

    /*!
     * Destructor
     */
    virtual ~MFileDataStore();

    //! \reimp
    /*!
     * If \c isWritable returns \c false, this method returns \c false.
     */
    virtual bool createValue(const QString &key, const QVariant &value);
    /*!
     * If \c isWritable returns \c false, this method returns \c false.
     */
    virtual bool setValue(const QString &key, const QVariant &value);
    /*!
     * If \c isReadable returns \c false, this method returns an empty QVariant.
     */
    virtual QVariant value(const QString &key) const;
    /*!
     * If \c isReadable returns \c false, this method returns an empty list.
     */
    virtual QStringList allKeys() const;
    /*!
     * If \c isWritable returns \c false, this method does nothing.
     */
    virtual void remove(const QString &key);
    /*!
     * If \c isWritable returns \c false, this method does nothing.
     */
    virtual void clear();
    /*!
     * If \c isReadable returns \c false, this method returns \c false.
     */
    virtual bool contains(const QString &key) const;
    //! \reimp_end

    /*!
     * Queries if this data store is readable. If this method returns \c true you
     * can use the reading methods of this class (\c value, \c allKeys, \c contains).
     * If this method returns \c false, the reading methods don't provide the real data.
     * \sa value, allKeys, contains
     * \return \c true if the data store can be read.
     */
    bool isReadable() const;

    /*!
     * Queries if this data store is writable. If this method returns \c true you
     * can use the writing methods of this class (\c setValue, \c remove, \c clear).
     * If this method returns \c false, the writing methods don't modify the data store.
     * \sa setValue, remove, clear
     * \return \c true if the data store can be written.
     */
    bool isWritable() const;

private:
    /*!
     * Takes a snapshot of keys and values in the underlying QSettings.
     * This allows us to check for external modifications. QSettings seems
     * to pick up such modifications automatically, so we compare the settings
     * to the snapshot when we notice a file change.
     */
    void takeSnapshot();

private slots:
    /*!
     * Notifies that the settings file has been changed in the filesystem externally
     * \param fileName The name of the file modified
     */
    void fileChanged(const QString &fileName);

    /*!
     * Notifies that the directory of the settings file has been changed in the filesystem externally
     * \param fileName The name of the file modified
     */
    void directoryChanged(const QString &fileName);

protected:
    MFileDataStorePrivate * const d_ptr;

private:
    Q_DECLARE_PRIVATE(MFileDataStore)
    Q_DISABLE_COPY(MFileDataStore)
};

#endif // MFILEDATASTORE_H
