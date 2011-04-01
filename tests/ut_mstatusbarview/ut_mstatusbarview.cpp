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

#include <mstatusbar.h>
#include "mstatusbar_p.h"
#include "ut_mstatusbarview.h"
#include "mstatusbar_p.h"

#ifdef HAVE_DBUS
#include <mdbusinterface.h>
#endif

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xdamage.h>
#endif //HAVE_XDAMAGE

#define SWIPE_THRESHOLD 30
#define SCENE_WIDTH 100

// an arbitrary point
#define START_POINT QPointF(15.20, 2.10)

bool gPreviousFilterCalled;
static bool previousFilter(void *)
{
    gPreviousFilterCalled = true;
    return false;
}

#ifdef HAVE_DBUS
// MDBusInterface stubs
MDBusInterface::MDBusInterface(const QString &service, const QString &path, const char *interface,
                             const QDBusConnection &connection, QObject *parent) :
    QDBusAbstractInterface(service, path, interface, connection, parent)
{
}

QDBus::CallMode gDBusInterfaceCallModeValue;
QString gDBusInterfaceCallMethodValue;
QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode mode, const QString &method, const QVariant &, const QVariant &,
                                  const QVariant &, const QVariant &, const QVariant & , const QVariant &,
                                  const QVariant &, const QVariant &)
{
    gDBusInterfaceCallModeValue = mode;
    gDBusInterfaceCallMethodValue = method;
    return QDBusMessage();
}
#endif

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
        app(0)
{
}

Qt::HANDLE g_pixmapHandle;
Atom gStatusBarPixmapAtom;
Atom gStatusBarPixmapPropertyType;
int gStatusBarPixmapPropertyFormat;
QByteArray gStatusBarPixmapPropertyData;
Status gStatusBarPixmapPropertyReturnStatus;

void* gReturnedPixmapPropertyData;
bool gReturnedPixmapPropertyDataFreed;

void initStatusBarPixmapProperty(Atom actualType, int actualFormat, quint32 data, int status)
{
    gStatusBarPixmapPropertyType = actualType;
    gStatusBarPixmapPropertyFormat = actualFormat;
    gStatusBarPixmapPropertyData = QByteArray((const char*)&data, 4);
    gStatusBarPixmapPropertyReturnStatus = status;
}

Atom gStatusBarPropertyWindowAtom;
Atom gStatusBarPropertyWindowPropertyType;
int gStatusBarPropertyWindowPropertyFormat;
Window gStatusBarPropertyWindowPropertyData;
Status gStatusBarPropertyWindowPropertyReturnStatus;

void* gReturnedPropertyWindowPropertyData;
bool gReturnedPropertyWindowPropertyDataFreed;

void initStatusBarPropertyWindowProperty(Atom actualType, int actualFormat, Window data, int status)
{
    gStatusBarPropertyWindowPropertyType = actualType;
    gStatusBarPropertyWindowPropertyFormat = actualFormat;
    gStatusBarPropertyWindowPropertyData = data;
    gStatusBarPropertyWindowPropertyReturnStatus = status;
}

void resetXPropertyStubData()
{
    gStatusBarPixmapPropertyType = 0;
    gStatusBarPixmapPropertyFormat = 0;
    gStatusBarPixmapPropertyData.clear();
    gStatusBarPixmapPropertyReturnStatus = 0;

    gStatusBarPropertyWindowPropertyType = 0;
    gStatusBarPropertyWindowPropertyFormat = 0;
    gStatusBarPropertyWindowPropertyData = 0;
    gStatusBarPropertyWindowPropertyReturnStatus = 0;
}

int (*RealXGetWindowProperty)(Display *, Window, Atom, long, long, Bool, Atom, Atom*, int*, unsigned long *, unsigned long *, unsigned char **);

int XGetWindowProperty(
    Display* d,
    Window w,
    Atom propertyAtom,
    long offset,
    long length,
    Bool delete_property,
    Atom req_type,
    Atom *actualType,
    int *actualFormat,
    unsigned long *nitems,
    unsigned long *bytes_after,
    unsigned char **data)
{
    if (propertyAtom == gStatusBarPropertyWindowAtom) {
        *actualType = gStatusBarPropertyWindowPropertyType;
        *actualFormat = gStatusBarPropertyWindowPropertyFormat;
        *data = (unsigned char*)&gStatusBarPropertyWindowPropertyData;
        *nitems = 1;
        *bytes_after = 0;

        gReturnedPropertyWindowPropertyData = *data;
        gReturnedPropertyWindowPropertyDataFreed = false;

        return gStatusBarPixmapPropertyReturnStatus;
    } else if (propertyAtom == gStatusBarPixmapAtom) {
        *actualType = gStatusBarPixmapPropertyType;
        *actualFormat = gStatusBarPixmapPropertyFormat;
        *data = (unsigned char*)gStatusBarPixmapPropertyData.data();
        *nitems = 1;
        *bytes_after = 0;

        gReturnedPixmapPropertyData = *data;
        gReturnedPixmapPropertyDataFreed = false;

        return gStatusBarPixmapPropertyReturnStatus;
    } else {
        return (*RealXGetWindowProperty)(d, w, propertyAtom,
                                               offset, length, delete_property, req_type,
                                               actualType, actualFormat, nitems,
                                               bytes_after, data);
    }
}

int (*RealXFree)(void *);

int XFree(void *data)
{
    if (data == gReturnedPropertyWindowPropertyData) {
        gReturnedPropertyWindowPropertyDataFreed = true;
        return Success;
    } else if (data == gReturnedPixmapPropertyData) {
        gReturnedPixmapPropertyDataFreed = true;
        return Success;
    } else {
        return (*RealXFree)(data);
    }
}


void Ut_MStatusBarView::initTestCase()
{
#ifdef Q_WS_X11
    QLibrary libX11("/usr/lib/libX11.so.6");

    RealXGetWindowProperty =
        reinterpret_cast<int (*)(Display *, Window, Atom,
                 long, long, Bool,
                 Atom, Atom*,
                 int*, unsigned long *,
                 unsigned long *, unsigned char **)>(libX11.resolve("XGetWindowProperty"));

    RealXFree = reinterpret_cast<int (*)(void*)>(libX11.resolve("XFree"));
#endif

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mstatusbarview" };
    app = new MApplication(argc, app_name);

#ifdef Q_WS_X11
    createX11SharedPixmap();

    gStatusBarPixmapAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PIXMAP", False);
    gStatusBarPropertyWindowAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW", False);
#endif

    QAbstractEventDispatcher::instance()->setEventFilter(previousFilter);
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
    resetXPropertyStubData();

    gPreviousFilterCalled = false;
#ifdef HAVE_DBUS
    // Just initialize to an invalid value
    gDBusInterfaceCallModeValue = QDBus::AutoDetect;
    gDBusInterfaceCallMethodValue = QString();
#endif
}

void Ut_MStatusBarView::cleanup()
{
    hapticsDone = false;

    delete m_statusbar;
    m_statusbar = 0;
}

#ifdef HAVE_DBUS
void Ut_MStatusBarView::testWhenMouseMovesAboveThresholdStatusIndicatorMenuAppears()
{
    initHelper();
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD + 1));
    QCOMPARE(gDBusInterfaceCallModeValue, QDBus::NoBlock);
    QCOMPARE(gDBusInterfaceCallMethodValue, QString("open"));
}

void Ut_MStatusBarView::testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear()
{
    initHelper();
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD - 1));
    QVERIFY(gDBusInterfaceCallModeValue != QDBus::NoBlock);
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
    QVERIFY(gDBusInterfaceCallModeValue != QDBus::NoBlock);
}

void Ut_MStatusBarView::testWhenUsingTapSwipeDoesNothing()
{
    initHelper();
    m_subject->modifiableStyle()->setUseSwipeGesture(false);
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD - 1));
    QVERIFY(gDBusInterfaceCallModeValue != QDBus::NoBlock);
}

void Ut_MStatusBarView::testTapFunctionality()
{
    initHelper();
    m_subject->modifiableStyle()->setUseSwipeGesture(false);

    // Test that a tap where a release is out of bounds is not recognised
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT + QPointF(SCENE_WIDTH, SCENE_WIDTH) * 2);
    QVERIFY(gDBusInterfaceCallModeValue != QDBus::NoBlock);

    // Test that a tap where a release is within bounds is recognised
    mouseDownWorker(START_POINT);
    mouseUpWorker(START_POINT + QPointF(5,5));
    QCOMPARE(gDBusInterfaceCallModeValue, QDBus::NoBlock);
    QCOMPARE(gDBusInterfaceCallMethodValue, QString("open"));
}

void Ut_MStatusBarView::testStatusIndicatorMenuDisabling()
{
    initHelper();
    m_subject->modifiableStyle()->setEnableStatusIndicatorMenu(false);
    mouseDownWorker(START_POINT);
    mouseMoveWorker(START_POINT + QPointF(0, SWIPE_THRESHOLD + 1));
    QVERIFY(gDBusInterfaceCallModeValue != QDBus::NoBlock);
}
#endif

void Ut_MStatusBarView::testPressDimming()
{
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, 1, Success);
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

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

#ifdef Q_WS_X11

void Ut_MStatusBarView::testGettingPropertyWindowId()
{
    Window expectedWindow(69);
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, expectedWindow, Success);

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    QCOMPARE(m_subject->statusBarPropertyWindowId, expectedWindow);
    QVERIFY(gReturnedPropertyWindowPropertyDataFreed);
}

void Ut_MStatusBarView::testGettingSharedPixmapHandle_data()
{
    QTest::addColumn<quint32>("pixmapHandle");
    QTest::addColumn<Atom>("propertyType");
    QTest::addColumn<int>("propertyFormat");
    QTest::addColumn<int>("returnStatus");
    QTest::addColumn<bool>("shouldGetPixmapHandle");

    QTest::newRow("Pixmap handle received successfully") << (quint32)g_pixmapHandle << XA_PIXMAP << 32 << Success << true;
    QTest::newRow("Getting pixmap handle failed") << (quint32)0 << (ulong)None << 0 << Success << false;
    QTest::newRow("Pixmap handle received successfully but handle is 0") << (quint32)0 << XA_PIXMAP << 32 << Success << false;
}

void Ut_MStatusBarView::testGettingSharedPixmapHandle()
{
    QFETCH(quint32, pixmapHandle);
    QFETCH(Atom, propertyType);
    QFETCH(int, propertyFormat);
    QFETCH(int, returnStatus);
    QFETCH(bool, shouldGetPixmapHandle);

    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, 1, Success);
    initStatusBarPixmapProperty(propertyType, propertyFormat, pixmapHandle, returnStatus);

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    if (shouldGetPixmapHandle) {
        QCOMPARE(m_subject->sharedPixmap.isNull(), false);
        QCOMPARE(m_subject->sharedPixmap.handle(), g_pixmapHandle);
    } else {
        QCOMPARE(m_subject->sharedPixmap.isNull(), true);
    }
    QCOMPARE(gReturnedPixmapPropertyDataFreed, returnStatus == Success);
}

void Ut_MStatusBarView::testThatPropertyNewValueOfXPropertyNotifyEventFetchesPixmapHandleAndPropertyDeleteRemovesPixmapHandle()
{
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, 1234, Success);
    // Pixmap property is not received during initialization
    initStatusBarPixmapProperty(None, 0, 0, Success);

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    XEvent xevent;
    xevent.type = PropertyNotify;
    xevent.xproperty.atom = gStatusBarPixmapAtom;
    xevent.xproperty.state = PropertyNewValue;
    xevent.xproperty.window = 1234;
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    void *message = static_cast<void*>(&xevent);
    MStatusBarEventListener::xWindowPropertyEventFilter(message);

    QCOMPARE(m_subject->sharedPixmap.isNull(), false);
    QCOMPARE(m_subject->sharedPixmap.handle(), g_pixmapHandle);

    // Verify that previous filter was also called
    QVERIFY(gPreviousFilterCalled);

    //PropertyDelete state
    xevent.xproperty.state = PropertyDelete;
    message = static_cast<void*>(&xevent);
    MStatusBarEventListener::xWindowPropertyEventFilter(message);

    QCOMPARE(m_subject->sharedPixmap.isNull(), true);
}

void Ut_MStatusBarView::testSharedPixmapHandleProviderOffline()
{
    // Create status bar with valid pixmap handle
    Window propertyWindow(69);
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, propertyWindow, Success);
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);
    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    // Destroy property window
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, BadWindow);
    XEvent xevent;
    xevent.type = DestroyNotify;
    xevent.xdestroywindow.window = propertyWindow;
    void *message = static_cast<void*>(&xevent);
    MStatusBarEventListener::xWindowPropertyEventFilter(message);
    // Verify property poll timer
    QVERIFY(disconnect(&m_subject->propertyWindowPollTimer, SIGNAL(timeout()), m_subject, SLOT(onPropertyWindowPollTimerTimeout())));
    QVERIFY(m_subject->propertyWindowPollTimer.isActive());
    // Verify offline state
    QVERIFY(m_subject->sharedPixmap.isNull());

    // First timeout while provider still offline
    m_subject->onPropertyWindowPollTimerTimeout();
    QVERIFY(m_subject->propertyWindowPollTimer.isActive());
    QCOMPARE(m_subject->sharedPixmap.isNull(), true);

    // Second timeout after provider back online
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);
    m_subject->onPropertyWindowPollTimerTimeout();
    QVERIFY(!m_subject->propertyWindowPollTimer.isActive());
    QCOMPARE(m_subject->sharedPixmap.isNull(), false);
}

void Ut_MStatusBarView::testEventFilterIsSetCorrectly()
{
    QAbstractEventDispatcher::EventFilter currentEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(NULL);
    QVERIFY(currentEventFilter == MStatusBarEventListener::xWindowPropertyEventFilter);

    // Restore the filter for rest of the tests
    QAbstractEventDispatcher::instance()->setEventFilter(MStatusBarEventListener::xWindowPropertyEventFilter);
}

void Ut_MStatusBarView::testEventFilterWhenNoStatusBarInstance()
{
    cleanup();

    XEvent xevent;
    xevent.type = PropertyNotify;
    xevent.xproperty.atom = gStatusBarPixmapAtom;
    xevent.xproperty.state = PropertyNewValue;
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);
    void *message = static_cast<void*>(&xevent);
    // Verify that we can call the filter even without statusbar instance
    MStatusBarEventListener::xWindowPropertyEventFilter(message);
    // Verify that previous filter was called
    QVERIFY(gPreviousFilterCalled);

    // Verify that the filter is still correct
    QAbstractEventDispatcher::EventFilter currentEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(NULL);
    QVERIFY(currentEventFilter == MStatusBarEventListener::xWindowPropertyEventFilter);
}

#endif // Q_WS_X11

#ifdef HAVE_XDAMAGE
void Ut_MStatusBarView::testTrackXDamageEventsForSharedPixmapWhenEnterDisplay()
{
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, 1, Success);
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    // we don't even have a window
    QCOMPARE(m_subject->isOnDisplay, false);
    QCOMPARE(m_subject->isInSwitcher, false);

    // give it a shared pixmap
    XEvent xevent;
    xevent.type = PropertyNotify;
    xevent.xproperty.atom = gStatusBarPixmapAtom;
    xevent.xproperty.state = PropertyNewValue;

    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    void *message = static_cast<void*>(&xevent);
    MStatusBarEventListener::xWindowPropertyEventFilter(message);

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
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, 1, Success);
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    // give it a shared pixmap
    XEvent xevent;
    xevent.type = PropertyNotify;
    xevent.xproperty.atom = gStatusBarPixmapAtom;
    xevent.xproperty.state = PropertyNewValue;

    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    void *message = static_cast<void*>(&xevent);
    MStatusBarEventListener::xWindowPropertyEventFilter(message);

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
    initStatusBarPropertyWindowProperty(XA_WINDOW, 32, 1, Success);
    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    m_statusbar = new MStatusBar();
    m_subject = new TestMStatusBarView(m_statusbar);
    m_statusbar->setView(m_subject);

    // give it a shared pixmap
    XEvent xevent;
    xevent.type = PropertyNotify;
    xevent.xproperty.atom = gStatusBarPixmapAtom;
    xevent.xproperty.state = PropertyNewValue;

    initStatusBarPixmapProperty(XA_PIXMAP, 32, g_pixmapHandle, Success);

    void *message = static_cast<void*>(&xevent);
    MStatusBarEventListener::xWindowPropertyEventFilter(message);

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
    g_pixmapHandle = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(),
                             854, 70, QX11Info::appDepth());
    QApplication::syncX();
}

void Ut_MStatusBarView::freeX11SharedPixmap()
{
    if (g_pixmapHandle) {
        XFreePixmap(QX11Info::display(), g_pixmapHandle);
        g_pixmapHandle = 0;
    }
}
#endif

QTEST_APPLESS_MAIN(Ut_MStatusBarView)
