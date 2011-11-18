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

#include "mcrossfadedorientationanimation.h"
#include "mcrossfadedorientationanimation_p.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QPropertyAnimation>

#include <mwindow.h>
#include <mscenelayereffect.h>

#include <QDebug>

void MCrossFadedOrientationAnimationPrivate::init(const QRectF &visibleSceneRect)
{
    Q_Q(MCrossFadedOrientationAnimation);

    snapshot = 0;
    this->visibleSceneRect = visibleSceneRect;

    rootElementRotationAnimation = new QPropertyAnimation(0, "rotation", q);
    rootElementRotationAnimation->setDuration(q->style()->duration());
    rootElementRotationAnimation->setEasingCurve(q->style()->rotationEasingCurve());

    rootElementFadeInAnimation = new QPropertyAnimation(0, "opacity", q);
    // QT doesn’t try to draw QGraphicsItems with opacity 0.
    // This is an undocummented behavior (QTBUG-18267).
    // root element must always be drawn because there might be
    // a scene layer effect in there that ignores the parent's opacity, so
    // it will always be drawn.
    rootElementFadeInAnimation->setStartValue(0.001);
    rootElementFadeInAnimation->setEndValue(1.0);
    rootElementFadeInAnimation->setDuration(q->style()->duration());
    rootElementFadeInAnimation->setEasingCurve(q->style()->fadingEasingCurve());

    rootElementPositionAnimation = new QPropertyAnimation(0, "pos", q);
    rootElementPositionAnimation->setDuration(q->style()->duration());
    rootElementPositionAnimation->setEasingCurve(q->style()->translationEasingCurve());

    snapshotRotationAnimation = new QPropertyAnimation(0, "rotation", q);
    snapshotRotationAnimation->setDuration(q->style()->duration());
    snapshotRotationAnimation->setEasingCurve(q->style()->rotationEasingCurve());

    snapshotFadeOutAnimation = new QPropertyAnimation(0, "opacity", q);
    snapshotFadeOutAnimation->setStartValue(1.0);
    snapshotFadeOutAnimation->setEndValue(0.0);
    snapshotFadeOutAnimation->setDuration(q->style()->duration());
    snapshotFadeOutAnimation->setEasingCurve(q->style()->fadingEasingCurve());

    snapshotPositionAnimation = new QPropertyAnimation(0, "pos", q);
    snapshotPositionAnimation->setDuration(q->style()->duration());
    snapshotPositionAnimation->setEasingCurve(q->style()->translationEasingCurve());

}

void MCrossFadedOrientationAnimationPrivate::createRootElementSnapshot()
{
    Q_Q(MCrossFadedOrientationAnimation);
    QGraphicsScene *scene = q->rootElement()->scene();

    Q_ASSERT(snapshot == 0);

    // Since we want a snapshot only from the root element, we have to
    // temporarily hide the scene background.

    // hide scene background
    snapshot = new MSnapshotItem(visibleSceneRect);
    // show scene background

    scene->addItem(snapshot);
    snapshot->updateSnapshot();
    snapshot->setPos(0.0, 0.0);
    snapshot->setTransformOriginPoint(snapshotRotationPoint);

    snapshotRotationAnimation->setTargetObject(snapshot);
    snapshotFadeOutAnimation->setTargetObject(snapshot);
    snapshotPositionAnimation->setTargetObject(snapshot);
}

void MCrossFadedOrientationAnimationPrivate::destroyRootElementSnapshot()
{
    snapshotRotationAnimation->setTargetObject(0);
    snapshotFadeOutAnimation->setTargetObject(0);
    snapshotPositionAnimation->setTargetObject(0);

    if (snapshot) {
        delete snapshot;
        snapshot = 0;
    }
}

void MCrossFadedOrientationAnimationPrivate::setSnapshotRotationAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    // Unlike the root element, the snapshot always begin from 0.0.
    // Therefore we have to translate the root element rotation angles to snapshot
    // rotation angles.

    snapshotRotationAnimation->setStartValue(0.0);

    qreal snapshotEndAngle;
    if (startAngle == M::Angle0) {
        if (endAngle == M::Angle270)
            snapshotEndAngle = -90.0;
        else
            snapshotEndAngle = endAngle;

    } else if (startAngle == M::Angle90) {
        snapshotEndAngle = endAngle - startAngle;

    } else if (startAngle == M::Angle180) {
        if (endAngle == M::Angle0)
            snapshotEndAngle = 180.0;
        else
            snapshotEndAngle = endAngle - startAngle;
    } else {
        // startAngle == M::Angle270

        if (endAngle == M::Angle0) {
            snapshotEndAngle = 90.0;
        } else if (endAngle == M::Angle90) {
            snapshotEndAngle = 180.0;
        } else if (endAngle == M::Angle180) {
            snapshotEndAngle = -90.0;
        } else {
            // M::Angle270
            snapshotEndAngle = 0.0;
        }
    }
    snapshotRotationAnimation->setEndValue(snapshotEndAngle);
}

void MCrossFadedOrientationAnimationPrivate::calculateSnapshotRotationPoint(
        M::OrientationAngle startAngle)
{
    // The snapshot item always begin its rotation from 0 degrees, unlike the root
    // element, which begins from startAngle.
    // Since both snapshot and root element must be rotated from the same point
    // in the scene, their rotation points (transform origin points) in their respective local
    // coordinates must map to the same point in scene coordinates.
    //
    // The style option "rotation point" is in local root element coordinates.
    // Here we translate it into snapshot local coordinates.

    Q_Q(MCrossFadedOrientationAnimation);

    // rotation point of the root element in its local coordinate system.
    QPointF rootElementRotationPoint = q->style()->rotationPoint();

    switch (startAngle) {
        case M::Angle0:
            snapshotRotationPoint = rootElementRotationPoint;
            break;
        case M::Angle90:
            snapshotRotationPoint.rx() = visibleSceneRect.width() - rootElementRotationPoint.y();
            snapshotRotationPoint.ry() = rootElementRotationPoint.x();
            break;
        case M::Angle180:
            snapshotRotationPoint.rx() = visibleSceneRect.width() - rootElementRotationPoint.x();
            snapshotRotationPoint.ry() = visibleSceneRect.height() - rootElementRotationPoint.y();
            break;
        default: // M::Angle270:
            snapshotRotationPoint.rx() = rootElementRotationPoint.y();
            snapshotRotationPoint.ry() = visibleSceneRect.height() - rootElementRotationPoint.x();
            break;
    }
}

void MCrossFadedOrientationAnimationPrivate::setSnapshotPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    snapshotPositionAnimation->setStartValue(
            calculateRotationPointSceneCoords(startAngle) - snapshotRotationPoint);

    snapshotPositionAnimation->setEndValue(
            calculateRotationPointSceneCoords(endAngle) - snapshotRotationPoint);
}

void MCrossFadedOrientationAnimationPrivate::setRootElementPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    Q_Q(MCrossFadedOrientationAnimation);

    // rotation point in local item coordinates.
    QPointF rotationPointLocal = q->style()->rotationPoint();

    rootElementPositionAnimation->setStartValue(
            calculateRotationPointSceneCoords(startAngle) - rotationPointLocal);

    rootElementPositionAnimation->setEndValue(
            calculateRotationPointSceneCoords(endAngle) - rotationPointLocal);
}

void MCrossFadedOrientationAnimationPrivate::setRootElementRotationAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    if (startAngle == M::Angle270 && endAngle == M::Angle90) {
        // 180 degrees rotation. Do it clockwise like the snapshot item.
        rootElementRotationAnimation->setStartValue(-90);
        rootElementRotationAnimation->setEndValue(endAngle);

    } else if (startAngle == M::Angle180 && endAngle == M::Angle0) {
        // 180 degrees rotation. Do it clockwise like the snapshot item.
        rootElementRotationAnimation->setStartValue(-180);
        rootElementRotationAnimation->setEndValue(endAngle);
    } else if (startAngle == M::Angle270 && endAngle == M::Angle0) {
        // Do it clockwise, which is the shortest rotation.
        rootElementRotationAnimation->setStartValue(-90);
        rootElementRotationAnimation->setEndValue(endAngle);
    } else if (startAngle == M::Angle0 && endAngle == M::Angle270) {
        // Do it counterclockwise, which is the shortest rotation.
        rootElementRotationAnimation->setStartValue(startAngle);
        rootElementRotationAnimation->setEndValue(-90);
    } else {
        // Easy cases. No tweaks needed.
        rootElementRotationAnimation->setStartValue(startAngle);
        rootElementRotationAnimation->setEndValue(endAngle);
    }
}

QPointF MCrossFadedOrientationAnimationPrivate::calculateRotationPointSceneCoords(M::OrientationAngle angle)
{
    Q_Q(MCrossFadedOrientationAnimation);

    // rotation point in local item coordinates
    QPointF rotationPointLocal = q->style()->rotationPoint();

    // rotation point in scene coordinates
    QPointF rotationPointScene;

    rotationPointScene.rx() = visibleSceneRect.x();
    rotationPointScene.ry() = visibleSceneRect.y();

    switch (angle) {
        case M::Angle0:
            rotationPointScene.rx() += rotationPointLocal.x();
            rotationPointScene.ry() += rotationPointLocal.y();
            break;
        case M::Angle90:
            rotationPointScene.rx() += visibleSceneRect.width() - rotationPointLocal.y();
            rotationPointScene.ry() += rotationPointLocal.x();
            break;
        case M::Angle180:
            rotationPointScene.rx() += visibleSceneRect.width() - rotationPointLocal.x();
            rotationPointScene.ry() += visibleSceneRect.height() - rotationPointLocal.y();
            break;
        default: // M::Angle270:
            rotationPointScene.rx() += rotationPointLocal.y();
            rotationPointScene.ry() += visibleSceneRect.height() - rotationPointLocal.x();
            break;
    }

    return rotationPointScene;
}

bool MCrossFadedOrientationAnimationPrivate::renderedByTranslucentWindow()
{
    Q_Q(MCrossFadedOrientationAnimation);

    QGraphicsScene *scene = q->rootElement()->scene();
    if (!scene || scene->views().count() == 0)
        return false;

    MWindow *window = qobject_cast<MWindow*>(scene->views().at(0));

    return window && window->testAttribute(Qt::WA_TranslucentBackground);
}

QGraphicsWidget *MCrossFadedOrientationAnimationPrivate::findLayerEffect(
        QGraphicsItem *currentItem, int currentLevel)
{
    QGraphicsWidget *layerEffect = 0;

    if (currentItem->isWidget()) {
        layerEffect =
            qobject_cast<MSceneLayerEffect*>(
                    static_cast<QGraphicsWidget*>(currentItem));

        if (layerEffect)
            return layerEffect;
    }

    // We don't go deeper than 3 levels when searching for a layer effect
    // Maximum depth case would be:
    //
    //      1                         2                           3
    // subRootElement -> sceneWindowAndLayerEffectBinder -> sceneLayerEffect
    if (currentLevel == 3) {
        return 0;
    }

    int nextLevel = currentLevel + 1;
    foreach (QGraphicsItem* child, currentItem->childItems()) {
        layerEffect = findLayerEffect(child, nextLevel);
        if (layerEffect)
            break;
    }

    return layerEffect;
}

void MCrossFadedOrientationAnimationPrivate::fetchBackgroundLayerEffect()
{
    Q_Q(MCrossFadedOrientationAnimation);

    backgroundLayerEffectPointer.clear();

    QGraphicsWidget *layerEffect = findLayerEffect(q->rootElement(), 0);
    if (!layerEffect)
        return;

    backgroundLayerEffectPointer = layerEffect;
}

MCrossFadedOrientationAnimation::MCrossFadedOrientationAnimation(
        const QRectF &visibleSceneRect, QObject *parent) :
    MOrientationAnimation(new MCrossFadedOrientationAnimationPrivate, parent)
{
    Q_D(MCrossFadedOrientationAnimation);
    d->init(visibleSceneRect);
}

MCrossFadedOrientationAnimation::~MCrossFadedOrientationAnimation()
{
    Q_D(MCrossFadedOrientationAnimation);

    d->destroyRootElementSnapshot();
}

void MCrossFadedOrientationAnimation::addSceneWindow(MSceneWindow *sceneWindow)
{
    Q_UNUSED(sceneWindow);
}

void MCrossFadedOrientationAnimation::removeSceneWindow(MSceneWindow *sceneWindow)
{
    Q_UNUSED(sceneWindow);
}

void MCrossFadedOrientationAnimation::setTargetRotationAngle(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    Q_D(MCrossFadedOrientationAnimation);

    d->setRootElementRotationAnimationValues(startAngle, endAngle);
    d->setRootElementPositionAnimationValues(startAngle, endAngle);
    d->setSnapshotRotationAnimationValues(startAngle, endAngle);

    d->calculateSnapshotRotationPoint(startAngle);
    d->setSnapshotPositionAnimationValues(startAngle, endAngle);
}

void MCrossFadedOrientationAnimation::rootElementChanged()
{
    Q_D(MCrossFadedOrientationAnimation);
    d->rootElementRotationAnimation->setTargetObject(rootElement());
    d->rootElementFadeInAnimation->setTargetObject(rootElement());
    d->rootElementPositionAnimation->setTargetObject(rootElement());
}

void MCrossFadedOrientationAnimation::updateState(
        QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MCrossFadedOrientationAnimation);

    if (newState == QAbstractAnimation::Running) {
        if (d->renderedByTranslucentWindow()) {
            // for cases like status menu and system dialogs (i.e. translucent MWindows
            // with a dimmed background) we want the layer effect to stand still
            // (position, rotation and opacity) during the entire rotation animation.
            // That will improve the visual quality of the animation.
            d->fetchBackgroundLayerEffect();
        }
        QGraphicsWidget *backgroundLayerEffect = d->backgroundLayerEffectPointer.data();

        if (backgroundLayerEffect) {
            // we don't want it to show up in the snapshot
            backgroundLayerEffect->hide();

            // don't let the animation affect his opacity.
            backgroundLayerEffect->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
        }

        d->createRootElementSnapshot();

        if (backgroundLayerEffect) {
            // bring it back, now that the snapshot has been taken
            backgroundLayerEffect->show();
        }

        // Let the scene windows and widgets have their final sizes and positions
        // within the root element.
        emit orientationChanged();

        rootElement()->setTransformOriginPoint(style()->rotationPoint());

    } else if (newState == QAbstractAnimation::Stopped) {
        d->destroyRootElementSnapshot();

        QGraphicsWidget *backgroundLayerEffect = d->backgroundLayerEffectPointer.data();
        if (backgroundLayerEffect) {
            // bring the flag back to its default value
            backgroundLayerEffect->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, false);
        }
    }

    MParallelAnimationGroup::updateState(newState, oldState);
}
