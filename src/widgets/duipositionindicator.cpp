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

#include <duitheme.h>
#include "duipositionindicator.h"
#include "duipositionindicator_p.h"
#include "duipositionindicatorview.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiPositionIndicator)

DuiPositionIndicator::DuiPositionIndicator(QGraphicsItem *parent)
    : DuiWidgetController(new DuiPositionIndicatorModel, parent)
{
    setAcceptedMouseButtons(0);
}

DuiPositionIndicator::DuiPositionIndicator(DuiPositionIndicatorPrivate* /*dd*/, DuiPositionIndicatorModel *model,
        QGraphicsItem *parent)
    : DuiWidgetController(NULL, model, parent)
{
    setAcceptedMouseButtons(0);
}

DuiPositionIndicator::~DuiPositionIndicator()
{
}

QSizeF DuiPositionIndicator::viewportSize() const
{
    return model()->viewportSize();
}

QRectF DuiPositionIndicator::range() const
{
    return model()->range();
}

QPointF DuiPositionIndicator::position() const
{
    return model()->position();
}

void DuiPositionIndicator::setPosition(const QPointF &newPosition)
{
    if (position() != newPosition) {
        model()->setPosition(newPosition);
    }
}
void DuiPositionIndicator::setViewportSize(const QSizeF &newViewportSize)
{
    if (viewportSize() != newViewportSize) {
        model()->setViewportSize(newViewportSize);
    }
}

void DuiPositionIndicator::setRange(const QRectF &newRange)
{
    if (range() != newRange) {
        model()->setRange(newRange);
    }
}
