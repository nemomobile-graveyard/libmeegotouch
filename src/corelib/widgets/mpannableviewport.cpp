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
      positionIndicator(0),
      inputMethodAreaHeight(0)
{
}

MPannableViewportPrivate::~MPannableViewportPrivate()
{
}

void MPannableViewportPrivate::setNewRange(const QRectF &newRange)
{
    // Make the viewport artifically larger so that panning is possible even
    // if partially covered by the input method area.
    // See NB#175181
    QRectF actualRange = newRange.adjusted(0, 0, 0, inputMethodAreaHeight);

    currentRange = actualRange;
    recalculatePhysRange();
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

    QRectF physRange(currentRange.topLeft(), physicsRangeSize);

    if (physRange != q->physics()->range()) {
        q->physics()->setRange(physRange);
        emit q->rangeChanged(q->range());
    }
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

    if (q->isOnDisplay() && q->sceneManager() != 0 && q->widget() != 0) {
        QRectF visibleSceneRect(QPoint(0, 0), q->sceneManager()->visibleSceneSize());
        MOnDisplayChangeEvent event(MOnDisplayChangeEvent::MustBeResolved, visibleSceneRect);

        // If I just do a send(q->widget(), event) and widget() happens not to be a MWidget
        // (e.g., plain QGraphicsWidget layouting MWidgets inside) the event propagation
        // just won't happen.
        sendOnDisplayChangeEventToMWidgets(q->widget(), &event);
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

void MPannableViewportPrivate::setInputMethodArea(const QRect &imArea)
{
    const QRectF restoredRange(currentRange.adjusted(0, 0, 0, -inputMethodAreaHeight));

    inputMethodAreaHeight = imArea.height();

    setNewRange(restoredRange);
}

void MPannableViewportPrivate::_q_pannedWidgetGeometryChanged()
{
    correctWidgetPositionAfterGeometryChange();
    ensureFocusedPannedWidgetIsVisible();
}

void MPannableViewportPrivate::correctWidgetPositionAfterGeometryChange()
{
    Q_Q(MPannableViewport);

    QPointF physicsPosition = q->position();
    QPointF roundedP = QPointF(floor(physicsPosition.x()), floor(physicsPosition.y()));

    if (roundedP != -pannedWidget->pos())
        q->updatePosition(physicsPosition);
}

void MPannableViewportPrivate::ensureFocusedPannedWidgetIsVisible()
{
    Q_Q(MPannableViewport);

    if (q->sceneManager()
        && pannedWidget->focusItem()) {
        q->sceneManager()->ensureCursorVisible();
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
    Q_D(MPannableViewport);

    QGraphicsWidget *oldwidget  = widget();

    d->pannedWidget = NULL;
    d->viewportLayout->setWidget(NULL);

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
        d->inputMethodAreaHeight = 0;

        if (d->pannedWidget) {
            d->setNewRange(QRectF(QPointF(), d->pannedWidget->size()));
        } else {
            d->setNewRange(QRectF());
        }
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

    if (d->pannedWidget)
        disconnect(d->pannedWidget, SIGNAL(geometryChanged()), this, SLOT(_q_pannedWidgetGeometryChanged()));

    d->pannedWidget = widget;
    d->viewportLayout->setWidget(widget);

    if (widget) {
        connect(widget, SIGNAL(geometryChanged()), this, SLOT(_q_pannedWidgetGeometryChanged()));

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
}

QGraphicsWidget *MPannableViewport::widget() const
{
    Q_D(const MPannableViewport);
    return d->pannedWidget;
}

void MPannableViewport::setRange(const QRectF &r)
{
    Q_D(MPannableViewport);

    if (!autoRange())
        d->setNewRange(r);
}

QRectF MPannableViewport::range() const
{
    return MPannableWidget::range();
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

    updatePosition(position());

    MPannableWidget::updateGeometry();
}

void MPannableViewport::updateData(const QList<const char *> &modifications)
{
    Q_D(MPannableViewport);
    const char *member;

    foreach(member, modifications) {
        if (member == MPannableWidgetModel::VerticalPanningPolicy) {
            if (verticalPanningPolicy() == PanningAlwaysOff) {
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() & !Qt::Vertical);
            } else {
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() | Qt::Vertical);
            }
        }

        if (member == MPannableWidgetModel::HorizontalPanningPolicy) {
            if (horizontalPanningPolicy() == PanningAlwaysOff) {
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() & !Qt::Horizontal);
            } else {
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() | Qt::Horizontal);
            }
        }
    }
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
