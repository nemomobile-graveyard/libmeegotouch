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

#ifndef DUIPHYSICS2DPANNING_P_H
#define DUIPHYSICS2DPANNING_P_H

#include <QSizeF>
#include <QRectF>
#include <QPointF>
#include "duiphysics2dpanning.h"

class DuiStyle;
class QTimeLine;
class DuiPannableWidget;

class DuiPhysics2DPanningPrivate
{
public:
    DuiPhysics2DPanningPrivate(DuiPannableWidget *parentPannableWidget);
    virtual ~DuiPhysics2DPanningPrivate();

    DuiPannableWidget *parentPannableWidget;

    DuiPhysics2DIntegrationStrategy::IntegrationData   integrationData;

    QRectF  range;
    qreal   posX;
    qreal   posY;
    qreal   velX;
    qreal   velY;
    qreal   pointerSpringX;
    qreal   pointerSpringY;
    QPointF sceneLastPos;

    QTimeLine *timeLine;
    int currFrame;

    DuiPhysics2DIntegrationStrategy *integrationStrategy;
};

#endif
