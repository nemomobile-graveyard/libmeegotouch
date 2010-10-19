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
#ifndef MSERVICEMAPPERAPPLICATION_H
#define MSERVICEMAPPERAPPLICATION_H

// we need this class only to have a receiver for the
// Disconnected signal sent by libdbus, so that we can
// exit the daemon.

#include <QCoreApplication>

class MServiceMapperApplication : public QCoreApplication
{
  Q_OBJECT
public:
    /// constructor for this class
    MServiceMapperApplication( int & argc, char ** argv );

public slots:
    /// this slot is called when the connection to the dbus
    /// server got disconnected.
    void handleDBusServerDied();
};

#endif
