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
#ifndef MCROSSFADEDORIENTATIONANIMATION_P_H
#define MCROSSFADEDORIENTATIONANIMATION_P_H

#include "morientationanimation_p.h"

#include "msnapshotitem.h"

#include <QWeakPointer>

class QPropertyAnimation;

class MCrossFadedOrientationAnimationPrivate : public MOrientationAnimationPrivate
{
    Q_DECLARE_PUBLIC(MCrossFadedOrientationAnimation)

public:
    void init(const QRectF &visibleSceneRect);
    void createRootElementSnapshot();
    void destroyRootElementSnapshot();

    void setSnapshotRotationAnimationValues(
            M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setSnapshotPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setRootElementRotationAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setRootElementPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void calculateSnapshotRotationPoint(M::OrientationAngle startAngle);

    // Calculates the rotation point in scene coordinates.
    QPointF calculateRotationPointSceneCoords(M::OrientationAngle angle);

    bool renderedByTranslucentWindow();
    static QGraphicsWidget *findLayerEffect(QGraphicsItem *currentItem, int currentLevel);
    void fetchBackgroundLayerEffect();

    MSnapshotItem *snapshot;

    // visible scene rect, in scene coordinates.
    // This is the rectangle of the scene that is rendered by MWindow.
    // Equals to the bounding rectangle of the root element in scene coordinates.
    QRectF visibleSceneRect;

    QPropertyAnimation *rootElementRotationAnimation;
    QPropertyAnimation *rootElementFadeInAnimation;
    QPropertyAnimation *rootElementPositionAnimation;

    QPropertyAnimation *snapshotRotationAnimation;
    QPropertyAnimation *snapshotFadeOutAnimation;
    QPropertyAnimation *snapshotPositionAnimation;

    // The rotation point of the snapshot item, in its local coordinates.
    QPointF snapshotRotationPoint;

    QWeakPointer<QGraphicsWidget> backgroundLayerEffectPointer;
};

#endif
