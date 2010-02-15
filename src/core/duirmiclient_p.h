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

#ifndef DUIRMICLIENT_P_H
#define DUIRMICLIENT_P_H

#include <QLocalSocket>
#include <QByteArray>

class QDataStream;
class DuiRmiClient;

class DuiRmiClientPrivate
{
    Q_DECLARE_PUBLIC(DuiRmiClient)
public:
    DuiRmiClientPrivate(const QString &key);
    virtual ~DuiRmiClientPrivate();
    virtual void writeData(const QByteArray &) = 0;
    virtual void connectToServer() = 0;
    virtual QDataStream &stream() = 0;

    virtual void initConnection();
    virtual void finalizeConnection();

    QString key() const;

    void returnValue(const QVariant &v);

    DuiRmiClient *q_ptr;

    virtual void _q_readyRead() = 0;
private:
    QString _key;
};

class DuiRmiClientPrivateSocket: public DuiRmiClientPrivate
{
    Q_DECLARE_PUBLIC(DuiRmiClient)
public:

    DuiRmiClientPrivateSocket(const QString &key, DuiRmiClient *q);

    void writeData(const QByteArray &);
    void connectToServer();
    void initConnection();
    void finalizeConnection();
    QDataStream &stream();

    virtual void _q_readyRead();

private:
    QLocalSocket _socket;
    QByteArray   _buf;
    QDataStream *_stream;
    quint16 return_sz;
};

#endif //DUIRMICLIENT_P_H
