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

#ifndef DUIPARTICLEENGINE_H
#define DUIPARTICLEENGINE_H

#include <QtGlobal>
#include <QColor>

//! \internal

/*!
    \brief Struct which contains all information of a particle.
*/
struct DuiParticle {
    uint id;
    bool alive;
    int birthtime; // ms
    int killtime; // ms
    qreal px, py; // pixels
    qreal vx, vy; // pixels / ms
    qreal ax, ay; // pixels / ms^2
    qreal scale;
    qreal sx, sy;
    QRgb color;
};

#include <QAbstractAnimation>
#include <QGraphicsItem>
#include <QPointF>

class DuiParticleEnginePrivate;
class QPainter;

class DuiParticleEngine : public QAbstractAnimation, public QGraphicsItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DuiParticleEngine)

public:
    enum Type {
        Single,
        Continuous
    };
protected:
    /*!
      \brief Pointer to the private data class.

      This pointer should not be used directly, but through Q_D macro.
     */
    DuiParticleEnginePrivate *const d_ptr;

    /*!
      \brief Constructs the animation with paraller animation group inside.

      This constructor is meant to be used inside the libdui to share the
      private data class pointer.
     */
    DuiParticleEngine(DuiParticleEnginePrivate *dd, QObject *parent, DuiParticleEngine::Type type = Continuous);

public:

    DuiParticleEngine(QObject *parent, DuiParticleEngine::Type type = Continuous);
    virtual ~DuiParticleEngine();

    virtual QRectF boundingRect() const;

    void init(int particleCount);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    virtual void updateCurrentTime(int msecs);

    virtual void stepParticle(DuiParticle &p, int dt, int msecs);
    virtual void initParticle(DuiParticle &p, int curmsecs) = 0;
};

//! \internal_end

#endif
