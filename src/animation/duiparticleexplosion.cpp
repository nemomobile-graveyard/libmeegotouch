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

#include "duiparticleexplosion.h"

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <math.h>

DuiParticleExplosion::DuiParticleExplosion(QObject *parent) :
    DuiParticleEngine(parent, DuiParticleEngine::Single)
{
}

DuiParticleExplosion::~DuiParticleExplosion()
{
}

int DuiParticleExplosion::duration() const
{
    return -1;
}

void DuiParticleExplosion::setPos(const QPointF &pos)
{
    position = pos;
}

void DuiParticleExplosion::initParticle(DuiParticle &p, int curmsecs)
{
    p.alive = true;
    p.px = position.x();
    p.py = position.y();

    qreal vel = 0.05 + 0.5 * (qrand() % 1000) / 1000.0;
    qreal dir = (qrand() % 61415) / 10000.0; // dir = [0, 2PI]

    p.vx = sin(dir) * vel;
    p.vy = cos(dir) * vel;

    p.ax = -p.vx * 0.001;
    p.ay = -p.vy * 0.001;

    p.killtime = curmsecs + 5000;
}

#endif

