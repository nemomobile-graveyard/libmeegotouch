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

#include "morientationtracker.h"
#include "morientationtracker_p.h"
#include "mkeyboardstatetracker.h"
#include "mapplication.h"
#include "mwindow.h"
#include "mcomponentdata.h"
#include "mdeviceprofile.h"
#include <QCoreApplication>
#include <QVariant>
#include <QTimer>

#include <MDebug>

MOrientationTracker *MOrientationTrackerPrivate::tracker = 0;

MOrientationTrackerPrivate::MOrientationTrackerPrivate(MOrientationTracker *controller) :
    currentAngle(M::Angle0),
    currentIsCovered(false)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , currentIsKeyboardOpen(MKeyboardStateTracker::instance()->isOpen())
    , topEdgeProperty("Screen.TopEdge")
    , isCoveredProperty("Screen.IsCovered")
#endif
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

    //do not change orientation when closing keyboard and old orientation is allowed:
    if (currentIsKeyboardOpen == true && isKeyboardOpen == false
        && MDeviceProfile::instance()->orientationAngleIsSupported(currentAngle, isKeyboardOpen)){
        currentIsKeyboardOpen = isKeyboardOpen;
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
            if (!window->isOrientationAngleLocked()) {
                if (!window->isOrientationLocked() || window->orientation() == orientation)
                    window->setOrientationAngle(angle);
            }
        }
    }
#endif
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
