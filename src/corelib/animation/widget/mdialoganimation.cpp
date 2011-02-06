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

#include "mdialoganimation.h"
#include "mdialoganimation_p.h"
#include "manimationcreator.h"
#include "mdialog.h"
#include "mscenemanager.h"
#include "movershotbeziereasingcurve.h"

#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>

const QString MDialogAnimationPrivate::DialogBoxObjectName = "MDialogBox";

MDialogAnimationPrivate::MDialogAnimationPrivate() :
    MAbstractWidgetAnimationPrivate(),
    direction(MDialogAnimation::In),
    delayedTitleBarAnimation(0),
    delayedContentsViewportAnimation(0),
    delayedButtonBoxAnimation(0),
    titleBarAnimationDelay(0),
    contentsViewportAnimationDelay(0),
    buttonBoxAnimationDelay(0),
    titleBarOpacityAnimation(0),
    titleBarPositionAnimation(0),
    contentsViewportOpacityAnimation(0),
    contentsViewportScaleAnimation(0),
    contentsViewportPositionAnimation(0),
    buttonBoxOpacityAnimation(0),
    buttonBoxPositionAnimation(0),
    dialogBox(0),
    titleBar(0),
    contentsViewport(0),
    buttonBox(0)
{
}

void MDialogAnimationPrivate::init()
{
    Q_Q(MDialogAnimation);

    direction = MDialogAnimation::In;

    titleBarAnimationDelay = new QPauseAnimation;
    titleBarOpacityAnimation = new QPropertyAnimation;
    titleBarPositionAnimation = new QPropertyAnimation;
    titleBarOpacityAnimation->setPropertyName("opacity");
    titleBarPositionAnimation->setPropertyName("paintOffset");

    contentsViewportAnimationDelay = new QPauseAnimation;
    contentsViewportOpacityAnimation = new QPropertyAnimation;
    contentsViewportScaleAnimation = new QPropertyAnimation;
    contentsViewportPositionAnimation = new QPropertyAnimation;
    contentsViewportOpacityAnimation->setPropertyName("opacity");
    contentsViewportScaleAnimation->setPropertyName("scale");
    contentsViewportPositionAnimation->setPropertyName("paintOffset");

    buttonBoxAnimationDelay = new QPauseAnimation;
    buttonBoxOpacityAnimation = new QPropertyAnimation;
    buttonBoxPositionAnimation = new QPropertyAnimation;
    buttonBoxOpacityAnimation->setPropertyName("opacity");
    buttonBoxPositionAnimation->setPropertyName("paintOffset");

    QParallelAnimationGroup *titleBarAnimation = new QParallelAnimationGroup;
    titleBarAnimation->addAnimation(titleBarOpacityAnimation);
    titleBarAnimation->addAnimation(titleBarPositionAnimation);

    delayedTitleBarAnimation = new QSequentialAnimationGroup(q);
    delayedTitleBarAnimation->addAnimation(titleBarAnimationDelay);
    delayedTitleBarAnimation->addAnimation(titleBarAnimation);

    QParallelAnimationGroup *contentsViewportAnimation = new QParallelAnimationGroup;
    contentsViewportAnimation->addAnimation(contentsViewportOpacityAnimation);
    contentsViewportAnimation->addAnimation(contentsViewportScaleAnimation);
    contentsViewportAnimation->addAnimation(contentsViewportPositionAnimation);

    delayedContentsViewportAnimation = new QSequentialAnimationGroup(q);
    delayedContentsViewportAnimation->addAnimation(contentsViewportAnimationDelay);
    delayedContentsViewportAnimation->addAnimation(contentsViewportAnimation);

    QParallelAnimationGroup *buttonBoxAnimation = new QParallelAnimationGroup;
    buttonBoxAnimation->addAnimation(buttonBoxOpacityAnimation);
    buttonBoxAnimation->addAnimation(buttonBoxPositionAnimation);

    delayedButtonBoxAnimation = new QSequentialAnimationGroup(q);
    delayedButtonBoxAnimation->addAnimation(buttonBoxAnimationDelay);
    delayedButtonBoxAnimation->addAnimation(buttonBoxAnimation);
}

void MDialogAnimationPrivate::findComponentsForAnimation()
{
    Q_Q(MDialogAnimation);

    dialogBox = findDialogBox(targetWidget);
    QGraphicsItem *parentItem = dialogBox ? dialogBox : targetWidget;

    foreach (QGraphicsItem *childItem, parentItem->childItems()) {
        if (childItem->isWidget()) {
            QGraphicsWidget *childWidget = static_cast<QGraphicsWidget*>(childItem);
            if (!childWidget->objectName().isEmpty()) {
                if (!titleBar && childWidget->objectName() == q->style()->titleBarObjectName()) {
                    titleBar = qobject_cast<MWidgetController*>(childWidget);
                    if (titleBar) {
                        titleBarOpacityAnimation->setTargetObject(childWidget);
                        titleBarPositionAnimation->setTargetObject(childWidget);
                    }
                } else if (!contentsViewport && childWidget->objectName() == q->style()->contentsViewportObjectName()) {
                    contentsViewport = qobject_cast<MWidgetController*>(childWidget);
                    if (contentsViewport) {
                        contentsViewportOpacityAnimation->setTargetObject(childWidget);
                        contentsViewportScaleAnimation->setTargetObject(childWidget);
                        contentsViewportPositionAnimation->setTargetObject(childWidget);
                    }
                } else if (!buttonBox && childWidget->objectName() == q->style()->buttonBoxObjectName()) {
                    buttonBox = qobject_cast<MWidgetController*>(childWidget);
                    if (buttonBox) {
                        buttonBoxOpacityAnimation->setTargetObject(childWidget);
                        buttonBoxPositionAnimation->setTargetObject(childWidget);
                    }
                }
                if (titleBar && contentsViewport && buttonBox)
                    break;
            }
        }
    }
}

QPointF MDialogAnimationPrivate::setupPositionAnimation(const QPointF &widgetPos)
{
    Q_Q(MDialogAnimation);

    QPointF distance = origin - widgetPos;
    return distance * (1 - q->style()->scale());
}

MDialogAnimation::MDialogAnimation(QObject *parent) :
        MAbstractWidgetAnimation(new MDialogAnimationPrivate, parent)
{
    Q_D(MDialogAnimation);

    d->init();
}

MDialogAnimation::MDialogAnimation(MDialogAnimationPrivate *dd, QObject *parent) :
        MAbstractWidgetAnimation(dd, parent)
{
    Q_D(MDialogAnimation);

    d->init();
}

QGraphicsWidget* MDialogAnimationPrivate::findDialogBox(QGraphicsItem *parentItem)
{
    foreach (QGraphicsItem *childItem, parentItem->childItems()) {
        if (childItem->isWidget()) {
            QGraphicsWidget *childWidget = static_cast<QGraphicsWidget*>(childItem);
            if (childWidget->objectName() == DialogBoxObjectName)
                return childWidget;
        }
    }
    return 0;
}

QPointF MDialogAnimation::origin() const
{
    Q_D(const MDialogAnimation);

    return d->origin;
}

void MDialogAnimation::setOrigin(const QPointF &pos)
{
    Q_D(MDialogAnimation);

    d->origin = pos;
}

void MDialogAnimation::restoreTargetWidgetState()
{
}

void MDialogAnimation::setTransitionDirection(TransitionDirection direction)
{
    Q_D(MDialogAnimation);

    d->direction = direction;

    if (d->direction == In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");
}

void MDialogAnimation::updateState(QAbstractAnimation::State newState,
                                   QAbstractAnimation::State oldState)
{
    Q_D(MDialogAnimation);

    QAbstractAnimation::updateState(newState, oldState);

    if (!d->targetWidget || !d->targetWidget->sceneManager())
        return;


    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        d->findComponentsForAnimation();

        if (style().objectName().isNull())
            style().setObjectName("In");

        d->origin.rx() = d->targetWidget->sceneManager()->visibleSceneSize().width() / 2;
        d->origin = d->contentsViewport->mapFromScene(d->origin);
        if (d->dialogBox)
            d->origin.ry() = d->dialogBox->geometry().height() / 2;
        else
            d->origin.ry() = d->targetWidget->geometry().height() / 2;

        d->setupDurations();
        d->setupEasingCurves();

        d->setupTitleBarAnimation();
        d->setupContentsViewportAnimation();
        d->setupButtonBoxAnimation();
    }
}

void MDialogAnimationPrivate::setupTitleBarAnimation()
{
    if (!titleBar || !titleBar->isVisible()) {
        delete delayedTitleBarAnimation;
        delayedTitleBarAnimation = 0;
        return;
    }

    Q_Q(MDialogAnimation);

    if (direction == MDialogAnimation::In) {
        titleBar->setOpacity(0);
        titleBarOpacityAnimation->setStartValue(0);
        titleBarOpacityAnimation->setEndValue(q->style()->opacity());
        titleBarPositionAnimation->setEndValue(QPointF(0,0));
        titleBarPositionAnimation->setStartValue(QPointF(0,q->style()->titleBarAnimationDistance()));
    } else {
        titleBarOpacityAnimation->setStartValue(q->style()->opacity());
        titleBarOpacityAnimation->setEndValue(0);


        titleBarPositionAnimation->setStartValue(QPointF(0,0));
        titleBarPositionAnimation->setEndValue(QPointF(0,q->style()->titleBarAnimationDistance()));
    }

    origin.ry() += titleBar->sizeHint(Qt::PreferredSize).height();

    q->addAnimation(delayedTitleBarAnimation);
}

void MDialogAnimationPrivate::setupContentsViewportAnimation()
{
    if (!contentsViewport || !contentsViewport->isVisible()) {
        delete delayedContentsViewportAnimation;
        delayedContentsViewportAnimation = 0;
        return;
    }

    Q_Q(MDialogAnimation);

    QPointF contentsViewportPos = contentsViewport->pos();
    if (direction == MDialogAnimation::In) {
        contentsViewport->setOpacity(0);
        contentsViewportOpacityAnimation->setStartValue(0);
        contentsViewportOpacityAnimation->setEndValue(q->style()->opacity());

        contentsViewportScaleAnimation->setStartValue(q->style()->scale());
        contentsViewportScaleAnimation->setEndValue(1);

        contentsViewportPositionAnimation->setStartValue(setupPositionAnimation(contentsViewportPos));
        contentsViewportPositionAnimation->setEndValue(QPointF(0,0));
    } else {
        contentsViewportOpacityAnimation->setStartValue(q->style()->opacity());
        contentsViewportOpacityAnimation->setEndValue(0);

        contentsViewportScaleAnimation->setStartValue(1);
        contentsViewportScaleAnimation->setEndValue(q->style()->scale());

        contentsViewportPositionAnimation->setStartValue(QPointF(0,0));
        contentsViewportPositionAnimation->setEndValue(setupPositionAnimation(contentsViewportPos));
    }

    q->addAnimation(delayedContentsViewportAnimation);
}

void MDialogAnimationPrivate::setupButtonBoxAnimation()
{
    if (!buttonBox || !buttonBox->isVisible()) {
        delete delayedButtonBoxAnimation;
        delayedButtonBoxAnimation = 0;
        return;
    }

    Q_Q(MDialogAnimation);

    if (direction == MDialogAnimation::In) {
        buttonBox->setOpacity(0);
        buttonBoxOpacityAnimation->setStartValue(0);
        buttonBoxOpacityAnimation->setEndValue(q->style()->opacity());

        buttonBoxPositionAnimation->setStartValue(QPointF(0,-q->style()->buttonBoxAnimationDistance()));
        buttonBoxPositionAnimation->setEndValue(QPointF(0,0));
    } else {
        buttonBoxOpacityAnimation->setStartValue(q->style()->opacity());
        buttonBoxOpacityAnimation->setEndValue(0);

        buttonBoxPositionAnimation->setStartValue(QPointF(0,0));
        buttonBoxPositionAnimation->setEndValue(QPointF(0,-q->style()->buttonBoxAnimationDistance()));
    }

    q->addAnimation(delayedButtonBoxAnimation);
}

void MDialogAnimationPrivate::setupDurations()
{
    Q_Q(MDialogAnimation);

    titleBarAnimationDelay->setDuration(q->style()->titleBarAnimationDelay());
    contentsViewportAnimationDelay->setDuration(q->style()->contentsViewportAnimationDelay());
    buttonBoxAnimationDelay->setDuration(q->style()->buttonBoxAnimationDelay());

    titleBarOpacityAnimation->setDuration(q->style()->titleBarAnimationDuration());
    titleBarPositionAnimation->setDuration(q->style()->titleBarAnimationDuration());
    contentsViewportOpacityAnimation->setDuration(q->style()->contentsViewportOpacityAnimationDuration());
    contentsViewportScaleAnimation->setDuration(q->style()->contentsViewportScaleAnimationDuration());
    contentsViewportPositionAnimation->setDuration(q->style()->contentsViewportScaleAnimationDuration());
    buttonBoxOpacityAnimation->setDuration(q->style()->buttonBoxAnimationDuration());
    buttonBoxPositionAnimation->setDuration(q->style()->buttonBoxAnimationDuration());
}

void MDialogAnimationPrivate::setupEasingCurves()
{
    Q_Q(MDialogAnimation);

    titleBarOpacityAnimation->setEasingCurve(q->style()->titleBarAnimationEasingCurve());
    titleBarPositionAnimation->setEasingCurve(q->style()->titleBarAnimationEasingCurve());
    contentsViewportOpacityAnimation->setEasingCurve(q->style()->contentsViewportOpacityAnimationEasingCurve());
    contentsViewportScaleAnimation->setEasingCurve(q->style()->contentsViewportScaleAnimationEasingCurve());
    contentsViewportPositionAnimation->setEasingCurve(q->style()->contentsViewportScaleAnimationEasingCurve());
    buttonBoxOpacityAnimation->setEasingCurve(q->style()->buttonBoxAnimationEasingCurve());
    buttonBoxPositionAnimation->setEasingCurve(q->style()->buttonBoxAnimationEasingCurve());
}

#include "moc_mdialoganimation.cpp"

M_REGISTER_ANIMATION(MDialogAnimation)
