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

#include <QGraphicsLinearLayout>
#include <DuiDebug>

#include "duidockwidget_p.h"
#include "duidockwidget.h"

#include "duidockwidgetmodel.h"
#include "duitheme.h"
#include "duitoolbar.h"
#include "duiapplication.h"
#include "duiscenemanager.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiDockWidget)

DuiDockWidgetPrivate::DuiDockWidgetPrivate()
    : toolbar(0)
{
}

DuiDockWidgetPrivate::~DuiDockWidgetPrivate()
{
}

void DuiDockWidgetPrivate::init()
{
    Q_Q(DuiDockWidget);

    // set size policies for layoutduiapplicationwindow_p and widget itself
    layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    q->setLayout(layout);
}

DuiDockWidget::DuiDockWidget(QGraphicsWidget *parent)
    : DuiSceneWindow(new DuiDockWidgetPrivate(), new DuiDockWidgetModel(), DuiSceneWindow::DockWidget, "", parent)
{
    Q_D(DuiDockWidget);

    d->init();
}

DuiDockWidget::~DuiDockWidget()
{
    // make sure we have nothing docked that would be deleted implicitly
    undockToolBar();
}

void DuiDockWidget::dockToolBar(DuiToolBar *toolbar)
{
    Q_D(DuiDockWidget);

    // undock previous toolbar if any was there
    if (0 != d->toolbar)
        undockToolBar();

    // keep track of this toolbar
    d->toolbar = toolbar;

    if (0 != toolbar) {
        d->layout->addItem(toolbar);
        d->toolbar->setParentItem(this);
    }
}

void DuiDockWidget::undockToolBar()
{
    Q_D(DuiDockWidget);

    if (d->toolbar) {

        // actually remove toolbar from layout
        const int layoutCount = d->layout->count();
        for (int i = 0; i < layoutCount; ++i) {
            if (d->layout->itemAt(i) == d->toolbar) {
                d->layout->removeAt(i);
            }
        }
        d->toolbar->setParentItem(0);
        // don't keep track of toolbar anymore
        d->toolbar = 0;
    }
}

DuiToolBar *DuiDockWidget::toolBar()
{
    Q_D(DuiDockWidget);
    return d->toolbar;
}
