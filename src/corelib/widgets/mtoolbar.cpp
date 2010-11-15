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

#include "mtoolbar.h"
#include "mtoolbar_p.h"

#include <MDebug>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneContextMenuEvent>

#include "mtheme.h"
#include "mwidgetaction.h"
#include "mbutton.h"
#include "mbuttongroup.h"
#include "mwidget.h"

#include "mwidgetcreator.h"

M_REGISTER_WIDGET(MToolBar)

const MTheme::ViewType MToolBar::tabType = "tab";

MToolBarPrivate::MToolBarPrivate()
{
}

MToolBarPrivate::~MToolBarPrivate()
{
}

void MToolBarPrivate::initLayout()
{
    Q_Q(MToolBar);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(q);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
}

MToolBar::MToolBar(QGraphicsItem *parent, const QString &viewType)
    : MWidgetController(new MToolBarPrivate(), new MWidgetModel(), parent)
{
    Q_D(MToolBar);
    d->initLayout();
    Q_UNUSED(viewType);
}

MToolBar::MToolBar(MToolBarPrivate *dd, MWidgetModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MToolBar);
    d->initLayout();
}

MToolBar::~MToolBar()
{
}

void MToolBar::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

