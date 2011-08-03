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
    currentAngle(M::Angle0),
    currentIsCovered(false),
    currentIsTvConnected(false),
    currentIsKeyboardOpen(MKeyboardStateTracker::instance()->isOpen())
#ifdef HAVE_CONTEXTSUBSCRIBER
    , videoRouteProperty(new ContextProperty("com.nokia.policy.video_route"))
    , topEdgeProperty(new ContextProperty("Screen.TopEdge"))
    , remoteTopEdgeProperty(new ContextProperty("RemoteScreen.TopEdge"))
    , isCoveredProperty(new ContextProperty("Screen.IsCovered"))
    , isFlatProperty(new ContextProperty("Position.IsFlat"))
    , remoteTopEdgeListener(0)
    , currentWindowAngleProperty(new MContextProperty("/Screen/CurrentWindow/OrientationAngle"))
    , desktopAngleProperty(new MContextProperty("/Screen/Desktop/OrientationAngle"))
    , isSubscribedToSensorProperties(false)
    , hasJustSubscribedToSensorProperties(false)
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
    videoRouteProperty->unsubscribe();
    topEdgeProperty->unsubscribe();
    remoteTopEdgeProperty->unsubscribe();
    isCoveredProperty->unsubscribe();
    isFlatProperty->unsubscribe();
    Q_ASSERT(!currentWindowAngleProperty->isSubscribed());
    Q_ASSERT(!desktopAngleProperty->isSubscribed());

    connect(topEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(remoteTopEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(isCoveredProperty, SIGNAL(valueChanged()),
            this, SLOT(isCoveredChanged()));
    connect(videoRouteProperty, SIGNAL(valueChanged()),
            this, SLOT(videoRouteChanged()));
    connect(isFlatProperty, SIGNAL(valueChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(MKeyboardStateTracker::instance(), SIGNAL(stateChanged()),
            this, SLOT(updateOrientationAngle()));
    connect(remoteTopEdgeListener, SIGNAL(nameAppeared()),
            this, SLOT(updateOrientationAngle()));
    connect(remoteTopEdgeListener, SIGNAL(nameDisappeared()),
            this, SLOT(updateOrientationAngle()));
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

    isCoveredChanged();
    videoRouteChanged();
}

MOrientationTrackerPrivate::~MOrientationTrackerPrivate()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete videoRouteProperty;
    delete topEdgeProperty;
    delete remoteTopEdgeListener;
    delete remoteTopEdgeProperty;
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
        updateOrientationAngle();
    } else if (!updatesRequired && isSubscribedToSensorProperties)
        unsubscribeFromSensorProperties();
#endif
}

void MOrientationTrackerPrivate::updateOrientationAngle()
{
    if (rotationsDisabled) {
        pendingOrientationAngleUpdate = true;
        return;
    }

#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!isSubscribedToSensorProperties) {
        return;
    }

    QString topEdge = topEdgeProperty->value().toString();
    QString remoteTopEdge = remoteTopEdgeProperty->value().toString();

    if ((remoteTopEdgeListener->isServicePresent() != MServiceListener::Present) || remoteTopEdge.isEmpty()) {
        M::OrientationAngle angle = angleForTopEdge(topEdge);
        bool isDeviceFlat = isFlatProperty->value().toBool();
        bool isKeyboardOpen = MKeyboardStateTracker::instance()->isOpen();
        doUpdateOrientationAngle(angle, isKeyboardOpen, isDeviceFlat, currentIsTvConnected);
    } else {
        M::OrientationAngle angle = angleForTopEdge(remoteTopEdge);
        rotateWindows(angle);
    }
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
    rotateWindows(angle);
#else
    Q_UNUSED(angle);
    Q_UNUSED(isKeyboardOpen);
    Q_UNUSED(isDeviceFlat);
    Q_UNUSED(tvIsConnected);
#endif //HAVE_CONTEXTSUBSCRIBER
}

#ifdef HAVE_CONTEXTSUBSCRIBER
void MOrientationTrackerPrivate::rotateWindows(M::OrientationAngle angle)
{
    if (angle == currentAngle && !hasJustSubscribedToSensorProperties) {
        return;
    }

    hasJustSubscribedToSensorProperties = false;
    currentAngle = angle;
    // instead of emitting a signal we have to explicitely call setOrientationAngle
    // on windows, because this is very often excuted before the application's
    // event loop is started and leads to crash in QMetaObject

    bool haveCurrWindowAngle = currentWindowAnglePropertyContainsValidAngle();

    foreach(MWindow * window, MApplication::windows()) {

        // If there's no current app. window angle to follow, those windows are
        // free to rotate like the others
        if (haveCurrWindowAngle &&
            (windowsFollowingCurrentAppWindow.contains(window) ||
             windowsFollowingWithConstraintsCurrentAppWindow.contains(window)))
            continue;

        if (windowsFollowingDesktop.contains(window))
            continue;

        rotateToAngleIfAllowed(angle, window);
    }
}
#endif //HAVE_CONTEXTSUBSCRIBER

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

#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!d->isSubscribedToSensorProperties) {
        MOrientationTrackerPrivate *priv = const_cast<MOrientationTracker*>(this)->d_ptr;
        // Momentarily subscribe to sensor properties to ensure we have an up to date
        // currentAngle value.
        priv->subscribeToSensorProperties();
        priv->updateOrientationAngle();
        priv->unsubscribeFromSensorProperties();
    }
#endif

    return d->currentAngle;
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

void MOrientationTracker::doUpdateOrientationAngle(
        M::OrientationAngle angle, bool isKeyboardOpen,
        bool isDeviceFlat, bool tvIsConnected)
{
    Q_D(MOrientationTracker);
    d->doUpdateOrientationAngle(angle, isKeyboardOpen, isDeviceFlat, tvIsConnected);
}
#ifdef HAVE_CONTEXTSUBSCRIBER
void MOrientationTrackerPrivate::subscribeToSensorProperties()
{
    if (!isSubscribedToSensorProperties) {
        //waiting for properties to synchronize
        topEdgeProperty->subscribe();
        remoteTopEdgeProperty->subscribe();
        isCoveredProperty->subscribe();
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
    remoteTopEdgeListener->startListening(true);
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
    if (rotationsDisabled) {
        pendingCurrentAppWindowOrientationAngleChangeHandling = true;
        return;
    }

    QVariant angleVariant = currentWindowAngleProperty->value();

    // We won't do anything if the property is not there.
    if (angleVariant.isNull() || !angleVariant.isValid())
        return;

    int angleInt = angleVariant.toInt();
    if (angleInt != 0 && angleInt != 90 && angleInt != 180 && angleInt != 270) {
        // We won't do anything if the property is holding an unsupported/invalid angle.
        // I.e., an angle that doesn't correspond to any value in M::OrientationAngle
        // enumeration.
        return;
    }

    M::OrientationAngle angle = static_cast<M::OrientationAngle>(angleInt);
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
            updateOrientationAngle();
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
