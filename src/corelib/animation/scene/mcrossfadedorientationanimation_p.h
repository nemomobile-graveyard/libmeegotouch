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
    void createSourceSnapshot();
    void createTargetSnapshot();

    void destroySnapshots();

    void setSnapshotRotationAnimationValues(
            M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setSnapshotPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setTargetSnapshotRotationAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setTargetSnapshotPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    void setSnapshotRotationPoints(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle);

    QPointF calculateSnapshotRotationPoint(M::OrientationAngle startAngle) const;

    // Calculates the rotation point in scene coordinates.
    QPointF calculateRotationPointSceneCoords(M::OrientationAngle angle);

    bool renderedByTranslucentWindow();
    static QGraphicsWidget *findLayerEffect(QGraphicsItem *currentItem, int currentLevel);
    void fetchBackgroundLayerEffect();

    // Connects to the geometryChanged() signal of all widgets inside the
    // root-element. The available widgets are remembered in rootElementWidgets.
    void connectToGeometryChanges();

    // Disconnects from the geometryChanged() signal of all
    // widgets of rootElementWidgets and clears rootElementWidgets.
    void disconnectFromGeometryChanges();

    // Helper method for connectToGeometryChanges() to recursively
    // connect to the geometryChanged() signal of all children.
    void watchGeometryChanges(QGraphicsWidget* widget);
    void _q_onGeometryChanged();

    MSnapshotItem *sourceSnapshot;
    MSnapshotItem *targetSnapshot;

    // visible scene rect, in scene coordinates.
    // This is the rectangle of the scene that is rendered by MWindow.
    // Equals to the bounding rectangle of the root element in scene coordinates.
    QRectF visibleSceneRect;

    QPropertyAnimation *targetSnapshotRotationAnimation;
    QPropertyAnimation *targetSnapshotFadeInAnimation;
    QPropertyAnimation *targetSnapshotPositionAnimation;

    QPropertyAnimation *sourceSnapshotRotationAnimation;
    QPropertyAnimation *sourceSnapshotFadeOutAnimation;
    QPropertyAnimation *sourceSnapshotPositionAnimation;

    // The rotation point of the snapshot item, in its local coordinates.
    QPointF sourceSnapshotRotationPoint;
    QPointF targetSnapshotRotationPoint;

    QWeakPointer<QGraphicsWidget> backgroundLayerEffectPointer;
    QGraphicsItem *backgroundLayerParentItem;

    M::OrientationAngle endAngle;

    bool dirtyTargetSnapshot;
    QList<QWeakPointer<QGraphicsWidget> > rootElementWidgets;

private:
    MSnapshotItem* createSnapshot();
};

#endif
