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

#include <DuiTheme>

#include "duicontainer.h"
#include "duicontainer_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiContainer)


DuiContainerPrivate::DuiContainerPrivate()
{
}

DuiContainerPrivate::~DuiContainerPrivate()
{
}

void DuiContainerPrivate::init(const QString &newTitle)
{
    Q_Q(DuiContainer);

    q->model()->setCentralWidget(new DuiWidget(q));

    q->setTitle(newTitle);

    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

DuiContainer::DuiContainer(QGraphicsItem *parent)
    : DuiWidgetController(new DuiContainerPrivate(), new DuiContainerModel(), parent)
{
    Q_D(DuiContainer);

    d->init();
}

DuiContainer::DuiContainer(const QString &title, QGraphicsItem *parent)
    : DuiWidgetController(new DuiContainerPrivate(), new DuiContainerModel(), parent)
{
    Q_D(DuiContainer);

    d->init(title);
}

DuiContainer::~DuiContainer()
{
}

QGraphicsWidget *DuiContainer::centralWidget()
{
    return model()->centralWidget();
}

void DuiContainer::setCentralWidget(QGraphicsWidget *centralWidget, bool destroy)
{
    if (centralWidget != NULL) {
        const QGraphicsWidget *oldCentralWidget = model()->centralWidget();

        // Set the new central widget
        model()->setCentralWidget(centralWidget);

        // Destroy the old central widget if requested
        if (destroy) {
            delete oldCentralWidget;
        }
    }
}

QString DuiContainer::title() const
{
    return model()->title();
}

void DuiContainer::setTitle(const QString &newTitle)
{
    model()->setTitle(newTitle);
}

QString DuiContainer::text() const
{
    return model()->text();
}

void DuiContainer::setText(const QString &text)
{
    model()->setText(text);
}

QString DuiContainer::iconID() const
{
    return model()->icon();
}

void DuiContainer::setIconID(const QString &iconID)
{
    model()->setIcon(iconID);
}

bool DuiContainer::headerVisible() const
{
    return model()->headerVisible();
}

void DuiContainer::setHeaderVisible(const bool visibility)
{
    model()->setHeaderVisible(visibility);
}

void DuiContainer::toggleHeaderVisible()
{
    setHeaderVisible(!model()->headerVisible());
}

bool DuiContainer::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void DuiContainer::setProgressIndicatorVisible(bool visibility)
{
    model()->setProgressIndicatorVisible(visibility);
}

void DuiContainer::toggleProgressIndicatorVisible()
{
    setProgressIndicatorVisible(!isProgressIndicatorVisible());
}
