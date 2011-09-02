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

#include "mlistindexview.h"
#include "mlistindexview_p.h"
#include "mlistview_p.h"

#include <MAbstractWidgetAnimation>
#include <MApplicationPage>
#include <MSheet>
#include <MLabel>
#include <MPannableViewport>
#include <MPositionIndicator>
#include <MScalableImage>
#include <MSceneManager>

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QTapAndHoldGesture>

#include "mlistindex.h"
#include "mlistindextooltip.h"
#include "mlist.h"

MListIndexViewPrivate::MListIndexViewPrivate()
  : q_ptr(0),
    controller(0),
    panel(0),
    layout(0),
    panelLayout(0),
    container(0),
    appearanceAnimation(0),
    tooltipAppearanceAnimation(0),
    panelAppearanceAnimation(0),
    tooltipWidget(0),
    list(0),
    viewport(0),
    groupTitleHeight(0),
    isFastScrolling(false),
    lastFastScrollRow(-1),
    contentOpacity(1.0),
    viewportSize(),
    viewportAdjustedRange(),
    positionIndicatorPosition(),
    ScrollToDelay(64)
{
}

MListIndexViewPrivate::~MListIndexViewPrivate()
{
    qDeleteAll(groupTitleLabelWidgets);
    groupTitleLabelWidgets.clear();
}

void MListIndexViewPrivate::init()
{
    Q_Q(MListIndexView);
    q->connect(controller, SIGNAL(groupTitlesChanged()), q, SLOT(_q_updateTitles()));

    appearanceAnimation = new QPropertyAnimation(q, "contentOpacity", q);
    tooltipAppearanceAnimation = new QPropertyAnimation(tooltip(), "opacity", q);

    scrollToQueueTimer.setInterval(ScrollToDelay);
    scrollToQueueTimer.setSingleShot(true);
    scrollToQueueTimer.connect(&scrollToQueueTimer, SIGNAL(timeout()), q, SLOT(_q_scrollToLastRow()));
}

void MListIndexViewPrivate::initLayout()
{
    qDeleteAll(groupTitleLabelWidgets);
    groupTitleLabelWidgets.clear();

    if (!panel) {
        panel = new QGraphicsWidget(controller);
        panelAppearanceAnimation = new QPropertyAnimation(panel, "opacity", controller);
        panel->hide();
    }

    layout = new QGraphicsAnchorLayout(controller);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addCornerAnchors(panel, Qt::TopLeftCorner, layout, Qt::TopLeftCorner);
    layout->addCornerAnchors(panel, Qt::BottomRightCorner, layout, Qt::BottomRightCorner);

    panelLayout = new QGraphicsLinearLayout(Qt::Vertical, panel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(0);
}

void MListIndexViewPrivate::connectToList()
{
    Q_Q(MListIndexView);

    if (list != q->model()->list()) {
        if (list) {
            q->disconnect(list, SIGNAL(parentChanged()), q, SLOT(_q_attachToListContainer()));
            q->disconnect(list, SIGNAL(panningStarted()), q, SLOT(_q_showIfNeeded()));
            q->disconnect(list, SIGNAL(panningStopped()), q, SLOT(_q_hideIfNeeded()));
        }
        list = q->model()->list();
        if (list) {
            q->connect(list, SIGNAL(parentChanged()), q, SLOT(_q_attachToListContainer()));
            q->connect(list, SIGNAL(panningStarted()), q, SLOT(_q_showIfNeeded()));
            q->connect(list, SIGNAL(panningStopped()), q, SLOT(_q_hideIfNeeded()));
        }
    }
    _q_attachToListContainer();
}

void MListIndexViewPrivate::setGroupTitle(int item, int index)
{
    if (item < groupTitleLabelWidgets.count())
        groupTitleLabelWidgets[item]->setText(list->itemModel()->index(index, 0).data(Qt::DisplayRole).toString());
}

MLabel *MListIndexViewPrivate::createGroupTitleLabel()
{
    Q_Q(MListIndexView);

    MLabel *label = new MLabel(panel);
    label->setWordWrap(false);
    label->setTextElide(false);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleName(q->style()->shortcutStyleName());
    return label;
}

MListIndexTooltip *MListIndexViewPrivate::tooltip()
{
    if (!tooltipWidget) {
        tooltipWidget = new MListIndexTooltip(controller);
        tooltipWidget->hide();
    }
    return tooltipWidget;
}

MWindow *MListIndexViewPrivate::currentWindow()
{
    QGraphicsScene *scene = controller->scene();

    if (!scene || scene->views().isEmpty())
        return 0;

    return qobject_cast<MWindow*>(scene->views().at(0));
}

void MListIndexViewPrivate::setDisplayMode(int mode)
{
    MList::DisplayMode displayMode = static_cast<MList::DisplayMode>(mode);

    switch(displayMode) {
    case MList::Floating:
    case MList::Auto:
    case MList::Show:
        controller->show();
        hideViewportPositionIndicator();
        _q_appearOnSceneWindow();
        break;
    case MList::Hide:
    default:
        showViewportPositionIndicator();
        controller->hide();
        break;
    };
}

void MListIndexViewPrivate::_q_attachToListContainer()
{
    Q_Q(MListIndexView);
    if (container) {
        if (container->windowType() == MSceneWindow::ApplicationPage)
            q->disconnect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_updateGeometry()));
        q->disconnect(container, SIGNAL(widthChanged()), q, SLOT(_q_updateGeometry()));
        q->disconnect(container, SIGNAL(heightChanged()), q, SLOT(_q_updateGeometry()));
        q->disconnect(container, SIGNAL(displayEntered()), q, SLOT(_q_appearOnSceneWindow()));
    }

    if (q->model()->list()) {
        container = MListViewPrivateNamespace::findParentWidgetOfType<MSceneWindow>(q->model()->list());

        if (container) {
            controller->setParentItem(container);
            controller->setZValue(container->zValue() + 1);

            if (container->windowType() == MSceneWindow::ApplicationPage)
                q->connect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_updateGeometry()));
            q->connect(container, SIGNAL(widthChanged()), q, SLOT(_q_updateGeometry()));
            q->connect(container, SIGNAL(heightChanged()), q, SLOT(_q_updateGeometry()));
            q->connect(container, SIGNAL(displayEntered()), q, SLOT(_q_appearOnSceneWindow()));
            _q_updateGeometry();
        }
    }
    _q_attachToViewport();
}

void MListIndexViewPrivate::_q_attachToViewport()
{
    Q_Q(MListIndexView);
    if (viewport) {
        q->disconnect(viewport, SIGNAL(viewportSizeChanged(QSizeF)), q, SLOT(_q_updatePositionIndicatorPosition(QSizeF)));
        q->disconnect(viewport, SIGNAL(positionChanged(QPointF)), q, SLOT(_q_updatePositionIndicatorPosition(QPointF)));
        q->disconnect(viewport, SIGNAL(rangeChanged(QRectF)), q, SLOT(_q_updatePositionIndicatorPosition(QRectF)));
    }

    if (q->model()->list()) {
        viewport = MListViewPrivateNamespace::findParentWidgetOfType<MPannableViewport>(q->model()->list());

        if (viewport) {
            q->connect(viewport, SIGNAL(viewportSizeChanged(QSizeF)), q, SLOT(_q_updatePositionIndicatorPosition(QSizeF)));
            q->connect(viewport, SIGNAL(positionChanged(QPointF)), q, SLOT(_q_updatePositionIndicatorPosition(QPointF)));
            q->connect(viewport, SIGNAL(rangeChanged(QRectF)), q, SLOT(_q_updatePositionIndicatorPosition(QRectF)));
            _q_updatePositionIndicatorPosition(viewport->size());
        }
    }
}

void MListIndexViewPrivate::scrollToGroupHeader(int y)
{
    Q_Q(MListIndexView);
    int groupCount = list->itemModel()->rowCount();
    int fastScrollRow = groupCount * ((qreal)y / q->size().height());
    if (fastScrollRow != lastFastScrollRow && fastScrollRow >= 0 && fastScrollRow < groupCount)  {
        // Force updating the title of the bubble.
        if (lastFastScrollRow == -1)
            tooltip()->setTitle(list->itemModel()->index(fastScrollRow, 0).data(list->indexMagnifierDataRole()).toString());

        lastFastScrollRow = fastScrollRow;
        if (!scrollToQueueTimer.isActive()) {
            scrollToQueueTimer.start();
        }
    }

    int tooltipY = y;
    qreal tooltipHeight = tooltip()->size().height();
    tooltipY -= tooltipHeight / 2;
    if (tooltipY < 0)
        tooltipY = 0;
    else if (tooltipY + tooltipHeight > q->size().height())
        tooltipY = q->size().height() - tooltipHeight;
    tooltip()->setPos(tooltip()->pos().x(), tooltipY);
}

void MListIndexViewPrivate::_q_showIfNeeded()
{
    Q_Q(MListIndexView);

    if (list && list->itemModel() && list->itemModel()->rowCount() > 1) {
        if (q->model()->displayMode() != MList::Hide) {
            appearanceAnimation->stop();
            appearanceAnimation->setStartValue(q->contentOpacity());
            appearanceAnimation->setEndValue(1.0);
            appearanceAnimation->start();
        }
    }
}

void MListIndexViewPrivate::_q_appearOnSceneWindow()
{
    Q_Q(MListIndexView);

    // Do not show the list index in the switcher.
    MWindow *window = currentWindow();
    if (window && window->isInSwitcher())
        return;

    // Show only if there's more than just 1 group.
    if (list && list->itemModel() && list->itemModel()->rowCount() > 1) {
        _q_showIfNeeded();
        QTimer::singleShot(q->style()->appearDelay(), q, SLOT(_q_hideIfNeeded()));
    } else {
        q->setContentOpacity(0);
    }
}

void MListIndexViewPrivate::_q_hideIfNeeded()
{
    Q_Q(MListIndexView);

    if (q->model()->displayMode() != MList::Show &&
            !isFastScrolling &&
            !list->model()->listIsMoving()) {
        appearanceAnimation->stop();
        appearanceAnimation->setStartValue(q->contentOpacity());
        appearanceAnimation->setEndValue(0);
        appearanceAnimation->start();
    }
}

void MListIndexViewPrivate::_q_updateGeometry()
{
    Q_Q(MListIndexView);
    if (container) {
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
    }

    layout->invalidate();

    q->updateGeometry();
    if (viewport)
        controller->setPos(containerRect.width() - controller->preferredWidth(), containerRect.top() + viewport->pos().y());

    tooltip()->setPos(controller->mapFromParent(containerRect.left(), tooltip()->pos().y()));
    tooltip()->resize(containerRect.width(), tooltip()->size().height());

    positionIndicatorPosition.setX((controller->preferredWidth() - q->style()->positionIndicator()->width()) / 2);
}

void MListIndexViewPrivate::_q_updateTitles()
{
    Q_Q(MListIndexView);

    qDeleteAll(groupTitleLabelWidgets);
    groupTitleLabelWidgets.clear();

    if (q->model()->displayMode() == MList::Floating || !list || !list->itemModel())
        return;

    if (!groupTitleHeight)
        return;

    int groupCount = list->itemModel()->rowCount();
    int visibleTitlesCount = qMin(int(q->size().height() / groupTitleHeight), groupCount);

    if (!visibleTitlesCount)
        return;

    for (int i = groupTitleLabelWidgets.count(); i < visibleTitlesCount; ++i) {
        MLabel *label = createGroupTitleLabel();
        groupTitleLabelWidgets.append(label);
        panelLayout->addItem(label);
        panelLayout->setAlignment(label, Qt::AlignCenter);
    }

    qreal skip = qreal(visibleTitlesCount) / qreal(groupCount);
    int index = 0;
    for (int i = 0; i < groupCount; ++i) {
        if (i * skip >= index) {
            setGroupTitle(index, i);
            ++index;
        }
    }
}

void MListIndexViewPrivate::_q_scrollToLastRow()
{
    if (lastFastScrollRow >= 0) {
        QModelIndex index = list->itemModel()->index(lastFastScrollRow, 0);
        list->scrollTo(index, MList::PositionAtTopHint);
        tooltip()->setTitle(index.data(list->indexMagnifierDataRole()).toString());
    }
}

void MListIndexViewPrivate::_q_updatePositionIndicatorPosition(const QSizeF &viewportSize)
{
    this->viewportSize = viewportSize;
    _q_updatePositionIndicatorPosition(viewport->range());
    _q_updatePositionIndicatorPosition(viewport->position());
}

void MListIndexViewPrivate::_q_updatePositionIndicatorPosition(const QPointF &viewportPosition)
{
    Q_Q(MListIndexView);

    qreal distanceK = viewportPosition.y() / viewportAdjustedRange.height();
    qreal railHeight = q->size().height();

    int indicatorHeight = q->style()->positionIndicator()->height();
    railHeight -= indicatorHeight;
    int indicatorPositionY = distanceK * railHeight;

    if (indicatorPositionY + indicatorHeight >= int(q->size().height())) {
        indicatorPositionY = q->size().height() - indicatorHeight;
    }

    if (indicatorPositionY < 0) {
        indicatorPositionY = 0;
    }

    this->positionIndicatorPosition.setY(indicatorPositionY);
}

void MListIndexViewPrivate::_q_updatePositionIndicatorPosition(const QRectF &viewportRange)
{
    viewportAdjustedRange = viewportRange;
    _q_updatePositionIndicatorPosition(viewport->position());
    _q_updateGeometry();
}

void MListIndexViewPrivate::beginFastScrolling(const QPointF &pos)
{   
    _q_showIfNeeded();
    isFastScrolling = true;
    updateFastScrolling(pos);

    // the tooltip should appear immediately (without fade-in)
    tooltipAppearanceAnimation->stop();
    tooltip()->setOpacity(1.0);
    tooltip()->show();
}

void MListIndexViewPrivate::updateFastScrolling(const QPointF &pos)
{
    showTitlesPanel();
    int y = controller->mapFromScene(pos).y();
    scrollToGroupHeader(y);
}

void MListIndexViewPrivate::endFastScrolling()
{
    Q_Q(MListIndexView);

    isFastScrolling = false;
    if (scrollToQueueTimer.isActive()) {
        scrollToQueueTimer.stop();
        _q_scrollToLastRow();
    }

    lastFastScrollRow = -1;

    tooltipAppearanceAnimation->stop();
    tooltipAppearanceAnimation->setDuration(q->style()->fadeDuration());
    tooltipAppearanceAnimation->setStartValue(tooltip()->opacity());
    tooltipAppearanceAnimation->setEndValue(0);
    tooltipAppearanceAnimation->start();

    panelAppearanceAnimation->stop();
    panelAppearanceAnimation->setDuration(q->style()->fadeDuration());
    panelAppearanceAnimation->setStartValue(panel->opacity());
    panelAppearanceAnimation->setEndValue(0);
    panelAppearanceAnimation->start();

    _q_hideIfNeeded();
}

void MListIndexViewPrivate::updateGroupTitleHeight()
{
    MLabel *label = createGroupTitleLabel();
    groupTitleHeight = label->sizeHint(Qt::MinimumSize).height();
    delete label;
}

void MListIndexViewPrivate::showTitlesPanel()
{
    Q_Q(MListIndexView);
    if (q->model()->displayMode() == MList::Floating) {
        panel->hide();
    } else {
        // the titles panel appears without fade-in, like the tooltip
        panelAppearanceAnimation->stop();
        panel->setOpacity(1.0);
        panel->show();
    }
}

void MListIndexViewPrivate::hideViewportPositionIndicator()
{
    if (viewport)
        viewport->positionIndicator()->setEnabled(false);
}

void MListIndexViewPrivate::showViewportPositionIndicator()
{
    if (viewport)
        viewport->positionIndicator()->setEnabled(true);
}

MListIndexView::MListIndexView(MListIndex *controller) : MWidgetView(controller), d_ptr(new MListIndexViewPrivate)
{
    Q_D(MListIndexView);

    d->q_ptr = this;
    d->controller = controller;
    d->init();
}

MListIndexView::~MListIndexView()
{
    delete d_ptr;
}

void MListIndexView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(MListIndexView);
    MWidgetView::resizeEvent(event);

    d->_q_updateTitles();
}

void MListIndexView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char *> modifications;
    modifications << MListIndexModel::List;
    modifications << MListIndexModel::DisplayMode;
    updateData(modifications);
}

void MListIndexView::applyStyle()
{
    Q_D(MListIndexView);

    MWidgetView::applyStyle();

    d->updateGroupTitleHeight();
    d->tooltip()->setStyleName(style()->tooltipStyleName());
    d->appearanceAnimation->setDuration(style()->fadeDuration());
}

qreal MListIndexView::contentOpacity() const
{
    Q_D(const MListIndexView);
    return d->contentOpacity;
}

void MListIndexView::setContentOpacity(qreal opacity)
{
    Q_D(MListIndexView);
    d->contentOpacity = qMin(qMax(opacity, qreal(0.0)), qreal(1.0));
    update();
}

void MListIndexView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    if (contentOpacity() == 0)
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(contentOpacity() * oldOpacity);
    MWidgetView::drawBackground(painter, option);
    painter->setOpacity(oldOpacity);
}

void MListIndexView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MListIndexView);

    if (contentOpacity() == 0)
        return;

    qreal oldOpacity = painter->opacity();
    painter->setOpacity(contentOpacity() * oldOpacity);

    MWidgetView::drawContents(painter, option);

    if (style()->positionIndicator() && d->viewport && !d->isFastScrolling)
        painter->drawPixmap(d->positionIndicatorPosition, *style()->positionIndicator());

    painter->setOpacity(oldOpacity);
}

void MListIndexView::updateData(const QList<const char *> &modifications)
{
    Q_D(MListIndexView);
    MWidgetView::updateData(modifications);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];

        if (member == MListIndexModel::List) {
            if (model()->list()) {
                d->initLayout();
                d->connectToList();
            }
        }
        else if (member == MListIndexModel::DisplayMode) {
            d->setDisplayMode(model()->displayMode());
        }
    }
}

void MListIndexView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    Q_D(MListIndexView);
    if (d->isFastScrolling || (d->list && d->list->itemModel()->rowCount() > 1)) {
        if (gesture->state() == Qt::GestureStarted)
            d->beginFastScrolling(event->mapToGraphicsScene(gesture->hotSpot()));
        else if (gesture->state() == Qt::GestureFinished || gesture->state() == Qt::GestureCanceled)
            d->endFastScrolling();
        else if (gesture->state() == Qt::GestureUpdated)
            d->updateFastScrolling(event->mapToGraphicsScene(gesture->hotSpot()) + gesture->offset());

        event->accept(gesture);
    } else
        event->ignore(gesture);
}

QSizeF MListIndexView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MListIndexView);
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    if (model() && d->viewport)
        return QSizeF(style()->preferredSize().width(), d->containerRect.height() - d->viewport->pos().y());

    return QSizeF();
}

#include "moc_mlistindexview.cpp"

M_REGISTER_VIEW_NEW(MListIndexView, MListIndex)
