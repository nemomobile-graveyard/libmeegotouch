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

#include "mlistitem.h"
#include "mlistitem_p.h"

#include "mwidgetcreator.h"

M_REGISTER_WIDGET(MListItem)
      
MListItemPrivate::MListItemPrivate()
    : MWidgetControllerPrivate()
{
}

MListItemPrivate::~MListItemPrivate()
{
}

void MListItemPrivate::updateLongTapConnections()
{
    Q_Q(MListItem);
    if (q->receivers(SIGNAL(longTapped(QPointF))) > 0)
        q->grabGestureWithCancelPolicy(Qt::TapAndHoldGesture, Qt::GestureFlags(), MWidget::MouseEventCancelOnGestureFinished);
    else
        q->ungrabGesture(Qt::TapAndHoldGesture);
}

MListItem::MListItem(QGraphicsItem *parent)
    : MWidgetController(new MListItemPrivate, new MListItemModel, parent)
{
}

MListItem::~MListItem()
{
}

void MListItem::click()
{
    emit clicked();
}

void MListItem::longTap(const QPointF &pos)
{
    emit longTapped(pos);
}

void MListItem::connectNotify(const char *signal)
{
    Q_D(MListItem);

    if (QLatin1String(signal) == SIGNAL(longTapped(QPointF))) {
        d->updateLongTapConnections();
    }
}

void MListItem::disconnectNotify(const char *signal)
{
    Q_D(MListItem);

    if (QLatin1String(signal) == SIGNAL(longTapped(QPointF))) {
        d->updateLongTapConnections();
    }
}
