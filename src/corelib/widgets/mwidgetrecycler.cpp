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

#include <MWidget>
#include "mwidgetrecycler.h"
#include "mwidgetrecycler_p.h"

const char * MWidgetRecycler::RecycledObjectIdentifier = "_q_RecycleIdentifier";

MWidgetRecyclerPrivate::MWidgetRecyclerPrivate()
    : widgetCount(0), maxWidgetCount(10)
{
}

MWidgetRecyclerPrivate::~MWidgetRecyclerPrivate()
{
    qDeleteAll(widgets);
}

void MWidgetRecyclerPrivate::resetWidgetState(MWidget *widget)
{
    // lets just move it far far away, so that noone can see it
    widget->setPos(-QWIDGETSIZE_MAX, -QWIDGETSIZE_MAX);
    //widget->setVisible(false);
    //widget->setParentItem(NULL);
    //widget->setParentLayoutItem(NULL);
}

bool MWidgetRecyclerPrivate::hasEnoughSpaceFor(MWidget *widget)
{
    return widgets.count(widget->metaObject()->className()) < maxWidgetCount;
}

void MWidgetRecyclerPrivate::put(MWidget *widget)
{
    QVariant recycleIdentifier = widget->property(MWidgetRecycler::RecycledObjectIdentifier);

    if(!recycleIdentifier.isValid())
        widgets.insert(widget->metaObject()->className(), widget);
    else
        widgets.insert(recycleIdentifier.toString(), widget);
}

MWidget *MWidgetRecyclerPrivate::take(const QString &recycleId)
{
    return widgets.take(recycleId);
}

MWidgetRecycler::MWidgetRecycler()
    : d_ptr(new MWidgetRecyclerPrivate())
{
}

MWidgetRecycler::~MWidgetRecycler()
{
    delete d_ptr;
}

MWidgetRecycler *MWidgetRecycler::instance()
{
    static MWidgetRecycler recycler;
    return &recycler;
}

void MWidgetRecycler::setMaxItemsPerClass(int count)
{
    d_ptr->maxWidgetCount = count;
}

int MWidgetRecycler::maxItemsPerClass() const
{
    return d_ptr->maxWidgetCount;
}

void MWidgetRecycler::recycle(MWidget *widget)
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

MWidget *MWidgetRecycler::take(const QString &recycleId)
{
    MWidget *widget = d_ptr->take(recycleId);

    return widget;
}
