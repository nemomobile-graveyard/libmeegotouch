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
#include <QtGui>
#include <QLocalServer>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <X11/extensions/Xdamage.h>

#include "ut_duiextensionhandle.h"
#include <duiextensionhandle.h>
#include <duiappletserver.h>
#include "duiappletsetgeometrymessage.h"
#include "duiappletmousemessage.h"
#include "duiappletalivemessageresponse.h"
#include "duiappletpixmaptakenintousemessage.h"
#include "duiappletupdategeometrymessage.h"
#include "duiappletvisibilitymessage.h"
#include "duiappletobjectmenuactionselectedmessage.h"
#include "duiappleticonchangedmessage.h"
#include "duiapplettitlechangedmessage.h"
#include "duiapplettextchangedmessage.h"
#include <duiapplication.h>
#include <duiapplicationwindow.h>
#include <duiscene.h>
#include <duilocale.h>
#include "duisettingslanguageparser.h"
#include "duiappletsharedmutex_stub.h"
#include "duiappletobjectmenurequestmessage.h"
#include "duiappletobjectmenumessage.h"
#include "duiappletobjectmenuactionselectedmessage.h"
#include "duiobjectmenu_stub.h"
#include <QList>
#include <QAction>
#include <duiappletid_stub.h>
#include <duiscenemanager.h>

DuiApplication *app;
DuiApplicationWindow *appWin;

bool visibility;
QPointF mousePressPosition;
QPointF mouseReleasePosition;
QPointF mouseMovePosition;
QPointF contextMenuEventPos;
Qt::MouseButtons mouseEventButtons;
Qt::MouseButton mouseEventButton;
uint remoteActionIndex = 0;
bool Ut_DuiExtensionHandle::listenForConnection = true;
QSizeF Ut_DuiExtensionHandle::minSize = QSizeF();
QSizeF Ut_DuiExtensionHandle::prefSize = QSizeF();
QSizeF Ut_DuiExtensionHandle::maxSize = QSizeF();
QSize Ut_DuiExtensionHandle::visibleSceneSize = QSize(1000, 1000);
bool Ut_DuiExtensionHandle::contextMenuOpened;

// DuiWidget stubs (used by DuiExtensionHandle)
DuiWidgetPrivate::DuiWidgetPrivate() : selected(false)
{
}
DuiWidgetPrivate::~DuiWidgetPrivate()
{
}

void DuiWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *)
{
    Ut_DuiExtensionHandle::contextMenuOpened = true;
}

// DuiSceneManager stubs (used by DuiExtensionHandle)
void DuiSceneManager::appearSceneWindow(DuiSceneWindow *, DuiSceneWindow::DeletionPolicy)
{
}

// A test handle implementation to allow mouse event sending
DuiTestExtensionHandle::DuiTestExtensionHandle() : DuiExtensionHandle()
{
}

void DuiTestExtensionHandle::messageReceivedFromRunner(const DuiAppletMessage &message)
{
    messageReceived(message);
}

void DuiTestExtensionHandle::sendMouseEvent(QGraphicsSceneMouseEvent &event)
{
    if (event.type() == QEvent::GraphicsSceneMousePress)
        mousePressEvent(&event);
    else if (event.type() == QEvent::GraphicsSceneMouseRelease)
        mouseReleaseEvent(&event);
    else if (event.type() == QEvent::GraphicsSceneMouseMove)
        mouseMoveEvent(&event);
}

void DuiTestExtensionHandle::sendContextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    contextMenuEvent(event);
}

void DuiTestExtensionHandle::aliveMessageResponseTimeout()
{
    communicationTimerTimeout();
}

void DuiTestExtensionHandle::connectionTimeout()
{
    communicationTimerTimeout();
}

DuiTestExtensionHandlePrivate *DuiTestExtensionHandle::privateClass()
{
    Q_D(DuiTestExtensionHandle);

    return d;
}

// AppletCommunicator stubs to get rid of IPC and to send the size hint
bool DuiAppletServer::startServer(const QString &serverName)
{
    Q_UNUSED(serverName);
    return Ut_DuiExtensionHandle::listenForConnection;
}

// Store information from the different message types sent by the handle
bool DuiAppletCommunicator::sendMessage(const DuiAppletMessage &message)
{
    const DuiAppletMouseMessage *mouseMessage = 0;
    switch (message.type()) {
    case DuiAppletMessage::MOUSE_PRESS_MESSAGE:
    case DuiAppletMessage::MOUSE_RELEASE_MESSAGE:
    case DuiAppletMessage::MOUSE_MOVE_MESSAGE:
        mouseMessage = dynamic_cast<const DuiAppletMouseMessage *>(&message);
        mouseEventButton = mouseMessage->button();
        mouseEventButtons = mouseMessage->buttons();
        if (message.type() == DuiAppletMessage::MOUSE_PRESS_MESSAGE)
            mousePressPosition = (dynamic_cast<const DuiAppletMouseMessage &>(message)).position();
        else if (message.type() == DuiAppletMessage::MOUSE_RELEASE_MESSAGE)
            mouseReleasePosition = (dynamic_cast<const DuiAppletMouseMessage &>(message)).position();
        else if (message.type() == DuiAppletMessage::MOUSE_MOVE_MESSAGE)
            mouseMovePosition = (dynamic_cast<const DuiAppletMouseMessage &>(message)).position();
        break;
    case DuiAppletMessage::OBJECT_MENU_REQUEST_MESSAGE:
        contextMenuEventPos = (dynamic_cast<const DuiAppletObjectMenuRequestMessage &>(message)).pos();
        break;
    case DuiAppletMessage::VISIBILITY_MESSAGE:
        visibility = (dynamic_cast<const DuiAppletVisibilityMessage &>(message)).isVisible();
        break;
    case DuiAppletMessage::OBJECT_MENU_ACTION_SELECTED_MESSAGE:
        remoteActionIndex = (dynamic_cast<const DuiAppletObjectMenuActionSelectedMessage &>(message)).index();
        break;
    default:
        break;
    }

    return true;
}

// QLocalServer stubs
bool QLocalServer::listen(const QString &name)
{
    Q_UNUSED(name);
    return true;
}

bool QLocalServer::waitForNewConnection(int msec, bool *timedOut)
{
    Q_UNUSED(msec);
    Q_UNUSED(timedOut);
    return true;
}

// QTimer stubs
bool timerCalled = false;

void QTimer::stop()
{
    timerCalled = true;
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

// DuiSettingsLanguageParser stubs
DuiSettingsLanguageBinary *DuiSettingsLanguageParser::createSettingsBinary()
{
    return new DuiSettingsLanguageBinary;
}

// Unit test cases
void Ut_DuiExtensionHandle::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiextensionhandle" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiExtensionHandle::cleanupTestCase()
{
    delete appWin;
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_DuiExtensionHandle::init()
{
    mouseEventButtons = Qt::NoButton;
    mouseEventButton = Qt::NoButton;
    mousePressPosition = QPointF();
    mouseReleasePosition = QPointF();
    mouseMovePosition = QPointF();
    contextMenuEventPos = QPointF();
    remoteActionIndex = 0;
    visibility = true;

    handle = new DuiTestExtensionHandle();
    handle->setViewType("default");
    connect(this, SIGNAL(connectionFromRunnerEstablished()), handle, SLOT(connectionEstablished()));
    connect(this, SIGNAL(widgetVisibilityChanged(bool)), handle, SLOT(visibilityEvent(bool)));
    connect(this, SIGNAL(applicationVisibilityChanged(bool)), handle, SLOT(applicationVisibilityChanged(bool)));
    connect(this, SIGNAL(operationComplete(QString, QString, QString)), handle->privateClass(), SLOT(operationComplete(QString, QString, QString)));
    connect(this, SIGNAL(operationProgress(QString, QString, int)), handle->privateClass(), SLOT(operationProgress(QString, QString, int)));
    listenForConnection = true;
    elapsedQTime = 0;
    timerCalled = false;

    gStartQProcess = true;

    minSize = QSizeF();
    prefSize = QSizeF();
    maxSize = QSizeF();
    visibleSceneSize = QSize(1000, 1000);
    gDuiAppletSharedMutexStub->stubReset();
    gDuiAppletSharedMutexStub->stubSetReturnValue("init", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("lock", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("unlock", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("tryLock", true);
    contextMenuOpened = false;
    Ut_DuiExtensionHandle::minSize = QSizeF();
    Ut_DuiExtensionHandle::prefSize = QSizeF();
    Ut_DuiExtensionHandle::maxSize = QSizeF();
    Ut_DuiExtensionHandle::visibleSceneSize = QSize(1000, 1000);
    appWin->scene()->addItem(handle);
}

void Ut_DuiExtensionHandle::cleanup()
{
    delete handle;
}

void Ut_DuiExtensionHandle::testInitializationWithCorrectData()
{
    // Use /bin/true as a runner binary that exists (should exist on every system)
    QCOMPARE(handle->state(), DuiExtensionHandleModel::STOPPED);
    handle->init("/bin/true", "metaDataFileName");
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiExtensionHandleModel::RUNNING);
    QCOMPARE(handle->property("pid").toInt(), 100);

    // Check that correct arguments were passed on to the runner
    QCOMPARE(gQProcessProgram, QString("/bin/true"));
    QCOMPARE(gQProcessArguments.count(), 2);
    QVERIFY(gQProcessArguments.at(0).length() > 0);
    QCOMPARE(gQProcessArguments.at(1), QString("metaDataFileName"));

    // Check that shared mutex was created with a proper key
    QVERIFY(!gDuiAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0).isNull());

    // Check that a pixmap mutex was created
    QVERIFY(handle->model()->pixmapMutex() != NULL);

    // Check that no action is added when settings file is not defined
    QCOMPARE(handle->actions().count(), 0);
}

void Ut_DuiExtensionHandle::testReinit()
{
    handle->init("/bin/true", "metaDataFileName");
    emit connectionFromRunnerEstablished();

    // Test killing
    QRectF geom = handle->geometry();
    handle->kill();
    QVERIFY(handle->model()->pixmapMutex() == NULL);
    QCOMPARE(handle->state(), DuiExtensionHandleModel::STOPPED);

    // Check that a pixmap mutex was deleted
    QVERIFY(handle->model()->pixmapMutex() == NULL);

    // Test reinit after killing. This will call kill() again.
    handle->reinit();

    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiExtensionHandleModel::RUNNING);
    QCOMPARE(handle->geometry(), geom);

    QCOMPARE(handle->property("pid").toInt(), 100);

    // Check that shared mutex was created with a proper key
    QVERIFY(!gDuiAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0).isNull());

    // Check that a pixmap mutex was created
    QVERIFY(handle->model()->pixmapMutex() != NULL);

    // Test reinit after reinit.
    handle->reinit();
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiExtensionHandleModel::RUNNING);
    QCOMPARE(handle->property("pid").toInt(), 100);
}

void Ut_DuiExtensionHandle::testAppletCommunicationFails()
{
    // cause the connection to the applet to fail, test that the state is correct
    listenForConnection = false;
    handle->init("/bin/true", "metaDataFileName");
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);
    QCOMPARE(handle->property("pid").toInt(), 0);
}

void Ut_DuiExtensionHandle::testAppletMalfunction()
{
    // start a process which doesn't exit immediately
    handle->init("/bin/sleep", "metaDataFileName");
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiExtensionHandleModel::RUNNING);

    // cause the response timer to timeout
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();

    // test that reinit has been called
    QCOMPARE(handle->state(), DuiExtensionHandleModel::RUNNING);

    // cause the response timer to timeout
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();

    // test that the applet is left in broken state
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);

    // test that the applet is left in broken state after it breaks immediately after restart
    handle->reinit();
    handle->aliveMessageResponseTimeout();
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);

    // test that the applet tries to reinit itself if broken after 30 seconds of startup
    handle->reinit();
    emit connectionFromRunnerEstablished();
    elapsedQTime = 30000;
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiExtensionHandleModel::RUNNING);
}

void Ut_DuiExtensionHandle::testAppletRunnerFailsToStart()
{
    gStartQProcess = false;
    QCOMPARE(handle->state(), DuiExtensionHandleModel::STOPPED);
    handle->init("foobar", "metaDataFileName");
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);
    QCOMPARE(handle->property("pid").toInt(), 0);
}

void Ut_DuiExtensionHandle::testScaling()
{
    handle->init("/bin/true", "metaDataFileName");
    QVector<QSizeF> sizeHints;
    sizeHints << QSizeF(1500, 1000) << QSizeF(2500, 2000) << QSizeF(3500, 3000) << QSizeF(4500, 4000);
    DuiAppletUpdateGeometryMessage message(sizeHints);
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->model()->sizeHints(), sizeHints);
}

void Ut_DuiExtensionHandle::testMouseEventScaling()
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

void Ut_DuiExtensionHandle::testAppletCommunication()
{
    handle->setAliveResponseTimeout(0);
    handle->init("/bin/true", "metaDataFileName");
    DuiAppletAliveMessageResponse message;
    handle->messageReceivedFromRunner(message);
    QCOMPARE(timerCalled, true);
}

void Ut_DuiExtensionHandle::testRelayingMousePress()
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

void Ut_DuiExtensionHandle::testRelayingMouseRelease()
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

void Ut_DuiExtensionHandle::testRelayingMouseMove()
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

void Ut_DuiExtensionHandle::testThatAppletBreaksIfConnectionIsNotEstablished()
{
    // Initialize applet handle
    handle->init("/bin/true", "metaDataFileName");
    QCOMPARE(handle->state(), DuiExtensionHandleModel::STARTING);

    // Let connection time out
    handle->connectionTimeout();

    // Verify that applet tries to restart itself
    QCOMPARE(handle->state(), DuiExtensionHandleModel::STARTING);

    // Connection times out again
    handle->connectionTimeout();

    // Verify that the applet is broken
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);
}

void Ut_DuiExtensionHandle::testVisibility()
{
    // Visibility should be true only if both the widget and the application are visible
    emit widgetVisibilityChanged(false);
    QVERIFY(!visibility);
    handle->exitDisplayEvent();
    QVERIFY(!visibility);
    emit widgetVisibilityChanged(true);
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

void Ut_DuiExtensionHandle::testSettingRemoteActions()
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

void Ut_DuiExtensionHandle::testRemoteActionsInBrokenState()
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

void Ut_DuiExtensionHandle::testContextMenuTrigger()
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

void Ut_DuiExtensionHandle::testObjectMenuMessage()
{
    handle->init("/bin/true", "metaDataFileName");

    QList<QAction *> actions;
    QAction action(this);
    action.setText("action");
    actions.append(&action);

    DuiAppletObjectMenuMessage message(actions);

    handle->messageReceivedFromRunner(message);

    actions = handle->actions();
    QCOMPARE(actions.at(0)->text(), QString("action"));
}

void Ut_DuiExtensionHandle::testRemoteActionTriggered()
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

void Ut_DuiExtensionHandle::testPlaceHolderInitialization()
{
    handle->initPlaceHolder("package.deb");
    QCOMPARE(handle->state(), DuiExtensionHandleModel::INSTALLING);
    QCOMPARE(handle->model()->packageName(), QString("package.deb"));
}

void Ut_DuiExtensionHandle::testPlaceHolderInitializationWithError()
{
    handle->initPlaceHolder("package.deb", "error");
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);
    QCOMPARE(handle->model()->installationError(), QString("error"));
}

void Ut_DuiExtensionHandle::testInstallationOperationCompleteWithError()
{
    handle->initPlaceHolder("package.deb");
    handle->privateClass()->processStartTime = QTime::currentTime();
    handle->privateClass()->restartCount = 1;
    emit operationComplete("Install", handle->model()->packageName(), "com.nokia.package_manager.Error.InvalidPackage");

    // Verify that applet is broken
    QCOMPARE(handle->state(), DuiExtensionHandleModel::BROKEN);
}

void Ut_DuiExtensionHandle::testAppletInstallationProgess()
{
    handle->initPlaceHolder("package.deb");
    emit operationProgress("Install", handle->model()->packageName(), 15);

    // Verify that progress is updated in the model
    QCOMPARE(handle->model()->installationProgress(), 15);
}

void Ut_DuiExtensionHandle::testSetSizeHints()
{
    QVector<QSizeF> sizeHints;
    sizeHints.append(QSizeF(10, 40));
    sizeHints.append(QSizeF(20, 30));
    sizeHints.append(QSizeF(30, 20));
    sizeHints.append(QSizeF(40, 10));
    handle->setSizeHints(sizeHints);
    QCOMPARE(handle->model()->sizeHints(), sizeHints);
}

QTEST_APPLESS_MAIN(Ut_DuiExtensionHandle)
