#ifndef MCROSSFADEDORIENTATIONANIMATION_P_H
#define MCROSSFADEDORIENTATIONANIMATION_P_H

#include "morientationanimation_p.h"

#include "msnapshotitem.h"

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
};

#endif
