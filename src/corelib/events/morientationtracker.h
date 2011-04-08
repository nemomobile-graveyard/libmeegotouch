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

#ifndef MORIENTATIONTRACKER_H
#define MORIENTATIONTRACKER_H

#include <QObject>

#include "mnamespace.h"
#include "mexport.h"

class MOrientationTrackerPrivate;

//! \internal

/*!
 * Class responsible for tracking the accelerometer properties and signaling events
 * when the device should change its orientation.
 * At the moment this is not a part of M api - use MDeviceProfile for the information.
 * Initially designed as singleton as MDeviceProfile might need to access some more information about
 * the concrete phone position, that is not available from fired events.
 */
class M_CORE_EXPORT MOrientationTracker: public QObject
{
    Q_OBJECT
public:
    static MOrientationTracker *instance();

    M::OrientationAngle orientationAngle() const;

Q_SIGNALS:
    void faceFlippedDown();
    void faceUp();

protected:
    MOrientationTrackerPrivate *const d_ptr;

private:
    MOrientationTracker();
    virtual ~MOrientationTracker();

    Q_DISABLE_COPY(MOrientationTracker)
    Q_DECLARE_PRIVATE(MOrientationTracker)

    friend class MDeviceProfile;
    friend class MApplicationPrivate;
    friend class MWindow;
    friend class MWindowPrivate;

    //! \reimp
    virtual void childEvent(QChildEvent *event);
    //! \reimp_end

#ifdef UNIT_TEST
    friend class Ut_MOrientationTracker;
#endif

    // For testing purposes
    void doUpdateOrientationAngle(M::OrientationAngle angle, bool isKeyboardOpen,
                                  bool isDeviceFlat, bool tvIsConnected);
    void disableRotations();
    void enableRotations();

#ifdef Q_WS_X11
    void handleCurrentAppWindowChange();
    void handleCurrentAppWindowOrientationAngleChange();
#endif
};

//! \internal_end

#endif // MORIENTATIONTRACKER_H
