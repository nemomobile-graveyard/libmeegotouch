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

#include "duiparticlefountain.h"

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include <math.h>

DuiParticleFountain::DuiParticleFountain(QObject *parent) :
    DuiParticleEngine(parent, DuiParticleEngine::Continuous)
{
}

DuiParticleFountain::~DuiParticleFountain()
{
}

int DuiParticleFountain::duration() const
{
    return -1;
}

void DuiParticleFountain::initParticle(DuiParticle &p, int curmsecs)
{
    p.alive = true;
    p.px = 864 / 2;
    p.py = 480 / 2;

    p.vx = -0.1 + 0.2 * (qrand() % 1000) / 1000.0;
    p.vy = -0.3 - 0.2 * (qrand() % 1000) / 1000.0;

    p.ax = 0;
    p.ay = 0.0005;

    p.color = qRgba(qrand() % 255, qrand() % 255, qrand() % 255, 255);

    p.killtime = curmsecs + 1000 + qrand() % 1000;
}

#endif

