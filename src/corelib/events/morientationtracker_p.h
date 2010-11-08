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

#ifndef MORIENTATIONTRACKER_P_H
#define MORIENTATIONTRACKER_P_H

#include <QObject>
#include <mnamespace.h>
#include <QPointer>

/* Must be last, as it conflicts with some of the Qt defined types */
#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif //Q_WS_X11

class MOrientationTracker;
class MWindow;
#ifdef HAVE_CONTEXTSUBSCRIBER
class ContextProperty;
#endif

class MOrientationTrackerPrivate : public QObject
{
    Q_OBJECT
public:
    MOrientationTrackerPrivate(MOrientationTracker *controller);
    ~MOrientationTrackerPrivate();
    static MOrientationTracker *tracker;
    M::OrientationAngle currentAngle;
    bool currentIsCovered;
    bool currentIsTvConnected;
    bool currentIsKeyboardOpen;
    void doUpdateOrientationAngle(M::OrientationAngle angle, bool isKeyboardOpen,
                                  bool isDeviceFlat, bool tvIsConnected);
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty *videoRouteProperty;
    ContextProperty *topEdgeProperty;
    ContextProperty *isCoveredProperty;
#endif
#ifdef Q_WS_X11
    bool handleX11PropertyEvent(XPropertyEvent* event);
    void handleCurrentAppWindowOrientationAngleChange();
    void handleCurrentAppWindowChange();
    WId fetchWIdCurrentAppWindow();
    M::OrientationAngle fetchCurrentAppWindowOrientationAngle();
    WId widCurrentAppWindow;
    bool currentAppWindowHadXPropertyChangeMask;
    QList<MWindow* > windowsFollowingCurrentAppWindow;
    void startFollowingCurrentAppWindow(MWindow* win);
    void stopFollowingCurrentAppWindow(MWindow* win);
#endif

public slots:
    void isCoveredChanged();
    void videoRouteChanged();
    void updateOrientationAngle();

protected:
    MOrientationTracker *q_ptr;

private slots:
    void initContextSubscriber();

private:
    QPointer<QObject> debugInterface;
#ifdef Q_WS_X11
    Atom orientationAngleAtom;
    Atom currentAppWindowAtom;
#endif
    Q_DECLARE_PUBLIC(MOrientationTracker)
};

class MOrientationTrackerTestInterface : public QObject
{
    Q_OBJECT
public:
    MOrientationTrackerTestInterface(MOrientationTrackerPrivate *d, QObject * parent = 0);
public Q_SLOTS:
    void doUpdateOrientationAngle(M::OrientationAngle angle, bool isKeyboardOpen,
                                  bool isDeviceFlat, bool tvIsConnected);
#ifdef Q_WS_X11
    void handleCurrentAppWindowChange();
    void handleCurrentAppWindowOrientationAngleChange();
#endif
private:
    MOrientationTrackerPrivate *d;
};

#endif // MORIENTATIONTRACKER_P_H
