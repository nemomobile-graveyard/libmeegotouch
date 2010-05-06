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

#include "mgriditem.h"
#include "mgriditem_p.h"
#include <mwidgetstyle.h>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MGridItem)


MGridItemPrivate::MGridItemPrivate():
    MWidgetControllerPrivate()
{
}

MGridItemPrivate::~MGridItemPrivate()
{
}

MGridItem::MGridItem(QGraphicsItem *parent):
    MWidgetController(new MGridItemPrivate(), new MGridItemModel(), parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

MGridItem::MGridItem(MGridItemPrivate *dd, MGridItemModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
}

MGridItem::~MGridItem()
{
}

QString MGridItem::image() const
{
    return model()->image();
}

QPixmap MGridItem::pixmap() const
{
    Q_D(const MGridItem);
    return d->pixmap;
}

QString MGridItem::title() const
{
    return model()->title();
}

QString MGridItem::subtitle() const
{
    return model()->subtitle();
}

bool MGridItem::isImageVisible() const
{
    return model()->imageVisible();
}

bool MGridItem::isTitleVisible() const
{
    return model()->titleVisible();
}

bool MGridItem::isSubtitleVisible() const
{
    return model()->subtitleVisible();
}

void MGridItem::setSelected(bool selected)
{
    if (selected)
        style().setModeSelected();
    else
        style().setModeDefault();

    MWidgetController::setSelected(selected);
}

void MGridItem::setImage(const QString &id)
{
    model()->setImage(id);
}

void MGridItem::setPixmap(const QPixmap &pixmap)
{
    Q_D(MGridItem);
    d->pixmap = pixmap;

    if (!model()->image().isEmpty()) {
        // will make view update
        model()->setImage("");
    } else {
        emit pixmapChanged();
    }
}

void MGridItem::setTitle(const QString &text)
{
    model()->setTitle(text);
}

void MGridItem::setSubtitle(const QString &text)
{
    model()->setSubtitle(text);
}

void MGridItem::setImageVisible(bool visible)
{
    model()->setImageVisible(visible);
}

void MGridItem::setTitleVisible(bool visible)
{
    model()->setTitleVisible(visible);
}

void MGridItem::setSubtitleVisible(bool visible)
{
    model()->setSubtitleVisible(visible);
}

