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

#include "mservicemapperadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class MServiceMapperAdaptor
 */

MServiceMapperAdaptor::MServiceMapperAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

MServiceMapperAdaptor::~MServiceMapperAdaptor()
{
    // destructor
}

QString MServiceMapperAdaptor::serviceName(const QString &interfaceName)
{
    // handle method call com.nokia.MServiceFwIf.serviceName
    QString out0;
    QMetaObject::invokeMethod(parent(), "serviceName", Q_RETURN_ARG(QString, out0), Q_ARG(QString, interfaceName));
    return out0;
}

QStringList MServiceMapperAdaptor::serviceNames(const QString &interfaceName)
{
    // handle method call com.nokia.MServiceFwIf.serviceNames
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "serviceNames", Q_RETURN_ARG(QStringList, out0), Q_ARG(QString, interfaceName));
    return out0;
}

QString MServiceMapperAdaptor::servicePath(const QString &interfaceName)
{
    // handle method call com.nokia.MServiceFwIf.servicePath
    QString out0;
    QMetaObject::invokeMethod(parent(), "servicePath", Q_RETURN_ARG(QString, out0), Q_ARG(QString, interfaceName));
    return out0;
}

QString MServiceMapperAdaptor::interfaceName(const QString &serviceName)
{
    // handle method call com.nokia.MServiceFwIf.interfaceName
    QString out0;
    QMetaObject::invokeMethod(parent(), "interfaceName", Q_RETURN_ARG(QString, out0), Q_ARG(QString, serviceName));
    return out0;
}
