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

#ifndef DUIDEVICEPROFILE_H
#define DUIDEVICEPROFILE_H

#include "duiexport.h"
#include "duinamespace.h"
#include <QObject>

class QSize;
class DuiDeviceProfilePrivate;

/*! \brief The DuiDeviceProfile class holds physical and simulated properties of the target device

  Target device properties are specified in CSS as part of the DirectUI theme, through the attributes of the DuiDeviceStyle class.
 */
class DUI_EXPORT DuiDeviceProfile : public QObject
{
    Q_OBJECT

public:
    //! Default constructor. Only used by DuiApplication class.
    //! Client classes should be using DuiDeviceProfile::instance() method.
    DuiDeviceProfile(QObject *parent = 0);

    //! Default destructor.
    virtual ~DuiDeviceProfile();

    //! Returns the global DuiDeviceProfile instance
    static DuiDeviceProfile *instance();

    //! Return the target device resolution
    QSize resolution() const;

    //! Return the target device pixels per inch
    QSize pixelsPerInch() const;

protected:
    DuiDeviceProfilePrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiDeviceProfile)
    Q_DECLARE_PRIVATE(DuiDeviceProfile)
};

#endif
