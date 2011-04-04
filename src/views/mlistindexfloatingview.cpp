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
#include <MSheet>
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
    list(NULL),
    tooltipWidget(NULL),
    tooltipVerticalOffset(0),
    contentHeight(0),
    fastScrollPosition()
{
}

MListIndexFloatingViewPrivate::~MListIndexFloatingViewPrivate()
{
}

void MListIndexFloatingViewPrivate::init()
{
    Q_Q(MListIndexFloatingView);
    q->connect(&scrollToDelayQueue, SIGNAL(timeout()), q, SLOT(_q_scrollListToCurrentIndex()));
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

void MListIndexFloatingViewPrivate::connectToList()
{
    Q_Q(MListIndexFloatingView);

    if (list)
        q->disconnect(list, SIGNAL(parentChanged()), q, SLOT(_q_listParentChanged()));

    if (q->model()->list())
        q->connect(q->model()->list(), SIGNAL(parentChanged()), q, SLOT(_q_listParentChanged()));

    list = q->model()->list();
}

void MListIndexFloatingViewPrivate::updateLayout()
{
    Q_Q(MListIndexFloatingView);

    if(container) {
        controller->resize(controller->preferredWidth(), containerRect.height() - q->model()->offset().y());
        controller->setPos(containerRect.x() + containerRect.width() - controller->preferredWidth(), containerRect.y() + q->model()->offset().y());
        updateContentHeight();
    }
}

void MListIndexFloatingViewPrivate::updateContentHeight()
{
    Q_Q(MListIndexFloatingView);
    contentHeight = (controller->size().height() - (q->style()->marginTop() + q->style()->marginBottom()));
}

void MListIndexFloatingViewPrivate::attachToContainer()
{
    Q_Q(MListIndexFloatingView);

    if (container) {
        if (container->windowType() == MSceneWindow::ApplicationPage)
            container->disconnect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_recalculateListIndexRegion()));
        else {
            container->disconnect(container, SIGNAL(widthChanged()), q, SLOT(_q_recalculateListIndexRegion()));
            container->disconnect(container, SIGNAL(heightChanged()), q, SLOT(_q_recalculateListIndexRegion()));
        }
    }

    if (q->model()->list()) {
        container = MListViewPrivateNamespace::findParentWidgetOfType<MSceneWindow>(q->model()->list());

        if (container) {
            controller->setParentItem(container);
            controller->setZValue(container->zValue() + 1);

            if (container->windowType() == MSceneWindow::ApplicationPage)
                q->connect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_recalculateListIndexRegion()));
            else {
                q->connect(container, SIGNAL(widthChanged()), q, SLOT(_q_recalculateListIndexRegion()));
                q->connect(container, SIGNAL(heightChanged()), q, SLOT(_q_recalculateListIndexRegion()));
            }
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

void MListIndexFloatingViewPrivate::updateTooltipPosition(int y)
{
    Q_Q(MListIndexFloatingView);

    if (tooltip()) {
        qreal top = 0;
        qreal tooltipHeight = tooltip()->size().height();

        if (y > tooltipHeight / 2 && y < tooltipVerticalOffset)
            top = y - tooltipHeight / 2;
        else if (y >= tooltipVerticalOffset)
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

    qreal offset = (qreal)y / contentHeight;
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
                queueListScrollTo();
                updateTooltipData();

                tooltip()->snap(q->style()->floatingSnapDistance() * snapDirection);
            }
        }
    }
}

void MListIndexFloatingViewPrivate::queueListScrollTo()
{
    if (!scrollToDelayQueue.isActive())
        scrollToDelayQueue.start(50);
}

void MListIndexFloatingViewPrivate::_q_scrollListToCurrentIndex()
{
    Q_Q(MListIndexFloatingView);
    q->model()->list()->scrollTo(currentScrollToIndex, MList::PositionAtTopHint);
    scrollToDelayQueue.stop();
}

void MListIndexFloatingViewPrivate::_q_recalculateListIndexRegion()
{
    if (container->windowType() == MSceneWindow::ApplicationPage)
        containerRect = static_cast<MApplicationPage*>(container)->exposedContentRect();
    else if (container->windowType() == MSceneWindow::Sheet) {
        QGraphicsWidget *headerWidget = static_cast<MSheet*>(container)->headerWidget();
        QSizeF containerSize = container->size();
        if (headerWidget)
            containerRect = QRectF(QPointF(0, headerWidget->size().height()),
                                   QSize(containerSize.width(), containerSize.height() - headerWidget->size().height()));
        else
            containerRect = QRectF(QPointF(), containerSize);
    } else
        containerRect = QRectF(QPointF(), container->size());

    updateLayout();
}

void MListIndexFloatingViewPrivate::_q_recalculateTooltipOffsets()
{
    tooltipVerticalOffset = controller->size().height() - tooltip()->size().height() / 2;
}

void MListIndexFloatingViewPrivate::_q_listParentChanged()
{
    initLayout();
}

void MListIndexFloatingViewPrivate::beginFastScrolling(const QPointF &pos)
{
    Q_Q(MListIndexFloatingView);

    if (q->model()->shortcutIndexes().isEmpty())
        return;

    updateFastScrolling(pos);
    fastScrollPosition = pos;

    tooltip()->show();
}

void MListIndexFloatingViewPrivate::updateFastScrolling(const QPointF &offset)
{
    fastScrollPosition += offset;
    int y = controller->mapFromScene(fastScrollPosition).y();
    scrollToGroupHeader(y);
    updateTooltipPosition(y);
}

void MListIndexFloatingViewPrivate::endFastScrolling()
{
    tooltip()->hide();
    fastScrollPosition = QPointF(0,0);
}

MListIndexFloatingView::MListIndexFloatingView(MListIndex *controller)
    : MWidgetView(*(new MListIndexFloatingViewPrivate),  controller)
{
    Q_D(MListIndexFloatingView);
    d->controller = controller;
    d->init();
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
            if (model()->list()) {
                d->initLayout();
                d->connectToList();
            }
        } else if (member == MListIndexModel::ShortcutIndexes) {
            d->tooltip()->setIndexCount(qMin(style()->floatingIndexCount(), model()->shortcutIndexes().count()));
        } else if (member == MListIndexModel::ShortcutLabels) {
            d->updateTooltipData();
        }
    }
}

void MListIndexFloatingView::applyStyle()
{
    Q_D(MListIndexFloatingView);

    MWidgetView::applyStyle();
    d->tooltip()->setStyleName(style()->floatingStyleName());
    d->updateContentHeight();
}

void MListIndexFloatingView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MListIndexFloatingView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    Q_D(MListIndexFloatingView);

    if (d->tooltip()->isVisible()) {
        event->accept(gesture);
        return;
    }

    MWidgetView::tapAndHoldGestureEvent(event, gesture);
}

void MListIndexFloatingView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    Q_D(MListIndexFloatingView);
    if (gesture->state() == Qt::GestureStarted)
        d->beginFastScrolling(event->mapToGraphicsScene(gesture->hotSpot()));
    else if (gesture->state() == Qt::GestureFinished || gesture->state() == Qt::GestureCanceled)
        d->endFastScrolling();
    else if (gesture->state() == Qt::GestureUpdated)
        d->updateFastScrolling(gesture->delta());

    event->accept(gesture);
}

#include "moc_mlistindexfloatingview.cpp"

M_REGISTER_VIEW_NEW(MListIndexFloatingView, MListIndex)
