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

#include "mondisplaychangeevent.h"
#include "mondisplaychangeevent_p.h"

static int mOnDisplayChangeEventNumber = -1;

MOnDisplayChangeEventPrivate::MOnDisplayChangeEventPrivate(MOnDisplayChangeEvent::State newState, const QRectF &newViewRect) :
    state(newState),
    viewRect(newViewRect)
{}

MOnDisplayChangeEventPrivate::~MOnDisplayChangeEventPrivate()
{}

MOnDisplayChangeEvent::MOnDisplayChangeEvent(State state, const QRectF &viewRect) :
    QGraphicsSceneEvent(MOnDisplayChangeEvent::eventNumber()),
    d_ptr(new MOnDisplayChangeEventPrivate(state, viewRect))
{}

MOnDisplayChangeEvent::MOnDisplayChangeEvent(bool visible, const QRectF &viewRect) :
    QGraphicsSceneEvent(MOnDisplayChangeEvent::eventNumber()),
    d_ptr(new MOnDisplayChangeEventPrivate(visible ? FullyOnDisplay : FullyOffDisplay, viewRect))
{}

MOnDisplayChangeEvent::State MOnDisplayChangeEvent::state() const
{
    Q_D(const MOnDisplayChangeEvent);
    return d->state;
}

QRectF MOnDisplayChangeEvent::viewRect() const
{
    Q_D(const MOnDisplayChangeEvent);
    return d->viewRect;
}

MOnDisplayChangeEvent::~MOnDisplayChangeEvent()
{
    delete d_ptr;
}

//Static
QEvent::Type MOnDisplayChangeEvent::eventNumber()
{
    if (mOnDisplayChangeEventNumber < 0)
        mOnDisplayChangeEventNumber = QEvent::registerEventType();
    return (QEvent::Type)mOnDisplayChangeEventNumber;
}
