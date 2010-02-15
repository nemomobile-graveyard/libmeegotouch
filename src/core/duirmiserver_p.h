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

#ifndef DUIRMISERVER_P_H
#define DUIRMISERVER_P_H

#include <QVariant>
#include <QLocalServer>

class QDataStream;

class DuiRmiServerPrivate
{
    Q_DECLARE_PUBLIC(DuiRmiServer)
public:

    DuiRmiServerPrivate(const QString &key);
    virtual ~DuiRmiServerPrivate();
    virtual void exportObject(QObject *p);
    QObject *currentObject();
    QString key() const;

    virtual void _q_incoming() = 0;
    virtual void _q_readData() = 0;

    DuiRmiServer *q_ptr;
private:
    QString _key;
    QObject *_obj;
};

class DuiRmiServerPrivateSocket: public DuiRmiServerPrivate
{
    Q_DECLARE_PUBLIC(DuiRmiServer)

public:
    DuiRmiServerPrivateSocket(const QString &key);

    virtual void exportObject(QObject *p);

    virtual void _q_incoming();
    virtual void _q_readData();

private:
    void invoke(QDataStream &);

    QLocalServer  _serv;
    QLocalSocket *_sock;
    quint16 method_size;
};

#endif //DUIRMISERVER_P_H
