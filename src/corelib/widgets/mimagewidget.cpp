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

#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QFileInfo>
#include <QPixmap>

#include "mimagewidget.h"
#include "mimagewidget_p.h"
#include "mtheme.h"
#include "mwidgetstyle.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MImageWidget)

MImageWidgetPrivate::MImageWidgetPrivate()
    : MWidgetControllerPrivate(),
      pixmap(0),
      deletePixmap(true)
{
}

MImageWidgetPrivate::~MImageWidgetPrivate()
{
    cleanUp();
}

void MImageWidgetPrivate::cleanUp()
{
    if (pixmap != 0) {
        if (deletePixmap)
            delete pixmap;
        else
            MTheme::releasePixmap(pixmap);
        pixmap = 0;
    }
    image = QImage();
}

QSizeF MImageWidgetPrivate::imageDataSize() const
{
    Q_Q(const MImageWidget);
    QSizeF imageSize = QSizeF(0, 0);

    if ((pixmap != 0 && !pixmap->isNull()) || (!image.isNull())) {
        if (q->model()->crop().isEmpty()) {
            if (image.isNull())
                imageSize = pixmap->size();
            else
                imageSize = image.size();
        } else {
            imageSize = q->model()->crop().size();
        }
    }

    return imageSize;
}

MImageWidgetPrivate &MImageWidgetPrivate::operator=(const MImageWidgetPrivate &other)
{
    cleanUp();

    if (other.pixmap != 0) {
        if (other.deletePixmap) {
            if (other.pixmap)
                pixmap = new QPixmap(*(other.pixmap));
            else
                image = other.image;
        }
        else
            setImage(other.q_func()->model()->imageId(), other.q_func()->model()->imageSize());
    }

    deletePixmap = other.deletePixmap;
    q_func()->setImage(other.q_func()->model()->imageId(), other.q_func()->model()->imageSize());

    return *this;
}

// initialize from image name
void MImageWidgetPrivate::setImage(const QString &id, const QSize &s)
{
    Q_Q(MImageWidget);

    if (id == q->model()->imageId() && s == q->model()->imageSize())
        return;

    cleanUp();

    q->model()->beginTransaction();
    q->model()->setImageId(id);
    q->model()->setImageSize(s);
    q->model()->commitTransaction();

    deletePixmap = false;
}

void MImageWidgetPrivate::deepCopy(const MImageWidget &other)
{
    Q_Q(MImageWidget);
    *q->d_func() = *(other.d_func());

    qreal fx(0), fy(0);

    other.zoomFactor(&fx, &fy);
    q->setZoomFactor(fx, fy);

    q->model()->setCrop(other.crop());
    q->model()->setAspectRatioMode(other.aspectRatioMode());

    q->d_func()->image = other.d_func()->image;
}

void MImageWidgetPrivate::init() {
    Q_Q(MImageWidget);
    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    q->setSizePolicy(policy);
}

MImageWidget::MImageWidget(QGraphicsItem *parent) :
    MWidgetController(new MImageWidgetPrivate(), new MImageWidgetModel(), parent)
{
    Q_D(MImageWidget);
    d->init();
}

MImageWidget::MImageWidget(const QString &id, QGraphicsItem *parent) :
    MWidgetController(new MImageWidgetPrivate(), new MImageWidgetModel(), parent)
{
    Q_D(MImageWidget);
    setImage(id);
    d->init();
}

MImageWidget::MImageWidget(const QImage *image, QGraphicsItem *parent) :
    MWidgetController(new MImageWidgetPrivate(), new MImageWidgetModel(), parent)
{
    Q_D(MImageWidget);
    d->pixmap = NULL;
    d->image = *image;
    d->init();
}

MImageWidget::MImageWidget(const QPixmap *pixmap, QGraphicsItem *parent) :
    MWidgetController(new MImageWidgetPrivate(), new MImageWidgetModel(), parent)
{
    Q_D(MImageWidget);
    d->pixmap = new QPixmap(*pixmap);
    d->init();
}

MImageWidget::MImageWidget(const MImageWidget &other) :
    MWidgetController(new MImageWidgetPrivate(), new MImageWidgetModel(), 0)
{
    Q_D(MImageWidget);
    d->deepCopy(other);
    d->init();
}

MImageWidget &MImageWidget::operator=(const MImageWidget &other)
{
    Q_D(MImageWidget);
    d->deepCopy(other);
    return *this;
}

MImageWidget::~MImageWidget()
{
}

void MImageWidget::setImage(const QString &id, const QSize &s)
{
    Q_D(MImageWidget);
    d->setImage(id, s);

    model()->setCrop(QRect());

    updateGeometry();
    update();
}

QString MImageWidget::image() const
{
    return model()->imageId();
}

QString MImageWidget::imageId() const
{
    return model()->imageId();
}

QSize MImageWidget::imageSize() const
{
    Q_D(const MImageWidget);
    if (d->pixmap)
        return d->pixmap->size();
    else
        return d->image.size();
}

const QPixmap *MImageWidget::pixmap() const
{
    Q_D(const MImageWidget);
    if (d->pixmap)
        return d->pixmap;
    else {
        d->imagePlaceHolder = QPixmap::fromImage(d->image);
        return &d->imagePlaceHolder;
    }
}

void MImageWidget::setZoomFactor(qreal factor)
{
    if (factor < 0.0) return;

    model()->setZoomFactorX(factor);
    model()->setZoomFactorY(factor);
}

void MImageWidget::setZoomFactor(qreal fx, qreal fy)
{
    if (fx >= 0.0)
        model()->setZoomFactorX(fx);

    if (fy >= 0.0)
        model()->setZoomFactorY(fy);

    // If zoomFactorX not equals zoomFactorY, set mode to Qt::IgnoreAspectRatio automatic
    if (fx != fy)
        model()->setAspectRatioMode(Qt::IgnoreAspectRatio);
}

void MImageWidget::zoomFactor(qreal *fx, qreal *fy) const
{
    Q_D(const MImageWidget);

    if(fx)
        *fx = model()->zoomFactorX();
    if(fy)
        *fy = model()->zoomFactorY();

    if ((fx && *fx == 0) || (fy && *fy == 0)) {
        // If the zoom factor is 0, calculate it with imageSize, targetSize and widgetSize
        QSizeF buffer;
        QSizeF imageSize = d->imageDataSize();
        if (imageSize.isEmpty())
            return;

        buffer = imageSize;

        QSizeF marginSize = QSizeF(style()->marginLeft() + style()->marginRight(), style()->marginTop() + style()->marginBottom());
        QSizeF widgetSize = size() - marginSize;

        buffer.scale(widgetSize, model()->aspectRatioMode());

        if (fx && *fx == 0)
            *fx = buffer.width() / imageSize.width();

        if (fy && *fy == 0)
            *fy = buffer.height() / imageSize.height();
    }
}

void MImageWidget::zoomIn()
{
    qreal fx(0), fy(0);

    zoomFactor(&fx, &fy);
    setZoomFactor(fx * 2.0, fy * 2.0);
}

void MImageWidget::zoomOut()
{
    qreal fx(0), fy(0);

    zoomFactor(&fx, &fy);
    setZoomFactor(fx / 2.0, fy / 2.0);
}

void MImageWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    if (model()->aspectRatioMode() == mode)
        return;
    model()->setAspectRatioMode(mode);
}

Qt::AspectRatioMode MImageWidget::aspectRatioMode() const
{
    return model()->aspectRatioMode();
}

void MImageWidget::setCrop(const QRectF &rect)
{
    Q_D(MImageWidget);
    if (d->pixmap == 0 && d->image.isNull())
        return;

    if (!rect.isValid())
        return;

    model()->setCrop(rect);
}

QRectF MImageWidget::crop() const
{
    return model()->crop();
}

void MImageWidget::setImage(const QString &id)
{
    setImage(id, QSize());
}

void MImageWidget::setImage(const QImage &image)
{
    Q_D(MImageWidget);
    d->setImage(QString());

    d->cleanUp();
    d->image = image;
    d->deletePixmap = false;

    model()->setCrop(QRect());

    updateGeometry();
    update();
}

void MImageWidget::setPixmap(const QPixmap &pixmap)
{
    Q_D(MImageWidget);
    d->setImage(QString());

    d->cleanUp();
    d->pixmap = new QPixmap(pixmap);
    d->deletePixmap = true;

    model()->setCrop(QRect());

    updateGeometry();
    update();
}
