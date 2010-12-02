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

#include <QtTest/QtTest>

#include "ut_mapplethandle.h"
#include <mextensionhandle.h>
#include <mapplethandle.h>
#include "mappleticonchangedmessage.h"
#include "mapplettitlechangedmessage.h"
#include "mapplettextchangedmessage.h"
#include <mappletid_stub.h>
#include <mobjectmenu_stub.h>
#include <mappletsharedmutex_stub.h>
#include <mapplication.h>

// MWidget stubs (used by MExtensionHandle)
MWidgetPrivate::MWidgetPrivate() :
        explicitlyHidden(false),
        layoutHidden(false),
        q_ptr(NULL),
        onDisplay(false),
        selected(false)
{
}

MWidgetPrivate::~MWidgetPrivate()
{
}

void MWidgetPrivate::doEnterDisplayEvent()
{
}

void MWidgetPrivate::doExitDisplayEvent()
{
}
// AppletCommunicator stubs to get rid of IPC
bool MAppletServer::startServer(const QString &)
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
MTestAppletHandle::MTestAppletHandle() : MAppletHandle()
{
}

void MTestAppletHandle::messageReceivedFromRunner(const MAppletMessage &message)
{
    messageReceived(message);
}

// Unit test cases
void Ut_MAppletHandle::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplethandle" };
    app = new MApplication(argc, app_name);
}

void Ut_MAppletHandle::cleanupTestCase()
{
    delete app;
}

void Ut_MAppletHandle::init()
{
    handle = new MTestAppletHandle();
    handle->setViewType("default");
    gMAppletSharedMutexStub->stubReset();
    gMAppletSharedMutexStub->stubSetReturnValue("init", true);
}

void Ut_MAppletHandle::cleanup()
{
    delete handle;
}

void Ut_MAppletHandle::testInitialization()
{
    QString runnerBinary("runnerBinary");
    QString appletInstanceFileDataPath("appletInstanceFileDataPath");
    QString metaDataFileName("metaDataFileName");
    MAppletId appletId;
    handle->init(runnerBinary, appletInstanceFileDataPath, metaDataFileName, appletId);
    QCOMPARE(handle->model()->appletId().toString(), appletId.toString());
    QVERIFY(handle->model()->appletSettings() != NULL);

    // Check that correct arguments were passed on to the runner
    QCOMPARE(gQProcessProgram, runnerBinary);
    QCOMPARE(gQProcessArguments.count(), 4);
    QCOMPARE(gQProcessArguments.at(2), appletId.toString());
    QCOMPARE(gQProcessArguments.at(3), appletInstanceFileDataPath);
}

void Ut_MAppletHandle::testAppletIconMessageReceived()
{
    QSignalSpy spy(handle, SIGNAL(appletIconChanged(QString)));
    MAppletIconChangedMessage message;
    message.setIcon("test-icon");
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->appletIcon(), QString("test-icon"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).count(), 1);
    QCOMPARE(spy.at(0).at(0), QVariant("test-icon"));
}

void Ut_MAppletHandle::testAppletTitleMessageReceived()
{
    QSignalSpy spy(handle, SIGNAL(appletTitleChanged(QString)));
    MAppletTitleChangedMessage message;
    message.setTitle("test-title");
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->appletTitle(), QString("test-title"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).count(), 1);
    QCOMPARE(spy.at(0).at(0), QVariant("test-title"));
}

void Ut_MAppletHandle::testAppletTextMessageReceived()
{
    QSignalSpy spy(handle, SIGNAL(appletTextChanged(QString)));
    MAppletTextChangedMessage message;
    message.setText("test-text");
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->appletText(), QString("test-text"));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).count(), 1);
    QCOMPARE(spy.at(0).at(0), QVariant("test-text"));
}

QTEST_APPLESS_MAIN(Ut_MAppletHandle)
