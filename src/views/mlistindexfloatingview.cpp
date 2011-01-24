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

#include "mlistindexfloatingview.h"
#include "mlistindexfloatingview_p.h"
#include "mlistview_p.h"

#include <mlistindex.h>

#include <MApplicationPage>
#include <MCancelEvent>
#include <MPannableViewport>
#include <MSeparator>
#include "mlistindextooltip.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QTapAndHoldGesture>

MListIndexFloatingViewPrivate::MListIndexFloatingViewPrivate()
    : MWidgetViewPrivate(),
    controller(NULL),
    container(NULL),
    tooltipWidget(NULL),
    tooltipVerticalOffset(0.0)
{
}

MListIndexFloatingViewPrivate::~MListIndexFloatingViewPrivate()
{
}

void MListIndexFloatingViewPrivate::initLayout()
{
    attachToContainer();
    configureController();
}

void MListIndexFloatingViewPrivate::configureController()
{
    controller->show();
    controller->setOpacity(1.0);
}

void MListIndexFloatingViewPrivate::updateLayout()
{
    Q_Q(MListIndexFloatingView);

    if(container) {
        controller->resize(controller->preferredWidth(), containerRect.height() - q->model()->offset().y());
        controller->setPos(containerRect.x() + containerRect.width() - controller->preferredWidth(), containerRect.y() + q->model()->offset().y());
    }
}

void MListIndexFloatingViewPrivate::attachToContainer()
{
    Q_Q(MListIndexFloatingView);

    if (container)
        container->disconnect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_exposedContentRectChanged()));

    if (q->model()->list()) {
        container = MListViewPrivateNamespace::findParentWidgetOfType<MApplicationPage>(q->model()->list());

        if (container) {
            controller->setParentItem(container);

            container->connect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_recalculateListIndexRegion()));
            _q_recalculateListIndexRegion();
        }
    }
}

MListIndexTooltip *MListIndexFloatingViewPrivate::tooltip()
{
    Q_Q(MListIndexFloatingView);

    if (!tooltipWidget) {
        tooltipWidget = new MListIndexTooltip(controller);
        tooltipWidget->setIndexCount(q->style()->floatingIndexCount());
        // should be hidden by default;
        tooltipWidget->hide();

        q->connect(tooltipWidget, SIGNAL(geometryChanged()), q, SLOT(_q_recalculateTooltipOffsets()));
        _q_recalculateTooltipOffsets();
    }

    return tooltipWidget;
}

void MListIndexFloatingViewPrivate::updateTooltipPosition(const QPointF &pos)
{
    Q_Q(MListIndexFloatingView);

    if (tooltip()) {
        qreal top = 0;
        qreal tooltipHeight = tooltip()->size().height();

        if (pos.y() > tooltipHeight / 2 && pos.y() < tooltipVerticalOffset)
            top = pos.y() - tooltipHeight / 2;
        else if (pos.y() >= tooltipVerticalOffset)
            top = controller->size().height() - tooltipHeight;

        tooltip()->setPos(-tooltip()->size().width() - q->style()->floatingIndexOffset(), top);
    }
}

void MListIndexFloatingViewPrivate::updateTooltipData()
{
    Q_Q(MListIndexFloatingView);

    int shortcutCount = q->model()->shortcutIndexes().count();
    int shortcutIndex = q->model()->shortcutIndexes().indexOf(currentScrollToIndex);

    int floatingIndexCount = qMin(q->style()->floatingIndexCount(), shortcutCount);
    int startIndex = qMax(shortcutIndex - floatingIndexCount + 2, 0);
    int endIndex = qMin(startIndex + floatingIndexCount - 1, shortcutCount - 1);
    startIndex = qMin(startIndex, shortcutCount - floatingIndexCount);

    for (int i = startIndex; i <= endIndex; i++) {
        tooltip()->setIndexText(i - startIndex, q->model()->shortcutLabels().at(i));
    }
    tooltip()->setIndexSelected(shortcutIndex - startIndex);
}

void MListIndexFloatingViewPrivate::scrollToGroupHeader(int y)
{
    Q_Q(MListIndexFloatingView);

    qreal offset = (qreal)y / controller->size().height();
    int shortcutCount = q->model()->shortcutIndexes().count();
    int shortcutIndex = shortcutCount * offset;

    if (shortcutIndex < shortcutCount && shortcutIndex >= 0) {
        QModelIndex scrollToIndex = q->model()->shortcutIndexes().at(shortcutIndex);
        if (scrollToIndex.isValid()) {
            if (scrollToIndex != currentScrollToIndex) {
                int snapDirection = -1;
                if (q->model()->shortcutIndexes().indexOf(currentScrollToIndex) < shortcutIndex)
                    snapDirection = 1;

                currentScrollToIndex = scrollToIndex;
                q->model()->list()->scrollTo(scrollToIndex, MList::PositionAtTopHint);
                updateTooltipData();

                tooltip()->snap(q->style()->floatingSnapDistance() * snapDirection);
            }
        }
    }
}

void MListIndexFloatingViewPrivate::_q_recalculateListIndexRegion()
{
    containerRect = container->exposedContentRect();
    updateLayout();
}

void MListIndexFloatingViewPrivate::_q_listParentChanged()
{
    initLayout();
}

void MListIndexFloatingViewPrivate::_q_recalculateTooltipOffsets()
{
    tooltipVerticalOffset = controller->size().height() - tooltip()->size().height() / 2;
}

MListIndexFloatingView::MListIndexFloatingView(MListIndex *controller)
    : MWidgetView(*(new MListIndexFloatingViewPrivate),  controller)
{
    Q_D(MListIndexFloatingView);
    d->controller = controller;
}

MListIndexFloatingView::~MListIndexFloatingView()
{
}

void MListIndexFloatingView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char*> modifications;
    modifications << MListIndexModel::List;
    modifications << MListIndexModel::ShortcutLabels;
    modifications << MListIndexModel::ShortcutIndexes;
    updateData(modifications);
}

void MListIndexFloatingView::updateData(const QList<const char *> &modifications)
{
    Q_D(MListIndexFloatingView);

    MWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];
        if (member == MListIndexModel::List) {
            if (model()->list())
                d->initLayout();
        } else if (member == MListIndexModel::ShortcutIndexes) {
            d->tooltip()->setIndexCount(qMin(style()->floatingIndexCount(), model()->shortcutIndexes().count()));
        }
    }
}

void MListIndexFloatingView::applyStyle()
{
    Q_D(MListIndexFloatingView);

    MWidgetView::applyStyle();
    d->tooltip()->setStyleName(style()->floatingStyleName());
}

void MListIndexFloatingView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MListIndexFloatingView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexFloatingView);

    if (model()->shortcutIndexes().isEmpty()) {
        event->ignore();
        return;
    }

    MWidgetView::mousePressEvent(event);

    d->scrollToGroupHeader(event->pos().y());
    d->updateTooltipPosition(event->pos());

    event->accept();

    d->tooltip()->show();
}

void MListIndexFloatingView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexFloatingView);

    if (model()->shortcutIndexes().isEmpty()) {
        event->ignore();
        return;
    }

    MWidgetView::mousePressEvent(event);

    d->scrollToGroupHeader(event->pos().y());
    d->updateTooltipPosition(event->pos());

    event->accept();

    d->tooltip()->show();
}

void MListIndexFloatingView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexFloatingView);

    MWidgetView::mouseReleaseEvent(event);
    event->accept();

    d->tooltip()->hide();
}

void MListIndexFloatingView::cancelEvent(MCancelEvent *event)
{
    Q_D(MListIndexFloatingView);

    MWidgetView::cancelEvent(event);
    event->accept();

    d->tooltip()->hide();
}

void MListIndexFloatingView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    event->accept(gesture);
}

void MListIndexFloatingView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    event->accept(gesture);
}

#include "moc_mlistindexfloatingview.cpp"

M_REGISTER_VIEW_NEW(MListIndexFloatingView, MListIndex)
