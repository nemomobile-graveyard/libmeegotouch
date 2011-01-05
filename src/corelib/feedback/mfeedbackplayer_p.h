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

#ifndef MFEEDBACKPLAYERPRIV_H
#define MFEEDBACKPLAYERPRIV_H

#include <QLocalSocket>
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>
#include <QDataStream>
#include <QObject>

#include "mexport.h"

class MFeedbackPlayerPrivate : public QObject
{
    Q_OBJECT
public:
    MFeedbackPlayerPrivate(QObject *parent);
    virtual ~MFeedbackPlayerPrivate();

    bool init(const QString &applicationName);

    void sendPlaybackRequest(const QString &name);

private slots:
    void onConnected();
    void onSocketError(QLocalSocket::LocalSocketError socketError);
    void connectIdle();

public:
    QLocalSocket socket;
    QDataStream socketStream;
    QString applicationName;

    // Number of reconnection attempts. This value is zeroed when a connection is
    // successfully established.
    int reconnectionAttempts;

    // Contains the intervals, in milliseconds, to wait before attempting
    // to reconnect to feedback-manager daemon.
    // list[0] is the time to wait before trying to connect and reconnect for the first time.
    // If the first attempt fails then it waits list[1] milliseconds before attempting
    // to reconnect for the second time and so on.
    // It gives up trying to reconnect when failedReconnections == list.size()
    QList<int> reconnectionIntervalsList;

    // Contains time (in milliseconds) from the previous successfull connection
    // to the feedback daemon.
    QElapsedTimer previousSuccessfullConnection;

    // Contains number of successive successfull connections to feedback daemon
    // that have happened in less than fastReconnectionTime.
    int fastReconnectionCount;
};

#endif
