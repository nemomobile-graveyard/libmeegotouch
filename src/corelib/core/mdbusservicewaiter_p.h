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

#ifndef MDBUSSERVICEWAITER_P_H
#define MDBUSSERVICEWAITER_P_H


#include <QObject>

#include <QString>


/*!
 * A class which has functionality to wait until D-BUS service has been registered.
 *
 * Utility class which is intended to be used before entering QCoreApplication::exec(). The
 * class has function to block until the service is registered (or timeout occurs) which is
 * meant to be used right after construction. The instances of the class are intended to be
 * lightweight and have short lifespan, the class itself is needed mainly for receiving
 * appropriate signals.
 */
class MDBusServiceWaiter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout)

public:

    /*!
     * Constructor for the waiter class.
     * Initializes timeout and monitoring of D-BUS service registration.
     * \param name name of the D-BUS service that is going to be waited for.
     * \param parent parent object.
     */
    MDBusServiceWaiter(const QString &name, QObject *parent = 0);

    /*!
     * Function which will wait until the service has been registered. If the service is
     * initially registered when calling this function, the function will return immediately
     * true.
     * \return true if service has been registered before timeout occurs, otherwise false.
     * If no connection to session bus is present, this function will return immediately
     * false.
     */
    bool waitForRegistration();

    int timeout() const;
    void setTimeout(int newTimeout);

private slots:
    void serviceTimedOut();
    void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);

private:
    bool success;
    bool finished;
    QString serviceName;

    int m_timeout;

    bool isInvalid;
};


#endif
