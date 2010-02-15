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

#include "duiimagewidgetview.h"

#include <QPixmap>

#include "duiimagewidget.h"
#include "duiimagewidget_p.h"
#include "duiviewcreator.h"
#include "private/duiwidgetview_p.h"

class DuiImageWidgetViewPrivate : public DuiWidgetViewPrivate
{
public:
    DuiImageWidgetViewPrivate();
    ~DuiImageWidgetViewPrivate();

    DuiImageWidget *controller;
};

DuiImageWidgetViewPrivate::DuiImageWidgetViewPrivate()
    : controller(0)
{
}

DuiImageWidgetViewPrivate::~DuiImageWidgetViewPrivate()
{
}

DuiImageWidgetView::DuiImageWidgetView(DuiImageWidget *controller) :
    DuiWidgetView(* new DuiImageWidgetViewPrivate, controller)
{
    Q_D(DuiImageWidgetView);
    d->controller = controller;
}

DuiImageWidgetView::DuiImageWidgetView(DuiImageWidgetViewPrivate &dd, DuiImageWidget *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiImageWidgetView);
    d->controller = controller;
}

DuiImageWidgetView::~DuiImageWidgetView()
{
}

void DuiImageWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    Q_D(const DuiImageWidgetView);

    // no image, return
    QSizeF imageSize = d->controller->d_func()->imageDataSize();
    if (imageSize.isEmpty()) return;

    // the source image section size will be used finally
    QSizeF sourceSize = imageSize;

    // get target size, bounded by widget size
    QSizeF widgetSize = size();
    QSizeF targetSize = widgetSize;
    QSizeF t;

    // get the image display size
    qreal fx, fy;
    d->controller->zoomFactor(&fx, &fy);

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

    // limited by target size
    t = targetSize.boundedTo(t);

    // protection codes
    if (sourceSize.width() < 1.0)
        sourceSize.setWidth(1.0);
    if (sourceSize.height() < 1.0)
        sourceSize.setHeight(1.0);

    // get values from controller
    const QPixmap *pixmap = d->controller->pixmap();

    qreal leftBorder = style()->borderLeft();
    qreal topBorder = style()->borderTop();
    qreal rightBorder = style()->borderRight();
    qreal bottomBorder = style()->borderBottom();

    qreal w = leftBorder + rightBorder;
    qreal h = topBorder + bottomBorder;

    // calculate if need draw border
    // notice: no border on the cropped edge
    QSizeF originSize = pixmap->size();

    if (originSize.width() > sourceSize.width())
        w = 0;

    if (originSize.height() > sourceSize.height())
        h = 0;

    // calculate the rectangle of draw
    qreal dx = (widgetSize.width() - t.width()) / 2.0;
    qreal dy = (widgetSize.height() - t.height()) / 2.0;

    // calculate draw rect
    QRectF drawRect, sourceRect, border;
    drawRect.setRect(dx, dy, t.width(), t.height());

    // draw borders outside of target
    // if both borders equals 0, do not draw border
    if (w > 0 || h > 0) {

        QColor borderColor = style()->borderColor();
        qreal borderOpacity = style()->borderOpacity();
        QBrush brush(borderColor);

        qreal oldOpacity = painter->opacity();
        painter->setOpacity(borderOpacity);

        dx = drawRect.x() - leftBorder;
        dy = drawRect.y() - topBorder;

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

    // draw image
    QPointF topLeft = d->controller->crop().topLeft();

    if (topLeft == QPointF(-1.0, -1.0)) {

        // calculate default crop section
        dx = (originSize.width() - sourceSize.width()) / 2.0;
        dy = (originSize.height() - sourceSize.height()) / 2.0;

        sourceRect = QRectF(dx, dy, sourceSize.width(), sourceSize.height());
    } else {

        // calculate crop section by given topLeft corner
        dx = topLeft.x();
        dy = topLeft.y();

        sourceRect = QRectF(dx, dy, qMin(dx + sourceSize.width(), originSize.width()),
                            qMin(dy + sourceSize.height(), originSize.height()));
    }

    painter->drawPixmap(drawRect, *pixmap, sourceRect);
}

void DuiImageWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    DuiWidgetView::resizeEvent(event);
    update();
}

QSizeF DuiImageWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiImageWidgetView);

    QSizeF s = DuiWidgetView::sizeHint(which, constraint);

    if (s == QSizeF(-1, -1)) {
        if (which == Qt::MinimumSize)
            return QSize(0, 0);
        if (which == Qt::MaximumSize)
            return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        return d->controller->imageSize();
    }

    if (which == Qt::MinimumSize || which == Qt::MaximumSize)
        return s;

    if (style()->preferredSize().isValid())
        return style()->preferredSize();

    return d->controller->imageSize();
}

DUI_REGISTER_VIEW_NEW(DuiImageWidgetView, DuiImageWidget)

