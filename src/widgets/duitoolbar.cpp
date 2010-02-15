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

#include "duitoolbar.h"
#include "duitoolbar_p.h"

#include <DuiDebug>
#include <QGraphicsLinearLayout>

#include "duitheme.h"
#include "duiwidgetaction.h"
#include "duibutton.h"
#include "duibuttongroup.h"
#include "duiwidget.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiToolBar)

DuiToolBarPrivate::DuiToolBarPrivate()
{
}

DuiToolBarPrivate::~DuiToolBarPrivate()
{
}

void DuiToolBarPrivate::initLayout()
{
    Q_Q(DuiToolBar);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    q->setLayout(layout);
}

DuiToolBar::DuiToolBar(QGraphicsItem *parent, const QString &viewType)
    : DuiWidgetController(new DuiToolBarPrivate(), new DuiWidgetModel(), parent)
{
    Q_D(DuiToolBar);
    d->initLayout();
    Q_UNUSED(viewType);
}

DuiToolBar::DuiToolBar(DuiToolBarPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    Q_D(DuiToolBar);
    d->initLayout();
}

DuiToolBar::~DuiToolBar()
{
}

