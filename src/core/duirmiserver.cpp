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

#include "duirmiserver.h"
#include "duirmiserver_p.h"

#include <QLocalSocket>
#include <QDataStream>
#include <QMetaObject>
#include <QGenericArgument>
#include <QFile>
#include <QTextStream>
#include <DuiDebug>

QGenericArgument unmarshall(const char *name, const void *data)
{
    return QGenericArgument(name, data);
}

DuiRmiServerPrivate::DuiRmiServerPrivate(const QString &key)
    : _key(key), _obj(0)
{
}

DuiRmiServerPrivate::~DuiRmiServerPrivate()
{
}

void DuiRmiServerPrivate::exportObject(QObject *p)
{
    _obj = p;
}

// TODO object selection from multiple sources
QObject *DuiRmiServerPrivate::currentObject()
{
    return _obj;
}

QString DuiRmiServerPrivate::key() const
{
    return _key;
}

DuiRmiServerPrivateSocket::DuiRmiServerPrivateSocket(const QString &key)
    : DuiRmiServerPrivate(key), method_size(0)
{
}

void DuiRmiServerPrivateSocket::exportObject(QObject *p)
{
    Q_Q(DuiRmiServer);
    DuiRmiServerPrivate::exportObject(p);

    q->connect(&_serv, SIGNAL(newConnection()), q, SLOT(_q_incoming()));

    if (QFile::exists("/tmp/" + key()))
        QFile::remove("/tmp/" + key());

    if (!_serv.listen(key()))
        duiDebug("DuiRmiServerPrivateSocket") << "system error, can't listen to local socket";
}

void DuiRmiServerPrivateSocket::_q_incoming()
{
    Q_Q(DuiRmiServer);
    QLocalSocket *s = _serv.nextPendingConnection();
    q->connect(s, SIGNAL(disconnected()), s, SLOT(deleteLater()));
    if (!s)
        return;
    _sock = s;
    q->connect(_sock, SIGNAL(readyRead()), q, SLOT(_q_readData()));
}

void DuiRmiServerPrivateSocket::_q_readData()
{
    uint sz = _sock->bytesAvailable();

    QDataStream stream(_sock);
    stream.setVersion(QDataStream::Qt_4_0);

    if (method_size == 0) {
        if (sz < (int)sizeof(quint16))
            return;
        stream >> method_size;
    }

    if (sz < method_size)
        return;

    invoke(stream);
}

void DuiRmiServerPrivateSocket::invoke(QDataStream &stream)
{
    char *className   = 0;
    char *methodName  = 0;
    quint16 arglength = 0;

    QVariant arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9;

    stream >> arglength >> className >> methodName;
    switch (arglength) {
    case 0:
        QMetaObject::invokeMethod(currentObject(),
                                  methodName);
        break;
    case 1:
        stream >> arg0;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()));

        break;
    case 2:
        stream >> arg0 >> arg1;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()));
        break;
    case 3:
        stream >> arg0 >> arg1 >> arg2;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()));
        break;
    case 4:
        stream >> arg0 >> arg1 >> arg2 >> arg3;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()));
        break;
    case 5:
        stream >> arg0 >> arg1 >> arg2 >> arg3 >> arg4;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()),
                                  unmarshall(arg4.typeName(),
                                             arg4.data()));
        break;
    case 6:
        stream >> arg0 >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()),
                                  unmarshall(arg4.typeName(),
                                             arg4.data()),
                                  unmarshall(arg5.typeName(),
                                             arg5.data()));
        break;
    case 7:
        stream >> arg0 >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()),
                                  unmarshall(arg4.typeName(),
                                             arg4.data()),
                                  unmarshall(arg5.typeName(),
                                             arg5.data()),
                                  unmarshall(arg6.typeName(),
                                             arg6.data()));
        break;
    case 8:
        stream >> arg0 >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6 >> arg7;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()),
                                  unmarshall(arg4.typeName(),
                                             arg4.data()),
                                  unmarshall(arg5.typeName(),
                                             arg5.data()),
                                  unmarshall(arg6.typeName(),
                                             arg6.data()),
                                  unmarshall(arg7.typeName(),
                                             arg7.data()));
        break;
    case 9:
        stream >> arg0 >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6 >> arg7
               >> arg8;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()),
                                  unmarshall(arg4.typeName(),
                                             arg4.data()),
                                  unmarshall(arg5.typeName(),
                                             arg5.data()),
                                  unmarshall(arg6.typeName(),
                                             arg6.data()),
                                  unmarshall(arg7.typeName(),
                                             arg7.data()),
                                  unmarshall(arg8.typeName(),
                                             arg8.data()));
        break;
    case 10:
        stream >> arg0 >> arg1 >> arg2 >> arg3 >> arg4 >> arg5 >> arg6 >> arg7
               >> arg8 >> arg9;
        QMetaObject::invokeMethod(currentObject(),
                                  methodName,
                                  unmarshall(arg0.typeName(),
                                             arg0.data()),
                                  unmarshall(arg1.typeName(),
                                             arg1.data()),
                                  unmarshall(arg2.typeName(),
                                             arg2.data()),
                                  unmarshall(arg3.typeName(),
                                             arg3.data()),
                                  unmarshall(arg4.typeName(),
                                             arg4.data()),
                                  unmarshall(arg5.typeName(),
                                             arg5.data()),
                                  unmarshall(arg6.typeName(),
                                             arg6.data()),
                                  unmarshall(arg7.typeName(),
                                             arg7.data()),
                                  unmarshall(arg8.typeName(),
                                             arg8.data()),
                                  unmarshall(arg9.typeName(),
                                             arg9.data()));
        break;
    default:
        break;

    }

    delete[] className;
    delete[] methodName;
    method_size = 0;
}


DuiRmiServer::DuiRmiServer(const QString &key, QObject *p)
    : QObject(p),
      d_ptr(new DuiRmiServerPrivateSocket(key))
{
    d_ptr->q_ptr = this;
}


DuiRmiServer::~DuiRmiServer()
{
    delete d_ptr;
}


void DuiRmiServer::exportObject(QObject *obj)
{
    Q_D(DuiRmiServer);
    d->exportObject(obj);
}

#include "moc_duirmiserver.cpp"

