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

#include <qglobal.h>
#include <cstdlib>
#include <QCoreApplication>

#include "duiapplication.h"
#include "duicomponentdata.h"
#include "duiondisplaychangeevent.h"
#include "duiapplicationwindow.h"
#include <DuiDebug>
#include "duiapplication_p.h"

/* Must be last, as it conflicts with some of the Qt defined types */
#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xdamage.h>
#endif

DuiApplicationPrivate::DuiApplicationPrivate()
{
#ifdef Q_WS_X11
    XDamageQueryExtension(QX11Info::display(), &xDamageEventBase, &xDamageErrorBase);
#endif
}

DuiApplicationPrivate::~DuiApplicationPrivate()
{
    delete componentData;
}
#ifdef Q_WS_X11
void DuiApplicationPrivate::setWindowVisibility(Window window, bool visible)
{
    if (!DuiApplication::windows().empty()) {
        Q_FOREACH(DuiWindow * win, DuiApplication::windows()) {
            if (win && win->winId() == window) {
                DuiOnDisplayChangeEvent ev(visible, QRectF(QPointF(0, 0), win->visibleSceneSize()));
                DuiApplication::instance()->sendEvent(win, &ev);
            }
        }
    }
}

int DuiApplicationPrivate::handleXError(Display *, XErrorEvent *)
{
    return 0;
}

void DuiApplicationPrivate::removeWindowsFromSwitcher(bool remove)
{
    if (!DuiApplication::windows().empty()) {
        Display *dpy  = QX11Info::display();
        Atom stateAtom = XInternAtom(dpy, "_NET_WM_STATE", True);
        if (stateAtom != None) {
            Atom skipAtom = XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", True);
            if (remove) {
                Q_FOREACH(DuiWindow * win, DuiApplication::windows()) {
                    XChangeProperty(dpy, win->winId(), stateAtom,
                                    XA_ATOM, 32, PropModeAppend,
                                    reinterpret_cast<unsigned char *>(&skipAtom), 1);
                }
            } else {
                Q_FOREACH(DuiWindow * win, DuiApplication::windows()) {
                    XEvent ev;
                    memset(&ev, 0, sizeof(ev));
                    ev.xclient.type         = ClientMessage;
                    ev.xclient.display      = dpy;
                    ev.xclient.window       = win->winId();
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
    }
}
#endif

DuiApplication *DuiApplication::instance()
{
    return qobject_cast<DuiApplication *>(QCoreApplication::instance());
}

DuiApplication::DuiApplication(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service)
    : QApplication(argc, argv),
      d_ptr(new DuiApplicationPrivate)
{
    Q_D(DuiApplication);
    d->q_ptr = this;

    d->init(argc, argv, appIdentifier, service);
}

DuiApplication::DuiApplication(int &argc, char **argv, DuiApplicationService *service) :
    QApplication(argc, argv),
    d_ptr(new DuiApplicationPrivate)
{
    Q_D(DuiApplication);
    d->q_ptr = this;

    d->init(argc, argv, QString(), service);
}

void DuiApplicationPrivate::init(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *newService)
{
    componentData = new DuiComponentData(argc, argv, appIdentifier, newService);
    QObject::connect(componentData, SIGNAL(localeSettingsChanged()), qApp, SIGNAL(localeSettingsChanged()));
}

void DuiApplicationPrivate::releasePrestart()
{
    if (DuiApplication::prestartMode() != Dui::NoPrestart) {

        DuiComponentData::setPrestarted(false);

        // Call the virtual handler and emit the signal
        DuiApplication *duiApp = DuiApplication::instance();
        if (duiApp) {
            duiApp->releasePrestart();
            emit duiApp->prestartReleased();
        }

#ifdef Q_WS_X11
        // Ensure that windows are visible in the switcher again
        if (DuiApplication::prestartMode() == Dui::LazyShutdown) {
            removeWindowsFromSwitcher(false);
        }
#endif
    }
}

void DuiApplicationPrivate::restorePrestart()
{
    if (DuiApplication::prestartMode() == Dui::LazyShutdown) {

        DuiComponentData::setPrestarted(true);

        // Call the virtual handler and emit the signal
        DuiApplication *duiApp = DuiApplication::instance();
        if (duiApp) {
            duiApp->restorePrestart();
            emit duiApp->prestartRestored();
        }

#ifdef Q_WS_X11
        // Explicitly remove windows from the switcher because they
        // are hidden but should look like closed
        removeWindowsFromSwitcher(true);
#endif
    }
}

DuiApplication::~DuiApplication()
{
    delete d_ptr;
}

QString DuiApplication::deviceName()
{
    return DuiComponentData::deviceName();
}

bool DuiApplication::softwareRendering()
{
    return DuiComponentData::softwareRendering();
}

bool DuiApplication::fullScreen()
{
    return DuiComponentData::fullScreen();
}

bool DuiApplication::showBoundingRect()
{
    return DuiComponentData::showBoundingRect();
}

bool DuiApplication::showFps()
{
    return DuiComponentData::showFps();
}

bool DuiApplication::showSize()
{
    return DuiComponentData::showSize();
}

bool DuiApplication::showPosition()
{
    return DuiComponentData::showPosition();
}

bool DuiApplication::showMargins()
{
    return DuiComponentData::showMargins();
}

bool DuiApplication::showObjectNames()
{
    return DuiComponentData::showObjectNames();
}

bool DuiApplication::showCursor()
{
    return DuiComponentData::showCursor();
}

void DuiApplication::setShowPosition(bool show)
{
    DuiComponentData::setShowPosition(show);
}

void DuiApplication::setShowSize(bool show)
{
    DuiComponentData::setShowSize(show);
}

void DuiApplication::setShowMargins(bool show)
{
    DuiComponentData::setShowMargins(show);
}

void DuiApplication::setShowObjectNames(bool show)
{
    DuiComponentData::setShowObjectNames(show);
}

void DuiApplication::setShowBoundingRect(bool show)
{
    DuiComponentData::setShowBoundingRect(show);
}

void DuiApplication::setShowFps(bool show)
{
    DuiComponentData::setShowFps(show);
}

void DuiApplication::setShowCursor(bool show)
{
    DuiComponentData::setShowCursor(show);
}

DuiApplicationWindow *DuiApplication::activeApplicationWindow()
{
    return DuiComponentData::activeApplicationWindow();
}

DuiWindow *DuiApplication::activeWindow()
{
    return DuiComponentData::activeWindow();
}

QList<DuiWindow *> DuiApplication::windows()
{
    return DuiComponentData::windows();
}

DuiFeedbackPlayer *DuiApplication::feedbackPlayer()
{
    return DuiComponentData::feedbackPlayer();
}

bool DuiApplication::isLoadDuiInputContextEnabled()
{
    return DuiComponentData::isLoadDuiInputContextEnabled();
}

void DuiApplication::setLoadDuiInputContext(bool enable)
{
    DuiComponentData::setLoadDuiInputContext(enable);
}

#ifdef Q_WS_X11
bool DuiApplication::x11EventFilter(XEvent *event)
{
    Q_D(DuiApplication);

    if (event->type == VisibilityNotify) {
        XVisibilityEvent *xevent = (XVisibilityEvent *) event;

        switch (xevent->state) {
        case VisibilityFullyObscured:
            DuiApplicationPrivate::setWindowVisibility(xevent->window, false);
            break;
        case VisibilityUnobscured:
            DuiApplicationPrivate::setWindowVisibility(xevent->window, true);
            break;
        default:
            break;
        }
    } else if (event->type == d->xDamageEventBase + XDamageNotify) {
        XDamageNotifyEvent *xevent = (XDamageNotifyEvent *) event;
        XserverRegion parts;
        XRectangle *rects;
        int n_rect;

        // It is possible that the Damage has already been destroyed so register an error handler to suppress X errors
        XErrorHandler errh = XSetErrorHandler(DuiApplicationPrivate::handleXError);

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

    return false;
}
#endif

QString DuiApplication::appName()
{
    return DuiComponentData::appName();
}

QString DuiApplication::binaryName()
{
    return DuiComponentData::binaryName();
}

void DuiApplicationPrivate::stdExit(int status)
{
    std::exit(status);
}

void DuiApplication::setPrestartMode(Dui::PrestartMode mode)
{
    // Set prestart mode only if the app was started with -prestart.
    // This way we can later check if the app was originally prestarted or not.
    if (DuiComponentData::prestarted()) {
        if (mode == Dui::LazyShutdown) {
            DuiApplication::setQuitOnLastWindowClosed(false);
        }
        DuiComponentData::setPrestartMode(mode);
    }
}

Dui::PrestartMode DuiApplication::prestartMode()
{
    return DuiComponentData::prestartMode();
}

void DuiApplication::releasePrestart()
{
    if (DuiApplication::activeApplicationWindow()) {
        DuiApplication::activeApplicationWindow()->show();
        DuiApplication::activeApplicationWindow()->activateWindow();
        DuiApplication::activeApplicationWindow()->raise();
    }
}

void DuiApplication::restorePrestart()
{
    if (DuiApplication::activeApplicationWindow()) {
        DuiApplication::activeApplicationWindow()->hide();
        DuiApplication::activeApplicationWindow()->lower();
    }
}

bool DuiApplication::isPrestarted()
{
    return (DuiComponentData::prestartMode() != Dui::NoPrestart) && DuiComponentData::prestarted();
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

void DuiApplication::fastFloatMath(bool val)
{
    if (val) {
        set_runfast();
    } else {
        set_ieee();
    }
}

#endif
