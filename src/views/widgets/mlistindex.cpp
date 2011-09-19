/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mlistindex.h"
#include "mlistindex_p.h"
#include "mlistindexview.h"
#include "mlist.h"

#include <QGraphicsSceneContextMenuEvent>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MListIndex)

MListIndexPrivate::MListIndexPrivate()
    : q_ptr(NULL)
{
}

MListIndexPrivate::~MListIndexPrivate()
{
}

void MListIndexPrivate::init()
{
    Q_Q(MListIndex);

    // Force creating the view.
    q->setView(new MListIndexView(q));

    q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    q->grabGestureWithCancelPolicy(Qt::PanGesture, Qt::GestureFlags(), MWidget::MouseEventCancelOnGestureStarted);
}

void MListIndexPrivate::updateListConnections(MList *list)
{
    Q_Q(MListIndex);

    if (q->model()->list())
        q->model()->list()->disconnect(q);

    if (list)
        q->connect(list, SIGNAL(visibleChanged()), q, SLOT(_q_updateVisibility()));
}

void MListIndexPrivate::_q_updateVisibility()
{
    Q_Q(MListIndex);

    if (q->model()->list()) {
        if (q->model()->list()->isVisible() && q->model()->displayMode() != MList::Hide)
            q->show();
        else
            q->hide();
    }
}

MListIndex::MListIndex(QGraphicsItem *parent)
    : MWidgetController(new MListIndexModel, parent),
    d_ptr(new MListIndexPrivate)
{
    Q_D(MListIndex);
    d->q_ptr = this;
    d->init();
}

MListIndex::~MListIndex()
{
    delete d_ptr;
}

void MListIndex::setList(MList *list)
{
    Q_D(MListIndex);

    d->updateListConnections(list);
    model()->setList(list);
}

void MListIndex::setDisplayMode(MList::DisplayMode displayMode)
{
    model()->setDisplayMode(displayMode);
}

MList::DisplayMode MListIndex::displayMode()
{
    return (MList::DisplayMode)model()->displayMode();
}

void MListIndex::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}

void MListIndex::updateGroupTitles()
{
    emit groupTitlesChanged();
}

#include "moc_mlistindex.cpp"
