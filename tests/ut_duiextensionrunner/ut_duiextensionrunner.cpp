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

#include "ut_duiextensionrunner.h"
#include <duiextensionrunner.h>
#include <duiappletclient.h>
#include <duiappletmetadata.h>
#include <duiappletmousemessage.h>
#include <duiappletvisibilitymessage.h>
#include <duiappletorientationmessage.h>
#include <duiappletsetgeometrymessage.h>
#include <duiappletalivemessagerequest.h>
#include <duiappletpixmapmodifiedmessage.h>
#include <duiappletupdategeometrymessage.h>
#include <duiappletobjectmenurequestmessage.h>
#include <duiappletobjectmenumessage.h>
#include <duicomponentdata.h>
#include <duiappletsharedmutex_stub.h>
#include <duiappletid_stub.h>
#include <duiappletsettings_stub.h>
#include <duigconfdatastore.h>
#include <DuiAction>
#include <DuiWidget>
#include <DuiApplication>
#include <QtTest/QtTest>
#include <QGraphicsLinearLayout>

// DuiWindow stubs (to prevent crashing)
DuiWindow::DuiWindow(QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(0)
{
}
DuiWindow::~DuiWindow()
{
}

// QTimer stubs (used by DuiExtensionRunner)
void QTimer::start(int)
{
    if (Ut_DuiExtensionRunner::timerImmediateTimeout) {
        emit timeout();
    }
}

// QCoreApplication stubs (used by DuiExtensionRunner)
void QCoreApplication::quit()
{
    Ut_DuiExtensionRunner::quitCalled = true;
}

// DuiComponentData stubs
bool DuiComponentData::emulateTwoFingerGestures()
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

bool DuiAppletCommunicator_connectToProcessReturn = true;
bool DuiAppletClient::connectToServer(const QString &serverName)
{
    Q_UNUSED(serverName);
    return DuiAppletCommunicator_connectToProcessReturn;
}
DuiAppletMessage::DuiAppletMessageType DuiAppletCommunicator_sentMessageType;
QRectF DuiAppletCommunicator_sentPixmapModifiedMessageGeometry;
QList<QString> DuiAppletCommunicator_sentObjectMenuActionList;
QVector<QSizeF> DuiAppletCommunicator_sentUpdateGeometryMessageSizeHints;
bool DuiAppletCommunicator::sendMessage(const DuiAppletMessage &message)
{
    DuiAppletCommunicator_sentMessageType = message.type();
    if (DuiAppletCommunicator_sentMessageType == DuiAppletMessage::PIXMAP_MODIFIED_MESSAGE) {
        const DuiAppletPixmapModifiedMessage pixmapModifiedMessage = dynamic_cast<const DuiAppletPixmapModifiedMessage &>(message);
        DuiAppletCommunicator_sentPixmapModifiedMessageGeometry = pixmapModifiedMessage.geometry();
    } else if (DuiAppletCommunicator_sentMessageType == DuiAppletMessage::UPDATE_GEOMETRY_MESSAGE) {
        const DuiAppletUpdateGeometryMessage updateGeometryMessage = dynamic_cast<const DuiAppletUpdateGeometryMessage &>(message);
        DuiAppletCommunicator_sentUpdateGeometryMessageSizeHints = updateGeometryMessage.sizeHints();
    } else if (DuiAppletCommunicator_sentMessageType == DuiAppletMessage::OBJECT_MENU_MESSAGE) {
        const DuiAppletObjectMenuMessage objectMenuMessage = dynamic_cast<const DuiAppletObjectMenuMessage &>(message);
        DuiAppletCommunicator_sentObjectMenuActionList = objectMenuMessage.actionList();
    }
    return true;
}

bool DuiAppletMetaData_isValidReturn = true;
bool DuiAppletMetaData::isValid() const
{
    return DuiAppletMetaData_isValidReturn;
}

bool Ut_DuiExtensionRunner::timerImmediateTimeout;
bool Ut_DuiExtensionRunner::quitCalled;

// Unit test cases
void Ut_DuiExtensionRunner::init()
{
    m_instance = new DuiExtensionRunner();
    connect(this, SIGNAL(sendMessage(DuiAppletMessage)), m_instance, SLOT(messageReceived(DuiAppletMessage)));

    timerImmediateTimeout = false;
    quitCalled = false;
    gDuiAppletSharedMutexStub->stubReset();
    gDuiAppletSharedMutexStub->stubSetReturnValue("init", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("lock", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("unlock", true);
    gDuiAppletSharedMutexStub->stubSetReturnValue("tryLock", true);
    DuiAppletCommunicator_connectToProcessReturn = true;
    DuiAppletCommunicator_sentMessageType = DuiAppletMessage::INVALID;
    DuiAppletCommunicator_sentPixmapModifiedMessageGeometry = QRectF();
    DuiAppletCommunicator_sentUpdateGeometryMessageSizeHints = QVector<QSizeF>(Qt::NSizeHints);
    sceneRenderCalled = false;
    gDuiAppletSettingsStub->stubReset();
}

void Ut_DuiExtensionRunner::cleanup()
{
    delete m_instance;
    m_instance = NULL;
}

void Ut_DuiExtensionRunner::initTestCase()
{
}

void Ut_DuiExtensionRunner::cleanupTestCase()
{
}

void Ut_DuiExtensionRunner::testInitializationSuccess()
{
    QCOMPARE(m_instance->init("servername"), true);
    QCOMPARE(gDuiAppletSharedMutexStub->stubLastCallTo("init").parameter<QString>(0), QString("servername"));
}

void Ut_DuiExtensionRunner::testInitializationFailCommunicationFails()
{
    DuiAppletCommunicator_connectToProcessReturn = false;
    QCOMPARE(m_instance->init("servername"), false);
}

void Ut_DuiExtensionRunner::testInitializationFailSharedMemoryAttachFails()
{
    gDuiAppletSharedMutexStub->stubSetReturnValue("init", false);
    QCOMPARE(m_instance->init("servername"), false);
}

void Ut_DuiExtensionRunner::testRelayingMouseEvents()
{
    m_instance->init("servername");

    // Install event filter
    MouseEventFilter filter;
    QCoreApplication::instance()->installEventFilter(&filter);

    // Emit mouse press message
    DuiAppletMouseMessage message1(DuiAppletMessage::MOUSE_PRESS_MESSAGE, QPointF(-5, 2), Qt::RightButton, Qt::RightButton | Qt::MidButton);
    emit sendMessage(message1);
    QCOMPARE(filter.events.count(), 1);
    QCOMPARE(filter.events.at(0)->type(), QEvent::GraphicsSceneMousePress);
    QCOMPARE(filter.events.at(0)->button(), Qt::RightButton);
    QCOMPARE(filter.events.at(0)->buttons(), Qt::RightButton | Qt::MidButton);

    // Emit mouse release message
    DuiAppletMouseMessage message2(DuiAppletMessage::MOUSE_RELEASE_MESSAGE, QPointF(0, 4), Qt::LeftButton, Qt::LeftButton | Qt::MidButton);
    emit sendMessage(message2);
    QCOMPARE(filter.events.count(), 2);
    QCOMPARE(filter.events.at(1)->type(), QEvent::GraphicsSceneMouseRelease);
    QCOMPARE(filter.events.at(1)->button(), Qt::LeftButton);
    QCOMPARE(filter.events.at(1)->buttons(), Qt::LeftButton | Qt::MidButton);

    // Emit mouse move message
    DuiAppletMouseMessage message3(DuiAppletMessage::MOUSE_MOVE_MESSAGE, QPointF(-5, 2), Qt::MidButton, Qt::MidButton);
    emit sendMessage(message3);
    QCOMPARE(filter.events.count(), 3);
    QCOMPARE(filter.events.at(2)->type(), QEvent::GraphicsSceneMouseMove);
    QCOMPARE(filter.events.at(2)->button(), Qt::MidButton);
    QCOMPARE(filter.events.at(2)->buttons(), Qt::MidButton);

    // Remove event filter
    QCoreApplication::instance()->removeEventFilter(&filter);
}

void Ut_DuiExtensionRunner::testAppletRunnerQuitsIfMessageReceivingTimeouts()
{
    m_instance->init("servername");

    // Verify that the quit is not always called
    DuiAppletMouseMessage message1(DuiAppletMessage::MOUSE_PRESS_MESSAGE, QPointF(-5, 2), Qt::RightButton, Qt::RightButton | Qt::MidButton);
    emit sendMessage(message1);
    QVERIFY(!quitCalled);

    // Verify that quit is called when the timer times out
    timerImmediateTimeout = true;
    emit sendMessage(message1);
    QVERIFY(quitCalled);
}

// This test is here to test to nothing happens. Not segfaults or anything. That's why it doesn't really compare anything
void Ut_DuiExtensionRunner::testMessageReceivedBeforeAppletInit()
{
    {
        // Mouse messages are more or less the same so it's enough to test only one
        DuiAppletMouseMessage msg(DuiAppletMessage::MOUSE_PRESS_MESSAGE, QPointF(-5, 2), Qt::RightButton, Qt::RightButton | Qt::MidButton);
        emit sendMessage(msg);
    }

    {
        DuiAppletVisibilityMessage msg(true);
        emit sendMessage(msg);
    }

    {
        DuiAppletOrientationMessage msg;
        emit sendMessage(msg);
    }

    {
        DuiAppletSetGeometryMessage msg;
        emit sendMessage(msg);
    }

    {
        DuiAppletAliveMessageRequest msg;
        emit sendMessage(msg);
    }
}

void Ut_DuiExtensionRunner::testPaintingOnPixmap()
{
    connect(this, SIGNAL(changeScene(QList<QRectF>)), m_instance, SLOT(sceneChanged(QList<QRectF>)));

    m_instance->init("servername");

    // Create target pixmap and fill it with full opaque white
    QPixmap targetPixmap(150, 100);
    // Painting with transparent will activate the alpha channel in the pixmap
    targetPixmap.fill(Qt::transparent);
    targetPixmap.fill(Qt::white);

    // Assign the pixmap to the applet runner to be painted to.
    DuiAppletSetGeometryMessage msg(QRectF(0, 0, 150, 100), targetPixmap.handle());
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

void Ut_DuiExtensionRunner::testSendUpdateGeometryMessage()
{
    m_instance->init("servername");
    m_instance->widget = new DuiWidget;
    m_instance->widget->setMinimumSize(QSizeF(50, 50));
    m_instance->widget->setPreferredSize(QSizeF(100, 100));
    m_instance->widget->setMaximumSize(QSizeF(150, 150));

    // Put the widget into the parent widget's layout for layout request watching
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(m_instance->parentWidget->layout());
    QVERIFY(layout != NULL);
    layout->addItem(m_instance->widget);

    m_instance->sendUpdateGeometryMessage();
    QCOMPARE(DuiAppletCommunicator_sentMessageType, DuiAppletMessage::UPDATE_GEOMETRY_MESSAGE);
    QCOMPARE(DuiAppletCommunicator_sentUpdateGeometryMessageSizeHints.at(Qt::MinimumSize), QSizeF(50, 50));
    QCOMPARE(DuiAppletCommunicator_sentUpdateGeometryMessageSizeHints.at(Qt::PreferredSize), QSizeF(100, 100));
    QCOMPARE(DuiAppletCommunicator_sentUpdateGeometryMessageSizeHints.at(Qt::MaximumSize), QSizeF(150, 150));
}

void Ut_DuiExtensionRunner::testSceneChanged()
{
    connect(this, SIGNAL(changeScene(QList<QRectF>)), m_instance, SLOT(sceneChanged(QList<QRectF>)));

    m_instance->init("servername");

    // Create target pixmap
    QPixmap targetPixmap(30, 30);

    // Assign the pixmap to the applet runner to be painted to.
    DuiAppletSetGeometryMessage msg(QRectF(0, 0, 30, 30), targetPixmap.handle());
    emit sendMessage(msg);

    // Update subregion of the pixmap
    QList<QRectF> region;
    region.append(QRectF(0, 0, 10, 10));
    region.append(QRectF(10, 10, 10, 10));
    emit changeScene(region);

    QCOMPARE(DuiAppletCommunicator_sentMessageType, DuiAppletMessage::PIXMAP_MODIFIED_MESSAGE);
    QCOMPARE(DuiAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF(0, 0, 20, 20));
    QVERIFY(sceneRenderCalled);

    // Check that no pixmap modified messages are sent when invisible
    DuiAppletCommunicator_sentMessageType = DuiAppletMessage::INVALID;
    DuiAppletCommunicator_sentPixmapModifiedMessageGeometry = QRectF();
    sceneRenderCalled = false;

    emit sendMessage(DuiAppletVisibilityMessage(false));
    emit changeScene(region);
    QCOMPARE(DuiAppletCommunicator_sentMessageType, DuiAppletMessage::INVALID);
    QCOMPARE(DuiAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF());
    QVERIFY(!sceneRenderCalled);

    region.append(QRectF(20, 20, 10, 10));
    emit changeScene(region);
    QCOMPARE(DuiAppletCommunicator_sentMessageType, DuiAppletMessage::INVALID);
    QCOMPARE(DuiAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF());
    QVERIFY(!sceneRenderCalled);

    // Check that pixmap modified message with cumulated rect is sent when set visible again
    emit sendMessage(DuiAppletVisibilityMessage(true));
    QCOMPARE(DuiAppletCommunicator_sentMessageType, DuiAppletMessage::PIXMAP_MODIFIED_MESSAGE);
    QCOMPARE(DuiAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF(0, 0, 30, 30));
    QVERIFY(sceneRenderCalled);

    // Check that cumulation of changed rect doesn't happen when applet is visible
    region.clear();
    region.append(QRectF(5, 5, 15, 15));
    sceneRenderCalled = false;
    emit changeScene(region);
    QCOMPARE(DuiAppletCommunicator_sentMessageType, DuiAppletMessage::PIXMAP_MODIFIED_MESSAGE);
    QCOMPARE(DuiAppletCommunicator_sentPixmapModifiedMessageGeometry, QRectF(5, 5, 15, 15));
    QVERIFY(sceneRenderCalled);
}

void Ut_DuiExtensionRunner::testObjectMenuRequestMessage()
{
    m_instance->init("servername");
    m_instance->widget = new DuiWidget;

    // Put the widget into the parent widget's layout for layout request watching
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(m_instance->parentWidget->layout());
    QVERIFY(layout != NULL);
    layout->addItem(m_instance->widget);

    DuiAction action1(m_instance->widget);
    action1.setText("objectmenuaction1");
    action1.setLocation(DuiAction::ObjectMenuLocation);
    m_instance->widget->addAction(&action1);

    DuiAction action2(m_instance->widget);
    action2.setText("normalaction");
    action2.setLocation(DuiAction::ToolBarLocation);
    m_instance->widget->addAction(&action2);

    DuiAction action3(m_instance->widget);
    action3.setText("objectmenuaction2");
    action3.setLocation(DuiAction::ObjectMenuLocation);
    m_instance->widget->addAction(&action3);

    DuiAction action4(m_instance->widget);
    action4.setText("actionforallplaces");
    action4.setLocation(DuiAction::EveryLocation);
    m_instance->widget->addAction(&action4);

    m_instance->widget->setGeometry(3, 4, 10, 10);

    DuiAppletObjectMenuRequestMessage message(QPoint(3, 4));
    emit sendMessage(message);

    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.size(), 3);
    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.at(0), QString("objectmenuaction1"));
    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.at(1), QString("objectmenuaction2"));
    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.at(2), QString("actionforallplaces"));

    emit sendMessage(message);
    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.size(), 3);
}

void Ut_DuiExtensionRunner::testObjectMenuRequestMessageGetsPropagated()
{
    m_instance->init("servername");

    // Bottommost widget. This widget has an action that shouldn't get selected
    m_instance->widget = new DuiWidget;
    // Middle widget. This widget has an action that is supposed to get selected
    DuiWidget widget2(m_instance->widget);
    // Topmost widget. This widget has no actions so the search for actions should continue below this widget
    DuiWidget widget3(&widget2);

    // Put the widget into the parent widget's layout for layout request watching
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(m_instance->parentWidget->layout());
    QVERIFY(layout != NULL);
    layout->addItem(m_instance->widget);

    // Add action to bottommost widget
    DuiAction action(m_instance->widget);
    action.setText("wrong_action");
    action.setLocation(DuiAction::ObjectMenuLocation);
    m_instance->widget->addAction(&action);

    // Add action to middle widget
    DuiAction action2(&widget2);
    action2.setText("correct_action");
    action2.setLocation(DuiAction::ObjectMenuLocation);
    widget2.addAction(&action2);

    m_instance->widget->setGeometry(0, 0, 10, 10);
    widget2.setGeometry(0, 0, 10, 10);
    widget3.setGeometry(0, 0, 10, 10);

    DuiAppletObjectMenuRequestMessage message(QPoint(3, 4));
    emit sendMessage(message);

    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.size(), 1);
    QCOMPARE(DuiAppletCommunicator_sentObjectMenuActionList.at(0), QString("correct_action"));
}

QTEST_MAIN(Ut_DuiExtensionRunner)
