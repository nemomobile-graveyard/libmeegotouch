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

#include "morientationchangeevent.h"
#include "morientationchangeevent_p.h"

static int mOrientationChangeEventNumber = -1;

MOrientationChangeEvent::MOrientationChangeEvent(M::Orientation newOrientation) :
    QGraphicsSceneEvent(MOrientationChangeEvent::eventNumber()),
    d_ptr(new MOrientationChangeEventPrivate(newOrientation))
{
}

MOrientationChangeEvent::~MOrientationChangeEvent()
{
    delete d_ptr;
}

QEvent::Type MOrientationChangeEvent::eventNumber()
{
    if (mOrientationChangeEventNumber < 0)
        mOrientationChangeEventNumber = QEvent::registerEventType();
    return (QEvent::Type)mOrientationChangeEventNumber;
}

M::Orientation MOrientationChangeEvent::orientation() const
{
    Q_D(const MOrientationChangeEvent);
    return d->orientation;
}

MOrientationChangeEventPrivate::MOrientationChangeEventPrivate(M::Orientation o) :
    orientation(o)
{
}
