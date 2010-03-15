/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duibasicorientationanimation.h"

#include "duibasicorientationanimation_p.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>

#include <QGraphicsWidget>
#include "duiscenewindow.h"
#include "duiapplicationpage.h"
#include <duipannableviewport.h>

void DuiBasicOrientationAnimationPrivate::_q_onPhase0Finished()
{
    Q_Q(DuiBasicOrientationAnimation);

    upcomingPhase = phase1;
    hideComponents();

    emit q->orientationChanged();
}

void DuiBasicOrientationAnimationPrivate::_q_onPhase1Finished()
{
    upcomingPhase = phase2;
    showComponents();
}

void DuiBasicOrientationAnimationPrivate::_q_onPhase2Finished()
{
    upcomingPhase = phase0;
}

void DuiBasicOrientationAnimationPrivate::hideComponents()
{
    if (navigationBar) {
        navigationBar->hide();
    }

    if (homeButtonPanel) {
        homeButtonPanel->hide();
    }

    if (escapeButtonPanel) {
        escapeButtonPanel->hide();
    }

    if (dockWidget) {
        dockWidget->hide();
    }
}

void DuiBasicOrientationAnimationPrivate::showComponents()
{
    if (navigationBar) {
        navigationBar->show();
    }

    if (homeButtonPanel) {
        homeButtonPanel->show();
    }

    if (escapeButtonPanel) {
        escapeButtonPanel->show();
    }

    // Show only if we're going to portrait
    if (dockWidget &&
            (endOrientationAngle == Dui::Angle90 || endOrientationAngle == Dui::Angle270)) {
        dockWidget->show();
    }
}

void DuiBasicOrientationAnimationPrivate::setupNavigationBarAnimations()
{
    navigationBarSlideOutAnimation->setStartValue(navigationBar->y());
    navigationBarSlideOutAnimation->setEndValue(-navigationBar->boundingRect().height());
    navigationBarSlideOutAnimation->setTargetObject(navigationBar);

    navigationBarSlideInAnimation->setTargetObject(navigationBar);
    navigationBarSlideInAnimation->setStartValue(navigationBarSlideOutAnimation->endValue());
    navigationBarSlideInAnimation->setEndValue(navigationBarSlideOutAnimation->startValue());
}

void DuiBasicOrientationAnimationPrivate::setupHomeButtonAnimations()
{
    setupNavigationButtonAnimations(homeButtonPanel,
                                    homeButtonSlideInAnimation, homeButtonSlideOutAnimation);
}

void DuiBasicOrientationAnimationPrivate::setupEscapeButtonAnimations()
{
    setupNavigationButtonAnimations(escapeButtonPanel,
                                    escapeButtonSlideInAnimation, escapeButtonSlideOutAnimation);
}

void DuiBasicOrientationAnimationPrivate::setupNavigationButtonAnimations(DuiSceneWindow *button,
        QPropertyAnimation *slideInAnimation, QPropertyAnimation *slideOutAnimation)
{
    slideOutAnimation->setStartValue(button->y());

    float slideOutEndY;

    if (navigationBar) {
        float navBarStartY = navigationBarSlideOutAnimation->startValue().toPointF().y();
        float navBarHeight = navigationBar->boundingRect().height();
        slideOutEndY = -navBarHeight + (button->y() - navBarStartY);
    } else {
        slideOutEndY = -button->boundingRect().height();
    }

    slideOutAnimation->setEndValue(slideOutEndY);
    slideOutAnimation->setTargetObject(button);

    slideInAnimation->setTargetObject(button);
    slideInAnimation->setStartValue(slideOutAnimation->endValue());
    slideInAnimation->setEndValue(slideOutAnimation->startValue());
}

void DuiBasicOrientationAnimationPrivate::setupDockWidgetAnimations()
{

    if (startOrientationAngle == Dui::Angle0 || startOrientationAngle == Dui::Angle180) {
        // landscape
        dockWidgetSlideOutAnimation->setTargetObject(0);
    } else {
        // portrait
        float initialScreenHeight = landscapeScreenSize.width();
        dockWidgetSlideOutAnimation->setStartValue(dockWidget->y());
        dockWidgetSlideOutAnimation->setEndValue(initialScreenHeight);
        dockWidgetSlideOutAnimation->setTargetObject(dockWidget);
    }

    if (endOrientationAngle == Dui::Angle0 || endOrientationAngle == Dui::Angle180) {
        // landscape
        dockWidgetSlideInAnimation->setTargetObject(0);
    } else {
        // portrait
        float finalScreenHeight = landscapeScreenSize.width();
        dockWidgetSlideInAnimation->setTargetObject(dockWidget);
        dockWidgetSlideInAnimation->setStartValue(finalScreenHeight);
        dockWidgetSlideInAnimation->setEndValue(finalScreenHeight - dockWidget->boundingRect().height());
    }

}

void DuiBasicOrientationAnimationPrivate::addGenericSceneWindowAnimations(DuiSceneWindow *sceneWindow)
{
    Q_Q(DuiBasicOrientationAnimation);
    SceneWindowAnimationPair animationPair;

    animationPair.fadeOutAnimation = new QPropertyAnimation(sceneWindow, "opacity", phase0);
    animationPair.fadeOutAnimation->setStartValue(sceneWindow->opacity());
    animationPair.fadeOutAnimation->setEndValue(0.0);
    animationPair.fadeOutAnimation->setDuration(q->style()->phaseZeroDuration());

    animationPair.fadeInAnimation = new QPropertyAnimation(sceneWindow, "opacity", phase2);
    animationPair.fadeInAnimation->setStartValue(0.0);
    animationPair.fadeInAnimation->setDuration(q->style()->phaseTwoDuration());
    animationPair.fadeInAnimation->setEndValue(animationPair.fadeOutAnimation->startValue());

    genericAnimationsHash[sceneWindow] = animationPair;
}

void DuiBasicOrientationAnimationPrivate::removeGenericSceneWindowAnimations(DuiSceneWindow *sceneWindow)
{
    if (!genericAnimationsHash.contains(sceneWindow)) {
        return;
    }

    SceneWindowAnimationPair animationPair = genericAnimationsHash.take(sceneWindow);

    phase0->removeAnimation(animationPair.fadeOutAnimation);
    delete animationPair.fadeOutAnimation;

    phase2->removeAnimation(animationPair.fadeInAnimation);
    delete animationPair.fadeInAnimation;
}

void DuiBasicOrientationAnimationPrivate::addApplicationPageAnimations(DuiSceneWindow *applicationPage)
{
    Q_Q(DuiBasicOrientationAnimation);
    SceneWindowAnimationPair animationPair;
    QList<QGraphicsItem *> childItemsList = applicationPage->childItems();
    int childCount = childItemsList.count();
    int i = 0;
    DuiPannableViewport *pageViewport = 0;
    QGraphicsItem *childItem;
    QGraphicsWidget *childWidget;
    QGraphicsWidget *targetObject;

    // Try to find the page's pannable viewport
    while (pageViewport == 0 && i < childCount) {
        childItem = childItemsList.at(i);

        if (childItem->isWidget()) {
            childWidget = static_cast<QGraphicsWidget *>(childItem);
            pageViewport = qobject_cast<DuiPannableViewport *>(childWidget);
        }

        ++i;
    }

    if (pageViewport) {
        targetObject = pageViewport;
    } else {
        // We will have to fade the central widget only. The drawback is that the position
        // indicator of the pannable viewport will not get faded in & out, which is visually
        // less appealing. :-)
        targetObject = (static_cast<DuiApplicationPage *>(applicationPage))->centralWidget();
    }

    animationPair.fadeOutAnimation = new QPropertyAnimation(targetObject, "opacity", phase0);
    animationPair.fadeOutAnimation->setStartValue(targetObject->opacity());
    animationPair.fadeOutAnimation->setEndValue(0.0);
    animationPair.fadeOutAnimation->setDuration(q->style()->phaseZeroDuration());

    animationPair.fadeInAnimation = new QPropertyAnimation(targetObject, "opacity", phase2);
    animationPair.fadeInAnimation->setStartValue(0.0);
    animationPair.fadeInAnimation->setDuration(q->style()->phaseTwoDuration());
    animationPair.fadeInAnimation->setEndValue(animationPair.fadeOutAnimation->startValue());

    genericAnimationsHash[applicationPage] = animationPair;
}

void DuiBasicOrientationAnimationPrivate::removeApplicationPageAnimations(DuiSceneWindow *applicationPage)
{
    removeGenericSceneWindowAnimations(applicationPage);
}

DuiSceneWindow *DuiBasicOrientationAnimationPrivate::fetchLayerEffect(DuiSceneWindow *sceneWindow)
{
    DuiSceneWindow *layerEffect = 0;

    if (sceneWindow->parentItem() && sceneWindow->parentItem()->isWidget()) {
        QGraphicsWidget *parentWidget = static_cast<QGraphicsWidget *>(sceneWindow->parentItem());

        DuiSceneWindow *parentSceneWindow = qobject_cast<DuiSceneWindow *>(parentWidget);
        if (parentSceneWindow && parentSceneWindow->windowType() == DuiSceneWindow::LayerEffect) {
            layerEffect = parentSceneWindow;
        }
    }

    return layerEffect;
}

DuiBasicOrientationAnimation::DuiBasicOrientationAnimation(const QSize &landscapeScreenSize, QObject *parent) :
    DuiParallelAnimationGroup(new DuiBasicOrientationAnimationPrivate, parent)
{
    Q_D(DuiBasicOrientationAnimation);

    d->rootElement = NULL;
    d->sequentialPhasesAnimation = new QSequentialAnimationGroup;
    addAnimation(d->sequentialPhasesAnimation);

    d->landscapeScreenSize = landscapeScreenSize;

    d->phase0 = new QParallelAnimationGroup(d->sequentialPhasesAnimation);
    d->phase1 = new QParallelAnimationGroup(d->sequentialPhasesAnimation);
    d->phase2 = new QParallelAnimationGroup(d->sequentialPhasesAnimation);
    d->upcomingPhase = d->phase0;

    d->navigationBarSlideOutAnimation = new QPropertyAnimation(NULL, "y", d->phase0);
    d->navigationBarSlideOutAnimation->setDuration(style()->phaseZeroDuration());

    d->homeButtonSlideOutAnimation = new QPropertyAnimation(NULL, "y", d->phase0);
    d->homeButtonSlideOutAnimation->setDuration(style()->phaseZeroDuration());

    d->escapeButtonSlideOutAnimation = new QPropertyAnimation(NULL, "y", d->phase0);
    d->escapeButtonSlideOutAnimation->setDuration(style()->phaseZeroDuration());

    d->dockWidgetSlideOutAnimation = new QPropertyAnimation(NULL, "y", d->phase0);
    d->dockWidgetSlideOutAnimation->setDuration(style()->phaseZeroDuration());

    d->rotationAnimation = new QPropertyAnimation(NULL, "rotation", d->phase1);
    d->rotationAnimation->setDuration(style()->phaseOneDuration());
    d->positionAnimation = new QPropertyAnimation(NULL, "pos", d->phase1);
    d->positionAnimation->setDuration(style()->phaseOneDuration());
    d->originAnimation = new QPropertyAnimation(NULL, "transformOriginPoint", d->phase1);
    d->originAnimation->setDuration(style()->phaseOneDuration());

    d->navigationBarSlideInAnimation = new QPropertyAnimation(NULL, "y", d->phase2);
    d->navigationBarSlideInAnimation->setDuration(style()->phaseTwoDuration());

    d->homeButtonSlideInAnimation = new QPropertyAnimation(NULL, "y", d->phase2);
    d->homeButtonSlideInAnimation->setDuration(style()->phaseTwoDuration());

    d->escapeButtonSlideInAnimation = new QPropertyAnimation(NULL, "y", d->phase2);
    d->escapeButtonSlideInAnimation->setDuration(style()->phaseTwoDuration());

    d->dockWidgetSlideInAnimation = new QPropertyAnimation(NULL, "y", d->phase2);
    d->dockWidgetSlideInAnimation->setDuration(style()->phaseTwoDuration());

    d->navigationBar = 0;
    d->homeButtonPanel = 0;
    d->escapeButtonPanel = 0;
    d->dockWidget = 0;

    // would be better to be a state machine instead of just an animation group...
    connect(d->phase0, SIGNAL(finished()), SLOT(_q_onPhase0Finished()));
    connect(d->phase1, SIGNAL(finished()), SLOT(_q_onPhase1Finished()));
    connect(d->phase2, SIGNAL(finished()), SLOT(_q_onPhase2Finished()));
}

DuiBasicOrientationAnimation::~DuiBasicOrientationAnimation()
{
}

void DuiBasicOrientationAnimation::addSceneWindow(DuiSceneWindow *window)
{
    Q_D(DuiBasicOrientationAnimation);

    if (window->windowType() == DuiSceneWindow::NavigationBar) {
        d->navigationBar = window;

    } else if (window->windowType() == DuiSceneWindow::ApplicationPage) {

        d->addApplicationPageAnimations(window);

    } else if (window->windowType() == DuiSceneWindow::HomeButtonPanel) {

        d->homeButtonPanel = window;

    } else if (window->windowType() == DuiSceneWindow::EscapeButtonPanel) {

        d->escapeButtonPanel = window;

    } else if (window->windowType() == DuiSceneWindow::DockWidget) {

        d->dockWidget = window;
        d->setupDockWidgetAnimations();
        if (d->upcomingPhase == d->phase1) {
            // We're about to start or have started already
            // phase 1. During that phase the dock widget must
            // be hidden.
            d->dockWidget->hide();
        }
    } else {
        DuiSceneWindow *layerEffect = d->fetchLayerEffect(window);
        if (layerEffect) {
            // FIXME
            // see also: DuiBasicOrientationAnimation::removeSceneWindow
            // The change made below is workaround for nb#159308
            // and should be reverted when solution is found.
            // d->addGenericSceneWindowAnimations(layerEffect);
            d->addGenericSceneWindowAnimations(window);
        } else {
            d->addGenericSceneWindowAnimations(window);
        }
    }
}

void DuiBasicOrientationAnimation::removeSceneWindow(DuiSceneWindow *window)
{
    Q_D(DuiBasicOrientationAnimation);

    if (window->windowType() == DuiSceneWindow::NavigationBar) {

        d->navigationBar = 0;
        d->navigationBarSlideInAnimation->setTargetObject(0);
        d->navigationBarSlideOutAnimation->setTargetObject(0);

    } else if (window->windowType() == DuiSceneWindow::HomeButtonPanel) {

        d->homeButtonPanel = 0;
        d->homeButtonSlideInAnimation->setTargetObject(0);
        d->homeButtonSlideOutAnimation->setTargetObject(0);

    } else if (window->windowType() == DuiSceneWindow::EscapeButtonPanel) {

        d->escapeButtonPanel = 0;
        d->escapeButtonSlideInAnimation->setTargetObject(0);
        d->escapeButtonSlideOutAnimation->setTargetObject(0);

    } else if (window->windowType() == DuiSceneWindow::DockWidget) {
        d->dockWidget = 0;
        d->dockWidgetSlideInAnimation->setTargetObject(0);
        d->dockWidgetSlideOutAnimation->setTargetObject(0);

    } else if (window->windowType() == DuiSceneWindow::ApplicationPage) {

        d->removeApplicationPageAnimations(window);

    } else {
        DuiSceneWindow *layerEffect = d->fetchLayerEffect(window);
        if (layerEffect) {
            // FIXME
            // see also: DuiBasicOrientationAnimation::addSceneWindow
            // The change made below is workaround for nb#159308
            // and should be reverted when solution is found.
            // d->removeGenericSceneWindowAnimations(layerEffect);
            d->removeGenericSceneWindowAnimations(window);
        } else {
            d->removeGenericSceneWindowAnimations(window);
        }
    }

}

void DuiBasicOrientationAnimation::setTargetRotationAngle(Dui::OrientationAngle start,
        Dui::OrientationAngle end)
{
    Q_D(DuiBasicOrientationAnimation);

    d->startOrientationAngle = start;
    d->endOrientationAngle = end;

    if (start == Dui::Angle270 && end == Dui::Angle0) {
        d->rotationAnimation->setStartValue(-90);
        d->rotationAnimation->setEndValue(0);
    } else if (start == Dui::Angle0 && end == Dui::Angle270) {
        d->rotationAnimation->setStartValue(360);
        d->rotationAnimation->setEndValue(270);
    } else {
        d->rotationAnimation->setStartValue(start);
        d->rotationAnimation->setEndValue(end);
    }

    if (start == Dui::Angle0 || start == Dui::Angle180) {
        d->positionAnimation->setStartValue(QPointF(0, 0));
        d->originAnimation->setStartValue(QPointF(d->landscapeScreenSize.width() / 2, d->landscapeScreenSize.height() / 2));
    } else {
        d->positionAnimation->setStartValue(QPointF((d->landscapeScreenSize.width() - d->landscapeScreenSize.height()) / 2,
                                            (d->landscapeScreenSize.height() - d->landscapeScreenSize.width()) / 2));
        d->originAnimation->setStartValue(QPointF(d->landscapeScreenSize.height() / 2, d->landscapeScreenSize.width() / 2));
    }

    if (end == Dui::Angle0 || end == Dui::Angle180) {
        d->positionAnimation->setEndValue(QPointF(0, 0));
        d->originAnimation->setEndValue(QPointF(d->landscapeScreenSize.width() / 2, d->landscapeScreenSize.height() / 2));
    } else {
        d->positionAnimation->setEndValue(QPointF((d->landscapeScreenSize.width() - d->landscapeScreenSize.height()) / 2,
                                          (d->landscapeScreenSize.height() - d->landscapeScreenSize.width()) / 2));
        d->originAnimation->setEndValue(QPointF(d->landscapeScreenSize.height() / 2, d->landscapeScreenSize.width() / 2));
    }

    if (d->dockWidget) {
        d->setupDockWidgetAnimations();
    }
}

void DuiBasicOrientationAnimation::rootElementChanged()
{
    Q_D(DuiBasicOrientationAnimation);
    d->rotationAnimation->setTargetObject(rootElement());
    d->positionAnimation->setTargetObject(rootElement());
    d->originAnimation->setTargetObject(rootElement());
}

void DuiBasicOrientationAnimation::setRootElement(QGraphicsWidget *rootElement)
{
    Q_D(DuiBasicOrientationAnimation);
    if (d->rootElement != rootElement) {
        d->rootElement = rootElement;
        rootElementChanged();
    }
}

QGraphicsWidget *DuiBasicOrientationAnimation::rootElement()
{
    Q_D(DuiBasicOrientationAnimation);
    return d->rootElement;
}

void DuiBasicOrientationAnimation::updateState(QAbstractAnimation::State newState,
        QAbstractAnimation::State oldState)
{
    Q_D(DuiBasicOrientationAnimation);

    if (newState == QAbstractAnimation::Running) {
        if (d->navigationBar)
            d->setupNavigationBarAnimations();

        if (d->homeButtonPanel)
            d->setupHomeButtonAnimations();

        if (d->escapeButtonPanel)
            d->setupEscapeButtonAnimations();
    }

    DuiParallelAnimationGroup::updateState(newState, oldState);
}

#include "moc_duibasicorientationanimation.cpp"
