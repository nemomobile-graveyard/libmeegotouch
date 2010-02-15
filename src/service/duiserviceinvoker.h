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

#ifndef DUISERVICEINVOKER_H
#define DUISERVICEINVOKER_H

#include <QObject>
#include <duiexport.h>

/*!
 * A helper service that invokes service actions.
 */
class DUI_EXPORT DuiServiceInvoker : public QObject
{
    Q_OBJECT

private:
    /*!
     * Hidden default constructor
     */
    DuiServiceInvoker();

    Q_DISABLE_COPY(DuiServiceInvoker)

    //! Pointer to the singleton instance
    static DuiServiceInvoker *theInstance;

public:
    /*!
     * Returns a singleton instance of this class.
     */
    static DuiServiceInvoker *instance();

public slots:
    /*!
     * Invokes a \c DuiServiceAction service.
     */
    void invoke();
};

#endif // DUISERVICEINVOKER_H
