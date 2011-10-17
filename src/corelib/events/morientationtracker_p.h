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

#ifndef MORIENTATIONTRACKER_P_H
#define MORIENTATIONTRACKER_P_H

#include <QObject>
#include <mnamespace.h>
#include <QPointer>
#include <QString>

class MOrientationTracker;
class MWindow;
#ifdef HAVE_CONTEXTSUBSCRIBER
class ContextProperty;
class ContextPropertyInfo;
class MServiceListener;

// A simple wrapper to ContextProperty that provides the missing
// isSusbscribed() getter.
class MContextProperty : public QObject {
    Q_OBJECT
public:
    MContextProperty(const QString &key, QObject *parent = 0);
    virtual ~MContextProperty();

    void subscribeAndWaitForSubscription();
    void unsubscribe();
    bool isSubscribed() const;

    QVariant value() const;
    QVariant value(const QVariant &def) const;

Q_SIGNALS:
    void valueChanged();

private:
    ContextProperty *m_contextProperty;
    bool m_isSubscribed;
};

#endif


class MOrientationTrackerPrivate : public QObject
{
    Q_OBJECT
public:
    MOrientationTrackerPrivate(MOrientationTracker *controller);
    ~MOrientationTrackerPrivate();
    static MOrientationTracker *tracker;

    bool currentIsTvConnected;
    bool currentIsKeyboardOpen;
    // Returns the closest orientation angle to the given one that is allowed
    // according to current profile.
    M::OrientationAngle findClosestAllowedAngle(M::OrientationAngle angle, bool isKeyboardOpen) const;
    void reevaluateSubscriptionToSensorProperties();
    void rotateToAngleIfAllowed(M::OrientationAngle angle, MWindow* window);
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool fetchCurrentWindowAngle(M::OrientationAngle &angle) const;
    M::OrientationAngle computeTrackerOrientationAngle() const;
    void updateWindowOrientationAngle(MWindow *window);
    M::OrientationAngle updateOrientationAngleWithDeviceAngle(M::OrientationAngle *currentAngle) const;
    M::OrientationAngle updateOrientationAngle(M::OrientationAngle *currentAngle) const;
    bool windowIsFollowingDeviceOrientation(MWindow *window);
    bool checkIfOrientationUpdatesRequired();
    M::OrientationAngle angleForTopEdge(const QString topEdge) const;
    void subscribeToSensorProperties();
    void unsubscribeFromSensorProperties();
    void waitForSensorPropertiesToSubscribe();
    //Properties based on sensors states
    ContextProperty *videoRouteProperty;
    ContextProperty *topEdgeProperty;
    ContextProperty *remoteTopEdgeProperty;
    ContextProperty *isFlatProperty;
    MServiceListener *remoteTopEdgeListener;
    //Properties set by window manager
    MContextProperty *currentWindowAngleProperty;
    MContextProperty *desktopAngleProperty;
    bool isSubscribedToSensorProperties;
#endif
    bool rotationsDisabled;
    bool pendingOrientationAngleUpdate;
    bool pendingCurrentAppWindowOrientationAngleChangeHandling;

    //windows from this list follow "/Screen/CurrentWindow/OrientationAngle" (set by meego window manager).
    //by default it includes modal dialogs.
    QList<QPointer<MWindow> > windowsFollowingCurrentAppWindow;
    //windows from this list will follow "/Screen/CurrentWindow/OrientationAngle" but with
    //regard to restrictions as orientation lock or device profile data.
    //This list includes windows off display which do not get iconic state (WM_STATE),
    //and windows which have _MEEGOTOUCH_ALWAYS_MAPPED property set to 1 or 2.
    QList<QPointer<MWindow> > windowsFollowingWithConstraintsCurrentAppWindow;
    void startFollowingCurrentAppWindow(MWindow* win, bool limitedByConstraints = false);
    void stopFollowingCurrentAppWindow(MWindow* win, bool limitedByConstraints = false);
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool currentWindowAnglePropertyContainsValidAngle();
#endif //HAVE_CONTEXTSUBSCRIBER

    void startFollowingDesktop(MWindow *win);
    void stopFollowingDesktop(MWindow *win);
    QList<QPointer<MWindow> > windowsFollowingDesktop;

public slots:
    void videoRouteChanged();
    void updateOrientationAngleOfWindows();
    void disableRotations();
    void enableRotations();
#ifdef HAVE_CONTEXTSUBSCRIBER
    void handleCurrentAppWindowOrientationAngleChange();
    void handleDesktopOrientationChange();
#endif // HAVE_CONTEXTSUBSCRIBER

protected:
    MOrientationTracker *q_ptr;

private:
    Q_DECLARE_PUBLIC(MOrientationTracker)
};

#endif // MORIENTATIONTRACKER_P_H
