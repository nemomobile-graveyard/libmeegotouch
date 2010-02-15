/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duiorientationtracker.h"
#include "duiorientationtracker_p.h"
#include "duiapplication.h"
#include "duiwindow.h"
#include <QCoreApplication>
#include <QVariant>
#include <QTimer>

#include <DuiDebug>

DuiOrientationTracker *DuiOrientationTrackerPrivate::tracker = 0;

DuiOrientationTrackerPrivate::DuiOrientationTrackerPrivate(DuiOrientationTracker *controller) :
    currentAngle(Dui::Angle0),
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
#endif

    initContextSubscriber();
}

void DuiOrientationTrackerPrivate::initContextSubscriber()
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

void DuiOrientationTrackerPrivate::topEdgeChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Dui::OrientationAngle angle;
    Dui::Orientation orientation;
    QString edge = topEdgeProperty.value().toString();

    if (edge == "top") {
        angle = Dui::Angle0;
        orientation = Dui::Landscape;
    } else if (edge == "left") {
        angle = Dui::Angle270;
        orientation = Dui::Portrait;
    } else if (edge == "right") {
        angle = Dui::Angle90;
        orientation = Dui::Portrait;
    } else if (edge == "bottom") {
        angle = Dui::Angle180;
        orientation = Dui::Landscape;
    } else {
        angle = currentAngle;
    }

    if (angle != currentAngle) {
        currentAngle = angle;
        // instead of emitting a signal we have to explicitely call setOrientationAngle
        // on windows, because this is very often excuted before the application's
        // event loop is started and leads to crash in QMetaObject
        foreach(DuiWindow * window, DuiApplication::windows()) {
            if (!window->isOrientationAngleLocked()) {
                if (!window->isOrientationLocked() || window->orientation() == orientation)
                    window->setOrientationAngle(angle);
            }
        }
    }
#endif
}

void DuiOrientationTrackerPrivate::isCoveredChanged()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_Q(DuiOrientationTracker);

    bool isCovered = isCoveredProperty.value().toBool();

    if (isCovered != currentIsCovered) {
        duiDebug("DuiOrientationTracker") << "Covered:" << isCovered;

        currentIsCovered = isCovered;
        if (isCovered)
            emit q->faceFlippedDown();
        else
            emit q->faceUp();
    }
#endif
}

DuiOrientationTracker::DuiOrientationTracker() :
    d_ptr(new DuiOrientationTrackerPrivate(this))
{
    setParent(QCoreApplication::instance()); //get collected when needed.
}

DuiOrientationTracker *DuiOrientationTracker::instance()
{
    if (!DuiOrientationTrackerPrivate::tracker)
        DuiOrientationTrackerPrivate::tracker = new DuiOrientationTracker();
    return DuiOrientationTrackerPrivate::tracker;
}

DuiOrientationTracker::~DuiOrientationTracker()
{
    if (this == DuiOrientationTrackerPrivate::tracker)
        DuiOrientationTrackerPrivate::tracker = 0;
}

Dui::OrientationAngle DuiOrientationTracker::orientationAngle() const
{
    Q_D(const DuiOrientationTracker);

    return d->currentAngle;
}
