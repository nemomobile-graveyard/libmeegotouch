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

#include <QtTest/QtTest>

#include "ut_duiapplethandle.h"
#include <duiextensionhandle.h>
#include <duiapplethandle.h>
#include "duiappleticonchangedmessage.h"
#include "duiapplettitlechangedmessage.h"
#include "duiapplettextchangedmessage.h"
#include <duiappletid_stub.h>
#include <duiobjectmenu_stub.h>
#include <duiappletsharedmutex_stub.h>
#include <duiapplication.h>

// DuiWidget stubs (used by DuiExtensionHandle)
DuiWidgetPrivate::DuiWidgetPrivate() : selected(false)
{
}

DuiWidgetPrivate::~DuiWidgetPrivate()
{
}

// AppletCommunicator stubs to get rid of IPC
bool DuiAppletServer::startServer(const QString &)
{
    return true;
}

// QProcess stubs
bool gStartQProcess = true;
QString gQProcessProgram;
QStringList gQProcessArguments;
void QProcess::start(const QString &program, const QStringList &arguments, OpenMode)
{
    gQProcessProgram = program;
    gQProcessArguments = arguments;

    if (gStartQProcess) {
        emit started();
    } else {
        emit error(QProcess::FailedToStart);
    }
}

Q_PID QProcess::pid() const
{
    if (gStartQProcess) {
        return 100;
    } else {
        return 0;
    }
}

// A test applet handle implementation to allow mouse event sending
DuiTestAppletHandle::DuiTestAppletHandle() : DuiAppletHandle()
{
}

void DuiTestAppletHandle::messageReceivedFromRunner(const DuiAppletMessage &message)
{
    messageReceived(message);
}

// Unit test cases
void Ut_DuiAppletHandle::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiapplethandle" };
    app = new DuiApplication(argc, app_name);
}

void Ut_DuiAppletHandle::cleanupTestCase()
{
    delete app;
}

void Ut_DuiAppletHandle::init()
{
    handle = new DuiTestAppletHandle();
    handle->setViewType("default");
    gDuiAppletSharedMutexStub->stubReset();
    gDuiAppletSharedMutexStub->stubSetReturnValue("init", true);
}

void Ut_DuiAppletHandle::cleanup()
{
    delete handle;
}

void Ut_DuiAppletHandle::testInitialization()
{
    QString runnerBinary("runnerBinary");
    QString appletInstanceFileDataPath("appletInstanceFileDataPath");
    QString metaDataFileName("metaDataFileName");
    DuiAppletId appletId;
    handle->init(runnerBinary, appletInstanceFileDataPath, metaDataFileName, appletId);
    QCOMPARE(handle->model()->appletId().toString(), appletId.toString());
    QVERIFY(handle->model()->appletSettings() != NULL);

    // Check that correct arguments were passed on to the runner
    QCOMPARE(gQProcessProgram, runnerBinary);
    QCOMPARE(gQProcessArguments.count(), 4);
    QCOMPARE(gQProcessArguments.at(2), appletId.toString());
    QCOMPARE(gQProcessArguments.at(3), appletInstanceFileDataPath);
}

void Ut_DuiAppletHandle::testAppletIconMessageReceived()
{
    QSignalSpy spy(handle, SIGNAL(appletIconChanged(QString)));
    DuiAppletIconChangedMessage message;
    message.setIcon("test-icon");
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->appletIcon(), QString("test-icon"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).count(), 1);
    QCOMPARE(spy.at(0).at(0), QVariant("test-icon"));
}

void Ut_DuiAppletHandle::testAppletTitleMessageReceived()
{
    QSignalSpy spy(handle, SIGNAL(appletTitleChanged(QString)));
    DuiAppletTitleChangedMessage message;
    message.setTitle("test-title");
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->appletTitle(), QString("test-title"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).count(), 1);
    QCOMPARE(spy.at(0).at(0), QVariant("test-title"));
}

void Ut_DuiAppletHandle::testAppletTextMessageReceived()
{
    QSignalSpy spy(handle, SIGNAL(appletTextChanged(QString)));
    DuiAppletTextChangedMessage message;
    message.setText("test-text");
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->appletText(), QString("test-text"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).count(), 1);
    QCOMPARE(spy.at(0).at(0), QVariant("test-text"));
}

QTEST_APPLESS_MAIN(Ut_DuiAppletHandle)
