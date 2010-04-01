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

#ifndef DUIORIENTATIONTRACKER_P_H
#define DUIORIENTATIONTRACKER_P_H

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#include <QObject>
#include <duinamespace.h>

class DuiOrientationTracker;

class DuiOrientationTrackerPrivate : public QObject
{
    Q_OBJECT
public:
    DuiOrientationTrackerPrivate(DuiOrientationTracker *controller);
    static DuiOrientationTracker *tracker;
    Dui::OrientationAngle currentAngle;
    bool currentIsCovered;

#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty topEdgeProperty;
    ContextProperty isCoveredProperty;
#endif

public slots:
    void topEdgeChanged();
    void isCoveredChanged();

protected:
    DuiOrientationTracker *q_ptr;

private slots:
    void initContextSubscriber();
    void updateOrientationAngle();

private:
    Q_DECLARE_PUBLIC(DuiOrientationTracker)
};

#endif // DUIORIENTATIONTRACKER_P_H
