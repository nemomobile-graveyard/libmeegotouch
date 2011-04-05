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

#include <MTheme>

#include "mcontainer.h"
#include "mcontainer_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MContainer)


MContainerPrivate::MContainerPrivate()
{
}

MContainerPrivate::~MContainerPrivate()
{
}

void MContainerPrivate::_q_onCentralWidgetDestroyed()
{
    Q_Q(MContainer);
    q->model()->setCentralWidget(0);
}

void MContainerPrivate::init(const QString &newTitle)
{
    Q_Q(MContainer);

    q->model()->setCentralWidget(new MWidget(q));

    q->setTitle(newTitle);

    q->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MContainer::MContainer(QGraphicsItem *parent)
    : MWidgetController(new MContainerPrivate(), new MContainerModel(), parent)
{
    Q_D(MContainer);

    d->init();
}

MContainer::MContainer(const QString &title, QGraphicsItem *parent)
    : MWidgetController(new MContainerPrivate(), new MContainerModel(), parent)
{
    Q_D(MContainer);

    d->init(title);
}

MContainer::~MContainer()
{
}

QGraphicsWidget *MContainer::centralWidget()
{
    return model()->centralWidget();
}

void MContainer::setCentralWidget(QGraphicsWidget *centralWidget, bool destroy)
{
    QGraphicsWidget *oldCentralWidget = model()->centralWidget();

    // Set the new central widget
    model()->setCentralWidget(centralWidget);

    // Destroy the old central widget if requested
    if (destroy) {
        delete oldCentralWidget;

        if (centralWidget)
            connect(centralWidget, SIGNAL(destroyed()), SLOT(_q_onCentralWidgetDestroyed()));
    } else {
        // pass the ownership back to the caller.
        oldCentralWidget->setParentItem(0);
        oldCentralWidget->setParent(0);
        if (scene())
            scene()->removeItem(oldCentralWidget);

        if (oldCentralWidget)
            disconnect(oldCentralWidget, SIGNAL(destroyed()), this, SLOT(_q_onCentralWidgetDestroyed()));
    }

}

QString MContainer::title() const
{
    return model()->title();
}

void MContainer::setTitle(const QString &newTitle)
{
    model()->setTitle(newTitle);
}

QString MContainer::text() const
{
    return model()->text();
}

void MContainer::setText(const QString &text)
{
    model()->setText(text);
}

QString MContainer::iconID() const
{
    return model()->icon();
}

void MContainer::setIconID(const QString &iconID)
{
    model()->setIcon(iconID);
}

bool MContainer::headerVisible() const
{
    return model()->headerVisible();
}

void MContainer::setHeaderVisible(const bool visibility)
{
    model()->setHeaderVisible(visibility);
}

void MContainer::toggleHeaderVisible()
{
    setHeaderVisible(!model()->headerVisible());
}

bool MContainer::isProgressIndicatorVisible() const
{
    return model()->progressIndicatorVisible();
}

void MContainer::setProgressIndicatorVisible(bool visibility)
{
    model()->setProgressIndicatorVisible(visibility);
}

void MContainer::toggleProgressIndicatorVisible()
{
    setProgressIndicatorVisible(!isProgressIndicatorVisible());
}

#include "moc_mcontainer.cpp"
