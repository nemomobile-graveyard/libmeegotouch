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

#include "duiparticlecloud.h"

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include "duiscenemanager.h"
#include <math.h>

DuiParticleCloud::DuiParticleCloud(QObject *parent) :
    DuiParticleEngine(parent, DuiParticleEngine::Continuous)
{
}

DuiParticleCloud::~DuiParticleCloud()
{
}

int DuiParticleCloud::duration() const
{
    return -1;
}

void DuiParticleCloud::setPos(const QPointF &pos)
{
    position = pos;
}

void DuiParticleCloud::initParticle(DuiParticle &p, int curmsecs)
{
    p.alive = true;

    qreal targetX, targetY;
    int size = 50;
    int margin = 30;

    qreal dir = qrand() % 360;
    qreal dist = size + qrand() % margin;

    targetX = cos(dir) * dist + position.x();
    targetY = sin(dir) * dist + position.y();

    if (curmsecs == 0) {
        p.px = targetX;//position.x();
        p.py = targetY;//position.y();
    }

    qreal t = 1000 + qrand() % 1000;
    p.vx *= 0.5;
    p.vy *= 0.5;

    p.killtime = curmsecs + t;
    p.birthtime = curmsecs;

    p.ax = 2.0 * (targetX - p.vx * t - p.px) / (t * t);
    p.ay = 2.0 * (targetY - p.vy * t - p.py) / (t * t);

    //p.color = qRgba(/*qrand() % 128, qrand() % 128,*/255,255, 128 + qrand() % 128, 0);
    //p.color = qRgba(/*qrand() % 128, qrand() % 128,*/255,255, 0 + qrand() % 255, 0);
    p.color = qRgba(qrand() % 164, qrand() % 128, 196 + qrand() % 60, 0);
}

void DuiParticleCloud::stepParticle(DuiParticle &p, int dt, int msecs)
{
    qreal x = sin(3.1415 * (msecs - p.birthtime) / (p.killtime - p.birthtime));
    p.color = qRgba(qRed(p.color), qGreen(p.color), qBlue(p.color), 255.0 * x);
    p.scale = x;

    DuiParticleEngine::stepParticle(p, dt, msecs);
}

#endif

