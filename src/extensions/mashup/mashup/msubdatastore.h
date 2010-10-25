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

#ifndef MSUBDATASTORE_H
#define MSUBDATASTORE_H

#include "mdatastore.h"

/*!
 * MSubDataStore takes another DataStore object and gives a limited view to it.
 *
 * MSubDataStore doesn't store any data by itself but instead it uses another DataStore
 * object for this. MSubDataStore offers a limited view to the keys of the underlying DataStore.
 * It only allows access to keys with a given prefix. This can be seen as a namespace limitation.
 * The prefix or namespace as well as the underlying DataStore object are given at
 * construction time and they can't be changed after that.
 */
class M_EXTENSIONS_EXPORT MSubDataStore : public MDataStore
{
    Q_OBJECT

    //! The prefix of the sub data store.
    QString _prefix;
    //! The length of the prefix string. Calculated only once for efficiency.
    int _prefixLength;
    //! The base data store object this object uses.
    MDataStore &_baseStore;

public:
    /*!
     * Constructs a new MSubDataStore view to another DataStore object.
     * \param prefix the namespace for the MSubDataStore.
     * \param baseStore the underlying base DataStore object.
     */
    MSubDataStore(const QString &prefix, MDataStore &baseStore);

    /*!
     * Destructs the MSubDataStore.
     */
    virtual ~MSubDataStore();

    /*!
     * Gets the prefix or namespace that was given to the class at construction time.
     * \return the prefix.
     */
    QString prefix() const;

    //! \reimp
    virtual bool createValue(const QString &key, const QVariant &value);
    virtual bool setValue(const QString &key, const QVariant &value);
    virtual QVariant value(const QString &key) const;
    virtual QStringList allKeys() const;
    virtual void remove(const QString &key);
    virtual void clear();
    virtual bool contains(const QString &key) const;
    //! \reimp_end

private slots:
    /*!
     * A slot for getting information about changes in the basestore.
     * If the key has the prefix of this subdatastore, the valueChanged signal is emitted.
     */
    void baseStoreValueChanged(const QString &key, const QVariant &value);
};

#endif // MSUBDATASTORE_H
