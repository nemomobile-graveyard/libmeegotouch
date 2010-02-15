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

#ifndef DUIPOSITIONINDICATORMODEL_H
#define DUIPOSITIONINDICATORMODEL_H

#include <duiwidgetmodel.h>

#include <QHash>
#include <QSizeF>
#include <QRectF>
#include <QPointF>

class DUI_EXPORT DuiPositionIndicatorModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiPositionIndicatorModel)

    DUI_MODEL_PROPERTY(QSizeF, viewportSize, ViewportSize, true, QSizeF())
    DUI_MODEL_PROPERTY(QRectF, pannedRange, PannedRange, true, QRectF())
    DUI_MODEL_PROPERTY(QPointF, pannedPos, PannedPos, true, QPointF())
};

#endif

