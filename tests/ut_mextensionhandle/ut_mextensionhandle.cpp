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
#include <QLocalServer>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xdamage.h>
#endif //HAVE_XDAMAGE

#include "ut_mextensionhandle.h"
#include <mextensionhandle.h>
#include <mappletserver.h>
#include "mappletsetgeometrymessage.h"
#include "mappletmousemessage.h"
#include "mappletalivemessageresponse.h"
#include "mappletpixmaptakenintousemessage.h"
#include "mappletupdategeometrymessage.h"
#include "mappletvisibilitymessage.h"
#include "mappletobjectmenuactionselectedmessage.h"
#include "mappleticonchangedmessage.h"
#include "mapplettitlechangedmessage.h"
#include "mapplettextchangedmessage.h"
#include <mapplication.h>
#include <mapplicationwindow.h>
#include <mscene.h>
#include <mlocale.h>
#include "msettingslanguageparser.h"
#include "mappletsharedmutex_stub.h"
#include "mappletobjectmenurequestmessage.h"
#include "mappletobjectmenumessage.h"
#include "mappletobjectmenuactionselectedmessage.h"
#include "mobjectmenu_stub.h"
#include <QList>
#include <QAction>
#include <mappletid_stub.h>
#include <mscenemanager.h>

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

MApplication *app;

bool visibility;
QPointF mousePressPosition;
QPointF mouseReleasePosition;
QPointF mouseMovePosition;
QPointF contextMenuEventPos;
Qt::MouseButtons mouseEventButtons;
Qt::MouseButton mouseEventButton;
uint remoteActionIndex = 0;
bool Ut_MExtensionHandle::listenForConnection = true;
bool Ut_MExtensionHandle::contextMenuOpened;


void MWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *)
{
    Ut_MExtensionHandle::contextMenuOpened = true;
}

// ContextProperty stubs
#ifdef HAVE_CONTEXTSUBSCRIBER
QString gContextPropertyValue = "";
QVariant ContextProperty::value() const
{
    return gContextPropertyValue;
}
#endif

// MSceneManager stubs (used by MExtensionHandle)
void MSceneManager::appearSceneWindow(MSceneWindow *, MSceneWindow::DeletionPolicy)
{
}

// A test handle implementation to allow mouse event sending
MTestExtensionHandle::MTestExtensionHandle() : MExtensionHandle()
{
}

void MTestExtensionHandle::messageReceivedFromRunner(const MAppletMessage &message)
{
    messageReceived(message);
}

void MTestExtensionHandle::sendMouseEvent(QGraphicsSceneMouseEvent &event)
{
    if (event.type() == QEvent::GraphicsSceneMousePress)
        mousePressEvent(&event);
    else if (event.type() == QEvent::GraphicsSceneMouseRelease)
        mouseReleaseEvent(&event);
    else if (event.type() == QEvent::GraphicsSceneMouseMove)
        mouseMoveEvent(&event);
}

void MTestExtensionHandle::sendContextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    contextMenuEvent(event);
}

void MTestExtensionHandle::aliveMessageResponseTimeout()
{
    communicationTimerTimeout();
}

void MTestExtensionHandle::connectionTimeout()
{
    communicationTimerTimeout();
}

MTestExtensionHandlePrivate *MTestExtensionHandle::privateClass()
{
    Q_D(MTestExtensionHandle);

    return d;
}

// AppletCommunicator stubs to get rid of IPC and to send the size hint
bool MAppletServer::startServer(const QString &serverName)
{
    Q_UNUSED(serverName);
    return Ut_MExtensionHandle::listenForConnection;
}

// Store information from the different message types sent by the handle
bool MAppletCommunicator::sendMessage(const MAppletMessage &message)
{
    const MAppletMouseMessage *mouseMessage = 0;
    switch (message.type()) {
    case MAppletMessage::MousePressMessage:
    case MAppletMessage::MouseReleaseMessage:
    case MAppletMessage::MouseMoveMessage:
        mouseMessage = static_cast<const MAppletMouseMessage *>(&message);
        mouseEventButton = mouseMessage->button();
        mouseEventButtons = mouseMessage->buttons();
        if (message.type() == MAppletMessage::MousePressMessage)
            mousePressPosition = mouseMessage->position();
        else if (message.type() == MAppletMessage::MouseReleaseMessage)
            mouseReleasePosition = mouseMessage->position();
        else if (message.type() == MAppletMessage::MouseMoveMessage)
            mouseMovePosition = mouseMessage->position();
        break;
    case MAppletMessage::ObjectMenuRequestMessage:
        contextMenuEventPos = (static_cast<const MAppletObjectMenuRequestMessage &>(message)).pos();
        break;
    case MAppletMessage::VisibilityMessage:
        visibility = (static_cast<const MAppletVisibilityMessage &>(message)).isVisible();
        break;
    case MAppletMessage::ObjectMenuActionSelectedMessage:
        remoteActionIndex = (static_cast<const MAppletObjectMenuActionSelectedMessage &>(message)).index();
        break;
    default:
        break;
    }

    return true;
}

// QTimer stubs
QList<QTimer*> gQTimerStartCalls;
void QTimer::start()
{
    gQTimerStartCalls.append(this);
    id = 1;
}

void QTimer::start(int)
{
    start();
}

bool gQTimerStopCalled = false;
QList<QTimer*> gQTimerStopCalls;

void QTimer::stop()
{
    gQTimerStopCalled = true;
    gQTimerStopCalls.append(this);
    id = -1;
}

int elapsedQTime = 0;

// QTime stubs
int QTime::elapsed() const
{
    return elapsedQTime;
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

// Unit test cases
void Ut_MExtensionHandle::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mextensionhandle" };
    app = new MApplication(argc, app_name);
}

void Ut_MExtensionHandle::cleanupTestCase()
{
    delete app;
}

void Ut_MExtensionHandle::init()
{
    mouseEventButtons = Qt::NoButton;
    mouseEventButton = Qt::NoButton;
    mousePressPosition = QPointF();
    mouseReleasePosition = QPointF();
    mouseMovePosition = QPointF();
    contextMenuEventPos = QPointF();
    remoteActionIndex = 0;
    visibility = true;

    handle = new MTestExtensionHandle();
    connect(this, SIGNAL(connectionFromRunnerEstablished()), handle, SLOT(connectionEstablished()));
    connect(this, SIGNAL(operationComplete(QString, QString, QString)), handle, SLOT(operationComplete(QString, QString, QString)));
    connect(this, SIGNAL(operationProgress(QString, QString, int)), handle, SLOT(operationProgress(QString, QString, int)));

    listenForConnection = true;
    elapsedQTime = 0;
    gQTimerStartCalls.clear();
    gQTimerStopCalled = false;
    gQTimerStopCalls.clear();

#ifdef HAVE_CONTEXTSUBSCRIBER
    gContextPropertyValue = "";
#endif

    gStartQProcess = true;

    gMAppletSharedMutexStub->stubReset();
    gMAppletSharedMutexStub->stubSetReturnValue("init", true);
    gMAppletSharedMutexStub->stubSetReturnValue("lock", true);
    gMAppletSharedMutexStub->stubSetReturnValue("unlock", true);
    gMAppletSharedMutexStub->stubSetReturnValue("tryLock", true);
    contextMenuOpened = false;
}

void Ut_MExtensionHandle::cleanup()
{
    delete handle;
}

void Ut_MExtensionHandle::testInitializationWithCorrectData()
{
    // Use /bin/true as a runner binary that exists (should exist on every system)
    QCOMPARE(handle->state(), MExtensionHandleModel::STOPPED);
    handle->init("/bin/true", "metaDataFileName");
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), MExtensionHandleModel::RUNNING);
    QCOMPARE(handle->property("pid").toInt(), 100);

    // Check that correct arguments were passed on to the runner
    QCOMPARE(gQProcessProgram, QString("/bin/true"));
    QCOMPARE(gQProcessArguments.count(), 2);
    QVERIFY(gQProcessArguments.at(0).length() > 0);
    QCOMPARE(gQProcessArguments.at(1), QString("metaDataFileName"));

    // Check that shared mutex was created with a proper key
    QVERIFY(!gMAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0).isNull());

    // Check that a pixmap mutex was created
    QVERIFY(handle->model()->pixmapMutex() != NULL);

    // Check that no action is added when settings file is not defined
    QCOMPARE(handle->actions().count(), 0);
}

void Ut_MExtensionHandle::testReinit()
{
    handle->init("/bin/true", "metaDataFileName");
    emit connectionFromRunnerEstablished();

    // Test killing
    QRectF geom = handle->geometry();
    handle->kill();
    QVERIFY(handle->model()->pixmapMutex() == NULL);
    QCOMPARE(handle->state(), MExtensionHandleModel::STOPPED);

    // Check that a pixmap mutex was deleted
    QVERIFY(handle->model()->pixmapMutex() == NULL);

    // Test reinit after killing. This will call kill() again.
    handle->reinit();

    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), MExtensionHandleModel::RUNNING);
    QCOMPARE(handle->geometry(), geom);

    QCOMPARE(handle->property("pid").toInt(), 100);

    // Check that shared mutex was created with a proper key
    QVERIFY(!gMAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0).isNull());

    // Check that a pixmap mutex was created
    QVERIFY(handle->model()->pixmapMutex() != NULL);

    // Test reinit after reinit.
    handle->reinit();
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), MExtensionHandleModel::RUNNING);
    QCOMPARE(handle->property("pid").toInt(), 100);
}

void Ut_MExtensionHandle::testAppletCommunicationFails()
{
    // cause the connection to the applet to fail, test that the state is correct
    listenForConnection = false;
    handle->init("/bin/true", "metaDataFileName");
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);
    QCOMPARE(handle->property("pid").toInt(), 0);
}

void Ut_MExtensionHandle::testAppletMalfunction()
{
    // start a process which doesn't exit immediately
    handle->init("/bin/sleep", "metaDataFileName");
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), MExtensionHandleModel::RUNNING);

    // cause the response timer to timeout
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();

    // test that reinit has been called
    QCOMPARE(handle->state(), MExtensionHandleModel::RUNNING);

    // cause the response timer to timeout
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();

    // test that the applet is left in broken state
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);

    // test that the applet is left in broken state after it breaks immediately after restart
    handle->reinit();
    handle->aliveMessageResponseTimeout();
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);

    // test that the applet tries to reinit itself if broken after 30 seconds of startup
    handle->reinit();
    emit connectionFromRunnerEstablished();
    elapsedQTime = 30000;
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), MExtensionHandleModel::RUNNING);
}

void Ut_MExtensionHandle::testAppletRunnerFailsToStart()
{
    gStartQProcess = false;
    QCOMPARE(handle->state(), MExtensionHandleModel::STOPPED);
    handle->init("foobar", "metaDataFileName");
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);
    QCOMPARE(handle->property("pid").toInt(), 0);
}

void Ut_MExtensionHandle::testScaling()
{
    handle->init("/bin/true", "metaDataFileName");
    QVector<QSizeF> sizeHints;
    sizeHints << QSizeF(1500, 1000) << QSizeF(2500, 2000) << QSizeF(3500, 3000) << QSizeF(4500, 4000);
    MAppletUpdateGeometryMessage message(sizeHints);
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->model()->sizeHints(), sizeHints);
}

void Ut_MExtensionHandle::testMouseEventScaling()
{
    handle->model()->setScale(0.5);

    QPointF input(100, 50);
    QPointF expected = input / handle->model()->scale();

    // Send mouse events to the applethandle and test that scaling works for them as well
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(input);
    handle->sendMouseEvent(pressEvent);
    QCOMPARE(mousePressPosition, expected);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setPos(input);
    handle->sendMouseEvent(releaseEvent);
    QCOMPARE(mouseReleasePosition, expected);

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(input);
    handle->sendMouseEvent(moveEvent);
    QCOMPARE(mouseMovePosition, expected);
}

void Ut_MExtensionHandle::testAppletCommunication()
{
    handle->setAliveResponseTimeout(0);
    handle->init("/bin/true", "metaDataFileName");
    MAppletAliveMessageResponse message;
    handle->messageReceivedFromRunner(message);
    QCOMPARE(gQTimerStopCalled, true);
}

void Ut_MExtensionHandle::testRelayingMousePress()
{
    handle->init("/bin/true", "metaDataFileName");

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    event.setPos(QPointF(-3, 4));
    event.setButtons(Qt::LeftButton | Qt::RightButton);
    event.setButton(Qt::LeftButton);

    handle->sendMouseEvent(event);

    QCOMPARE(mousePressPosition, QPointF(-3, 4));
    QCOMPARE(mouseEventButtons, Qt::LeftButton | Qt::RightButton);
    QCOMPARE(mouseEventButton, Qt::LeftButton);
}

void Ut_MExtensionHandle::testRelayingMouseRelease()
{
    handle->init("/bin/true", "metaDataFileName");

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseRelease);
    event.setPos(QPointF(-3, 4));
    event.setButtons(Qt::MidButton | Qt::RightButton);
    event.setButton(Qt::MidButton);

    handle->sendMouseEvent(event);

    QCOMPARE(mouseReleasePosition, QPointF(-3, 4));
    QCOMPARE(mouseEventButtons, Qt::MidButton | Qt::RightButton);
    QCOMPARE(mouseEventButton, Qt::MidButton);
}

void Ut_MExtensionHandle::testRelayingMouseMove()
{
    handle->init("/bin/true", "metaDataFileName");

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseMove);
    event.setPos(QPointF(-3, 4));
    event.setButtons(Qt::RightButton);
    event.setButton(Qt::RightButton);

    handle->sendMouseEvent(event);

    QCOMPARE(mouseMovePosition, QPointF(-3, 4));
    QCOMPARE(mouseEventButtons, Qt::RightButton);
    QCOMPARE(mouseEventButton, Qt::RightButton);
}

void Ut_MExtensionHandle::testThatAppletBreaksIfConnectionIsNotEstablished()
{
    // Initialize applet handle
    handle->init("/bin/true", "metaDataFileName");
    QCOMPARE(handle->state(), MExtensionHandleModel::STARTING);

    // Let connection time out
    handle->connectionTimeout();

    // Verify that applet tries to restart itself
    QCOMPARE(handle->state(), MExtensionHandleModel::STARTING);

    // Connection times out again
    handle->connectionTimeout();

    // Verify that the applet is broken
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);
}

void Ut_MExtensionHandle::testVisibility()
{
    handle->setVisible(false);
    QVERIFY(!visibility);
    handle->exitDisplayEvent();
    QVERIFY(!visibility);
    handle->setVisible(true);
    QVERIFY(!visibility);
    handle->enterDisplayEvent();
    QVERIFY(visibility);
}

// Returns true if the list of actions contains an item with the specified text
bool actionsContains(const QList<QAction *>& actions, const QString &text)
{
    bool returnValue = false;

    foreach(QAction * action, actions) {
        if (action->text() == text) {
            returnValue = true;
            break;
        }
    }

    return returnValue;
}

void Ut_MExtensionHandle::testSettingRemoteActions()
{
    handle->init("/bin/true", "metaDataFileName");

    // Add one action
    handle->setRemoteActions(QStringList() << "action1");
    QList<QAction *> actions = handle->actions();
    QCOMPARE(actionsContains(actions, "action1"), true);

    // Add another action. This should replace the previous action
    handle->setRemoteActions(QStringList() << "action2");
    actions = handle->actions();
    QCOMPARE(actionsContains(actions, "action1"), false);
    QCOMPARE(actionsContains(actions, "action2"), true);

    // Make the actions list empty
    handle->setRemoteActions();
    actions = handle->actions();
    QCOMPARE(actionsContains(actions, "action1"), false);
    QCOMPARE(actionsContains(actions, "action2"), false);
}

void Ut_MExtensionHandle::testRemoteActionsInBrokenState()
{
    handle->init("/bin/true", "metaDataFileName");

    // Add one action
    handle->setRemoteActions(QStringList() << "action1");

    // Put the applet to broken state by time outting the connection twice
    handle->connectionTimeout();
    handle->connectionTimeout();

    // Check that there is no more an applet specific action
    QCOMPARE(actionsContains(handle->actions(), "action1"), false);
}

void Ut_MExtensionHandle::testContextMenuTrigger()
{
    handle->init("/bin/true", "metaDataFileName");

    QGraphicsSceneContextMenuEvent event(QEvent::ContextMenu);
    event.setPos(QPoint(-3, 4));

    // Opening a content menu in any other state than RUNNING should use the base class functionality
    handle->sendContextMenuEvent(&event);
    QVERIFY(contextMenuOpened);

    // Opening a context menu in the RUNNING state should ask the actions from the applet
    contextMenuOpened = false;
    emit connectionFromRunnerEstablished();
    handle->sendContextMenuEvent(&event);
    QCOMPARE(contextMenuEventPos, QPointF(-3, 4));
    QVERIFY(!contextMenuOpened);
}

void Ut_MExtensionHandle::testObjectMenuMessage()
{
    handle->init("/bin/true", "metaDataFileName");

    QList<QAction *> actions;
    QAction action(this);
    action.setText("action");
    actions.append(&action);

    MAppletObjectMenuMessage message(actions);

    handle->messageReceivedFromRunner(message);

    actions = handle->actions();
    QCOMPARE(actions.at(0)->text(), QString("action"));
}

void Ut_MExtensionHandle::testRemoteActionTriggered()
{
    handle->init("/bin/true", "metaDataFileName");

    // Add two actions
    handle->setRemoteActions(QStringList() << "action1" << "action2");

    // Find the newly added actions
    QAction *action1 = NULL, *action2 = NULL;
    foreach(QAction * a, handle->actions()) {
        if (a->text() == "action1") {
            action1 = a;
        } else if (a->text() == "action2") {
            action2 = a;
        }
    }

    remoteActionIndex = 100;
    action1->trigger();
    QCOMPARE(remoteActionIndex, (uint)0);
    action2->trigger();
    QCOMPARE(remoteActionIndex, (uint)1);
}

void Ut_MExtensionHandle::testPlaceHolderInitialization()
{
    handle->initPlaceHolder("package.deb");
    QCOMPARE(handle->state(), MExtensionHandleModel::INSTALLING);
    QCOMPARE(handle->model()->packageName(), QString("package.deb"));
}

void Ut_MExtensionHandle::testPlaceHolderInitializationWithError()
{
    handle->initPlaceHolder("package.deb", "error");
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);
    QCOMPARE(handle->model()->installationError(), QString("error"));
}

void Ut_MExtensionHandle::testInstallationOperationCompleteWithError()
{
    handle->initPlaceHolder("package.deb");
    handle->privateClass()->processStartTime = QTime::currentTime();
    handle->privateClass()->restartCount = 1;
    emit operationComplete("Install", handle->model()->packageName(), "com.nokia.package_manager.Error.InvalidPackage");

    // Verify that applet is broken
    QCOMPARE(handle->state(), MExtensionHandleModel::BROKEN);
}

void Ut_MExtensionHandle::testAppletInstallationProgess()
{
    handle->initPlaceHolder("package.deb");
    emit operationProgress("Install", handle->model()->packageName(), 15);

    // Verify that progress is updated in the model
    QCOMPARE(handle->model()->installationProgress(), 15);
}

void Ut_MExtensionHandle::testSetSizeHints()
{
    QVector<QSizeF> sizeHints;
    sizeHints.append(QSizeF(10, 40));
    sizeHints.append(QSizeF(20, 30));
    sizeHints.append(QSizeF(30, 20));
    sizeHints.append(QSizeF(40, 10));
    handle->setSizeHints(sizeHints);
    QCOMPARE(handle->model()->sizeHints(), sizeHints);
}

#ifdef HAVE_CONTEXTSUBSCRIBER
void Ut_MExtensionHandle::testWhenDisplayBlankedThenAliveCheckingStops()
{
    handle->init("/bin/true", "metaDataFileName");
    emit connectionFromRunnerEstablished();
    QVERIFY(qFind(gQTimerStartCalls, &handle->privateClass()->aliveTimer) != gQTimerStartCalls.end());
    QVERIFY(qFind(gQTimerStartCalls, &handle->privateClass()->communicationTimer) != gQTimerStartCalls.end());

    gContextPropertyValue = "blanked";
    gQTimerStopCalls.clear();
    handle->updateDisplayState();
    QVERIFY(qFind(gQTimerStopCalls, &handle->privateClass()->aliveTimer) != gQTimerStopCalls.end());
    QVERIFY(qFind(gQTimerStopCalls, &handle->privateClass()->communicationTimer) != gQTimerStopCalls.end());

    gContextPropertyValue = "";
    gQTimerStartCalls.clear();
    handle->updateDisplayState();
    QVERIFY(qFind(gQTimerStartCalls, &handle->privateClass()->aliveTimer) != gQTimerStartCalls.end());
    QVERIFY(qFind(gQTimerStartCalls, &handle->privateClass()->communicationTimer) != gQTimerStartCalls.end());
}
#endif


QTEST_APPLESS_MAIN(Ut_MExtensionHandle)
