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

#include "mimagewidgetview.h"
#include "mimagewidgetview_p.h"

#include <QPixmap>

#include "mimagewidget.h"
#include "mimagewidget_p.h"
#include "mviewcreator.h"
#include "private/mwidgetview_p.h"

MImageWidgetViewPrivate::MImageWidgetViewPrivate()
    : controller(NULL),
    cachedPixmapSize(0, 0),
    borderOpacity(1.0),
    brush(Qt::transparent),
    leftBorder(0.0),
    topBorder(0.0),
    rightBorder(0.0),
    bottomBorder(0.0)
{
}

MImageWidgetViewPrivate::~MImageWidgetViewPrivate()
{
}

void MImageWidgetViewPrivate::calculateDrawRect(const QSizeF &imageSize)
{
    Q_Q(MImageWidgetView);

    // no image, return
    if (imageSize.isEmpty())
        return;

    // get target size, bounded by widget size
    QSizeF widgetSize = q->size();
    QSizeF targetSize = widgetSize;
    QSizeF t;

    // get the image display size
    qreal fx, fy;
    controller->zoomFactor(&fx, &fy);

    t.setWidth(imageSize.width()*fx);
    t.setHeight(imageSize.height()*fy);

    // limited by target size
    t = targetSize.boundedTo(t);

    // calculate the rectangle of draw
    qreal dx = (widgetSize.width() - t.width()) / 2.f;
    qreal dy = (widgetSize.height() - t.height()) / 2.f;

    // calculate draw rect
    drawRect.setRect(dx, dy, t.width(), t.height());
}

QSizeF MImageWidgetViewPrivate::calculateSourceSize(const QSizeF &imageSize)
{
    QSizeF sourceSize = imageSize;
    QSizeF targetSize = controller->size();

    // protection codes
    if (sourceSize.width() < 1.0)
        sourceSize.setWidth(1.f);
    if (sourceSize.height() < 1.0)
        sourceSize.setHeight(1.f);

    QSizeF t;

    // get the image display size
    qreal fx, fy;
    controller->zoomFactor(&fx, &fy);

    t.setWidth(imageSize.width()*fx);
    t.setHeight(imageSize.height()*fy);

    // update sourceSize for crop section by compare with targetSize, simulate zoom effect
    qreal value;
    if (t.width() > targetSize.width()) {
        value = sourceSize.width();
        sourceSize.setWidth(qRound(value * targetSize.width() / t.width()));
    }
    if (t.height() > targetSize.height()) {
        value = sourceSize.height();
        sourceSize.setHeight(qRound(value * targetSize.height() / t.height()));
    }

    return sourceSize;
}

void MImageWidgetViewPrivate::calculateSourceRect(const QSizeF &imageSize)
{
    QPointF topLeft = controller->crop().topLeft();
    QSizeF originalSize = controller->imageSize();
    QSizeF sourceSize = calculateSourceSize(imageSize);

    if (topLeft == QPointF(-1.0, -1.0)) {
        // calculate default crop section
        qreal dx = (originalSize.width() - sourceSize.width()) / 2.f;
        qreal dy = (originalSize.height() - sourceSize.height()) / 2.f;

        sourceRect = QRectF(dx, dy, sourceSize.width(), sourceSize.height());
    } else {
        // calculate crop section by given topLeft corner
        qreal dx = topLeft.x();
        qreal dy = topLeft.y();

        sourceRect = QRectF(dx, dy, qMin(dx + sourceSize.width(), originalSize.width()),
                            qMin(dy + sourceSize.height(), originalSize.height()));
    }
}

void MImageWidgetViewPrivate::checkPixmapSize()
{
    Q_Q(MImageWidgetView);

    const QPixmap *pixmap = controller->pixmap();
    if (pixmap->size() != cachedPixmapSize) {
        cachedPixmapSize = pixmap->size();
        q->updateGeometry();
    }
}

void MImageWidgetViewPrivate::drawBorders(QPainter *painter, const QRectF &drawRect) const
{
    qreal w = leftBorder + rightBorder;
    qreal h = topBorder + bottomBorder;

    if (w > 0 || h > 0) {
        QRectF border;

        qreal oldOpacity = painter->opacity();
        painter->setOpacity(borderOpacity);

        qreal dx = drawRect.x() - leftBorder;
        qreal dy = drawRect.y() - topBorder;

        if (w > 0 && h == 0) {
            // only have horizontal border
            border = QRectF(dx, drawRect.y(), leftBorder, drawRect.height());
            painter->fillRect(border, brush);

            border = QRectF(drawRect.x() + drawRect.width(), drawRect.y(), rightBorder, drawRect.height());
            painter->fillRect(border, brush);
        } else if (w == 0 && h > 0) {
            // only have vertical border
            border = QRectF(drawRect.x(), dy, drawRect.width(), topBorder);
            painter->fillRect(border, brush);

            border = QRectF(drawRect.x(), drawRect.y() + drawRect.height(), drawRect.width(), bottomBorder);
            painter->fillRect(border, brush);
        } else {
            // draw top border
            border = QRectF(dx, dy, drawRect.width() + w, topBorder);
            painter->fillRect(border, brush);

            // draw left border
            border = QRectF(dx, drawRect.y(), leftBorder, drawRect.height());
            painter->fillRect(border, brush);

            // draw right border
            border = QRectF(drawRect.x() + drawRect.width(), drawRect.y(), rightBorder, drawRect.height());
            painter->fillRect(border, brush);

            // draw bottom border
            border = QRectF(dx, drawRect.y() + drawRect.height(), drawRect.width() + w, bottomBorder);
            painter->fillRect(border, brush);
        }
        painter->setOpacity(oldOpacity);
    }

}

void MImageWidgetViewPrivate::applyStyle()
{
    Q_Q(MImageWidgetView);

    borderOpacity = q->style()->borderOpacity();
    brush.setColor(q->style()->borderColor());

    leftBorder = q->style()->borderLeft();
    topBorder = q->style()->borderTop();
    rightBorder = q->style()->borderRight();
    bottomBorder = q->style()->borderBottom();
}

const QPixmap *MImageWidgetViewPrivate::createPixmapFromTheme()
{
    QSize imageSize = controller->model()->imageSize();
    QString imageId = controller->imageId();

    if (!imageSize.isValid() || imageSize.isNull())
        imageSize = preferredImageSize();

    if (!imageSize.isValid())
        return MTheme::pixmap(imageId);

    return MTheme::pixmap(imageId, imageSize);
}

QSize MImageWidgetViewPrivate::preferredImageSize()
{
    Q_Q(MImageWidgetView);

    QSize imageSize = controller->preferredSize().toSize();
    imageSize.setWidth(imageSize.width() - (q->marginLeft() + q->marginRight()));
    imageSize.setHeight(imageSize.height() - (q->marginTop() + q->marginBottom()));

    return imageSize;
}

void MImageWidgetViewPrivate::updateImageGeometry()
{
    Q_Q(MImageWidgetView);
    QSizeF imageSize = q->imageDataSize();

    calculateDrawRect(imageSize);
    calculateSourceRect(imageSize);
}

MImageWidgetView::MImageWidgetView(MImageWidget *controller) :
    MWidgetView(* new MImageWidgetViewPrivate, controller)
{
    Q_D(MImageWidgetView);
    d->controller = controller;
}

MImageWidgetView::MImageWidgetView(MImageWidgetViewPrivate &dd, MImageWidget *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MImageWidgetView);
    d->controller = controller;
}

MImageWidgetView::~MImageWidgetView()
{
}

void MImageWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const MImageWidgetView);

    const QPixmap *pixmap = d->controller->d_func()->getPixmap();

    d->drawBorders(painter, d->drawRect);

    if (pixmap) {
        const_cast<MImageWidgetViewPrivate*>(d)->checkPixmapSize();
        painter->drawPixmap(d->drawRect, *pixmap, d->sourceRect);
    } else
        painter->drawImage(d->drawRect, d->controller->d_func()->getImage(), d->sourceRect);
}

void MImageWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);
    update();
}

void MImageWidgetView::setGeometry(const QRectF &rect)
{
    Q_D(MImageWidgetView);
    MWidgetView::setGeometry(rect);

    d->updateImageGeometry();
}

void MImageWidgetView::updateGeometry()
{
    Q_D(MImageWidgetView);

    MWidgetView::updateGeometry();

    d->updateImageGeometry();
}

void MImageWidgetView::applyStyle()
{
    Q_D(MImageWidgetView);
    MWidgetView::applyStyle();

    d->applyStyle();

    if (!model()->imageId().isEmpty())
        d->controller->d_func()->setPixmap(d->createPixmapFromTheme(), false);
}

void MImageWidgetView::updateData(const QList<const char *> &modifications)
{
    Q_D(MImageWidgetView);
    MWidgetView::updateData(modifications);

    bool needsGeometryUpdate = false;
    bool needsPixmap = false;
    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MImageWidgetModel::ZoomFactorX ||
            member == MImageWidgetModel::ZoomFactorY ||
            member == MImageWidgetModel::Crop ||
            member == MImageWidgetModel::AspectRatioMode) {
            needsGeometryUpdate = true;
        } else if (member == MImageWidgetModel::ImageId ||
                   member == MImageWidgetModel::ImageSize) {
            needsPixmap = true;
        }
    }

    if (needsPixmap && !model()->imageId().isEmpty())
        d->controller->d_func()->setPixmap(d->createPixmapFromTheme(), false);

    if (needsGeometryUpdate || needsPixmap)
        updateGeometry();
}

QSizeF MImageWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MImageWidgetView);

    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return QSizeF(-1,-1);

    QSizeF size = d->controller->imageSize();
    if (constraint.width() >= 0 && constraint.width() < size.width())
        size.scale(QSizeF(constraint.width(), QWIDGETSIZE_MAX), Qt::KeepAspectRatio);
    else if (constraint.height() >= 0 && constraint.height() < size.height())
        size.scale(QSizeF(QWIDGETSIZE_MAX, constraint.height()), Qt::KeepAspectRatio);
    return size;
}

QSize MImageWidgetView::imageDataSize()
{
    Q_D(MImageWidgetView);

    return d->controller->d_func()->imageDataSize(model()->crop()).toSize();
}

M_REGISTER_VIEW_NEW(MImageWidgetView, MImageWidget)

