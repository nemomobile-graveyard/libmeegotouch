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

#include "duiorientationchangeevent.h"
#include "duiorientationchangeevent_p.h"

static int duiOrientationChangeEventNumber = -1;

DuiOrientationChangeEvent::DuiOrientationChangeEvent(Dui::Orientation newOrientation) :
    QGraphicsSceneEvent(DuiOrientationChangeEvent::eventNumber()),
    d_ptr(new DuiOrientationChangeEventPrivate(newOrientation))
{
}

DuiOrientationChangeEvent::~DuiOrientationChangeEvent()
{
    delete d_ptr;
}

QEvent::Type DuiOrientationChangeEvent::eventNumber()
{
    if (duiOrientationChangeEventNumber < 0)
        duiOrientationChangeEventNumber = QEvent::registerEventType();
    return (QEvent::Type)duiOrientationChangeEventNumber;
}

Dui::Orientation DuiOrientationChangeEvent::orientation() const
{
    Q_D(const DuiOrientationChangeEvent);
    return d->orientation;
}

DuiOrientationChangeEventPrivate::DuiOrientationChangeEventPrivate(Dui::Orientation o) :
    orientation(o)
{
}
