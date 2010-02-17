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

#include "duiscalableimage.h"
#include "duiscalableimage_p.h"
#include "duigles2renderer.h"

#include <duitheme.h>

#include <QPainter>
#include <DuiDebug>


DuiScalableImagePrivate::DuiScalableImagePrivate()
    : m_imageType(DuiScalable9), m_image(NULL),
      m_useGLRenderer(false)
{
}

DuiScalableImagePrivate::~DuiScalableImagePrivate()
{
}

void DuiScalableImagePrivate::validateSize() const
{
    if (!m_image) {
        return;
    }

    int w = m_image->width();
    int h = m_image->height();

    int cornerWidth = 0;
    int cornerHeight = 0;
    if (m_imageType == DuiScalable9) {
        cornerWidth =  m_preferredMargins.left() + m_preferredMargins.right();
        cornerHeight = m_preferredMargins.top() + m_preferredMargins.bottom();
    }

    if (w < cornerWidth) {
        duiWarning("DuiScalableImage") << "Width of" << pixmapId << " image" << w << "is smaller than combined corner width " << cornerWidth << ". This might cause rendering artifacts.";
    }
    if (h < cornerHeight) {
        duiWarning("DuiScalableImage") << "Height of " << pixmapId << " image" << h << "is smaller than combined corner height" << cornerHeight << ". This might cause rendering artifacts.";
    }
}

void DuiScalableImagePrivate::drawScalable9(int x, int y, int w, int h, QPainter *painter) const
{
    QMargins margins = m_preferredMargins;

    if( w == -1 )
        w = m_image->width();
    if( h == -1 )
        h = m_image->height();

    int cornerWidth = 0;
    int cornerHeight = 0;
    if( m_imageType == DuiScalable9 ) {
        cornerWidth = margins.left() + margins.right();
        cornerHeight = margins.top() + margins.bottom();
    }

    //Make sure that the size of the drawn image is
    //bigger than the 4 corner blocks. If necessary,
    //use smaller border values than those set with setBorders API
    //call.
    if (w < cornerWidth || h < cornerHeight) {
        margins.setLeft(qMax(0, margins.left() - (cornerWidth - w + 1) / 2));
        margins.setRight(qMax(0, margins.right() - (cornerWidth - w + 1) / 2));
        margins.setTop(qMax(0, margins.top() - (cornerHeight - h + 1) / 2));
        margins.setBottom(qMax(0, margins.bottom() - (cornerHeight - h + 1) / 2));
    }
    
    //the image is used in it's native size
    //no need to scale just draw it
    if (m_image->size() == QSize(w, h)) {
        painter->drawPixmap(x, y, *m_image);
    }
    //the image needs some scaling
    else {
        qDrawBorderPixmap(painter, QRect(x, y, w, h), margins, *m_image);
    }
}

void DuiScalableImagePrivate::drawScalable1(int x, int y, int w, int h, QPainter *painter) const
{
    painter->drawPixmap(QRect(x, y, w, h), *m_image, m_image->rect());
}

DuiScalableImage::DuiScalableImage()
    : d_ptr(new DuiScalableImagePrivate())
{
    Q_D(DuiScalableImage);
    d->q_ptr = this;
}

DuiScalableImage::DuiScalableImage(const QPixmap *pixmap, int left, int right, int top, int bottom, const QString &pixmapId)
    : d_ptr(new DuiScalableImagePrivate())
{
    Q_D(DuiScalableImage);
    d->q_ptr = this;

    setBorders(left, right, top, bottom);
    setPixmap(pixmap);
    d->pixmapId = pixmapId;
}

DuiScalableImage::DuiScalableImage(DuiScalableImagePrivate *dd)
    : d_ptr(dd)
{
    Q_D(DuiScalableImage);
    d->q_ptr = this;
}

DuiScalableImage::~DuiScalableImage()
{
    delete d_ptr;
}

void DuiScalableImage::borders(int *left, int *right, int *top, int *bottom) const
{
    Q_D(const DuiScalableImage);

    if (left != NULL) {
        *left = d->m_preferredMargins.left();
    }
    if (right != NULL) {
        *right = d->m_preferredMargins.right();
    }
    if (top != NULL) {
        *top = d->m_preferredMargins.top();
    }
    if (bottom != NULL) {
        *bottom = d->m_preferredMargins.bottom();
    }
}

void DuiScalableImage::setBorders(int left, int right, int top, int bottom)
{
    Q_D(DuiScalableImage);

    d->m_preferredMargins = QMargins(left, top, right, bottom);

    if (d->m_preferredMargins.isNull()) {
        d->m_imageType = DuiScalableImagePrivate::DuiScalable1;
    }
    /*else if( (top == 0 && bottom == 0) ) {//&& (right != 0 && left != 0 ) ) {
        d->m_imageType = DuiScalableImagePrivate::DuiScalable3H;
        duiDebug("DuiScalableImage) << "************** DuiScalable3H *************";
    }
    else if( (right == 0 && left == 0) ) {//&& (top != 0 &&  bottom != 0 ) ) {
        d->m_imageType = DuiScalableImagePrivate::DuiScalable3V;
        duiDebug("DuiScalableImage) << "************** DuiScalable3V *************";
    }*/
    else {
        d->m_imageType = DuiScalableImagePrivate::DuiScalable9;
    }
    d->validateSize();
}

void DuiScalableImage::setPixmap(const QPixmap *pixmap)
{
    Q_D(DuiScalableImage);

    d->m_image = pixmap;
    d->pixmapId = "";

    d->validateSize();
}

const QPixmap *DuiScalableImage::pixmap() const
{
    Q_D(const DuiScalableImage);
    return d->m_image;
}

void DuiScalableImage::draw(int x, int y, int w, int h, QPainter *painter) const
{
    Q_D(const DuiScalableImage);

    if (!d->m_image) {
        duiWarning("DuiScalableImage") << "DuiScalableImage::draw() scalable image: " << d->pixmapId << " not properly initialized yet";
        painter->fillRect(QRect(x, y, w, h), QColor(255, 0, 0));
        return;
    }

    switch (d->m_imageType) {
    case DuiScalableImagePrivate::DuiScalable1:
        d->drawScalable1(x, y, w, h, painter);
        break;
    case DuiScalableImagePrivate::DuiScalable9:
        d->drawScalable9(x, y, w, h, painter);
        break;
    default:
        duiWarning("DuiScalableImage") << d->pixmapId << " draw() unsupported type " << d->m_imageType;
    }
}

void DuiScalableImage::draw(const QPoint &pos, const QSize &size, QPainter *painter) const
{
    draw(pos.x(), pos.y(), size.width(), size.height(), painter);
}

void DuiScalableImage::draw(const QRect &rect, QPainter *painter) const
{
    draw(rect.x(), rect.y(), rect.width(), rect.height(), painter);
}

void DuiScalableImage::enableOptimizedRendering(bool enable)
{
    Q_D(DuiScalableImage);
    d->m_useGLRenderer = enable;
}

