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
#include <QGraphicsSceneMouseEvent>
#include <QAbstractEventDispatcher>
#include <QTimer>
#include "mviewconstants.h"
#include "mstatusbarview.h"
#include <mstatusbar.h>
#include <mapplication.h>
#include <mscenemanager.h>
#include <mscene.h>

#ifdef HAVE_DBUS
#include <mdbusinterface.h>
#endif

#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xdamage.h>
#endif //HAVE_XDAMAGE

#ifdef Q_WS_X11
#include <X11/Xatom.h>

#define PROPERTY_WINDOW_INITIAL_POLL_TIMEOUT 3000
#define PROPERTY_WINDOW_MAX_POLL_TIMEOUT 300000 // five minutes

Atom MStatusBarView::propertyWindowAtom = 0;
Atom MStatusBarView::pixmapHandleAtom = 0;
QAbstractEventDispatcher::EventFilter previousEventFilter = NULL;
static QSharedPointer<MStatusBarEventListener> eventListenerInstance;
static MStatusBarView *statusBarViewInstance = NULL;

MStatusBarEventListener::MStatusBarEventListener()
{
    // Setup event filter to follow _MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW property notify events
    if (previousEventFilter == NULL) {
        previousEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(xWindowPropertyEventFilter);
    }
}

MStatusBarEventListener::~MStatusBarEventListener()
{
        QAbstractEventDispatcher *dispatcher = QAbstractEventDispatcher::instance();
        if (dispatcher) {
            if (previousEventFilter) {
                dispatcher->setEventFilter(previousEventFilter);
                previousEventFilter = NULL;
            } else {
                dispatcher->setEventFilter(NULL);
            }
        }
}

bool MStatusBarEventListener::xWindowPropertyEventFilter(void *message)
{
    XEvent *xevent = static_cast<XEvent*>(message);

    if (statusBarViewInstance != NULL) {
        if (xevent->type == PropertyNotify) {
            statusBarViewInstance->handleXPropertyNotify(xevent->xproperty);
        } else if (xevent->type == DestroyNotify) {
            statusBarViewInstance->handleXDestroyNotify(xevent->xdestroywindow);
        }
    }

    bool handled = false;
    if (previousEventFilter && previousEventFilter != xWindowPropertyEventFilter) {
        handled = previousEventFilter(message);
    }

    return handled;
}

void MStatusBarView::handleXDestroyNotify(const XDestroyWindowEvent &destroyWindowEvent)
{
    if (statusBarPropertyWindowId == destroyWindowEvent.window) {
        handlePixmapProviderOffline();
    }
}

void MStatusBarView::handleXPropertyNotify(const XPropertyEvent &propertyEvent)
{
    if (propertyEvent.window != statusBarPropertyWindowId
        || propertyEvent.atom != pixmapHandleAtom) {
        // we are only interested in changes in the pixmapHandle property of the status
        // bar property window.
        return;
    }

    switch (propertyEvent.state) {
    case  PropertyNewValue:
        updateStatusBarSharedPixmapHandle();
    break;
    case PropertyDelete:
        handlePixmapProviderOffline();
    break;
    default:
    break;
    }
}

Window MStatusBarView::fetchStatusBarPropertyWindowId()
{
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytesAfter = 0;
    unsigned char *data = NULL;

    int status = XGetWindowProperty(QX11Info::display(),
                                    QX11Info::appRootWindow(),
                                    propertyWindowAtom,
                                    0,
                                    1,
                                    False,
                                    XA_WINDOW,
                                    &actualType,
                                    &actualFormat,
                                    &nitems,
                                    &bytesAfter,
                                    &data);

    Window window(0);
    if (status == Success && data != None) {
        window = *(Window *)data;

        XFree(data);
    }

    return window;
}

bool MStatusBarView::updateSharedPixmapHandleFromPropertyWindow(const Window &window)
{
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytesAfter = 0;

     union {
        unsigned char* asUChar;
        unsigned long* asULong;
    } data = {0};

    int status = XGetWindowProperty(QX11Info::display(),
                                    window,
                                    pixmapHandleAtom,
                                    0,
                                    1,
                                    False,
                                    XA_PIXMAP,
                                    &actualType,
                                    &actualFormat,
                                    &nitems,
                                    &bytesAfter,
                                    &data.asUChar);

    bool success = false;
    if (status == Success) {
        if (actualType == XA_PIXMAP && actualFormat == 32 && data.asULong[0] != 0) {
            handleSharedPixmapHandleReceived(data.asULong[0]);
            success = true;
        }
        XFree(data.asUChar);
    }

    return success;
}
#endif //Q_WS_X11

namespace{
    const qreal SharedPixmapHeight = 36;
}

MStatusBarView::MStatusBarView(MStatusBar *controller) :
    MSceneWindowView(controller),
    controller(controller),
    pressDown(false)
#ifdef Q_WS_X11
    , isOnDisplay(false)
    , isInSwitcher(false)
#endif
{
#ifdef Q_WS_X11
    pixmapDamage = 0;

    MApplication *mApplication = static_cast<MApplication *>(QCoreApplication::instance());
    connect(mApplication, SIGNAL(damageEvent(Qt::HANDLE&, short&, short&, unsigned short&, unsigned short&)),
            SLOT(handlePixmapDamageEvent(Qt::HANDLE&, short&, short&, unsigned short&, unsigned short&)));
    connect(controller, SIGNAL(displayExited()),
            this, SLOT(handleDisplayExited()));
    connect(controller, SIGNAL(displayEntered()),
            this, SLOT(handleDisplayEntered()));

    if (controller->scene() && !controller->scene()->views().isEmpty()) {
        MWindow* win = qobject_cast<MWindow*>(controller->scene()->views().at(0));
        if (win) {
            isInSwitcher = win->isInSwitcher();
            connect(win, SIGNAL(switcherEntered()), this, SLOT(handleSwitcherEntered()));
            connect(win, SIGNAL(switcherExited()), this, SLOT(handleSwitcherExited()));
        }
    }

    statusBarViewInstance = this;
    propertyWindowAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW", False);
    pixmapHandleAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PIXMAP", False);

    // Create event listener on first statusbar creation. To assure correct QAbstractEventDispatcher filter
    // handling the listener has to be destroyed (restoring previous filter) only when application is destroyed
    // (hence the use of global static shared pointer).
    if (eventListenerInstance.isNull()) {
        eventListenerInstance = QSharedPointer<MStatusBarEventListener>(new MStatusBarEventListener);
    }

    connect(&propertyWindowPollTimer, SIGNAL(timeout()), this, SLOT(onPropertyWindowPollTimerTimeout()));
    propertyWindowPollTimer.setInterval(PROPERTY_WINDOW_INITIAL_POLL_TIMEOUT);

    updateStatusBarSharedPixmapHandle();
#endif // Q_WS_X11
}

MStatusBarView::~MStatusBarView()
{
#ifdef Q_WS_X11
    destroyXDamageForSharedPixmap();

    if (this == statusBarViewInstance) {
        statusBarViewInstance = NULL;
    }
#endif // Q_WS_X11
}

#ifdef Q_WS_X11
void MStatusBarView::onPropertyWindowPollTimerTimeout()
{
    int currentInterval = propertyWindowPollTimer.interval();
    // Increase the interval as long as it's under maximum limit
    if (currentInterval < PROPERTY_WINDOW_MAX_POLL_TIMEOUT) {
        propertyWindowPollTimer.setInterval(currentInterval*2);
    } else {
        // Stop timer when maximum interval reached
        propertyWindowPollTimer.stop();
    }

    updateStatusBarSharedPixmapHandle();
}

void MStatusBarView::updateStatusBarSharedPixmapHandle()
{
    // Get rid of the current shared pixmap, if any
    if (!sharedPixmap.isNull()) {
        destroyXDamageForSharedPixmap();
        sharedPixmap = QPixmap();
    }

    // Get the status bar property window id from the root window property
    statusBarPropertyWindowId = fetchStatusBarPropertyWindowId();

    if (statusBarPropertyWindowId > 0) {
        // We want to be notified when its properties changes and when it gets destroyed.
        XSelectInput(QX11Info::display(), statusBarPropertyWindowId,
                     PropertyChangeMask | StructureNotifyMask);
    }

    // Update the shared pixmap handle from the status bar property window property
    if (statusBarPropertyWindowId > 0 && updateSharedPixmapHandleFromPropertyWindow(statusBarPropertyWindowId)) {
        propertyWindowPollTimer.stop();
    } else {
        // start polling if we are not doing it already.
        if (!propertyWindowPollTimer.isActive()
            && propertyWindowPollTimer.interval() < PROPERTY_WINDOW_MAX_POLL_TIMEOUT) {
            propertyWindowPollTimer.start();
        }
    }
}
#endif // Q_WS_X11

void MStatusBarView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

#ifdef Q_WS_X11
    if (sharedPixmap.isNull() || (controller->sceneManager() == 0)) {
        return;
    }

    QRectF sourceRect;

    sourceRect.setX(0);
    sourceRect.setY(controller->sceneManager()->orientation() == M::Landscape ? 0 : SharedPixmapHeight);
    sourceRect.setWidth(size().width());
    sourceRect.setHeight(SharedPixmapHeight);

    // provider can die under mysterious circumstances so sharedPixmap can be invalid
    // Catch the bad_alloc in case the shared pixmap have been deleted without PropertyDelete notify
    QT_TRY {
        painter->save();
        painter->setClipRect(0, 0, size().width(), sharedPixmap.height()/2);
        painter->drawPixmap(QPointF(0.0, 0.0), sharedPixmap, sourceRect);
        painter->restore();
    } QT_CATCH (...) {
        mWarning("MStatusBarView") << "drawContents: Cannot draw sharedPixmap.";
        const_cast<MStatusBarView*>(this)->destroyXDamageForSharedPixmap();
        const_cast<MStatusBarView*>(this)->sharedPixmap = QPixmap();
    }

    if (pressDown) {
        painter->save();
        painter->setOpacity(style()->pressDimFactor());
        painter->fillRect(QRectF(QPointF(0.0, 0.0), sourceRect.size()), Qt::black);
        painter->restore();
    }

#else
    Q_UNUSED(painter);
#endif // Q_WS_X11
}

void MStatusBarView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    firstPos = event->pos();
    playHapticsFeedback();

    if (pressDown)
        return;

    pressDown = true;
    update();
}

void MStatusBarView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (style()->useSwipeGesture()) {
        if(firstPos.y()+ style()->swipeThreshold() < event->pos().y()) {
            showStatusIndicatorMenu();
        }
    } else {
        QPointF touch = event->scenePos();
        // shape includes the reactive margins
        QRectF rect = controller->mapToScene(controller->shape()).boundingRect();
        rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                    M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
        if (rect.contains(touch)) {
            if (!pressDown) {
                pressDown = true;
                update();
            }
        } else {
            if (pressDown) {
                pressDown = false;
                update();
            }
        }
    }
}

void MStatusBarView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!pressDown)
        return;

    pressDown = false;
    update();

    if (style()->useSwipeGesture()) {
        return;
    }

    QPointF touch = event->scenePos();
    // shape includes the reactive margins
    QRectF rect = controller->mapToScene(controller->shape()).boundingRect();
    rect.adjust(-M_RELEASE_MISS_DELTA, -M_RELEASE_MISS_DELTA,
                M_RELEASE_MISS_DELTA, M_RELEASE_MISS_DELTA);
    if(rect.contains(touch)) {
        showStatusIndicatorMenu();
    }
}

#ifdef Q_WS_X11
bool MStatusBarView::shouldStayUpToDate()
{
    return isOnDisplay && !isInSwitcher;
}

void MStatusBarView::updateXDamageForSharedPixmap()
{
    if (shouldStayUpToDate()) {
        if (!pixmapDamage) {
            if (!sharedPixmap.isNull()) {
                setupXDamageForSharedPixmap();
            } else {
                mWarning("MStatusBarView")
                    << "Couldn't track XDamage events since there's no shared pixmap.";
            }
        }
    } else {
        destroyXDamageForSharedPixmap();
    }
}

void MStatusBarView::setupXDamageForSharedPixmap()
{
    Q_ASSERT(!sharedPixmap.isNull());
#ifdef HAVE_XDAMAGE
    pixmapDamage = XDamageCreate(QX11Info::display(), sharedPixmap.handle(), XDamageReportRawRectangles);
#endif //HAVE_XDAMAGE
}

void MStatusBarView::destroyXDamageForSharedPixmap()
{
#ifdef HAVE_XDAMAGE
    if (pixmapDamage) {
        XDamageDestroy(QX11Info::display(), pixmapDamage);
        pixmapDamage = 0;
    }
#endif //HAVE_XDAMAGE
}

void MStatusBarView::handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                             unsigned short &width, unsigned short &height)
{
    if (damage == pixmapDamage) {
        Q_ASSERT(shouldStayUpToDate());
        controller->update(x, y, width, height);
    }
}

void MStatusBarView::handleDisplayEntered()
{
    isOnDisplay = true;
    updateXDamageForSharedPixmap();
}

void MStatusBarView::handleDisplayExited()
{
    isOnDisplay = false;
    updateXDamageForSharedPixmap();
}

void MStatusBarView::handleSharedPixmapHandleReceived(quint32 handle)
{
    sharedPixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);

    destroyXDamageForSharedPixmap();
    updateXDamageForSharedPixmap();

    if (shouldStayUpToDate()) {
        update();
    }
}

void MStatusBarView::handlePixmapProviderOffline()
{
    destroyXDamageForSharedPixmap();
    sharedPixmap = QPixmap();

    // reset interval as we're starting a new polling session
    propertyWindowPollTimer.setInterval(PROPERTY_WINDOW_INITIAL_POLL_TIMEOUT);
    propertyWindowPollTimer.start();
}

void MStatusBarView::handleSwitcherEntered()
{
    isInSwitcher = true;
    updateXDamageForSharedPixmap();
}

void MStatusBarView::handleSwitcherExited()
{
    isInSwitcher = false;
    updateXDamageForSharedPixmap();
}
#endif // Q_WS_X11

void MStatusBarView::showStatusIndicatorMenu()
{
#ifdef HAVE_DBUS
    if (style()->enableStatusIndicatorMenu()) {
        MDBusInterface interface("com.meego.core.MStatusIndicatorMenu",
                                            "/statusindicatormenu",
                                            "com.meego.core.MStatusIndicatorMenu",
                                            QDBusConnection::sessionBus());
        interface.call(QDBus::NoBlock, "open");
    }
#endif
}

void MStatusBarView::playHapticsFeedback()
{
    if (style()->enableStatusIndicatorMenu()) {
        style()->pressFeedback().play();
    }
}

M_REGISTER_VIEW_NEW(MStatusBarView, MStatusBar)
