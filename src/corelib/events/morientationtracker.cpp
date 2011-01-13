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
#include "mwindow_p.h"
#include "mcomponentdata.h"
#include "mdeviceprofile.h"
#include <QCoreApplication>
#include <QVariant>
#include <QTimer>
#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#include <MDebug>

#include "morientationtracker.h"
#include "morientationtracker_p.h"
#include "mkeyboardstatetracker_p.h"

MOrientationTracker *MOrientationTrackerPrivate::tracker = 0;

MOrientationTrackerPrivate::MOrientationTrackerPrivate(MOrientationTracker *controller) :
    currentAngle(M::Angle0),
    currentIsCovered(false),
    currentIsTvConnected(false),
    currentIsKeyboardOpen(MKeyboardStateTracker::instance()->isOpen())
#ifdef HAVE_CONTEXTSUBSCRIBER
    , videoRouteProperty(new ContextProperty("com.nokia.policy.video_route"))
    , topEdgeProperty(new ContextProperty("Screen.TopEdge"))
    , isCoveredProperty(new ContextProperty("Screen.IsCovered"))
    , isFlatProperty(new ContextProperty("Position.IsFlat"))
    , isSubscribed(false)
    , hasJustSubscribed(false)
#endif
#ifdef Q_WS_X11
    , widCurrentAppWindow(0)
    , currentAppWindowHadXPropertyChangeMask(false)
#endif //Q_WS_X11
    , q_ptr(controller)
#ifdef Q_WS_X11
    , orientationAngleAtom(0)
    , currentAppWindowAtom(0)
#endif //Q_WS_X11
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
    connect(topEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(isCoveredProperty, SIGNAL(valueChanged()),
            this, SLOT(isCoveredChanged()));
    connect(videoRouteProperty, SIGNAL(valueChanged()),
            this, SLOT(videoRouteChanged()));
    connect(isFlatProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
            this, SLOT(updateOrientationAngle()));

    waitForSensorPropertiesToSubscribe();
#endif //HAVE_CONTEXTSUBSCRIBER

    isCoveredChanged();
    videoRouteChanged();

#ifdef Q_WS_X11
    orientationAngleAtom = XInternAtom(QX11Info::display(),
                                       "_MEEGOTOUCH_ORIENTATION_ANGLE", False);
    currentAppWindowAtom = XInternAtom(QX11Info::display(),
                                       "_MEEGOTOUCH_CURRENT_APP_WINDOW", False);
#endif //Q_WS_X11
}

MOrientationTrackerPrivate::~MOrientationTrackerPrivate()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete videoRouteProperty;
    delete topEdgeProperty;
    delete isCoveredProperty;
    delete isFlatProperty;
#endif
}

void MOrientationTrackerPrivate::videoRouteChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    QString value = videoRouteProperty->value().toString();
    mDebug("MOrientationTracker") << "VideoRoute:" << value;

    currentIsTvConnected = (value == "tvout" ||
                            value == "builtinandtvout");
    updateOrientationAngle();
#endif
}

void MOrientationTrackerPrivate::isCoveredChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_Q(MOrientationTracker);

    bool isCovered = isCoveredProperty->value().toBool();

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

void MOrientationTrackerPrivate::resolveIfOrientationUpdatesRequired()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool updatesRequired = false;
    foreach(MWindow* win, MApplication::windows()) {
#ifdef Q_WS_X11
        if (win && (win->isOnDisplay()) &&
            !windowsFollowingCurrentAppWindow.contains(win) &&
            !windowsFollowingWithConstraintsCurrentAppWindow.contains(win))
#else
        if (win && win->isOnDisplay())
#endif
        {
            updatesRequired = true;
            break;
        }
    }
    if (updatesRequired && !isSubscribed)
        subscribeToSensorProperies();
    else if (!updatesRequired && isSubscribed)
        unsubscribeToSensorProperties();
#endif
}

void MOrientationTrackerPrivate::updateOrientationAngle()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!isSubscribed) {
        return;
    }

    M::OrientationAngle angle = currentAngle;
    QString edge = topEdgeProperty->value().toString();

    angle = angleForTopEdge(edge);

    bool isDeviceFlat = isFlatProperty->value().toBool();

    bool isKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();

    doUpdateOrientationAngle(angle, isKeyboardOpen, isDeviceFlat, currentIsTvConnected);
#endif
}

void MOrientationTrackerPrivate::doUpdateOrientationAngle(M::OrientationAngle angle, bool isKeyboardOpen,
                                                          bool isDeviceFlat, bool tvIsConnected)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //Check if the keyboard was just closed, old angle is supported and
    //if device is lying flat (like on the table)
    if (currentIsKeyboardOpen == true && isKeyboardOpen == false
        && MDeviceProfile::instance()->orientationAngleIsSupported(currentAngle, isKeyboardOpen) &&
        isDeviceFlat) {
        currentIsKeyboardOpen = isKeyboardOpen;
        //do nothing
        return;
    }
    currentIsKeyboardOpen = isKeyboardOpen;

    if (tvIsConnected) // TV forces landscape for now, no transformations
        angle = M::Angle0;
    else if (!MDeviceProfile::instance()->orientationAngleIsSupported(angle, isKeyboardOpen)) {
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

    if (angle != currentAngle || hasJustSubscribed) {
        hasJustSubscribed = false;
        currentAngle = angle;
        // instead of emitting a signal we have to explicitely call setOrientationAngle
        // on windows, because this is very often excuted before the application's
        // event loop is started and leads to crash in QMetaObject
        foreach(MWindow * window, MApplication::windows()) {
            if(!windowsFollowingCurrentAppWindow.contains(window) &&
               !windowsFollowingWithConstraintsCurrentAppWindow.contains(window))
                rotateToAngleIfAllowed(angle, window);
        }
    }
#else
    Q_UNUSED(angle);
    Q_UNUSED(isKeyboardOpen);
    Q_UNUSED(isDeviceFlat);
    Q_UNUSED(tvIsConnected);
#endif //HAVE_CONTEXTSUBSCRIBER
}

void MOrientationTrackerPrivate::rotateToAngleIfAllowed(M::OrientationAngle angle, MWindow* window)
{
    M::Orientation orientation = M::Landscape;
    if (angle == M::Angle90 || angle == M::Angle270)
        orientation = M::Portrait;
    if (!window->isOrientationAngleLocked() &&
        (!window->isOrientationLocked() || window->orientation() == orientation)) {
        window->setOrientationAngle(angle);
    }
}

#ifdef HAVE_CONTEXTSUBSCRIBER
M::OrientationAngle MOrientationTrackerPrivate::angleForTopEdge(QString topEdge) const
{
    if (topEdge == "left")
        return M::Angle270;
    else if (topEdge == "right")
        return M::Angle90;
    else if (topEdge == "bottom")
        return M::Angle180;
    else
        return M::Angle0;
}
#endif //HAVE_CONTEXTSUBSCRIBER

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

void MOrientationTracker::childEvent(QChildEvent *event)
{
    Q_UNUSED(event);
}

void MOrientationTracker::doUpdateOrientationAngle(
        M::OrientationAngle angle, bool isKeyboardOpen,
        bool isDeviceFlat, bool tvIsConnected)
{
    Q_D(MOrientationTracker);
    d->doUpdateOrientationAngle(angle, isKeyboardOpen, isDeviceFlat, tvIsConnected);
}
#ifdef HAVE_CONTEXTSUBSCRIBER

void MOrientationTrackerPrivate::subscribeToSensorProperies()
{
    if(!isSubscribed) {
        //waiting for properties to synchronize
        topEdgeProperty->subscribe();
        isCoveredProperty->subscribe();
        videoRouteProperty->subscribe();
        isFlatProperty->subscribe();
        MKeyboardStateTracker::instance()->d_ptr->subscribe();
        waitForSensorPropertiesToSubscribe();
    }
    updateOrientationAngle();
}

void MOrientationTrackerPrivate::unsubscribeToSensorProperties()
{
    if(isSubscribed) {
        isSubscribed = false;
        topEdgeProperty->unsubscribe();
        isCoveredProperty->unsubscribe();
        videoRouteProperty->unsubscribe();
        isFlatProperty->unsubscribe();
        MKeyboardStateTracker::instance()->d_ptr->unsubscribe();
        MKeyboardStateTracker::instance()->d_ptr->present = false;
    }
}

void MOrientationTrackerPrivate::waitForSensorPropertiesToSubscribe()
{
    topEdgeProperty->waitForSubscription(true);
    isCoveredProperty->waitForSubscription(true);
    videoRouteProperty->waitForSubscription(true);
    isFlatProperty->waitForSubscription(true);
    isSubscribed = true;
    hasJustSubscribed = true;
}
#endif //HAVE_CONTEXTSUBSCRIBER

#ifdef Q_WS_X11

void MOrientationTracker::handleCurrentAppWindowChange()
{
    Q_D(MOrientationTracker);
    d->handleCurrentAppWindowChange();
}

void MOrientationTracker::handleCurrentAppWindowOrientationAngleChange()
{
    Q_D(MOrientationTracker);
    d->handleCurrentAppWindowOrientationAngleChange();
}

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
    //windows like system dialog will follow current window to any orientation
    foreach(MWindow * win, windowsFollowingCurrentAppWindow) {
        win->setOrientationAngle(angle);
    }
    //some special windows have their constraints and we have to follow them
    foreach(MWindow * window, windowsFollowingWithConstraintsCurrentAppWindow) {
        rotateToAngleIfAllowed(angle, window);
    }
}

void MOrientationTrackerPrivate::handleCurrentAppWindowChange()
{
    Display* display = QX11Info::display();
    if(!display)
        return;

    XWindowAttributes attributes;
    if (widCurrentAppWindow) {
        XGetWindowAttributes(display, widCurrentAppWindow, &attributes);
        //We unset PropertyChangeMask if it was not set before
        if (!currentAppWindowHadXPropertyChangeMask)
            attributes.your_event_mask &= ~PropertyChangeMask;
        XSelectInput(display, widCurrentAppWindow, attributes.your_event_mask);
    }

    //fetch ID of new current window
    widCurrentAppWindow = fetchWIdCurrentAppWindow();

    //current window is invalid or not set then return
    if (widCurrentAppWindow == 0)
        return;

    XGetWindowAttributes(display, widCurrentAppWindow, &attributes);
    //remember if PropertyChangeMask is already set
    currentAppWindowHadXPropertyChangeMask = (bool)(attributes.your_event_mask & PropertyChangeMask);
    //set PropertyChangeMask for current app window if it is not set
    XSelectInput(display, widCurrentAppWindow, attributes.your_event_mask | PropertyChangeMask);
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

M::OrientationAngle MOrientationTrackerPrivate::fetchCurrentAppWindowOrientationAngle(int* error)
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

    if (error)
        *error = status;

    return angle;
}

void MOrientationTrackerPrivate::startFollowingCurrentAppWindow(MWindow *win, bool limitedByConstraints)
{
    M::OrientationAngle angle = fetchCurrentAppWindowOrientationAngle();
    if (limitedByConstraints && !windowsFollowingWithConstraintsCurrentAppWindow.contains(win)) {
        windowsFollowingWithConstraintsCurrentAppWindow.append(win);
        rotateToAngleIfAllowed(angle, win);
    } else {
        windowsFollowingCurrentAppWindow.append(win);
        win->setOrientationAngle(angle);
    }
    handleCurrentAppWindowChange();
}

void MOrientationTrackerPrivate::stopFollowingCurrentAppWindow(MWindow *win, bool limitedByConstraints)
{
    if (limitedByConstraints)
        windowsFollowingWithConstraintsCurrentAppWindow.removeAll(win);
    else
        windowsFollowingCurrentAppWindow.removeAll(win);

    if (windowsFollowingCurrentAppWindow.count() == 0 &&
        windowsFollowingWithConstraintsCurrentAppWindow.count() == 0) {
        if (widCurrentAppWindow) {
            XWindowAttributes attributes;
            XGetWindowAttributes(QX11Info::display(), widCurrentAppWindow, &attributes);
            //We unset PropertyChangeMask if it was not set before
            if (!currentAppWindowHadXPropertyChangeMask)
                attributes.your_event_mask &= ~PropertyChangeMask;
            XSelectInput(QX11Info::display(), widCurrentAppWindow, attributes.your_event_mask);
            widCurrentAppWindow = 0;
        }
        currentAppWindowHadXPropertyChangeMask = false;
    }
}

#endif //Q_WS_X11
