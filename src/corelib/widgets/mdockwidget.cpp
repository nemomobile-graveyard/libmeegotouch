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

#include <QGraphicsLinearLayout>
#include <MDebug>

#include "mdockwidget_p.h"
#include "mdockwidget.h"

#include "mdockwidgetmodel.h"
#include "mtheme.h"
#include "mtoolbar.h"
#include "mapplication.h"
#include "mscenemanager.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MDockWidget)

MDockWidgetPrivate::MDockWidgetPrivate()
    : toolbar(0)
{
}

MDockWidgetPrivate::~MDockWidgetPrivate()
{
}

void MDockWidgetPrivate::init()
{
    Q_Q(MDockWidget);

    // set size policies for layoutduiapplicationwindow_p and widget itself
    layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    q->setLayout(layout);
}

MDockWidget::MDockWidget(QGraphicsWidget *parent)
    : MSceneWindow(new MDockWidgetPrivate(), new MDockWidgetModel(), MSceneWindow::DockWidget, "", parent)
{
    Q_D(MDockWidget);

    d->init();
}

MDockWidget::~MDockWidget()
{
    // make sure we have nothing docked that would be deleted implicitly
    undockToolBar();
}

void MDockWidget::dockToolBar(MToolBar *toolbar)
{
    Q_D(MDockWidget);

    // undock previous toolbar if any was there
    if (0 != d->toolbar)
        undockToolBar();

    // keep track of this toolbar
    d->toolbar = toolbar;

    if (0 != toolbar) {
        d->layout->addItem(toolbar);
        d->toolbar->setParentItem(this);
        d->toolbar->show();
    }
}

void MDockWidget::undockToolBar()
{
    Q_D(MDockWidget);

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

MToolBar *MDockWidget::toolBar()
{
    Q_D(MDockWidget);
    return d->toolbar;
}
