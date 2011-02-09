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

#include <mdbusinterface.h>
#include <QDBusPendingCall>
#include <QDBusPendingReply>
#include "ut_mappletinstantiator.h"
#include "mappletinstantiator.h"

// QDBusInterface stubs (used by MRemoteAction)
MDBusInterface::MDBusInterface(const QString &service, const QString &path, const char *interface, const QDBusConnection &connection, QObject *parent) :
        QDBusAbstractInterface(service, path, interface, connection, parent)
{
    Ut_MAppletInstantiator::callServiceNames.append(service);
    Ut_MAppletInstantiator::callObjectPaths.append(path);
    Ut_MAppletInstantiator::callInterfaces.append(interface);
}

MDBusInterface::~MDBusInterface()
{
}

// QDBusAbstractInterface stubs (used by MRemoteAction)
QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode, const QString &method,
        const QVariant &arg1,
        const QVariant &arg2,
        const QVariant &arg3,
        const QVariant &arg4,
        const QVariant &arg5,
        const QVariant &arg6,
        const QVariant &arg7,
        const QVariant &arg8)
{
    if (Ut_MAppletInstantiator::captureCalls) {
        QList<QVariant> args;
        if (arg1.isValid()) {
            args.append(arg1);
            if (arg2.isValid()) {
                args.append(arg2);
                if (arg3.isValid()) {
                    args.append(arg3);
                    if (arg4.isValid()) {
                        args.append(arg4);
                        if (arg5.isValid()) {
                            args.append(arg5);
                            if (arg6.isValid()) {
                                args.append(arg6);
                                if (arg7.isValid()) {
                                    args.append(arg7);
                                    if (arg8.isValid()) {
                                        args.append(arg8);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Ut_MAppletInstantiator::callMethods.append(method);
        Ut_MAppletInstantiator::callArguments.append(args);
    }

    return QDBusMessage();
}

// QDBusPendingCall stubs (used by MAppletInstantiator)
QDBusPendingCall QDBusAbstractInterface::asyncCall(const QString &method,
        const QVariant &arg1,
        const QVariant &arg2,
        const QVariant &arg3,
        const QVariant &arg4,
        const QVariant &arg5,
        const QVariant &arg6,
        const QVariant &arg7,
        const QVariant &arg8)

{
    if (Ut_MAppletInstantiator::captureCalls) {
        QList<QVariant> args;
        if (arg1.isValid()) {
            args.append(arg1);
            if (arg2.isValid()) {
                args.append(arg2);
                if (arg3.isValid()) {
                    args.append(arg3);
                    if (arg4.isValid()) {
                        args.append(arg4);
                        if (arg5.isValid()) {
                            args.append(arg5);
                            if (arg6.isValid()) {
                                args.append(arg6);
                                if (arg7.isValid()) {
                                    args.append(arg7);
                                    if (arg8.isValid()) {
                                        args.append(arg8);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Ut_MAppletInstantiator::callMethods.append(Ut_MAppletInstantiator::callInterfaces.last() + '.' + method);
        Ut_MAppletInstantiator::callArguments.append(args);
    }

    return QDBusPendingCall::fromCompletedCall(QDBusMessage());
}

bool QDBusPendingCall::isError() const
{
    return Ut_MAppletInstantiator::callError;
}

// QDBusPendingReply stubs (used by MAppletInstantiator)
void QDBusPendingReplyData::setMetaTypes(int, int const *)
{
}

QVariant QDBusPendingReplyData::argumentAt(int) const
{
    return Ut_MAppletInstantiator::replyDataMap;
}

bool Ut_MAppletInstantiator::captureCalls = false;
QList<QString> Ut_MAppletInstantiator::callServiceNames;
QList<QString> Ut_MAppletInstantiator::callObjectPaths;
QList<QString> Ut_MAppletInstantiator::callInterfaces;
QList<QString> Ut_MAppletInstantiator::callMethods;
QList< QList<QVariant> > Ut_MAppletInstantiator::callArguments;
bool Ut_MAppletInstantiator::callError = false;
QMap<QString, QVariant> Ut_MAppletInstantiator::replyDataMap;

void Ut_MAppletInstantiator::initTestCase()
{
}

void Ut_MAppletInstantiator::cleanupTestCase()
{
}

void Ut_MAppletInstantiator::init()
{
    captureCalls = false;
    callServiceNames.clear();
    callObjectPaths.clear();
    callInterfaces.clear();
    callMethods.clear();
    callArguments.clear();
    callError = false;
    replyDataMap.clear();
    connect(this, SIGNAL(receivePackageData(QDBusPendingCallWatcher *)), MAppletInstantiator::instance(), SLOT(receivePackageData(QDBusPendingCallWatcher *)));
}

void Ut_MAppletInstantiator::cleanup()
{
}

void Ut_MAppletInstantiator::testInstantiateAppletInPackageToKnownCanvas()
{
    static const QString TEST_PACKAGE_NAME = "testPackage.deb";
    static const QString TEST_PACKAGE_BASE_NAME = "testPackage";
    static const QString CANVAS_DBUS_ADDRESS = "service/path/to/canvas";
    captureCalls = true;

    // Test that telling the instantiator that a package is being installed asks for the meta data of the package
    MAppletInstantiator::instance()->instantiateAppletsInPackage(TEST_PACKAGE_NAME, CANVAS_DBUS_ADDRESS);
    QCOMPARE(callServiceNames.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callObjectPaths.last(), QString("/com/nokia/package_manager"));
    QCOMPARE(callInterfaces.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callMethods.count(), 1);
    QCOMPARE(callMethods.at(0), QString("com.nokia.package_manager.FetchPackageData"));
    QCOMPARE(callArguments.at(0).count(), 2);
    QCOMPARE(callArguments.at(0).at(0).type(), QVariant::String);
    QCOMPARE(callArguments.at(0).at(0).toString(), TEST_PACKAGE_BASE_NAME);
    QCOMPARE(callArguments.at(0).at(1).type(), QVariant::String);
    QCOMPARE(callArguments.at(0).at(1).toString(), QString());
}

void Ut_MAppletInstantiator::testInstantiateAppletInPackageToKnownCanvasFromLocalFile()
{
    static const QString TEST_PACKAGE_NAME = "./testPackage.deb";
    static const QString CANVAS_DBUS_ADDRESS = "service/path/to/canvas";
    captureCalls = true;

    // Test that telling the instantiator that a package is being installed asks for the meta data of the package
    MAppletInstantiator::instance()->instantiateAppletsInPackage(TEST_PACKAGE_NAME, CANVAS_DBUS_ADDRESS);
    QCOMPARE(callServiceNames.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callObjectPaths.last(), QString("/com/nokia/package_manager"));
    QCOMPARE(callInterfaces.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callMethods.count(), 1);
    QCOMPARE(callMethods.at(0), QString("com.nokia.package_manager.FetchPackageDataFile"));
    QCOMPARE(callArguments.at(0).count(), 1);
    QCOMPARE(callArguments.at(0).at(0).type(), QVariant::String);
    QCOMPARE(callArguments.at(0).at(0).toString(), TEST_PACKAGE_NAME);
}

void Ut_MAppletInstantiator::testInstantiateAppletInPackageToInvalidCanvas()
{
    static const QString TEST_PACKAGE_NAME = "testPackage.deb";
    static const QString INVALID_CANVAS_DBUS_ADDRESS = "invalidCanvas";
    captureCalls = true;

    // Test that instantiating a package to a canvas with an invalid address does nothing
    MAppletInstantiator::instance()->instantiateAppletsInPackage(TEST_PACKAGE_NAME, INVALID_CANVAS_DBUS_ADDRESS);

    QCOMPARE(callMethods.count(), 0);
}

void Ut_MAppletInstantiator::testReceivePackageData()
{
    static const QString TEST_PACKAGE_NAME = "testPackage.deb";
    static const QString CANVAS_DBUS_ADDRESS = "service/path/to/canvas";
    static const QString DESKTOP_FILE_NAME = "test.desktop";
    captureCalls = true;

    // Test that receiving the meta data of the package being installed informs the target canvas about it and starts the installation
    QDBusMessage msg;
    QDBusPendingCall call = QDBusPendingCall::fromCompletedCall(msg);
    QDBusPendingCallWatcher watcher(call, NULL);
    watcher.setProperty("packageName", TEST_PACKAGE_NAME);
    watcher.setProperty("canvasDBusAddress", CANVAS_DBUS_ADDRESS);
    replyDataMap.insert("Maemo-Desktop-File", DESKTOP_FILE_NAME);
    emit receivePackageData(&watcher);

    // The canvas should be told
    QCOMPARE(callServiceNames.first(), QString("service"));
    QCOMPARE(callObjectPaths.first(), QString("/path/to/canvas"));
    QCOMPARE(callInterfaces.first(), QString("com.meego.core.MAppletInstanceManager"));
    QVERIFY(callMethods.count() > 1);
    QCOMPARE(callMethods.first(), QString("com.meego.core.MAppletInstanceManager.instantiateAppletFromPackage"));
    QCOMPARE(callArguments.first().count(), 2);
    QCOMPARE(callArguments.first().at(0).type(), QVariant::String);
    QCOMPARE(callArguments.first().at(0).toString(), TEST_PACKAGE_NAME);
    QCOMPARE(callArguments.first().at(1).type(), QVariant::Map);
    QCOMPARE(callArguments.first().at(1).toMap().value("Maemo-Desktop-File"), QVariant(DESKTOP_FILE_NAME));

    // The installation should start
    QCOMPARE(callServiceNames.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callObjectPaths.last(), QString("/com/nokia/package_manager"));
    QCOMPARE(callInterfaces.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callMethods.last(), QString("com.nokia.package_manager.Install"));
    QCOMPARE(callArguments.last().count(), 1);
    QCOMPARE(callArguments.last().at(0).type(), QVariant::String);
    QCOMPARE(callArguments.last().at(0).toString(), TEST_PACKAGE_NAME);
}

void Ut_MAppletInstantiator::testReceivePackageDataFromLocalFile()
{
    static const QString TEST_PACKAGE_NAME = "./testPackage.deb";
    static const QString CANVAS_DBUS_ADDRESS = "service/path/to/canvas";
    static const QString DESKTOP_FILE_NAME = "test.desktop";
    captureCalls = true;

    // Test that receiving the meta data of the package being installed informs the target canvas about it and starts the installation
    QDBusMessage msg;
    QDBusPendingCall call = QDBusPendingCall::fromCompletedCall(msg);
    QDBusPendingCallWatcher watcher(call, NULL);
    watcher.setProperty("packageName", TEST_PACKAGE_NAME);
    watcher.setProperty("canvasDBusAddress", CANVAS_DBUS_ADDRESS);
    replyDataMap.insert("Maemo-Desktop-File", DESKTOP_FILE_NAME);
    emit receivePackageData(&watcher);

    // The installation should start
    QCOMPARE(callServiceNames.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callObjectPaths.last(), QString("/com/nokia/package_manager"));
    QCOMPARE(callInterfaces.last(), QString("com.nokia.package_manager"));
    QCOMPARE(callMethods.last(), QString("com.nokia.package_manager.InstallFile"));
    QCOMPARE(callArguments.last().count(), 1);
    QCOMPARE(callArguments.last().at(0).type(), QVariant::String);
    QCOMPARE(callArguments.last().at(0).toString(), TEST_PACKAGE_NAME);
}

void Ut_MAppletInstantiator::testReceivePackageDataWithError()
{
    static const QString TEST_PACKAGE_NAME = "testPackage.deb";
    static const QString CANVAS_DBUS_ADDRESS = "service/path/to/canvas";
    captureCalls = true;

    // Test that receiving an error instead of meta data does nothing
    callError = true;
    QDBusMessage msg;
    QDBusPendingCall call = QDBusPendingCall::fromCompletedCall(msg);
    QDBusPendingCallWatcher watcher(call, NULL);
    watcher.setProperty("packageName", TEST_PACKAGE_NAME);
    watcher.setProperty("canvasDBusAddress", CANVAS_DBUS_ADDRESS);
    emit receivePackageData(&watcher);

    // The canvas should be told with no meta data
    QCOMPARE(callServiceNames.first(), QString("service"));
    QCOMPARE(callObjectPaths.first(), QString("/path/to/canvas"));
    QCOMPARE(callInterfaces.first(), QString("com.meego.core.MAppletInstanceManager"));
    QVERIFY(callMethods.count() > 1);
    QCOMPARE(callMethods.first(), QString("com.meego.core.MAppletInstanceManager.instantiateAppletFromPackage"));
    QCOMPARE(callArguments.first().count(), 2);
    QCOMPARE(callArguments.first().at(0).type(), QVariant::String);
    QCOMPARE(callArguments.first().at(0).toString(), TEST_PACKAGE_NAME);
    QCOMPARE(callArguments.first().at(1).type(), QVariant::Map);
    QCOMPARE(callArguments.first().at(1).toMap().size(), 0);
}

QTEST_APPLESS_MAIN(Ut_MAppletInstantiator)
