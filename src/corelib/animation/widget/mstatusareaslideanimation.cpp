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

#include "mstatusareaslideanimation.h"
#include "mstatusareaslideanimation_p.h"
#include "manimationcreator.h"

#include "mscenemanager_p.h"

#include <QPauseAnimation>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <mwidgetcontroller.h>
#include <mscenemanager.h>
#include <mstatusbar.h>

void MStatusAreaSlideAnimationPrivate::init()
{
    Q_Q(MStatusAreaSlideAnimation);

    direction = MStatusAreaSlideAnimation::In;

    widgetAnimationDelay = new QPauseAnimation;
    widgetPosAnimation = new QPropertyAnimation;
    widgetPosAnimation->setPropertyName("pos");
    widgetPosAnimation->setEasingCurve(QEasingCurve::OutExpo);
    widgetAnimation = new QSequentialAnimationGroup();
    widgetAnimation->addAnimation(widgetAnimationDelay);
    widgetAnimation->addAnimation(widgetPosAnimation);

    statusBarAnimationDelay = new QPauseAnimation;
    statusBarPosAnimation = new QPropertyAnimation;
    statusBarPosAnimation->setPropertyName("paintOffset");
    statusBarPosAnimation->setEasingCurve(QEasingCurve::InExpo);
    statusBarAnimation = new QSequentialAnimationGroup();
    statusBarAnimation->addAnimation(statusBarPosAnimation);
    statusBarAnimation->addAnimation(statusBarAnimationDelay);

    q->addAnimation(widgetAnimation);
    q->addAnimation(statusBarAnimation);

    q->connect(widgetAnimationDelay, SIGNAL(finished()), SLOT(_q_onDelayFinished()));
}

void MStatusAreaSlideAnimationPrivate::_q_onDelayFinished()
{
    targetWidget->show();
}

void MStatusAreaSlideAnimationPrivate::findStatusBar(QGraphicsItem *parentItem)
{
    statusBarPointer.clear();
    if (parentItem) {
        foreach (QGraphicsItem *childItem, parentItem->childItems()) {
            if (childItem->isWidget()) {
                QGraphicsWidget *childWidget = static_cast<QGraphicsWidget*>(childItem);
                if (childWidget)
                    statusBarPointer = qobject_cast<MStatusBar*>(childWidget);
                if (!statusBarPointer.isNull())
                    break;
            }
        }
    }
}

void MStatusAreaSlideAnimationPrivate::restoreStatusBarOriginalState()
{
    if (!statusBarPointer.isNull()) {
        statusBarPointer.data()->setZValue(MSceneManagerPrivate::StatusBar);
        statusBarPointer.data()->setPaintOffset(QPointF(0,0));
    }
}


MStatusAreaSlideAnimation::MStatusAreaSlideAnimation(MStatusAreaSlideAnimationPrivate *dd, QObject *parent) :
    MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MStatusAreaSlideAnimation);

    d->init();
}

MStatusAreaSlideAnimation::MStatusAreaSlideAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MStatusAreaSlideAnimationPrivate, parent)
{
    Q_D(MStatusAreaSlideAnimation);

    d->init();
}

MStatusAreaSlideAnimation::~MStatusAreaSlideAnimation()
{
}

void MStatusAreaSlideAnimation::setTargetWidget(MWidgetController *widget)
{
    Q_D(MStatusAreaSlideAnimation);
    MAbstractWidgetAnimation::setTargetWidget(widget);

    d->played = false;
    d->widgetPosAnimation->setTargetObject(targetWidget());
}

void MStatusAreaSlideAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MStatusAreaSlideAnimation);

    d->direction = direction;

    d->statusBarAnimation->removeAnimation(d->statusBarPosAnimation);
    d->statusBarAnimation->removeAnimation(d->statusBarAnimationDelay);
    d->widgetAnimation->removeAnimation(d->widgetAnimationDelay);
    d->widgetAnimation->removeAnimation(d->widgetPosAnimation);

    if (d->direction == In) {
        style().setObjectName("In");

        d->widgetAnimation->addAnimation(d->widgetAnimationDelay);
        d->widgetAnimation->addAnimation(d->widgetPosAnimation);

        d->statusBarAnimation->addAnimation(d->statusBarPosAnimation);
        d->statusBarAnimation->addAnimation(d->statusBarAnimationDelay);
    }
    else {
        style().setObjectName("Out");

        d->widgetAnimation->addAnimation(d->widgetPosAnimation);
        d->widgetAnimation->addAnimation(d->widgetAnimationDelay);

        d->statusBarAnimation->addAnimation(d->statusBarAnimationDelay);
        d->statusBarAnimation->addAnimation(d->statusBarPosAnimation);
    }
}

void MStatusAreaSlideAnimation::restoreTargetWidgetState()
{
    Q_D(MStatusAreaSlideAnimation);
    if (d->played) {
        targetWidget()->setPos(d->originalPos);
        d->restoreStatusBarOriginalState();
    }
}

void MStatusAreaSlideAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MStatusAreaSlideAnimation);

    if (!targetWidget())
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        d->findStatusBar(targetWidget()->parentItem());
        QGraphicsWidget *statusBar = d->statusBarPointer.data();
        d->statusBarPosAnimation->setTargetObject(statusBar);
        if (statusBar) {
            statusBar->setZValue(0);
        }

        d->originalPos = targetWidget()->pos();

        QPointF offscreenPos, translation;
        offscreenPos = QPointF(0, -targetWidget()->size().height());
        translation = QPointF(0, statusBar ? statusBar->size().height() : targetWidget()->size().height());
        if (d->direction == In) {
            targetWidget()->hide();
            d->widgetPosAnimation->setStartValue(offscreenPos);
            d->widgetPosAnimation->setEndValue(offscreenPos+translation);
            if (statusBar) {
                d->statusBarPosAnimation->setStartValue(QPointF(0,0));
                d->statusBarPosAnimation->setEndValue(translation);
            }

        } else {
            d->widgetPosAnimation->setStartValue(offscreenPos+translation);
            d->widgetPosAnimation->setEndValue(offscreenPos);
            if (statusBar) {
                d->statusBarPosAnimation->setStartValue(translation);
                d->statusBarPosAnimation->setEndValue(QPointF(0,0));
            }
        }

        d->widgetAnimationDelay->setDuration(style()->widgetSlideDelay());
        d->widgetPosAnimation->setDuration(style()->widgetSlideDuration());
        d->widgetPosAnimation->setEasingCurve(style()->widgetSlideEasingCurve());

        d->statusBarAnimationDelay->setDuration(style()->statusBarSlideDelay());
        d->statusBarPosAnimation->setDuration(style()->statusBarSlideDuration());
        d->statusBarPosAnimation->setEasingCurve(style()->statusBarSlideEasingCurve());

        d->played = true;
    } else if (oldState == QAbstractAnimation::Running &&
               newState == QAbstractAnimation::Stopped) {
        if (d->direction == Out) {
            d->restoreStatusBarOriginalState();
        }
    }
}

#include "moc_mstatusareaslideanimation.cpp"

M_REGISTER_ANIMATION(MStatusAreaSlideAnimation)
