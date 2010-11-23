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

#include "mwidgetaction.h"
#include "mwidgetaction_p.h"

#include <QPointer>
#include <QEvent>

#include "mwidget.h"

MWidgetActionPrivate::MWidgetActionPrivate()
    : widget(0)
    , widgetInUse(false)
{
}

MWidgetActionPrivate::~MWidgetActionPrivate()
{
    if (widget) {
        delete widget;
        widget = 0;
    }
}

MWidgetAction::MWidgetAction(QObject *parent)
    : MAction(*new MWidgetActionPrivate, parent)
{
}

MWidgetAction::~MWidgetAction()
{
}

void MWidgetAction::setWidget(MWidget *widget)
{
    Q_D(MWidgetAction);

    if (widget == d->widget || d->widgetInUse)
        return;

    // we are the owner, delete evtl. set widget
    delete d->widget;

    // set new default widget
    d->widget = widget;
    if (widget) {
        setVisible(widget->isVisible());

        d->widget->hide();
        d->widget->setParentItem(0);
        d->widgetInUse = false;

        if (! isEnabled())
            d->widget->setEnabled(false);
    }

    d->sendDataChanged();
}

MWidget *MWidgetAction::widget() const
{
    Q_D(const MWidgetAction);

    return d->widget;
}

MWidget *MWidgetAction::requestWidget(MWidget *parent)
{
    Q_D(MWidgetAction);

    if (d->widgetInUse || !d->widget)
        return 0;

    d->widget->setParentItem(parent);
    d->widgetInUse = true;

    return d->widget;
}

void MWidgetAction::releaseWidget(MWidget *widget)
{
    Q_D(MWidgetAction);

    if (widget == d->widget && d->widget) {
        d->widget->hide();
        d->widget->setParentItem(0);
        d->widgetInUse = false;
    }
}

bool MWidgetAction::isWidgetInUse() const
{
    Q_D(const MWidgetAction);
    return d->widgetInUse;
}

bool MWidgetAction::event(QEvent *event)
{
    Q_D(MWidgetAction);

    if (event->type() == QEvent::ActionChanged) {
        if (d->widget)
            d->widget->setEnabled(isEnabled());
    }
    return MAction::event(event);
}

bool MWidgetAction::eventFilter(QObject *obj, QEvent *event)
{
    return MAction::eventFilter(obj, event);
}
