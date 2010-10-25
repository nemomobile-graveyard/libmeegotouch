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

#ifndef MDATAACCESS_H
#define MDATAACCESS_H

#include "mexport.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <QStringList>

/*!
 * \brief Interface for reading and storing key values.
 *
 * Users can read and write key values using this interface. The user
 * also get notified when changes happen in the key values.
 */
class M_CORE_EXPORT MDataAccess : public QObject
{
    Q_OBJECT

public:
    /*!
     * Destroys the MDataAccess.
     */
    virtual ~MDataAccess() {}

    /*!
     * Returns a value for a key.
     * If the key doesn't exist, an invalid (QVariant::Invalid) value is returned.
     * \param key the key.
     * \return the requested value.
     */
    virtual QVariant value(const QString &key) const = 0;

    /*!
     * Sets a new value for a key. If the key isn't found, nothing
     * happens and \c false is returned.
     * \param key the key to be changed.
     * \param value the new value.
     * \return \c true if setting was successful, \c false otherwise
     */
    virtual bool setValue(const QString &key, const QVariant &value) = 0;

    /*!
     * Returns a list of all specified keys.
     */
    virtual QStringList allKeys() const = 0;

    /*!
     * Returns \c true if there exists a key called \a key and \c false otherwise.
     *
     * \param key the key to test
     * \return a boolean value telling if the key exists or not
     * \sa value() and setValue().
     */
    virtual bool contains(const QString &key) const = 0;

Q_SIGNALS:
    /*!
     * A signal that is emitted when a key value changes.
     * \param key the key that changed.
     * \param value the new value.
     */
    void valueChanged(const QString &key, const QVariant &value);
};

#endif // MDATAACCESS_H
