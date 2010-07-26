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

#include <QTimer>


MInputMethodStatePrivate::MInputMethodStatePrivate()
    : orientation(M::Angle0),
      emitInputMethodAreaTimer(new QTimer),
      q_ptr(NULL)
{
}

MInputMethodStatePrivate::~MInputMethodStatePrivate()
{
    delete emitInputMethodAreaTimer;
}

void MInputMethodStatePrivate::init()
{
    Q_Q(MInputMethodState);

    // Wait 75 milliseconds before sending updated area to allow glass
    // widget to correct focus. The focus may be wrongly set on the initial
    // mouse press.
    const int waitForEmitTime = 75;
    emitInputMethodAreaTimer->setSingleShot(true);
    emitInputMethodAreaTimer->setInterval(waitForEmitTime);

    q->connect(emitInputMethodAreaTimer, SIGNAL(timeout()),
               q, SLOT(_q_emitInputMethodAreaChanged()));
}

void MInputMethodStatePrivate::_q_emitInputMethodAreaChanged()
{
    Q_Q(MInputMethodState);
    emit q->inputMethodAreaChanged(region);
}

MInputMethodState::MInputMethodState()
    : d_ptr(new MInputMethodStatePrivate)
{
    Q_D(MInputMethodState);

    d->q_ptr = this;
    d->init();
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
        d->emitInputMethodAreaTimer->start();
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
    Q_D(MInputMethodState);
    static int idCounter = 0;
    // generate an application local unique identifier for the toolbar.
    int newId = idCounter;
    idCounter++;
    d->toolbars.insert(newId, fileName);
    emit toolbarRegistered(newId, fileName);
    return newId;
}

void MInputMethodState::unregisterToolbar(int id)
{
    Q_D(MInputMethodState);
    d->toolbars.remove(id);
    emit toolbarUnregistered(id);
}

void MInputMethodState::setToolbarItemAttribute(int id, const QString &item,
                                                const QString &attribute, const QVariant &value)
{
    emit toolbarItemAttributeChanged(id, item, attribute, value);
}

QString MInputMethodState::toolbar(int id) const
{
    Q_D(const MInputMethodState);
    return d->toolbars.value(id);
}

#include "moc_minputmethodstate.cpp"
