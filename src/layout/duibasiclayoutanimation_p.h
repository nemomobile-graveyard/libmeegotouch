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

#ifndef DUIBASICLAYOUTANIMATION_P_H
#define DUIBASICLAYOUTANIMATION_P_H

#include "duibasiclayoutanimation.h"
#include "duilayoutanimation_p.h"

#include <QtCore/QSet>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QEasingCurve>

/** Private animator class. */
class DuiBasicLayoutAnimationPrivate : public DuiLayoutAnimationPrivate
{
    Q_DECLARE_PUBLIC(DuiBasicLayoutAnimation)
public:
    /** Constructor */
    DuiBasicLayoutAnimationPrivate();
    /** Destructor */
    virtual ~DuiBasicLayoutAnimationPrivate();

    /** Keep track of the time elapsed since the last update.
      * This is used because we may miss some timer events.
      */
    QTime timeSinceLastUpdate;
    /** The number of milliseconds elapsed since last update
     *
     * This records the value of timeSinceLastUpdate when an update happens
     */
    qreal recordedTimeSinceLastUpdate;
    /** Trigger animation step. */
    void tick();
};

#endif // Header Guard
