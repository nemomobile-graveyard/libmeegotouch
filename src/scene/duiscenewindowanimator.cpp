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

#include "duiscenewindowanimator.h"

#include <QTimeLine>
#include "duiscenelayereffect.h"
#include "duiscenewindow.h"
#include "duiscenemanager.h"
#include "duiscene.h"
#include "duiscenewindowanimatorstyle.h"
#include "duitheme.h"

class DuiSceneWindowAnimatorPrivate
{
    Q_DECLARE_PUBLIC(DuiSceneWindowAnimator)

public:
    void startTimeline(QTimeLine::Direction direction, int transitionDuration, bool transitionFade);
    bool fade;
    enum TransitionType {
        None,
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,
        MoveTo,
        Expand,
        Shrink
    };
    TransitionType transition;

    void setupWindow();
    void setupEffect(bool forward, const QString &transitionType);
protected:
    DuiSceneWindowAnimator *q_ptr;
private: // public for DuiSceneWindowAnimator
    DuiSceneWindow                 *window;
    DuiSceneLayerEffect            *effect;
    QTimeLine                       timeline;
    const DuiSceneWindowAnimatorStyle    *style;
    QString                         effectType;

    QPointF                         startPosition;
    QPointF                         endPosition;
};

void DuiSceneWindowAnimatorPrivate::startTimeline(QTimeLine::Direction direction, int transitionDuration, bool transitionFade)
{
    fade = transitionFade;

    timeline.setDirection(direction);
    timeline.setDuration(transitionDuration);
    timeline.setUpdateInterval(30);
    timeline.start();
}

DuiSceneWindowAnimator::DuiSceneWindowAnimator(DuiSceneWindow *window, DuiSceneLayerEffect *effect) :
    d_ptr(new DuiSceneWindowAnimatorPrivate)
{
    Q_D(DuiSceneWindowAnimator);
    d->window = window;
    d->effect = effect;
    d->style = NULL;
    d->q_ptr = this;

    DuiSceneWindow::WindowType windowType = window->windowType();
    switch (windowType) {
    case DuiSceneWindow::Dialog:
        d->effectType = "messagebox";
        break;
    case DuiSceneWindow::Notification:
        d->effectType = window->viewType();
        break;
    case DuiSceneWindow::PopupList:
        d->effectType = "popuplist";
        break;
    default:
        d->effectType = "";
        break;
    }

    connect(&d->timeline, SIGNAL(valueChanged(qreal)), this, SLOT(windowAnimating(qreal)));
    connect(&d->timeline, SIGNAL(finished()), this, SLOT(windowAnimationDone()));
}

DuiSceneWindowAnimator::~DuiSceneWindowAnimator()
{
    Q_D(DuiSceneWindowAnimator);
    DuiTheme::releaseStyle(d->style);
    delete d_ptr;
}

void DuiSceneWindowAnimator::updateStyle()
{
    Q_D(DuiSceneWindowAnimator);
    const DuiSceneWindowAnimatorStyle *style =
        dynamic_cast<const DuiSceneWindowAnimatorStyle *>(
            DuiTheme::style("DuiSceneWindowAnimatorStyle", QString(""), d->effectType, QString(""), Dui::Landscape));

    if (d->style != style) {
        DuiTheme::releaseStyle(d->style);
        d->style = style;
        styleUpdated();
    } else {
        DuiTheme::releaseStyle(style);
    }
}

void DuiSceneWindowAnimator::styleUpdated()
{
    Q_D(DuiSceneWindowAnimator);
    if (d->window) {
        // TODO: check if these are needed, when orientation change is revisited

        QSizeF size = d->window->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
        d->window->resize(size);
    }
    if (d->effect)
        d->effect->resize(DuiSceneManager::instance()->visibleSceneSize());
}

void DuiSceneWindowAnimator::windowAnimationDone()
{
    Q_D(DuiSceneWindowAnimator);

    if (d->timeline.direction() == QTimeLine::Forward) {
        emit windowAnimationCompleted(this, true);
    } else {
        if (d->window)
            d->window->hide();
        emit windowAnimationCompleted(this, false);
    }
}

void DuiSceneWindowAnimator::setWindow(DuiSceneWindow *window)
{
    Q_D(DuiSceneWindowAnimator);
    d->window = window;
}

DuiSceneWindow *DuiSceneWindowAnimator::window() const
{
    Q_D(const DuiSceneWindowAnimator);
    return d->window;
}

DuiSceneLayerEffect *DuiSceneWindowAnimator::effect() const
{
    Q_D(const DuiSceneWindowAnimator);
    return d->effect;
}

void DuiSceneWindowAnimator::forward()
{
    Q_D(DuiSceneWindowAnimator);

    d->setupWindow();
    d->setupEffect(true, d->style->transitionInType());

    if (d->effect)
        d->effect->setOpacity(0.0);

    d->startTimeline(QTimeLine::Forward, d->style->transitionInDuration(), d->style->transitionInFade());
}

void DuiSceneWindowAnimator::forwardNow()
{
    Q_D(DuiSceneWindowAnimator);
    d->setupWindow();
    d->setupEffect(true, d->style->transitionInType());
    d->timeline.setDirection(QTimeLine::Forward);
    windowAnimating(1.0);
    windowAnimationDone();
}

void DuiSceneWindowAnimator::backward()
{
    Q_D(DuiSceneWindowAnimator);

    d->setupWindow();
    d->setupEffect(false, d->style->transitionOutType());

    d->startTimeline(QTimeLine::Backward, d->style->transitionOutDuration(), d->style->transitionOutFade());
}

void DuiSceneWindowAnimator::backwardNow()
{
    Q_D(DuiSceneWindowAnimator);
    d->setupWindow();
    d->setupEffect(false, d->style->transitionOutType());
    d->timeline.setDirection(QTimeLine::Backward);
    windowAnimating(0.0);
    windowAnimationDone();
}

void DuiSceneWindowAnimatorPrivate::setupWindow()
{
    if (window) {
        QSizeF size = window->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
        window->resize(size);
    }
}

void DuiSceneWindowAnimatorPrivate::setupEffect(bool forward, const QString &transitionType)
{
    if (effect != NULL) {
        DuiSceneManager::instance()->scene()->setActiveWindow(effect);
        effect->resize(DuiSceneManager::instance()->visibleSceneSize());

        if (transitionType == "move.up") {
            transition = MoveUp;
            if (forward) {
                window->setPos(QPointF(window->pos().x(), DuiSceneManager::instance()->visibleSceneSize().height()));
            }
            endPosition = QPointF(window->pos().x(), window->pos().y() >= DuiSceneManager::instance()->visibleSceneSize().height() ? DuiSceneManager::instance()->visibleSceneSize().height() - window->size().height() : -window->size().height());
        } else if (transitionType == "move.down") {
            transition = MoveDown;
            if (forward) {
                window->setPos(QPointF(window->pos().x(), -window->size().height()));
            }
            endPosition = QPointF(window->pos().x(), window->pos().y() < 0 ? 0 : DuiSceneManager::instance()->visibleSceneSize().height());
        } else if (transitionType == "move.left") {
            transition = MoveLeft;
            if (forward) {
                window->setPos(QPointF(DuiSceneManager::instance()->visibleSceneSize().width(), window->pos().y()));
            }
            endPosition = QPointF(window->pos().x() >= DuiSceneManager::instance()->visibleSceneSize().width() ? DuiSceneManager::instance()->visibleSceneSize().width() - window->size().width() : -window->size().width(), window->pos().y());
        } else if (transitionType == "move.right") {
            transition = MoveRight;
            if (forward) {
                window->setPos(QPointF(-window->size().width(), window->pos().y()));
            }
            endPosition = QPointF(window->pos().x() < 0 ? 0 : DuiSceneManager::instance()->visibleSceneSize().width(), window->pos().y());
        } else if (transitionType == "move.to") {
            transition = MoveTo;
            endPosition = forward ? style->transitionInDestination() : style->transitionOutDestination();
        } else if (transitionType == "expand") {
            transition = Expand;
            endPosition = window->pos();
        } else if (transitionType == "shrink") {
            transition = Shrink;
            endPosition = window->pos();
        } else {
            transition = None;
            endPosition = window->pos();
        }

        startPosition = window->pos();
    }
}

void DuiSceneWindowAnimator::windowAnimating(qreal value)
{
    Q_D(DuiSceneWindowAnimator);

    if (d->effect) {
        if (value == 0)
            d->effect->setOpacity(0);
        else if (value == 1 || !d->fade)
            d->effect->setOpacity(1);
        else
            d->effect->setOpacity(value);
    }

    if (!d->window) return;

    switch (d->transition) {
    case DuiSceneWindowAnimatorPrivate::MoveUp:
    case DuiSceneWindowAnimatorPrivate::MoveDown:
    case DuiSceneWindowAnimatorPrivate::MoveLeft:
    case DuiSceneWindowAnimatorPrivate::MoveRight:
    case DuiSceneWindowAnimatorPrivate::MoveTo:
        if (d->timeline.direction() == QTimeLine::Forward) {
            d->window->setPos(d->startPosition.x() + (d->endPosition.x() - d->startPosition.x()) * value,
                              d->startPosition.y() + (d->endPosition.y() - d->startPosition.y()) * value);
        } else {
            d->window->setPos(d->startPosition.x() + (d->endPosition.x() - d->startPosition.x()) *(1 - value),
                              d->startPosition.y() + (d->endPosition.y() - d->startPosition.y()) *(1 - value));
        }
        break;
    case DuiSceneWindowAnimatorPrivate::Expand:
        if (d->timeline.direction() == QTimeLine::Forward) {
            QSizeF size = d->window->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
            d->window->resize(size.width(), size.height() * value);
        }
        break;
    case DuiSceneWindowAnimatorPrivate::Shrink:
        if (d->timeline.direction() == QTimeLine::Backward) {
            QSizeF size = d->window->effectiveSizeHint(Qt::PreferredSize, QSizeF(-1, -1));
            d->window->resize(size.width(), size.height() * value);
        }
        break;
    default:  //Do nothing
        break;
    }
}
