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

#ifndef UT_MEXTENSIONHANDLE
#define UT_MEXTENSIONHANDLE

#include <QObject>
#include "mextensionhandle.h"
#include "mextensionhandle_p.h"
#include "mextensionhandlestyle.h"

class MTestExtensionHandlePrivate : public MExtensionHandlePrivate
{
};

class MTestExtensionHandle : public MExtensionHandle
{
    Q_DECLARE_PRIVATE(MTestExtensionHandle)

public:
    MTestExtensionHandle();
    void sendMouseEvent(QGraphicsSceneMouseEvent &event);
    void messageReceivedFromRunner(const MAppletMessage &message);
    void aliveMessageResponseTimeout();
    void connectionTimeout();
    void sendContextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    MTestExtensionHandlePrivate *privateClass();

};

// Test case must inherit QObject
class Ut_MExtensionHandle : public QObject
{
    Q_OBJECT

private:
    //! ExtensionHandle instance under testing.
    MTestExtensionHandle *handle;

public:
    //! Indicates whether communication server listens to incoming connections or not.
    static bool listenForConnection;
    //! Whether the shared memory is attached or not
    static bool sharedMemoryAttached;
    //! Whether the MWidget::contextMenuEvent() has been called
    static bool contextMenuOpened;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testInitializationWithCorrectData();
    void testReinit();
    void testAppletCommunicationFails();
    void testAppletMalfunction();
    void testAppletRunnerFailsToStart();
    void testScaling();
    void testMouseEventScaling();
    void testAppletCommunication();
    void testRelayingMousePress();
    void testRelayingMouseRelease();
    void testRelayingMouseMove();
    void testThatAppletBreaksIfConnectionIsNotEstablished();
    void testVisibility();
    void testSettingRemoteActions();
    void testRemoteActionsInBrokenState();
    void testContextMenuTrigger();
    void testObjectMenuMessage();
    void testRemoteActionTriggered();
    void testPlaceHolderInitialization();
    void testPlaceHolderInitializationWithError();
    void testInstallationOperationCompleteWithError();
    void testAppletInstallationProgess();
    void testSetSizeHints();
#ifdef HAVE_CONTEXTSUBSCRIBER
    void testWhenDisplayBlankedThenAliveCheckingStops();
#endif

signals:
    void messageReceived(const MAppletMessage &message);
    void connectionFromRunnerEstablished();
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    void operationProgress(const QString &operation, const QString &pkg, int);
};
#endif // UT_MEXTENSIONHANDLE
