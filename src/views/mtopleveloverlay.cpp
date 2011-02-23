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
#include "mtopleveloverlay.h"

#include <MSceneManager>
#include <QPanGesture>

namespace {
    //! Top level overlay should be on top of every widget, even status bar.
    //! Therefore we set the z value of this top level widget to 1.0 which
    //! is more than default Z of root element in MScene.
    const qreal MTopLevelOverlayZValue = 1.0f;
}

MTopLevelOverlay::MTopLevelOverlay(const MSceneManager *sceneManager)
    : sceneManager(sceneManager)
{
    setFlag(QGraphicsItem::ItemHasNoContents, true);
    setZValue(MTopLevelOverlayZValue);

    rotateAndResizeToFullscreen(sceneManager->orientationAngle());
    QObject::connect(sceneManager, SIGNAL(orientationAngleChanged(M::OrientationAngle)),
                     this, SLOT(rotateAndResizeToFullscreen(M::OrientationAngle)));
}

bool MTopLevelOverlay::isAppeared() const
{
    return isVisible();
}

void MTopLevelOverlay::panGestureEvent(QGestureEvent *event, QPanGesture *panGesture)
{
    // Accept gesture if magnifier is appeared. This is done to prevent panning of
    // application page but will of course prevent other uses of pan gestures as well.
    if (panGesture->state() == Qt::GestureStarted
        && isAppeared()) {
        event->accept(panGesture);
    } else {
        event->ignore(panGesture);
    }
}

void MTopLevelOverlay::rotateAndResizeToFullscreen(M::OrientationAngle orientationAngle)
{
    // Set geometry to new fullscreen and center it for rotation.
    // Need to occupy whole screen to be able to catch panning gestures.
    const QRectF landscapeRect(QPointF(), sceneManager->visibleSceneSize(M::Landscape));
    QRectF newRect(QPointF(), sceneManager->visibleSceneSize());
    newRect.moveTopLeft(landscapeRect.center() - newRect.center());
    setGeometry(newRect);

    // Rotate with pivot at center.
    const qreal angle = static_cast<qreal>(orientationAngle);
    setTransformOriginPoint(rect().center()); // Origin point is given in local coordinates.
    setRotation(angle);
}
