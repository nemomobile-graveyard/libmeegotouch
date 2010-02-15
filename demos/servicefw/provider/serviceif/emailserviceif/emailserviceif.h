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

#ifndef EMAILSERVICEIF_H
#define EMAILSERVICEIF_H

/*
 * automatically generated with the command line :
 * ../../../../../tools/dui-servicefwgen -p com.nokia.EmailServiceIf
 */

#include <QObject>

#include <duiservicefwbaseif.h>
#include <emailserviceifproxy.h>

class EmailServiceIf : public DuiServiceFwBaseIf
{
    Q_OBJECT

public:
    QString editSendA(const QString &message);
    QString editSend(const QString &message);

public:
    /*!
     * @brief Constructs a base interface
     * @param preferredService, define the preferred service provider. Leave
       empty if no preferred provider. In most cases, this should be left
       empty.
     * @param parent Parent object
     */

    EmailServiceIf(const QString &preferredService = "", QObject *parent = 0);

    /*!
     * @brief Set the service name
     * @param service Name of the desired service
     */

    void setService(const QString &service);
Q_SIGNALS:
    void messageSent(const QString &message);

};
#endif
