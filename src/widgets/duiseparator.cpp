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

#include "duitheme.h"
#include "duiseparator.h"
#include "duiseparator_p.h"
#include "duiwidgetmodel.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiSeparator)

DuiSeparatorPrivate::DuiSeparatorPrivate() :
    DuiWidgetControllerPrivate()
{
}

DuiSeparatorPrivate::~DuiSeparatorPrivate()
{
}

DuiSeparator::DuiSeparator(QGraphicsItem *parent, Qt::Orientation orientation) :
    DuiWidgetController(new DuiSeparatorPrivate(), new DuiSeparatorModel(), parent)
{
    if (orientation != Qt::Horizontal)
        model()->setOrientation(orientation);
}

DuiSeparator::~DuiSeparator()
{
}

Qt::Orientation DuiSeparator::orientation()
{
    return model()->orientation();
}

void DuiSeparator::setOrientation(Qt::Orientation orientation)
{
    model()->setOrientation(orientation);
}

