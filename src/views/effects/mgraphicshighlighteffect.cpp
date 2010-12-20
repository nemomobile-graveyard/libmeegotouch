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
 

#include "mgraphicshighlighteffect.h"
#include "mgraphicshighlighteffect_p.h"

#include <QPainter>
#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include <meffectcreator.h>
M_REGISTER_EFFECT(MGraphicsHighlightEffect)

MGraphicsHighlightEffectPrivate::MGraphicsHighlightEffectPrivate()
    : q_ptr(NULL),
    highlightColor(Qt::black),
    strength(0.5),
    composed(NULL),
    composer(NULL),
    animationGroup(NULL),
    delayAnimation(NULL),
    strengthAnimation(NULL)
{
}

MGraphicsHighlightEffectPrivate::~MGraphicsHighlightEffectPrivate()
{
    delete composed;
    delete composer;
}

void MGraphicsHighlightEffectPrivate::applyStyle()
{
    Q_Q(MGraphicsHighlightEffect);

    highlightColor = q->style()->color();
    strength = q->style()->startStrength();

    if (!delayAnimation)
        delayAnimation = new QPauseAnimation(q);

    delayAnimation->setDuration(q->style()->delay());

    if (!strengthAnimation) {
        strengthAnimation = new QPropertyAnimation(q);
        strengthAnimation->setPropertyName("strength");
        strengthAnimation->setTargetObject(q);
    }

    strengthAnimation->setDuration(q->style()->duration());
    strengthAnimation->setStartValue(q->style()->startStrength());
    strengthAnimation->setEndValue(q->style()->endStrength());

    if (!animationGroup) {
        animationGroup = new QSequentialAnimationGroup(q);
        animationGroup->addAnimation(delayAnimation);
        animationGroup->addAnimation(strengthAnimation);
        animationGroup->setLoopCount(1);
    }

    animationGroup->start();
}

void MGraphicsHighlightEffectPrivate::drawComposedImage(QPoint offset, QPainter *painter, const QPixmap *pixmap)
{
    if (!composed || (pixmap->size().width() != composed->size().width() ||
                      pixmap->size().height() != composed->size().height())) {
        if (composed)
            delete composed;
        composed = new QImage(pixmap->size(), QImage::Format_ARGB32_Premultiplied);
    }

    if (!composer)
        composer = new QPainter();

    composed->fill(0);
    if (composer->begin(composed)) {
        composer->setCompositionMode(QPainter::CompositionMode_Source);
        composer->drawPixmap(QRect(QPoint(0, 0), pixmap->size()), *pixmap);
        composer->setCompositionMode(QPainter::CompositionMode_SourceAtop);
        composer->setOpacity(strength);
        composer->fillRect(QRect(QPoint(0, 0), pixmap->size()), highlightColor);
        composer->setOpacity(1.0);
        composer->end();
    }

    painter->drawImage(offset, *composed);
}

MGraphicsHighlightEffect::MGraphicsHighlightEffect(QObject *parent)
    : MGraphicsEffect(parent),
    d_ptr(new MGraphicsHighlightEffectPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->applyStyle();
}

MGraphicsHighlightEffect::~MGraphicsHighlightEffect()
{
    delete d_ptr;
}

void MGraphicsHighlightEffect::setStrength(qreal strength)
{
    Q_D(MGraphicsHighlightEffect);
    d->strength = strength;
    update();
}

qreal MGraphicsHighlightEffect::strength() const
{
    Q_D(const MGraphicsHighlightEffect);
    return d->strength;
}

void MGraphicsHighlightEffect::setHighlightColor(const QColor &color)
{
    Q_D(MGraphicsHighlightEffect);
    d->highlightColor = color;
    update();
}

const QColor &MGraphicsHighlightEffect::highlightColor() const
{
    Q_D(const MGraphicsHighlightEffect);
    return d->highlightColor;
}

void MGraphicsHighlightEffect::draw(QPainter *painter)
{
    Q_D(MGraphicsHighlightEffect);

    QPoint offset;
    QPixmap pixmap;

    if (sourceIsPixmap()) {
        pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset, QGraphicsEffect::PadToEffectiveBoundingRect);
    } else {
        pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset, QGraphicsEffect::PadToEffectiveBoundingRect);
        painter->setWorldTransform(QTransform());
    }

    d->drawComposedImage(offset, painter, &pixmap);
}
