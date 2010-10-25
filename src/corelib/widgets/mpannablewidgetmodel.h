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

#ifndef MPANNABLEWIDGETMODEL_H
#define MPANNABLEWIDGETMODEL_H

#include <mwidgetmodel.h>

class M_CORE_EXPORT MPannableWidgetModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MPannableWidgetModel)

    M_MODEL_PROPERTY(bool, enabled, Enabled, true, true)
    M_MODEL_PROPERTY(int, verticalPanningPolicy, VerticalPanningPolicy, true, 0)
    M_MODEL_PROPERTY(int, horizontalPanningPolicy, HorizontalPanningPolicy, true, 1)

    //Deprecated:
    M_MODEL_PROPERTY(qreal, panThreshold, PanThreshold, true, 10.0)
    M_MODEL_PROPERTY(qreal, panClickThreshold, PanClickThreshold, true, 0.2)
    M_MODEL_PROPERTY(Qt::Orientations, panDirection, PanDirection, true, Qt::Vertical)
};

#endif
