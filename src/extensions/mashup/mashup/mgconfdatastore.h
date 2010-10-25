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

#ifndef MGCONFDATASTORE_H
#define MGCONFDATASTORE_H

#include <MExport>
#include "mdatastore.h"

class MGConfItem;
class MGConfDataStorePrivate;

/*!
 * A data store class that uses GConf as the backend.
 *
 * This datastore can access all keys inside a GConf path given in the constructor.
 * The access is limited to a single level of hierarchy, you can't access keys
 * of any subdirectories of the path. For example, the following code will
 * create the datastore with the GConf path <tt>/example</tt> and set a
 * value of the key called <tt>userKey</tt> under that path:
 *
 * \code
 * MGConfDataStore dataStore("/example");
 * dataStore.setValue("userKey", 123);
 * \endcode
 *
 */
class M_EXTENSIONS_EXPORT MGConfDataStore : public MDataStore
{
    Q_OBJECT

public:
    /*!
     * Constructor.
     * \param keyPath The gconf path in which the datastore operates.
     */
    MGConfDataStore(const QString &keyPath);

    /*!
     * Destructor.
     */
    virtual ~MGConfDataStore();

    //! \reimp
    virtual bool setValue(const QString &key, const QVariant &value);
    virtual bool createValue(const QString &key, const QVariant &value);
    virtual QVariant value(const QString &key) const;
    virtual QStringList allKeys() const;
    virtual void remove(const QString &key);
    virtual void clear();

    /*!
     * Note that this function returns \c true if a GConf key mapping for the
     * key has been set even if the key wouldn't have a value.
     */
    virtual bool contains(const QString &key) const;
    //! \reimp_end

private slots:
    /*!
     * Notifies the object that a GConf value has changed.
     * If the caller of this slot is valid this causes the valueChanged()
     * signal to be emitted.
     */
    void gconfValueChanged();

protected:
    MGConfDataStorePrivate * const d_ptr;

private:
    Q_DECLARE_PRIVATE(MGConfDataStore)
    Q_DISABLE_COPY(MGConfDataStore)
};

#endif  /* MGCONFDATASTORE_H */
