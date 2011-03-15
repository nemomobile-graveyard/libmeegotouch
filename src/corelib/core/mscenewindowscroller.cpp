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

#include "mscenewindowscroller.h"
#include "mscenewindow.h"
#include "mscenemanager.h"
#include "minputmethodstate.h"
#include <QDebug>

QPoint MSceneWindowScroller::queryScrollingAmount(const QGraphicsWidget *widget,
                                                  const QRect &targetRect,
                                                  const QPoint &originPoint,
                                                  const QPoint &currentOffset)
{
    const MSceneWindow *sceneWindow = static_cast<const MSceneWindow *>(widget);

    // Don't scroll window types that can have some kind of top bar, and,
    // that do have some other way of scrolling their contents.
    if (sceneWindow->windowType() == MSceneWindow::ApplicationPage ||
        sceneWindow->windowType() == MSceneWindow::Sheet)
    {
        return QPoint();
    }

    // Moving currently only in vertical direction.
    // Assuming panel is at the bottom of scene, and also assuming no parent's are being scrolled.

    const QRect sceneRect(QRect(QPoint(), sceneWindow->sceneManager()->visibleSceneSize(M::Landscape)));
    const QRect mappedSceneRect(sceneWindow->mapRectFromScene(sceneRect).toRect());

    const int distanceFromBottom = (mappedSceneRect.bottom() - currentOffset.y()) - sceneWindow->rect().toRect().bottom();

    // Negative offset increases distance from bottom.
    const QPoint offset(targetRect.topLeft() - originPoint);
    int newDistanceFromBottom = distanceFromBottom - offset.y();

    if (newDistanceFromBottom > 0) {
        // Don't allow gap between panel top and bottom of scene window.
        const QRect inputPanelRect = MInputMethodState::instance()->inputMethodArea();
        const QRect mappedPanelRect = sceneWindow->mapRectFromScene(inputPanelRect).toRect();
        newDistanceFromBottom = qMin(newDistanceFromBottom, mappedPanelRect.height());
    } else {
        newDistanceFromBottom = 0;
    }

    return QPoint(0, distanceFromBottom - newDistanceFromBottom);
}

void MSceneWindowScroller::applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset)
{
    if (!contentsOffset.isNull()) {
        MSceneWindow *sceneWindow = static_cast<MSceneWindow *>(widget);
        emit sceneWindowDislocationRequest(sceneWindow, contentsOffset);
    }
}

void MSceneWindowScroller::restoreScrolling(QGraphicsWidget *widget)
{
    MSceneWindow *sceneWindow = static_cast<MSceneWindow *>(widget);
    emit sceneWindowUndoDislocationRequest(sceneWindow);
}
