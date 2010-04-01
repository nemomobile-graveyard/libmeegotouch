/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUIAGGREGATEDATAACCESS_H
#define DUIAGGREGATEDATAACCESS_H

#include <DuiDataAccess>
#include "duiexport.h"

class DuiAggregateDataAccessPrivate;

/*!
 * \brief DuiAggregateDataAccess can be used to combine two or several objects implementing DuiDataAccess interface behind common
 * DuiDataAccess interface.
 *
 * Keys are primarily set and acquired from primary access. If the particular key is not defined in primary access then its
 * set or acquired from the secondary access. DuiDataAccess interfaces can be chained so that DuiAggregateDataAccess is defined
 * as the secondary DuiDataAccess in the constructor of this object.
 * \code
 * // DuiAggregateDataAccess with two DuiDataAccesses:
 * DuiDataAccess& primaryAccess;
 * DuiDataAccess& secondaryAccess;
 * DuiAggregateDataAccess* aggregate = new DuiAggregateDataAccess(primaryAccess, secondaryAccess);
 *
 * // DuiAggregateDataAccess with three DuiDataAccesses:
 * DuiDataAccess& primaryAccess;
 * DuiDataAccess& secondaryAccess;
 * DuiDataAccess& tertiaryAccess;
 * DuiAggregateDataAccess* secondaryAggregate = new DuiAggregateDataAccess(secondaryAccess, tertiaryAccess);
 * DuiAggregateDataAccess* aggregate = new DuiAggregateDataAccess(primaryAccess, *secondaryAggregate);
 * \endcode
 *
 * Ownership of the primary and secondary data access are maintained on the caller. Caller needs to make sure that primary and secondary data access objects
 * exist whenever DuiAggregateDataAccess exists. For instance, DuiAggregateDataAccess object has to be destroyed before associated primary and secondary
 * data access objects are destroyed.
 */
class DUI_EXPORT DuiAggregateDataAccess : public DuiDataAccess
{
    Q_OBJECT

public:
    /*!
     * Default constructor.
     * \param primaryAccess Primary DuiDataAccess object. Values are set and accessed from this data access primarily.
     * \param secondaryAccess Secondary DuiDataAccess object. Values are set and accessed from this data access only if values cannot be set or accessed from the primary access.
     */
    DuiAggregateDataAccess(DuiDataAccess &primaryAccess, DuiDataAccess &secondaryAccess);

    /*!
     * Destructor.
     */
    virtual ~DuiAggregateDataAccess();

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
    DuiAggregateDataAccessPrivate *const d_ptr;

    /*!
     * Constructor for derived classes that allows injection of private implementation object of a derived class.
     */
    DuiAggregateDataAccess(DuiAggregateDataAccessPrivate &dd);

private slots:
    /*!
     * Slot called when values are changed in secondary DuiDataAccess object.
     * If value is unique in this DuiAggregateDataAccess the valueChanged signal is emitted onwards.
     */
    void secondaryAccessValueChanged(const QString &key, const QVariant &value);

private:
    /*!
     * Initializes this object
     */
    void init();

private:
    Q_DECLARE_PRIVATE(DuiAggregateDataAccess)
};

#endif // DUIAGGREGATEDATAACCESS_H
