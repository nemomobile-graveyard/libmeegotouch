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
#include "mcontentfadeandslideanimation.h"
#include "mcontentfadeandslideanimation_p.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>

namespace {
    const QPointF farFarAway(-100000, -100000);
}

void SnapshotItem::updateSnapshot(QGraphicsWidget* target)
{
    QSize newSize = target->sceneTransform().mapRect(target->boundingRect()).size().toSize();

    if (newSize != pixmap.size())
        pixmap = QPixmap(newSize);

    if (pixmap.isNull())
        return;

    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

    if (!painter.isActive())
        return;

    const QPointF oldPos(target->pos());
    // move target far away so nothing is below
    target->setPos(farFarAway);
    target->scene()->render(&painter, QRectF(), target->sceneBoundingRect());
    target->setPos(oldPos);
}

QRectF SnapshotItem::boundingRect() const
{
    return QRectF(0, 0, pixmap.width(), pixmap.height());
}

void SnapshotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->drawPixmap(QPoint(0, 0), pixmap);
}


MContentFadeAndSlideAnimationPrivate::MContentFadeAndSlideAnimationPrivate()
    : snapshotItem(0)
    , contentItem(0)
    , previous(0)
    , fadeOut(0)
    , slideOut(0)
    , current(0)
    , fadeIn(0)
    , slideIn(0)
    , currentWithDelay(0)
    , delay(0)
{
}

void MContentFadeAndSlideAnimationPrivate::init()
{
    Q_Q(MContentFadeAndSlideAnimation);

    snapshotItem = new SnapshotItem;

    previous = new QParallelAnimationGroup();

    fadeOut = new QPropertyAnimation();
    fadeOut->setPropertyName("opacity");
    fadeOut->setTargetObject(snapshotItem);
    previous->addAnimation(fadeOut);

    slideOut = new QPropertyAnimation();
    slideOut->setPropertyName("pos");
    slideOut->setTargetObject(snapshotItem);
    previous->addAnimation(slideOut);

    current = new QParallelAnimationGroup();

    fadeIn = new QPropertyAnimation();
    fadeIn->setPropertyName("opacity");
    current->addAnimation(fadeIn);

    slideIn = new QPropertyAnimation();
    slideIn->setPropertyName("pos");
    current->addAnimation(slideIn);

    currentWithDelay = new QSequentialAnimationGroup();
    delay = new QPauseAnimation;

    currentWithDelay->addAnimation(delay);
    currentWithDelay->addAnimation(current);

    q->addAnimation(previous);
    q->addAnimation(currentWithDelay);
}

void MContentFadeAndSlideAnimationPrivate::findContentItem()
{
    Q_Q(MContentFadeAndSlideAnimation);

    contentItem = 0;

    if (!targetWidget || q->style()->contentObjectName().isEmpty())
        return;

    foreach(QGraphicsItem* item, targetWidget->childItems()) {
        if (!item->isWidget())
            continue;
        QGraphicsWidget *widget = static_cast<QGraphicsWidget*>(item);
        if (widget->objectName() == q->style()->contentObjectName()) {
            contentItem = widget;
            break;
        }
    }
}


MContentFadeAndSlideAnimation::MContentFadeAndSlideAnimation(QObject *parent) :
    MAbstractWidgetAnimation(new MContentFadeAndSlideAnimationPrivate, parent)
{
    Q_D(MContentFadeAndSlideAnimation);

    d->init();
}

MContentFadeAndSlideAnimation::~MContentFadeAndSlideAnimation()
{
    Q_D(MContentFadeAndSlideAnimation);

    delete d->snapshotItem;
}

void MContentFadeAndSlideAnimation::restoreTargetWidgetState()
{}

void MContentFadeAndSlideAnimation::setTransitionDirection(MAbstractWidgetAnimation::TransitionDirection direction)
{
    Q_D(MContentFadeAndSlideAnimation);

    if (direction == MAbstractWidgetAnimation::In)
        style().setObjectName("In");
    else
        style().setObjectName("Out");

    // contentObjectName might change so we must search for content again
    d->findContentItem();
}

void MContentFadeAndSlideAnimation::takeContentSnapshot()
{
    Q_D(MContentFadeAndSlideAnimation);

    if (d->contentItem)
        d->snapshotItem->updateSnapshot(d->contentItem);
}

void MContentFadeAndSlideAnimation::setTargetWidget(MWidgetController *targetWidget)
{
    Q_D(MContentFadeAndSlideAnimation);

    MAbstractWidgetAnimation::setTargetWidget(targetWidget);

    d->findContentItem();
}

void MContentFadeAndSlideAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MContentFadeAndSlideAnimation);

    if (!d->contentItem)
        return;

    if (oldState == QAbstractAnimation::Stopped &&
        newState == QAbstractAnimation::Running)
    {
        d->fadeOut->setStartValue(1.0);
        d->fadeOut->setEndValue(style()->fadeOutOpacity());
        d->fadeOut->setDuration(style()->fadeOutDuration());
        d->fadeOut->setEasingCurve(style()->fadeOutEasingCurve());
        d->fadeOut->setTargetObject(d->snapshotItem);

        QRectF startRect = d->contentItem->boundingRect();
        QRectF finalRect = startRect.translated(style()->slideOutDisplacement());
        QTransform transform = d->contentItem->sceneTransform();
        d->slideOut->setStartValue(transform.mapRect(startRect).topLeft());
        d->slideOut->setEndValue(transform.mapRect(finalRect).topLeft());
        d->slideOut->setDuration(style()->slideOutDuration());
        d->slideOut->setEasingCurve(style()->slideOutEasingCurve());
        d->slideOut->setTargetObject(d->snapshotItem);

        d->delay->setDuration(style()->delay());

        d->fadeIn->setStartValue(style()->fadeInOpacity());
        d->fadeIn->setEndValue(d->contentItem->opacity());
        d->fadeIn->setDuration(style()->fadeInDuration());
        d->fadeIn->setEasingCurve(style()->fadeInEasingCurve());
        d->fadeIn->setTargetObject(d->contentItem);

        d->slideIn->setStartValue(d->contentItem->pos() + style()->slideInDisplacement());
        d->slideIn->setEndValue(d->contentItem->pos());
        d->slideIn->setDuration(style()->slideInDuration());
        d->slideIn->setEasingCurve(style()->slideInEasingCurve());
        d->slideIn->setTargetObject(d->contentItem);

        d->targetWidget->scene()->addItem(d->snapshotItem);

        // animation is delayed so apply initial values now
        d->contentItem->setOpacity(d->fadeIn->startValue().toReal());
        d->contentItem->setPos(d->slideIn->startValue().toPointF());

    } else if (oldState == QAbstractAnimation::Running &&
               newState == QAbstractAnimation::Stopped)
    {
        d->targetWidget->scene()->removeItem(d->snapshotItem);

        // apply final values (for cases when animation was stopped in the middle)
        d->contentItem->setOpacity(d->fadeIn->endValue().toReal());
        d->contentItem->setPos(d->slideIn->endValue().toPointF());
    }

    MAbstractWidgetAnimation::updateState(newState, oldState);
}

