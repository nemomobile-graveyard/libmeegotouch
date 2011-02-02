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

#include "maction.h"
#include "maction_p.h"

#include <QApplication>
#include <QEvent>
#include <QActionEvent>
#include <QGraphicsWidget>

MActionPrivate::MActionPrivate()
    : location(MAction::EveryLocation), styleAction(false), q_ptr(0)
{
}

MActionPrivate::~MActionPrivate()
{
}

void MActionPrivate::sendDataChanged()
{
    Q_Q(MAction);
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

MAction::MAction(QObject *parent)
    : QAction(parent),
      d_ptr(new MActionPrivate)
{
    Q_D(MAction);
    d->q_ptr = this;
}

MAction::MAction(MActionPrivate &dd, QObject *parent)
    : QAction(parent),
      d_ptr(& dd)
{
    Q_D(MAction);
    d->q_ptr = this;
}

MAction::MAction(const QString &text, QObject *parent)
    : QAction(text, parent),
      d_ptr(new MActionPrivate)
{
    Q_D(MAction);
    d->q_ptr = this;
}

MAction::MAction(const QString &iconID, const QString &text, QObject *parent)
    : QAction(text, parent),
      d_ptr(new MActionPrivate)
{
    Q_D(MAction);
    d->iconID = iconID;
    d->q_ptr = this;
}

MAction::~MAction()
{
    delete d_ptr;
}

QString MAction::iconID() const
{
    Q_D(const MAction);
    return d->iconID;
}

void MAction::setIconID(const QString &iconID)
{
    Q_D(MAction);
    if (d->iconID == iconID)
        return;

    d->iconID = iconID;
    d->sendDataChanged();
}

QString MAction::toggledIconID() const
{
    Q_D(const MAction);
    return d->toggledIconID;
}

void MAction::setToggledIconID(const QString &id)
{
    Q_D(MAction);
    if (d->toggledIconID == id)
        return;

    d->toggledIconID = id;
    d->sendDataChanged();
}

MAction::Locations MAction::location() const
{
    Q_D(const MAction);
    return d->location;
}

void MAction::setLocation(MAction::Locations location)
{
    Q_D(MAction);
    if (d->location == location)
        return;

    d->location = location;
    d->sendDataChanged();
}

void MAction::setStyleAction(bool styleAction)
{
    Q_D(MAction);
    if (d->styleAction != styleAction) {
        d->styleAction = styleAction;
        d->sendDataChanged();
    }
}

bool MAction::isStyleAction() const
{
    Q_D(const MAction);
    return d->styleAction;
}
