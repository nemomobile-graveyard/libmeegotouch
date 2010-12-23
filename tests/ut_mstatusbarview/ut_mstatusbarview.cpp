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

#include <QObject>
#include <QtDBus/QtDBus>
#include <QGraphicsSceneMouseEvent>
#include <QDBusPendingReply>
#include <MApplication>

#include <mstatusbar.h>
#include "mstatusbar_p.h"
#include "ut_mstatusbarview.h"

#define SWIPE_THRESHOLD 30
#define SCENE_WIDTH 100

// an arbitrary point
#define START_POINT QPointF(15.20, 2.10)

// QGraphicsItem::sceneBoundingRect mock
QRectF QGraphicsItem::sceneBoundingRect() const
{
    return QRect(0, 0, SCENE_WIDTH, SCENE_WIDTH);
}

// Stub the Dbus call used by status bar to open a menu
bool dbusCallMade = false;
QString dbusService = QString();
QString dbusPath = QString();
QString dbusInterface = QString();
QDBus::CallMode callMode = QDBus::Block;
QString dbusMethod = QString();
QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode callmode, const QString &method,
        const QVariant &/*arg1*/,
        const QVariant &/*arg2*/,
        const QVariant &/*arg3*/,
        const QVariant &/*arg4*/,
        const QVariant &/*arg5*/,
        const QVariant &/*arg6*/,
        const QVariant &/*arg7*/,
        const QVariant &/*arg8*/)
{
    dbusCallMade = true;
    dbusInterface = this->interface();
    dbusService = this->service();
    dbusPath = this->path();
    callMode = callmode;
    dbusMethod = method;
    QDBusMessage message;
    return message;
}

// QGraphicsSceneMouseEvent::pos and QGraphicsSceneMouseEvent::scenePos mock
QPointF mouseDownPos;
QPointF mouseMovePos;
QPointF mouseUpPos;

namespace MouseEvent {
    enum Event {
        MouseDown,
        MouseMove,
        MouseUp
    };
};

MouseEvent::Event mouseEvent;

QPointF QGraphicsSceneMouseEvent::pos() const
{
    switch(mouseEvent) {
        case MouseEvent::MouseDown:
            return mouseDownPos;
        case MouseEvent::MouseMove:
            return mouseMovePos;
        case MouseEvent::MouseUp:
            return mouseUpPos;
    };

    return QPointF();
}

QPointF QGraphicsSceneMouseEvent::scenePos() const
{
    return pos();
}

bool hapticsDone = false;
void MFeedback::play() const
{
    hapticsDone = true;
}

Ut_MStatusBarView::Ut_MStatusBarView():
        m_subject(0),
        m_statusbar(0),
        app(0)
{
}

void Ut_MStatusBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mstatusbarview" };
    app = new MApplication(argc, app_name);
}

void Ut_MStatusBarView::cleanupTestCase()
{
    delete app;
}

void Ut_MStatusBarView::init()
{
    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);
    m_subject->modifiableStyle()->setUseSwipeGesture(true);
    m_subject->modifiableStyle()->setSwipeThreshold(SWIPE_THRESHOLD);
}

void Ut_MStatusBarView::cleanup()
{
    dbusCallMade = false;
    hapticsDone = false;
    dbusCallMade = false;
    dbusInterface = QString();
    dbusService = QString();
    dbusPath = QString();
    callMode = QDBus::NoBlock;
    dbusMethod = QString();

    delete m_statusbar;
    m_statusbar = 0;
}

#ifdef Q_WS_X11
//void Ut_MStatusBarView::testXDamageWhileTogglingVisibility()
//{
//    m_subject->sharedPixmap = QPixmap(50,50);
//    m_subject->isPixmapProviderOnline = true;
//    m_subject->sharedPixmapHandleReceived(new QDBusPendingCallWatcher);
//    QVERIFY(m_subject->pixmapDamage != 0);
//    m_subject->disablePixmapUpdates();
//    QVERIFY(m_subject->pixmapDamage == 0);
//}

//void Ut_MStatusBarView::testXDamageWhileTogglingProviderAvailability()
//{
//    m_subject->sharedPixmap = QPixmap(50,50);
//    m_subject->updatesEnabled = true;
//    m_subject->sharedPixmapHandleReceived(new QDBusPendingCallWatcher);
//    QVERIFY(m_subject->pixmapDamage != 0);
//    m_subject->handlePixmapProviderOffline();
//    QVERIFY(m_subject->pixmapDamage == 0);
//}

void Ut_MStatusBarView::testWhenMouseMovesAboveThresholdStatusIndicatorMenuAppears()
{
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD + 1));
    QCOMPARE(dbusCallMade, true);
    QCOMPARE(dbusService, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_SERVICE);
    QCOMPARE(dbusInterface, QString(MStatusBarView::STATUS_INDICATOR_MENU_DBUS_INTERFACE));
    QCOMPARE(dbusPath, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_PATH);
    QCOMPARE(callMode, QDBus::NoBlock);
    QCOMPARE(dbusMethod, QString("open"));
}

void Ut_MStatusBarView::testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear()
{
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD - 1));
    QCOMPARE(dbusCallMade, false);
}

void Ut_MStatusBarView::testWhenMousePressHapticsDone()
{
    mouseDownWorker(START_POINT);
    QCOMPARE(hapticsDone, true);
}

void Ut_MStatusBarView::testWhenUsingSwipeTapDoesNothing()
{
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT);
    QCOMPARE(dbusCallMade, false);
}

void Ut_MStatusBarView::testWhenUsingTapSwipeDoesNothing()
{
    m_subject->modifiableStyle()->setUseSwipeGesture(false);
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD - 1));
    QCOMPARE(dbusCallMade, false);
}

void Ut_MStatusBarView::testTapFunctionality()
{
    m_subject->modifiableStyle()->setUseSwipeGesture(false);

    // Test that a tap where a release is out of bounds is not recognised
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT + QPointF(SCENE_WIDTH, SCENE_WIDTH) * 2);
    QCOMPARE(dbusCallMade, false);

    // Test that a tap where a release is within bounds is recognised
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT + QPointF(5,5));
    QCOMPARE(dbusCallMade, true);
    QCOMPARE(dbusService, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_SERVICE);
    QCOMPARE(dbusInterface, QString(MStatusBarView::STATUS_INDICATOR_MENU_DBUS_INTERFACE));
    QCOMPARE(dbusPath, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_PATH);
    QCOMPARE(callMode, QDBus::NoBlock);
    QCOMPARE(dbusMethod, QString("open"));
}



// Helpers
void Ut_MStatusBarView::mouseDownWorker(QPointF downAt)
{
    mouseDownPos = downAt;
    mouseEvent = MouseEvent::MouseDown;
    QGraphicsSceneMouseEvent mouseDownEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&mouseDownEvent);
}

void Ut_MStatusBarView::mouseMoveWorker(QPointF moveTo)
{
    mouseMovePos = moveTo;
    mouseEvent = MouseEvent::MouseMove;
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    m_subject->mouseMoveEvent(&mouseMoveEvent);
}

void Ut_MStatusBarView::mouseUpWorker(QPointF upAt)
{
    mouseUpPos = upAt;
    mouseEvent = MouseEvent::MouseUp;
    QGraphicsSceneMouseEvent mouseReleaseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(&mouseReleaseEvent);
}

#endif

QTEST_APPLESS_MAIN(Ut_MStatusBarView)
