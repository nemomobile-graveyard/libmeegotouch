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

#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QFileInfo>
#include <QPixmap>

#include "duiimagewidget.h"
#include "duiimagewidget_p.h"
#include "duitheme.h"
#include "duiwidgetstyle.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiImageWidget)

DuiImageWidgetPrivate::DuiImageWidgetPrivate()
    : DuiWidgetControllerPrivate(),
      pixmap(0), deletePixmap(true)
{
}

DuiImageWidgetPrivate::~DuiImageWidgetPrivate()
{

    cleanUp();
}

void DuiImageWidgetPrivate::cleanUp()
{
    if (pixmap != 0) {
        if (deletePixmap)
            delete pixmap;
        else
            DuiTheme::releasePixmap(pixmap);
        pixmap = 0;
    }
}

QSizeF DuiImageWidgetPrivate::imageDataSize() const
{
    Q_Q(const DuiImageWidget);
    QSizeF imageSize = QSizeF(0, 0);

    if (pixmap != 0 && !pixmap->isNull()) {
        if (q->model()->crop().isEmpty())
            imageSize = pixmap->size();
        else
            imageSize = q->model()->crop().size();
    }

    return imageSize;
}

DuiImageWidgetPrivate &DuiImageWidgetPrivate::operator=(const DuiImageWidgetPrivate &other)
{
    cleanUp();

    if (other.pixmap != 0) {
        if (other.deletePixmap)
            pixmap = new QPixmap(*(other.pixmap));
        else
            setImageName(other.imageName);
    }
    deletePixmap = other.deletePixmap;

    imageName = other.imageName;
    return *this;
}

// initialize from image name
void DuiImageWidgetPrivate::setImageName(const QString &imageName, const QSize &s)
{
    this->imageName = imageName;

    cleanUp();

    if (s.isValid())
        pixmap = DuiTheme::pixmap(imageName, s);
    else
        pixmap = DuiTheme::pixmap(imageName);

    deletePixmap = false;
}

void DuiImageWidgetPrivate::deepCopy(const DuiImageWidget &other)
{
    Q_Q(DuiImageWidget);
    *q->d_func() = *(other.d_func());

    qreal fx, fy;
    other.zoomFactor(&fx, &fy);
    q->setZoomFactor(fx, fy);

    q->model()->setCrop(other.crop());
    q->model()->setAspectRatioMode(other.aspectRatioMode());
}

DuiImageWidget::DuiImageWidget(QGraphicsItem *parent) :
    DuiWidgetController(new DuiImageWidgetPrivate(), new DuiImageWidgetModel(), parent)
{
}

DuiImageWidget::DuiImageWidget(const QString &imagename, QGraphicsItem *parent) :
    DuiWidgetController(new DuiImageWidgetPrivate(), new DuiImageWidgetModel(), parent)
{
    Q_D(DuiImageWidget);
    d->setImageName(imagename);
}

DuiImageWidget::DuiImageWidget(const QImage *image, QGraphicsItem *parent) :
    DuiWidgetController(new DuiImageWidgetPrivate(), new DuiImageWidgetModel(), parent)
{
    Q_D(DuiImageWidget);
    d->pixmap = new QPixmap(QPixmap::fromImage(*image));
}

DuiImageWidget::DuiImageWidget(const QPixmap *pixmap, QGraphicsItem *parent) :
    DuiWidgetController(new DuiImageWidgetPrivate(), new DuiImageWidgetModel(), parent)
{
    Q_D(DuiImageWidget);
    d->pixmap = new QPixmap(*pixmap);
}

DuiImageWidget::DuiImageWidget(const DuiImageWidget &other) :
    DuiWidgetController(new DuiImageWidgetPrivate(), new DuiImageWidgetModel(), 0)
{
    Q_D(DuiImageWidget);
    d->deepCopy(other);
}

DuiImageWidget &DuiImageWidget::operator=(const DuiImageWidget &other)
{
    Q_D(DuiImageWidget);
    d->deepCopy(other);
    return *this;
}

DuiImageWidget::~DuiImageWidget()
{
}

void DuiImageWidget::setImage(const QString &id, const QSize &s)
{
    Q_D(DuiImageWidget);
    d->setImageName(id, s);

    update();
}

QString DuiImageWidget::image() const
{
    Q_D(const DuiImageWidget);
    return d->imageName;
}

QSize DuiImageWidget::imageSize() const
{
    Q_D(const DuiImageWidget);
    if (d->pixmap != 0)
        return d->pixmap->size();

    return QSize();
}

const QPixmap *DuiImageWidget::pixmap() const
{
    Q_D(const DuiImageWidget);
    return d->pixmap;
}

void DuiImageWidget::setZoomFactor(qreal factor)
{
    if (factor < 0.0) return;

    model()->setZoomFactorX(factor);
    model()->setZoomFactorY(factor);
}

void DuiImageWidget::setZoomFactor(qreal fx, qreal fy)
{
    if (fx < 0.0 || fy < 0.0) return;

    model()->setZoomFactorX(fx);
    model()->setZoomFactorY(fy);

    // If zoomFactorX not equals zoomFactorY, set mode to Qt::IgnoreAspectRatio automatic
    if (fx != fy)
        model()->setAspectRatioMode(Qt::IgnoreAspectRatio);
}

void DuiImageWidget::zoomFactor(qreal *fx, qreal *fy) const
{
    if (fx == 0 || fy == 0) return;

    Q_D(const DuiImageWidget);

    QSizeF imageSize = d->imageDataSize();
    if (imageSize.isEmpty()) return;

    // if factor not equals 0, calculate it with imageSize, targetSize and widgetSize
    QSizeF buffer;

    qreal factorX = model()->zoomFactorX();
    qreal factorY = model()->zoomFactorY();
    if (factorX == 0 || factorY == 0) {
        buffer = imageSize;

        qreal left, top, right, bottom;

        left = style()->marginLeft();
        top = style()->marginTop();
        right = style()->marginRight();
        bottom = style()->marginBottom();

        QSizeF marginSize = QSizeF(left + right, top + bottom);
        QSizeF widgetSize = size() - marginSize;

        buffer.scale(widgetSize, model()->aspectRatioMode());
    }

    if (factorX == 0)
        *fx = buffer.width() / imageSize.width();
    else
        *fx = factorX;

    if (factorY == 0)
        *fy = buffer.height() / imageSize.height();
    else
        *fy = factorY;
}

void DuiImageWidget::zoomIn()
{
    qreal fx, fy;
    zoomFactor(&fx, &fy);
    setZoomFactor(fx * 2.0, fy * 2.0);
}

void DuiImageWidget::zoomOut()
{
    qreal fx, fy;
    zoomFactor(&fx, &fy);
    setZoomFactor(fx / 2.0, fy / 2.0);
}

void DuiImageWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    if (model()->aspectRatioMode() == mode) return;
    model()->setAspectRatioMode(mode);
}

Qt::AspectRatioMode DuiImageWidget::aspectRatioMode() const
{
    return model()->aspectRatioMode();
}

void DuiImageWidget::setCrop(const QRectF &rect)
{
    Q_D(DuiImageWidget);
    if (d->pixmap == 0) return;
    if (rect.size().width() < 0 || rect.size().height() < 0) return;

    QSizeF imageSize = d->pixmap->size();

    // protect the crop section size not beyond the image size
    QRectF r;

    if (rect.isValid()) {
        qreal dx = rect.x();
        qreal dy = rect.y();

        if (dx < 0 || dx > imageSize.width() - 2)
            dx = -1;
        if (dy < 0 || dy > imageSize.height() - 2)
            dy = -1;

        r.setX(dx);
        r.setY(dy);
    } else {
        r.setX(rect.x());
        r.setY(rect.y());
    }

    r.setWidth(qMin(imageSize.width(), rect.width()));
    r.setHeight(qMin(imageSize.height(), rect.height()));

    model()->setCrop(r);
}

QRectF DuiImageWidget::crop() const
{
    return model()->crop();
}

void DuiImageWidget::setImage(const QString &id)
{
    Q_D(DuiImageWidget);
    d->setImageName(id);

    update();
}

void DuiImageWidget::setImage(const QImage &image)
{
    Q_D(DuiImageWidget);

    d->cleanUp();
    d->pixmap = new QPixmap(QPixmap::fromImage(image));
    d->deletePixmap = true;

    update();
}

void DuiImageWidget::setPixmap(const QPixmap &pixmap)
{
    Q_D(DuiImageWidget);

    d->cleanUp();
    d->pixmap = new QPixmap(pixmap);
    d->deletePixmap = true;

    update();
}

