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
 
#include "mdetailedlistitem.h"
#include "mdetailedlistitem_p.h"

#include <MImageWidget>
#include <MLabel>
#include <MStylableWidget>

#include <QGraphicsGridLayout>

MDetailedListItemPrivate::MDetailedListItemPrivate(MDetailedListItem::ItemStyle style)
    : layoutGrid(NULL),
    image(NULL),
    sideTopImage(NULL),
    sideBottomImage(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    sideBottomLabel(NULL),
    listItemStyle(style)
{

}

MDetailedListItemPrivate::~MDetailedListItemPrivate()
{
}

void MDetailedListItemPrivate::createLayout()
{
    Q_Q(MDetailedListItem);

    if (!layoutGrid) {
        layoutGrid = new QGraphicsGridLayout(q);
        layoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        layoutGrid->setContentsMargins(0, 0, 0, 0);
        layoutGrid->setSpacing(0);
    }

    switch (listItemStyle) {
    case MDetailedListItem::IconTitleSubtitleAndTwoSideIcons: {
            q->setObjectName("DetailedListItemIconTitleSubtitleAndTwoSideIcons");
            q->setIconStyle(MDetailedListItem::Icon);

            layout()->addItem(q->imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layout()->addItem(q->titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layout()->addItem(q->subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layout()->addItem(q->sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            layout()->addItem(q->sideBottomImageWidget(), 1, 2, Qt::AlignRight | Qt::AlignBottom);

            break;
        }
    case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            q->setObjectName("DetailedListItemIconTitleSubtitleAndSideIconWithLabel");
            q->setIconStyle(MDetailedListItem::Icon);

            layout()->addItem(q->imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layout()->addItem(q->titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layout()->addItem(q->subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layout()->addItem(q->sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignTop);
            layout()->addItem(q->sideBottomLabelWidget(), 1, 2, Qt::AlignRight | Qt::AlignBottom);

            break;
        }
    case MDetailedListItem::ThumbnailTitleAndTwoSideIcons: {
            contentPanel = new MStylableWidget(q);
            contentLayoutGrid = new QGraphicsGridLayout(contentPanel);
            contentLayoutGrid->setContentsMargins(0, 0, 0, 0);
            contentLayoutGrid->setSpacing(0);
            contentLayoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            contentPanel->setObjectName("DetailedListItemThumbnailTitleAndTwoSideIcons");
            contentPanel->setContentsMargins(0, 8, 16, 8);

            q->setIconStyle(MDetailedListItem::Thumbnail);

            layout()->addItem(q->imageWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

            contentLayoutGrid->addItem(q->titleLabelWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(q->sideTopImageWidget(), 0, 1, Qt::AlignRight | Qt::AlignTop);
            contentLayoutGrid->addItem(new QGraphicsWidget(q), 1, 1, Qt::AlignRight | Qt::AlignVCenter);
            contentLayoutGrid->addItem(q->sideBottomImageWidget(), 2, 1, Qt::AlignRight | Qt::AlignBottom);

            contentPanel->setLayout(contentLayoutGrid);
            layout()->addItem(contentPanel, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

            break;
        }
    case MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons: {
            contentPanel = new MStylableWidget(q);
            contentLayoutGrid = new QGraphicsGridLayout(contentPanel);
            contentLayoutGrid->setContentsMargins(0, 0, 0, 0);
            contentLayoutGrid->setSpacing(0);
            contentLayoutGrid->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

            contentPanel->setObjectName("DetailedListItemThumbnailTitleSubtitleAndTwoSideIcons");
            contentPanel->setContentsMargins(0, 8, 16, 8);

            q->setIconStyle(MDetailedListItem::Thumbnail);

            layout()->addItem(q->imageWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

            contentLayoutGrid->addItem(q->titleLabelWidget(), 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(q->subtitleLabelWidget(), 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
            contentLayoutGrid->addItem(q->sideTopImageWidget(), 0, 1, Qt::AlignRight | Qt::AlignTop);
            contentLayoutGrid->addItem(q->sideBottomImageWidget(), 1, 1, Qt::AlignRight | Qt::AlignBottom);

            contentPanel->setLayout(contentLayoutGrid);
            layout()->addItem(contentPanel, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

            break;
        }
    default:
        break;
    }
}

void MDetailedListItemPrivate::clearLayout()
{
    if (layout()) {
        for (int i = 0; i < layout()->count(); i++) {
            QGraphicsLayoutItem *item = layoutGrid->itemAt(0);
            layoutGrid->removeAt(0);
            delete item;
        }
        image = NULL;
        titleLabel = NULL;
        subtitleLabel = NULL;
        sideTopImage = NULL;
        sideBottomImage = NULL;
        sideBottomLabel = NULL;

        if (contentLayoutGrid) {
            for (int i = 0; i < contentLayoutGrid->count(); i++) {
                QGraphicsLayoutItem *item = contentLayoutGrid->itemAt(0);
                contentLayoutGrid->removeAt(0);
                delete item;
            }
            delete contentLayoutGrid;
            contentLayoutGrid = NULL;
            delete contentPanel;
            contentPanel = NULL;
        }
    }
}

QGraphicsGridLayout *MDetailedListItemPrivate::layout()
{
    return layoutGrid;
}

MDetailedListItem::MDetailedListItem(MDetailedListItem::ItemStyle style, QGraphicsItem *parent)
    : MListItem(parent), d_ptr(new MDetailedListItemPrivate(style))
{
    Q_D(MDetailedListItem);
    d->q_ptr = this;

    setItemStyle(style);
}

MDetailedListItem::~MDetailedListItem()
{
}

void MDetailedListItem::initLayout()
{
    setLayout(createLayout());
}

QGraphicsLayout *MDetailedListItem::createLayout()
{
    Q_D(MDetailedListItem);

    clearLayout();
    d->createLayout();

    return d->layout();
}

void MDetailedListItem::clearLayout()
{
    Q_D(MDetailedListItem);

    d->clearLayout();
}

void MDetailedListItem::setItemStyle(ItemStyle itemStyle)
{
    Q_D(MDetailedListItem);

    if (itemStyle == d->listItemStyle)
        return;
    
    initLayout();
}

MDetailedListItem::ItemStyle MDetailedListItem::itemStyle() const
{
    Q_D(const MDetailedListItem);
    return d->listItemStyle;
}

void MDetailedListItem::setIconStyle(IconStyle style)
{
    if (style == Thumbnail)
        imageWidget()->setObjectName("CommonThumbnail");
    else if (style == Icon)
        imageWidget()->setObjectName("CommonMainIcon");
}

void MDetailedListItem::setImageWidget(MImageWidget *image)
{
    Q_D(MDetailedListItem);

    for (int i = 0; i < d->layout()->count(); i++) {
        if (d->layout()->itemAt(i) == d->image) {
            d->layout()->removeAt(i);
            delete d->image;
            d->image = NULL;
            break;
        }
    }

    if (image) {
        d->image = image;
        if (d->listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons)
            d->layout()->addItem(d->image, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
        else if (d->listItemStyle == MDetailedListItem::ThumbnailTitleAndTwoSideIcons ||
                 d->listItemStyle == MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons)
            d->layout()->addItem(d->image, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MImageWidget *MDetailedListItem::imageWidget()
{
    Q_D(MDetailedListItem);

    if (!d->image) {
        d->image = new MImageWidget(this);
    }
    return d->image;
}

MImageWidget *MDetailedListItem::sideTopImageWidget()
{
    Q_D(MDetailedListItem);

    if (!d->sideTopImage) {
        d->sideTopImage = new MImageWidget(this);
        d->sideTopImage->setObjectName("CommonTopSideIcon");
    }
    return d->sideTopImage;
}

MImageWidget *MDetailedListItem::sideBottomImageWidget()
{
    Q_D(MDetailedListItem);

    if (!d->sideBottomImage) {
        d->sideBottomImage = new MImageWidget(this);
        d->sideBottomImage->setObjectName("CommonBottomSideIcon");
    }
    return d->sideBottomImage;
}

MLabel *MDetailedListItem::titleLabelWidget()
{
    Q_D(MDetailedListItem);

    if (!d->titleLabel) {
        d->titleLabel = new MLabel(this);
        d->titleLabel->setObjectName("CommonTitleWithLeftMargin");
    }

    return d->titleLabel;
}

void MDetailedListItem::setTitle(const QString &title)
{
    titleLabelWidget()->setText(title);
}

QString MDetailedListItem::title()
{
    return titleLabelWidget()->text();
}

MLabel *MDetailedListItem::subtitleLabelWidget()
{
    Q_D(MDetailedListItem);

    if (!d->subtitleLabel) {
        d->subtitleLabel = new MLabel(this);
        d->subtitleLabel->setObjectName("CommonSubTitleWithLeftMargin");
    }

    return d->subtitleLabel;
}

void MDetailedListItem::setSubtitle(const QString &subtitle)
{
    subtitleLabelWidget()->setText(subtitle);
}

QString MDetailedListItem::subtitle()
{
    return subtitleLabelWidget()->text();
}

MLabel *MDetailedListItem::sideBottomLabelWidget()
{
    Q_D(MDetailedListItem);

    if (!d->sideBottomLabel) {
        d->sideBottomLabel = new MLabel(this);
        d->sideBottomLabel->setObjectName("CommonBottomSideSubTitle");
    }

    return d->sideBottomLabel;
}

void MDetailedListItem::setSideBottomTitle(const QString &text)
{
    sideBottomLabelWidget()->setText(text);
}

QString MDetailedListItem::sideBottomTitle()
{
    return sideBottomLabelWidget()->text();
}

