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

#include "ut_mremoteaction.h"
#include "mdbusinterface.h"
#include <maction_stub.h>
#include "maction_p.h"
#include <QDBusPendingCall>

bool Ut_MRemoteAction::captureCalls = false;
QString Ut_MRemoteAction::callServiceName;
QString Ut_MRemoteAction::callObjectPath;
QString Ut_MRemoteAction::callInterface;
QList<QString> Ut_MRemoteAction::callMethods;
QList< QList<QVariant> > Ut_MRemoteAction::callArguments;

// MActionPrivate stubs
MActionPrivate::MActionPrivate()
  : location(MAction::EveryLocation), styleAction(false), q_ptr(0)
{
}

MActionPrivate::~MActionPrivate()
{
}

// MDBusInterface stubs (used by MRemoteAction)
MDBusInterface::MDBusInterface(const QString &service, const QString &path, const char *interface, const QDBusConnection &connection, QObject *parent) : QDBusAbstractInterface(service, path, interface, connection, parent)
{
    Ut_MRemoteAction::callServiceName = service;
    Ut_MRemoteAction::callObjectPath = path;
    Ut_MRemoteAction::callInterface = interface;
}

MDBusInterface::~MDBusInterface()
{
}

// QDBusAbstractInterface stubs (used by MRemoteAction)
QDBusPendingCall QDBusAbstractInterface::asyncCallWithArgumentList(const QString &method, const QList<QVariant> &args)
{
    if (Ut_MRemoteAction::captureCalls) {
        Ut_MRemoteAction::callMethods.append(method);
        Ut_MRemoteAction::callArguments.append(args);
    }

    return QDBusPendingCall::fromCompletedCall(QDBusMessage());
}

void Ut_MRemoteAction::init()
{
    captureCalls = false;
    callServiceName = QString();
    callObjectPath = QString();
    callInterface = QString();
    callMethods.clear();
    callArguments.clear();
}

void Ut_MRemoteAction::cleanup()
{
}

void Ut_MRemoteAction::initTestCase()
{
}

void Ut_MRemoteAction::cleanupTestCase()
{
}

void Ut_MRemoteAction::testSerialization()
{
    // Create two arguments as QVariants
    QVariant arg1("arg1");
    QVariant arg2(2);

    // Serialize the QVariants into QBuffers
    QBuffer buffer1;
    QBuffer buffer2;
    buffer1.open(QIODevice::ReadWrite);
    buffer2.open(QIODevice::ReadWrite);
    QDataStream stream1(&buffer1);
    QDataStream stream2(&buffer2);
    stream1 << arg1;
    stream2 << arg2;
    buffer1.close();
    buffer2.close();

    // Encode the contents of the QBuffers in Base64
    QString arg1String(buffer1.buffer().toBase64().data());
    QString arg2String(buffer2.buffer().toBase64().data());

    // Test that an empty action serializes into an empty action
    MRemoteAction action1;
    QCOMPARE(action1.toString(), QString());

    // Test that a class deserialized from a string serializes into the same string
    QString action2String("serviceName objectPath interface methodName ");
    action2String.append(arg1String);
    action2String.append(' ');
    action2String.append(arg2String);
    MRemoteAction action2(action2String);
    QCOMPARE(action2.toString(), action2String);

    // Test that passing the parameters separately will result in the same string
    QList<QVariant> args;
    args.append(arg1);
    args.append(arg2);
    MRemoteAction action3("serviceName", "objectPath", "interface", "methodName", args);
    QCOMPARE(action3.toString(), action2String);
}

void Ut_MRemoteAction::testTrigger()
{
    // Create an action with a method call and some QVariant arguments
    QList<QVariant> args;
    args.append(QVariant("arg1"));
    args.append(QVariant(2));
    MRemoteAction action("serviceName", "objectPath", "interface", "methodName", args);
    captureCalls = true;
    action.trigger();

    // Make sure the corrent method was called with the corrent arguments
    QCOMPARE(callServiceName, QString("serviceName"));
    QCOMPARE(callObjectPath, QString("objectPath"));
    QCOMPARE(callInterface, QString("interface"));
    QCOMPARE(callMethods.count(), 1);
    QCOMPARE(callMethods.at(0), QString("methodName"));
    QCOMPARE(callArguments.count(), 1);
    QCOMPARE(callArguments.at(0).count(), 2);
    QCOMPARE(callArguments.at(0).at(0), QVariant("arg1"));
    QCOMPARE(callArguments.at(0).at(1), QVariant(2));
}

QTEST_MAIN(Ut_MRemoteAction)
