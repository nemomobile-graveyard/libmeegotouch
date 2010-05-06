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

#include "mfeedbackplayer_p.h"

#include <MDebug>
#include <QDir>
#include <QString>

static const char gSocketServer[] = "/tmp/mfeedbackd/player.sock";

MFeedbackPlayerPrivate::MFeedbackPlayerPrivate(QObject *parent)
    : QObject(parent)
{
    socketStream.setDevice(&socket);
    connect(&socket, SIGNAL(connected()), SLOT(onConnected()));
    connect(&socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            SLOT(onSocketError(QLocalSocket::LocalSocketError)));

    reconnectionAttempts = 0;

    // TODO: Load from a config file?
    reconnectionIntervalsList << 50;
    reconnectionIntervalsList << 300;
    reconnectionIntervalsList << 600;
    reconnectionIntervalsList << 1000;
    reconnectionIntervalsList << 1000;
    reconnectionIntervalsList << 2000;
    reconnectionIntervalsList << 2000;
    reconnectionIntervalsList << 5000;
    reconnectionIntervalsList << 5000;
    reconnectionIntervalsList << 10000;
    reconnectionIntervalsList << 10000;
    reconnectionIntervalsList << 30000;
    reconnectionIntervalsList << 30000;
    reconnectionIntervalsList << 300000;
    reconnectionIntervalsList << 300000; // 5*60*1000 (five minutes)
}

MFeedbackPlayerPrivate::~MFeedbackPlayerPrivate()
{
    // socket emits disconnected() in its destructor. If we don't
    // disconnect from this signal we will get called after being
    // destroyed which will cause a SIGSEGV
    socket.disconnect(this);
}

bool MFeedbackPlayerPrivate::init(const QString &applicationName)
{
    this->applicationName = applicationName;

    // Initial connection to server is immediate
    connectIdle();

    return true;
}

void MFeedbackPlayerPrivate::sendPlaybackRequest(const QString &name)
{
    if (socket.state() == QLocalSocket::ConnectedState &&
            name.isEmpty() == false) {
        socketStream << name;
        socket.flush();
    }
}

void MFeedbackPlayerPrivate::onConnected()
{
    reconnectionAttempts = 0;
    socketStream << applicationName;
}

void MFeedbackPlayerPrivate::connectIdle()
{
    socket.connectToServer(gSocketServer);
}

void MFeedbackPlayerPrivate::onSocketError(QLocalSocket::LocalSocketError socketError)
{
    Q_UNUSED(socketError);

    if (reconnectionAttempts < reconnectionIntervalsList.size()) {
        // Try to reconnect to mfeedbackd
        QTimer::singleShot(reconnectionIntervalsList[reconnectionAttempts],
                           this, SLOT(connectIdle()));
        reconnectionAttempts++;
    } else {
        // Give up
        qWarning("MFeedbackPlayer: Couldn't establish connection with mfeedbackd.");
    }
}
