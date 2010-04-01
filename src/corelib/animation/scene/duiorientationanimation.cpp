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

#include "duiorientationanimation.h"

#if QT_VERSION >= 0x040600


#include "duiorientationanimation_p.h"

///////////////////
// Private class //
///////////////////

DuiOrientationAnimationPrivate::DuiOrientationAnimationPrivate() :
    rootElement(0)
{
}

DuiOrientationAnimationPrivate::~DuiOrientationAnimationPrivate()
{
}

//////////////////
// Public class //
//////////////////

DuiOrientationAnimation::DuiOrientationAnimation(DuiOrientationAnimationPrivate *d,
        QObject *parent, DuiGroupAnimation::Type type) :
    DuiGroupAnimation(d, type, parent)
{
}

DuiOrientationAnimation::DuiOrientationAnimation(QObject *parent, DuiGroupAnimation::Type type) :
    DuiGroupAnimation(new DuiOrientationAnimationPrivate(), type, parent)
{
}

void DuiOrientationAnimation::setRootElement(QGraphicsWidget *rootElement)
{
    Q_D(DuiOrientationAnimation);
    if (d->rootElement != rootElement) {
        d->rootElement = rootElement;
        rootElementChanged();
    }
}

QGraphicsWidget *DuiOrientationAnimation::rootElement()
{
    Q_D(DuiOrientationAnimation);
    return d->rootElement;
}

void DuiOrientationAnimation::rootElementChanged()
{
}
#endif
