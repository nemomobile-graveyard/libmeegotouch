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

#ifndef MAGGREGATEDATAACCESS_H
#define MAGGREGATEDATAACCESS_H

#include <MDataAccess>
#include "mexport.h"

class MAggregateDataAccessPrivate;

/*!
 * \brief MAggregateDataAccess can be used to combine two or several objects implementing MDataAccess interface behind common
 * MDataAccess interface.
 *
 * Keys are primarily set and acquired from primary access. If the particular key is not defined in primary access then its
 * set or acquired from the secondary access. MDataAccess interfaces can be chained so that MAggregateDataAccess is defined
 * as the secondary MDataAccess in the constructor of this object.
 * \code
 * // MAggregateDataAccess with two MDataAccesses:
 * MDataAccess& primaryAccess;
 * MDataAccess& secondaryAccess;
 * MAggregateDataAccess* aggregate = new MAggregateDataAccess(primaryAccess, secondaryAccess);
 *
 * // MAggregateDataAccess with three MDataAccesses:
 * MDataAccess& primaryAccess;
 * MDataAccess& secondaryAccess;
 * MDataAccess& tertiaryAccess;
 * MAggregateDataAccess* secondaryAggregate = new MAggregateDataAccess(secondaryAccess, tertiaryAccess);
 * MAggregateDataAccess* aggregate = new MAggregateDataAccess(primaryAccess, *secondaryAggregate);
 * \endcode
 *
 * Ownership of the primary and secondary data access are maintained on the caller. Caller needs to make sure that primary and secondary data access objects
 * exist whenever MAggregateDataAccess exists. For instance, MAggregateDataAccess object has to be destroyed before associated primary and secondary
 * data access objects are destroyed.
 */
class M_EXTENSIONS_EXPORT MAggregateDataAccess : public MDataAccess
{
    Q_OBJECT

public:
    /*!
     * Default constructor.
     * \param primaryAccess Primary MDataAccess object. Values are set and accessed from this data access primarily.
     * \param secondaryAccess Secondary MDataAccess object. Values are set and accessed from this data access only if values cannot be set or accessed from the primary access.
     */
    MAggregateDataAccess(MDataAccess &primaryAccess, MDataAccess &secondaryAccess);

    /*!
     * Destructor.
     */
    virtual ~MAggregateDataAccess();

    //! \reimp
    virtual bool setValue(const QString &key, const QVariant &value);
    virtual QVariant value(const QString &key) const;
    /*!
     * Returns the combined key list of both primary and secondary data access objects. All duplicate keys are removed.
     * \return Keys from both primary and secondary data access objects with duplicates removed.
     */
    virtual QStringList allKeys() const;
    virtual bool contains(const QString &key) const;
    //! \reimp_end

protected:
    //! Pointer to private implementation object.
    MAggregateDataAccessPrivate *const d_ptr;

    /*!
     * Constructor for derived classes that allows injection of private implementation object of a derived class.
     */
    MAggregateDataAccess(MAggregateDataAccessPrivate &dd);

private slots:
    /*!
     * Slot called when values are changed in secondary MDataAccess object.
     * If value is unique in this MAggregateDataAccess the valueChanged signal is emitted onwards.
     */
    void secondaryAccessValueChanged(const QString &key, const QVariant &value);

private:
    /*!
     * Initializes this object
     */
    void init();

private:
    Q_DECLARE_PRIVATE(MAggregateDataAccess)
};

#endif // MAGGREGATEDATAACCESS_H
