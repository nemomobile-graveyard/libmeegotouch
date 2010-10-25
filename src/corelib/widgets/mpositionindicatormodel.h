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

#ifndef MPOSITIONINDICATORMODEL_H
#define MPOSITIONINDICATORMODEL_H

#include <mwidgetmodel.h>

#include <QHash>
#include <QSizeF>
#include <QRectF>
#include <QPointF>

class M_CORE_EXPORT MPositionIndicatorModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MPositionIndicatorModel)

    M_MODEL_PROPERTY(QSizeF, viewportSize, ViewportSize, true, QSizeF())
    M_MODEL_PROPERTY(QRectF, range, Range, true, QRectF())
    M_MODEL_PROPERTY(QPointF, position, Position, true, QPointF())
};

#endif

