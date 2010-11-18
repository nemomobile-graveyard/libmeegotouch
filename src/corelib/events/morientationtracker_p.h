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
#include <QString>

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
    void resolveIfOrientationUpdatesRequired();
    void rotateToAngleIfAllowed(M::OrientationAngle angle, MWindow* window);
#ifdef HAVE_CONTEXTSUBSCRIBER
    M::OrientationAngle angleForTopEdge(const QString topEdge) const;
    void subscribeToSensorProperies();
    void unsubscribeToSensorProperties();
    void waitForSensorPropertiesToSubscribe();
    ContextProperty *videoRouteProperty;
    ContextProperty *topEdgeProperty;
    ContextProperty *isCoveredProperty;
    ContextProperty *isFlatProperty;
    bool isSubscribed;
    bool hasJustSubscribed;
#endif
#ifdef Q_WS_X11
    bool handleX11PropertyEvent(XPropertyEvent* event);
    void handleCurrentAppWindowOrientationAngleChange();
    void handleCurrentAppWindowChange();
    WId fetchWIdCurrentAppWindow();
    M::OrientationAngle fetchCurrentAppWindowOrientationAngle();
    WId widCurrentAppWindow;
    bool currentAppWindowHadXPropertyChangeMask;
    //windows from this list follow _MEEGOTOUCH_CURRENT_APP_WINDOW (set by meego window manager).
    //by default it includes modal dialogs.
    QList<QPointer<MWindow> > windowsFollowingCurrentAppWindow;
    //windows from this list will follow _MEEGOTOUCH_CURRENT_APP_WINDOW but with
    //regard to restrictions as orientation lock or device profile data.
    //This list includes windows off display which do not get iconic state (WM_STATE),
    //and windows which have _MEEGOTOUCH_ALWAYS_MAPPED property set to 1 or 2.
    QList<QPointer<MWindow> > windowsFollowingWithConstraintsCurrentAppWindow;
    void startFollowingCurrentAppWindow(MWindow* win, bool limitedByConstraints = false);
    void stopFollowingCurrentAppWindow(MWindow* win, bool limitedByConstraints = false);
#endif

public slots:
    void isCoveredChanged();
    void videoRouteChanged();
    void updateOrientationAngle();

protected:
    MOrientationTracker *q_ptr;

private:
#ifdef Q_WS_X11
    Atom orientationAngleAtom;
    Atom currentAppWindowAtom;
#endif
    Q_DECLARE_PUBLIC(MOrientationTracker)
};

#endif // MORIENTATIONTRACKER_P_H
