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

#include "duiaction.h"
#include "duiaction_p.h"

#include <QApplication>
#include <QEvent>
#include <QActionEvent>
#include <QGraphicsWidget>

DuiActionPrivate::DuiActionPrivate()
    : location(DuiAction::EveryLocation), styleAction(false)
{
}

DuiActionPrivate::~DuiActionPrivate()
{
}

void DuiActionPrivate::sendDataChanged()
{
    Q_Q(DuiAction);
    QActionEvent e(QEvent::ActionChanged, q);

    QList<QGraphicsWidget *> gWidgets = q->associatedGraphicsWidgets();

    const int size = gWidgets.size();
    for (int i = 0; i < size; ++i) {
        QGraphicsWidget *w = gWidgets.at(i);
        QApplication::sendEvent(w, &e);
    }

    QApplication::sendEvent(q, &e);

    emit q->changed();
}

DuiAction::DuiAction(QObject *parent)
    : QAction(parent),
      d_ptr(new DuiActionPrivate)
{
    Q_D(DuiAction);
    d->q_ptr = this;
}

DuiAction::DuiAction(DuiActionPrivate &dd, QObject *parent)
    : QAction(parent),
      d_ptr(& dd)
{
    Q_D(DuiAction);
    d->q_ptr = this;
}

DuiAction::DuiAction(const QString &text, QObject *parent)
    : QAction(text, parent),
      d_ptr(new DuiActionPrivate)
{
    Q_D(DuiAction);
    d->q_ptr = this;
}

DuiAction::DuiAction(const QString &iconID, const QString &text, QObject *parent)
    : QAction(text, parent),
      d_ptr(new DuiActionPrivate)
{
    Q_D(DuiAction);
    d->iconID = iconID;
    d->q_ptr = this;
}

DuiAction::~DuiAction()
{
    delete d_ptr;
}

QString DuiAction::iconID() const
{
    Q_D(const DuiAction);
    return d->iconID;
}

void DuiAction::setIconID(const QString &iconID)
{
    Q_D(DuiAction);
    if (d->iconID == iconID)
        return;

    d->iconID = iconID;
    d->sendDataChanged();
}

DuiAction::Locations DuiAction::location() const
{
    Q_D(const DuiAction);
    return d->location;
}

void DuiAction::setLocation(DuiAction::Locations location)
{
    Q_D(DuiAction);
    if (d->location == location)
        return;

    d->location = location;
    d->sendDataChanged();
}

void DuiAction::setStyleAction(bool styleAction)
{
    Q_D(DuiAction);
    if (d->styleAction != styleAction) {
        d->styleAction = styleAction;
        d->sendDataChanged();
    }
}

bool DuiAction::isStyleAction() const
{
    Q_D(const DuiAction);
    return d->styleAction;
}
