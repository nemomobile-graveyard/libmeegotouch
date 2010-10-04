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

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#include <QObject>
#include <mnamespace.h>


/* Must be last, as it conflicts with some of the Qt defined types */
#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif //Q_WS_X11

class MOrientationTracker;
class MWindow;

class MOrientationTrackerPrivate : public QObject
{
    Q_OBJECT
public:
    MOrientationTrackerPrivate(MOrientationTracker *controller);
    static MOrientationTracker *tracker;
    M::OrientationAngle currentAngle;
    bool currentIsCovered;

#ifdef HAVE_CONTEXTSUBSCRIBER
    bool currentIsKeyboardOpen;
    ContextProperty topEdgeProperty;
    ContextProperty isCoveredProperty;
#endif
#ifdef Q_WS_X11
    bool handleX11PropertyEvent(XPropertyEvent* event);
    void handleCurrentAppWindowOrientationAngleChange();
    void handleCurrentAppWindowChange();
    WId fetchWIdCurrentAppWindow();
    M::OrientationAngle fetchCurrentAppWindowOrientationAngle();
    WId widCurrentAppWindow;
    QList<MWindow* > windowsFollowingCurrentAppWindow;
    void startFollowingCurrentAppWindow(MWindow* win);
    void stopFollowingCurrentAppWindow(MWindow* win);
#endif

public slots:
    void isCoveredChanged();

protected:
    MOrientationTracker *q_ptr;

private slots:
    void initContextSubscriber();
    void updateOrientationAngle();

private:
    Atom orientationAngleAtom;
    Atom currentAppWindowAtom;
    Q_DECLARE_PUBLIC(MOrientationTracker)
};

#endif // MORIENTATIONTRACKER_P_H
