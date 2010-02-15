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

#include "duigriditem.h"
#include "duigriditem_p.h"
#include <duiwidgetstyle.h>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiGridItem)


DuiGridItemPrivate::DuiGridItemPrivate():
    DuiWidgetControllerPrivate()
{
}

DuiGridItemPrivate::~DuiGridItemPrivate()
{
}

DuiGridItem::DuiGridItem(QGraphicsItem *parent):
    DuiWidgetController(new DuiGridItemPrivate(), new DuiGridItemModel(), parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

DuiGridItem::DuiGridItem(DuiGridItemPrivate *dd, DuiGridItemModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

DuiGridItem::~DuiGridItem()
{
}

QString DuiGridItem::image() const
{
    return model()->image();
}

QPixmap DuiGridItem::pixmap() const
{
    Q_D(const DuiGridItem);
    return d->pixmap;
}

QString DuiGridItem::title() const
{
    return model()->title();
}

QString DuiGridItem::subtitle() const
{
    return model()->subtitle();
}

bool DuiGridItem::isImageVisible() const
{
    return model()->imageVisible();
}

bool DuiGridItem::isTitleVisible() const
{
    return model()->titleVisible();
}

bool DuiGridItem::isSubtitleVisible() const
{
    return model()->subtitleVisible();
}

void DuiGridItem::setSelected(bool selected)
{
    if (selected)
        style().setModeSelected();
    else
        style().setModeDefault();

    DuiWidgetController::setSelected(selected);
}

void DuiGridItem::setImage(const QString &id)
{
    model()->setImage(id);
}

void DuiGridItem::setPixmap(const QPixmap &pixmap)
{
    Q_D(DuiGridItem);
    d->pixmap = pixmap;

    if (!model()->image().isEmpty()) {
        // will make view update
        model()->setImage("");
    } else {
        emit pixmapChanged();
    }
}

void DuiGridItem::setTitle(const QString &text)
{
    model()->setTitle(text);
}

void DuiGridItem::setSubtitle(const QString &text)
{
    model()->setSubtitle(text);
}

void DuiGridItem::setImageVisible(bool visible)
{
    model()->setImageVisible(visible);
}

void DuiGridItem::setTitleVisible(bool visible)
{
    model()->setTitleVisible(visible);
}

void DuiGridItem::setSubtitleVisible(bool visible)
{
    model()->setSubtitleVisible(visible);
}

