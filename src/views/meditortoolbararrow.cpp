/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "meditortoolbararrow.h"

MEditorToolbarArrow::MEditorToolbarArrow(QGraphicsItem *parentItem)
    : MStylableWidget(parentItem),
      arrowDirection(ArrowDown)
{
}

MEditorToolbarArrow::ArrowDirection MEditorToolbarArrow::direction()
{
    return arrowDirection;
}

void MEditorToolbarArrow::setDirection(ArrowDirection direction)
{
    arrowDirection = direction;

    switch (direction) {
    case ArrowUp:
        style().setModeUp();
        break;
    case ArrowDown:
        style().setModeDown();
        break;
    }

    // Update new size.
    applyStyle();
}
