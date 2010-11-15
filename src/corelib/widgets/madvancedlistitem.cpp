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
#include <QGraphicsLinearLayout>

MAdvancedListItemPrivate::MAdvancedListItemPrivate(MAdvancedListItem::ItemStyle style)
    : q_ptr(NULL),
    layoutGrid(NULL),
    progress(NULL),
    titleLabel(NULL),
    image(NULL),
    sideTopImage(NULL),
    sideBottomImage(NULL),
    isLayoutInitialized(false),
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
            layout()->addItem(q->imageWidget(), 0, 0, 3, 1);

            layout()->addItem(q->titleLabelWidget(), 0, 1);
            layout()->addItem(q->progressIndicator(), 1, 1);

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
    case MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon: {
            q->sideTopImageWidget()->setParentItem(q);
            q->sideBottomImageWidget()->setParentItem(q);

            layout()->addItem(q->imageWidget(), 0, 0, 3, 1);
            layout()->addItem(q->titleLabelWidget(), 0, 1);
            layout()->addItem(q->progressIndicator(), 1, 1, 1, 2);
            layout()->addItem(q->sideTopImageWidget(), 0, 2, Qt::AlignBottom);
            layout()->addItem(new QGraphicsWidget(q), 2, 1);
            break;
        }
    default:
        break;
    }
}

void MAdvancedListItemPrivate::clearLayout()
{
    Q_Q(MAdvancedListItem);

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
    setObjectName("CommonPanel");
}

MAdvancedListItem::~MAdvancedListItem()
{
    delete d_ptr;
}

void MAdvancedListItem::initLayout()
{
    Q_D(MAdvancedListItem);

    if (d->isLayoutInitialized)
        return;

    setLayout(createLayout());
    d->isLayoutInitialized = true;
}

void MAdvancedListItem::setItemStyle(ItemStyle itemStyle)
{
    Q_D(MAdvancedListItem);

    if (itemStyle == d->listItemStyle)
        return;

    d->listItemStyle = itemStyle;
    d->isLayoutInitialized = false;
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

    if (imageWidget) {
        d->image = imageWidget;
        d->image->setObjectName("CommonMainIcon");
        if (d->layout())
            d->layout()->addItem(d->image, 0, 0, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
    }
}

MProgressIndicator * MAdvancedListItem::progressIndicator()
{
    Q_D(MAdvancedListItem);

    if (!d->progress) {
        d->progress = new MProgressIndicator(this, MProgressIndicator::barType);
        d->progress->setObjectName("CommonProgressBar");
    }

    return d->progress;
}

MLabel * MAdvancedListItem::titleLabelWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->titleLabel) {
        d->titleLabel = new MLabel(this);
        d->titleLabel->setTextElide(true);
        d->titleLabel->setObjectName("CommonTitle");
    }

    return d->titleLabel;
}

MImageWidget * MAdvancedListItem::sideTopImageWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->sideTopImage) {
        d->sideTopImage = new MImageWidget(this);
        d->sideTopImage->setObjectName("CommonSubIconTop");
    }

    return d->sideTopImage;
}

MImageWidget * MAdvancedListItem::sideBottomImageWidget()
{
    Q_D(MAdvancedListItem);

    if (!d->sideBottomImage) {
        d->sideBottomImage = new MImageWidget(this);
        d->sideBottomImage->setObjectName("CommonSubIconBottom");
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

void MAdvancedListItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MListItem::resizeEvent(event);
    initLayout();
}

