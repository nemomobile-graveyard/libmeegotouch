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

#ifndef MDEVICEPROFILE_H
#define MDEVICEPROFILE_H

#include "mexport.h"
#include "mnamespace.h"
#include <QObject>

class QSize;
class MDeviceProfilePrivate;

/*! \brief The MDeviceProfile class holds physical and simulated properties of the target device

 */
class M_CORE_EXPORT MDeviceProfile : public QObject
{
    Q_OBJECT

public:
    //! Default constructor. Only used by MApplication class.
    //! Client classes should be using MDeviceProfile::instance() method.
    MDeviceProfile(QObject *parent = 0);

    //! Default destructor.
    virtual ~MDeviceProfile();

    //! Returns the global MDeviceProfile instance
    static MDeviceProfile *instance();

    //! Return the target device resolution
    QSize resolution() const;

    //! Return the target device pixels per inch
    QSize pixelsPerInch() const;

    //! Return whether the status bar should be shown on target device
    bool showStatusbar() const;

    /*!
      \brief Returns the resulting orientation of a device for a given rotation angle.

      The rotation is relative to the position where the device's display has its
      top edge on top.

      For devices whose display native resolution is landscape:
        \li Angle0 and Angle180 are landscape angles.
        \li Angle90 and Angle270 are portrait angles.

      For devices whose display native resolution is portrait:
        \li Angle0 and Angle180 are portrait angles.
        \li Angle90 and Angle270 are landscape angles.

        orientationFromAngle(M::Angle0) returns the display's native orientation.
     */
    M::Orientation orientationFromAngle(M::OrientationAngle angle) const;

    //! Returns true if orientation angle is supported for given keyboard state.
    bool orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const;

    //! Returns the number of pixels required to display a length of \a mm millimeters on this device.
    int mmToPixels(qreal mm);

    //! Returns the number of pixels required to display a length of \a mm millimeters on this device using floating point precision.
    qreal mmToPixelsF(qreal mm);

    //! Returns point size in pixels on this device.
    int ptToPixels(qreal pt);

    //! Returns point size in pixels using floating point precision on this device.
    qreal ptToPixelsF(qreal pt);

    //! Returns the result of pixels to points conversion
    int pixelsToPt(int pixels);

    //! Returns the result of pixels to points conversion
    qreal pixelsToPtF(qreal pixels);


protected:
    MDeviceProfilePrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MDeviceProfile)
    Q_DECLARE_PRIVATE(MDeviceProfile)
};

#endif
