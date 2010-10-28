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

#ifndef MDEVICEPROFILE_P_H
#define MDEVICEPROFILE_P_H

#include "mdeviceprofile.h"

class MDeviceStyle;

class MDeviceProfilePrivate
{
    Q_DECLARE_PUBLIC(MDeviceProfile)

public:
    MDeviceProfilePrivate();
    virtual ~MDeviceProfilePrivate();

    bool load(const QString& filename);

    void autodetection();
    bool hasStatusbarProvider();

    QSize resolution;
    QSize pixelsPerInch;
    bool showStatusBar;

    QSet<M::OrientationAngle> supportedOrientationsForKeyboardOpen;
    QSet<M::OrientationAngle> supportedOrientationsForKeyboardClosed;
protected:
    MDeviceProfile *q_ptr;
private:
    int pixelsPerMm;
    qreal pixelsPerMmF;
    int pixelsPerPt;
    qreal pixelsPerPtF;
};

#endif
