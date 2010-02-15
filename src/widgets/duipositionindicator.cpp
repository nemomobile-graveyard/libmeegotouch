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

QRectF DuiPositionIndicator::pannedRange() const
{
    return model()->pannedRange();
}

QPointF DuiPositionIndicator::pannedPos() const
{
    return model()->pannedPos();
}

void DuiPositionIndicator::updateSizePosData(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos)
{
    if (this->viewportSize() != viewportSize) {
        model()->setViewportSize(viewportSize);
    }

    if (this->pannedRange() != pannedRange) {
        model()->setPannedRange(pannedRange);
    }

    if (this->pannedPos() != pannedPos) {
        model()->setPannedPos(pannedPos);
    }
}

