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

#include <QApplication>
#include <QLocalSocket>
#include <QUdpSocket>
#include <QFileInfo>
#include <MApplication>
#include <MDebug>

#ifdef Q_OS_LINUX
// support AF_UNIX datagram sockets
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
// end support AF_UNIX datagram sockets
#endif

#include "msyslogclient.h"

MSyslogClientSocket::MSyslogClientSocket()
    :
    m_socket(0),
    m_syslogId()
{
    // map Qt log levels to syslog severities
    severityMap[QtDebugMsg]    = 6;
    severityMap[QtWarningMsg]  = 4;
    severityMap[QtCriticalMsg] = 3;
    severityMap[QtFatalMsg]    = 2;
}

MSyslogClientSocket::~MSyslogClientSocket()
{
    this->close();
}

void MSyslogClientSocket::close()
{
    if (m_socket) {
        m_socket->close();
        delete m_socket;
        m_socket = 0;
    }
}

bool MSyslogClientSocket::connectToServer(const QUrl &url)
{
    this->close();

    if(url.isEmpty() || !url.isValid())
        return false;

    if (url.scheme() == "file") {
        QString socketFileName = url.toLocalFile();
        QLocalSocket *localSocket = new QLocalSocket();
        localSocket->connectToServer(socketFileName, QIODevice::WriteOnly);

        if (localSocket->waitForConnected(5000)) {
            m_socket = localSocket;
        } else {
            delete localSocket;

#ifdef Q_OS_LINUX
            // QLocalSocket opens a socket of type (AF_UNIX, SOCK_STREAM) on
            // Linux, now we fall back to unsupported (AF_UNIX, SOCK_DGRAM).

            struct sockaddr_un addr;
            memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            strncpy(addr.sun_path, socketFileName.toUtf8().data(),
                sizeof(addr.sun_path) - 1);

            int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
            if (sockfd == -1) {
                mDebug("mInitSyslogConnection") <<
                    "couldn't create socket descriptor.";
                return false;
            }

            int addrLen = sizeof(addr.sun_family) + strlen(addr.sun_path);
            if (connect(sockfd, (struct sockaddr *) &addr, addrLen) == -1) {
                mDebug("mInitSyslogConnection") <<
                    "failed to connect to domain socket.";
                return false;
            }

            QUdpSocket * udpSocket = new QUdpSocket();
            if (!udpSocket->setSocketDescriptor(sockfd,
                QAbstractSocket::ConnectedState, QIODevice::WriteOnly)) {
                ::close(sockfd);
                delete udpSocket;
                return false;
            }

            m_socket = udpSocket;
#else
            return false;
#endif
        }
    } else if (url.scheme() == "udp") {
        QString host = url.host();
        if (host.isEmpty()) {
            mDebug("mInitSyslogConnection") << "host field is empty";
            return false;
        }

        int port = url.port() != -1 ? url.port() : 514;
        QUdpSocket *udpSocket = new QUdpSocket();
        udpSocket->connectToHost(host, port, QIODevice::WriteOnly);
        if (!udpSocket->waitForConnected(5000)) {
            delete udpSocket;
            return false;
        }
        m_socket = udpSocket;

    } else {
        mDebug("mInitSyslogConnection") << "unsupported URL scheme";
        return false;
    }

    return true;
}

qint64 MSyslogClientSocket::sendMsg(QtMsgType type, const char *msg)
{
    // we log to facility 'user' (1)
    QString syslogMsg = QString("<%1>%2: %3")
        .arg((1 << 3) + severityMap[type])
        .arg(syslogId())
        .arg(msg);

    return m_socket->write(syslogMsg.toUtf8().data());
}

qint64 MSyslogClientSocket::sendMsg(const char *msg)
{
    // we log to facility 'user' (1)
    QString syslogMsg = QString("<%1>%2: %3")
        .arg((1 << 3) + severityMap[QtDebugMsg])
        .arg(syslogId())
        .arg(msg);

    return m_socket->write(syslogMsg.toUtf8().data());
}

const QString &MSyslogClientSocket::syslogId()
{
    // generate identifier
    if (m_syslogId.isEmpty()) {
        if (!MApplication::binaryName().isEmpty()) {
            m_syslogId = QString("%1[%2]")
                .arg(QFileInfo(MApplication::binaryName()).fileName())
                .arg(QCoreApplication::applicationPid());
        }
    }

    return m_syslogId;
}
