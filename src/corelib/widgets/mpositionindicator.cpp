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

#include <mtheme.h>
#include "mpositionindicator.h"
#include "mpositionindicator_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MPositionIndicator)

MPositionIndicator::MPositionIndicator(QGraphicsItem *parent)
    : MWidgetController(new MPositionIndicatorModel, parent)
{
    setAcceptedMouseButtons(0);
}

MPositionIndicator::MPositionIndicator(MPositionIndicatorPrivate* /*dd*/, MPositionIndicatorModel *model,
        QGraphicsItem *parent)
    : MWidgetController(NULL, model, parent)
{
    setAcceptedMouseButtons(0);
}

MPositionIndicator::~MPositionIndicator()
{
}

QSizeF MPositionIndicator::viewportSize() const
{
    return model()->viewportSize();
}

QRectF MPositionIndicator::range() const
{
    return model()->range();
}

QPointF MPositionIndicator::position() const
{
    return model()->position();
}

void MPositionIndicator::setPosition(const QPointF &newPosition)
{
    if (position() != newPosition) {
        model()->setPosition(newPosition);
    }
}
void MPositionIndicator::setViewportSize(const QSizeF &newViewportSize)
{
    if (viewportSize() != newViewportSize) {
        model()->setViewportSize(newViewportSize);
    }
}

void MPositionIndicator::setRange(const QRectF &newRange)
{
    if (range() != newRange) {
        model()->setRange(newRange);
    }
}
