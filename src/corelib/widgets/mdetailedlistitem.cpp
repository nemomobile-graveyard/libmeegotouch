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
#include <QGraphicsLinearLayout>

MDetailedListItemPrivate::MDetailedListItemPrivate(MDetailedListItem::ItemStyle style)
    : q_ptr(NULL),
    layoutGrid(NULL),
    image(NULL),
    sideTopImage(NULL),
    sideBottomImage(NULL),
    titleLabel(NULL),
    subtitleLabel(NULL),
    sideBottomLabel(NULL),
    isLayoutInitialized(false),
    listItemStyle(style),
    iconStyle(MDetailedListItem::Icon)
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
            q->titleLabelWidget()->setObjectName("CommonTitle");
            q->setIconStyle(MDetailedListItem::Icon);

            layout()->addItem(q->imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layout()->addItem(q->titleLabelWidget(), 0, 1, Qt::AlignLeft | Qt::AlignTop);
            layout()->addItem(q->subtitleLabelWidget(), 1, 1, Qt::AlignLeft | Qt::AlignBottom);
            layout()->addItem(new QGraphicsWidget(q), 2, 1, 1, 2);
            layout()->addItem(q->sideTopImageWidget(), 0, 2, Qt::AlignRight | Qt::AlignBottom);
            layout()->addItem(q->sideBottomImageWidget(), 1, 2, Qt::AlignRight | Qt::AlignTop);

            break;
        }
    case MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel: {
            q->titleLabelWidget()->setObjectName("CommonTitle");
            q->setIconStyle(MDetailedListItem::Icon);

            layout()->addItem(q->imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);

            layout()->addItem(q->titleLabelWidget(), 0, 1, 1, 3, Qt::AlignLeft | Qt::AlignTop);
            layout()->addItem(q->sideTopImageWidget(), 0, 4, Qt::AlignRight | Qt::AlignBottom);

            layout()->addItem(q->subtitleLabelWidget(), 1, 1, 1, 2);

            layout()->addItem(q->sideBottomLabelWidget(), 1, 3, 1, 2);


            layout()->addItem(new QGraphicsWidget(q), 2, 1);
            break;
        }
    case MDetailedListItem::ThumbnailTitleAndTwoSideIcons: {
            q->titleLabelWidget()->setObjectName("CommonSingleTitle");
            q->setIconStyle(MDetailedListItem::Thumbnail);

            layout()->addItem(q->imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);

            layout()->addItem(q->titleLabelWidget(), 0, 1, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
            layout()->addItem(new QGraphicsWidget(q), 2, 1, Qt::AlignRight | Qt::AlignVCenter);

            QGraphicsWidget * panel = new QGraphicsWidget(q);
            QGraphicsLinearLayout * panelLayout = new QGraphicsLinearLayout(Qt::Vertical);
            panelLayout->setContentsMargins(0, 0, 0, 0);
            panelLayout->setSpacing(0);
            panel->setLayout(panelLayout);

            q->sideTopImageWidget()->setParentItem(panel);
            q->sideBottomImageWidget()->setParentItem(panel);

            panelLayout->addItem(q->sideTopImageWidget());
            panelLayout->addItem(q->sideBottomImageWidget());

            layout()->addItem(panel, 0, 2, 2, 1, Qt::AlignVCenter);

            break;
        }
    case MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons: {
            q->titleLabelWidget()->setObjectName("CommonTitle");
            q->setIconStyle(MDetailedListItem::Thumbnail);

            layout()->addItem(q->imageWidget(), 0, 0, 3, 1);

            layout()->addItem(q->titleLabelWidget(), 0, 1);
            layout()->addItem(q->subtitleLabelWidget(), 1, 1);

            QGraphicsWidget * panel = new QGraphicsWidget(q);
            QGraphicsLinearLayout * panelLayout = new QGraphicsLinearLayout(Qt::Vertical);
            panelLayout->setContentsMargins(0, 0, 0, 0);
            panelLayout->setSpacing(0);
            panel->setLayout(panelLayout);

            q->sideTopImageWidget()->setParentItem(panel);
            q->sideBottomImageWidget()->setParentItem(panel);

            panelLayout->addItem(q->sideTopImageWidget());
            panelLayout->addItem(q->sideBottomImageWidget());

            layout()->addItem(panel, 0, 2, 3, 1, Qt::AlignVCenter);
            layout()->addItem(new QGraphicsWidget(q), 2, 1);
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
    }
}

QGraphicsGridLayout *MDetailedListItemPrivate::layout()
{
    return layoutGrid;
}

void MDetailedListItemPrivate::applyIconStyle()
{
    if (!image)
        return;

    if (iconStyle == MDetailedListItem::Thumbnail)
        image->setObjectName("CommonThumbnail");
    else if (iconStyle == MDetailedListItem::Icon)
        image->setObjectName("CommonMainIcon");
}

MDetailedListItem::MDetailedListItem(MDetailedListItem::ItemStyle style, QGraphicsItem *parent)
    : MListItem(parent), d_ptr(new MDetailedListItemPrivate(style))
{
    Q_D(MDetailedListItem);
    d->q_ptr = this;

    setItemStyle(style);
    setObjectName("CommonPanel");
}

MDetailedListItem::~MDetailedListItem()
{
    delete d_ptr;
}

void MDetailedListItem::initLayout()
{
    Q_D(MDetailedListItem);

    if (d->isLayoutInitialized)
        return;

    setLayout(createLayout());
    d->isLayoutInitialized = true;
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

    d->listItemStyle = itemStyle;
    d->isLayoutInitialized = false;
    initLayout();
}

MDetailedListItem::ItemStyle MDetailedListItem::itemStyle() const
{
    Q_D(const MDetailedListItem);
    return d->listItemStyle;
}

void MDetailedListItem::setIconStyle(IconStyle style)
{
    Q_D(MDetailedListItem);

    if(style == d->iconStyle)
        return;

    d->iconStyle = style;
    d->applyIconStyle();
}

MDetailedListItem::IconStyle MDetailedListItem::iconStyle() const
{
    Q_D(const MDetailedListItem);

    return d->iconStyle;
}

void MDetailedListItem::setImageWidget(MImageWidget *image)
{
    Q_D(MDetailedListItem);

    if (d->image) {
        if (d->layout())
            for (int i = 0; i < d->layout()->count(); i++) {
                if (d->layout()->itemAt(i) == d->image) {
                    d->layout()->removeAt(i);
                    break;
                }
            }
        delete d->image;
        d->image = NULL;
    }

    if (image) {
        d->image = image;
        if (d->listItemStyle == MDetailedListItem::IconTitleSubtitleAndTwoSideIcons ||
            d->listItemStyle == MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel) {
            setIconStyle(Icon);
            if (d->layout())
                d->layout()->addItem(imageWidget(), 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
        } else if (d->listItemStyle == MDetailedListItem::ThumbnailTitleAndTwoSideIcons) {
            setIconStyle(Thumbnail);
            if (d->layout())
                d->layout()->addItem(imageWidget(), 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
        } else if (d->listItemStyle == MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons) {
            setIconStyle(Thumbnail);
            if (d->layout())
                d->layout()->addItem(imageWidget(), 0, 0, 3, 1);
        }
        d->applyIconStyle();
    }
}

MImageWidget *MDetailedListItem::imageWidget()
{
    Q_D(MDetailedListItem);

    if (!d->image) {
        d->image = new MImageWidget(this);
        d->applyIconStyle();
    }
    return d->image;
}

MImageWidget *MDetailedListItem::sideTopImageWidget()
{
    Q_D(MDetailedListItem);

    if (!d->sideTopImage) {
        d->sideTopImage = new MImageWidget(this);
        d->sideTopImage->setObjectName("CommonSubIconTop");
    }
    return d->sideTopImage;
}

MImageWidget *MDetailedListItem::sideBottomImageWidget()
{
    Q_D(MDetailedListItem);

    if (!d->sideBottomImage) {
        d->sideBottomImage = new MImageWidget(this);
        d->sideBottomImage->setObjectName("CommonSubIconBottom");
    }
    return d->sideBottomImage;
}

MLabel *MDetailedListItem::titleLabelWidget()
{
    Q_D(MDetailedListItem);

    if (!d->titleLabel) {
        d->titleLabel = new MLabel(this);
        d->titleLabel->setTextElide(true);
        d->titleLabel->setObjectName("CommonTitle");
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
        d->subtitleLabel->setTextElide(true);
        d->subtitleLabel->setObjectName("CommonSubTitle");
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
        d->sideBottomLabel->setTextElide(true);
        d->sideBottomLabel->setAlignment(Qt::AlignRight);
        d->sideBottomLabel->setObjectName("CommonItemInfo");
        d->sideBottomLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
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

void MDetailedListItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MListItem::resizeEvent(event);
    initLayout();
}

