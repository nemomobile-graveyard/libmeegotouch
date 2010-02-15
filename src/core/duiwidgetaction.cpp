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

#include "duiwidgetaction.h"
#include "duiwidgetaction_p.h"

#include <QPointer>
#include <QEvent>

#include "duiwidget.h"

DuiWidgetActionPrivate::DuiWidgetActionPrivate()
    : widget(0)
    , widgetInUse(false)
{
}

DuiWidgetActionPrivate::~DuiWidgetActionPrivate()
{
    if (widget) {
        delete widget;
        widget = 0;
    }
}

DuiWidgetAction::DuiWidgetAction(QObject *parent)
    : DuiAction(*new DuiWidgetActionPrivate, parent)
{
}

DuiWidgetAction::~DuiWidgetAction()
{
}

void DuiWidgetAction::setWidget(DuiWidget *widget)
{
    Q_D(DuiWidgetAction);

    if (widget == d->widget || d->widgetInUse)
        return;

    // we are the owner, delete evtl. set widget
    delete d->widget;

    // set new default widget
    d->widget = widget;
    if (!widget)
        return;

    setVisible(widget->isVisible());

    d->widget->hide();
    d->widget->setParentItem(0);
    d->widgetInUse = false;

    if (! isEnabled())
        d->widget->setEnabled(false);
}

DuiWidget *DuiWidgetAction::widget() const
{
    Q_D(const DuiWidgetAction);

    return d->widget;
}

DuiWidget *DuiWidgetAction::requestWidget(DuiWidget *parent)
{
    Q_D(DuiWidgetAction);

    if (d->widgetInUse || !d->widget)
        return 0;

    d->widget->setParentItem(parent);
    d->widgetInUse = true;

    return d->widget;
}

void DuiWidgetAction::releaseWidget(DuiWidget *widget)
{
    Q_D(DuiWidgetAction);

    if (widget == d->widget && d->widget) {
        d->widget->hide();
        d->widget->setParentItem(0);
        d->widgetInUse = false;
    }
}

bool DuiWidgetAction::isWidgetInUse() const
{
    Q_D(const DuiWidgetAction);
    return d->widgetInUse;
}

bool DuiWidgetAction::event(QEvent *event)
{
    Q_D(DuiWidgetAction);

    if (event->type() == QEvent::ActionChanged) {
        if (d->widget)
            d->widget->setEnabled(isEnabled());
    }
    return DuiAction::event(event);
}

bool DuiWidgetAction::eventFilter(QObject *obj, QEvent *event)
{
    return DuiAction::eventFilter(obj, event);
}
