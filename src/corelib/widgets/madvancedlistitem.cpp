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

#include "madvancedlistitem.h"
#include "madvancedlistitem_p.h"

#include <MImageWidget>
#include <MLabel>
#include <MProgressIndicator>

#include <QGraphicsGridLayout>

MAdvancedListItemPrivate::MAdvancedListItemPrivate(MAdvancedListItem::ItemStyle style)
    : layoutGrid(NULL),
    progress(NULL),
    titleLabel(NULL),
    image(NULL),
    sideTopImage(NULL),
    sideBottomImage(NULL),
    listItemStyle(style)
{

}

MAdvancedListItemPrivate::~MAdvancedListItemPrivate()
{
}

void MAdvancedListItemPrivate::createLayout()
{
    Q_Q(MAdvancedListItem);

    switch (listItemStyle) {
    case MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons: {
            q->setObjectName("AdvancedListItemIconWithTitleProgressIndicatorAndTwoSideIcons");

            layout()->addItem(q->imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layout()->addItem(q->titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layout()->addItem(q->progressIndicator(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layout()->addItem(q->sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            layout()->addItem(q->sideBottomImageWidget(), 1, 2, Qt::AlignRight | Qt::AlignBottom);
            break;
        }
    case MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon: {
            q->setObjectName("AdvancedListItemIconWithTitleProgressIndicatorAndTopSideIcon");

            layout()->addItem(q->imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layout()->addItem(q->titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layout()->addItem(q->progressIndicator(), 1, 1, 1, 2, Qt::AlignLeft | Qt::AlignBottom);
            layout()->addItem(q->sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            break;
        }
    default:
        break;
    }
}

void MAdvancedListItemPrivate::clearLayout()
{
    Q_Q(MAdvancedListItem);

    delete layoutGrid;
    layoutGrid = new QGraphicsGridLayout(q);
    layoutGrid->setContentsMargins(0, 0, 0, 0);
    layoutGrid->setSpacing(0);
}

QGraphicsGridLayout *MAdvancedListItemPrivate::layout()
{
    return layoutGrid;
}

MAdvancedListItem::MAdvancedListItem(MAdvancedListItem::ItemStyle style, QGraphicsItem *parent)
    : MListItem(parent), d_ptr(new MAdvancedListItemPrivate(style))
{
    Q_D(MAdvancedListItem);
    d->q_ptr = this;
}

MAdvancedListItem::~MAdvancedListItem()
{
}

void MAdvancedListItem::initLayout()
{
    setLayout(createLayout());
}

void MAdvancedListItem::setItemStyle(ItemStyle itemStyle)
{
    Q_D(MAdvancedListItem);

    if (itemStyle == d->listItemStyle)
        return;

    d->listItemStyle = itemStyle;
    initLayout();
}

MAdvancedListItem::ItemStyle MAdvancedListItem::itemStyle() const
{
    Q_D(const MAdvancedListItem);

    return d->listItemStyle;
}

QString MAdvancedListItem::title()
{
    return titleLabelWidget()->text();
}

void MAdvancedListItem::setTitle(const QString &title)
{
    titleLabelWidget()->setText(title);
}

MImageWidget * MAdvancedListItem::imageWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->image) {
        d->image = new MImageWidget(this);
        d->image->setObjectName("CommonMainIcon");
    }

    return d->image;
}

void MAdvancedListItem::setImageWidget(MImageWidget * imageWidget)
{
    Q_D(MAdvancedListItem);

    if (d->image) {
        for (int i = 0; i < d->layout()->count(); i++) {
            if (d->layout()->itemAt(i) == d->image) {
                d->layout()->removeAt(i);
                break;
            }
        }
        delete d->image;
        d->image = NULL;
    }

    if (imageWidget) {
        d->image = imageWidget;
        d->layout()->addItem(d->image, 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MProgressIndicator * MAdvancedListItem::progressIndicator()
{
    Q_D(MAdvancedListItem);

    if (!d->progress) {
        d->progress = new MProgressIndicator(this, MProgressIndicator::barType);
        d->progress->setObjectName("CommonProgressIndicator");
    }

    return d->progress;
}

MLabel * MAdvancedListItem::titleLabelWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->titleLabel) {
        d->titleLabel = new MLabel(this);
        d->titleLabel->setObjectName("CommonTitleWithLeftMargin");
    }

    return d->titleLabel;
}

MImageWidget * MAdvancedListItem::sideTopImageWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->sideTopImage) {
        d->sideTopImage = new MImageWidget(this);
        d->sideTopImage->setObjectName("CommonTopSideIcon");
    }

    return d->sideTopImage;
}

MImageWidget * MAdvancedListItem::sideBottomImageWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->sideBottomImage) {
        d->sideBottomImage = new MImageWidget(this);
        d->sideBottomImage->setObjectName("CommonBottomSideIcon");
    }

    return d->sideBottomImage;
}

QGraphicsLayout *MAdvancedListItem::createLayout()
{
    Q_D(MAdvancedListItem);

    clearLayout();
    d->createLayout();

    return d->layout();
}

void MAdvancedListItem::clearLayout()
{
    Q_D(MAdvancedListItem);

    d->clearLayout();
}


