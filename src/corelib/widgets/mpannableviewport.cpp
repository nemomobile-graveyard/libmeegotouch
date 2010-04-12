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

#include <QGraphicsLinearLayout>
#include <QObject>
#include <QRectF>
#include <math.h>

#include "mpositionindicator.h"
#include "mpannableviewport.h"
#include "mpannableviewport_p.h"
#include "mpannableviewportlayout.h"
#include <mscenemanager.h>
#include <mondisplaychangeevent.h>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MPannableViewport)

namespace
{
    //! Z-value of the panned widget
    const int ZValuePannedWidget = 0;
    //! Z-value of the position indicator
    const int ZValuePosInd       = 1;
}

MPannableViewportPrivate::MPannableViewportPrivate()
    : MPannableWidgetPrivate(),
      currentRange(QRectF()),
      pannedWidget(0),
      viewportLayout(0),
      positionIndicator(0)
{
}

MPannableViewportPrivate::~MPannableViewportPrivate()
{
}


void MPannableViewportPrivate::setNewRange(const QRectF &newRange)
{
    Q_Q(MPannableViewport);

    if (currentRange != newRange) {
        currentRange = newRange;
        emit q->rangeChanged(currentRange);
    }
}


void MPannableViewportPrivate::recalculatePhysRange()
{
    Q_Q(MPannableViewport);

    // Recalculates the range of the physics. Takes into consideration
    // the current size of the viewport and the current range

    QSizeF physicsRangeSize = currentRange.size() - q->size();

    if (physicsRangeSize.width() < 0.0) {
        physicsRangeSize.setWidth(0.0);
    }

    if (physicsRangeSize.height() < 0.0) {
        physicsRangeSize.setHeight(0.0);
    }

    q->physics()->setRange(QRectF(currentRange.topLeft(), physicsRangeSize));

}

void MPannableViewportPrivate::sendOnDisplayChangeEventToMWidgets(QGraphicsItem *item,
        MOnDisplayChangeEvent *event)
{
    Q_Q(MPannableViewport);
    bool isMWidget = false;

    if (item->isWidget()) {
        MWidget *mWidget = qobject_cast<MWidget *>(static_cast<QGraphicsWidget *>(item));

        if (mWidget) {
            isMWidget = true;

            // Let that MWidget handle the propagation from now on.
            q->scene()->sendEvent(mWidget, event);
        }
    }

    if (!isMWidget) {
        // That guy won't send the word forward. So we have to do it for him.
        QList<QGraphicsItem *> childItemsList = item->childItems();
        int childItemsCount = childItemsList.count();

        for (int i = 0; i < childItemsCount; i++) {
            sendOnDisplayChangeEventToMWidgets(childItemsList.at(i), event);
        }
    }
}

void MPannableViewportPrivate::_q_resolvePannedWidgetIsOnDisplay()
{
    Q_Q(MPannableViewport);
    MOnDisplayChangeEvent *event = 0;

    if (q->isOnDisplay() && q->sceneManager() != 0) {
        QRectF visibleSceneRect(QPoint(0, 0), q->sceneManager()->visibleSceneSize());
        event = new MOnDisplayChangeEvent(MOnDisplayChangeEvent::MustBeResolved, visibleSceneRect);

        // If I just do a send(q->widget(), event) and widget() happens not to be a MWidget
        // (e.g., plain QGraphicsWidget layouting MWidgets inside) the event propagation
        // just won't happen.
        sendOnDisplayChangeEventToMWidgets(q->widget(), event);
    }
}

void MPannableViewportPrivate::_q_positionIndicatorEnabledChanged()
{
    Q_Q(MPannableViewport);

    if (positionIndicator->isEnabled()) {
        q->connect(q,
                   SIGNAL(viewportSizeChanged(QSizeF)),
                   positionIndicator,
                   SLOT(setViewportSize(QSizeF)), Qt::UniqueConnection);
        q->connect(q,
                   SIGNAL(positionChanged(QPointF)),
                   positionIndicator,
                   SLOT(setPosition(QPointF)), Qt::UniqueConnection);
        q->connect(q,
                   SIGNAL(rangeChanged(QRectF)),
                   positionIndicator,
                   SLOT(setRange(QRectF)), Qt::UniqueConnection);
    } else {
        q->disconnect(q,
                      SIGNAL(viewportSizeChanged(QSizeF)),
                      positionIndicator,
                      SLOT(setViewportSize(QSizeF)));
        q->disconnect(q,
                      SIGNAL(positionChanged(QPointF)),
                      positionIndicator,
                      SLOT(setPosition(QPointF)));
        q->disconnect(q,
                      SIGNAL(rangeChanged(QRectF)),
                      positionIndicator,
                      SLOT(setRange(QRectF)));
    }
}

MPannableViewport::MPannableViewport(QGraphicsItem *parent)
    : MPannableWidget(new MPannableViewportPrivate(), new MPannableViewportModel, parent)
{
    Q_D(MPannableViewport);
    setFlags(QGraphicsItem::ItemClipsChildrenToShape);

    setPosition(QPointF());
    setRange(QRectF());

    d->positionIndicator = new MPositionIndicator(this);
    d->positionIndicator->setZValue(ZValuePosInd);
    d->_q_positionIndicatorEnabledChanged();

    connect(d->positionIndicator,
            SIGNAL(enabledChanged()),
            SLOT(_q_positionIndicatorEnabledChanged()));


    d->viewportLayout = new MPannableViewportLayout;
    d->viewportLayout->setPanningDirections(panDirection());
    d->viewportLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(d->viewportLayout);

    connect(this,
            SIGNAL(panningStopped()),
            SLOT(_q_resolvePannedWidgetIsOnDisplay()));
}

MPannableViewport::~MPannableViewport()
{
    QGraphicsWidget *oldwidget  = widget();
    setWidget(0);
    if (oldwidget) {
        delete oldwidget;
        oldwidget = 0;
    }
}

void MPannableViewport::setAutoRange(bool enable)
{
    Q_D(MPannableViewport);

    model()->setAutoRange(enable);

    if (enable) {
        if (d->pannedWidget) {
            d->setNewRange(QRectF(QPointF(), d->pannedWidget->size()));
        } else {
            d->setNewRange(QRectF());
        }

        // Recalculates the physics range when automatic range is taken
        // into use
        d->recalculatePhysRange();
    }
}

bool MPannableViewport::autoRange() const
{
    return model()->autoRange();
}

void MPannableViewport::setClipping(bool enabled)
{
    model()->setClipWidget(enabled);

    if (enabled) {
        setFlags(QGraphicsItem::ItemClipsChildrenToShape);
    } else {
        setFlags(0);
    }
}

bool MPannableViewport::hasClipping() const
{
    return model()->clipWidget();
}

void MPannableViewport::setWidget(QGraphicsWidget *widget)
{
    Q_D(MPannableViewport);

    d->pannedWidget = widget;
    d->viewportLayout->setWidget(widget);

    if (widget) {
        widget->setPos(-position());
        widget->setZValue(ZValuePannedWidget);
    }

    if (autoRange()) {
        if (widget) {
            d->setNewRange(QRectF(QPointF(), widget->size()));
        } else {
            d->setNewRange(QRectF());
        }
    }

    // Recalculates the physics range for the new panned widget
    d->recalculatePhysRange();
}

QGraphicsWidget *MPannableViewport::widget() const
{
    Q_D(const MPannableViewport);
    return d->pannedWidget;
}

void MPannableViewport::setRange(const QRectF &r)
{
    Q_D(MPannableViewport);

    if (!autoRange()) {
        d->setNewRange(r);

        // Recalculates the physics range for new manually set range
        d->recalculatePhysRange();
    }
}

QRectF MPannableViewport::range() const
{
    Q_D(const MPannableViewport);

    return d->currentRange;
}

void MPannableViewport::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MPannableViewport);
    Q_UNUSED(event);

    if (autoRange()) {
        if (d->pannedWidget) {
            d->setNewRange(QRectF(QPointF(), d->pannedWidget->size()));
        } else {
            d->setNewRange(QRectF());
        }
    }

    // Recalculates the physics range because viewport size has changed
    d->recalculatePhysRange();

    emit viewportSizeChanged(event->newSize());

    d->positionIndicator->resize(event->newSize());
}

void MPannableViewport::updatePosition(const QPointF &p)
{
    Q_D(MPannableViewport);

    QPointF roundedP = QPointF(floor(p.x()), floor(p.y()));

    // Parameter p is in the local coordinate system of pannedWidget but
    // pannedWidget position is in pannable viewport's coordinate system.
    //
    // Therefore pannedWidget position is set as a negate of physics position (p),
    // because the panned widget needs to flow to the opposite direction of the
    // panning.

    if (d->pannedWidget && d->pannedWidget->pos() != -roundedP) {
        d->pannedWidget->setPos(-roundedP);
        emit positionChanged(roundedP);
    }
}

void MPannableViewport::setPanDirection(const Qt::Orientations &panDirection)
{
    Q_D(MPannableViewport);

    MPannableWidget::setPanDirection(panDirection);

    if (d->viewportLayout) {
        d->viewportLayout->setPanningDirections(panDirection);
    }

    updateGeometry();
}

void MPannableViewport::updateGeometry()
{
    Q_D(MPannableViewport);

    if (autoRange()) {
        if (d->pannedWidget) {
            d->setNewRange(QRectF(QPointF(), d->pannedWidget->size()));
        } else {
            d->setNewRange(QRectF());
        }
    }

    // Recalculates the physics range because panned widget size has changed
    d->recalculatePhysRange();
    updatePosition(position());

    MPannableWidget::updateGeometry();
}

void MPannableViewport::setPositionIndicator(MPositionIndicator *positionIndicator)
{
    Q_D(MPannableViewport);

    if (!positionIndicator) {
        return;
    }

    delete d->positionIndicator;

    d->positionIndicator = positionIndicator;
    d->positionIndicator->setZValue(ZValuePosInd);
    d->positionIndicator->setParent(this);
    d->positionIndicator->setParentItem(this);

    d->_q_positionIndicatorEnabledChanged();

    connect(d->positionIndicator,
            SIGNAL(enabledChanged()),
            SLOT(_q_positionIndicatorEnabledChanged()));
}

MPositionIndicator* MPannableViewport::positionIndicator() const
{
    Q_D(const MPannableViewport);

    return d->positionIndicator;
}

#include "moc_mpannableviewport.cpp"
