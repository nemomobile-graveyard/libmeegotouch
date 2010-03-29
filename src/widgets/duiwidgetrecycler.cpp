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

#include <DuiWidget>
#include "duiwidgetrecycler.h"
#include "duiwidgetrecycler_p.h"

DuiWidgetRecyclerPrivate::DuiWidgetRecyclerPrivate()
    : widgetCount(0), maxWidgetCount(10)
{
}

DuiWidgetRecyclerPrivate::~DuiWidgetRecyclerPrivate()
{
    qDeleteAll(widgets);
}

void DuiWidgetRecyclerPrivate::resetWidgetState(DuiWidget *widget)
{
    // lets just move it far far away, so that noone can see it
    widget->setPos(-QWIDGETSIZE_MAX, -QWIDGETSIZE_MAX);
    //widget->setVisible(false);
    //widget->setParentItem(NULL);
    //widget->setParentLayoutItem(NULL);
}

bool DuiWidgetRecyclerPrivate::hasEnoughSpaceFor(DuiWidget *widget)
{
    return widgets.count(widget->metaObject()->className()) < maxWidgetCount;
}

void DuiWidgetRecyclerPrivate::put(DuiWidget *widget)
{
    widgets.insert(widget->metaObject()->className(), widget);
}

DuiWidget *DuiWidgetRecyclerPrivate::take(const QString &className)
{
    return widgets.take(className);
}

DuiWidgetRecycler::DuiWidgetRecycler()
    : d_ptr(new DuiWidgetRecyclerPrivate())
{
}

DuiWidgetRecycler::~DuiWidgetRecycler()
{
    delete d_ptr;
}

DuiWidgetRecycler *DuiWidgetRecycler::instance()
{
    static DuiWidgetRecycler recycler;
    return &recycler;
}

void DuiWidgetRecycler::setMaxItemsPerClass(int count)
{
    d_ptr->maxWidgetCount = count;
}

int DuiWidgetRecycler::maxItemsPerClass() const
{
    return d_ptr->maxWidgetCount;
}

void DuiWidgetRecycler::recycle(DuiWidget *widget)
{
    if (widget) {
        if (d_ptr->hasEnoughSpaceFor(widget)) {
            d_ptr->resetWidgetState(widget);
            d_ptr->put(widget);
        } else {
            delete widget;
        }
    }
}

DuiWidget *DuiWidgetRecycler::take(const QString &className)
{
    DuiWidget *widget = d_ptr->take(className);

    return widget;
}
