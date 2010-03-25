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

#include <QGraphicsLinearLayout>
#include <QObject>
#include <QRectF>
#include <math.h>

#include "duipositionindicator.h"
#include "duipannableviewport.h"
#include "duipannableviewport_p.h"
#include "duipannableviewportlayout.h"
#include <duiscenemanager.h>
#include <duiondisplaychangeevent.h>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiPannableViewport)

namespace
{
    //! Z-value of the panned widget
    const int ZValuePannedWidget = 0;
    //! Z-value of the position indicator
    const int ZValuePosInd       = 1;
}

DuiPannableViewportPrivate::DuiPannableViewportPrivate()
    : DuiPannableWidgetPrivate(),
      currentRange(QRectF()),
      pannedWidget(0),
      viewportLayout(0),
      positionIndicator(0)
{
}

DuiPannableViewportPrivate::~DuiPannableViewportPrivate()
{
}


void DuiPannableViewportPrivate::setNewRange(const QRectF &newRange)
{
    Q_Q(DuiPannableViewport);

    if (currentRange != newRange) {
        currentRange = newRange;
        emit q->rangeChanged(currentRange);
    }
}


void DuiPannableViewportPrivate::recalculatePhysRange()
{
    Q_Q(DuiPannableViewport);

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

void DuiPannableViewportPrivate::sendOnDisplayChangeEventToDuiWidgets(QGraphicsItem *item,
        DuiOnDisplayChangeEvent *event)
{
    Q_Q(DuiPannableViewport);
    bool isDuiWidget = false;

    if (item->isWidget()) {
        DuiWidget *duiWidget = qobject_cast<DuiWidget *>(static_cast<QGraphicsWidget *>(item));

        if (duiWidget) {
            isDuiWidget = true;

            // Let that DuiWidget handle the propagation from now on.
            q->scene()->sendEvent(duiWidget, event);
        }
    }

    if (!isDuiWidget) {
        // That guy won't send the word forward. So we have to do it for him.
        QList<QGraphicsItem *> childItemsList = item->childItems();
        int childItemsCount = childItemsList.count();

        for (int i = 0; i < childItemsCount; i++) {
            sendOnDisplayChangeEventToDuiWidgets(childItemsList.at(i), event);
        }
    }
}

void DuiPannableViewportPrivate::_q_resolvePannedWidgetIsOnDisplay()
{
    Q_Q(DuiPannableViewport);
    DuiOnDisplayChangeEvent *event = 0;

    if (q->isOnDisplay() && q->sceneManager() != 0) {
        QRectF visibleSceneRect(QPoint(0, 0), q->sceneManager()->visibleSceneSize());
        event = new DuiOnDisplayChangeEvent(DuiOnDisplayChangeEvent::MustBeResolved, visibleSceneRect);

        // If I just do a send(q->widget(), event) and widget() happens not to be a DuiWidget
        // (e.g., plain QGraphicsWidget layouting DuiWidgets inside) the event propagation
        // just won't happen.
        sendOnDisplayChangeEventToDuiWidgets(q->widget(), event);
    }
}

DuiPannableViewport::DuiPannableViewport(QGraphicsItem *parent)
    : DuiPannableWidget(new DuiPannableViewportPrivate(), new DuiPannableViewportModel, parent)
{
    Q_D(DuiPannableViewport);
    setFlags(QGraphicsItem::ItemClipsChildrenToShape);

    setPosition(QPointF());
    setRange(QRectF());

    d->positionIndicator = new DuiPositionIndicator(this);
    d->positionIndicator->setZValue(ZValuePosInd);
    connect(this,
            SIGNAL(viewportSizeChanged(QSizeF)),
            d->positionIndicator,
            SLOT(setViewportSize(QSizeF)));
    connect(this,
            SIGNAL(positionChanged(QPointF)),
            d->positionIndicator,
            SLOT(setPosition(QPointF)));
    connect(this,
            SIGNAL(rangeChanged(QRectF)),
            d->positionIndicator,
            SLOT(setRange(QRectF)));

    d->viewportLayout = new DuiPannableViewportLayout;
    d->viewportLayout->setPanningDirections(panDirection());
    d->viewportLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(d->viewportLayout);

    connect(this,
            SIGNAL(panningStopped()),
            SLOT(_q_resolvePannedWidgetIsOnDisplay()));
}

DuiPannableViewport::~DuiPannableViewport()
{
    QGraphicsWidget *oldwidget  = widget();
    setWidget(0);
    if (oldwidget) {
        delete oldwidget;
        oldwidget = 0;
    }
}

void DuiPannableViewport::setAutoRange(bool enable)
{
    Q_D(DuiPannableViewport);

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

bool DuiPannableViewport::autoRange() const
{
    return model()->autoRange();
}

void DuiPannableViewport::setClipping(bool enabled)
{
    model()->setClipWidget(enabled);

    if (enabled) {
        setFlags(QGraphicsItem::ItemClipsChildrenToShape);
    } else {
        setFlags(0);
    }
}

bool DuiPannableViewport::hasClipping() const
{
    return model()->clipWidget();
}

void DuiPannableViewport::setWidget(QGraphicsWidget *widget)
{
    Q_D(DuiPannableViewport);

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

QGraphicsWidget *DuiPannableViewport::widget() const
{
    Q_D(const DuiPannableViewport);
    return d->pannedWidget;
}

void DuiPannableViewport::setRange(const QRectF &r)
{
    Q_D(DuiPannableViewport);

    if (!autoRange()) {
        d->setNewRange(r);

        // Recalculates the physics range for new manually set range
        d->recalculatePhysRange();
    }
}

QRectF DuiPannableViewport::range() const
{
    Q_D(const DuiPannableViewport);

    return d->currentRange;
}

void DuiPannableViewport::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(DuiPannableViewport);
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

void DuiPannableViewport::updatePosition(const QPointF &p)
{
    Q_D(DuiPannableViewport);

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

void DuiPannableViewport::setPanDirection(const Qt::Orientations &panDirection)
{
    Q_D(DuiPannableViewport);

    DuiPannableWidget::setPanDirection(panDirection);

    if (d->viewportLayout) {
        d->viewportLayout->setPanningDirections(panDirection);
    }

    updateGeometry();
}

void DuiPannableViewport::updateGeometry()
{
    Q_D(DuiPannableViewport);

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

    DuiPannableWidget::updateGeometry();
}

#include "moc_duipannableviewport.cpp"
