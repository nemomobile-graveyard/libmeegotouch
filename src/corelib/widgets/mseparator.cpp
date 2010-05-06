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

#include "mtheme.h"
#include "mseparator.h"
#include "mseparator_p.h"
#include "mwidgetmodel.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MSeparator)

MSeparatorPrivate::MSeparatorPrivate() :
    MWidgetControllerPrivate()
{
}

MSeparatorPrivate::~MSeparatorPrivate()
{
}

MSeparator::MSeparator(QGraphicsItem *parent, Qt::Orientation orientation) :
    MWidgetController(new MSeparatorPrivate(), new MSeparatorModel(), parent)
{
    if (orientation != Qt::Horizontal)
        model()->setOrientation(orientation);
}

MSeparator::~MSeparator()
{
}

Qt::Orientation MSeparator::orientation()
{
    return model()->orientation();
}

void MSeparator::setOrientation(Qt::Orientation orientation)
{
    model()->setOrientation(orientation);
}

