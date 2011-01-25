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

#include <QPainter>
#include <QPropertyAnimation>

#include "mspinnerview.h"
#include "mspinnerview_p.h"

#include "mprogressindicator.h"

#include "mtheme.h"

#define ANIMATION_SLOWDOWN_RATIO 4

MSpinnerViewPrivate::MSpinnerViewPrivate()
    :  q_ptr(NULL),
       controller(NULL),
       positionAnimation(NULL),
       currentFrame(0)
{
}

MSpinnerViewPrivate::~MSpinnerViewPrivate()
{
    delete positionAnimation;
    releaseUsedPixmaps();
}

QPropertyAnimation *MSpinnerViewPrivate::createAnimation()
{
    Q_Q(MSpinnerView);

    QPropertyAnimation *animation = new QPropertyAnimation(q, "currentFrame", q);
    animation->setLoopCount(-1);

    return animation;
}

void MSpinnerViewPrivate::refreshStyle()
{
    Q_Q(MSpinnerView);

    const MWindow *mWindow = getMWindow();

    if (mWindow && mWindow->isInSwitcher()) {
        positionAnimation->setDuration(q->style()->period() * ANIMATION_SLOWDOWN_RATIO);
    } else {
        positionAnimation->setDuration(q->style()->period());
    }
    positionAnimation->setStartValue(0);
    positionAnimation->setEndValue(q->style()->numberOfFrames());
    reloadFrames();
}

void MSpinnerViewPrivate::refreshModel()
{
    Q_Q(MSpinnerView);

    if (q->model()->unknownDuration()) {
        if (positionAnimation->state() == QPropertyAnimation::Paused)
            positionAnimation->resume();
        else
            positionAnimation->start();
    } else {
        if (positionAnimation->state() == QPropertyAnimation::Running)
            positionAnimation->pause();
    }
}

const MWindow* MSpinnerViewPrivate::getMWindow()
{
    QGraphicsScene *scene = controller->scene();

    if (!scene || scene->views().isEmpty())
        return 0;

    return qobject_cast<MWindow*>(scene->views().at(0));
}

void MSpinnerViewPrivate::reloadFrames()
{
    Q_Q(MSpinnerView);

    releaseUsedPixmaps();
    // Starts from 1 because that's the agreed convention
    // for the image file names
    for (int i = 1; i <= q->style()->numberOfFrames(); i++) {
        QString frame_name = QString("%1_%2_%3").arg(q->style()->baseImageName())\
                                                .arg(q->style()->baseImageSize())\
                                                .arg(i);
        const QPixmap *p = MTheme::pixmap(frame_name);
        animationPixmaps << p;
    }
}

void MSpinnerViewPrivate::releaseUsedPixmaps()
{
    foreach(const QPixmap *p, animationPixmaps)
        MTheme::releasePixmap(p);
    animationPixmaps.clear();
}

void MSpinnerViewPrivate::_q_resumeAnimation()
{
    Q_Q(MSpinnerView);

    if (q->model()->unknownDuration()) {
        if (positionAnimation->state() == QPropertyAnimation::Paused)
            positionAnimation->resume();
    }
}

void MSpinnerViewPrivate::_q_pauseAnimation()
{
    if (positionAnimation->state() == QPropertyAnimation::Running)
        positionAnimation->pause();
}

void MSpinnerViewPrivate::_q_pauseOrResumeAnimation()
{
    Q_Q(MSpinnerView);

    if (controller->isVisible() && controller->isOnDisplay()) {
        const MWindow *mWindow = getMWindow();
        if (mWindow) {
            q->connect(mWindow, SIGNAL(switcherEntered()), SLOT(_q_switcherEntered()), Qt::UniqueConnection);
            q->connect(mWindow, SIGNAL(switcherExited()), SLOT(_q_switcherExited()), Qt::UniqueConnection);
            if (mWindow->isInSwitcher()) {
                _q_switcherEntered();
            }
        }
        _q_resumeAnimation();
    } else {
        _q_pauseAnimation();
    }
}

void MSpinnerViewPrivate::_q_switcherEntered()
{
    Q_Q(MSpinnerView);

    positionAnimation->setDuration(q->style()->period() * ANIMATION_SLOWDOWN_RATIO);
}

void MSpinnerViewPrivate::_q_switcherExited()
{
    Q_Q(MSpinnerView);

    positionAnimation->setDuration(q->style()->period());
}

MSpinnerView::MSpinnerView(MProgressIndicator *controller) :
    MWidgetView(controller),
    d_ptr(new MSpinnerViewPrivate)
{
    Q_D(MSpinnerView);
    d->q_ptr = this;
    d->controller = controller;
    d->positionAnimation = d->createAnimation();

    connect(controller, SIGNAL(visibleChanged()), this, SLOT(_q_pauseOrResumeAnimation()));

    connect(controller, SIGNAL(displayEntered()), this, SLOT(_q_pauseOrResumeAnimation()));
    connect(controller, SIGNAL(displayExited()), this, SLOT(_q_pauseOrResumeAnimation()));
}


MSpinnerView::~MSpinnerView()
{
    delete d_ptr;
}

int MSpinnerView::currentFrame() const
{
    Q_D(const MSpinnerView);

    return d->currentFrame;
}

void MSpinnerView::setCurrentFrame(int currentFrame)
{
    Q_D(MSpinnerView);

    if (d->currentFrame == currentFrame)
        return;

    if (currentFrame >= d->animationPixmaps.length())
        currentFrame = 0;

    d->currentFrame = currentFrame;
    update();
}

void MSpinnerView::updateData(const QList<const char *>& modifications)
{
    Q_D(MSpinnerView);
    MWidgetView::updateData(modifications);

    foreach(const char * member, modifications) {
        if (member == MProgressIndicatorModel::UnknownDuration)
            d->refreshModel();
    }
    update();
}

void MSpinnerView::applyStyle()
{
    Q_D(MSpinnerView);
    MWidgetView::applyStyle();

    d->refreshStyle();
}

void MSpinnerView::setupModel()
{
    Q_D(MSpinnerView);
    MWidgetView::setupModel();

    d->refreshModel();
}

void MSpinnerView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MSpinnerView);

    if (d->animationPixmaps.isEmpty())
        return;

    const QPixmap *p = d->animationPixmaps[d->currentFrame];
    if (p && !p->isNull())
        painter->drawPixmap(QRectF(QPointF(0, 0), size()), *p, QRectF(p->rect()));
}

#include "moc_mspinnerview.cpp"

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(MSpinnerView, MProgressIndicator)
