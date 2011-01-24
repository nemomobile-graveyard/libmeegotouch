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

#include "mbasiclistitemdeletionanimation.h"
#include "mbasiclistitemdeletionanimation_p.h"
#include "manimationcreator.h"

#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QDebug>

MBasicListItemDeletionAnimationPrivate::MBasicListItemDeletionAnimationPrivate()
    : q_ptr(0)
{
}

MBasicListItemDeletionAnimationPrivate::~MBasicListItemDeletionAnimationPrivate()
{
}

void MBasicListItemDeletionAnimationPrivate::_q_resetAnimation()
{
    Q_Q(MBasicListItemDeletionAnimation);

    resetTargetWidgets();
    q->clear();
}

void MBasicListItemDeletionAnimationPrivate::appendTargetWidget(MWidget *cell)
{
    targets.insert(cell, TargetDefaultProperties(cell->zValue(), cell->opacity(), cell->scale(), cell->isVisible()));
}

void MBasicListItemDeletionAnimationPrivate::resetTargetWidgets()
{
    foreach (MWidget *cell, targets.keys()) {
        TargetDefaultProperties properties = targets.value(cell);
        cell->setZValue(properties.zValue);
        cell->setOpacity(properties.opacity);
        cell->setScale(properties.scale);
        cell->setVisible(properties.visible);
    }
    targets.clear();
}

QPropertyAnimation *MBasicListItemDeletionAnimationPrivate::createOpacityAnimation(MWidget *cell)
{
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation;
    opacityAnimation->setTargetObject(cell);
    opacityAnimation->setPropertyName("opacity");
    opacityAnimation->setStartValue(cell->opacity());

    return opacityAnimation;
}

QPropertyAnimation *MBasicListItemDeletionAnimationPrivate::createScaleAnimation(MWidget *cell)
{
    QPropertyAnimation *scaleAnimation = new QPropertyAnimation;
    scaleAnimation->setTargetObject(cell);
    scaleAnimation->setPropertyName("scale");
    scaleAnimation->setStartValue(cell->scale());

    return scaleAnimation;
}

QPropertyAnimation *MBasicListItemDeletionAnimationPrivate::createPositionAnimation(MWidget *cell, const QPointF &targetPos)
{
    QPropertyAnimation *moveAnimation = new QPropertyAnimation;
    moveAnimation->setTargetObject(cell);
    moveAnimation->setPropertyName("pos");
    moveAnimation->setStartValue(cell->pos());
    moveAnimation->setEndValue(targetPos);

    return moveAnimation;
}

QPauseAnimation *MBasicListItemDeletionAnimationPrivate::createDelayAnimation(int delay)
{
    QPauseAnimation *delayAnimation = new QPauseAnimation();
    delayAnimation->setDuration(delay);
    return delayAnimation;
}

MBasicListItemDeletionAnimation::MBasicListItemDeletionAnimation(QObject *parent)
    : MParallelAnimationGroup(parent),
    d_ptr(new MBasicListItemDeletionAnimationPrivate)
{
    d_ptr->q_ptr = this;
    connect(this, SIGNAL(finished()), SLOT(_q_resetAnimation()));
}

MBasicListItemDeletionAnimation::~MBasicListItemDeletionAnimation()
{
    Q_D(MBasicListItemDeletionAnimation);
    d->resetTargetWidgets();
    clear();

    delete d_ptr;
}

void MBasicListItemDeletionAnimation::appendBeforeTarget(MWidget *cell)
{
    Q_D(MBasicListItemDeletionAnimation);
    d->appendTargetWidget(cell);
}

void MBasicListItemDeletionAnimation::appendDeleteTarget(MWidget *cell)
{
    Q_D(MBasicListItemDeletionAnimation);
    d->appendTargetWidget(cell);

    // Set target cell z value above the parent
    cell->setZValue(cell->parentItem()->zValue() + 1);
    cell->setTransformOriginPoint(cell->boundingRect().center());

    // Opacity animation
    QPropertyAnimation *opacityAnimation = d->createOpacityAnimation(cell);
    opacityAnimation->setDuration(style()->deleteDuration());
    opacityAnimation->setEndValue(style()->deleteOpacity());
    opacityAnimation->setEasingCurve(style()->deleteCurve());

    // Scale animation
    QPropertyAnimation *scaleAnimation = d->createScaleAnimation(cell);
    scaleAnimation->setDuration(style()->deleteDuration());
    scaleAnimation->setEndValue(style()->deleteScale());
    scaleAnimation->setEasingCurve(style()->deleteCurve());

    QParallelAnimationGroup *transformAnimation = new QParallelAnimationGroup(this);
    transformAnimation->addAnimation(opacityAnimation);
    transformAnimation->addAnimation(scaleAnimation);

    // Animation sequence
    QSequentialAnimationGroup *animation = new QSequentialAnimationGroup(this);
    animation->addAnimation(d->createDelayAnimation(style()->deleteDelay()));
    animation->addAnimation(transformAnimation);

    addAnimation(animation);
}

void MBasicListItemDeletionAnimation::appendAfterTarget(MWidget *cell, const QPointF &targetDestination)
{
    Q_D(MBasicListItemDeletionAnimation);
    d->appendTargetWidget(cell);

    // Set target cell z value as the parent
    cell->setZValue(cell->parentItem()->zValue());

    // Position animation
    QPropertyAnimation *positionAnimation = d->createPositionAnimation(cell, targetDestination);
    positionAnimation->setDuration(style()->moveDuration());
    positionAnimation->setEasingCurve(style()->moveCurve());

    QSequentialAnimationGroup *animation = new QSequentialAnimationGroup(this);
    animation->addAnimation(d->createDelayAnimation(style()->moveDelay() + style()->deleteDelay()));
    animation->addAnimation(positionAnimation);

    addAnimation(animation);
}

M_REGISTER_ANIMATION(MBasicListItemDeletionAnimation)

#include "moc_mbasiclistitemdeletionanimation.cpp"
