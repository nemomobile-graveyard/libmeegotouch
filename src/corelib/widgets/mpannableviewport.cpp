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

#include "minputmethodstate.h"

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
      pannedWidget(0),
      viewportLayout(0),
      positionIndicator(0),
      rangeHeightExtension(0),
      inputMethodAreaHeight(0)
{
}

MPannableViewportPrivate::~MPannableViewportPrivate()
{
}

void MPannableViewportPrivate::setNewRange(const QRectF &newRange)
{
    Q_Q(MPannableViewport);

    QSizeF physRangeSize(newRange.size());

    if (physRangeSize.width() < 0.0) {
        physRangeSize.setWidth(0.0);
    }

    if (physRangeSize.height() < 0.0) {
        physRangeSize.setHeight(0.0);
    }

    const QRectF physRange(newRange.topLeft(),
                           physRangeSize + QSizeF(0, rangeHeightExtension));

    if (physRange != q->physics()->range()) {
        q->physics()->setRange(physRange);
        emit q->rangeChanged(q->range());
    }
}

void MPannableViewportPrivate::setInputMethodArea(const QRect &imArea)
{
    Q_Q(MPannableViewport);

    // Assume only bottom part of pannable area is covered by SIP.
    const int overlappingHeight = (q->mapRectFromScene(imArea).toRect()
                                   & q->rect().toRect()).height();

    // Increase panning range to so that user can pan areas beneath
    // software input panel visible.
    inputMethodAreaHeight = overlappingHeight;
    updateExtendedVerticalRange();
}

void MPannableViewportPrivate::updateExtendedVerticalRange()
{
    Q_Q(MPannableViewport);
    const QRectF restoredRange(q->range().adjusted(0, 0, 0, -rangeHeightExtension));

    // Only touch range extension if auto range is enabled.
    if (q->autoRange()) {
        // Choose whichever is bigger.
        rangeHeightExtension = qMax<qreal>(0.0, inputMethodAreaHeight);
    } else {
        rangeHeightExtension = 0.0f;
    }

    setNewRange(restoredRange);
}

void MPannableViewportPrivate::applyAutoRange()
{
    Q_Q(MPannableViewport);
    if (q->autoRange()) {
        if (pannedWidget) {
            setNewRange(QRectF(QPointF(), pannedWidget->size() - q->size()));
        } else {
            setNewRange(QRectF());
        }
    }
}

void MPannableViewportPrivate::scrollTo(const QPointF &endPosition)
{
    Q_Q(MPannableViewport);

    if (scrollToAnimation.state() == QAbstractAnimation::Running) {
        if (endPosition.toPoint() == scrollToAnimation.endValue().toPointF().toPoint()) {
            return;
        }
        scrollToAnimation.stop();
    }

    if (q->position() != endPosition) {
        // TODO: Add animation parameters to some style.
        scrollToAnimation.setStartValue(q->position());
        scrollToAnimation.setEndValue(endPosition);
        scrollToAnimation.setEasingCurve(QEasingCurve::InOutCubic);
        scrollToAnimation.setDuration(300);
        scrollToAnimation.start();
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
        positionIndicator->setRange(q->range());
        positionIndicator->setPosition(q->position());
        positionIndicator->setViewportSize(q->size());

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

void MPannableViewportPrivate::_q_pannedWidgetHeightOutOfViewport()
{
    Q_Q(MPannableViewport);;
    // If current position is bigger than new panned widget size (in panning direction) then
    // position should be updated to avoid unnecessary panning animation.
    if (q->panDirection().testFlag(Qt::Vertical)) {

        // Need to update range so we can use it here.
        applyAutoRange();

        // Note that range also contains the height extension.
        const qreal rangeBottomLimit = q->range().bottom();
        if (rangeBottomLimit < q->position().y()) {
            q->setPosition(QPointF(q->position().x(), rangeBottomLimit));
        }
    }

    // The heightChanged signal is called from an event handler. Singleshot timer
    // will ensure that relocation will be triggered only after sizes of the viewport
    // settle and correct microfocus is returned from scene. This is a workaround
    // until we find out why we don't have the correct value of the microfocus rect
    // returned while calling it from the height changed slot.
    QTimer::singleShot(0, q, SLOT(_q_ensureFocusedPannedWidgetIsVisible()));
}

void MPannableViewportPrivate::_q_pannedWidgetWidthOutOfViewport()
{
    Q_Q(MPannableViewport);;
    // If current position is bigger than new panned widget size (in panning direction) then
    // position should be updated to avoid unnecessary panning animation.
    if (q->panDirection().testFlag(Qt::Horizontal)) {
        qreal updatedPosition = pannedWidget->size().width() - q->size().width();
        if(updatedPosition < q->position().x())
            q->setPosition(QPointF(qMax((qreal)0, updatedPosition), q->position().y()));
    }
}

void MPannableViewportPrivate::_q_ensureFocusedPannedWidgetIsVisible()
{
    Q_Q(MPannableViewport);

    if (q->sceneManager() &&
        pannedWidget && pannedWidget->focusItem()) {
        q->sceneManager()->ensureCursorVisible();
    }
}

bool MPannableViewportPrivate::isTopmostVerticallyPannableViewport() const
{
    Q_Q(const MPannableViewport);
    const QGraphicsWidget* parentWidget = q->parentWidget();

    while(parentWidget) {
        const MPannableViewport *viewport = qobject_cast<const MPannableViewport *>(parentWidget);
        if (viewport && (viewport->panDirection() & Qt::Vertical)) {
            return false;
        }
        parentWidget = parentWidget->parentWidget();
    }
    return true;
}

void MPannableViewportPrivate::_q_handleInputMethodAreaChanged(const QRect &newArea)
{
    if (isTopmostVerticallyPannableViewport()) {
        setInputMethodArea(newArea);
    } else if (inputMethodAreaHeight != 0) {
        // This is not the topmost pannable viewport anymore.
        inputMethodAreaHeight = 0;
        updateExtendedVerticalRange();
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
    d->viewportLayout->setSpacing(0);
    setLayout(d->viewportLayout);

    connect(this,
            SIGNAL(panningStopped()),
            SLOT(_q_resolvePannedWidgetIsOnDisplay()));

    connect(MInputMethodState::instance(), SIGNAL(inputMethodAreaChanged(QRect)),
            this, SLOT(_q_handleInputMethodAreaChanged(QRect)));

    d->scrollToAnimation.setTargetObject(this);
    d->scrollToAnimation.setPropertyName("position");
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
    d->applyAutoRange();
}

bool MPannableViewport::autoRange() const
{
    return model()->autoRange();
}

void MPannableViewport::setClipping(bool enabled)
{
    model()->setClipWidget(enabled);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, enabled);
}

bool MPannableViewport::hasClipping() const
{
    return model()->clipWidget();
}

void MPannableViewport::setWidget(QGraphicsWidget *widget)
{
    Q_D(MPannableViewport);

    if (d->pannedWidget) {
        disconnect(d->pannedWidget, SIGNAL(heightChanged()), this, SLOT(_q_pannedWidgetHeightOutOfViewport()));
        disconnect(d->pannedWidget, SIGNAL(widthChanged()), this, SLOT(_q_pannedWidgetWidthOutOfViewport()));
    }

    d->pannedWidget = widget;
    d->viewportLayout->setWidget(widget);

    if (widget) {
        connect(widget, SIGNAL(heightChanged()), this, SLOT(_q_pannedWidgetHeightOutOfViewport()));
        connect(widget, SIGNAL(widthChanged()), this, SLOT(_q_pannedWidgetWidthOutOfViewport()));

        d->viewportLayout->invalidate();
        widget->setZValue(ZValuePannedWidget);
    }

    d->applyAutoRange();
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

    d->applyAutoRange();

    emit viewportSizeChanged(event->newSize());

    d->positionIndicator->resize(event->newSize());
}

void MPannableViewport::updatePosition(const QPointF &p)
{
    Q_D(MPannableViewport);

    QPointF roundedP = QPointF(floorf(p.x()), floorf(p.y()));

    // Parameter p is in the local coordinate system of pannedWidget but
    // pannedWidget position is in pannable viewport's coordinate system.
    //
    // Therefore pannedWidget position is set as a negate of physics position (p),
    // because the panned widget needs to flow to the opposite direction of the
    // panning.

    if (d->pannedWidget && d->pannedWidget->pos() != -roundedP) {
        d->viewportLayout->setPanningPosition(-roundedP);
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

    d->applyAutoRange();

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
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() & ~Qt::Vertical);
            } else {
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() | Qt::Vertical);
            }
        }

        if (member == MPannableWidgetModel::HorizontalPanningPolicy) {
            if (horizontalPanningPolicy() == PanningAlwaysOff) {
                d->viewportLayout->setPanningDirections( d->viewportLayout->panningDirections() & ~Qt::Horizontal);
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
