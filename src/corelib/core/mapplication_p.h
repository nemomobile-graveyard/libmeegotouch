/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MAPPLICATION_P_H
#define MAPPLICATION_P_H

#include <QObject>
#include "mcomponentdata.h"
#include <MLocale>
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <mdynamicpropertywatcher.h>
#endif

#include <mwindow.h>

class MTheme;
class MApplication;
class MSceneWindow;
class MFeedbackPlayer;
class MApplicationService;
class MDeviceProfile;

class MApplicationPrivate
{
    Q_DECLARE_PUBLIC(MApplication)

public:
    MApplicationPrivate();
    virtual ~MApplicationPrivate();

    MComponentData *componentData;

    int xDamageEventBase;
    int xDamageErrorBase;

#ifdef Q_WS_X11
    /**
     * Emits one of the signals minimizing(), minimized() or
     * minimizingCanceled() dependent from \a progress:
     * 0 ..... Minimizing has been canceled
     * 1 ..... Minimizing has been started
     * 100 ... Minimizing has been finished
     *
     * If the window is already closed no signal will be emitted.
     *
     * The method assures that the signal minimizingCanceled()
     * is emitted in case if the window \a window is closed
     * after sending a minimizing() signal but before a progress
     * of 0 or 100 has been received.
     */
    void emitMinimizeSignals(MWindow *window, int progress);
    void _q_windowClosedDuringMinimizing(QObject *window);

    static int handleXError(Display *display, XErrorEvent *event);
#endif

protected:
    MApplication *q_ptr;

private:
    void init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);

    static void releasePrestart();
    static void restorePrestart();
    static void hideWindows();
    static void stdExit(int status);
    static bool prestartModeIsLazyShutdown();
    static bool prestartModeIsMultiWindowed();

#ifdef Q_WS_X11
    static void setWindowVisibility(MWindow *window, bool visible);
    static void setX11PrestartPropertyForWindows(bool set);
    static MWindow * windowForId(Window window);
    static bool hasXStateAtom(Window window, Atom atom);
    void handleXVisibilityEvent(XVisibilityEvent *xevent);
    void handleXPropertyEvent(XPropertyEvent *xevent);
    Atom visibleAtom;
    Atom minimizeAnimationAtom;
    MMultiObjectsPropertyWatcher* windowClosedWatcher;
#endif
    friend class MApplicationServicePrivate;
    friend class MWindow;
    friend class MWindowPrivate;
};


#endif
