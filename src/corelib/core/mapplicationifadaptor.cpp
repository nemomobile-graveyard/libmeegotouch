/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mapplicationifadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <MDebug>

/*
 * Implementation of adaptor class MApplicationIfAdaptor
 */

MApplicationIfAdaptor::MApplicationIfAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

MApplicationIfAdaptor::~MApplicationIfAdaptor()
{
    // destructor
}

void MApplicationIfAdaptor::close()
{
    mDebug("MApplicationIfAdaptor") << "MApplicationIfAdaptor::close()";
    // handle method call com.nokia.MApplicationIfAdaptor.close
    QMetaObject::invokeMethod(parent(), "close");
}

void MApplicationIfAdaptor::exit()
{
    mDebug("MApplicationIfAdaptor") << "MApplicationIfAdaptor::exit()";
    // handle method call com.nokia.MApplicationIfAdaptor.exit
    QMetaObject::invokeMethod(parent(), "exit");
}

void MApplicationIfAdaptor::launch()
{
    mDebug("MApplicationIfAdaptor") << "MApplicationIfAdaptor::launch()";
    QMetaObject::invokeMethod(parent(), "launch");
}

void MApplicationIfAdaptor::launch(const QStringList &parameters)
{
    mDebug("MApplicationIfAdaptor") << "MApplicationIfAdaptor::launch(const QStringList&)";
    QMetaObject::invokeMethod(parent(), "launch", Q_ARG(QStringList, parameters));
}
