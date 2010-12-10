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

#include "ut_mextensionrunner.h"
#include <mextensionrunner.h>
#include <mappletclient.h>
#include <mappletmetadata.h>
#include <mappletmousemessage.h>
#include <mappletvisibilitymessage.h>
#include <mappletorientationmessage.h>
#include <mappletsetgeometrymessage.h>
#include <mappletalivemessagerequest.h>
#include <mappletpixmapmodifiedmessage.h>
#include <mappletupdategeometrymessage.h>
#include <mappletobjectmenurequestmessage.h>
#include <mappletobjectmenumessage.h>
#include <mcomponentdata.h>
#include <mappletsharedmutex_stub.h>
#include <mappletid_stub.h>
#include <mappletsettings_stub.h>
#include <mgconfdatastore.h>
#include <MAction>
#include <MWidget>
#include <MApplication>
#include <QtTest/QtTest>
#include <QGraphicsLinearLayout>

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

// ContextProperty stubs
#ifdef HAVE_CONTEXTSUBSCRIBER
QString gContextPropertyValue = "";
QVariant ContextProperty::value() const
{
    return gContextPropertyValue;
}
#endif

// MWindow stubs (to prevent crashing)
MWindow::MWindow(QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(0)
{
}
MWindow::~MWindow()
{
}

// QTimer stubs (used by MExtensionRunner)
QList<QTimer*> gQTimerStartCalls;
void QTimer::start()
{
    gQTimerStartCalls.append(this);
    id = 1;
    if (Ut_MExtensionRunner::timerImmediateTimeout) {
        emit timeout();
    }
}

void QTimer::start(int)
{
    start();
}

QList<QTimer*> gQTimerStopCalls;
void QTimer::stop()
{
    gQTimerStopCalls.append(this);
    id = -1;
}

// QCoreApplication stubs (used by MExtensionRunner)
void QCoreApplication::quit()
{
    Ut_MExtensionRunner::quitCalled = true;
}

// MComponentData stubs
bool MComponentData::emulateTwoFingerGestures()
{
    return false;
}

// QGraphicsScene stubs
bool sceneRenderCalled;
void QGraphicsScene::render(QPainter *, const QRectF &, const QRectF &, Qt::AspectRatioMode)
{
    sceneRenderCalled = true;
}


MouseEventFilter::~MouseEventFilter()
{
    foreach(QGraphicsSceneMouseEvent * e, events) {
        delete e;
    }
}

bool MouseEventFilter::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o);

    if (e->type() == QEvent::GraphicsSceneMousePress || e->type() == QEvent::GraphicsSceneMouseRelease || e->type() == QEvent::GraphicsSceneMouseMove) {
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(e);
        QGraphicsSceneMouseEvent *storedEvent = new QGraphicsSceneMouseEvent(e->type());
        storedEvent->setButton(mouseEvent->button());
        storedEvent->setButtons(mouseEvent->buttons());
        events.append(storedEvent);
    }

    return false;
}

bool MAppletCommunicator_connectToProcessReturn = true;
bool MAppletClient::connectToServer(const QString &serverName)
{
    Q_UNUSED(serverName);
    return MAppletCommunicator_connectToProcessReturn;
}
MAppletMessage::MAppletMessageType MAppletCommunicator_sentMessageType;
QRectF MAppletCommunicator_sentPixmapModifiedMessageGeometry;
QList<QString> MAppletCommunicator_sentObjectMenuActionList;
QVector<QSizeF> MAppletCommunicator_sentUpdateGeometryMessageSizeHints;
bool MAppletCommunicator::sendMessage(const MAppletMessage &message)
{
    MAppletCommunicator_sentMessageType = message.type();
    if (MAppletCommunicator_sentMessageType == MAppletMessage::PixmapModifiedMessage) {
        const MAppletPixmapModifiedMessage pixmapModifiedMessage = dynamic_cast<const MAppletPixmapModifiedMessage &>(message);
        MAppletCommunicator_sentPixmapModifiedMessageGeometry = pixmapModifiedMessage.geometry();
    } else if (MAppletCommunicator_sentMessageType == MAppletMessage::UpdateGeometryMessage) {
        const MAppletUpdateGeometryMessage updateGeometryMessage = dynamic_cast<const MAppletUpdateGeometryMessage &>(message);
        MAppletCommunicator_sentUpdateGeometryMessageSizeHints = updateGeometryMessage.sizeHints();
    } else if (MAppletCommunicator_sentMessageType == MAppletMessage::ObjectMenuMessage) {
        const MAppletObjectMenuMessage objectMenuMessage = dynamic_cast<const MAppletObjectMenuMessage &>(message);
        MAppletCommunicator_sentObjectMenuActionList = objectMenuMessage.actionList();
    }
    return true;
}

bool MAppletMetaData_isValidReturn = true;
bool MAppletMetaData::isValid() const
{
    return MAppletMetaData_isValidReturn;
}

bool Ut_MExtensionRunner::timerImmediateTimeout;
bool Ut_MExtensionRunner::quitCalled;

// Unit test cases
void Ut_MExtensionRunner::init()
{
    m_instance = new MExtensionRunner();
    connect(this, SIGNAL(sendMessage(MAppletMessage)), m_instance, SLOT(messageReceived(MAppletMessage)));

    timerImmediateTimeout = false;
    gQTimerStartCalls.clear();
    gQTimerStopCalls.clear();
    quitCalled = false;
    gMAppletSharedMutexStub->stubReset();
    gMAppletSharedMutexStub->stubSetReturnValue("init", true);
    gMAppletSharedMutexStub->stubSetReturnValue("lock", true);
    gMAppletSharedMutexStub->stubSetReturnValue("unlock", true);
    gMAppletSharedMutexStub->stubSetReturnValue("tryLock", true);
    MAppletCommunicator_connectToProcessReturn = true;
    MAppletCommunicator_sentMessageType = MAppletMessage::InvalidMessage;
    MAppletCommunicator_sentPixmapModifiedMessageGeometry = QRectF();
    MAppletCommunicator_sentUpdateGeometryMessageSizeHints = QVector<QSizeF>(Qt::NSizeHints);
    sceneRenderCalled = false;
    gMAppletSettingsStub->stubReset();

#ifdef HAVE_CONTEXTSUBSCRIBER
    gContextPropertyValue = "";
#endif
}

void Ut_MExtensionRunner::cleanup()
{
    delete m_instance;
    m_instance = NULL;
}

void Ut_MExtensionRunner::initTestCase()
{
}

void Ut_MExtensionRunner::cleanupTestCase()
{
}

void Ut_MExtensionRunner::testInitializationSuccess()
{
    QCOMPARE(m_instance->init("servername"), true);
    QCOMPARE(gMAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0), QString("servername"));
}

void Ut_MExtensionRunner::testInitializationFailCommunicationFails()
{
    MAppletCommunicator_connectToProcessReturn = false;
    QCOMPARE(m_instance->init("servername"), false);
}

void Ut_MExtensionRunner::testInitializationFailSharedMemoryAttachFails()
{
    gMAppletSharedMutexStub->stubSetReturnValue("init", false);
    QCOMPARE(m_instance->init("servername"), false);
}

void Ut_MExtensionRunner::testRelayingMouseEvents()
{
    m_instance->init("servername");

    // Install event filter
    MouseEventFilter filter;
    QCoreApplication::instance()->installEventFilter(&filter);

    // Emit mouse press message
    MAppletMouseMessage message1(MAppletMessage::MousePressMessage, QPointF(-5, 2), Qt::RightButton, Qt::RightButton | Qt::MidButton);
    emit sendMessage(message1);
    QCOMPARE(filter.events.count(), 1);
    QCOMPARE(filter.events.at(0)->type(), QEvent::GraphicsSceneMousePress);
    QCOMPARE(filter.events.at(0)->button(), Qt::RightButton);
    QCOMPARE(filter.events.at(0)->buttons(), Qt::RightButton | Qt::MidButton);

    // Emit mouse release message
    MAppletMouseMessage message2(MAppletMessage::MouseReleaseMessage, QPointF(0, 4), Qt::LeftButton, Qt::LeftButton | Qt::MidButton);
    emit sendMessage(message2);
    QCOMPARE(filter.events.count(), 2);
    QCOMPARE(filter.events.at(1)->type(), QEvent::GraphicsSceneMouseRelease);
    QCOMPARE(filter.events.at(1)->button(), Qt::LeftButton);
    QCOMPARE(filter.events.at(1)->buttons(), Qt::LeftButton | Qt::MidButton);

    // Emit mouse move message
    MAppletMouseMessage message3(MAppletMessage::MouseMoveMessage, QPointF(-5, 2), Qt::MidButton, Qt::MidButton);
    emit sendMessage(message3);
    QCOMPARE(filter.events.count(), 3);
    QCOMPARE(filter.events.at(2)->type(), QEvent::GraphicsSceneMouseMove);
    QCOMPARE(filter.events.at(2)->button(), Qt::MidButton);
    QCOMPARE(filter.events.at(2)->buttons(), Qt::MidButton);

    // Remove event filter
    QCoreApplication::instance()->removeEventFilter(&filter);
}

void Ut_MExtensionRunner::testAppletRunnerQuitsIfMessageReceivingTimeouts()
{
    m_instance->init("servername");

    // Verify that the quit is not always called
    MAppletMouseMessage message1(MAppletMessage::MousePressMessage, QPointF(-5, 2), Qt::RightButton, Qt::RightButton | Qt::MidButton);
    emit sendMessage(message1);
    QVERIFY(!quitCalled);

    // Verify that quit is called when the timer times out
    timerImmediateTimeout = true;
    emit sendMessage(message1);
    QVERIFY(quitCalled);
}

// This test is here to test to nothing happens. Not segfaults or anything. That's why it doesn't really compare anything
void Ut_MExtensionRunner::testMessageReceivedBeforeAppletInit()
{
    {
        // Mouse messages are more or less the same so it's enough to test only one
        MAppletMouseMessage msg(MAppletMessage::MousePressMessage, QPointF(-5, 2), Qt::RightButton, Qt::RightButton | Qt::MidButton);
        emit sendMessage(msg);
    }

    {
        MAppletVisibilityMessage msg(true);
        emit sendMessage(msg);
    }

    {
        MAppletOrientationMessage msg;
        emit sendMessage(msg);
    }

    {
        MAppletSetGeometryMessage msg;
        emit sendMessage(msg);
    }

    {
        MAppletAliveMessageRequest msg;
        emit sendMessage(msg);
    }
}

void Ut_MExtensionRunner::testPaintingOnPixmap()
{
    connect(this, SIGNAL(changeScene(QList<QRectF>)), m_instance, SLOT(sceneChanged(QList<QRectF>)));

    m_instance->init("servername");

    // Create target pixmap and fill it with full opaque white
    QRectF targetRect(0, 0, 150, 100);
#ifdef Q_WS_X11
    // create a 32bit pixmap with enabled alpha channel
    Pixmap pixmap = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(), targetRect.width(), targetRect.height(), 32);
    QApplication::syncX();
    QPixmap targetPixmap = QPixmap::fromX11Pixmap(pixmap, QPixmap::ExplicitlyShared);
#else
    QPixmap targetPixmap(targetRect.width(), targetRect.height());
    // Painting with transparent will activate the alpha channel in the pixmap
    targetPixmap.fill(Qt::transparent);
#endif
    targetPixmap.fill(Qt::white);

    // Assign the pixmap to the applet runner to be painted to.
    MAppletSetGeometryMessage msg(targetRect, targetPixmap.handle());
    emit sendMessage(msg);

    // Update subregion of the pixmap
    QList<QRectF> region;
    region.append(QRectF(25, 26, 30, 25));
    emit changeScene(region);

    // Verify that subregion was cleared to full transparent
    QImage result = targetPixmap.toImage();
    QVERIFY(!result.isNull());
    QCOMPARE(result.pixel(QPoint(25, 26)), qRgba(0, 0, 0, 0));
    QCOMPARE(result.pixel(QPoint(54, 26)), qRgba(0, 0, 0, 0));
    QCOMPARE(result.pixel(QPoint(54, 50)), qRgba(0, 0, 0, 0));
    QCOMPARE(result.pixel(QPoint(25, 50)), qRgba(0, 0, 0, 0));
    QCOMPARE(result.pixel(QPoint(40, 38)), qRgba(0, 0, 0, 0));

    // Verify that the rest of the pixmap is intact
    QCOMPARE(result.pixel(QPoint(0, 0)), qRgba(255, 255, 255, 255));
    QCOMPARE(result.pixel(QPoint(24, 25)), qRgba(255, 255, 255, 255));
    QCOMPARE(result.pixel(QPoint(24, 50)), qRgba(255, 255, 255, 255));
    QCOMPARE(result.pixel(QPoint(75, 75)), qRgba(255, 255, 255, 255));
    QCOMPARE(result.pixel(QPoint(40, 70)), qRgba(255, 255, 255, 255));
}

void Ut_MExtensionRunner::testSendUpdateGeometryMessage()
{
    m_instance->init("servername");
    m_instance->widget = new MWidget;
    m_instance->widget->setMinimumSize(QSizeF(50, 50));
    m_instance->widget->setPreferredSize(QSizeF(100, 100));
    m_instance->widget->setMaximumSize(QSizeF(150, 150));

    // Put the widget into the parent widget's layout for layout request watching
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(m_instance->parentWidget->layout());
    QVERIFY(layout != NULL);
    layout->addItem(m_instance->widget);

    m_instance->sendUpdateGeometryMessage();
    QCOMPARE(MAppletCommunicator_sentMessageType, MAppletMessage::UpdateGeometryMessage);
    QCOMPARE(MAppletCommunicator_sentUpdateGeometryMessageSizeHints.at(Qt::MinimumSize), QSizeF(50, 50));
    QCOMPARE(MAppletCommunicator_sentUpdateGeometryMessageSizeHints.at(Qt::PreferredSize), QSizeF(100, 100));
    QCOMPARE(MAppletCommunicator_sentUpdateGeometryMessageSizeHints.at(Qt::MaximumSize), QSizeF(150, 150));
}

void Ut_MExtensionRunner::testSceneChanged()
{
    connect(this, SIGNAL(changeScene(QList<QRectF>)), m_instance, SLOT(sceneChanged(QList<QRectF>)));

    m_instance->init("servername");

    // Create target pixmap
    QRectF targetRect(0, 0, 30, 30);
#ifdef Q_WS_X11
    Pixmap pixmap = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(), targetRect.width(), targetRect.height(), QX11Info::appDepth());
    QApplication::syncX();
    QPixmap targetPixmap = QPixmap::fromX11Pixmap(pixmap, QPixmap::ExplicitlyShared);
#else
    QPixmap targetPixmap(targetRect.width(), targetRect.height());
#endif

    // Assign the pixmap to the applet runner to be painted to.
    MAppletSetGeometryMessage msg(targetRect, targetPixmap.handle());
    emit sendMessage(msg);

    // Update subregion of the pixmap
    QList<QRectF> region;
    region.append(QRectF(0, 0, 10, 10));
    region.append(QRectF(10, 10, 10, 10));
    emit changeScene(region);

    QCOMPARE(MAppletCommunicator_sentMessageType, MAppletMessage::PixmapModifiedMessage);
    QCOMPARE(MAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF(0, 0, 20, 20));
    QVERIFY(sceneRenderCalled);

    // Check that no pixmap modified messages are sent when invisible
    MAppletCommunicator_sentMessageType = MAppletMessage::InvalidMessage;
    MAppletCommunicator_sentPixmapModifiedMessageGeometry = QRectF();
    sceneRenderCalled = false;

    emit sendMessage(MAppletVisibilityMessage(false));
    emit changeScene(region);
    QCOMPARE(MAppletCommunicator_sentMessageType, MAppletMessage::InvalidMessage);
    QCOMPARE(MAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF());
    QVERIFY(!sceneRenderCalled);

    region.append(QRectF(20, 20, 10, 10));
    emit changeScene(region);
    QCOMPARE(MAppletCommunicator_sentMessageType, MAppletMessage::InvalidMessage);
    QCOMPARE(MAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF());
    QVERIFY(!sceneRenderCalled);

    // Check that pixmap modified message with cumulated rect is sent when set visible again
    emit sendMessage(MAppletVisibilityMessage(true));
    QCOMPARE(MAppletCommunicator_sentMessageType, MAppletMessage::PixmapModifiedMessage);
    QCOMPARE(MAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF(0, 0, 30, 30));
    QVERIFY(sceneRenderCalled);

    // Check that cumulation of changed rect doesn't happen when applet is visible
    region.clear();
    region.append(QRectF(5, 5, 15, 15));
    sceneRenderCalled = false;
    emit changeScene(region);
    QCOMPARE(MAppletCommunicator_sentMessageType, MAppletMessage::PixmapModifiedMessage);
    QCOMPARE(MAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF(5, 5, 15, 15));
    QVERIFY(sceneRenderCalled);
}

void Ut_MExtensionRunner::testObjectMenuRequestMessage()
{
    m_instance->init("servername");
    m_instance->widget = new MWidget;

    // Put the widget into the parent widget's layout for layout request watching
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(m_instance->parentWidget->layout());
    QVERIFY(layout != NULL);
    layout->addItem(m_instance->widget);

    MAction action1(m_instance->widget);
    action1.setText("objectmenuaction1");
    action1.setLocation(MAction::ObjectMenuLocation);
    m_instance->widget->addAction(&action1);

    MAction action2(m_instance->widget);
    action2.setText("normalaction");
    action2.setLocation(MAction::ToolBarLocation);
    m_instance->widget->addAction(&action2);

    MAction action3(m_instance->widget);
    action3.setText("objectmenuaction2");
    action3.setLocation(MAction::ObjectMenuLocation);
    m_instance->widget->addAction(&action3);

    MAction action4(m_instance->widget);
    action4.setText("actionforallplaces");
    action4.setLocation(MAction::EveryLocation);
    m_instance->widget->addAction(&action4);

    m_instance->widget->setGeometry(3, 4, 10, 10);

    MAppletObjectMenuRequestMessage message(QPoint(3, 4));
    emit sendMessage(message);

    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.size(), 3);
    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.at(0), QString("objectmenuaction1"));
    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.at(1), QString("objectmenuaction2"));
    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.at(2), QString("actionforallplaces"));

    emit sendMessage(message);
    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.size(), 3);
}

void Ut_MExtensionRunner::testObjectMenuRequestMessageGetsPropagated()
{
    m_instance->init("servername");

    // Bottommost widget. This widget has an action that shouldn't get selected
    m_instance->widget = new MWidget;
    // Middle widget. This widget has an action that is supposed to get selected
    MWidget widget2(m_instance->widget);
    // Topmost widget. This widget has no actions so the search for actions should continue below this widget
    MWidget widget3(&widget2);

    // Put the widget into the parent widget's layout for layout request watching
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(m_instance->parentWidget->layout());
    QVERIFY(layout != NULL);
    layout->addItem(m_instance->widget);

    // Add action to bottommost widget
    MAction action(m_instance->widget);
    action.setText("wrong_action");
    action.setLocation(MAction::ObjectMenuLocation);
    m_instance->widget->addAction(&action);

    // Add action to middle widget
    MAction action2(&widget2);
    action2.setText("correct_action");
    action2.setLocation(MAction::ObjectMenuLocation);
    widget2.addAction(&action2);

    m_instance->widget->setGeometry(0, 0, 10, 10);
    widget2.setGeometry(0, 0, 10, 10);
    widget3.setGeometry(0, 0, 10, 10);

    MAppletObjectMenuRequestMessage message(QPoint(3, 4));
    emit sendMessage(message);

    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.size(), 1);
    QCOMPARE(MAppletCommunicator_sentObjectMenuActionList.at(0), QString("correct_action"));
}

#ifdef HAVE_CONTEXTSUBSCRIBER
void Ut_MExtensionRunner::testWhenDisplayBlankedThenAliveCheckingStops()
{
    gContextPropertyValue = "blanked";
    gQTimerStopCalls.clear();
    m_instance->updateDisplayState();
    QVERIFY(qFind(gQTimerStopCalls, m_instance->aliveTimer) != gQTimerStopCalls.end());

    gQTimerStartCalls.clear();
    m_instance->messageReceived(MAppletAliveMessageRequest());
    QCOMPARE(gQTimerStartCalls.count(), 0);

    gContextPropertyValue = "";
    m_instance->updateDisplayState();
    QVERIFY(qFind(gQTimerStartCalls, m_instance->aliveTimer) != gQTimerStartCalls.end());
}
#endif

QTEST_MAIN(Ut_MExtensionRunner)
