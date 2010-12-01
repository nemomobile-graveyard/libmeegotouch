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

#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>

namespace {
    qreal curve_points[] = {0, 0.0009, 0.0037, 0.0085, 0.0156, 0.0252, 0.0375, 0.0530, 0.0719, 0.0949,
                            0.1226, 0.1557, 0.1954, 0.2432, 0.3014, 0.3736, 0.4658, 0.5866, 0.6698,
                            0.7263, 0.7700, 0.8057, 0.8358, 0.8618, 0.8845, 0.9046, 0.9225, 0.9384,
                            0.9528, 0.9657, 0.9773, 0.9879, 0.9974, 1.0060, 1.0138, 1.0208, 1.0271,
                            1.0327, 1.0377, 1.0422, 1.0461, 1.0496, 1.0525, 1.0551, 1.0572, 1.0589,
                            1.0603, 1.0613, 1.0619, 1.0623, 1.0623, 1.0621, 1.0615, 1.0607, 1.0597,
                            1.0584, 1.0568, 1.0550, 1.0530, 1.0508, 1.0484, 1.0458, 1.0430, 1.0399,
                            1.0368, 1.0340, 1.0313, 1.0289, 1.0266, 1.0244, 1.0224, 1.0206, 1.0188,
                            1.0172, 1.0157, 1.0142, 1.0129, 1.0117, 1.0105, 1.0094, 1.0084, 1.0075,
                            1.0066, 1.0058, 1.0051, 1.0044, 1.0038, 1.0032, 1.0027, 1.0023, 1.0019,
                            1.0015, 1.0012, 1.0009, 1.0006, 1.0004, 1.0003, 1.0002, 1.0001, 1.0000};

    qreal bezierEasingFunction(qreal progress) {
        return curve_points[(int)(progress * (sizeof curve_points / sizeof curve_points[0] - 1))];
    }
}

const QString MDialogAnimationPrivate::DialogBoxObjectName = "MDialogBox";

MDialogAnimationPrivate::MDialogAnimationPrivate() :
    MAbstractWidgetAnimationPrivate(),
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
    titleBarPositionAnimation->setPropertyName("pos");

    contentsViewportAnimationDelay = new QPauseAnimation;
    contentsViewportOpacityAnimation = new QPropertyAnimation;
    contentsViewportScaleAnimation = new QPropertyAnimation;
    contentsViewportPositionAnimation = new QPropertyAnimation;
    contentsViewportOpacityAnimation->setPropertyName("opacity");
    contentsViewportScaleAnimation->setPropertyName("scale");
    contentsViewportPositionAnimation->setPropertyName("pos");

    buttonBoxAnimationDelay = new QPauseAnimation;
    buttonBoxOpacityAnimation = new QPropertyAnimation;
    buttonBoxPositionAnimation = new QPropertyAnimation;
    buttonBoxOpacityAnimation->setPropertyName("opacity");
    buttonBoxPositionAnimation->setPropertyName("pos");

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
    return widgetPos + distance * (1 - q->style()->scale());
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

    QPointF titleBarPos = titleBar->pos();

    if (direction == MDialogAnimation::In) {
        titleBar->setOpacity(0);
        titleBarOpacityAnimation->setStartValue(0);
        titleBarOpacityAnimation->setEndValue(q->style()->opacity());

        titleBarPositionAnimation->setEndValue(titleBarPos);
        titleBarPos.ry() += q->style()->titleBarAnimationDistance();
        titleBarPositionAnimation->setStartValue(titleBarPos);
    } else {
        titleBarOpacityAnimation->setStartValue(q->style()->opacity());
        titleBarOpacityAnimation->setEndValue(0);

        titleBarPositionAnimation->setStartValue(titleBarPos);
        titleBarPos.ry() += q->style()->titleBarAnimationDistance();
        titleBarPositionAnimation->setEndValue(titleBarPos);
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
        contentsViewportPositionAnimation->setEndValue(contentsViewportPos);
    } else {
        contentsViewportOpacityAnimation->setStartValue(q->style()->opacity());
        contentsViewportOpacityAnimation->setEndValue(0);

        contentsViewportScaleAnimation->setStartValue(1);
        contentsViewportScaleAnimation->setEndValue(q->style()->scale());

        contentsViewportPositionAnimation->setStartValue(contentsViewportPos);
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

    qreal buttonBoxPosY = qMin(contentsViewport->sizeHint(Qt::PreferredSize).height(),
                               contentsViewport->sizeHint(Qt::MaximumSize).height());

    QPointF buttonBoxPos(0, buttonBoxPosY);
    if (titleBar && titleBar->isVisible())
        buttonBoxPos.ry() += titleBar->sizeHint(Qt::PreferredSize).height();

    if (direction == MDialogAnimation::In) {
        buttonBox->setOpacity(0);
        buttonBoxOpacityAnimation->setStartValue(0);
        buttonBoxOpacityAnimation->setEndValue(q->style()->opacity());

        buttonBoxPositionAnimation->setEndValue(buttonBoxPos);
        buttonBoxPos.ry() -= q->style()->buttonBoxAnimationDistance();
        buttonBoxPositionAnimation->setStartValue(buttonBoxPos);
    } else {
        buttonBoxOpacityAnimation->setStartValue(q->style()->opacity());
        buttonBoxOpacityAnimation->setEndValue(0);

        buttonBoxPositionAnimation->setStartValue(buttonBoxPos);
        buttonBoxPos.ry() -= q->style()->buttonBoxAnimationDistance();
        buttonBoxPositionAnimation->setEndValue(buttonBoxPos);
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

    QEasingCurve bezierEasingCurve;
    bezierEasingCurve.setCustomType(bezierEasingFunction);

    titleBarOpacityAnimation->setEasingCurve(bezierEasingCurve);
    titleBarPositionAnimation->setEasingCurve(bezierEasingCurve);
    contentsViewportOpacityAnimation->setEasingCurve(q->style()->contentsViewportOpacityAnimationEasingCurve());
    contentsViewportScaleAnimation->setEasingCurve(q->style()->contentsViewportScaleAnimationEasingCurve());
    contentsViewportPositionAnimation->setEasingCurve(q->style()->contentsViewportScaleAnimationEasingCurve());
    buttonBoxOpacityAnimation->setEasingCurve(bezierEasingCurve);
    buttonBoxPositionAnimation->setEasingCurve(bezierEasingCurve);
}

#include "moc_mdialoganimation.cpp"

M_REGISTER_ANIMATION(MDialogAnimation)
