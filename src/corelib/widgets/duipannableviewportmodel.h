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

#ifndef DUIPANNABLEVIEWPORTMODEL_H
#define DUIPANNABLEVIEWPORTMODEL_H

#include <duipannablewidgetmodel.h>

class DUI_EXPORT DuiPannableViewportModel : public DuiPannableWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiPannableViewportModel)

    DUI_MODEL_PROPERTY(bool, autoRange, AutoRange, true, true)
    DUI_MODEL_PROPERTY(bool, clipWidget, ClipWidget, true, true)
};

#endif

