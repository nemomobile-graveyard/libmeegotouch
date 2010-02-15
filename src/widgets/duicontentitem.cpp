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

#include <QPixmap>
#include <duiwidgetstyle.h>
#include "duicontentitem.h"
#include "duicontentitem_p.h"
#include "duicontentitemmodel.h"
#include <QDebug>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiContentItem)

DuiContentItemPrivate::DuiContentItemPrivate():
    DuiWidgetControllerPrivate()
{
}

DuiContentItemPrivate::~DuiContentItemPrivate()
{
}

DuiContentItem::DuiContentItem(DuiContentItem::ContentItemStyle itemStyle, QGraphicsItem *parent)
    : DuiWidgetController(new DuiContentItemPrivate, new DuiContentItemModel, parent)
{
    model()->setItemStyle(itemStyle);
}

DuiContentItem::DuiContentItem(DuiContentItemPrivate *dd, DuiContentItemModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
}

DuiContentItem::~DuiContentItem()
{

}

QPixmap DuiContentItem::pixmap() const
{
    Q_D(const DuiContentItem);
    return d->pixmap;
}

QString DuiContentItem::title() const
{
    return model()->title();
}

QString DuiContentItem::subtitle() const
{
    return model()->subtitle();
}

DuiContentItem::ContentItemStyle DuiContentItem::itemStyle() const
{
    return (DuiContentItem::ContentItemStyle) model()->itemStyle();
}

DuiContentItem::ContentItemMode DuiContentItem::itemMode() const
{
    return (DuiContentItem::ContentItemMode) model()->itemMode();
}

void DuiContentItem::setPixmap(const QPixmap &pixmap)
{
    Q_D(DuiContentItem);
    d->pixmap = pixmap;
    model()->setItemImage(d->pixmap);
}

void DuiContentItem::setTitle(const QString &text)
{
    model()->setTitle(text);
}

void DuiContentItem::setSubtitle(const QString &text)
{
    model()->setSubtitle(text);
}

void DuiContentItem::setItemMode(ContentItemMode mode)
{
    model()->setItemMode(mode);
}

void DuiContentItem::click()
{
    emit clicked();
}

QVariant DuiContentItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        model()->setSelected(value.toBool());
    }

    return QGraphicsItem::itemChange(change, value);
}
