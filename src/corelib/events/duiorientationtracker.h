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

#ifndef DUIORIENTATIONTRACKER_H
#define DUIORIENTATIONTRACKER_H

#include <QObject>

#include "duinamespace.h"
#include "duiexport.h"

class DuiOrientationTrackerPrivate;

//! \internal

/*!
 * Class responsible for tracking the accelerometer properties and signaling events
 * when the device should change it's orientation.
 * At the moment this is not a part of DUI api - use DuiDeviceProfile for the information.
 * Initially designed as singleton as DuiDeviceProfile might need to access some more information about
 * the concrete phone position, that is not available from fired events.
 */
class DUI_EXPORT DuiOrientationTracker: public QObject
{
    Q_OBJECT
public:
    static DuiOrientationTracker *instance();

    Dui::OrientationAngle orientationAngle() const;

Q_SIGNALS:
    void faceFlippedDown();
    void faceUp();

protected:
    DuiOrientationTrackerPrivate *const d_ptr;

private:
    DuiOrientationTracker();
    virtual ~DuiOrientationTracker();

    Q_DISABLE_COPY(DuiOrientationTracker)
    Q_DECLARE_PRIVATE(DuiOrientationTracker)

    friend class DuiDeviceProfile;
};

//! \internal_end

#endif // DUIORIENTATIONTRACKER_H
