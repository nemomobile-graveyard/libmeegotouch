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

#include "duiinputmethodstate.h"
#include "duiinputmethodstate_p.h"


DuiInputMethodStatePrivate::DuiInputMethodStatePrivate()
    : orientation(Dui::Angle0)
{
}

DuiInputMethodState::DuiInputMethodState()
    : d_ptr(new DuiInputMethodStatePrivate)
{
}

DuiInputMethodState::~DuiInputMethodState()
{
    delete d_ptr;
}

DuiInputMethodState *DuiInputMethodState::instance()
{
    static DuiInputMethodState singleton;

    return &singleton;
}

void DuiInputMethodState::setInputMethodArea(const QRect &newRegion)
{
    Q_D(DuiInputMethodState);

    if (d->region != newRegion) {
        d->region = newRegion;
        emit inputMethodAreaChanged(newRegion);
    }
}

QRect DuiInputMethodState::inputMethodArea() const
{
    Q_D(const DuiInputMethodState);

    return d->region;
}

void DuiInputMethodState::setActiveWindowOrientationAngle(Dui::OrientationAngle newOrientation)
{
    Q_D(DuiInputMethodState);

    if (d->orientation != newOrientation) {
        d->orientation = newOrientation;
        emit activeWindowOrientationAngleChanged(newOrientation);
    }
}

Dui::OrientationAngle DuiInputMethodState::activeWindowOrientationAngle() const
{
    Q_D(const DuiInputMethodState);

    return d->orientation;
}
