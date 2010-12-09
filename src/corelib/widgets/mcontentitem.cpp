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

#include <QPixmap>
#include <mwidgetstyle.h>
#include "mcontentitem.h"
#include "mcontentitem_p.h"
#include "mcontentitemmodel.h"

#include "mwidgetcreator.h"
#include <mprogressindicator.h>
#include <mlabel.h>

M_REGISTER_WIDGET(MContentItem)

MContentItemPrivate::MContentItemPrivate():
    MWidgetControllerPrivate(),
    smallText(0), progressIndicator(0)
{
}

MContentItemPrivate::~MContentItemPrivate()
{
    if (smallText)
        delete smallText;
}

void MContentItemPrivate::updateLongTapConnections()
{
    Q_Q(MContentItem);

    if (q->receivers(SIGNAL(longTapped(QPointF))) > 0)
        q->grabGestureWithCancelPolicy(Qt::TapAndHoldGesture, Qt::GestureFlags(), MWidget::MouseEventCancelOnGestureFinished);
    else
        q->ungrabGesture(Qt::TapAndHoldGesture);
}

MContentItem::MContentItem(MContentItem::ContentItemStyle itemStyle, QGraphicsItem *parent)
    : MWidgetController(new MContentItemPrivate, new MContentItemModel, parent)
{
    model()->setItemStyle(itemStyle);
}

MContentItem::MContentItem(MContentItemPrivate *dd, MContentItemModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
}

MContentItem::~MContentItem()
{

}

QPixmap MContentItem::pixmap() const
{
    Q_D(const MContentItem);
    return d->pixmap;
}

QImage MContentItem::image() const
{
    Q_D(const MContentItem);
    return d->image;
}

QString MContentItem::imageID() const
{
    return model()->itemImageID();
}

QString MContentItem::title() const
{
    return model()->title();
}

QString MContentItem::subtitle() const
{
    return model()->subtitle();
}

MContentItem::ContentItemStyle MContentItem::itemStyle() const
{
    return (MContentItem::ContentItemStyle) model()->itemStyle();
}

MContentItem::ContentItemMode MContentItem::itemMode() const
{
    return (MContentItem::ContentItemMode) model()->itemMode();
}

void MContentItem::setPixmap(const QPixmap &pixmap)
{
    Q_D(MContentItem);
    d->pixmap = pixmap;
    model()->setItemPixmap(d->pixmap);
}

void MContentItem::setImage(const QImage &image)
{
    Q_D(MContentItem);
    d->image = image;
    model()->setItemImage(d->image);
}

void MContentItem::setImageID(const QString &id, const QSize &size)
{
    model()->beginTransaction();
    model()->setItemImageID(id);
    model()->setItemImageSize(size);
    model()->commitTransaction();
}

void MContentItem::setTitle(const QString &text)
{
    model()->setTitle(text);
}

void MContentItem::setSubtitle(const QString &text)
{
    if (additionalItem()) {
        additionalItem()->setVisible(false);
        setAdditionalItem(0);
    }

    model()->setSubtitle(text);
}

void MContentItem::setItemMode(ContentItemMode mode)
{
    model()->setItemMode(mode);
}

void MContentItem::click()
{
    emit clicked();
}

void MContentItem::longTap(const QPointF &pos)
{
    emit longTapped(pos);
}

QVariant MContentItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        model()->setSelected(value.toBool());
    }

    return MWidgetController::itemChange(change, value);
}

void MContentItem::setOptionalPixmap(const QPixmap &pixmap)
{
    Q_D(MContentItem);
    d->optionalPixmap = pixmap;
    model()->setOptionalPixmap(d->optionalPixmap);
}

void MContentItem::setOptionalImage(const QImage &image)
{
    Q_D(MContentItem);
    d->optionalImage = image;
    model()->setOptionalImage(d->optionalImage);
}

void MContentItem::setOptionalImageID(const QString &id, const QSize &size)
{
    model()->beginTransaction();
    model()->setOptionalImageID(id);
    model()->setOptionalImageSize(size);
    model()->commitTransaction();
}

QPixmap MContentItem::optionalPixmap() const
{
    Q_D(const MContentItem);
    return d->optionalPixmap;
}

QImage MContentItem::optionalImage() const
{
    Q_D(const MContentItem);
    return d->optionalImage;
}

QString MContentItem::optionalImageID() const
{
    return model()->optionalImageID();
}

void MContentItem::setAdditionalItem(MWidget* widget)
{
    model()->setAdditionalItem(widget);
}

MWidget* MContentItem::additionalItem() const
{
    return model()->additionalItem();
}

void MContentItem::enableProgressBar()
{
    Q_D(MContentItem);
    if(d->progressIndicator == NULL)
    {
        d->progressIndicator = new MProgressIndicator(this);
        d->progressIndicator->setViewType(MProgressIndicator::barType);
        d->progressIndicator->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
        model()->setAdditionalItem(d->progressIndicator);
    }
}

void MContentItem::setSmallItem(MWidget* widget)
{
    Q_D(MContentItem);

    if (d->smallText) {
        delete d->smallText;
        d->smallText = 0;
    }
    model()->setSmallItem(widget);
}

MWidget* MContentItem::smallItem() const
{
    return model()->smallItem();
}

void MContentItem::setSmallText(QString text)
{
    Q_D(MContentItem);

    if (d->smallText) {
        d->smallText->setText(text);
    } else {
        d->smallText = new MLabel(text);
        d->smallText->setAlignment( Qt::AlignRight );
        model()->setSmallItem(d->smallText);
    }
}

void MContentItem::connectNotify(const char *signal)
{
    Q_D(MContentItem);

    if (QLatin1String(signal) == SIGNAL(longTapped(QPointF))) {
        d->updateLongTapConnections();
    }
}

void MContentItem::disconnectNotify(const char *signal)
{
    Q_D(MContentItem);

    if (QLatin1String(signal) == SIGNAL(longTapped(QPointF))) {
        d->updateLongTapConnections();
    }
}

