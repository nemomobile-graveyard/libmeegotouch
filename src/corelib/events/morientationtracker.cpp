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

#include "mkeyboardstatetracker.h"
#include "mapplication.h"
#include "mwindow.h"
#include "mcomponentdata.h"
#include "mdeviceprofile.h"
#include <QCoreApplication>
#include <QVariant>
#include <QTimer>

#include <MDebug>

#include "morientationtracker.h"
#include "morientationtracker_p.h"

MOrientationTracker *MOrientationTrackerPrivate::tracker = 0;

MOrientationTrackerPrivate::MOrientationTrackerPrivate(MOrientationTracker *controller) :
    currentAngle(M::Angle0),
    currentIsCovered(false)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , currentIsKeyboardOpen(MKeyboardStateTracker::instance()->isOpen())
    , topEdgeProperty("Screen.TopEdge")
    , isCoveredProperty("Screen.IsCovered")
#endif
#ifdef Q_WS_X11
    , widCurrentAppWindow(0)
#endif //Q_WS_X11
    , q_ptr(controller)
{
    if (MComponentData::isOrientationForced()) {
        M::Orientation orientation = M::Landscape;

        currentAngle = MComponentData::forcedOrientationAngle();
        if (currentAngle == M::Angle90 || currentAngle == M::Angle270)
            orientation = M::Portrait;

        foreach(MWindow * window, MApplication::windows()) {
            if (window->orientation() == orientation)
                window->setOrientationAngle(currentAngle);
        }
        //orientation is fixed so we do not need to register for any signals
        return;
    }

#ifdef HAVE_CONTEXTSUBSCRIBER
    connect(&topEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(&isCoveredProperty, SIGNAL(valueChanged()),
            this, SLOT(isCoveredChanged()));
    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
            this, SLOT(updateOrientationAngle()));
#endif

    initContextSubscriber();

#ifdef Q_WS_X11
    orientationAngleAtom = XInternAtom(QX11Info::display(),
                                       "_MEEGOTOUCH_ORIENTATION_ANGLE", False);
    currentAppWindowAtom = XInternAtom(QX11Info::display(),
                                       "_MEEGOTOUCH_CURRENT_APP_WINDOW", False);
#endif //Q_WS_X11
}

void MOrientationTrackerPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //waiting for properties to synchronize
    topEdgeProperty.waitForSubscription();
    isCoveredProperty.waitForSubscription();

    //initiating the variables to current orientation
    updateOrientationAngle();
    isCoveredChanged();
#endif
}

void MOrientationTrackerPrivate::isCoveredChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_Q(MOrientationTracker);

    bool isCovered = isCoveredProperty.value().toBool();

    if (isCovered != currentIsCovered) {
        mDebug("MOrientationTracker") << "Covered:" << isCovered;

        currentIsCovered = isCovered;
        if (isCovered)
            emit q->faceFlippedDown();
        else
            emit q->faceUp();
    }
#endif
}

void MOrientationTrackerPrivate::updateOrientationAngle()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    M::OrientationAngle angle = M::Angle0;
    M::Orientation orientation = M::Landscape;
    QString edge = topEdgeProperty.value().toString();
    bool isKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();

    //Check if the keyboard was just closed, old angle is supported and
    //if device is lying flat (like on the table)
    if (currentIsKeyboardOpen == true && isKeyboardOpen == false
        && MDeviceProfile::instance()->orientationAngleIsSupported(currentAngle, isKeyboardOpen) &&
        topEdgeProperty.value().isNull()) {
        currentIsKeyboardOpen = isKeyboardOpen;
        //do nothing
        return;
    }
    currentIsKeyboardOpen = isKeyboardOpen;

    if (edge == "top" && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle0, isKeyboardOpen))) {
        angle = M::Angle0;
    } else if (edge == "left" && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle270, isKeyboardOpen))) {
        angle = M::Angle270;
        orientation = M::Portrait;
    } else if (edge == "right" && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle90, isKeyboardOpen))) {
        angle = M::Angle90;
        orientation = M::Portrait;
    } else if (edge == "bottom" && (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle180, isKeyboardOpen))) {
        angle = M::Angle180;
    } else {
        //it seems that orientation does not match allowed for current kybrd state.
        //check if the previous one was ok:
        if (MDeviceProfile::instance()->orientationAngleIsSupported(currentAngle, isKeyboardOpen)) {
            //it was: let's just use an old angle
            angle = currentAngle;
        } else {
            //it was not: let's use first allowed:
            if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle0, isKeyboardOpen))
                angle = M::Angle0;
            else if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle270, isKeyboardOpen))
                angle = M::Angle270;
            else if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle90, isKeyboardOpen))
                angle = M::Angle90;
            else if (MDeviceProfile::instance()->orientationAngleIsSupported(M::Angle180, isKeyboardOpen))
                angle = M::Angle180;
            else
                qFatal("MOrientationTrackerPrivate::updateOrientationAngle() - current keyboard state does not seem to be covered in target configuration file");
        }
    }

    if (angle != currentAngle) {
        currentAngle = angle;
        // instead of emitting a signal we have to explicitely call setOrientationAngle
        // on windows, because this is very often excuted before the application's
        // event loop is started and leads to crash in QMetaObject
        foreach(MWindow * window, MApplication::windows()) {
#ifdef Q_WS_X11
            if (!window->isOrientationAngleLocked() && !windowsFollowingCurrentAppWindow.contains(window)) {
#else
            if (!window->isOrientationAngleLocked()) {
#endif
                if (!window->isOrientationLocked() || window->orientation() == orientation)
                    window->setOrientationAngle(angle);
            }
        }
    }
#endif //HAVE_CONTEXTSUBSCRIBER
}

MOrientationTracker::MOrientationTracker() :
    d_ptr(new MOrientationTrackerPrivate(this))
{
    setParent(QCoreApplication::instance()); //get collected when needed.
}

MOrientationTracker *MOrientationTracker::instance()
{
    if (!MOrientationTrackerPrivate::tracker)
        MOrientationTrackerPrivate::tracker = new MOrientationTracker();
    return MOrientationTrackerPrivate::tracker;
}

MOrientationTracker::~MOrientationTracker()
{
    if (this == MOrientationTrackerPrivate::tracker)
        MOrientationTrackerPrivate::tracker = 0;
}

M::OrientationAngle MOrientationTracker::orientationAngle() const
{
    Q_D(const MOrientationTracker);

    return d->currentAngle;
}

#ifdef Q_WS_X11
bool MOrientationTrackerPrivate::handleX11PropertyEvent(XPropertyEvent *event)
{
    //handle only if there are any windows registered  to follow Current Window
    if (windowsFollowingCurrentAppWindow.count() > 0) {
        if (event->atom == orientationAngleAtom) {
            handleCurrentAppWindowOrientationAngleChange();
            return true;
        } else if (event->atom == currentAppWindowAtom) {
            handleCurrentAppWindowChange();
            return true;
        }
    }
    return false;
}

void MOrientationTrackerPrivate::handleCurrentAppWindowOrientationAngleChange()
{
    M::OrientationAngle angle = fetchCurrentAppWindowOrientationAngle();

    Q_FOREACH(MWindow * win, windowsFollowingCurrentAppWindow) {
        win->setOrientationAngle(angle);
    }
}

void MOrientationTrackerPrivate::handleCurrentAppWindowChange()
{
    //We stop listenieng to previous top window
    XSelectInput(QX11Info::display(), widCurrentAppWindow, 0);
    widCurrentAppWindow = fetchWIdCurrentAppWindow();
    //if current window is invalid or not set then return
    if (widCurrentAppWindow == 0)
        return;
    //And start listening to new top window
    XSelectInput(QX11Info::display(), widCurrentAppWindow, PropertyChangeMask);
}

WId MOrientationTrackerPrivate::fetchWIdCurrentAppWindow()
{
    WId currentWindowId = 0;
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytes = 0;

    union {
        unsigned char* asUChar;
        unsigned long* asULong;
    } data = {0};

    int status = XGetWindowProperty(QX11Info::display(), RootWindow(QX11Info::display(), 0),
                                    currentAppWindowAtom, 0, 1, False, AnyPropertyType,
                                    &actualType, &actualFormat, &nitems,
                                    &bytes, &data.asUChar);
    if (status == Success && actualType == XA_WINDOW && actualFormat == 32)
        currentWindowId = data.asULong[0];

    if (status == Success)
        XFree(data.asUChar);

    return currentWindowId;
}

M::OrientationAngle MOrientationTrackerPrivate::fetchCurrentAppWindowOrientationAngle()
{
    M::OrientationAngle angle = M::Angle0;
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytes = 0;

    union {
        unsigned char* asUChar;
        unsigned int* asInt;
    } data = {0};
    //we are using fetchWIdCurrentAppWindow() instead of stored value (widCurrentAppWindow)
    //to eliminate a risk of race condition
    int status = XGetWindowProperty(QX11Info::display(), fetchWIdCurrentAppWindow(),
                                    orientationAngleAtom, 0, 1, False, AnyPropertyType,
                                    &actualType, &actualFormat, &nitems,
                                    &bytes, &data.asUChar);

    if (status == Success && actualType == XA_CARDINAL && actualFormat == 32 && nitems == 1) {
        if (data.asInt[0] == M::Angle0 || data.asInt[0] == M::Angle90 || data.asInt[0] == M::Angle180
            || data.asInt[0] == M::Angle270 ){
            angle = (M::OrientationAngle)( data.asInt[0] );
        }
    }

    if (status == Success)
        XFree(data.asUChar);
    return angle;
}

void MOrientationTrackerPrivate::startFollowingCurrentAppWindow(MWindow *win)
{
    windowsFollowingCurrentAppWindow.append(win);
    M::OrientationAngle angle = fetchCurrentAppWindowOrientationAngle();
    win->setOrientationAngle(angle);
}

void MOrientationTrackerPrivate::stopFollowingCurrentAppWindow(MWindow *win)
{
    windowsFollowingCurrentAppWindow.removeAll(win);
    if (windowsFollowingCurrentAppWindow.count() == 0)
        XSelectInput(QX11Info::display(), widCurrentAppWindow, 0);
}

#endif //Q_WS_X11
