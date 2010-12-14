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

#include "mpannableviewportscroller.h"
#include "mpannableviewport.h"
#include "mpannableviewport_p.h"

QPoint MPannableViewportScroller::queryScrollingAmount(const QGraphicsWidget *widget,
                                                       const QRect &targetRect,
                                                       const QPoint &originPoint,
                                                       const QPoint &)
{
    const MPannableViewport *viewport = static_cast<const MPannableViewport *>(widget);

    if (viewport->verticalPanningPolicy() == MPannableViewport::PanningAlwaysOff) {
        return QPoint(); // unable to scroll
    }

    // We won't be scrolling horizontally currently to prevent movement caused by
    // errors in container widget margins etc. It's straightforward to add if needed.

    // First ensure that target rectangle is inside of area of the pannable viewport.
    // Note: We might even move against the wanted direction but this is required to
    // ensure the visibility of the area marked by target rectangle.
    QRect visibleTargetRect(targetRect);
    moveRectInsideArea(viewport->contentsRect().toRect(), visibleTargetRect);

    // Calculate how much pannable contents should be translated.
    const QPoint contentsOffset(visibleTargetRect.topLeft() - originPoint);

    // Calculate the new panning position, i.e. position of the pannable viewport
    // in panned widget coordinates.
    QPointF panningPos(viewport->position() - contentsOffset);

    // Get allowed range for position to be used with MPannableWidget::setPosition().
    QRectF posRange = viewport->range();

    // ...and limit our panning accordingly.
    panningPos.ry() = qMax(posRange.top(), panningPos.y()); // We can extend bottom limit.

    const QPoint translation(0, viewport->position().y() - panningPos.y());
    return translation;
}

void MPannableViewportScroller::applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset)
{
    MPannableViewport *viewport = static_cast<MPannableViewport *>(widget);

    viewport->d_func()->scrollTo(viewport->position() - contentsOffset);

    // Disables kinetic scrolling until next pointer event.
    viewport->physics()->stop();

    //TODO: After scrolling of every widget the topmost pannable viewport should make sure its range
    // covers at least the sip.
}

void MPannableViewportScroller::restoreScrolling(QGraphicsWidget *widget)
{
    MPannableViewport *viewport = static_cast<MPannableViewport *>(widget);
    viewport->d_func()->setAutoScrollingExtension(0);
}

