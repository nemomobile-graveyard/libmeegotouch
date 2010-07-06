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
 
#include "mbasiclistitem.h"
#include "mbasiclistitem_p.h"

#include <MLabel>
#include <MImageWidget>

#include <MWidgetStyle>

#include <QGraphicsGridLayout>
#include <QGraphicsWidget>

MBasicListItemPrivate::MBasicListItemPrivate(MBasicListItem::ItemStyle style)
    : q_ptr(NULL),
    layoutGrid(NULL),
    image(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    isLayoutInitialized(false),
    listItemStyle(style)
{
}

MBasicListItemPrivate::~MBasicListItemPrivate()
{
}

void MBasicListItemPrivate::createLayout()
{
    Q_Q(MBasicListItem);

    layoutGrid = new QGraphicsGridLayout(q);
    layoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    layoutGrid->setContentsMargins(0, 0, 0, 0);
    layoutGrid->setSpacing(0);

    switch (listItemStyle) {
    case MBasicListItem::SingleTitle: {
            q->titleLabelWidget()->setObjectName("CommonSingleTitle");
            layout()->addItem(q->titleLabelWidget(), 0, 0);
            break;
        }
    case MBasicListItem::TitleWithSubtitle: {
            q->titleLabelWidget()->setObjectName("CommonTitle");
            layout()->addItem(q->titleLabelWidget(), 0, 0);
            layout()->addItem(q->subtitleLabelWidget(), 1, 0);
            layout()->addItem(new QGraphicsWidget(q), 2, 0);
            break;
        }
    case MBasicListItem::IconWithTitle: {
            q->titleLabelWidget()->setObjectName("CommonSingleTitle");
            layout()->addItem(q->imageWidget(), 0, 0);
            layout()->addItem(q->titleLabelWidget(), 0, 1);
            break;
        }
    case MBasicListItem::IconWithTitleAndSubtitle: {
            q->titleLabelWidget()->setObjectName("CommonTitle");
            layout()->addItem(q->imageWidget(), 0, 0, 3, 1);
            layout()->addItem(q->titleLabelWidget(), 0, 1);
            layout()->addItem(q->subtitleLabelWidget(), 1, 1);
            layout()->addItem(new QGraphicsWidget(q), 2, 1);
            break;
        }
    default:
        break;
    }
}

void MBasicListItemPrivate::clearLayout()
{
    if (layout()) {
        for (int i = 0; i < layout()->count(); i++) {
            QGraphicsLayoutItem *item = layout()->itemAt(0);
            layout()->removeAt(0);
            delete item;
        }
        image = NULL;
        titleLabel = NULL;
        subtitleLabel = NULL;
    }
}

QGraphicsGridLayout *MBasicListItemPrivate::layout()
{
    return layoutGrid;
}

MBasicListItem::MBasicListItem(MBasicListItem::ItemStyle style, QGraphicsItem *parent)
    : MListItem(parent), d_ptr(new MBasicListItemPrivate(style))
{
    Q_D(MBasicListItem);
    d->q_ptr = this;

    setItemStyle(style);
    setObjectName("CommonPanel");
}

MBasicListItem::~MBasicListItem()
{
    delete d_ptr;
}

void MBasicListItem::initLayout()
{
    Q_D(MBasicListItem);

    if (d->isLayoutInitialized)
        return;

    setLayout(createLayout());
    d->isLayoutInitialized = true;
}

QGraphicsLayout *MBasicListItem::createLayout()
{
    Q_D(MBasicListItem);

    clearLayout();
    d->createLayout();

    return d->layout();
}

void MBasicListItem::clearLayout()
{
    Q_D(MBasicListItem);

    d->clearLayout();
}

void MBasicListItem::setItemStyle(ItemStyle itemStyle)
{
    Q_D(MBasicListItem);

    if (itemStyle == d->listItemStyle)
        return;

    d->listItemStyle = itemStyle;
    initLayout();
}

MBasicListItem::ItemStyle MBasicListItem::itemStyle() const
{
    Q_D(const MBasicListItem);

    return d->listItemStyle;
}

void MBasicListItem::setImageWidget(MImageWidget *imageWidget)
{
    Q_D(MBasicListItem);

    if (d->image) {
        for (int i = 0; i < d->layout()->count(); i++) {
            if (d->layout()->itemAt(i) == d->image) {
                d->layout()->removeAt(i);
                delete d->image;
                d->image = NULL;
                break;
            }
        }
    }

    if (imageWidget) {
        d->image = imageWidget;
        if (d->listItemStyle == MBasicListItem::IconWithTitle)
            d->layout()->addItem(d->image, 0, 0);
        else if (d->listItemStyle == MBasicListItem::IconWithTitleAndSubtitle)
            d->layout()->addItem(d->image, 0, 0, 3, 1);
    }
}

MImageWidget *MBasicListItem::imageWidget()
{
    Q_D(MBasicListItem);

    if (!d->image) {
        d->image = new MImageWidget(this);
        d->image->setObjectName("CommonMainIcon");
    }
    return d->image;
}

MLabel *MBasicListItem::titleLabelWidget()
{
    Q_D(MBasicListItem);

    if (!d->titleLabel) {
        d->titleLabel = new MLabel(this);
        d->titleLabel->setTextElide(true);
        d->titleLabel->setObjectName("CommonTitle");
    }

    return d->titleLabel;
}

MLabel *MBasicListItem::subtitleLabelWidget()
{
    Q_D(MBasicListItem);

    if (!d->subtitleLabel) {
        d->subtitleLabel = new MLabel(this);
        d->subtitleLabel->setObjectName("CommonSubTitle");
    }

    return d->subtitleLabel;
}

void MBasicListItem::setTitle(const QString &title)
{
    titleLabelWidget()->setText(title);
}

QString MBasicListItem::title()
{
    return titleLabelWidget()->text();
}

void MBasicListItem::setSubtitle(const QString &subtitle)
{
    subtitleLabelWidget()->setText(subtitle);
}

QString MBasicListItem::subtitle()
{
    return subtitleLabelWidget()->text();
}

void MBasicListItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MListItem::resizeEvent(event);
    initLayout();
}

