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

#include "mappletinstancemanagerdbusadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class MAppletInstanceManagerDBusAdaptor
 */

MAppletInstanceManagerDBusAdaptor::MAppletInstanceManagerDBusAdaptor(MAppletInstanceManager *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

MAppletInstanceManagerDBusAdaptor::~MAppletInstanceManagerDBusAdaptor()
{
    // destructor
}

void MAppletInstanceManagerDBusAdaptor::instantiateAppletFromPackage(const QString &packageName, const QMap<QString, QVariant> &metaData)
{
    // handle method call com.meego.core.MAppletInstanceManager.instantiateAppletFromPackage
    parent()->instantiateAppletFromPackage(packageName, metaData);
}

