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
#include "contextpropertyinfo.h"
#include "mservicelistener.h"
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
    // The ContextProperty constructor calls ContextProperty::subscribe(),
    // unsubscribe again to stay consistent with m_isSubscribed.
    m_contextProperty->unsubscribe();
    connect(m_contextProperty, SIGNAL(valueChanged()), SIGNAL(valueChanged()));
}

MContextProperty::~MContextProperty()
{
    unsubscribe();
    delete m_contextProperty;
    m_contextProperty = 0;
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

QVariant MContextProperty::value(const QVariant &def) const
{
    return m_contextProperty->value(def);
}
#endif

MOrientationTrackerPrivate::MOrientationTrackerPrivate(MOrientationTracker *controller) :
    currentIsTvConnected(false),
    currentIsKeyboardOpen(MKeyboardStateTracker::instance()->isOpen())
#ifdef HAVE_CONTEXTSUBSCRIBER
    , videoRouteProperty(new ContextProperty("com.nokia.policy.video_route"))
    , topEdgeProperty(new ContextProperty("Screen.TopEdge"))
    , remoteTopEdgeProperty(new ContextProperty("RemoteScreen.TopEdge"))
    , isFlatProperty(new ContextProperty("Position.IsFlat"))
    , remoteTopEdgeListener(0)
    , currentWindowAngleProperty(new MContextProperty("/Screen/CurrentWindow/OrientationAngle"))
    , desktopAngleProperty(new MContextProperty("/Screen/Desktop/OrientationAngle"))
    , isSubscribedToSensorProperties(false)
#endif
    , rotationsDisabled(false)
    , pendingOrientationAngleUpdate(false)
    , pendingCurrentAppWindowOrientationAngleChangeHandling(false)
    , q_ptr(controller)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    remoteTopEdgeListener = new MServiceListener(
        remoteTopEdgeProperty->info()->providerDBusType(),
        remoteTopEdgeProperty->info()->providerDBusName());
#endif

    if (MComponentData::isOrientationForced()) {
        M::OrientationAngle currentAngle = MComponentData::forcedOrientationAngle();
        M::Orientation orientation = MDeviceProfile::instance()->orientationFromAngle(currentAngle);

        foreach(MWindow * window, MApplication::windows()) {
            if (window->orientation() == orientation)
                window->setOrientationAngle(currentAngle);
        }
        //orientation is fixed so we do not need to register for any signals
        return;
    }

#ifdef HAVE_CONTEXTSUBSCRIBER
    videoRouteProperty->unsubscribe();
    topEdgeProperty->unsubscribe();
    remoteTopEdgeProperty->unsubscribe();
    isFlatProperty->unsubscribe();
    Q_ASSERT(!currentWindowAngleProperty->isSubscribed());
    Q_ASSERT(!desktopAngleProperty->isSubscribed());

    connect(topEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngleOfWindows()));
    connect(remoteTopEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngleOfWindows()));
    connect(videoRouteProperty, SIGNAL(valueChanged()),
            this, SLOT(videoRouteChanged()));
    connect(isFlatProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngleOfWindows()));
    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
            this, SLOT(updateOrientationAngleOfWindows()));
    connect(remoteTopEdgeListener, SIGNAL(nameAppeared()),
            this, SLOT(updateOrientationAngleOfWindows()));
    connect(remoteTopEdgeListener, SIGNAL(nameDisappeared()),
            this, SLOT(updateOrientationAngleOfWindows()));
    connect(currentWindowAngleProperty, SIGNAL(valueChanged()),
            this, SLOT(handleCurrentAppWindowOrientationAngleChange()));
    connect(desktopAngleProperty, SIGNAL(valueChanged()),
            this, SLOT(handleDesktopOrientationChange()));
#endif //HAVE_CONTEXTSUBSCRIBER

#ifdef Q_WS_X11
    connect(MApplication::instance(), SIGNAL(minimizing()),
            this, SLOT(disableRotations()));
    connect(MApplication::instance(), SIGNAL(minimized()),
            this, SLOT(enableRotations()));
    connect(MApplication::instance(), SIGNAL(minimizingCanceled()),
            this, SLOT(enableRotations()));
#endif

    videoRouteChanged();
}

MOrientationTrackerPrivate::~MOrientationTrackerPrivate()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete videoRouteProperty;
    delete topEdgeProperty;
    delete remoteTopEdgeListener;
    delete remoteTopEdgeProperty;
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
    updateOrientationAngleOfWindows();
#endif
}

#ifdef HAVE_CONTEXTSUBSCRIBER
bool MOrientationTrackerPrivate::checkIfOrientationUpdatesRequired()
{
    if (MApplication::isPrestarted()) {
        return false;
    }

    bool result = false;

    // If there's no current app. window angle to follow, those windows are free to
    // rotate like the others
    bool haveCurrWindowAngle = currentWindowAnglePropertyContainsValidAngle();

    foreach(MWindow* win, MApplication::windows()) {
#ifdef Q_WS_X11
        if (win && (win->isOnDisplay()) &&
            (!haveCurrWindowAngle || (!windowsFollowingCurrentAppWindow.contains(win) &&
            !windowsFollowingWithConstraintsCurrentAppWindow.contains(win))) &&
            !windowsFollowingDesktop.contains(win))
#else
        if (win && win->isOnDisplay())
#endif
        {
            result = true;
            break;
        }
    }

    return result;
}
#endif

void MOrientationTrackerPrivate::reevaluateSubscriptionToSensorProperties()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool updatesRequired = checkIfOrientationUpdatesRequired();

    if (updatesRequired && !isSubscribedToSensorProperties) {
        subscribeToSensorProperties();
        updateOrientationAngleOfWindows();
    } else if (!updatesRequired && isSubscribedToSensorProperties)
        unsubscribeFromSensorProperties();
#endif
}

void MOrientationTrackerPrivate::updateOrientationAngleOfWindows()
{
    if (rotationsDisabled) {
        pendingOrientationAngleUpdate = true;
        return;
    }

#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!isSubscribedToSensorProperties) {
        return;
    }

    QString remoteTopEdge = remoteTopEdgeProperty->value().toString();

    if ((remoteTopEdgeListener->isServicePresent() != MServiceListener::Present) || remoteTopEdge.isEmpty()) {

        foreach(MWindow * window, MApplication::windows()) {
            updateWindowOrientationAngle(window);
        }
    } else {
        M::OrientationAngle angle = angleForTopEdge(remoteTopEdge);
        foreach(MWindow * window, MApplication::windows()) {
            if (windowIsFollowingDeviceOrientation(window))
                rotateToAngleIfAllowed(angle, window);
        }
    }

    currentIsKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();
#endif
}

#ifdef HAVE_CONTEXTSUBSCRIBER

M::OrientationAngle MOrientationTrackerPrivate::updateOrientationAngleWithDeviceAngle(
        M::OrientationAngle *currentAngle) const
{
    M::OrientationAngle targetAngle = (M::OrientationAngle)-1;
    M::OrientationAngle deviceAngle = angleForTopEdge(topEdgeProperty->value().toString());
    bool isKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();

    if (MDeviceProfile::instance()->orientationAngleIsSupported(deviceAngle, isKeyboardOpen)) {
        // simple case. there's nothing stopping us from following the device orientation.
        targetAngle = deviceAngle;
    } else {
        //it seems that orientation does not match allowed for current kybrd state.
        //check if the previous one was ok:
        if (currentAngle
                && MDeviceProfile::instance()->orientationAngleIsSupported(*currentAngle, isKeyboardOpen)) {
            //it was: let's just use an old angle
            targetAngle = *currentAngle;
        } else {
            //it was not: let's use the closest allowed angle
            targetAngle = findClosestAllowedAngle(deviceAngle, isKeyboardOpen);
        }
    }

    Q_ASSERT((int)targetAngle != -1);
    return targetAngle;
}

M::OrientationAngle MOrientationTrackerPrivate::updateOrientationAngle(M::OrientationAngle *currentAngle) const
{
    M::OrientationAngle targetAngle = (M::OrientationAngle)-1;
    bool isDeviceFlat = isFlatProperty->value().toBool();
    bool isKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();

    //Check if the keyboard was just closed, old angle is supported and
    //if device is lying flat (like on the table)
    if (currentIsKeyboardOpen == true && isKeyboardOpen == false
        && currentAngle
        && MDeviceProfile::instance()->orientationAngleIsSupported(*currentAngle, isKeyboardOpen)
        && isDeviceFlat) {
        // do nothing
        targetAngle = *currentAngle;
    } else if (currentIsTvConnected) { // TV forces landscape for now, no transformations
        if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
            targetAngle = M::Angle0;
        } else {
            targetAngle = M::Angle270;
        }
    } else {
        if (isDeviceFlat) {
            // device orientation angle information is meaningless in this situation.
            if (currentAngle) {
                // therefore stay where we are.
                targetAngle = *currentAngle;
            } else {
                // since we don't have a currentAngle, let's try to use the angle of the current
                // application window
                if (!fetchCurrentWindowAngle(targetAngle)) {
                    // current window angle is invalid (or not present). Fallback to device angle.
                    targetAngle = updateOrientationAngleWithDeviceAngle(currentAngle);
                }
            }
        } else {
            targetAngle = updateOrientationAngleWithDeviceAngle(currentAngle);
        }
    }

    Q_ASSERT((int)targetAngle != -1);
    return targetAngle;
}
#endif

#ifdef HAVE_CONTEXTSUBSCRIBER
bool MOrientationTrackerPrivate::fetchCurrentWindowAngle(M::OrientationAngle &angle) const
{
    QVariant angleVariant;

    if (!currentWindowAngleProperty->isSubscribed()) {
        currentWindowAngleProperty->subscribeAndWaitForSubscription();
        angleVariant = currentWindowAngleProperty->value();
        currentWindowAngleProperty->unsubscribe();
    } else {
        angleVariant = currentWindowAngleProperty->value();
    }

    // We won't take it if the property is not there.
    if (angleVariant.isNull() || !angleVariant.isValid())
        return false;

    int angleInt = angleVariant.toInt();
    if (angleInt != 0 && angleInt != 90 && angleInt != 180 && angleInt != 270) {
        // We won't take it if the property is holding an unsupported/invalid angle.
        // I.e., an angle that doesn't correspond to any value in M::OrientationAngle
        // enumeration.
        return false;
    }

    angle = static_cast<M::OrientationAngle>(angleInt);
    return true;
}

M::OrientationAngle MOrientationTrackerPrivate::computeTrackerOrientationAngle() const
{
    // This angle is always the closest valid one to the device orientation.
    // Unlike the angle of regular windows, which keep their current orientation
    // if the device in an unsupported orientation. That's why we do not
    // pass currentAngle as a parameter here.
    M::OrientationAngle targetAngle = updateOrientationAngle(0);

    return targetAngle;
}
#endif //HAVE_CONTEXTSUBSCRIBER

#ifdef HAVE_CONTEXTSUBSCRIBER
bool MOrientationTrackerPrivate::windowIsFollowingDeviceOrientation(MWindow *window)
{
    if (windowsFollowingDesktop.contains(window))
        return false;

    // If there's no current app. window angle to follow, those windows are
    // free to rotate like the others
    if ((windowsFollowingCurrentAppWindow.contains(window) ||
         windowsFollowingWithConstraintsCurrentAppWindow.contains(window))
        && currentWindowAnglePropertyContainsValidAngle())
        return false;

    return true;
}
#endif //HAVE_CONTEXTSUBSCRIBER

#ifdef HAVE_CONTEXTSUBSCRIBER
void MOrientationTrackerPrivate::updateWindowOrientationAngle(MWindow *window)
{
    if (!windowIsFollowingDeviceOrientation(window))
        return;

    M::OrientationAngle windowAngle = window->orientationAngle();
    M::OrientationAngle targetAngle = updateOrientationAngle(&windowAngle);

    if (targetAngle != windowAngle)
        rotateToAngleIfAllowed(targetAngle, window);
}
#endif //HAVE_CONTEXTSUBSCRIBER

M::OrientationAngle MOrientationTrackerPrivate::findClosestAllowedAngle(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    // try angle+90 degrees
    int candidateAngle = (angle + 90) % 360;
    if (MDeviceProfile::instance()->orientationAngleIsSupported((M::OrientationAngle)candidateAngle, isKeyboardOpen)) {
        return (M::OrientationAngle)candidateAngle;
    }

    // try angle-90 degrees
    candidateAngle = angle - 90;
    if (candidateAngle < 0) {
        candidateAngle = 360 + candidateAngle;
    }
    if (MDeviceProfile::instance()->orientationAngleIsSupported((M::OrientationAngle)candidateAngle, isKeyboardOpen)) {
        return (M::OrientationAngle)candidateAngle;
    }

    // try angle+180 degrees
    candidateAngle = (angle + 180) % 360;
    if (MDeviceProfile::instance()->orientationAngleIsSupported((M::OrientationAngle)candidateAngle, isKeyboardOpen)) {
        return (M::OrientationAngle)candidateAngle;
    }

    // give up. That just cannot happen.
    qFatal("MOrientationTrackerPrivate::findClosestAllowedAngle() - "
           "current keyboard state does not seem to be covered in target configuration file");

    // to make compiler happy. should never be reached due to the qFatal() above
    return M::Angle0;
}

void MOrientationTrackerPrivate::rotateToAngleIfAllowed(M::OrientationAngle angle, MWindow* window)
{
    M::Orientation orientation = MDeviceProfile::instance()->orientationFromAngle(angle);
    if (!window->isOrientationAngleLocked() &&
        (!window->isOrientationLocked() || window->orientation() == orientation)) {
        // instead of emitting a signal we have to explicitely call setOrientationAngle
        // on windows, because this is very often excuted before the application's
        // event loop is started and leads to crash in QMetaObject
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
    M::OrientationAngle result = M::Angle0;

#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!d->isSubscribedToSensorProperties) {
        MOrientationTrackerPrivate *priv = const_cast<MOrientationTracker*>(this)->d_ptr;
        // Momentarily subscribe to sensor properties.
        priv->subscribeToSensorProperties();
        result = d->computeTrackerOrientationAngle();
        priv->unsubscribeFromSensorProperties();
    } else {
        result = d->computeTrackerOrientationAngle();
    }
#endif

    return result;
}

void MOrientationTracker::childEvent(QChildEvent *event)
{
    Q_UNUSED(event);
}

void MOrientationTracker::disableRotations()
{
    Q_D(MOrientationTracker);
    d->disableRotations();
}

void MOrientationTracker::enableRotations()
{
    Q_D(MOrientationTracker);
    d->enableRotations();
}

bool MOrientationTracker::isSubscribedToSensorProperties() const
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_D(const MOrientationTracker);
    return d->isSubscribedToSensorProperties;
#else
    return false;
#endif
}

#ifdef HAVE_CONTEXTSUBSCRIBER
void MOrientationTrackerPrivate::subscribeToSensorProperties()
{
    if (!isSubscribedToSensorProperties) {
        //waiting for properties to synchronize
        topEdgeProperty->subscribe();
        remoteTopEdgeProperty->subscribe();
        videoRouteProperty->subscribe();
        isFlatProperty->subscribe();
        MKeyboardStateTracker::instance()->d_ptr->subscribe();
        waitForSensorPropertiesToSubscribe();
    }
}

void MOrientationTrackerPrivate::unsubscribeFromSensorProperties()
{
    if (isSubscribedToSensorProperties) {
        isSubscribedToSensorProperties = false;
        topEdgeProperty->unsubscribe();
        remoteTopEdgeProperty->unsubscribe();
        videoRouteProperty->unsubscribe();
        isFlatProperty->unsubscribe();
        MKeyboardStateTracker::instance()->d_ptr->unsubscribe();
    }
}

void MOrientationTrackerPrivate::waitForSensorPropertiesToSubscribe()
{
    topEdgeProperty->waitForSubscription(true);
    videoRouteProperty->waitForSubscription(true);
    isFlatProperty->waitForSubscription(true);
    remoteTopEdgeListener->startListening(true);
    isSubscribedToSensorProperties = true;
}

void MOrientationTracker::handleCurrentAppWindowOrientationAngleChange()
{
    Q_D(MOrientationTracker);
    d->handleCurrentAppWindowOrientationAngleChange();
}

void MOrientationTrackerPrivate::handleCurrentAppWindowOrientationAngleChange()
{
    if (rotationsDisabled) {
        pendingCurrentAppWindowOrientationAngleChangeHandling = true;
        return;
    }

    M::OrientationAngle angle;
    if (!fetchCurrentWindowAngle(angle))
        return;

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
    } else if (!limitedByConstraints && !windowsFollowingCurrentAppWindow.contains(win)) {
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

#ifdef HAVE_CONTEXTSUBSCRIBER
bool MOrientationTrackerPrivate::currentWindowAnglePropertyContainsValidAngle()
{
    QVariant currentWindowAngleVariant = currentWindowAngleProperty->value();

    if (currentWindowAngleVariant.isNull() || !currentWindowAngleVariant.isValid()) {
        return false;
    }

    int currentWindowAngleValue = currentWindowAngleVariant.toInt();

    return currentWindowAngleValue == 0
        || currentWindowAngleValue == 90
        || currentWindowAngleValue == 180
        || currentWindowAngleValue == 270;
}
#endif //HAVE_CONTEXTSUBSCRIBER

void MOrientationTrackerPrivate::disableRotations()
{
    if (!rotationsDisabled) {
        rotationsDisabled = true;
        pendingOrientationAngleUpdate = false;
        pendingCurrentAppWindowOrientationAngleChangeHandling = false;
    }
}

void MOrientationTrackerPrivate::enableRotations()
{
    if (rotationsDisabled) {
        rotationsDisabled = false;

        if (pendingOrientationAngleUpdate) {
            updateOrientationAngleOfWindows();
            pendingOrientationAngleUpdate = false;
        }

        if (pendingCurrentAppWindowOrientationAngleChangeHandling) {
#ifdef HAVE_CONTEXTSUBSCRIBER
            handleCurrentAppWindowOrientationAngleChange();
#endif //HAVE_CONTEXTSUBSCRIBER
            pendingCurrentAppWindowOrientationAngleChangeHandling = false;
        }
    }
}

#ifdef HAVE_CONTEXTSUBSCRIBER
void MOrientationTrackerPrivate::handleDesktopOrientationChange()
{
    //if property is not set we default to portrait orientation
    int fallbackAngle =
            (MDeviceProfile::instance()->orientationFromAngle(M::Angle270) == M::Portrait)?270:0;
    M::OrientationAngle angle = (M::OrientationAngle)desktopAngleProperty->value(fallbackAngle).toInt();
    foreach(MWindow *win, windowsFollowingDesktop)
        if (win)
            rotateToAngleIfAllowed(angle, win);
}
#endif //HAVE_CONTEXTSUBSCRIBER

void MOrientationTrackerPrivate::startFollowingDesktop(MWindow *win)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!windowsFollowingDesktop.contains(win))
        windowsFollowingDesktop.append(win);

    if (!desktopAngleProperty->isSubscribed())
        desktopAngleProperty->subscribeAndWaitForSubscription();
    handleDesktopOrientationChange();
#else
    Q_UNUSED(win);
#endif //HAVE_CONTEXTSUBSCRIBER
}

void MOrientationTrackerPrivate::stopFollowingDesktop(MWindow *win)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    windowsFollowingDesktop.removeAll(win);
    if (windowsFollowingDesktop.isEmpty())
        desktopAngleProperty->unsubscribe();
#else
    Q_UNUSED(win);
#endif //HAVE_CONTEXTSUBSCRIBER
}
