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

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include "duiparticleengine.h"
#include "duiscenemanager.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiglrenderer.h"
#include <QPainter>


class DuiParticleEnginePrivate
{
    Q_DECLARE_PUBLIC(DuiParticleEngine)
protected:
    DuiParticleEngine *q_ptr;
private:
    DuiParticleEngine::Type type;
    DuiParticle *particles;
    int particleCount;
    int lastmsecs;
    QPixmap *pixmap;
    QPixmap *pixmaps;
};

DuiParticleEngine::DuiParticleEngine(DuiParticleEnginePrivate *dd, QObject *parent, DuiParticleEngine::Type type) :
    QAbstractAnimation(parent),
    QGraphicsItem(NULL),
    d_ptr(dd)
{
    Q_D(DuiParticleEngine);
    d->q_ptr = this;
    d->type = type;
    d->lastmsecs = 0;
    d->pixmap = new QPixmap("partikkeli.png");
}


DuiParticleEngine::DuiParticleEngine(QObject *parent, DuiParticleEngine::Type type) :
    QAbstractAnimation(parent),
    QGraphicsItem(NULL),
    d_ptr(new DuiParticleEnginePrivate)
{
    Q_D(DuiParticleEngine);
    d->q_ptr = this;
    d->type = type;
    d->lastmsecs = 0;
    d->pixmap = new QPixmap("partikkeli.png");
    d->pixmaps = new QPixmap[10];

    QImage baseParticle = d->pixmap->toImage();

    for (int i = 0; i < 10; ++i) {
        QImage particle = baseParticle.copy();

        const int particleHeight = particle.height();
        for (int y = 0; y < particleHeight; ++y) {
            uchar *scanline = particle.scanLine(y);

            const int particleWidth = particle.width();
            for (int x = 0; x < particleWidth; ++x) {
                uint *p = (uint *)scanline + x;

                qreal blueFactor = 0.3 + 0.7 * ((qreal)i) / 10.0;
                *p = qRgba(qRed(*p), qGreen(*p), qBlue(*p) * blueFactor, qAlpha(*p));
            }
        }

        d->pixmaps[i] = QPixmap::fromImage(particle);
    }
}

DuiParticleEngine::~DuiParticleEngine()
{
    delete [] d_ptr->particles;
    delete d_ptr->pixmap;
    delete d_ptr;
}

QRectF DuiParticleEngine::boundingRect() const
{
    return QRectF(QPointF(0, 0), DuiApplication::activeWindow()->visibleSceneSize());
}

void DuiParticleEngine::init(int particleCount)
{
    Q_D(DuiParticleEngine);
    d->particles = new DuiParticle[particleCount];
    d->particleCount = particleCount;
    for (int i = 0; i < particleCount; ++i) {
        DuiParticle &p = d->particles[i];
        p.id = i;
        p.alive = false;
        p.killtime = p.birthtime = 0;
        p.px = p.py = p.vx = p.vy = p.ax = p.ay = 0.0;
        p.scale = 1.0;
        p.sx = 128.0;
        p.sy = 128.0;
        p.color = qRgba(255, 255, 255, 255);
        initParticle(p, 0);
    }
}

void DuiParticleEngine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    Q_D(DuiParticleEngine);

#ifdef DUI_USE_OPENGL
#if QT_VERSION >= 0x040600
    if (painter->paintEngine()->type() == QPaintEngine::OpenGL2) {
#else
    if (painter->paintEngine()->type() == QPaintEngine::OpenGL) {
#endif
        DuiGLRenderer::instance()->drawParticles(d->particles, d->particleCount, *d->pixmap, painter->combinedTransform());
    } else
#endif
    {
        painter->save();
        painter->setCompositionMode(QPainter::CompositionMode_Plus);
        for (int i = 0; i < d->particleCount; ++i) {
            DuiParticle &p = d->particles[i];

            // particle not alive
            if (!p.alive)
                continue;

            painter->setOpacity((qreal)qAlpha(p.color) / 255.0);
            qreal sx = p.sx / 2.0;
            qreal sy = p.sy / 2.0;
            painter->drawPixmap(QRect(p.px - sx * p.scale, p.py - sy * p.scale, p.sx * p.scale, p.sy * p.scale), d->pixmaps[i%10]);

            // draw the particle with given color,opacity,scale
            // to position p.px,p.py (as center)
        }
        painter->restore();
    }
}

void DuiParticleEngine::updateCurrentTime(int msecs)
{
    Q_D(DuiParticleEngine);
    bool done = true;
    int dt = msecs - d->lastmsecs;

    for (int i = 0; i < d->particleCount; ++i) {
        DuiParticle &p = d->particles[i];

        // particle not alive
        if (!p.alive)
            continue;

        // should we kill the particle?
        if (p.killtime <= msecs) {
            p.alive = false;
            if (d->type == Continuous)
                initParticle(p, msecs);

            if (p.alive)
                done = false;

            continue;
        }
        stepParticle(p, dt, msecs);

        done = false;
    }

    // no more particles alive
    if (done) {
        stop();
        emit finished();
    }

    d->lastmsecs = msecs;
    update();
}

void DuiParticleEngine::stepParticle(DuiParticle &p, int dt, int msecs)
{
    Q_UNUSED(msecs);
    p.px += dt * dt * p.ax / 2.0 + p.vx * dt;
    p.py += dt * dt * p.ay / 2.0 + p.vy * dt;

    p.vx += dt * p.ax;
    p.vy += dt * p.ay;
}

#endif

