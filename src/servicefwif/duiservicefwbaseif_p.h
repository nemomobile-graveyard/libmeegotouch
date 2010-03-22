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

#ifndef DUISERVICEFWBASEIF_P_H
#define DUISERVICEFWBASEIF_P_H

class QDBusAbstractInterface;

class DuiServiceFwBaseIfPrivate
{
public:
    DuiServiceFwBaseIfPrivate(const QString &iface);
    ~DuiServiceFwBaseIfPrivate();

    // Pointer to the D-Bus interfaceProxy
    // assigned in derived class, deleted in this class
    QDBusAbstractInterface *interfaceProxy;

    // Interface to Service Framework, owned
    DuiServiceFwProxy *serviceFwProxyPtr;

    // Name of the active service provider
    QString service;

    // Name of the interface
    QString interface;
};

#endif // DUISERVICEFWBASEIF_P_H
