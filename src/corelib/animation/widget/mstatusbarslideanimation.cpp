/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mstatusbarslideanimation.h"
#include "manimationcreator.h"
#include "mabstractwidgetanimation_p.h"

#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <mwidgetcontroller.h>
#include <mscenemanager.h>
#include <mscenemanager_p.h>
#include <mscenewindow.h>

MStatusBarSlideAnimation::MStatusBarSlideAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MAbstractWidgetAnimationPrivate, parent)
{
    played = false;
    direction = In;

    positionAnimation = new QPropertyAnimation;
    positionAnimation->setPropertyName("pos");
    addAnimation(positionAnimation);

    sceneWindowStack = 0;
    sceneManager = 0;
}

MStatusBarSlideAnimation::~MStatusBarSlideAnimation()
{
}

void MStatusBarSlideAnimation::setTargetWidget(MWidgetController *widget)
{
    MAbstractWidgetAnimation::setTargetWidget(widget);

   played = false;
   positionAnimation->setTargetObject(targetWidget());
}

void MStatusBarSlideAnimation::setTransitionDirection(TransitionDirection direction)
{
    this->direction = direction;

    if (direction == In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");
}

void MStatusBarSlideAnimation::restoreTargetWidgetState()
{
    if (played)
        targetWidget()->setPos(originalPos);
}

void MStatusBarSlideAnimation::setRootElementsDisplacedByStatusBar(
    QList<QGraphicsWidget*> &elementsList)
{
    Q_ASSERT(state() == QAbstractAnimation::Stopped);
    destroyRootElementsAnimations();

    Q_FOREACH(QGraphicsWidget *rootElement, elementsList) {
        rootElementsDisplacedByStatusBar.append(
                    QWeakPointer<QGraphicsWidget>(rootElement));
    }

    createRootElementsAnimations();
}

void MStatusBarSlideAnimation::setSceneWindowStack(const QList<MSceneWindow *> *list)
{
    Q_ASSERT(state() == QAbstractAnimation::Stopped);
    sceneWindowStack = list;
}

void MStatusBarSlideAnimation::setSceneManager(MSceneManager *sceneManager)
{
    this->sceneManager = sceneManager;
}

void MStatusBarSlideAnimation::updateState(QAbstractAnimation::State newState,
                                           QAbstractAnimation::State oldState)
{

    if (!targetWidget())
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        if (style().objectName().isNull())
            style().setObjectName("In");

        setupStatusBarAnimation();
        setupRootElementsAnimations();
        createAndSetupSceneWindowsAnimations();
        played = true;
    } else if (newState == QAbstractAnimation::Stopped) {
        destroySceneWindowsAnimations();
    }
}

void MStatusBarSlideAnimation::setupStatusBarAnimation()
{
    Q_ASSERT(targetWidget());

    originalPos = targetWidget()->pos();

    QPointF offscreenPos;

    offscreenPos = QPointF(0, -targetWidget()->size().height());

    if (direction == In) {
        // because animation doesn't immediately set properties to start values
        // we must explicitly set targetWidget position here to prevent widget
        // poping out in finalPosition before animation starts
        targetWidget()->setPos(offscreenPos);
        positionAnimation->setStartValue(offscreenPos);
        positionAnimation->setEndValue(originalPos);
    } else {
        positionAnimation->setStartValue(originalPos);
        positionAnimation->setEndValue(offscreenPos);
    }

    positionAnimation->setEasingCurve(style()->easingCurve());
    positionAnimation->setDuration(style()->duration());
}

void MStatusBarSlideAnimation::setupRootElementsAnimations()
{
    Q_ASSERT(targetWidget());

    qreal startY;
    qreal endY;

    QEasingCurve easingCurve = style()->easingCurve();
    qreal duration = style()->duration();

    if (direction == In) {
        startY = 0.0f;
        endY = targetWidget()->size().height();
    } else {
        startY = targetWidget()->size().height();
        endY = 0.0f;
    }

    Q_FOREACH(QPropertyAnimation *rootElementAnimation, rootElementsAnimations) {
        rootElementAnimation->setStartValue(startY);
        rootElementAnimation->setEndValue(endY);
        rootElementAnimation->setEasingCurve(easingCurve);
        rootElementAnimation->setDuration(duration);
    }
}

void MStatusBarSlideAnimation::destroyRootElementsAnimations()
{
    Q_FOREACH(QPropertyAnimation *rootElementAnimation, rootElementsAnimations) {
        removeAnimation(rootElementAnimation);
        delete rootElementAnimation;
    }
    rootElementsAnimations.clear();
}

void MStatusBarSlideAnimation::createRootElementsAnimations()
{
    QGraphicsWidget *rootElement;

    Q_FOREACH(QWeakPointer<QGraphicsWidget> rootElementPointer,
              rootElementsDisplacedByStatusBar) {
        rootElement= rootElementPointer.data();
        if (!rootElement)
            continue;

        QPropertyAnimation *rootElementAnimation = new QPropertyAnimation;
        rootElementAnimation->setTargetObject(rootElement);
        rootElementAnimation->setPropertyName("y");
        addAnimation(rootElementAnimation);
        rootElementsAnimations.append(rootElementAnimation);
    }
}

void MStatusBarSlideAnimation::destroySceneWindowsAnimations()
{
    Q_FOREACH(QPropertyAnimation *sceneWindowAnimation, sceneWindowsAnimations) {
        removeAnimation(sceneWindowAnimation);
        delete sceneWindowAnimation;
    }
    sceneWindowsAnimations.clear();
}

void MStatusBarSlideAnimation::createAndSetupSceneWindowsAnimations()
{
    QEasingCurve easingCurve = style()->easingCurve();
    qreal duration = style()->duration();
    MSceneManagerPrivate *sceneManagerPriv = sceneManager->d_func();

    Q_FOREACH(MSceneWindow *sceneWindow, *sceneWindowStack) {

        if (sceneWindow->windowType() == MSceneWindow::Sheet ||
            sceneWindow->windowType() == MSceneWindow::ApplicationPage ||
            sceneWindow->windowType() == MSceneWindow::ObjectMenu ||
            sceneWindow->windowType() == MSceneWindow::ApplicationMenu)
        {
            QPropertyAnimation *geometryAnimation = new QPropertyAnimation;
            geometryAnimation->setTargetObject(sceneWindow);
            geometryAnimation->setPropertyName("geometry");
            geometryAnimation->setStartValue(sceneWindow->geometry());
            geometryAnimation->setEndValue(sceneManagerPriv->calculateSceneWindowGeometry(sceneWindow));
            geometryAnimation->setEasingCurve(easingCurve);
            geometryAnimation->setDuration(duration);
            addAnimation(geometryAnimation);
        }
    }
}

#include "moc_mstatusbarslideanimation.cpp"

M_REGISTER_ANIMATION(MStatusBarSlideAnimation)
