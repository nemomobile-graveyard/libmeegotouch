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
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <MApplication>
#include <MWindow>
#include <MSceneManager>
#include <MScene>

// our own version of the wrapper
#include "mstatusbarviewdbuswrapper.h"

#include <mstatusbar.h>
#include "mstatusbar_p.h"
#include "ut_mstatusbarview.h"

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#endif

#define SWIPE_THRESHOLD 30
#define SCENE_WIDTH 100

// an arbitrary point
#define START_POINT QPointF(15.20, 2.10)

// QGraphicsItem::sceneBoundingRect mock
QRectF QGraphicsItem::sceneBoundingRect() const
{
    return QRect(0, 0, SCENE_WIDTH, SCENE_WIDTH);
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

qreal gTestOpacity;
QRectF gFilledRect;
QColor gFillColor;
QRectF gPixmapTargetRect;
QRectF gPixmapSourceRect;

void QPainter::setOpacity(qreal opacity)
{
    gTestOpacity = opacity;
}

void QPainter::fillRect(const QRectF &rectangle, const QColor &color)
{
    gFilledRect = rectangle;
    gFillColor = color;
}

void QPainter::drawPixmap(const QRectF &target, const QPixmap &pixmap, const QRectF &source)
{
    Q_UNUSED(pixmap);

    gPixmapTargetRect = target;
    gPixmapSourceRect = source;
}

void QPainter::save()
{
}

void QPainter::restore()
{
}


Ut_MStatusBarView::Ut_MStatusBarView():
        m_subject(0),
        m_statusbar(0),
        m_pixmapHandle(0),
        app(0)
{
}

void Ut_MStatusBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mstatusbarview" };
    app = new MApplication(argc, app_name);

#ifdef Q_WS_X11
    createX11SharedPixmap();
#endif
}

void Ut_MStatusBarView::cleanupTestCase()
{
#ifdef Q_WS_X11
    freeX11SharedPixmap();
#endif
    delete app;
}

void Ut_MStatusBarView::init()
{
}

void Ut_MStatusBarView::cleanup()
{
    hapticsDone = false;

    delete m_statusbar;
    m_statusbar = 0;
}

void Ut_MStatusBarView::testWhenMouseMovesAboveThresholdStatusIndicatorMenuAppears()
{
    initHelper();
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD + 1));

    QVERIFY(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled);
}

void Ut_MStatusBarView::testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear()
{
    initHelper();
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD - 1));
    QCOMPARE(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled, false);
}

void Ut_MStatusBarView::testWhenMousePressHapticsDone()
{
    initHelper();
    mouseDownWorker(START_POINT);
    QCOMPARE(hapticsDone, true);
}

void Ut_MStatusBarView::testWhenUsingSwipeTapDoesNothing()
{
    initHelper();
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT);
    QCOMPARE(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled, false);
}

void Ut_MStatusBarView::testWhenUsingTapSwipeDoesNothing()
{
    initHelper();
    m_subject->modifiableStyle()->setUseSwipeGesture(false);
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD - 1));
    QCOMPARE(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled, false);
}

void Ut_MStatusBarView::testTapFunctionality()
{
    initHelper();
    m_subject->modifiableStyle()->setUseSwipeGesture(false);

    // Test that a tap where a release is out of bounds is not recognised
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT + QPointF(SCENE_WIDTH, SCENE_WIDTH) * 2);
    QCOMPARE(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled, false);

    // Test that a tap where a release is within bounds is recognised
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT + QPointF(5,5));
    QCOMPARE(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled, true);
}


void Ut_MStatusBarView::testPressDimming()
{
    initHelper();
    qreal styleDimCSS = m_subject->modifiableStyle()->pressDimFactor();
    QCOMPARE(styleDimCSS, m_subject->modifiableStyle()->pressDimFactor());

    m_subject->modifiableStyle()->setPressDimFactor(0.6);

    m_subject->sharedPixmap = QPixmap(50, 50);
    MSceneManager sceneManager;
    m_statusbar->appear(sceneManager.scene());

    QPainter painter;

    gTestOpacity = -1;
    gFillColor = -1;
    gFilledRect = QRectF();

    // Check that the dimming rectangle isn't being drawn by default
    m_subject->drawContents(&painter, NULL);
    QVERIFY(gFilledRect.isNull());

    mouseDownWorker(START_POINT);
    // Now the dimmer should be drawn
    m_subject->drawContents(&painter, NULL);

    QCOMPARE(gTestOpacity, m_subject->modifiableStyle()->pressDimFactor());
    QCOMPARE(gFillColor, QColor(Qt::black));
    QCOMPARE(gFilledRect.size(), gPixmapSourceRect.size());
    QCOMPARE(gFilledRect.topLeft(), gPixmapTargetRect.topLeft());

    // Release the mouse button and verify that the dimmer disappers
    mouseUpWorker(START_POINT);

    gFilledRect = QRectF();
    m_subject->drawContents(&painter, NULL);
    QVERIFY(gFilledRect.isNull());

    // These are owned by the scene now
    m_statusbar = NULL;
    m_subject = NULL;
}

void Ut_MStatusBarView::testStatusIndicatorMenuDisabling()
{
    initHelper();
    m_subject->modifiableStyle()->setEnableStatusIndicatorMenu(false);
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD + 1));
    QCOMPARE(MStatusBarViewDBusWrapper::self->m_openStatusIndicatorMenuCalled, false);
}

#ifdef Q_WS_X11
void Ut_MStatusBarView::testFetchingSharedPixmapHandle()
{
    MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = true;

    // It should ask for the handle straight away, regardless of whether we are on display
    // and regardless of whether we are on switcher.

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    MStatusBarViewDBusWrapper *dbusWrapper = MStatusBarViewDBusWrapper::self;

    QCOMPARE(dbusWrapper->havePendingSharedPixmapHandleReply(), true);

    dbusWrapper->emitSharedPixmapHandleFromProviderReceived(m_pixmapHandle, true);

    QCOMPARE(m_subject->sharedPixmap.handle(), m_pixmapHandle);
}

void Ut_MStatusBarView::testFetchSharedPixmapWhenProviderRegisters()
{
    MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = false;

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    MStatusBarViewDBusWrapper *dbusWrapper = MStatusBarViewDBusWrapper::self;

    // Initially we don't have a shared pixmap
    QCOMPARE(m_subject->sharedPixmap.isNull(), true);

    // querySharedPixmapHandleFromProvider() shouldn't have been called yet.
    QCOMPARE(dbusWrapper->havePendingSharedPixmapHandleReply(), false);

    dbusWrapper->m_isPixmapProviderServiceRegistered = true;
    dbusWrapper->emitPixmapProviderServiceRegistered();

    // querySharedPixmapHandleFromProvider() should have been called.
    QCOMPARE(dbusWrapper->havePendingSharedPixmapHandleReply(), true);

    dbusWrapper->emitSharedPixmapHandleFromProviderReceived(m_pixmapHandle, true);

    QCOMPARE(m_subject->sharedPixmap.handle(), m_pixmapHandle);
}

void Ut_MStatusBarView::testDiscardSharedPixmapWhenProviderUnregisters()
{
    MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = true;

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    MStatusBarViewDBusWrapper *dbusWrapper = MStatusBarViewDBusWrapper::self;

    dbusWrapper->emitSharedPixmapHandleFromProviderReceived(m_pixmapHandle, true);

    QCOMPARE(m_subject->sharedPixmap.handle(), m_pixmapHandle);

    dbusWrapper->m_isPixmapProviderServiceRegistered = false;
    dbusWrapper->emitPixmapProviderServiceUnregistered();

    QCOMPARE(m_subject->sharedPixmap.isNull(), true);
}
#endif // Q_WS_X11

#ifdef HAVE_XDAMAGE
void Ut_MStatusBarView::testTrackXDamageEventsForSharedPixmapWhenEnterDisplay()
{
    MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = true;

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    MStatusBarViewDBusWrapper *dbusWrapper = MStatusBarViewDBusWrapper::self;

    // we don't even have a window
    QCOMPARE(m_subject->isOnDisplay, false);
    QCOMPARE(m_subject->isInSwitcher, false);

    // give it a shared pixmap
    dbusWrapper->emitSharedPixmapHandleFromProviderReceived(m_pixmapHandle, true);

    // should not setup XDamage if we're not on display
    QCOMPARE(m_subject->pixmapDamage, static_cast<Qt::HANDLE>(0));

    // simulating emission of displayEntered()
    m_subject->handleDisplayEntered();
    QCOMPARE(m_subject->isOnDisplay, true);

    // Now we should be tracking XDamage events of our shared pixmap
    QVERIFY(m_subject->pixmapDamage != 0);
}

void Ut_MStatusBarView::testDestroyXDamageTrackingForSharedPixmapWhenExitDisplay()
{
    MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = true;

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    MStatusBarViewDBusWrapper *dbusWrapper = MStatusBarViewDBusWrapper::self;

    // give it a shared pixmap
    dbusWrapper->emitSharedPixmapHandleFromProviderReceived(m_pixmapHandle, true);

    // simulating emission of displayEntered()
    m_subject->handleDisplayEntered();

    // Now we should be tracking XDamage events of our shared pixmap
    QVERIFY(m_subject->pixmapDamage != 0);

    // simulating emission of displayExited()
    m_subject->handleDisplayExited();
    QCOMPARE(m_subject->isOnDisplay, false);

    // We should have destroyed our XDamage
    QCOMPARE(m_subject->pixmapDamage, static_cast<Qt::HANDLE>(0));
}

void Ut_MStatusBarView::testDestroyXDamageTrackingForSharedPixmapWhenEnterSwitcher()
{
    MStatusBarViewDBusWrapper::m_isPixmapProviderServiceRegistered = true;

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    MStatusBarViewDBusWrapper *dbusWrapper = MStatusBarViewDBusWrapper::self;

    // give it a shared pixmap
    dbusWrapper->emitSharedPixmapHandleFromProviderReceived(m_pixmapHandle, true);

    // simulating emission of displayEntered()
    m_subject->handleDisplayEntered();

    // Now we should be tracking XDamage events of our shared pixmap
    QVERIFY(m_subject->pixmapDamage != 0);

    QCOMPARE(m_subject->isInSwitcher, false);
    QCOMPARE(m_subject->isOnDisplay, true);

    // simulating emission of MWindow::switcherEntered()
    m_subject->handleSwitcherEntered();

    // We should have destroyed our XDamage now that we are in the switcher, even though we are also
    // still on display
    QCOMPARE(m_subject->isInSwitcher, true);
    QCOMPARE(m_subject->isOnDisplay, true);
    QCOMPARE(m_subject->pixmapDamage, static_cast<Qt::HANDLE>(0));
}
#endif //HAVE_XDAMAGE

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

void Ut_MStatusBarView::initHelper()
{
    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);
    m_subject->modifiableStyle()->setUseSwipeGesture(true);
    m_subject->modifiableStyle()->setSwipeThreshold(SWIPE_THRESHOLD);
    m_subject->modifiableStyle()->setEnableStatusIndicatorMenu(true);
}

#ifdef Q_WS_X11
void Ut_MStatusBarView::createX11SharedPixmap()
{
    // some pseudo-random dimensions. not really important for these tests
    m_pixmapHandle = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(),
                             854, 70, QX11Info::appDepth());
    QApplication::syncX();
}

void Ut_MStatusBarView::freeX11SharedPixmap()
{
    if (m_pixmapHandle) {
        XFreePixmap(QX11Info::display(), m_pixmapHandle);
        m_pixmapHandle = 0;
    }
}
#endif

QTEST_APPLESS_MAIN(Ut_MStatusBarView)
