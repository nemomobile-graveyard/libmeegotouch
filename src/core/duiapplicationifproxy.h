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

//! \internal

// Command line was: qdbusxml2cpp -c DuiApplicationIfProxy -p duiapplicationifproxy.h:duiapplicationifproxy.cpp com.nokia.DuiApplicationIf.xml

#ifndef DUIAPPLICATIONIFPROXY_H
#define DUIAPPLICATIONIFPROXY_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

#include "duiexport.h"

/*!
 * \class DuiApplicationIfProxy
 * \brief Proxy class for interface com.nokia.DuiApplicationIf
 *
 * This is the proxy for the DuiApplication QDBus service.
 */
class DUI_EXPORT DuiApplicationIfProxy: public QDBusAbstractInterface
{
    Q_OBJECT

public:
    DuiApplicationIfProxy(const QString &service, QObject *parent = 0);

    ~DuiApplicationIfProxy();

public Q_SLOTS: // METHODS
    QDBusPendingReply<> close();
    QDBusPendingReply<> exit();
    QDBusPendingReply<> launch();

Q_SIGNALS: // SIGNALS
};

namespace com
{
    namespace nokia
    {
        typedef ::DuiApplicationIfProxy DuiApplicationIf;
    }
}
#endif

//! \internal_end
