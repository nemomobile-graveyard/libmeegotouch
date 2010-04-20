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

#include "minputmethodstate.h"
#include "minputmethodstate_p.h"


MInputMethodStatePrivate::MInputMethodStatePrivate()
    : orientation(M::Angle0)
{
}

MInputMethodState::MInputMethodState()
    : d_ptr(new MInputMethodStatePrivate)
{
}

MInputMethodState::~MInputMethodState()
{
    delete d_ptr;
}

MInputMethodState *MInputMethodState::instance()
{
    static MInputMethodState singleton;

    return &singleton;
}

void MInputMethodState::setInputMethodArea(const QRect &newRegion)
{
    Q_D(MInputMethodState);

    if (d->region != newRegion) {
        d->region = newRegion;
        emit inputMethodAreaChanged(newRegion);
    }
}

QRect MInputMethodState::inputMethodArea() const
{
    Q_D(const MInputMethodState);

    return d->region;
}

void MInputMethodState::setActiveWindowOrientationAngle(M::OrientationAngle newOrientation)
{
    Q_D(MInputMethodState);

    if (d->orientation != newOrientation) {
        d->orientation = newOrientation;
        emit activeWindowOrientationAngleChanged(newOrientation);
    }
}

M::OrientationAngle MInputMethodState::activeWindowOrientationAngle() const
{
    Q_D(const MInputMethodState);

    return d->orientation;
}

int MInputMethodState::registerToolbar(const QString &fileName)
{
    static int idCounter = 0;
    // generate an application local unique identifier for the toolbar.
    int newId = idCounter;
    idCounter++;
    emit toolbarRegistered(newId, fileName);
    return newId;
}

void MInputMethodState::unregisterToolbar(int id)
{
    emit toolbarUnregistered(id);
}

void MInputMethodState::setToolbarItemAttribute(int id, const QString &item,
                                                const QString &attribute, const QVariant &value)
{
    emit toolbarItemAttributeChanged(id, item, attribute, value);
}
