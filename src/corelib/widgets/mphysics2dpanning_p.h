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

#ifndef MPHYSICS2DPANNING_P_H
#define MPHYSICS2DPANNING_P_H

#include <QRectF>
#include <QPointF>
#include <QVariantAnimation>
#include "mphysics2dpanning.h"

class QTimeLine;

class PanningAnimation : public QVariantAnimation
{
    virtual void updateCurrentValue(const QVariant&) {}
};

class MPhysics2DPanningPrivate
{
    Q_DECLARE_PUBLIC(MPhysics2DPanning)

public:
    MPhysics2DPanningPrivate(MPhysics2DPanning *publicClass);
    virtual ~MPhysics2DPanningPrivate();

    bool    enabled;

    QRectF  range;
    qreal   posX;
    qreal   posY;
    qreal   velX;
    qreal   velY;
    qreal   pointerSpringX;
    qreal   pointerSpringY;
    QPointF sceneLastPos;
    qreal   maxVel;

    PanningAnimation *panningAnimation;

    // Integration algorithm constants
    bool  pointerPressed;
    qreal pointerSpringK;
    qreal frictionC;
    qreal slideFrictionC;
    qreal borderSpringK;
    qreal borderFrictionC;
    Qt::Orientations panDirection;

    QList<QPointF>  positions;

    void _q_integrator(const QVariant &value);
    QPointF getVelocity();

protected:
    MPhysics2DPanning *q_ptr;
};

#endif
