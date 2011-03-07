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

#include "mabstractscroller.h"

#include <QRect>

MAbstractScroller::MAbstractScroller()
{
}

MAbstractScroller::~MAbstractScroller()
{
}

void MAbstractScroller::stopScrolling(QGraphicsWidget *)
{
    // Empty default implementation. Scrollers can use this if they lack animation.
}

void MAbstractScroller::restoreScrolling(QGraphicsWidget *)
{
    // Empty default implementation.
}

void MAbstractScroller::moveRectInsideArea(const QRect &area, QRect &rect)
{
    rect.moveLeft(qBound(area.left(), rect.x(), area.x() + area.width() - rect.width()));
    rect.moveTop(qBound(area.top(), rect.top(), area.y() + area.height() - rect.height()));
}
