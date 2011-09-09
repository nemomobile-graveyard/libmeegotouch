/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
    composer(NULL),
    animationGroup(NULL)
{
}

MGraphicsHighlightEffectPrivate::~MGraphicsHighlightEffectPrivate()
{
    delete composer;
}

void MGraphicsHighlightEffectPrivate::applyStyle()
{
    Q_Q(MGraphicsHighlightEffect);

    highlightColor = q->style()->color();
    strength = q->style()->startStrength();

    if (!animationGroup)
        animationGroup = new QSequentialAnimationGroup(q);

    animationGroup->clear();

    QPauseAnimation *delayAnimation = new QPauseAnimation(animationGroup);
    delayAnimation->setDuration(q->style()->delay());

    QPropertyAnimation *strengthAnimation = new QPropertyAnimation(q, "strength", animationGroup);
    strengthAnimation->setDuration(q->style()->duration());
    strengthAnimation->setStartValue(q->style()->startStrength());
    strengthAnimation->setEndValue(q->style()->endStrength());
    deltaStrength = -1;

    animationGroup->addAnimation(delayAnimation);
    animationGroup->addAnimation(strengthAnimation);
    animationGroup->setLoopCount(1);

    animationGroup->start();
}

void MGraphicsHighlightEffectPrivate::drawComposedImage(QPainter *painter)
{
    if (deltaStrength > 0) {
        if (!composer)
            composer = new QPainter();

        if (!composer->isActive()) {
            composer->begin(&pixmapCached);
            composer->setCompositionMode(QPainter::CompositionMode_SourceAtop);
        }
        composer->setOpacity(deltaStrength);
        composer->fillRect(pixmapCached.rect(), highlightColor);
    }

    painter->drawPixmap(offsetCached, pixmapCached);
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
    if (d->deltaStrength == -1)
        d->deltaStrength = strength;
    d->deltaStrength = strength - d->strength;
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

    if (d->pixmapCached.isNull())
        d->pixmapCached = sourcePixmap(Qt::LogicalCoordinates, &d->offsetCached, QGraphicsEffect::NoPad);

    d->drawComposedImage(painter);
}
