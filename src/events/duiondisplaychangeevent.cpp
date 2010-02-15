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

#include "duiondisplaychangeevent.h"
#include "duiondisplaychangeevent_p.h"

static int duiOnDisplayChangeEventNumber = -1;

DuiOnDisplayChangeEventPrivate::DuiOnDisplayChangeEventPrivate(DuiOnDisplayChangeEvent::State newState, const QRectF &newViewRect) :
    state(newState),
    viewRect(newViewRect)
{}

DuiOnDisplayChangeEventPrivate::~DuiOnDisplayChangeEventPrivate()
{}

DuiOnDisplayChangeEvent::DuiOnDisplayChangeEvent(State state, const QRectF &viewRect) :
    QGraphicsSceneEvent(DuiOnDisplayChangeEvent::eventNumber()),
    d_ptr(new DuiOnDisplayChangeEventPrivate(state, viewRect))
{}

DuiOnDisplayChangeEvent::DuiOnDisplayChangeEvent(bool visible, const QRectF &viewRect) :
    QGraphicsSceneEvent(DuiOnDisplayChangeEvent::eventNumber()),
    d_ptr(new DuiOnDisplayChangeEventPrivate(visible ? FullyOnDisplay : FullyOffDisplay, viewRect))
{}

DuiOnDisplayChangeEvent::State DuiOnDisplayChangeEvent::state() const
{
    Q_D(const DuiOnDisplayChangeEvent);
    return d->state;
}

QRectF DuiOnDisplayChangeEvent::viewRect() const
{
    Q_D(const DuiOnDisplayChangeEvent);
    return d->viewRect;
}

DuiOnDisplayChangeEvent::~DuiOnDisplayChangeEvent()
{
    delete d_ptr;
}

//Static
QEvent::Type DuiOnDisplayChangeEvent::eventNumber()
{
    if (duiOnDisplayChangeEventNumber < 0)
        duiOnDisplayChangeEventNumber = QEvent::registerEventType();
    return (QEvent::Type)duiOnDisplayChangeEventNumber;
}
