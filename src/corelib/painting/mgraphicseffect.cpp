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

#include "mgraphicseffect.h"
#include "mgraphicseffect_p.h"
#include <cmath>

MGraphicsEffectPrivate::MGraphicsEffectPrivate() :
    styleContainer(0),
    level(0),
    intensity(0.0),
    previousLevel(0)
{
}

void MGraphicsEffectPrivate::setLevel(int newLevel)
{
    Q_Q(MGraphicsEffect);

    newLevel = qMax(0, newLevel);

    if (newLevel != level) {
        previousLevel = level;
        level = newLevel;
        intensity = level ? 1.0 : 0.0;

        q->QGraphicsEffect::setEnabled(level || !qFuzzyIsNull(intensity));
        q->update();
    }
}

void MGraphicsEffectPrivate::setIntensity(qreal newIntensity)
{
    Q_Q(MGraphicsEffect);

    if (newIntensity != intensity) {
        intensity = newIntensity;

        q->QGraphicsEffect::setEnabled(level || !qFuzzyIsNull(intensity));
        q->update();
    }
}

MGraphicsEffect::MGraphicsEffect(QObject *parent) :
    QGraphicsEffect(parent),
    d_ptr(new MGraphicsEffectPrivate)
{
    Q_D(MGraphicsEffect);
    d->q_ptr = this;

    QGraphicsEffect::setEnabled(false);
}

MGraphicsEffect::~MGraphicsEffect()
{
    Q_D(MGraphicsEffect);

    delete d->styleContainer;
    delete d_ptr;
}

void MGraphicsEffect::setEnabled(bool enabled, EnablingMode mode)
{
    Q_D(MGraphicsEffect);
    if (mode == CumulativeEnabling)
        d->setLevel(d->level + (enabled ? 1 : -1));
    else
        d->setLevel(enabled ? 1 : 0);
}

qreal MGraphicsEffect::intensity() const
{
    Q_D(const MGraphicsEffect);
    return d->intensity;
}

void MGraphicsEffect::setIntensity(qreal newIntensity)
{
    Q_D(MGraphicsEffect);
    d->setIntensity(newIntensity);
}

qreal MGraphicsEffect::ratio() const
{
    Q_D(const MGraphicsEffect);

    qreal ratio0 = std::pow(baseRatio(), d->previousLevel);
    qreal ratio1 = std::pow(baseRatio(), d->level);

    qreal ratio;
    if (ratio0 > ratio1)
        ratio  = ratio0 + (ratio1 - ratio0) * d->intensity;
    else
        ratio  = ratio1 + (ratio0 - ratio1) * d->intensity;

    return ratio;
}

void MGraphicsEffect::setAnimation(QPropertyAnimation* animation)
{
    Q_D(MGraphicsEffect);
    delete d->animation;
    d->animation = animation;
}

QPropertyAnimation* MGraphicsEffect::animation() const
{
    Q_D(const MGraphicsEffect);
    return d->animation;
}



MGraphicsEffectStyleContainer &MGraphicsEffect::style()
{
    Q_D(MGraphicsEffect);

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
        d->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const MGraphicsEffectStyleContainer &MGraphicsEffect::style() const
{
    Q_D(const MGraphicsEffect);

    if (!d->styleContainer) {
        MGraphicsEffectPrivate *d_p = const_cast<MGraphicsEffectPrivate *>(d);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const char *MGraphicsEffect::styleType() const
{
    return "MGraphicsEffectStyle";
}

MGraphicsEffectStyleContainer *MGraphicsEffect::createStyleContainer() const
{
    return new MGraphicsEffectStyleContainer();
}
