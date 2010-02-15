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

#ifndef DUITHEMEDAEMONSERVER_H
#define DUITHEMEDAEMONSERVER_H

#include <QObject>
#include <QQueue>
#include <QLocalServer>
#include <DuiGConfItem>
#include <QTimer>


#include <duithemedaemon.h>

class QLocalSocket;
class DuiThemeDaemonClient;

//! \internal
class DuiThemeDaemonServer : public QObject
{
    Q_OBJECT
public:
    DuiThemeDaemonServer();
    virtual ~DuiThemeDaemonServer();

private slots:
    void clientConnected();
    void clientDisconnected();
    void clientDataAvailable();

    void themeChanged();
    void localeChanged();

    void processOneQueueItem();

private:
    void pixmapRequested(DuiThemeDaemonClient *client, const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id);
    void pixmapReleaseRequested(DuiThemeDaemonClient *client, const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id);
    void themeDaemonStatus(DuiThemeDaemonClient *client) const;

private:
    QLocalServer server;
    QHash<QLocalSocket *, DuiThemeDaemonClient *> registeredClients;
    DuiThemeDaemon daemon;

    DuiGConfItem currentTheme;
    DuiGConfItem currentLocale;

    QQueue<QPair<DuiThemeDaemonClient *, Dui::DuiThemeDaemonProtocol::PixmapIdentifier> > loadPixmapsQueue;
    QQueue<QPair<DuiThemeDaemonClient *, Dui::DuiThemeDaemonProtocol::PixmapIdentifier> > releasePixmapsQueue;
    QTimer processQueueTimer;
};
//! \internal_end
#endif

