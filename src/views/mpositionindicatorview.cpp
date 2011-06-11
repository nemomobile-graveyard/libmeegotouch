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

#include "mpositionindicatorview.h"
#include "mpositionindicatorview_p.h"
#include "qapplication.h"

#include <QString>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>

#include "mtheme.h"
#include "mviewcreator.h"
#include "mpositionindicator.h"
#include "mscalableimage.h"
#include "mdebug.h"

MPositionIndicatorViewPrivate::MPositionIndicatorViewPrivate()
    : controller(0),
      hideTimer(new QTimer()),
      visible(false),
      onDisplay(false)
{
}

MPositionIndicatorViewPrivate::~MPositionIndicatorViewPrivate()
{
    delete this->hideTimer;
}

void MPositionIndicatorViewPrivate::init(MPositionIndicator *controller)
{
    Q_Q(MPositionIndicatorView);

    this->controller = controller;
    hideTimer->setSingleShot(true);
    fadeAnimation = new QPropertyAnimation(controller, "opacity", q);

    q->connect(hideTimer, SIGNAL(timeout()), SLOT(hide()));
    q->connect(controller, SIGNAL(displayEntered()), SLOT(_q_displayEntered()));
    q->connect(controller, SIGNAL(displayExited()), SLOT(_q_displayExited()));
}

bool MPositionIndicatorViewPrivate::isInSwitcher() const
{
    if (const QGraphicsScene *const scene = controller->scene()) {
        const QList<QGraphicsView*> views = scene->views();
        if (views.isEmpty())
            return false;

        if (const MWindow *const win = qobject_cast<MWindow*>(views.first()))
            return win->isInSwitcher();
    }
    return false;
}

bool MPositionIndicatorViewPrivate::contentFullyVisible() const
{
    Q_Q(const MPositionIndicatorView);
    const QSizeF rangeSize = q->model()->range().size();
    return rangeSize.isNull();
}

void MPositionIndicatorViewPrivate::_q_displayEntered()
{
    Q_Q(MPositionIndicatorView);

    onDisplay = true;
    if (!isInSwitcher()) {
        hideTimer->start(q->style()->hideTimeout());
    }
}

void MPositionIndicatorViewPrivate::_q_displayExited()
{
    /* stop everything and keep indicator visible for next use */
    onDisplay = false;
    hideTimer->stop();
    fadeAnimation->stop();
    controller->setProperty("opacity", 1.0f);
    visible = true;
}

MPositionIndicatorView::MPositionIndicatorView(MPositionIndicator *controller) :
    MWidgetView(* new MPositionIndicatorViewPrivate, controller)
{
    Q_D(MPositionIndicatorView);
    d->init(controller);
}

MPositionIndicatorView::~MPositionIndicatorView()
{
    Q_D(MPositionIndicatorView);
    d->controller = 0;
}

void MPositionIndicatorView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MPositionIndicatorView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);
    Q_D(const MPositionIndicatorView);

    if (d->isInSwitcher()) {
        return;
    }

    const MPositionIndicatorModel *const activeModel = model();
    QSizeF  vpSize = activeModel->viewportSize();
    const QSizeF currentSize = size();
    QRectF  pRange = activeModel->range().adjusted(0, 0, vpSize.width(), vpSize.height());
    QPointF pPos   = activeModel->position();
    const MPositionIndicatorStyle *const activeStyle = style().operator->();

    if ((int)pRange.height() > (int)vpSize.height()) {
        const MScalableImage *indicator = activeStyle->indicatorImage();
        if (!indicator) {
            mWarning("MPositionIndicatorView") << "could not get \"indicator-image\"";
            return;
        }

        qreal distanceK = pPos.y() / pRange.height();
        qreal sizeK = vpSize.height() / pRange.height();
        qreal railHeight = currentSize.height();

        const MScalableImage *rail = activeStyle->backgroundImage();

        int indicatorPixmapSizeX = indicator->pixmap()->width();

        int indicatorHeight = sizeK * railHeight;
        if (activeStyle->minIndicatorSize() > indicatorHeight) {
            railHeight -= (activeStyle->minIndicatorSize() - indicatorHeight);
            indicatorHeight = activeStyle->minIndicatorSize();
        }

        int indicatorPositionY = distanceK * railHeight;

        if (indicatorPositionY + indicatorHeight >= int(currentSize.height())) {
            indicatorHeight = currentSize.height() - indicatorPositionY;
            indicatorPositionY -= 1;
        }

        if (indicatorPositionY < 0) {
            indicatorHeight += indicatorPositionY;
            indicatorPositionY = 0;
        }

        if (rail) {
            int railPixmapSizeX = rail->pixmap()->width();
            int railPositionX = (qApp->layoutDirection() == Qt::RightToLeft ? 0 : currentSize.width() - railPixmapSizeX);

            rail->draw((qreal)railPositionX,
                        0.0,
                        (qreal)railPixmapSizeX,
                        currentSize.height(),
                        painter);
        }

        int indicatorPositionX = (qApp->layoutDirection() == Qt::RightToLeft ? 0 : currentSize.width() - indicatorPixmapSizeX);

        indicator->draw(indicatorPositionX,
                        indicatorPositionY,
                        indicatorPixmapSizeX,
                        indicatorHeight,
                        painter);
    }

    if ((int)pRange.width() > (int)vpSize.width()) {
        const MScalableImage *indicator = activeStyle->indicatorImageHorizontal();
        if (!indicator) {
            mWarning("MPositionIndicatorView") << "could not get \"indicator-image-horizontal\"";
            return;
        }

        const MScalableImage *rail = activeStyle->backgroundImageHorizontal();

        int indicatorPixmapSizeY = indicator->pixmap()->height();
        int indicatorWidth = qMax(activeStyle->minIndicatorSize(), int((vpSize.width()/pRange.width())*size().width()));
        int indicatorPositionX = (pPos.x()/pRange.width())*size().width();

        if (indicatorPositionX + indicatorWidth > currentSize.width()) {
            indicatorWidth = currentSize.width() - indicatorPositionX;
        }

        if (indicatorPositionX < 0) {
            indicatorWidth += indicatorPositionX;
            indicatorPositionX = 0;
        }

        if (rail) {
            int railPixmapSizeY = rail->pixmap()->height();

            rail->draw( 0.0,
                        currentSize.height() - railPixmapSizeY,
                        currentSize.width(),
                        (qreal)railPixmapSizeY,
                        painter);
        }

        indicator->draw((qreal)indicatorPositionX,
                        currentSize.height() - indicatorPixmapSizeY,
                        (qreal)indicatorWidth,
                        (qreal)indicatorPixmapSizeY,
                        painter);
    }
}

void MPositionIndicatorView::updateData(const QList<const char *>& modifications)
{
    Q_D(MPositionIndicatorView);

    MWidgetView::updateData(modifications);

    if (d->onDisplay) {
        resetHideTimer();
    }
    update();
}

void MPositionIndicatorView::setupModel()
{
    Q_D(MPositionIndicatorView);

    MWidgetView::setupModel();

    if (d->onDisplay) {
        resetHideTimer();
    }
    update();
}

void MPositionIndicatorView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);
    update();
}

void MPositionIndicatorView::changeEvent(QEvent *event)
{
    Q_D(MPositionIndicatorView);

    MWidgetView::changeEvent(event);

    if (event->type() == QEvent::EnabledChange) {
        if (d->controller->isEnabled()) {
            resetHideTimer();
            d->controller->setVisible(true);
        } else {
            d->controller->setVisible(false);
        }
    }
}

void MPositionIndicatorView::hide()
{
    Q_D(MPositionIndicatorView);
    d->visible = false;
    d->fadeAnimation->stop();
    d->fadeAnimation->setEndValue(0.0f);

    // Only start the animation if the content is partly visible, otherwise
    // the position indicator won't be drawn at all (see drawContents())
    if (!d->contentFullyVisible()) {
        d->fadeAnimation->start();
        update();
    }
}

void MPositionIndicatorView::resetHideTimer()
{
    Q_D(MPositionIndicatorView);
    if (!d->visible) {
        d->fadeAnimation->stop();
        d->fadeAnimation->setEndValue(1.0f);

        // Only start the animation if the content is partly visible, otherwise
        // the position indicator won't be drawn at all (see drawContents())
        if (!d->contentFullyVisible()) {
            d->fadeAnimation->start();
        }
        d->visible = true;
    }
    d->hideTimer->stop();
    if (!d->isInSwitcher()) {
        d->hideTimer->start(style()->hideTimeout());
    }
}

M_REGISTER_VIEW_NEW(MPositionIndicatorView, MPositionIndicator)

#include "moc_mpositionindicatorview.cpp"
