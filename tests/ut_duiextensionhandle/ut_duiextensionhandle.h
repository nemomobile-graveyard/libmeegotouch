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

#ifndef UT_DUIEXTENSIONHANDLE
#define UT_DUIEXTENSIONHANDLE

#include <QObject>
#include "duiextensionhandle.h"
#include "duiextensionhandle_p.h"
#include "duiextensionhandlestyle.h"

class DuiTestExtensionHandlePrivate : public DuiExtensionHandlePrivate
{
};

class DuiTestExtensionHandle : public DuiExtensionHandle
{
    Q_DECLARE_PRIVATE(DuiTestExtensionHandle)

public:
    DuiTestExtensionHandle();
    void sendMouseEvent(QGraphicsSceneMouseEvent &event);
    void messageReceivedFromRunner(const DuiAppletMessage &message);
    void aliveMessageResponseTimeout();
    void connectionTimeout();
    void sendContextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    DuiTestExtensionHandlePrivate *privateClass();

    DuiExtensionHandleStyle *modifiableStyle() {
        DuiExtensionHandleStyleContainer &sc = dynamic_cast<DuiExtensionHandleStyleContainer &>(style());
        const DuiExtensionHandleStyle *const_s = sc.operator ->();
        DuiExtensionHandleStyle *s = const_cast<DuiExtensionHandleStyle *>(const_s);
        return s;
    }
};

// Test case must inherit QObject
class Ut_DuiExtensionHandle : public QObject
{
    Q_OBJECT

private:
    //! ExtensionHandle instance under testing.
    DuiTestExtensionHandle *handle;

public:
    //! Indicates whether communication server listens to incoming connections or not.
    static bool listenForConnection;

    //! Minimum size hint response
    static QSizeF minSize;
    //! Preferred size hint response
    static QSizeF prefSize;
    //! Maximum size hint response
    static QSizeF maxSize;
    //! The visible scene size returned by DuiSceneManager
    static QSize visibleSceneSize;

    //! Whether the shared memory is attached or not
    static bool sharedMemoryAttached;
    //! Whether the DuiWidget::contextMenuEvent() has been called
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

signals:
    void messageReceived(const DuiAppletMessage &message);
    void connectionFromRunnerEstablished();
    void widgetVisibilityChanged(bool visible);
    void applicationVisibilityChanged(bool visible);
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    void operationProgress(const QString &operation, const QString &pkg, int);
};
#endif // UT_DUIEXTENSIONHANDLE
