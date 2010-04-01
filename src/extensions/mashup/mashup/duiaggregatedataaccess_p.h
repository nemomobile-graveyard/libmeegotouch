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

#ifndef DUIAGGREGATEDATAACCESS_P_H
#define DUIAGGREGATEDATAACCESS_P_H

#include <duiaggregatedataaccess.h>

/*!
 * A private data class for DuiAggregateDataAccess.
 */
class DuiAggregateDataAccessPrivate
{
    Q_DECLARE_PUBLIC(DuiAggregateDataAccess)

public:
    /*!
     * Constructor.
     */
    DuiAggregateDataAccessPrivate(DuiDataAccess &primaryAccess, DuiDataAccess &secondaryAccess);

    /*!
     * Destructor.
     */
    virtual ~DuiAggregateDataAccessPrivate();

    //! The primary data access object
    DuiDataAccess &primaryAccess;

    //! The secondary data access object
    DuiDataAccess &secondaryAccess;

protected:
    DuiAggregateDataAccess *q_ptr;
};

#endif
