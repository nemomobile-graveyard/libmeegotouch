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

#ifndef MAGGREGATEDATAACCESS_P_H
#define MAGGREGATEDATAACCESS_P_H

#include <maggregatedataaccess.h>

/*!
 * A private data class for MAggregateDataAccess.
 */
class MAggregateDataAccessPrivate
{
    Q_DECLARE_PUBLIC(MAggregateDataAccess)

public:
    /*!
     * Constructor.
     */
    MAggregateDataAccessPrivate(MDataAccess &primaryAccess, MDataAccess &secondaryAccess);

    /*!
     * Destructor.
     */
    virtual ~MAggregateDataAccessPrivate();

    //! The primary data access object
    MDataAccess &primaryAccess;

    //! The secondary data access object
    MDataAccess &secondaryAccess;

protected:
    MAggregateDataAccess *q_ptr;
};

#endif
