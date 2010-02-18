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
    : m_imageType(DuiScalable9), m_imageSize(-1, -1), m_image(NULL),
      m_useGLRenderer(false), m_left(0), m_right(0), m_top(0), m_bottom(0)
{
}

DuiScalableImagePrivate::~DuiScalableImagePrivate()
{
    m_imageBlocks.clear();
    m_imageRects.clear();
}

void DuiScalableImagePrivate::validateSize(int &w, int &h) const
{
    if (w == -1)
        w = m_image->width();
    if (h == -1)
        h = m_image->height();

    int cornerWidth = 0;
    int cornerHeight = 0;
    if (m_imageType == DuiScalable9) {
        cornerWidth = m_imageRects[0].size().width() + m_imageRects[1].size().width();
        cornerHeight = m_imageRects[0].size().height() + m_imageRects[2].size().height();
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
    int left = m_preferredLeft;
    int right = m_preferredRight;
    int top = m_preferredTop;
    int bottom = m_preferredBottom;

    if( w == -1 )
        w = m_image->width();
    if( h == -1 )
        h = m_image->height();

    int cornerWidth = 0;
    int cornerHeight = 0;
    if( m_imageType == DuiScalable9 ) {
        cornerWidth = m_preferredLeft + m_preferredRight;
        cornerHeight = m_preferredTop + m_preferredBottom;
    }

    //Make sure that the size of the drawn image is
    //bigger than the 4 corner blocks. If necessary,
    //use smaller border values than those set with setBorders API
    //call.

    if (w < cornerWidth || h < cornerHeight) {
        left = qMax(0, left - (cornerWidth - w + 1) / 2);
        right = qMax(0, right - (cornerWidth - w + 1) / 2);
        top = qMax(0, top - (cornerHeight - h + 1) / 2);
        bottom = qMax(0, bottom - (cornerHeight - h + 1) / 2);

    }
    
    if (left != m_left || right != m_right || top != m_top || bottom != m_bottom) {
        m_left = left;
        m_right = right;
        m_top = top;
        m_bottom = bottom;
        
        calcImageRects();
    }

    //the image is used in it's native size
    //no need to scale just draw it
    if (m_image->size() == QSize(w, h)) {
        painter->drawPixmap(x, y, *m_image);
    }
    //the image needs some scaling
    else {
        //calculate some common values
        int cornerWidth   = m_imageRects[0].size().width() + m_imageRects[1].size().width();
        int cornerHeight  = m_imageRects[0].size().height() + m_imageRects[2].size().height();

        int edgeWidth = w - cornerWidth;
        int edgeHeight = h - cornerHeight;
        int rightX = x + m_imageRects[0].size().width() + edgeWidth;
        int bottomY = y + m_imageRects[0].size().height() + edgeHeight;
        int horEdgeX = x + m_imageRects[0].size().width();
        int verEdgeY = y + m_imageRects[0].size().height();

#ifdef DUI_USE_OPENGL
#if QT_VERSION >= 0x040600
        if (m_useGLRenderer && painter->paintEngine()->type() == QPaintEngine::OpenGL2) {
#else
        if (m_useGLRenderer && painter->paintEngine()->type() == QPaintEngine::OpenGL) {
#endif
            QList<QRect> targetRects;

            targetRects.append(QRect(QPoint(x, y), m_imageRects[0].size()));
            targetRects.append(QRect(QPoint(rightX, y), m_imageRects[1].size()));
            targetRects.append(QRect(QPoint(x, bottomY), m_imageRects[2].size()));
            targetRects.append(QRect(QPoint(rightX, bottomY), m_imageRects[3].size()));

            targetRects.append(QRect(horEdgeX, y, edgeWidth, m_imageRects[0].size().height()));
            targetRects.append(QRect(x, verEdgeY, m_imageRects[0].size().width(), edgeHeight));
            targetRects.append(QRect(horEdgeX, bottomY, edgeWidth, m_imageRects[2].size().height()));
            targetRects.append(QRect(rightX, verEdgeY, m_imageRects[1].size().width(), edgeHeight));

            targetRects.append(QRect(x + m_imageRects[0].size().width(), y + m_imageRects[0].size().height(), edgeWidth, edgeHeight));

            DuiGLES2Renderer *r = DuiGLES2Renderer::instance();
            r->begin(painter);
            r->bindTexture(*m_image);
            r->draw(targetRects, m_imageRects);
            r->end();
        } else {
#else
        {
#endif
            //draw corners
            //tl
            if (m_imageRects[0].width() && m_imageRects[0].height())
                painter->drawPixmap(QPoint(x, y), *m_image, m_imageRects[0]);
            //tr
            if (m_imageRects[1].width() && m_imageRects[1].height())
                painter->drawPixmap(QPoint(rightX, y), *m_image, m_imageRects[1]);
            //bl
            if (m_imageRects[2].width() && m_imageRects[2].height())
                painter->drawPixmap(QPoint(x, bottomY), *m_image, m_imageRects[2]);
            //br
            if (m_imageRects[3].width() && m_imageRects[3].height())
                painter->drawPixmap(QPoint(rightX, bottomY), *m_image, m_imageRects[3]);

            //draw edges
            //top
            if (m_imageRects[4].width() && m_imageRects[4].height())
                painter->drawPixmap(QRect(horEdgeX, y, edgeWidth, m_imageRects[0].size().height()), *m_image, m_imageRects[4]);
            //left
            if (m_imageRects[5].width() && m_imageRects[5].height())
                painter->drawPixmap(QRect(x, verEdgeY, m_imageRects[0].size().width(), edgeHeight), *m_image, m_imageRects[5]);
            //bottom
            if (m_imageRects[6].width() && m_imageRects[6].height())
                painter->drawPixmap(QRect(horEdgeX, bottomY, edgeWidth, m_imageRects[2].size().height()), *m_image, m_imageRects[6]);
            //right
            if (m_imageRects[7].width() && m_imageRects[7].height())
                painter->drawPixmap(QRect(rightX, verEdgeY, m_imageRects[1].size().width(), edgeHeight), *m_image, m_imageRects[7]);

            //TODO: Future optimizations, uncomment these when the graphics are updated
            //      to support solid center patch.
            //draw center
            //painter->setCompositionMode(QPainter::CompositionMode_Source);
            if (m_imageRects[8].width() && m_imageRects[8].height()) {
                painter->drawPixmap(QRect(x + m_imageRects[0].size().width(), y + m_imageRects[0].size().height(), edgeWidth, edgeHeight), *m_image, m_imageRects[8]);
                //painter->fillRect(QRect(x+m_imageRects[0].size().width(),y+m_imageRects[0].size().height(), edgeWidth, edgeHeight), QColor(0, 255, 0));
            }
            //painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
    }
}

void DuiScalableImagePrivate::drawScalable1(int x, int y, int w, int h, QPainter *painter) const
{
    painter->drawPixmap(QRect(x, y, w, h), *m_image, m_image->rect());
}

/*void DuiScalableImagePrivate::drawScalable3H(int x, int y, int w, int h, QPainter* painter) const
{
}

void DuiScalableImagePrivate::drawScalable3V(int x, int y, int w, int h, QPainter* painter) const
{
}*/

/*
    0  4  1
    *-----*
    |xxxxx|
  5 |xx8xx| 7
    |xxxxx|
    *-----*
    2  6  3

    0  2  1
    |xxxxx|
    |xxxxx|
    |xxxxx|

    0 -----
      XXXXX
    1 XXXXX
      XXXXX
    2 -----
*/
void DuiScalableImagePrivate::calcImageRects() const
{
    if (!m_image)
        return;

    if (m_imageType == DuiScalableImagePrivate::DuiScalable9) {
        m_imageSize = m_image->size();
        if (m_imageSize.width() < m_left + m_right) {
            duiWarning("DuiScalableImagePrivate") << "DuiScalableImage - " << pixmapId << " : Width of horizontal borders"
                                                  << m_left + m_right << "is bigger than the width of the whole image"
                                                  << m_imageSize.width();
        }
        if (m_imageSize.height() < m_top + m_bottom) {
            duiWarning("DuiScalableImagePrivate") << "DuiScalableImage - " << pixmapId << " : Height of vertical borders"
                                                  << m_top + m_bottom << "is bigger than the height of the whole image"
                                                  << m_imageSize.height();
        }

        //corner sizes
        QSize szTl(m_left, m_top);
        QSize szTr(m_right, m_top);
        QSize szBl(m_left, m_bottom);
        QSize szBr(m_right, m_bottom);

        //edge widths
        int h = m_imageSize.height() - m_top - m_bottom;
        int w = m_imageSize.width() - m_left - m_right;

        //bottom and right edge positions
        int b = m_imageSize.height() - m_bottom;
        int r = m_imageSize.width() - m_right;

        //edge and center block sizes
        QSize szL(m_left, h);
        QSize szR(m_right, h);
        QSize szT(w, m_top);
        QSize szB(w, m_bottom);
        QSize szC(w, h);

        //insert the block rects into array
        m_imageRects.clear();
        m_imageRects.append(QRect(QPoint(0, 0), szTl));
        m_imageRects.append(QRect(QPoint(r, 0), szTr));
        m_imageRects.append(QRect(QPoint(0, b), szBl));
        m_imageRects.append(QRect(QPoint(r, b), szBr));
        m_imageRects.append(QRect(QPoint(m_left, 0), szT));
        m_imageRects.append(QRect(QPoint(0, m_top), szL));
        m_imageRects.append(QRect(QPoint(m_left, b), szB));
        m_imageRects.append(QRect(QPoint(r, m_top), szR));
        m_imageRects.append(QRect(QPoint(m_left, m_top), szC));
    }
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
        *left = d->m_left;
    }
    if (right != NULL) {
        *right = d->m_right;
    }
    if (top != NULL) {
        *top = d->m_top;
    }
    if (bottom != NULL) {
        *bottom = d->m_bottom;
    }
}

void DuiScalableImage::setBorders(int left, int right, int top, int bottom)
{
    Q_D(DuiScalableImage);

    d->m_left   = left;
    d->m_right  = right;
    d->m_top    = top;
    d->m_bottom = bottom;

    d->m_preferredLeft = left;
    d->m_preferredRight = right;
    d->m_preferredTop = top;
    d->m_preferredBottom = bottom;

    if (left == 0 && right == 0 && top == 0 && bottom == 0) {
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

    d->calcImageRects();
}

void DuiScalableImage::setPixmap(const QPixmap *pixmap)
{
    Q_D(DuiScalableImage);

    d->m_image = pixmap;
    d->pixmapId = "";
    d->calcImageRects();
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

    if (d->m_imageSize != d->m_image->size())
        d->calcImageRects();

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

