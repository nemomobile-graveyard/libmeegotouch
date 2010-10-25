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

//! \internal

// Command line was: qdbusxml2cpp -c MApplicationIfProxy -p mapplicationifproxy.h:mapplicationifproxy.cpp com.nokia.MApplicationIf.xml

#ifndef MAPPLICATIONIFPROXY_H
#define MAPPLICATIONIFPROXY_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

#include "mexport.h"

/*!
 * \class MApplicationIfProxy
 * \brief Proxy class for interface com.nokia.MApplicationIf
 *
 * This is the proxy for the MApplication QDBus service.
 */
class M_CORE_EXPORT MApplicationIfProxy: public QDBusAbstractInterface
{
    Q_OBJECT

public:
    MApplicationIfProxy(const QString &service, QObject *parent = 0);

    ~MApplicationIfProxy();

public Q_SLOTS: // METHODS
    QDBusPendingReply<> close();
    QDBusPendingReply<> exit();
    QDBusPendingReply<> launch();
    QDBusPendingReply<> launch(const QStringList& parameters);

Q_SIGNALS: // SIGNALS
};

namespace com
{
    namespace nokia
    {
        typedef ::MApplicationIfProxy MApplicationIf;
    }
}
#endif

//! \internal_end
