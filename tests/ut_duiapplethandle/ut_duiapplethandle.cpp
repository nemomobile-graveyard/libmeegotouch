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
#include <QLocalServer>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <X11/extensions/Xdamage.h>

#include "ut_duiapplethandle.h"
#include <duiapplethandle.h>
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
uint appletSpecificActionIndex = 0;
bool Ut_DuiAppletHandle::listenForConnection = true;
QSizeF Ut_DuiAppletHandle::minSize = QSizeF();
QSizeF Ut_DuiAppletHandle::prefSize = QSizeF();
QSizeF Ut_DuiAppletHandle::maxSize = QSizeF();
QSize Ut_DuiAppletHandle::visibleSceneSize = QSize(1000, 1000);
bool Ut_DuiAppletHandle::contextMenuOpened;

// DuiWidget stubs (used by DuiAppletHandle)
DuiWidgetPrivate::DuiWidgetPrivate() : selected(false)
{
}

DuiWidgetPrivate::~DuiWidgetPrivate()
{
}

void DuiWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *)
{
    Ut_DuiAppletHandle::contextMenuOpened = true;
}

// DuiSceneManager stubs (used by DuiAppletHandle)
void DuiSceneManager::showWindow(DuiSceneWindow *, DuiSceneWindow::DeletionPolicy)
{
}

// A test applet handle implementation to allow mouse event sending
DuiTestAppletHandle::DuiTestAppletHandle() : DuiAppletHandle()
{
}

void DuiTestAppletHandle::messageReceivedFromRunner(const DuiAppletMessage &message)
{
    messageReceived(message);
}

void DuiTestAppletHandle::sendMouseEvent(QGraphicsSceneMouseEvent &event)
{
    if (event.type() == QEvent::GraphicsSceneMousePress)
        mousePressEvent(&event);
    else if (event.type() == QEvent::GraphicsSceneMouseRelease)
        mouseReleaseEvent(&event);
    else if (event.type() == QEvent::GraphicsSceneMouseMove)
        mouseMoveEvent(&event);
}

void DuiTestAppletHandle::sendContextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    contextMenuEvent(event);
}

void DuiTestAppletHandle::aliveMessageResponseTimeout()
{
    communicationTimerTimeout();
}

void DuiTestAppletHandle::connectionTimeout()
{
    communicationTimerTimeout();
}

DuiTestAppletHandlePrivate *DuiTestAppletHandle::privateClass()
{
    Q_D(DuiTestAppletHandle);

    return d;
}

// AppletCommunicator stubs to get rid of IPC and to send the size hint
bool DuiAppletServer::startServer(const QString &serverName)
{
    Q_UNUSED(serverName);
    return Ut_DuiAppletHandle::listenForConnection;
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
        visibility = (dynamic_cast<const DuiAppletVisibilityMessage &>(message)).visible();
        break;
    case DuiAppletMessage::OBJECT_MENU_ACTION_SELECTED_MESSAGE:
        appletSpecificActionIndex = (dynamic_cast<const DuiAppletObjectMenuActionSelectedMessage &>(message)).index();
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
void Ut_DuiAppletHandle::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiapplethandle" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiAppletHandle::cleanupTestCase()
{
    delete appWin;
    // this is commented out for now, to prevent crash at exit:
    // delete app;
}

void Ut_DuiAppletHandle::init()
{
    mouseEventButtons = Qt::NoButton;
    mouseEventButton = Qt::NoButton;
    mousePressPosition = QPointF();
    mouseReleasePosition = QPointF();
    mouseMovePosition = QPointF();
    contextMenuEventPos = QPointF();
    appletSpecificActionIndex = 0;
    visibility = true;

    handle = new DuiTestAppletHandle();
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
    Ut_DuiAppletHandle::minSize = QSizeF();
    Ut_DuiAppletHandle::prefSize = QSizeF();
    Ut_DuiAppletHandle::maxSize = QSizeF();
    Ut_DuiAppletHandle::visibleSceneSize = QSize(1000, 1000);
    appWin->scene()->addItem(handle);
}

void Ut_DuiAppletHandle::cleanup()
{
    delete handle;
}

void Ut_DuiAppletHandle::testInitializationWithCorrectData()
{
    // Use /bin/true as a runner binary that exists (should exist on every system)
    QCOMPARE(handle->state(), DuiAppletHandleModel::STOPPED);
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiAppletHandleModel::RUNNING);
    QCOMPARE(handle->property("pid").toInt(), 100);

    // Check that correct arguments were passed on to the runner
    QCOMPARE(gQProcessProgram, QString("/bin/true"));
    QCOMPARE(gQProcessArguments.count(), 4);
    QCOMPARE(gQProcessArguments.at(0), appletId.toString());
    QCOMPARE(gQProcessArguments.at(1), QString("appletInstanceFileDataPath"));
    QCOMPARE(gQProcessArguments.at(2), QString("metaDataFileName"));
    QVERIFY(gQProcessArguments.at(3).length() > 0);

    // Check that shared mutex was created with a proper key
    QVERIFY(!gDuiAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0).isNull());

    // Check that a pixmap mutex was created
    QVERIFY(handle->model()->pixmapMutex() != NULL);

    // Check that no action is added when settings file is not defined
    QCOMPARE(handle->actions().count(), 0);
}

void Ut_DuiAppletHandle::testReinit()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    emit connectionFromRunnerEstablished();

    // Test killing
    QRectF geom = handle->geometry();
    handle->kill();
    QVERIFY(handle->model()->pixmapMutex() == NULL);
    QCOMPARE(handle->state(), DuiAppletHandleModel::STOPPED);

    // Check that a pixmap mutex was deleted
    QVERIFY(handle->model()->pixmapMutex() == NULL);

    // Test reinit after killing. This will call kill() again.
    handle->reinit();

    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiAppletHandleModel::RUNNING);
    QCOMPARE(handle->geometry(), geom);

    QCOMPARE(handle->property("pid").toInt(), 100);

    // Check that shared mutex was created with a proper key
    QVERIFY(!gDuiAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0).isNull());

    // Check that a pixmap mutex was created
    QVERIFY(handle->model()->pixmapMutex() != NULL);

    // Test reinit after reinit.
    handle->reinit();
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiAppletHandleModel::RUNNING);
    QCOMPARE(handle->property("pid").toInt(), 100);
}

void Ut_DuiAppletHandle::testAppletCommunicationFails()
{
    // cause the connection to the applet to fail, test that the state is correct
    listenForConnection = false;
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);
    QCOMPARE(handle->property("pid").toInt(), 0);
}

void Ut_DuiAppletHandle::testAppletMalfunction()
{
    // start a process which doesn't exit immediately
    handle->init("/bin/sleep", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiAppletHandleModel::RUNNING);

    // cause the response timer to timeout
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();

    // test that reinit has been called
    QCOMPARE(handle->state(), DuiAppletHandleModel::RUNNING);

    // cause the response timer to timeout
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();

    // test that the applet is left in broken state
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);

    // test that the applet is left in broken state after it breaks immediately after restart
    handle->reinit();
    handle->aliveMessageResponseTimeout();
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);

    // test that the applet tries to reinit itself if broken after 30 seconds of startup
    handle->reinit();
    emit connectionFromRunnerEstablished();
    elapsedQTime = 30000;
    handle->aliveMessageResponseTimeout();
    emit connectionFromRunnerEstablished();
    QCOMPARE(handle->state(), DuiAppletHandleModel::RUNNING);
}

void Ut_DuiAppletHandle::testAppletRunnerFailsToStart()
{
    gStartQProcess = false;
    QCOMPARE(handle->state(), DuiAppletHandleModel::STOPPED);
    handle->init("foobar", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);
    QCOMPARE(handle->property("pid").toInt(), 0);
}

void Ut_DuiAppletHandle::testScaling()
{
    // Get the available width
    QSizeF maximumAppletSize(600, 400);
    handle->modifiableStyle()->setMaximumAppletSize(maximumAppletSize);

    // Initialize an applethandle instance and cause a 1500x1000 minimum size hint response to be sent
    // Expecting the scaling factor to be 1500 / screenWidth
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    QVector<QSizeF> sizeHints;
    sizeHints << QSizeF(1500, 1000) << QSizeF(2500, 2000) << QSizeF(3500, 3000) << QSizeF(4500, 4000);
    DuiAppletUpdateGeometryMessage message(sizeHints);
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->model()->appletScale(), maximumAppletSize.width() / 1500.0f);

    // Expecting the scaling factor to be 1
    sizeHints.replace(Qt::MinimumSize, QSizeF(maximumAppletSize.width(), 10));
    message.setSizeHints(sizeHints);
    handle->messageReceivedFromRunner(message);
    QCOMPARE(handle->model()->appletScale(), qreal(1.0f));

    // Expecting the scaling factor to be 1
    sizeHints.replace(Qt::MinimumSize, QSizeF(maximumAppletSize.width() * 0.5f, 10));
    message.setSizeHints(sizeHints);
    QCOMPARE(handle->model()->appletScale(), qreal(1.0f));
}

void Ut_DuiAppletHandle::testMouseEventScaling()
{
    // Get the width of the screen
    QSizeF maximumAppletSize(600, 400);
    handle->modifiableStyle()->setMaximumAppletSize(maximumAppletSize);

    // Send mouse events to the applethandle and test that scaling works for them as well
    QVector<QSizeF> sizeHints;
    sizeHints << QSizeF(1500, 1000) << QSizeF(2500, 2000) << QSizeF(3500, 3000) << QSizeF(4500, 4000);
    DuiAppletUpdateGeometryMessage message(sizeHints);
    handle->messageReceivedFromRunner(message);
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setPos(QPointF(100, 50));
    handle->sendMouseEvent(pressEvent);
    QCOMPARE(mousePressPosition, QPointF(100 * 1500 / maximumAppletSize.width(), 50 * 1500 / maximumAppletSize.width()));

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setPos(QPointF(100, 50));
    handle->sendMouseEvent(releaseEvent);
    QCOMPARE(mouseReleasePosition, QPointF(100 * 1500 / maximumAppletSize.width(), 50 * 1500 / maximumAppletSize.width()));

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setPos(QPointF(100, 50));
    handle->sendMouseEvent(moveEvent);
    QCOMPARE(mouseMovePosition, QPointF(100 * 1500 / maximumAppletSize.width(), 50 * 1500 / maximumAppletSize.width()));
}

void Ut_DuiAppletHandle::testAppletCommunication()
{
    handle->setAliveResponseTimeout(0);
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    DuiAppletAliveMessageResponse message;
    handle->messageReceivedFromRunner(message);
    QCOMPARE(timerCalled, true);
}

void Ut_DuiAppletHandle::testRelayingMousePress()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMousePress);
    event.setPos(QPointF(-3, 4));
    event.setButtons(Qt::LeftButton | Qt::RightButton);
    event.setButton(Qt::LeftButton);

    handle->sendMouseEvent(event);

    QCOMPARE(mousePressPosition, QPointF(-3, 4));
    QCOMPARE(mouseEventButtons, Qt::LeftButton | Qt::RightButton);
    QCOMPARE(mouseEventButton, Qt::LeftButton);
}

void Ut_DuiAppletHandle::testRelayingMouseRelease()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseRelease);
    event.setPos(QPointF(-3, 4));
    event.setButtons(Qt::MidButton | Qt::RightButton);
    event.setButton(Qt::MidButton);

    handle->sendMouseEvent(event);

    QCOMPARE(mouseReleasePosition, QPointF(-3, 4));
    QCOMPARE(mouseEventButtons, Qt::MidButton | Qt::RightButton);
    QCOMPARE(mouseEventButton, Qt::MidButton);
}

void Ut_DuiAppletHandle::testRelayingMouseMove()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    QGraphicsSceneMouseEvent event(QEvent::GraphicsSceneMouseMove);
    event.setPos(QPointF(-3, 4));
    event.setButtons(Qt::RightButton);
    event.setButton(Qt::RightButton);

    handle->sendMouseEvent(event);

    QCOMPARE(mouseMovePosition, QPointF(-3, 4));
    QCOMPARE(mouseEventButtons, Qt::RightButton);
    QCOMPARE(mouseEventButton, Qt::RightButton);
}

void Ut_DuiAppletHandle::testThatAppletBreaksIfConnectionIsNotEstablished()
{
    // Initialize applet handle
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);
    QCOMPARE(handle->state(), DuiAppletHandleModel::STARTING);

    // Let connection time out
    handle->connectionTimeout();

    // Verify that applet tries to restart itself
    QCOMPARE(handle->state(), DuiAppletHandleModel::STARTING);

    // Connection times out again
    handle->connectionTimeout();

    // Verify that the applet is broken
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);
}

void Ut_DuiAppletHandle::testVisibility()
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

void Ut_DuiAppletHandle::testSettingAppletSpecificActions()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    // Add one action
    handle->setAppletSpecificActions(QStringList() << "action1");
    QList<QAction *> actions = handle->actions();
    QCOMPARE(actionsContains(actions, "action1"), true);

    // Add another action. This should replace the previous action
    handle->setAppletSpecificActions(QStringList() << "action2");
    actions = handle->actions();
    QCOMPARE(actionsContains(actions, "action1"), false);
    QCOMPARE(actionsContains(actions, "action2"), true);

    // Make the actions list empty
    handle->setAppletSpecificActions();
    actions = handle->actions();
    QCOMPARE(actionsContains(actions, "action1"), false);
    QCOMPARE(actionsContains(actions, "action2"), false);
}

void Ut_DuiAppletHandle::testAppletSpecificActionsInBrokenState()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    // Add one action
    handle->setAppletSpecificActions(QStringList() << "action1");

    // Put the applet to broken state by time outting the connection twice
    handle->connectionTimeout();
    handle->connectionTimeout();

    // Check that there is no more an applet specific action
    QCOMPARE(actionsContains(handle->actions(), "action1"), false);
}

void Ut_DuiAppletHandle::testContextMenuTrigger()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

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

void Ut_DuiAppletHandle::testObjectMenuMessage()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    QList<QAction *> actions;
    QAction action(this);
    action.setText("action");
    actions.append(&action);

    DuiAppletObjectMenuMessage message(actions);

    handle->messageReceivedFromRunner(message);

    actions = handle->actions();
    QCOMPARE(actions.at(0)->text(), QString("action"));
}

void Ut_DuiAppletHandle::testAppletSpecificActionTriggered()
{
    handle->init("/bin/true", "appletInstanceFileDataPath", "metaDataFileName", appletId);

    // Add two actions
    handle->setAppletSpecificActions(QStringList() << "action1" << "action2");

    // Find the newly added actions
    QAction *action1 = NULL, *action2 = NULL;
    foreach(QAction * a, handle->actions()) {
        if (a->text() == "action1") {
            action1 = a;
        } else if (a->text() == "action2") {
            action2 = a;
        }
    }

    appletSpecificActionIndex = 100;
    action1->trigger();
    QCOMPARE(appletSpecificActionIndex, (uint)0);
    action2->trigger();
    QCOMPARE(appletSpecificActionIndex, (uint)1);
}

void Ut_DuiAppletHandle::testPlaceHolderInitialization()
{
    handle->initPlaceHolder(appletId, "package.deb");
    QCOMPARE(handle->state(), DuiAppletHandleModel::INSTALLING);
    QCOMPARE(handle->model()->packageName(), QString("package.deb"));
}

void Ut_DuiAppletHandle::testPlaceHolderInitializationWithError()
{
    handle->initPlaceHolder(appletId, "package.deb", "error");
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);
    QCOMPARE(handle->model()->installationError(), QString("error"));
}

void Ut_DuiAppletHandle::testInstallationOperationCompleteWithError()
{
    handle->initPlaceHolder(appletId, "package.deb");
    handle->privateClass()->processStartTime = QTime::currentTime();
    handle->privateClass()->restartCount = 1;
    emit operationComplete("Install", handle->model()->packageName(), "com.nokia.package_manager.Error.InvalidPackage");

    // Verify that applet is broken
    QCOMPARE(handle->state(), DuiAppletHandleModel::BROKEN);
}

void Ut_DuiAppletHandle::testAppletInstallationProgess()
{
    handle->initPlaceHolder(appletId, "package.deb");
    emit operationProgress("Install", handle->model()->packageName(), 15);

    // Verify that progress is updated in the model
    QCOMPARE(handle->model()->installationProgress(), 15);
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

void Ut_DuiAppletHandle::testSetSizeHints()
{
    QVector<QSizeF> sizeHints;
    sizeHints.append(QSizeF(10, 40));
    sizeHints.append(QSizeF(20, 30));
    sizeHints.append(QSizeF(30, 20));
    sizeHints.append(QSizeF(40, 10));
    handle->setSizeHints(sizeHints);
    QCOMPARE(handle->model()->sizeHints(), sizeHints);
}

QTEST_APPLESS_MAIN(Ut_DuiAppletHandle)
