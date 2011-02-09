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


#ifndef MDBUSINTEFACE_H
#define MDBUSINTEFACE_H

#include "mexport.h"
#include <QDBusAbstractInterface>

/*! \internal */

/*!
  \brief This class is a subclass QDBusAbstractInterface, which allows making asynchronous calls to
    DBus without any type of blocking introspection.
*/

class M_CORE_EXPORT MDBusInterface : public QDBusAbstractInterface
{
public:
    MDBusInterface(const QString &service, const QString &path, const char *interface,
                  const QDBusConnection &connection = QDBusConnection::sessionBus() , QObject *parent = 0);
    virtual ~MDBusInterface();
};

/*! \internal_end */

#endif // MDBUSINTEFACE_H
