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


#ifdef HAVE_CONTEXTSUBSCRIBER
MContextProperty::MContextProperty(const QString &key, QObject *parent)
    : QObject(parent), m_contextProperty(0), m_isSubscribed(false)
{
    m_contextProperty = new ContextProperty(key, this);
    connect(m_contextProperty, SIGNAL(valueChanged()), SIGNAL(valueChanged()));
}

MContextProperty::~MContextProperty()
{
}

void MContextProperty::subscribeAndWaitForSubscription()
{
    m_contextProperty->subscribe();
    m_contextProperty->waitForSubscription(true);
    m_isSubscribed = true;
}

void MContextProperty::unsubscribe() {
    if (m_isSubscribed) {
        m_contextProperty->unsubscribe();
        m_isSubscribed = false;
    }
}

bool MContextProperty::isSubscribed() const
{
    return m_isSubscribed;
}

QVariant MContextProperty::value() const
{
    return m_contextProperty->value();
}
#endif

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
    , currentWindowAngleProperty(new MContextProperty("/Screen/CurrentWindow/OrientationAngle"))
    , isSubscribedToSensorProperties(false)
    , hasJustSubscribedToSensorProperties(false)
#endif
    , q_ptr(controller)
{
    if (MComponentData::isOrientationForced()) {
        currentAngle = MComponentData::forcedOrientationAngle();
        M::Orientation orientation = MDeviceProfile::instance()->orientationFromAngle(currentAngle);

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
    connect(currentWindowAngleProperty, SIGNAL(valueChanged()),
            this, SLOT(handleCurrentAppWindowOrientationAngleChange()));

    waitForSensorPropertiesToSubscribe();
#endif //HAVE_CONTEXTSUBSCRIBER

    isCoveredChanged();
    videoRouteChanged();
}

MOrientationTrackerPrivate::~MOrientationTrackerPrivate()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete videoRouteProperty;
    delete topEdgeProperty;
    delete isCoveredProperty;
    delete isFlatProperty;
    delete currentWindowAngleProperty;
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
    if (!MApplication::isPrestarted()) {
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
    }
    if (updatesRequired && !isSubscribedToSensorProperties)
        subscribeToSensorProperies();
    else if (!updatesRequired && isSubscribedToSensorProperties)
        unsubscribeToSensorProperties();
#endif
}

void MOrientationTrackerPrivate::updateOrientationAngle()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!isSubscribedToSensorProperties) {
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

    if (tvIsConnected) { // TV forces landscape for now, no transformations
        if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
            angle = M::Angle0;
        } else {
            angle = M::Angle270;
        }
    } else if (!MDeviceProfile::instance()->orientationAngleIsSupported(angle, isKeyboardOpen)) {
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

    if (angle != currentAngle || hasJustSubscribedToSensorProperties) {
        hasJustSubscribedToSensorProperties = false;
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
    M::Orientation orientation = MDeviceProfile::instance()->orientationFromAngle(angle);
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
    delete d_ptr;
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
    if (!isSubscribedToSensorProperties) {
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
    if (isSubscribedToSensorProperties) {
        isSubscribedToSensorProperties = false;
        topEdgeProperty->unsubscribe();
        isCoveredProperty->unsubscribe();
        videoRouteProperty->unsubscribe();
        isFlatProperty->unsubscribe();
        MKeyboardStateTracker::instance()->d_ptr->unsubscribe();
    }
}

void MOrientationTrackerPrivate::waitForSensorPropertiesToSubscribe()
{
    topEdgeProperty->waitForSubscription(true);
    isCoveredProperty->waitForSubscription(true);
    videoRouteProperty->waitForSubscription(true);
    isFlatProperty->waitForSubscription(true);
    isSubscribedToSensorProperties = true;
    hasJustSubscribedToSensorProperties = true;
}

void MOrientationTracker::handleCurrentAppWindowOrientationAngleChange()
{
    Q_D(MOrientationTracker);
    d->handleCurrentAppWindowOrientationAngleChange();
}

void MOrientationTrackerPrivate::handleCurrentAppWindowOrientationAngleChange()
{
    M::OrientationAngle angle = (M::OrientationAngle)currentWindowAngleProperty->value().toInt();
    //windows like system dialog will follow current window to any orientation
    foreach(MWindow * win, windowsFollowingCurrentAppWindow) {
        win->setOrientationAngle(angle);
    }
    //some special windows have their constraints and we have to follow them
    foreach(MWindow * window, windowsFollowingWithConstraintsCurrentAppWindow) {
        rotateToAngleIfAllowed(angle, window);
    }
}
#endif //HAVE_CONTEXTSUBSCRIBER

void MOrientationTrackerPrivate::startFollowingCurrentAppWindow(MWindow *win, bool limitedByConstraints)
{
    if (limitedByConstraints && !windowsFollowingWithConstraintsCurrentAppWindow.contains(win)) {
        windowsFollowingWithConstraintsCurrentAppWindow.append(win);;
    } else {
        windowsFollowingCurrentAppWindow.append(win);
    }
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!currentWindowAngleProperty->isSubscribed())
        currentWindowAngleProperty->subscribeAndWaitForSubscription();
    handleCurrentAppWindowOrientationAngleChange();
#endif //HAVE_CONTEXTSUBSCRIBER
}

void MOrientationTrackerPrivate::stopFollowingCurrentAppWindow(MWindow *win, bool limitedByConstraints)
{
    if (limitedByConstraints)
        windowsFollowingWithConstraintsCurrentAppWindow.removeAll(win);
    else
        windowsFollowingCurrentAppWindow.removeAll(win);
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (windowsFollowingWithConstraintsCurrentAppWindow.isEmpty() &&
        windowsFollowingCurrentAppWindow.isEmpty())
        currentWindowAngleProperty->unsubscribe();
#endif //HAVE_CONTEXTSUBSCRIBER
}
