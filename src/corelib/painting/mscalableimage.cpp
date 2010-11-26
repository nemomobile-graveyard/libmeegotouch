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

#include "mscalableimage.h"
#include "mscalableimage_p.h"
#include "mgles2renderer.h"

#include <mtheme.h>

#include <QPainter>
#include <QPixmapCache>
#include <MDebug>

MScalableImagePrivate::MScalableImagePrivate()
    : m_imageType(MScalable9), m_image(NULL),
      q_ptr(NULL)
{
}

MScalableImagePrivate::~MScalableImagePrivate()
{
}

void MScalableImagePrivate::validateSize() const
{
    if (!m_image) {
        return;
    }

    int w = m_image->width();
    int h = m_image->height();

    int cornerWidth = 0;
    int cornerHeight = 0;
    if (m_imageType == MScalable9) {
        cornerWidth =  m_preferredMargins.left() + m_preferredMargins.right();
        cornerHeight = m_preferredMargins.top() + m_preferredMargins.bottom();
    }

    if ( (w < cornerWidth || h < cornerHeight) && !(w == 1 && h == 1) ) //1x1 means a temporary pixmap
        mWarning("MScalableImage") << QString("The size of the image '%1', %2x%3, is smaller than combined corner size %4x%5. This might cause rendering artifacts.").arg(pixmapId).arg(w).arg(h).arg(cornerWidth).arg(cornerHeight);
}

void MScalableImagePrivate::drawScalable9(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const
{
    QMargins margins = m_preferredMargins;

    if( w == -1 )
        w = m_image->width();
    if( h == -1 )
        h = m_image->height();

    int cornerWidth = 0;
    int cornerHeight = 0;
    if( m_imageType == MScalable9 ) {
        cornerWidth = margins.left() + margins.right();
        cornerHeight = margins.top() + margins.bottom();
    }

    //Make sure that the size of the drawn image is
    //bigger than the 4 corner blocks. If necessary,
    //use smaller border values than those set with setBorders API
    //call.
    if (w < cornerWidth) {
        margins.setLeft(qMax((qreal)0.0, margins.left() - (cornerWidth - w + 1) / 2));
        margins.setRight(qMax((qreal)0.0, margins.right() - (cornerWidth - w + 1) / 2));
    }
    if (h < cornerHeight) {
        margins.setTop(qMax((qreal)0.0, margins.top() - (cornerHeight - h + 1) / 2));
        margins.setBottom(qMax((qreal)0.0, margins.bottom() - (cornerHeight - h + 1) / 2));
    }

    if (w <= 0 || h <= 0) {
        // this should really not happen
        mWarning("MScalableImage") <<
            "Received request to draw pixmap of invalid size" << w << "x" << h;
        return;
    }

    //the image is used in it's native size
    //no need to scale just draw it
    QSize requiredSize(w, h);
    if (m_image->size() == requiredSize) {
        painter->drawPixmap(x, y, *m_image);
        return;
    }
    else {
        //the image doesn't fit directly into the required size.
        //check whether or not we're allowed to cache
        bool docache = painter->paintEngine()->type() != QPaintEngine::OpenGL
                    && painter->paintEngine()->type() != QPaintEngine::OpenGL2;

        if (docache) {
            //software rendering is not fast when scaling pixmaps, so we use the
            //global pixmap cache to avoid rescaling more than needed.
            //
            //TODO: this cache gets thrashed a bit with widgetsgallery, may want to look into possibly
            //increasing the size of QPixmapCache.
            QString key = QString("msi-%1-%2,%3").arg((*m_image).cacheKey()).arg(w).arg(h);
            QPixmap scaled;
            if (QPixmapCache::find(key, &scaled)) {
                //cached! draw and we're done
                painter->drawPixmap(x, y, scaled);
                return;
            }

            // draw into cache
            scaled = QPixmap(requiredSize);
            scaled.fill(Qt::transparent);
            QPainter p;
            p.begin(&scaled);
            p.setRenderHint(QPainter::SmoothPixmapTransform);
            drawBorderPixmap(&p, QRectF(0, 0, w, h), margins, *m_image);
            p.end();

            // draw to screen
            painter->drawPixmap(x, y, scaled);
            QPixmapCache::insert(key, scaled);
        } else {
            // caching isn't permitted for this case; scale and render direct to screen.
            #if defined(M_OS_MAEMO5)
                // don't use smooth pixmap transformation on the N900, as this
                // decreases the performance
                drawBorderPixmap(painter, QRectF(x, y, w, h), margins, *m_image);
            #else
                bool enabled = painter->renderHints() & QPainter::SmoothPixmapTransform;
                painter->setRenderHint(QPainter::SmoothPixmapTransform);
                drawBorderPixmap(painter, QRectF(x, y, w, h), margins, *m_image);
                painter->setRenderHint(QPainter::SmoothPixmapTransform, enabled);
            #endif //defined(M_OS_MAEMO5)
        }
    }
}

void MScalableImagePrivate::drawScalable1(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const
{
#if defined(M_OS_MAEMO5)
    // don't use smooth pixmap transformation on the N900, as this
    // decreases the performance
    painter->drawPixmap(QRectF(x, y, w, h), *m_image, m_image->rect());
#else
    //the image is used in it's native size
    //no need to scale just draw it
    if( m_image->size() == QSizeF(w, h) ) {
        painter->drawPixmap(QRectF(x, y, w, h), *m_image, m_image->rect());
    }
    //the image needs some scaling, draw the image using smooth scaling
    else {
        bool enabled = painter->renderHints() & QPainter::SmoothPixmapTransform;
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        painter->drawPixmap(QRectF(x, y, w, h), *m_image, m_image->rect());
        painter->setRenderHint(QPainter::SmoothPixmapTransform, enabled);
    }
#endif //defined(M_OS_MAEMO5)
}

void MScalableImagePrivate::drawBorderPixmap(QPainter *painter,
                                             const QRectF &targetRect,
                                             const QMargins &targetMargins,
                                             const QPixmap &pixmap,
                                             const QRect &sourceRect,
                                             const QMargins &sourceMargins,
                                             const QTileRules &rules,
                                             QDrawBorderPixmap::DrawingHints hints)
{
    //
    // The method is reimplementation of Qt's method qDrawBorderPixmap with floating point target rect support.
    // see Qt src/gui/painting/qdrawutil.[cpp,h].
    //

    QPainter::PixmapFragment d;
    d.opacity = 1.0;
    d.rotation = 0.0;

    QPixmapFragmentsArray opaqueData;
    QPixmapFragmentsArray translucentData;

    // source center
    const int sourceCenterTop = sourceRect.top() + sourceMargins.top();
    const int sourceCenterLeft = sourceRect.left() + sourceMargins.left();
    const int sourceCenterBottom = sourceRect.bottom() - sourceMargins.bottom() + 1;
    const int sourceCenterRight = sourceRect.right() - sourceMargins.right() + 1;
    const int sourceCenterWidth = sourceCenterRight - sourceCenterLeft;
    const int sourceCenterHeight = sourceCenterBottom - sourceCenterTop;
    // target center
    const qreal targetCenterTop = targetRect.top() + targetMargins.top();
    const qreal targetCenterLeft = targetRect.left() + targetMargins.left();
    const qreal targetCenterBottom = targetRect.bottom() - targetMargins.bottom();
    const qreal targetCenterRight = targetRect.right() - targetMargins.right();
    const qreal targetCenterWidth = targetCenterRight - targetCenterLeft;
    const qreal targetCenterHeight = targetCenterBottom - targetCenterTop;

    QVarLengthArray<qreal, 16> xTarget; // x-coordinates of target rectangles
    QVarLengthArray<qreal, 16> yTarget; // y-coordinates of target rectangles

    int columns = 3;
    int rows = 3;
    if (rules.horizontal != Qt::StretchTile && sourceCenterWidth != 0)
        columns = qMax(3, 2 + qCeil(targetCenterWidth / qreal(sourceCenterWidth)));
    if (rules.vertical != Qt::StretchTile && sourceCenterHeight != 0)
        rows = qMax(3, 2 + qCeil(targetCenterHeight / qreal(sourceCenterHeight)));

    xTarget.resize(columns + 1);
    yTarget.resize(rows + 1);

    bool oldAA = painter->testRenderHint(QPainter::Antialiasing);
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL
        && painter->paintEngine()->type() != QPaintEngine::OpenGL2
        && oldAA && painter->combinedTransform().type() != QTransform::TxNone) {
        painter->setRenderHint(QPainter::Antialiasing, false);
    }

    xTarget[0] = targetRect.left();
    xTarget[1] = targetCenterLeft;
    xTarget[columns - 1] = targetCenterRight;
    xTarget[columns] = targetRect.left() + targetRect.width();

    yTarget[0] = targetRect.top();
    yTarget[1] = targetCenterTop;
    yTarget[rows - 1] = targetCenterBottom;
    yTarget[rows] = targetRect.top() + targetRect.height();

    qreal dx = targetCenterWidth;
    qreal dy = targetCenterHeight;

    switch (rules.horizontal) {
    case Qt::StretchTile:
        dx = targetCenterWidth;
        break;
    case Qt::RepeatTile:
        dx = sourceCenterWidth;
        break;
    case Qt::RoundTile:
        dx = targetCenterWidth / qreal(columns - 2);
        break;
    }

    for (int i = 2; i < columns - 1; ++i)
        xTarget[i] = xTarget[i - 1] + dx;

    switch (rules.vertical) {
    case Qt::StretchTile:
        dy = targetCenterHeight;
        break;
    case Qt::RepeatTile:
        dy = sourceCenterHeight;
        break;
    case Qt::RoundTile:
        dy = targetCenterHeight / qreal(rows - 2);
        break;
    }

    for (int i = 2; i < rows - 1; ++i)
        yTarget[i] = yTarget[i - 1] + dy;

    // corners
    if (targetMargins.top() > 0 && targetMargins.left() > 0 && sourceMargins.top() > 0 && sourceMargins.left() > 0) { // top left
        d.x = (0.5 * (xTarget[1] + xTarget[0]));
        d.y = (0.5 * (yTarget[1] + yTarget[0]));
        d.sourceLeft = sourceRect.left();
        d.sourceTop = sourceRect.top();
        d.width = sourceMargins.left();
        d.height = sourceMargins.top();
        d.scaleX = qreal(xTarget[1] - xTarget[0]) / d.width;
        d.scaleY = qreal(yTarget[1] - yTarget[0]) / d.height;
        if (hints & QDrawBorderPixmap::OpaqueTopLeft)
            opaqueData.append(d);
        else
            translucentData.append(d);
    }
    if (targetMargins.top() > 0 && targetMargins.right() > 0 && sourceMargins.top() > 0 && sourceMargins.right() > 0) { // top right
        d.x = (0.5 * (xTarget[columns] + xTarget[columns - 1]));
        d.y = (0.5 * (yTarget[1] + yTarget[0]));
        d.sourceLeft = sourceCenterRight;
        d.sourceTop = sourceRect.top();
        d.width = sourceMargins.right();
        d.height = sourceMargins.top();
        d.scaleX = qreal(xTarget[columns] - xTarget[columns - 1]) / d.width;
        d.scaleY = qreal(yTarget[1] - yTarget[0]) / d.height;
        if (hints & QDrawBorderPixmap::OpaqueTopRight)
            opaqueData.append(d);
        else
            translucentData.append(d);
    }
    if (targetMargins.bottom() > 0 && targetMargins.left() > 0 && sourceMargins.bottom() > 0 && sourceMargins.left() > 0) { // bottom left
        d.x = (0.5 * (xTarget[1] + xTarget[0]));
        d.y =(0.5 * (yTarget[rows] + yTarget[rows - 1]));
        d.sourceLeft = sourceRect.left();
        d.sourceTop = sourceCenterBottom;
        d.width = sourceMargins.left();
        d.height = sourceMargins.bottom();
        d.scaleX = qreal(xTarget[1] - xTarget[0]) / d.width;
        d.scaleY = qreal(yTarget[rows] - yTarget[rows - 1]) / d.height;
        if (hints & QDrawBorderPixmap::OpaqueBottomLeft)
            opaqueData.append(d);
        else
            translucentData.append(d);
    }
    if (targetMargins.bottom() > 0 && targetMargins.right() > 0 && sourceMargins.bottom() > 0 && sourceMargins.right() > 0) { // bottom right
        d.x = (0.5 * (xTarget[columns] + xTarget[columns - 1]));
        d.y = (0.5 * (yTarget[rows] + yTarget[rows - 1]));
        d.sourceLeft = sourceCenterRight;
        d.sourceTop = sourceCenterBottom;
        d.width = sourceMargins.right();
        d.height = sourceMargins.bottom();
        d.scaleX = qreal(xTarget[columns] - xTarget[columns - 1]) / d.width;
        d.scaleY = qreal(yTarget[rows] - yTarget[rows - 1]) / d.height;
        if (hints & QDrawBorderPixmap::OpaqueBottomRight)
            opaqueData.append(d);
        else
            translucentData.append(d);
    }

    // horizontal edges
    if (targetCenterWidth > 0 && sourceCenterWidth > 0) {
        if (targetMargins.top() > 0 && sourceMargins.top() > 0) { // top
            QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueTop ? opaqueData : translucentData;
            d.sourceLeft = sourceCenterLeft;
            d.sourceTop = sourceRect.top();
            d.width = sourceCenterWidth;
            d.height = sourceMargins.top();
            d.y = (0.5 * (yTarget[1] + yTarget[0]));
            d.scaleX = dx / d.width;
            d.scaleY = qreal(yTarget[1] - yTarget[0]) / d.height;
            for (int i = 1; i < columns - 1; ++i) {
                d.x = (0.5 * (xTarget[i + 1] + xTarget[i]));
                data.append(d);
            }
            if (rules.horizontal == Qt::RepeatTile)
                data[data.size() - 1].width = ((xTarget[columns - 1] - xTarget[columns - 2]) / d.scaleX);
        }
        if (targetMargins.bottom() > 0 && sourceMargins.bottom() > 0) { // bottom
            QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueBottom ? opaqueData : translucentData;
            d.sourceLeft = sourceCenterLeft;
            d.sourceTop = sourceCenterBottom;
            d.width = sourceCenterWidth;
            d.height = sourceMargins.bottom();
            d.y = (0.5 * (yTarget[rows] + yTarget[rows - 1]));
            d.scaleX = dx / d.width;
            d.scaleY = qreal(yTarget[rows] - yTarget[rows - 1]) / d.height;
            for (int i = 1; i < columns - 1; ++i) {
                d.x = (0.5 * (xTarget[i + 1] + xTarget[i]));
                data.append(d);
            }
            if (rules.horizontal == Qt::RepeatTile)
                data[data.size() - 1].width = ((xTarget[columns - 1] - xTarget[columns - 2]) / d.scaleX);
        }
    }

    // vertical edges
    if (targetCenterHeight > 0 && sourceCenterHeight > 0) {
        if (targetMargins.left() > 0 && sourceMargins.left() > 0) { // left
            QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueLeft ? opaqueData : translucentData;
            d.sourceLeft = sourceRect.left();
            d.sourceTop = sourceCenterTop;
            d.width = sourceMargins.left();
            d.height = sourceCenterHeight;
            d.x = (0.5 * (xTarget[1] + xTarget[0]));
            d.scaleX = qreal(xTarget[1] - xTarget[0]) / d.width;
            d.scaleY = dy / d.height;
            for (int i = 1; i < rows - 1; ++i) {
                d.y = (0.5 * (yTarget[i + 1] + yTarget[i]));
                data.append(d);
            }
            if (rules.vertical == Qt::RepeatTile)
                data[data.size() - 1].height = ((yTarget[rows - 1] - yTarget[rows - 2]) / d.scaleY);
        }
        if (targetMargins.right() > 0 && sourceMargins.right() > 0) { // right
            QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueRight ? opaqueData : translucentData;
            d.sourceLeft = sourceCenterRight;
            d.sourceTop = sourceCenterTop;
            d.width = sourceMargins.right();
            d.height = sourceCenterHeight;
            d.x = (0.5 * (xTarget[columns] + xTarget[columns - 1]));
            d.scaleX = qreal(xTarget[columns] - xTarget[columns - 1]) / d.width;
            d.scaleY = dy / d.height;
            for (int i = 1; i < rows - 1; ++i) {
                d.y = (0.5 * (yTarget[i + 1] + yTarget[i]));
                data.append(d);
            }
            if (rules.vertical == Qt::RepeatTile)
                data[data.size() - 1].height = ((yTarget[rows - 1] - yTarget[rows - 2]) / d.scaleY);
        }
    }

    // center
    if (targetCenterWidth > 0 && targetCenterHeight > 0 && sourceCenterWidth > 0 && sourceCenterHeight > 0) {
        QPixmapFragmentsArray &data = hints & QDrawBorderPixmap::OpaqueCenter ? opaqueData : translucentData;
        d.sourceLeft = sourceCenterLeft;
        d.sourceTop = sourceCenterTop;
        d.width = sourceCenterWidth;
        d.height = sourceCenterHeight;
        d.scaleX = dx / d.width;
        d.scaleY = dy / d.height;

        qreal repeatWidth = (xTarget[columns - 1] - xTarget[columns - 2]) / d.scaleX;
        qreal repeatHeight = (yTarget[rows - 1] - yTarget[rows - 2]) / d.scaleY;

        for (int j = 1; j < rows - 1; ++j) {
            d.y = (0.5 * (yTarget[j + 1] + yTarget[j]));
            for (int i = 1; i < columns - 1; ++i) {
                d.x = (0.5 * (xTarget[i + 1] + xTarget[i]));
                data.append(d);
            }
            if (rules.horizontal == Qt::RepeatTile)
                data[data.size() - 1].width = repeatWidth;
        }
        if (rules.vertical == Qt::RepeatTile) {
            for (int i = 1; i < columns - 1; ++i)
                data[data.size() - i].height = repeatHeight;
        }
    }

    if (opaqueData.size())
        painter->drawPixmapFragments(opaqueData.data(), opaqueData.size(), pixmap, QPainter::OpaqueHint);
    if (translucentData.size())
        painter->drawPixmapFragments(translucentData.data(), translucentData.size(), pixmap);

    if (oldAA)
        painter->setRenderHint(QPainter::Antialiasing, true);}

MScalableImage::MScalableImage()
    : d_ptr(new MScalableImagePrivate())
{
    Q_D(MScalableImage);
    d->q_ptr = this;
}

MScalableImage::MScalableImage(const QPixmap *pixmap, int left, int right, int top, int bottom, const QString &pixmapId)
    : d_ptr(new MScalableImagePrivate())
{
    Q_D(MScalableImage);
    d->q_ptr = this;

    setBorders(left, right, top, bottom);

    d->m_image = pixmap;
    d->pixmapId = pixmapId;
    d->validateSize();
}

MScalableImage::MScalableImage(MScalableImagePrivate *dd)
    : d_ptr(dd)
{
    Q_D(MScalableImage);
    d->q_ptr = this;
}

MScalableImage::~MScalableImage()
{
    delete d_ptr;
}

void MScalableImage::borders(int *left, int *right, int *top, int *bottom) const
{
    Q_D(const MScalableImage);

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

void MScalableImage::setBorders(int left, int right, int top, int bottom)
{
    Q_D(MScalableImage);

    d->m_preferredMargins = QMargins(left, top, right, bottom);

    if (d->m_preferredMargins.isNull()) {
        d->m_imageType = MScalableImagePrivate::MScalable1;
    }
    /*else if( (top == 0 && bottom == 0) ) {//&& (right != 0 && left != 0 ) ) {
        d->m_imageType = MScalableImagePrivate::MScalable3H;
        mDebug("MScalableImage) << "************** MScalable3H *************";
    }
    else if( (right == 0 && left == 0) ) {//&& (top != 0 &&  bottom != 0 ) ) {
        d->m_imageType = MScalableImagePrivate::MScalable3V;
        mDebug("MScalableImage) << "************** MScalable3V *************";
    }*/
    else {
        d->m_imageType = MScalableImagePrivate::MScalable9;
    }
    d->validateSize();
}

void MScalableImage::setPixmap(const QPixmap *pixmap)
{
    Q_D(MScalableImage);

    d->m_image = pixmap;
    d->pixmapId = "";

    d->validateSize();
}

const QPixmap *MScalableImage::pixmap() const
{
    Q_D(const MScalableImage);
    return d->m_image;
}

void MScalableImage::draw(int x, int y, int w, int h, QPainter *painter) const
{
    draw((qreal)x, (qreal)y, (qreal)w, (qreal)h, painter); 
}

void MScalableImage::draw(const QPoint &pos, const QSize &size, QPainter *painter) const
{
    draw(pos.x(), pos.y(), size.width(), size.height(), painter);
}

void MScalableImage::draw(const QRect &rect, QPainter *painter) const
{
    draw(rect.x(), rect.y(), rect.width(), rect.height(), painter);
}

void MScalableImage::draw(qreal x, qreal y, qreal w, qreal h, QPainter *painter) const
{
    Q_D(const MScalableImage);

    if (!d->m_image) {
        mWarning("MScalableImage") << "MScalableImage::draw() scalable image: " << d->pixmapId << " not properly initialized yet";
        painter->fillRect(QRect(x, y, w, h), QColor(255, 0, 0));
        return;
    }

    switch (d->m_imageType) {
    case MScalableImagePrivate::MScalable1:
        d->drawScalable1(x, y, w, h, painter);
        break;
    case MScalableImagePrivate::MScalable9:
        d->drawScalable9(x, y, w, h, painter);
        break;
    default:
        mWarning("MScalableImage") << d->pixmapId << " draw() unsupported type " << d->m_imageType;
    }
}

void MScalableImage::draw(const QPointF &pos, const QSizeF &size, QPainter *painter) const
{
    draw(pos.x(), pos.y(), size.width(), size.height(), painter);
}

void MScalableImage::draw(const QRectF &rect, QPainter *painter) const
{
    draw(rect.x(), rect.y(), rect.width(), rect.height(), painter);
}

void MScalableImage::enableOptimizedRendering(bool enable)
{
    Q_UNUSED(enable);
}

void MScalableImage::draw(const QRect &rect, const QPoint& pixmapOffset, const QPixmap* pixmap, QPainter *painter) const
{
    if(!pixmap || pixmap->isNull()) {
        // if the pixmap is not valid, draw without filling
        draw(rect, painter);
        return;
    }
    // TODO: create HW version with proper shaders (2 textures, 2 sets of texcoords + shader)

    // SLOW: SW fallback, create temporary render target
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // paint scalable image to target
    QPainter imagePainter(&image);
    draw(QPoint(0,0), rect.size(), &imagePainter);

    // post process the resulting image (add mask)
    QImage fillImage = pixmap->toImage();
    fillImage = fillImage.convertToFormat(QImage::Format_ARGB32);
    int sourceWidth = fillImage.width();
    int sourceHeight = fillImage.height();

    int width = image.width();
    int height = image.height();

    for (int y = 0; y < height; ++y) {
        // target scanline
        uint* target = (uint *) image.scanLine(y);
        // source scanline (tiled)
        const uint* source = (const uint *) fillImage.scanLine((pixmapOffset.y() + y) % sourceHeight);
        for (int x = 0; x < width; ++x) {
            // tile the x coordinate
            uint color = *(source + ((pixmapOffset.x() + x) % sourceWidth));
            // alpha comes from target, colour from source
            *target = ((*target) & 0xff000000) | (color & 0x00ffffff);
            target++;
        }
    }

    painter->drawImage(rect, image);
}

