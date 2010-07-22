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

const MStatusBarStyle* MStatusBarStyleContainer::operator->() const
{
    MStatusBarStyle* style = new MStatusBarStyle;
    style->setSwipeThreshold(30);
    return style;
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

// QGraphicsSceneMouseEvent::pos mock
QPointF mouseDownPos;
QPointF mouseMovePos;
bool mouseDown;
bool mouseMove;
QPointF QGraphicsSceneMouseEvent::pos() const
{
    if (mouseDown)
        return mouseDownPos;
    else if (mouseMove)
        return mouseMovePos;
    else
        return QPointF();
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
    m_statusbar = new MStatusBar();
    m_subject= new MStatusBarView(m_statusbar);
}

void Ut_MStatusBarView::cleanupTestCase()
{
    delete m_statusbar;
    m_statusbar = 0;
    delete app;
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
    mouseDownWorker();
    mouseMoveworker(QPointF(15.20, 59.10));
    QCOMPARE(dbusCallMade, true);
    QCOMPARE(dbusService, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_SERVICE);
    QCOMPARE(dbusInterface, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_INTERFACE);
    QCOMPARE(dbusPath, MStatusBarView::STATUS_INDICATOR_MENU_DBUS_PATH);
    QCOMPARE(callMode, QDBus::NoBlock);
    QCOMPARE(dbusMethod, QString("open"));
}

void Ut_MStatusBarView::testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear()
{
    mouseDownWorker();
    mouseMoveworker(QPointF(15.20, 49.10));
    QCOMPARE(dbusCallMade, false);
}

void Ut_MStatusBarView::testWhenMousePressHapticsDone()
{
    mouseDownWorker();
    QCOMPARE(hapticsDone, true);
}

// Helpers
void Ut_MStatusBarView::mouseDownWorker()
{
    mouseDownPos = QPointF(12.24, 20.20); // Arbitary point
    mouseDown = true;
    QGraphicsSceneMouseEvent mouseDownEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&mouseDownEvent);
}

void Ut_MStatusBarView::mouseMoveworker(QPointF moveTo)
{
    mouseMovePos = moveTo;
    mouseMove = true;
    mouseDown = false;
    QGraphicsSceneMouseEvent mouseMoveEvent(QEvent::GraphicsSceneMouseMove);
    m_subject->mouseMoveEvent(&mouseMoveEvent);
}

#endif

QTEST_APPLESS_MAIN(Ut_MStatusBarView)
