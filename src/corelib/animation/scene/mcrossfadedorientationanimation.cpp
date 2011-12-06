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
#include <QGraphicsOpacityEffect>
#include <MDeviceProfile>

#include <mwindow.h>
#include <mscenelayereffect.h>

#include <QDebug>

void MCrossFadedOrientationAnimationPrivate::init(const QRectF &visibleSceneRect)
{
    Q_Q(MCrossFadedOrientationAnimation);

    sourceSnapshot = 0;
    targetSnapshot = 0;
    backgroundLayerParentItem = 0;
    this->visibleSceneRect = visibleSceneRect;

    targetSnapshotRotationAnimation = new QPropertyAnimation(0, "rotation", q);
    targetSnapshotRotationAnimation->setDuration(q->style()->duration());
    targetSnapshotRotationAnimation->setEasingCurve(q->style()->rotationEasingCurve());

    targetSnapshotFadeInAnimation = new QPropertyAnimation(0, "opacity", q);
    // QT doesn’t try to draw QGraphicsItems with opacity 0.
    // This is an undocummented behavior (QTBUG-18267).
    // root element must always be drawn because there might be
    // a scene layer effect in there that ignores the parent's opacity, so
    // it will always be drawn.
    targetSnapshotFadeInAnimation->setStartValue(0.001);
    targetSnapshotFadeInAnimation->setEndValue(1.0);
    targetSnapshotFadeInAnimation->setDuration(q->style()->duration());
    targetSnapshotFadeInAnimation->setEasingCurve(q->style()->fadingEasingCurve());

    targetSnapshotPositionAnimation = new QPropertyAnimation(0, "pos", q);
    targetSnapshotPositionAnimation->setDuration(q->style()->duration());
    targetSnapshotPositionAnimation->setEasingCurve(q->style()->translationEasingCurve());

    sourceSnapshotRotationAnimation = new QPropertyAnimation(0, "rotation", q);
    sourceSnapshotRotationAnimation->setDuration(q->style()->duration());
    sourceSnapshotRotationAnimation->setEasingCurve(q->style()->rotationEasingCurve());

    sourceSnapshotFadeOutAnimation = new QPropertyAnimation(0, "opacity", q);
    sourceSnapshotFadeOutAnimation->setStartValue(1.0);
    sourceSnapshotFadeOutAnimation->setEndValue(0.0);
    sourceSnapshotFadeOutAnimation->setDuration(q->style()->duration());
    sourceSnapshotFadeOutAnimation->setEasingCurve(q->style()->fadingEasingCurve());

    sourceSnapshotPositionAnimation = new QPropertyAnimation(0, "pos", q);
    sourceSnapshotPositionAnimation->setDuration(q->style()->duration());
    sourceSnapshotPositionAnimation->setEasingCurve(q->style()->translationEasingCurve());

    dirtyTargetSnapshot = false;
}

void MCrossFadedOrientationAnimationPrivate::createSourceSnapshot()
{
    Q_ASSERT(sourceSnapshot == 0);

    sourceSnapshot = createSnapshot();
    sourceSnapshot->setTransformOriginPoint(sourceSnapshotRotationPoint);

    sourceSnapshotRotationAnimation->setTargetObject(sourceSnapshot);
    sourceSnapshotFadeOutAnimation->setTargetObject(sourceSnapshot);
    sourceSnapshotPositionAnimation->setTargetObject(sourceSnapshot);
}

void MCrossFadedOrientationAnimationPrivate::destroySnapshots()
{
    sourceSnapshotRotationAnimation->setTargetObject(0);
    sourceSnapshotFadeOutAnimation->setTargetObject(0);
    sourceSnapshotPositionAnimation->setTargetObject(0);

    targetSnapshotRotationAnimation->setTargetObject(0);
    targetSnapshotFadeInAnimation->setTargetObject(0);
    targetSnapshotPositionAnimation->setTargetObject(0);

    delete sourceSnapshot;
    sourceSnapshot = 0;

    delete targetSnapshot;
    targetSnapshot = 0;
}

void MCrossFadedOrientationAnimationPrivate::setSnapshotRotationAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    // Unlike the root element, the snapshot always begin from 0.0.
    // Therefore we have to translate the root element rotation angles to snapshot
    // rotation angles.

    sourceSnapshotRotationAnimation->setStartValue(0.0);

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
    sourceSnapshotRotationAnimation->setEndValue(snapshotEndAngle);
}

QPointF MCrossFadedOrientationAnimationPrivate::calculateSnapshotRotationPoint(
        M::OrientationAngle startAngle) const
{
    // The snapshot item always begin its rotation from 0 degrees, unlike the root
    // element, which begins from startAngle.
    // Since both snapshot and root element must be rotated from the same point
    // in the scene, their rotation points (transform origin points) in their respective local
    // coordinates must map to the same point in scene coordinates.
    //
    // The style option "rotation point" is in local root element coordinates.
    // Here we translate it into snapshot local coordinates.

    Q_Q(const MCrossFadedOrientationAnimation);

    // rotation point of the root element in its local coordinate system.
    QPointF rootElementRotationPoint = q->style()->rotationPoint();

    QPointF rotationPoint;

    switch (startAngle) {
        case M::Angle0:
            rotationPoint = rootElementRotationPoint;
            break;
        case M::Angle90:
            rotationPoint.rx() = visibleSceneRect.width() - rootElementRotationPoint.y();
            rotationPoint.ry() = rootElementRotationPoint.x();
            break;
        case M::Angle180:
            rotationPoint.rx() = visibleSceneRect.width() - rootElementRotationPoint.x();
            rotationPoint.ry() = visibleSceneRect.height() - rootElementRotationPoint.y();
            break;
        default: // M::Angle270:
            rotationPoint.rx() = rootElementRotationPoint.y();
            rotationPoint.ry() = visibleSceneRect.height() - rootElementRotationPoint.x();
            break;
    }

    return rotationPoint;
}

void MCrossFadedOrientationAnimationPrivate::setSnapshotPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    sourceSnapshotPositionAnimation->setStartValue(
            calculateRotationPointSceneCoords(startAngle) - sourceSnapshotRotationPoint);

    sourceSnapshotPositionAnimation->setEndValue(
            calculateRotationPointSceneCoords(endAngle) - sourceSnapshotRotationPoint);
}

void MCrossFadedOrientationAnimationPrivate::setTargetSnapshotPositionAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    Q_UNUSED(endAngle);

    targetSnapshotPositionAnimation->setEndValue(QPointF());
    targetSnapshotPositionAnimation->setStartValue(
            calculateRotationPointSceneCoords(startAngle) - targetSnapshotRotationPoint);
}

void MCrossFadedOrientationAnimationPrivate::setTargetSnapshotRotationAnimationValues(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    if (startAngle == M::Angle180 && endAngle == M::Angle270) {
        targetSnapshotRotationAnimation->setStartValue(-90);
    } else if (startAngle == M::Angle270 && endAngle == M::Angle180) {
        targetSnapshotRotationAnimation->setStartValue(90);
    } else if (startAngle == M::Angle0 && endAngle == M::Angle270) {
        targetSnapshotRotationAnimation->setStartValue(90);
    } else if (startAngle == M::Angle270 && endAngle == M::Angle0) {
        targetSnapshotRotationAnimation->setStartValue(-90);
    } else if (startAngle == M::Angle0 && endAngle == M::Angle90) {
        targetSnapshotRotationAnimation->setStartValue(-90);
    } else if (startAngle == M::Angle90 && endAngle == M::Angle0) {
        targetSnapshotRotationAnimation->setStartValue(90);
    } else if (startAngle == M::Angle90 && endAngle == M::Angle180) {
        targetSnapshotRotationAnimation->setStartValue(-90);
    } else if (startAngle == M::Angle180 && endAngle == M::Angle90) {
        targetSnapshotRotationAnimation->setStartValue(90);
    } else if (startAngle == M::Angle0 && endAngle == M::Angle180){
        targetSnapshotRotationAnimation->setStartValue(-180);
    } else if (startAngle == M::Angle180 && endAngle == M::Angle0) {
        targetSnapshotRotationAnimation->setStartValue(-180);
    } else if (startAngle == M::Angle90 && endAngle == M::Angle270){
        targetSnapshotRotationAnimation->setStartValue(-180);
    } else if (startAngle == M::Angle270 && endAngle == M::Angle90) {
        targetSnapshotRotationAnimation->setStartValue(-180);
    }

    // animated snapshot has target orientation already so end value always = 0
    targetSnapshotRotationAnimation->setEndValue(0);
}

void MCrossFadedOrientationAnimationPrivate::setSnapshotRotationPoints(
        M::OrientationAngle startAngle, M::OrientationAngle endAngle)
{
    sourceSnapshotRotationPoint = calculateSnapshotRotationPoint(startAngle);
    targetSnapshotRotationPoint = calculateSnapshotRotationPoint(endAngle);
}

QPointF MCrossFadedOrientationAnimationPrivate::calculateRotationPointSceneCoords(
        M::OrientationAngle angle)
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

void MCrossFadedOrientationAnimationPrivate::connectToGeometryChanges()
{
    Q_Q(MCrossFadedOrientationAnimation);
    Q_ASSERT(rootElementWidgets.isEmpty());
    watchGeometryChanges(q->rootElement());
}

void MCrossFadedOrientationAnimationPrivate::disconnectFromGeometryChanges()
{
    Q_Q(MCrossFadedOrientationAnimation);

    foreach (const QWeakPointer<QGraphicsWidget> &widget, rootElementWidgets) {
        if (!widget.isNull()) {
            QObject::disconnect(widget.data(), SIGNAL(geometryChanged()), q, SLOT(_q_onGeometryChanged()));
        }
    }

    rootElementWidgets.clear();
}

void MCrossFadedOrientationAnimationPrivate::watchGeometryChanges(QGraphicsWidget* widget)
{
    Q_Q(MCrossFadedOrientationAnimation);
    if (!widget) {
        return;
    }

    QObject::connect(widget, SIGNAL(geometryChanged()), q, SLOT(_q_onGeometryChanged()));
    rootElementWidgets.append(widget);

    const QList<QGraphicsItem*> items = widget->childItems();
    foreach (QGraphicsItem* item, items) {
        if (item->isWidget()) {
            watchGeometryChanges(static_cast<QGraphicsWidget*>(item));
        }
    }
}

void MCrossFadedOrientationAnimationPrivate::_q_onGeometryChanged()
{
    dirtyTargetSnapshot = true;
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

    d->destroySnapshots();
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

    d->endAngle = endAngle;

    d->setTargetSnapshotRotationAnimationValues(startAngle, endAngle);
    d->setSnapshotRotationAnimationValues(startAngle, endAngle);

    d->setSnapshotRotationPoints(startAngle, endAngle);

    d->setTargetSnapshotPositionAnimationValues(startAngle, endAngle);
    d->setSnapshotPositionAnimationValues(startAngle, endAngle);
}

void MCrossFadedOrientationAnimation::rootElementChanged()
{
    Q_D(MCrossFadedOrientationAnimation);
    // disconnect from the geometryChanged() signals of the old root element's
    // children and reconnect to new root element's children
    d->disconnectFromGeometryChanges();
    d->connectToGeometryChanges();
    d->dirtyTargetSnapshot = true;
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

        // Since we want a snapshot only from the root element, we have to
        // temporarily hide the scene background.
        QGraphicsWidget *backgroundLayerEffect = d->backgroundLayerEffectPointer.data();

        if (backgroundLayerEffect) {
            // we don't want it to show up in the snapshot
            backgroundLayerEffect->hide();
        }

        d->createSourceSnapshot();

        rotateRootElement(d->endAngle);

        d->createTargetSnapshot();

        // hide so the snapshot will be used instead
        // opacity effect is used instead of simply call hide()
        // to prevent clearing focus on currently focused item
        QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect();
        opacityEffect->setOpacity(0);
        rootElement()->setGraphicsEffect(opacityEffect);

        if (backgroundLayerEffect) {
            // bring it back, now that the snapshot has been taken
            backgroundLayerEffect->show();

            // Remove it from the root element since it's hidden.
            // Otherwise the layer effect won't show up
            d->backgroundLayerParentItem = backgroundLayerEffect->parentItem();
            backgroundLayerEffect->setParentItem(0);
        }

        // add to scene so they are rendered
        rootElement()->scene()->addItem(d->targetSnapshot);
        rootElement()->scene()->addItem(d->sourceSnapshot);
    } else if (newState == QAbstractAnimation::Stopped) {
        d->disconnectFromGeometryChanges();
        d->destroySnapshots();

        QGraphicsWidget *backgroundLayerEffect = d->backgroundLayerEffectPointer.data();
        if (backgroundLayerEffect) {
            //remove item from scene and add it to its original parent
            rootElement()->scene()->removeItem(backgroundLayerEffect);
            if (d->backgroundLayerParentItem) {
                backgroundLayerEffect->setParentItem(d->backgroundLayerParentItem);
                d->backgroundLayerParentItem = 0;
            }
        }

        rootElement()->setGraphicsEffect(0);
    }

    MParallelAnimationGroup::updateState(newState, oldState);
}

void MCrossFadedOrientationAnimation::updateCurrentTime(int currentTime)
{
    Q_D(MCrossFadedOrientationAnimation);
    MOrientationAnimation::updateCurrentTime(currentTime);

    if (d->dirtyTargetSnapshot) {
        // the layout of a widget in the root element has been changed
        // and an update of the snapshot is necessary
        d->dirtyTargetSnapshot = false;

        rootElement()->scene()->removeItem(d->sourceSnapshot);
        rootElement()->graphicsEffect()->setEnabled(false);

        QGraphicsWidget *backgroundLayerEffect = d->backgroundLayerEffectPointer.data();
        if (backgroundLayerEffect) {
            //hide effect so it doesn't appear on snapshot
            backgroundLayerEffect->hide();
        }

        d->targetSnapshot->updateSnapshot();

        if (backgroundLayerEffect) {
            // bring effect back, now that the snapshot has been updated
            backgroundLayerEffect->show();
        }

        rootElement()->graphicsEffect()->setEnabled(true);
        rootElement()->scene()->addItem(d->sourceSnapshot);
    }
}

void MCrossFadedOrientationAnimationPrivate::createTargetSnapshot()
{
    Q_ASSERT(targetSnapshot == 0);

    targetSnapshot = createSnapshot();
    targetSnapshot->setTransformOriginPoint(targetSnapshotRotationPoint);

    targetSnapshotRotationAnimation->setTargetObject(targetSnapshot);
    targetSnapshotFadeInAnimation->setTargetObject(targetSnapshot);
    targetSnapshotPositionAnimation->setTargetObject(targetSnapshot);

    connectToGeometryChanges();
}

void MCrossFadedOrientationAnimation::rotateRootElement(M::OrientationAngle orientation)
{
    const QSize nativeScreenSize = MDeviceProfile::instance()->resolution();
    const M::Orientation nativeOrientation = MDeviceProfile::instance()->orientationFromAngle(M::Angle0);

    const M::OrientationAngle newAngle = orientation;
    rootElement()->setRotation(newAngle);
    if (MDeviceProfile::instance()->orientationFromAngle(newAngle) == nativeOrientation) {
        rootElement()->setPos(0, 0);
        rootElement()->setTransformOriginPoint(nativeScreenSize.width() / 2,
                                               nativeScreenSize.height() / 2);
    } else {
        rootElement()->setPos((nativeScreenSize.width() - nativeScreenSize.height()) / 2,
                              (nativeScreenSize.height() - nativeScreenSize.width()) / 2);
        rootElement()->setTransformOriginPoint(nativeScreenSize.height() / 2,
                                               nativeScreenSize.width() / 2);
    }

    // Let the scene windows and widgets have their final sizes and positions
    // within the root element.
    emit orientationChanged();
}

MSnapshotItem* MCrossFadedOrientationAnimationPrivate::createSnapshot()
{
    Q_Q(MCrossFadedOrientationAnimation);
    QGraphicsScene *scene = q->rootElement()->scene();

    MSnapshotItem *snapshotItem = new MSnapshotItem(visibleSceneRect);

    scene->addItem(snapshotItem);
    snapshotItem->updateSnapshot();
    snapshotItem->setPos(0.0, 0.0);
    scene->removeItem(snapshotItem);

    return snapshotItem;
}

#include "moc_mcrossfadedorientationanimation.cpp"
