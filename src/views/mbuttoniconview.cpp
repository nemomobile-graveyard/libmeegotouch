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

#include "mbuttoniconview.h"
#include "mbuttoniconview_p.h"

#include <QPainter>
#include <QTimeLine>

#include "mviewcreator.h"
#include "mbutton.h"
#include "mbutton_p.h"
#include "mlabel.h"
#include "mtheme.h"


MButtonIconViewPrivate::MButtonIconViewPrivate()
    : timelineShrink(new QTimeLine())
    , timelineGlow(new QTimeLine())
    , glowCacheKey( 0 )
    , glowRadius( 0 )
{
    timelineShrink->setCurveShape(QTimeLine::EaseInCurve);
    timelineGlow->setCurveShape(QTimeLine::SineCurve);
}

MButtonIconViewPrivate::~MButtonIconViewPrivate()
{
    delete timelineShrink;
    delete timelineGlow;
}

void MButtonIconViewPrivate::drawGlowIcon(QPainter *painter, const QRectF &iconRect) const
{
    Q_Q(const MButtonIconView);

    if (!icon)
        return;

    QRectF glowRect = iconRect;
    QPointF offset(-q->style()->glowRadius(),
                   -q->style()->glowRadius());
    glowRect.translate(offset);
    glowRect.setSize(QSizeF(icon->pixmap->width() + 2 * q->style()->glowRadius(), icon->pixmap->height() + 2 * q->style()->glowRadius()));

    painter->setOpacity(controller->effectiveOpacity() * timelineGlow->currentValue());
    painter->drawImage(glowRect, const_cast<MButtonIconViewPrivate*>(this)->glowIcon(icon->pixmap, q->style()->glowRadius(), q->style()->glowColor()));
}

MButtonIconView::MButtonIconView(MButton *controller) :
    MButtonView(* new MButtonIconViewPrivate, controller)
{
    Q_D(MButtonIconView);

    connect(d->timelineShrink, SIGNAL(valueChanged(qreal)),
            this, SLOT(scaleValueChanged(qreal)));

    connect(d->timelineGlow, SIGNAL(valueChanged(qreal)),
            this, SLOT(glowValueChanged(qreal)));
}

MButtonIconView::~MButtonIconView()
{
}

void MButtonIconView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MButtonIconView);

    bool scaling = (d->timelineShrink->state() == QTimeLine::Running) || (model()->down());
    bool glowing = (!model()->down()) && (d->timelineGlow->state() == QTimeLine::Running) &&
                   model()->iconVisible();

    if (!scaling && !glowing) {
        MButtonView::drawContents(painter, option);
    } else {
        QRectF iconRect = d->iconRect;

        if (scaling) {
            int w = size().width() / 2;
            int h = size().height() / 2;
            painter->translate(QPoint(w, h));

            // update iconRect and textRect for translate
            QPointF offset(-w, -h);
            iconRect.translate(offset);

            // Scales the painting if zoom timeline running or button pressed
            float value = d->timelineShrink->currentValue();
            float s = 1.0f - style()->shrinkFactor() * value;
            painter->setRenderHint(QPainter::SmoothPixmapTransform);
            painter->scale(s, s);
        }

        // draw glow icon
        if (glowing)
            d->drawGlowIcon(painter, iconRect);

        painter->setOpacity(d->controller->effectiveOpacity());

        drawIcon(painter, iconRect);
    }
}

void MButtonIconView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    MButtonView::drawBackground(painter, option);
}

void MButtonIconView::applyStyle()
{
    Q_D(MButtonIconView);

    MButtonView::applyStyle();

    if (style()->shrinkDuration() > 0)
        d->timelineShrink->setDuration(style()->shrinkDuration());

    if (style()->glowDuration() > 0)
        d->timelineGlow->setDuration(style()->glowDuration());
}

void MButtonIconView::scaleValueChanged(qreal value)
{
    Q_UNUSED(value);
    Q_D(MButtonIconView);

    float s = 1.0f - style()->shrinkFactor() * value;
    if (s != 1.0f)
        d->label->setTransformOriginPoint(d->label->size().width() / 2.0f, d->label->size().height() / 2.0f);
    else
        d->label->setTransformOriginPoint(0.0f, 0.0f);
    d->label->setScale(s);

    update();
}

void MButtonIconView::glowValueChanged(qreal value)
{
    Q_UNUSED(value);
    update();
}


void MButtonIconView::updateData(const QList<const char *>& modifications)
{
    Q_D(MButtonIconView);

    MButtonView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Down) {
            //start shrinking animation
            d->timelineShrink->setDirection(model()->down() ? QTimeLine::Forward : QTimeLine::Backward);
            if (style()->shrinkDuration() > 0 && d->timelineShrink->state() == QTimeLine::NotRunning)
                d->timelineShrink->start();

            //start glowing if the button was released
            if (!model()->down()) {
                if (d->timelineGlow->state() == QTimeLine::Running)
                    d->timelineGlow->setCurrentTime(0);
                else if (style()->glowDuration() > 0) {
                    d->timelineGlow->start();
                }
            }
        }
    }
}

M_REGISTER_VIEW_NEW(MButtonIconView, MButton)


////////////////////////////////////////////////////
// glow generation


// blur image
static void blur(const QImage *source, QImage *destination, int radius, const QColor &color)
{
    QSize s = destination->size();
    int width = s.width();
    int height = s.height();

    QImage tmp(s, QImage::Format_ARGB32);

    qreal GlowColorR  = color.redF();
    qreal GlowColorG  = color.greenF();
    qreal GlowColorB  = color.blueF();

    int total, alpha;
    QRgb *buffer;

    // horizontal
    for (int y = 0; y < height; ++y) {
        total = 0;
        buffer = (QRgb *)tmp.scanLine(y);

        // Process entire window for first pixel
        //for (int kx = -radius; kx <= radius; ++kx)
        if (y >= radius && y < height - radius) {
            total = qAlpha(source->pixel(0, y - radius));

            alpha = total / (radius * 2 + 1);
            *buffer = qRgba(0, 0, 0, alpha);
            buffer++;

            // Subsequent pixels just update window total
            for (int x = 1; x < width; ++x) {
                // Subtract pixel leaving window
                if (x - radius * 2 - 1 >= 0) // && x - radius*2 < source->size().width())
                    total -= qAlpha(source->pixel(x - radius * 2 - 1, y - radius));

                // Add pixel entering window
                if (x > 0 && x < source->size().width())
                    total += qAlpha(source->pixel(x, y - radius));

                alpha = total / (radius * 2 + 1);

                *buffer = qRgba(0, 0, 0, alpha);
                buffer++;
            }
        } else {
            for (int x = 0; x < width; ++x) {
                *buffer = qRgba(0, 0, 0, 0);
                buffer++;
            }
        }
    }

    qreal r, g, b, a;
    // vertical
    for (int x = 0; x < width; ++x) {
        total = 0;

        // Process entire window for first pixel
        //for (int ky = -radius; ky <= radius; ++ky)
        for (int ky = 0; ky <= radius; ++ky)
            total += qAlpha(tmp.pixel(x, ky));

        alpha = total / (radius * 2 + 1);

        r = alpha * GlowColorR * 2;
        g = alpha * GlowColorG * 2;
        b = alpha * GlowColorB * 2;
        a = alpha * 2;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (a > 255) a = 255;

        buffer = (QRgb *)destination->scanLine(0);
        *(buffer + x) = qRgba(r, g, b, a);

        // Subsequent pixels just update window total
        for (int y = 1; y < height; ++y) {
            // Subtract pixel leaving window
            if (y - radius - 1 >= 0)
                total -= qAlpha(tmp.pixel(x, y - radius - 1));

            // Add pixel entering window
            if (y + radius < height)
                total += qAlpha(tmp.pixel(x, y + radius));

            alpha = total / (radius * 2 + 1);

            r = alpha * GlowColorR * 2;
            g = alpha * GlowColorG * 2;
            b = alpha * GlowColorB * 2;
            a = alpha * 2;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (a > 255) a = 255;

            buffer = (QRgb *)destination->scanLine(y);
            *(buffer + x) = qRgba(r, g, b, a);
        }
    }
}

// TODO: to be removed when the glow pixmap generation is moved to application side as an effect.
const QImage& MButtonIconViewPrivate::glowIcon(const QPixmap* pm, int radius, const QColor &color)
{
    if( glowImage.isNull() || glowCacheKey != pm->cacheKey() || glowRadius != radius || glowColor != color ) {
        QImage image = pm->toImage();
        glowImage = QImage(pm->width() + 2 * radius, pm->height() + 2 * radius, QImage::Format_ARGB32);
        blur(&image, &glowImage, radius, color);
        glowCacheKey = pm->cacheKey();
        glowRadius = radius;
        glowColor = color;
    }
    return glowImage;  
}
