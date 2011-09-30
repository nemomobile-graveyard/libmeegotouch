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
#include "scroller.h"
#include "scrollablewidget.h"

QPoint Scroller::maximumOffset;

Scroller::Scroller()
{
}

Scroller::~Scroller()
{
}

QPoint Scroller::queryScrollingAmount(const QGraphicsWidget *widget,
                                      const QRect &targetRect,
                                      const QPoint &originPoint,
                                      const QPoint &currentOffset)
{
    const ScrollableWidget *scrollable = dynamic_cast<const ScrollableWidget *>(widget);
    if (!scrollable) {
        return QPoint();
    }

    const QRect boundaries(scrollable->boundingRect().toRect());

    QRect movedTargetRect(targetRect);
    moveRectInsideArea(boundaries, movedTargetRect);

    const QPoint offset(movedTargetRect.topLeft() - originPoint);
    const QPoint lowerBound(-maximumOffset - currentOffset);
    const QPoint upperBound(maximumOffset - currentOffset);
    const QPoint limitedOffset(qBound(lowerBound.x(), offset.x(), upperBound.x()),
                               qBound(lowerBound.y(), offset.y(), upperBound.y()));
    return limitedOffset;
}

void Scroller::applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset,
                              int duration, const QEasingCurve &easingCurve)
{
    ScrollableWidget *scrollableWidget = dynamic_cast<ScrollableWidget *>(widget);
    if (scrollableWidget) {
        scrollableWidget->scrollContents(contentsOffset);
    }

    lastScrollDuration = duration;
    lastEasingCurve = easingCurve;
}

void Scroller::restoreScrolling(QGraphicsWidget *widget)
{
    ScrollableWidget *scrollableWidget = dynamic_cast<ScrollableWidget *>(widget);
    if (scrollableWidget) {
        scrollableWidget->clearScroll();
    }
}
