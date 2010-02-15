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

#include "duirmiclient_p.h"
#include "duirmiclient.h"
#include <QDataStream>
#include <QByteArray>
#include <DuiDebug>

DuiRmiClientPrivate:: DuiRmiClientPrivate(const QString &key)
    : _key(key)
{
}

DuiRmiClientPrivate::~DuiRmiClientPrivate()
{
}

void DuiRmiClientPrivate::initConnection()
{
    //unused
}

void DuiRmiClientPrivate::finalizeConnection()
{
    // unused
}

QString DuiRmiClientPrivate::key() const
{
    return _key;
}

DuiRmiClientPrivateSocket::DuiRmiClientPrivateSocket(const QString &key, DuiRmiClient *q)
    : DuiRmiClientPrivate(key),
      return_sz(0)
{
    q_ptr = q;
    QObject::connect(&_socket, SIGNAL(readyRead()), q_ptr, SLOT(_q_readyRead()));
}


void DuiRmiClientPrivateSocket::initConnection()
{
    _buf.clear();
    _stream = new QDataStream(&_buf, QIODevice::WriteOnly);
    _stream->setVersion(QDataStream::Qt_4_0);

    connectToServer();
    if (!_socket.waitForConnected())
        duiDebug("DuiRmiClientPrivateSocket") << _socket.errorString() << key();
}

void DuiRmiClientPrivateSocket::finalizeConnection()
{
    uint sz = _buf.size();
    duiDebug("DuiRmiClientPrivateSocket") << sz;
    _stream->device()->seek(0);
    *_stream << (quint16)(sz - sizeof(quint16));

    if (_socket.state() == QLocalSocket::ConnectedState) {
        writeData(_buf);
    }

    _socket.close();
    delete _stream;
}

QDataStream &DuiRmiClientPrivateSocket::stream()
{
    return *_stream;
}

void DuiRmiClientPrivateSocket::writeData(const QByteArray &buffer)
{
    _socket.write(buffer);
    _socket.waitForBytesWritten();
    // _socket.disconnect();
}

void DuiRmiClientPrivateSocket::connectToServer()
{
    if (_socket.state() == QLocalSocket::UnconnectedState)
        _socket.connectToServer(key());
}

void DuiRmiClientPrivateSocket::_q_readyRead()
{
    Q_Q(DuiRmiClient);
    QDataStream stream(&_socket);
    stream.setVersion(QDataStream::Qt_4_0);

    if (return_sz == 0) {
        if (_socket.bytesAvailable() < (int)sizeof(quint16))
            return;
        stream >> return_sz;
    }

    if (_socket.bytesAvailable() < return_sz)
        return;

    QVariant v;
    stream >> v;
    emit q->returnValue(v);
}

DuiRmiClient::DuiRmiClient(const QString &key, QObject *p)
    : QObject(p),
      d_ptr(new DuiRmiClientPrivateSocket(key, this))
{
}
DuiRmiClient::~DuiRmiClient()
{
    delete d_ptr;
}

void DuiRmiClient::initConnection()
{
    Q_D(DuiRmiClient);
    d->initConnection();
}

void DuiRmiClient::finalizeConnection()
{
    Q_D(DuiRmiClient);
    d->finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method)
{
    Q_D(DuiRmiClient);
    initConnection();

    /* packet is composed of |block size|argument length|arguments...| */
    d->stream() << (quint16)0 << (quint16)0 << objectName << method;

    finalizeConnection();
}


void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)1 << objectName << method << arg0;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0, const QVariant &arg1)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)2 << objectName << method << arg0
                << arg1;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)3 << objectName << method << arg0
                << arg1 << arg2;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)4 << objectName << method << arg0
                << arg1 << arg2 << arg3;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3,
                          const QVariant &arg4)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)5 << objectName << method << arg0
                << arg1 << arg2 << arg3 << arg4;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3,
                          const QVariant &arg4,   const QVariant &arg5)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)6 << objectName << method << arg0
                << arg1 << arg2 << arg3 << arg4 << arg5;
    finalizeConnection();

}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3,
                          const QVariant &arg4,   const QVariant &arg5,
                          const QVariant &arg6)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)7 << objectName << method << arg0
                << arg1 << arg2 << arg3 << arg4 << arg5 << arg6;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3,
                          const QVariant &arg4,   const QVariant &arg5,
                          const QVariant &arg6,   const QVariant &arg7)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)8 << objectName << method << arg0
                << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7;
    finalizeConnection();

}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3,
                          const QVariant &arg4,   const QVariant &arg5,
                          const QVariant &arg6,   const QVariant &arg7,
                          const QVariant &arg8)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)9 << objectName << method << arg0
                << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;
    finalizeConnection();
}

void DuiRmiClient::invoke(const char *objectName, const char *method,
                          const QVariant &arg0,   const QVariant &arg1,
                          const QVariant &arg2,   const QVariant &arg3,
                          const QVariant &arg4,   const QVariant &arg5,
                          const QVariant &arg6,   const QVariant &arg7,
                          const QVariant &arg8,   const QVariant &arg9)
{
    Q_D(DuiRmiClient);
    initConnection();
    d->stream() << (quint16)0 << (quint16)10 << objectName << method << arg0
                << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8
                << arg9;
    finalizeConnection();
}

#include "moc_duirmiclient.cpp"

