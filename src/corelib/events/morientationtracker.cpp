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
#include <QCoreApplication>
#include <QVariant>
#include <QTimer>

#include <MDebug>

MOrientationTracker *MOrientationTrackerPrivate::tracker = 0;

MOrientationTrackerPrivate::MOrientationTrackerPrivate(MOrientationTracker *controller) :
    currentAngle(M::Angle0),
    currentIsCovered(false)
#ifdef HAVE_CONTEXTSUBSCRIBER
    , topEdgeProperty("Screen.TopEdge")
    , isCoveredProperty("Screen.IsCovered")
#endif
    , q_ptr(controller)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    connect(&topEdgeProperty, SIGNAL(valueChanged()),
            this, SLOT(topEdgeChanged()));
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
    topEdgeChanged();
    isCoveredChanged();
#endif
}

void MOrientationTrackerPrivate::topEdgeChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    M::OrientationAngle angle;
    M::Orientation orientation = M::Landscape;
    QString edge = topEdgeProperty.value().toString();

    if (edge == "top") {
        angle = M::Angle0;
    } else if (edge == "left") {
        angle = M::Angle270;
        orientation = M::Portrait;
    } else if (edge == "right") {
        //angle = M::Angle90; Disabled for now
        angle = M::Angle270;
        orientation = M::Portrait;
    } else if (edge == "bottom") {
        //angle = M::Angle180; Disabled for now
        angle = M::Angle0;
    } else {
        angle = M::Angle0;
    }

    // if hardware keyboard is opened, don't allow orientation angle to be changed.
    if (angle != currentAngle
        && !MKeyboardStateTracker::instance()->isOpen()) {
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
    if (MKeyboardStateTracker::instance()->isOpen()) {
        // if hardware keyboard is opened, set all window's orientation angle to 0.
        foreach (MWindow *window, MApplication::windows()) {
            if (!window->isOrientationAngleLocked()) {
                if (!window->isOrientationLocked() || window->orientation() == M::Landscape)
                    window->setOrientationAngle(M::Angle0);
            }
        }
        currentAngle = M::Angle0;
    } else {
        // if hardware keyboard is closed, set the real orientation.
        topEdgeChanged();
    }
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
