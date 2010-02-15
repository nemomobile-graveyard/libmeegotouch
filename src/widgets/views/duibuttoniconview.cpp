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

#include "duibuttoniconview.h"
#include "duibuttoniconview_p.h"

#include <QPainter>
#include <QTimeLine>

#include "duiviewcreator.h"
#include "duibutton.h"
#include "duibutton_p.h"
#include "duilabel.h"
#include "duitheme.h"


// TODO: to be removed when the glow pixmap generation is moved to application side as an effect.
QImage glow(const QImage &image, int radius, const QColor &color);

DuiButtonIconViewPrivate::DuiButtonIconViewPrivate()
    : timelineShrink(new QTimeLine())
    , timelineGlow(new QTimeLine())
{
    timelineShrink->setCurveShape(QTimeLine::EaseInCurve);
    timelineGlow->setCurveShape(QTimeLine::SineCurve);
}

DuiButtonIconViewPrivate::~DuiButtonIconViewPrivate()
{
    delete timelineShrink;
    delete timelineGlow;
}

void DuiButtonIconViewPrivate::drawGlowIcon(QPainter *painter, const QRectF &iconRect) const
{
    Q_Q(const DuiButtonIconView);

    if (!icon)
        return;

    QRectF glowRect = iconRect;
    QPointF offset(-q->style()->glowRadius(),
                   -q->style()->glowRadius());
    glowRect.translate(offset);
    glowRect.setSize(QSizeF(icon->width() + 2 * q->style()->glowRadius(), icon->height() + 2 * q->style()->glowRadius()));

    painter->setOpacity(controller->effectiveOpacity() * timelineGlow->currentValue());
    // TODO: cache the glow image/pixmap, and update it only if icon->handle() has changed.
    painter->drawImage(glowRect, glow(icon->toImage(), q->style()->glowRadius(), q->style()->glowColor()));
}

DuiButtonIconView::DuiButtonIconView(DuiButton *controller) :
    DuiButtonView(* new DuiButtonIconViewPrivate, controller)
{
    Q_D(DuiButtonIconView);

    connect(d->timelineShrink, SIGNAL(valueChanged(qreal)),
            this, SLOT(scaleValueChanged(qreal)));

    connect(d->timelineGlow, SIGNAL(valueChanged(qreal)),
            this, SLOT(glowValueChanged(qreal)));
}

DuiButtonIconView::~DuiButtonIconView()
{
}

void DuiButtonIconView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const DuiButtonIconView);

    bool scaling = (d->timelineShrink->state() == QTimeLine::Running) || (model()->down());
    bool glowing = (!model()->down()) && (d->timelineGlow->state() == QTimeLine::Running) &&
                   model()->iconVisible();

    if (!scaling && !glowing) {
        DuiButtonView::drawContents(painter, option);
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

void DuiButtonIconView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void DuiButtonIconView::applyStyle()
{
    Q_D(DuiButtonIconView);

    DuiButtonView::applyStyle();

    d->timelineShrink->setDuration(style()->shrinkDuration());
    d->timelineGlow->setDuration(style()->glowDuration());
}

void DuiButtonIconView::scaleValueChanged(qreal value)
{
    Q_UNUSED(value);
    Q_D(DuiButtonIconView);

    float s = 1.0f - style()->shrinkFactor() * value;
    if (s != 1.0f)
        d->label->setTransformOriginPoint(d->label->size().width() / 2.0f, d->label->size().height() / 2.0f);
    else
        d->label->setTransformOriginPoint(0.0f, 0.0f);
    d->label->setScale(s);

    update();
}

void DuiButtonIconView::glowValueChanged(qreal value)
{
    Q_UNUSED(value);
    update();
}


void DuiButtonIconView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiButtonIconView);

    DuiButtonView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiButtonModel::Down) {
            //start shrinking animation
            d->timelineShrink->setDirection(model()->down() ? QTimeLine::Forward : QTimeLine::Backward);
            if (d->timelineShrink->state() == QTimeLine::NotRunning)
                d->timelineShrink->start();

            //start glowing if the button was released
            if (!model()->down()) {
                if (d->timelineGlow->state() == QTimeLine::Running)
                    d->timelineGlow->setCurrentTime(0);
                else
                    d->timelineGlow->start();
            }
        }
    }
}

DUI_REGISTER_VIEW_NEW(DuiButtonIconView, DuiButton)


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

QImage glow(const QImage &image, int radius, const QColor &color)
{
    QImage g(image.width() + 2 * radius, image.height() + 2 * radius, QImage::Format_ARGB32);
    blur(&image, &g, radius, color);
    return g;
}
