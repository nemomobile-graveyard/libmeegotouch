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

#include <qglobal.h>
#include <cstdlib>
#include <QCoreApplication>

#include "mapplication.h"
#include "mcomponentdata.h"
#include "mondisplaychangeevent.h"
#include "mapplicationwindow.h"
#include "mwindow_p.h"
#include <MDebug>
#include "mapplication_p.h"

/* Must be last, as it conflicts with some of the Qt defined types */
#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif //Q_WS_X11

#ifdef HAVE_XDAMAGE
#include <X11/extensions/Xdamage.h>
#endif //HAVE_XDAMAGE

MApplicationPrivate::MApplicationPrivate():
        componentData(NULL),
        xDamageEventBase(0),
        xDamageErrorBase(0),
        q_ptr(NULL)
{
#ifdef HAVE_XDAMAGE
    XDamageQueryExtension(QX11Info::display(), &xDamageEventBase, &xDamageErrorBase);
#endif //HAVE_XDAMAGE
}

MApplicationPrivate::~MApplicationPrivate()
{
    delete componentData;
}
#ifdef Q_WS_X11
void MApplicationPrivate::setWindowVisibility(Window window, bool visible)
{
    Q_FOREACH(MWindow * win, MApplication::windows()) {
        if (win && win->winId() == window) {
            MOnDisplayChangeEvent ev(visible, QRectF(QPointF(0, 0), win->visibleSceneSize()));
            MApplication::instance()->sendEvent(win, &ev);
        }
    }
}

int MApplicationPrivate::handleXError(Display *, XErrorEvent *)
{
    return 0;
}


void MApplicationPrivate::setX11PrestartPropertyForWindows(bool set)
{
    Q_FOREACH(MWindow * win, MApplication::windows()) {
        win->d_ptr->setX11PrestartProperty(set);
    }
}

void MApplicationPrivate::removeWindowsFromSwitcher(bool remove)
{
    Q_FOREACH(MWindow * win, MApplication::windows()) {
        removeWindowFromSwitcher(win->winId(), remove);
    }
}

void MApplicationPrivate::removeWindowFromSwitcher(Window window, bool remove)
{
    Display *dpy  = QX11Info::display();
    Atom stateAtom = XInternAtom(dpy, "_NET_WM_STATE", True);
    if (stateAtom != None) {
        Atom skipAtom = XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", True);
        if (remove) {
            XChangeProperty(dpy, window, stateAtom,
                            XA_ATOM, 32, PropModeAppend,
                            reinterpret_cast<unsigned char *>(&skipAtom), 1);
        } else {
            XEvent ev;
            memset(&ev, 0, sizeof(ev));
            ev.xclient.type         = ClientMessage;
            ev.xclient.display      = dpy;
            ev.xclient.window       = window;
            ev.xclient.message_type = XInternAtom(dpy, "_NET_WM_STATE", False);
            ev.xclient.format       = 32;
            ev.xclient.data.l[0]    = 0;
            ev.xclient.data.l[1]    = skipAtom;
            ev.xclient.data.l[2]    = 0;

            XSendEvent(dpy, QX11Info::appRootWindow(), False, SubstructureRedirectMask | SubstructureNotifyMask,
                       &ev);
            XSync(dpy, False);
        }
    }
}
#endif

MApplication *MApplication::instance()
{
    return qobject_cast<MApplication *>(QCoreApplication::instance());
}

MApplication::MApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
    : QApplication(argc, argv),
      d_ptr(new MApplicationPrivate)
{
    Q_D(MApplication);
    d->q_ptr = this;

    d->init(argc, argv, appIdentifier, service);
}

MApplication::MApplication(int &argc, char **argv, MApplicationService *service) :
    QApplication(argc, argv),
    d_ptr(new MApplicationPrivate)
{
    Q_D(MApplication);
    d->q_ptr = this;

    d->init(argc, argv, QString(), service);
}

void MApplicationPrivate::init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *newService)
{
    componentData = new MComponentData(argc, argv, appIdentifier, newService);
}

void MApplicationPrivate::releasePrestart()
{
    if (MApplication::isPrestarted()) {

        MComponentData::setPrestarted(false);

        // Call the virtual handler and emit the signal
        MApplication *mApp = MApplication::instance();
        if (mApp) {
            mApp->releasePrestart();
            emit mApp->prestartReleased();
        }

#ifdef Q_WS_X11
        // Ensure that windows are visible in the switcher again
        if (prestartModeIsLazyShutdown()) {
            removeWindowsFromSwitcher(false);
        }
        setX11PrestartPropertyForWindows(false);
#endif
    }
}

void MApplicationPrivate::restorePrestart()
{
    if (!MApplication::isPrestarted() && prestartModeIsLazyShutdown()) {

        MComponentData::setPrestarted(true);

        // Call the virtual handler and emit the signal
        MApplication *mApp = MApplication::instance();
        if (mApp) {
            mApp->restorePrestart();
            emit mApp->prestartRestored();
        }

#ifdef Q_WS_X11
        // Explicitly remove windows from the switcher because they
        // are hidden but should look like closed
        removeWindowsFromSwitcher(true);
        setX11PrestartPropertyForWindows(true);
#endif
    }
}

void MApplicationPrivate::hideWindows()
{
    Q_FOREACH(MWindow * win, MApplication::windows()) {
        win->hide();
        win->lower();
    }
}

void MApplication::setPrestarted(bool enable)
{
    if (enable) {
        MApplicationPrivate::restorePrestart();
    } else {
        MApplicationPrivate::releasePrestart();
    }
}

bool MApplicationPrivate::prestartModeIsLazyShutdown()
{
    return MApplication::prestartMode() == M::LazyShutdown ||
            MApplication::prestartMode() == M::LazyShutdownMultiWindow;
}

bool MApplicationPrivate::prestartModeIsMultiWindowed()
{
    return MApplication::prestartMode() == M::TerminateOnCloseMultiWindow ||
            MApplication::prestartMode() == M::LazyShutdownMultiWindow;
}

MApplication::~MApplication()
{
    delete d_ptr;
}

QString MApplication::deviceName()
{
    return MComponentData::deviceName();
}

bool MApplication::softwareRendering()
{
    return MComponentData::softwareRendering();
}

bool MApplication::fullScreen()
{
    return MComponentData::fullScreen();
}

bool MApplication::showBoundingRect()
{
    return MComponentData::showBoundingRect();
}

bool MApplication::showFps()
{
    return MComponentData::showFps();
}

bool MApplication::logFps()
{
    return MComponentData::logFps();
}

bool MApplication::showSize()
{
    return MComponentData::showSize();
}

bool MApplication::showPosition()
{
    return MComponentData::showPosition();
}

bool MApplication::showMargins()
{
    return MComponentData::showMargins();
}

bool MApplication::showObjectNames()
{
    return MComponentData::showObjectNames();
}

bool MApplication::showCursor()
{
    return MComponentData::showCursor();
}

bool MApplication::emulateTwoFingerGestures()
{
    return MComponentData::emulateTwoFingerGestures();
}

void MApplication::setShowPosition(bool show)
{
    MComponentData::setShowPosition(show);
}

void MApplication::setShowSize(bool show)
{
    MComponentData::setShowSize(show);
}

void MApplication::setShowMargins(bool show)
{
    MComponentData::setShowMargins(show);
}

void MApplication::setShowObjectNames(bool show)
{
    MComponentData::setShowObjectNames(show);
}

void MApplication::setShowBoundingRect(bool show)
{
    MComponentData::setShowBoundingRect(show);
}

void MApplication::setShowFps(bool show)
{
    MComponentData::setShowFps(show);
}

void MApplication::setLogFps(bool show)
{
    MComponentData::setLogFps(show);
}

void MApplication::setShowCursor(bool show)
{
    MComponentData::setShowCursor(show);
}

MApplicationWindow *MApplication::activeApplicationWindow()
{
    return MComponentData::activeApplicationWindow();
}

MWindow *MApplication::activeWindow()
{
    return MComponentData::activeWindow();
}

QList<MWindow *> MApplication::windows()
{
    return MComponentData::windows();
}

MFeedbackPlayer *MApplication::feedbackPlayer()
{
    return MComponentData::feedbackPlayer();
}

bool MApplication::isLoadMInputContextEnabled()
{
    return MComponentData::isLoadMInputContextEnabled();
}

void MApplication::setLoadMInputContext(bool enable)
{
    MComponentData::setLoadMInputContext(enable);
}

#ifdef Q_WS_X11
bool MApplication::x11EventFilter(XEvent *event)
{
    Q_D(MApplication);

    if (event->type == VisibilityNotify) {
        XVisibilityEvent *xevent = (XVisibilityEvent *) event;

        switch (xevent->state) {
        case VisibilityFullyObscured:
            MApplicationPrivate::setWindowVisibility(xevent->window, false);
            break;
        case VisibilityUnobscured:
        case VisibilityPartiallyObscured:
            MApplicationPrivate::setWindowVisibility(xevent->window, true);
            break;
        default:
            break;
        }
    }
#ifdef HAVE_XDAMAGE
    else if (event->type == d->xDamageEventBase + XDamageNotify) {

        XDamageNotifyEvent *xevent = (XDamageNotifyEvent *) event;
        XserverRegion parts;
        XRectangle *rects;
        int n_rect;

        // It is possible that the Damage has already been destroyed so register an error handler to suppress X errors
        XErrorHandler errh = XSetErrorHandler(MApplicationPrivate::handleXError);

        // Get the damaged rectangles from the damage event
        parts = XFixesCreateRegion(QX11Info::display(), 0, 0);
        // TODO does subtracting 0 regions really make any sense?
        XDamageSubtract(QX11Info::display(), xevent->damage, None, parts);
        rects = XFixesFetchRegion(QX11Info::display(), parts, &n_rect);
        XFixesDestroyRegion(QX11Info::display(), parts);

        // Create separate damage event signals from the damaged rectangles
        for (int i = 0; i < n_rect; ++i)
            emit damageEvent(xevent->damage, rects[i].x, rects[i].y, rects[i].width, rects[i].height);

        // Free the rectangles and reset the original error handler
        XFree(rects);
        XSetErrorHandler(errh);

        return true;
    }
#else //HAVE_XDAMAGE
    Q_UNUSED(d);
#endif //HAVE_XDAMAGE

    return false;
}
#endif

QString MApplication::appName()
{
    return MComponentData::appName();
}

QString MApplication::binaryName()
{
    return MComponentData::binaryName();
}

void MApplicationPrivate::stdExit(int status)
{
    std::exit(status);
}

void MApplication::setPrestartMode(M::PrestartMode mode)
{
    // Set prestart mode only if the app was started with -prestart.
    // This way we can later check if the app was originally prestarted or not.
    // Prevent application from exiting after the last window has been closed if
    // lazy shutdown was set.

    if (MComponentData::prestarted()) {
        MComponentData::setPrestartMode(mode);
        if (MApplicationPrivate::prestartModeIsLazyShutdown()) {
            MApplication::setQuitOnLastWindowClosed(false);
        }
    }
}

M::PrestartMode MApplication::prestartMode()
{
    return MComponentData::prestartMode();
}

void MApplication::releasePrestart()
{
    // Show the window only if we are not in a multi-windowed mode.
    // In that case the user must re-implement this, because we don't know which
    // window needs to be shown.

    if (!MApplicationPrivate::prestartModeIsMultiWindowed()) {
        if (MApplication::activeWindow()) {
            MApplication::activeWindow()->show();
            MApplication::activeWindow()->activateWindow();
            MApplication::activeWindow()->raise();
        }
    }
}

void MApplication::restorePrestart()
{
    if (MApplicationPrivate::prestartModeIsLazyShutdown()) {
        MApplicationPrivate::hideWindows();
    }
}

bool MApplication::isPrestarted()
{
    return (MComponentData::prestartMode() != M::NoPrestart) && MComponentData::prestarted();
}

#ifdef __arm__
/*
 * When in "runfast" mode, single-precision VFP instructions execute in the
 * NEON pipeline. If using gcc, adding the flags -ffast-math -fno-math-errno,
 * and avoiding double precision is advisable, assuming results are still
 * accurate enough.
 */
static int set_runfast()
{
    int tmp;
    __asm__ volatile(
        "fmrx       %[tmp], fpscr\n\t"
        "orr        %[tmp], %[tmp], #(1 << 24)\n\t" /* flush-to-zero */
        "orr        %[tmp], %[tmp], #(1 << 25)\n\t" /* default NaN */
        "bic        %[tmp], %[tmp], #((1 << 15) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8))\n\t" /* clear exception bits */
        "fmxr       fpscr, %[tmp]\n\t"
        : [tmp] "=r"(tmp));
    return tmp;
}

/*
 * IEEE standard floating point calculation
 */
static int set_ieee()
{
    int tmp;
    __asm__ volatile(
        "fmrx       %[tmp], fpscr\n\t"
        "bic        %[tmp], %[tmp], #(1 << 24)\n\t" /* flush-to-zero */
        "bic        %[tmp], %[tmp], #(1 << 25)\n\t" /* default NaN */
        "fmxr       fpscr, %[tmp]\n\t"
        : [tmp] "=r"(tmp));
    return tmp;
}

void MApplication::fastFloatMath(bool val)
{
    if (val) {
        set_runfast();
    } else {
        set_ieee();
    }
}

#endif
