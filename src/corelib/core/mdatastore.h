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

#ifndef MDATASTORE_H
#define MDATASTORE_H

#include "mdataaccess.h"
#include "mexport.h"

/*!
 * Interface for reading and storing data.
 *
 * The difference between this class and \c MDataAccess is that this interface
 * can also be used to create and remove keys.
 */
class M_CORE_EXPORT MDataStore : public MDataAccess
{
    Q_OBJECT

public:
    /*!
     * Destroys the MDataStore.
     */
    virtual ~MDataStore() {}

    /*!
     * This will add a new key with the given value or change the value of an
     * existing key if the key already exists.
     *
     * Concrete implementations of MDataStore have to ensure that the data
     * is being synchronized with the backend when this is called.
     *
     * \param key the key to set the value for
     * \param value the value for the key
     * \return \c true if the key was changed or added, \c false otherwise
     */
    virtual bool createValue(const QString &key, const QVariant &value) = 0;

    /*!
     * Removes the data of the given key from the datastore.
     * Concrete implementations of MDataStore have to ensure that the data is being synchronized
     * with the backend when this is called.
     */
    virtual void remove(const QString &key) = 0;

    /*!
     * Removes all entries in this datastore.
     * Concrete implementations of MDataStore have to ensure that the data is being synchronized
     * with the backend when this is called.
     */
    virtual void clear() = 0;

};

#endif // MDATASTORE_H
