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
#include <X11/Xutil.h>
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
#if defined(HAVE_XDAMAGE) && defined(Q_WS_X11)
    XDamageQueryExtension(QX11Info::display(), &xDamageEventBase, &xDamageErrorBase);
#endif //HAVE_XDAMAGE && Q_WS_X11

#ifdef Q_WS_X11
    visibleAtom = XInternAtom(QX11Info::display(),
                              "_MEEGOTOUCH_VISIBLE_IN_SWITCHER", False);
    minimizeAnimationAtom = XInternAtom(QX11Info::display(),
                                        "_MEEGOTOUCH_MINIMIZE_ANIMATION", False);
#endif
}

MApplicationPrivate::~MApplicationPrivate()
{
    delete componentData;
}
#ifdef Q_WS_X11
void MApplicationPrivate::setWindowVisibility(MWindow * window, bool visible)
{
    // displayExited signal is delayed for one second if it's caused by
    // XVisibilityEvent or _MEEGOTOUCH_VISIBLE_IN_SWITCHER window property
    // (managed by MeegoTouch Home). Emitting will be canceled if VisibilityUnobscured
    // is received during the delay. Rationale for this is to prevent oscillation
    // of displayEntered / displayExited signals.
    if (visible) {
        MOnDisplayChangeEvent ev(visible, QRectF(QPointF(0, 0), window->visibleSceneSize()));
        MApplication::instance()->sendEvent(window, &ev);
    } else {
        window->d_ptr->sendDelayedExitDisplayEvent();
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
        win->d_ptr->resolveOrientationRules();
    }
}


MWindow * MApplicationPrivate::windowForId(Window window)
{
    Q_FOREACH(MWindow * p, MComponentData::windows()) {
        if (p && p->effectiveWinId() == static_cast<unsigned int>(window)) {
            return p;
        }
    }
    return NULL;
}

bool MApplicationPrivate::hasXStateAtom(Window window, Atom atom)
{
    Atom type;
    int format;
    bool found;
    long max_len = 1024;
    unsigned long nItems;
    unsigned long bytesAfter;
    unsigned char *data = NULL;

    static Atom stateAtom = XInternAtom(QX11Info::display(),
                                        "_NET_WM_STATE", True);

    if (XGetWindowProperty(QX11Info::display(), window, stateAtom, 0, max_len, False,
                           XA_ATOM, &type, &format, &nItems, &bytesAfter,
                           &data) == Success && data) {
        found = false;
        for (unsigned long i = 0; !found && i < nItems; i++) {
            if (data[i] == atom) {
                found = true;
            }
        }
        XFree (data);
        return found;
    } else {

        return false;
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
    componentData = MComponentData::createInstance(argc, argv, appIdentifier, newService);
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

bool MApplication::notify(QObject *receiver, QEvent *event)
{
    try {
        return QApplication::notify(receiver, event);
    } catch(std::exception& e) {
        mWarning("MApplication") << "Caught exception in notify:" << e.what();
	// make sure we die here...
	abort();
    }
    return false;
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

bool MApplication::showStyleNames()
{
    return MComponentData::showStyleNames();
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

void MApplication::setShowStyleNames(bool show)
{
    MComponentData::setShowStyleNames(show);
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
        d->handleXVisibilityEvent(xevent);

    } else if (event->type == PropertyNotify) {
        XPropertyEvent *xevent = (XPropertyEvent *) event;
        d->handleXPropertyEvent(xevent);
    } else if (event->type == ClientMessage) {
        XClientMessageEvent *e = (XClientMessageEvent*) event;
        if (e->message_type == d->minimizeAnimationAtom) {
            switch (e->data.l[0]) {
            case 0:   // minimizing was cancelled
                emit minimizingCanceled();
                break;
            case 1:   // minimizing started
                emit minimizing();
                break;
            case 100: // minimizing finished
                emit minimized();
                break;
            default:
                break;
            }
        }
    }
#ifdef HAVE_XDAMAGE
    else if (event->type == d->xDamageEventBase + XDamageNotify) {

        XDamageNotifyEvent *xevent = (XDamageNotifyEvent *) event;

        // xevent->more would inform us if there is more events for the
        // rendering operation. but there isn't interface to pass the
        // information to damageEvent.
        emit damageEvent(xevent->damage, xevent->area.x, xevent->area.y, xevent->area.width, xevent->area.height);

        return true;
    }
#endif //HAVE_XDAMAGE
    return false;
}

void MApplicationPrivate::handleXVisibilityEvent(XVisibilityEvent *xevent)
{
    static const bool wmRunning = MComponentData::isMeeGoWindowManagerRunning();

    // Listen only to synthetic events if meego window
    // manager is running.
    if (xevent->send_event || !wmRunning) {

        MWindow * window = MApplicationPrivate::windowForId(xevent->window);

        if (!window) return;

        switch (xevent->state) {
        case VisibilityFullyObscured:
            window->d_ptr->fullyObscured = true;
            if (!window->d_ptr->visibleInSwitcher) {
                setWindowVisibility(window, false);
            }
            break;

        case VisibilityUnobscured:
        case VisibilityPartiallyObscured:
            window->d_ptr->fullyObscured = false;
            setWindowVisibility(window, true);
            break;

        default:
            break;
        }
    }
}

void MApplicationPrivate::handleXPropertyEvent(XPropertyEvent *xevent)
{
    // _MEEGOTOUCH_VISIBLE_IN_SWITCHER is set by Home Screen for
    // windows that are in the switcher and visible. Set/unset the
    // flag for corresponding window because we need to combine this
    // information with VisibilityNotify's.

    MWindow *window = windowForId(xevent->window);
    if (window && xevent->state == PropertyNewValue) {
        Atom           type;
        int            format;
        unsigned long  nItems;
        unsigned long  bytesAfter;
        unsigned char *data = NULL;

        if (xevent->atom == visibleAtom) {
            // Read value of the property. Should be 1 or 0.
            if (XGetWindowProperty(QX11Info::display(), xevent->window, visibleAtom,
                                   0, 1, False, XA_CARDINAL, &type, &format, &nItems,
                                   &bytesAfter, &data) == Success && data) {
                const bool obscured = window->d_ptr->fullyObscured;
                bool visible = *data;
                window->d_ptr->visibleInSwitcher = visible;

                if (visible) {
                    setWindowVisibility(window, true);
                } else if (obscured){
                    setWindowVisibility(window, false);
                }

                XFree(data);
            }
        }
    }
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
    /*
      This exit function can be reached either from
      the MApplication constructor or from the event loop,
      since the MComponentCache was introduced.

      If this function is called from the event loop, we
      want to make sure that we use qApp->exit() so that
      qt can tear down itself correctly, see NB#195013
      for problems that can arise here.

      But when this function is called without a running
      event loop ( i.e. from the constructor ), then
      qApp->exit() does nothing, see its documentation for
      reference. To really exit in this case, we call
      the c exit() function.
     */

    // exit when an event loop is running
    qApp->exit( status );

    // exit when no event loop is running
    _exit( status );
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
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
    int tmp;
    __asm__ volatile(
        "fmrx       %[tmp], fpscr\n\t"
        "orr        %[tmp], %[tmp], #(1 << 24)\n\t" /* flush-to-zero */
        "orr        %[tmp], %[tmp], #(1 << 25)\n\t" /* default NaN */
        "bic        %[tmp], %[tmp], #((1 << 15) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8))\n\t" /* clear exception bits */
        "fmxr       fpscr, %[tmp]\n\t"
        : [tmp] "=r"(tmp));
    return tmp;
#else
    return 0;
#endif
}

/*
 * IEEE standard floating point calculation
 */
static int set_ieee()
{
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
    int tmp;
    __asm__ volatile(
        "fmrx       %[tmp], fpscr\n\t"
        "bic        %[tmp], %[tmp], #(1 << 24)\n\t" /* flush-to-zero */
        "bic        %[tmp], %[tmp], #(1 << 25)\n\t" /* default NaN */
        "fmxr       fpscr, %[tmp]\n\t"
        : [tmp] "=r"(tmp));
    return tmp;
#else
    return 0;
#endif
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
