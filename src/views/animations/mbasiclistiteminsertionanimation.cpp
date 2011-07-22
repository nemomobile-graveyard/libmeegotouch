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

#include "mbasiclistiteminsertionanimation.h"
#include "mbasiclistiteminsertionanimation_p.h"
#include "manimationcreator.h"

#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QDebug>

MBasicListItemInsertionAnimationPrivate::MBasicListItemInsertionAnimationPrivate()
    : q_ptr(0)
{
}

MBasicListItemInsertionAnimationPrivate::~MBasicListItemInsertionAnimationPrivate()
{
}

void MBasicListItemInsertionAnimationPrivate::_q_resetAnimation()
{
    Q_Q(MBasicListItemInsertionAnimation);

    resetTargetWidgets();
    q->clear();
}

void MBasicListItemInsertionAnimationPrivate::appendTargetWidget(MWidget *cell)
{
    targets.insert(new QWeakPointer<MWidget>(cell), TargetDefaultProperties(cell->zValue(), cell->opacity(), cell->scale(), cell->isVisible()));
}

void MBasicListItemInsertionAnimationPrivate::resetTargetWidgets()
{
    foreach (QWeakPointer<MWidget> *cell, targets.keys()) {
        if (!cell->isNull()) {
            TargetDefaultProperties properties = targets.value(cell);
            cell->data()->setZValue(properties.zValue);
            cell->data()->setOpacity(properties.opacity);
            cell->data()->setScale(properties.scale);
            cell->data()->setVisible(properties.visible);
        }
    }
    qDeleteAll(targets.keys());
    targets.clear();
}

QPropertyAnimation *MBasicListItemInsertionAnimationPrivate::createOpacityAnimation(MWidget *cell)
{
    QPropertyAnimation *opacityAnimation = new QPropertyAnimation;
    opacityAnimation->setTargetObject(cell);
    opacityAnimation->setPropertyName("opacity");
    opacityAnimation->setStartValue(cell->opacity());

    return opacityAnimation;
}

QPropertyAnimation *MBasicListItemInsertionAnimationPrivate::createScaleAnimation(MWidget *cell)
{
    QPropertyAnimation *scaleAnimation = new QPropertyAnimation;
    scaleAnimation->setTargetObject(cell);
    scaleAnimation->setPropertyName("scale");
    scaleAnimation->setStartValue(cell->scale());

    return scaleAnimation;
}

QPropertyAnimation *MBasicListItemInsertionAnimationPrivate::createPositionAnimation(MWidget *cell, const QPointF &targetPos)
{
    QPropertyAnimation *moveAnimation = new QPropertyAnimation;
    moveAnimation->setTargetObject(cell);
    moveAnimation->setPropertyName("pos");
    moveAnimation->setStartValue(cell->pos());
    moveAnimation->setEndValue(targetPos);

    return moveAnimation;
}

QPauseAnimation *MBasicListItemInsertionAnimationPrivate::createDelayAnimation(int delay)
{
    QPauseAnimation *delayAnimation = new QPauseAnimation();
    delayAnimation->setDuration(delay);
    return delayAnimation;
}

MBasicListItemInsertionAnimation::MBasicListItemInsertionAnimation(QObject *parent)
    : MParallelAnimationGroup(parent),
    d_ptr(new MBasicListItemInsertionAnimationPrivate)
{
    d_ptr->q_ptr = this;
    connect(this, SIGNAL(finished()), SLOT(_q_resetAnimation()));
}

MBasicListItemInsertionAnimation::~MBasicListItemInsertionAnimation()
{
    Q_D(MBasicListItemInsertionAnimation);
    d->resetTargetWidgets();
    clear();

    delete d_ptr;
}

void MBasicListItemInsertionAnimation::appendBeforeTarget(MWidget *cell)
{
    Q_D(MBasicListItemInsertionAnimation);
    d->appendTargetWidget(cell);
}

void MBasicListItemInsertionAnimation::appendInsertTarget(MWidget *cell)
{
    Q_D(MBasicListItemInsertionAnimation);
    d->appendTargetWidget(cell);

    // Set target cell z value above the parent
    cell->setZValue(cell->parentItem()->zValue() + 1);
    cell->setTransformOriginPoint(cell->boundingRect().center());

    qreal cellOpacity = cell->opacity();
    cell->setOpacity(style()->insertOpacity());

    // Opacity animation
    QPropertyAnimation *opacityAnimation = d->createOpacityAnimation(cell);
    opacityAnimation->setDuration(style()->insertDuration());
    opacityAnimation->setStartValue(style()->insertOpacity());
    opacityAnimation->setEndValue(cellOpacity);
    opacityAnimation->setEasingCurve(style()->insertCurve());

    qreal cellScale = cell->scale();
    cell->setScale(style()->insertScale());

    // Scale animation
    QPropertyAnimation *scaleAnimation = d->createScaleAnimation(cell);
    scaleAnimation->setDuration(style()->insertDuration());
    scaleAnimation->setStartValue(style()->insertScale());
    scaleAnimation->setEndValue(cellScale);
    scaleAnimation->setEasingCurve(style()->insertCurve());

    QParallelAnimationGroup *transformAnimation = new QParallelAnimationGroup(this);
    transformAnimation->addAnimation(opacityAnimation);
    transformAnimation->addAnimation(scaleAnimation);

    // Animation sequence
    QSequentialAnimationGroup *animation = new QSequentialAnimationGroup(this);
    animation->addAnimation(d->createDelayAnimation(style()->insertDelay()));
    animation->addAnimation(transformAnimation);

    addAnimation(animation);
}

void MBasicListItemInsertionAnimation::appendAfterTarget(MWidget *cell, const QPointF &targetDestination)
{
    Q_D(MBasicListItemInsertionAnimation);
    d->appendTargetWidget(cell);

    // Set target cell z value as the parent
    cell->setZValue(cell->parentItem()->zValue());

    // Position animation
    QPropertyAnimation *positionAnimation = d->createPositionAnimation(cell, targetDestination);
    positionAnimation->setDuration(style()->moveDuration());
    positionAnimation->setEasingCurve(style()->moveCurve());

    QSequentialAnimationGroup *animation = new QSequentialAnimationGroup(this);
    animation->addAnimation(d->createDelayAnimation(style()->moveDelay() + style()->insertDelay()));
    animation->addAnimation(positionAnimation);

    addAnimation(animation);
}

M_REGISTER_ANIMATION(MBasicListItemInsertionAnimation)

#include "moc_mbasiclistiteminsertionanimation.cpp"
