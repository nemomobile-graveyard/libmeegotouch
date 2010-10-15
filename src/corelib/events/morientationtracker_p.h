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

class MOrientationTracker;

class MOrientationTrackerPrivate : public QObject
{
    Q_OBJECT
public:
    MOrientationTrackerPrivate(MOrientationTracker *controller);
    static MOrientationTracker *tracker;
    M::OrientationAngle currentAngle;
    bool currentIsCovered;
    bool currentIsTvConnected;
    bool currentIsKeyboardOpen;
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty videoRouteProperty;
    ContextProperty topEdgeProperty;
    ContextProperty isCoveredProperty;
#endif

public slots:
    void isCoveredChanged();
    void videoRouteChanged();

protected:
    MOrientationTracker *q_ptr;

private slots:
    void initContextSubscriber();
    void updateOrientationAngle();

private:
    Q_DECLARE_PUBLIC(MOrientationTracker)
};

#endif // MORIENTATIONTRACKER_P_H
