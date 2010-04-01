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

#include "duiremoteaction.h"
#include "duiremoteaction_p.h"
#include <QDBusInterface>
#include <QBuffer>

DuiRemoteActionPrivate::DuiRemoteActionPrivate() :
    DuiActionPrivate()
{
}

DuiRemoteActionPrivate::~DuiRemoteActionPrivate()
{
}

DuiRemoteAction::DuiRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments, QObject *parent) : DuiAction(*new DuiRemoteActionPrivate, parent)
{
    Q_D(DuiRemoteAction);

    d->serviceName = serviceName;
    d->objectPath = objectPath;
    d->interface = interface;
    d->methodName = methodName;
    d->arguments = arguments;

    connect(this, SIGNAL(triggered()), this, SLOT(call()));
}

DuiRemoteAction::DuiRemoteAction(const QString &string, QObject *parent) : DuiAction(*new DuiRemoteActionPrivate, parent)
{
    fromString(string);

    connect(this, SIGNAL(triggered()), this, SLOT(call()));
}

DuiRemoteAction::DuiRemoteAction(DuiRemoteActionPrivate &dd, QObject *parent) : DuiAction(dd, parent)
{
    connect(this, SIGNAL(triggered()), this, SLOT(call()));
}

DuiRemoteAction::~DuiRemoteAction()
{
}

QString DuiRemoteAction::toString() const
{
    Q_D(const DuiRemoteAction);

    QString s;
    if (!d->serviceName.isEmpty() && !d->objectPath.isEmpty() && !d->interface.isEmpty() && !d->methodName.isEmpty()) {
        s.append(d->serviceName).append(' ');
        s.append(d->objectPath).append(' ');
        s.append(d->interface).append(' ');
        s.append(d->methodName);

        foreach(const QVariant & arg, d->arguments) {
            // Serialize the QVariant into a QBuffer
            QBuffer buffer;
            buffer.open(QIODevice::ReadWrite);
            QDataStream stream(&buffer);
            stream << arg;
            buffer.close();

            // Encode the contents of the QBuffer in Base64
            s.append(' ');
            s.append(buffer.buffer().toBase64().data());
        }
    }

    return s;
}

void DuiRemoteAction::fromString(const QString &string)
{
    Q_D(DuiRemoteAction);

    QStringList l = string.split(' ');

    if (l.count() > 3) {
        d->serviceName = l.at(0);
        d->objectPath = l.at(1);
        d->interface = l.at(2);
        d->methodName = l.at(3);
    }

    const int count = l.count();
    for (int i = 4; i < count; ++i) {
        QByteArray byteArray = QByteArray::fromBase64(l.at(i).toAscii());
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::ReadOnly);
        QDataStream stream(&buffer);
        QVariant arg;
        stream >> arg;
        buffer.close();

        d->arguments.append(arg);
    }
}

DuiRemoteAction::DuiRemoteAction(const DuiRemoteAction &action) : DuiAction(*new DuiRemoteActionPrivate, action.parent())
{
    fromString(action.toString());

    connect(this, SIGNAL(triggered()), this, SLOT(call()));
}

void DuiRemoteAction::call()
{
    Q_D(DuiRemoteAction);

    QDBusInterface interface(d->serviceName, d->objectPath, d->interface);
    interface.callWithArgumentList(QDBus::NoBlock, d->methodName, d->arguments);
}
