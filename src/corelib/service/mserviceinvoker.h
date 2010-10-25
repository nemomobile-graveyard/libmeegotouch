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

#ifndef MSERVICEINVOKER_H
#define MSERVICEINVOKER_H

#include <QObject>
#include <mexport.h>

/*!
 * A helper service that invokes service actions.
 */
class M_CORE_EXPORT MServiceInvoker : public QObject
{
    Q_OBJECT

private:
    /*!
     * Hidden default constructor
     */
    MServiceInvoker();

    Q_DISABLE_COPY(MServiceInvoker)

    //! Pointer to the singleton instance
    static MServiceInvoker *theInstance;

public:
    /*!
     * Returns a singleton instance of this class.
     */
    static MServiceInvoker *instance();

public slots:
    /*!
     * Invokes a \c MServiceAction service.
     */
    void invoke();
};

#endif // MSERVICEINVOKER_H
