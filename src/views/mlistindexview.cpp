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

#include "mlistindexview.h"
#include "mlistindexview_p.h"
#include "mlistview_p.h"

#include <MApplicationPage>
#include <MSheet>
#include <MLabel>
#include <MPannableViewport>
#include <MScalableImage>
#include <MSceneManager>

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <qmath.h>
#include <QPropertyAnimation>
#include <QTapAndHoldGesture>

#include "mlistindex.h"
#include "mlist.h"

MListIndexViewPrivate::MListIndexViewPrivate()
  : q_ptr(NULL),
    controller(NULL),
    layout(NULL),
    container(NULL),
    list(NULL),
    shortcutHeight(0),
    shortcutsCount(0),
    autoVisibilityAnimation(0),
    down(false),
    currentIndex(),
    fastScrollPosition()
{
    autoVisibilityTimer.setSingleShot(true);
    autoVisibilityTimer.stop();
}

MListIndexViewPrivate::~MListIndexViewPrivate()
{
    clearVisible();
}

void MListIndexViewPrivate::init()
{
    Q_Q(MListIndexView);
    q->connect(&autoVisibilityTimer, SIGNAL(timeout()), q, SLOT(_q_visibilityTimerTimeout()));
}

void MListIndexViewPrivate::applyStyleToShortcuts()
{
    Q_Q(MListIndexView);

    if (layout) {
        for (int i = 0; i < layout->count(); i++) {
            MLabel *shortcut = dynamic_cast<MLabel*>(layout->itemAt(i));
            if (shortcut)
                shortcut->setObjectName(q->style()->shortcutObjectName());
        }
        updateVisible();
    }
}

void MListIndexViewPrivate::initLayout()
{
    Q_Q(MListIndexView);

    clearVisible();

    if (!autoVisibilityAnimation) {
        autoVisibilityAnimation = new QPropertyAnimation(controller, "opacity", controller);
        autoVisibilityAnimation->setLoopCount(1);
        autoVisibilityAnimation->setStartValue(0.0);
        autoVisibilityAnimation->setEndValue(1.0);
        autoVisibilityAnimation->setDuration(timelineDuration());
        autoVisibilityAnimation->setKeyValueAt(delayTimelineDuration(), 0.0);
        autoVisibilityAnimation->setKeyValueAt(1.0 - delayTimelineDuration(), 1.0);
    }

    if (!layout) {
        layout = new QGraphicsLinearLayout(Qt::Vertical, controller);
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        controller->setLayout(layout);
    }
    
    if (q->model()) {
        shortcutsCount = 0;

        createContainer();

        if (q->model()->shortcutLabels().count() > 0) {
            //create the first shortcut label and get the font metrics from it
            MLabel *firstShortcut = createLabel(0);
            layout->addItem(firstShortcut);

            shortcutHeight = firstShortcut->minimumHeight();

            shortcutsCount = q->model()->shortcutLabels().count();
            updateVisible();
        }
    }
}

void MListIndexViewPrivate::updateLayout()
{
    Q_Q(MListIndexView);

    if(container) {
        clearVisible();
        controller->resize(controller->preferredWidth(), containerRect.height() - q->model()->offset().y());
        controller->setPos(containerRect.x() + containerRect.width() - controller->preferredWidth(), containerRect.y() + q->model()->offset().y());
    }
    updateVisible();
}

void MListIndexViewPrivate::connectToList()
{
    Q_Q(MListIndexView);

    if (list != q->model()->list()) {
        if (list) {
            q->disconnect(list, SIGNAL(parentChanged()), q, SLOT(_q_listParentChanged()));
            q->disconnect(list, SIGNAL(panningStarted()), q, SLOT(_q_listPanningStarted()));
            q->disconnect(list, SIGNAL(panningStopped()), q, SLOT(_q_listPanningStopped()));
        }
        if (q->model()->list()) {
            q->connect(q->model()->list(), SIGNAL(parentChanged()), q, SLOT(_q_listParentChanged()));
            q->connect(q->model()->list(), SIGNAL(panningStarted()), q, SLOT(_q_listPanningStarted()));
            q->connect(q->model()->list(), SIGNAL(panningStopped()), q, SLOT(_q_listPanningStopped()));
        }
        list = q->model()->list();
    }
}

QModelIndex MListIndexViewPrivate::locateShortcutIndex(int y, int x)
{
    Q_UNUSED(x);

    for (int i = 0; i < layout->count(); i++) {
        MLabel *shortcut = dynamic_cast<MLabel*>(layout->itemAt(i));
        if (shortcut && shortcut->geometry().contains(x, y)) {
            int flatIndex = controller->model()->shortcutLabels().indexOf(shortcut->text());
            return controller->model()->shortcutIndexes()[flatIndex];
        }
    }
    return QModelIndex();
}

MLabel *MListIndexViewPrivate::createLabel(int index)
{
    Q_Q(MListIndexView);
    Q_ASSERT(index < q->model()->shortcutLabels().count());

    MLabel *shortcut = new MLabel();
    shortcut->setAlignment(Qt::AlignCenter);
    shortcut->setObjectName(q->style()->shortcutObjectName());
    shortcut->setText(q->model()->shortcutLabels()[index]);

    return shortcut;
}

void MListIndexViewPrivate::clearVisible()
{
    if (layout) {
        for (int i = layout->count(); i > 0; i--) {
            QGraphicsLayoutItem *item = layout->itemAt(0);
            delete item;
        }
    }
}

void MListIndexViewPrivate::updateVisible()
{
    Q_Q(MListIndexView);

    if (shortcutHeight == 0)
        return;

    int fitCount = (controller->contentsRect().height() - q->model()->offset().y()) / shortcutHeight;

    if (fitCount > 0 && fitCount != layout->count()) {
        int skipCount = qCeil((qreal)shortcutsCount / (qreal)fitCount);
        if (skipCount > 0 && layout->count() != shortcutsCount / skipCount) {
            clearVisible();
            for (int i = 0; i < shortcutsCount; i += skipCount) {
                if (shortcutsCount - i <= skipCount)
                    layout->addItem(createLabel(shortcutsCount - 1));
                else
                    layout->addItem(createLabel(i));
            }
        }
    }
}

void MListIndexViewPrivate::createContainer()
{
    Q_Q(MListIndexView);
    if (container) {
        if (container->windowType() == MSceneWindow::ApplicationPage)
            q->disconnect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_exposedContentRectChanged()));
        else {
            q->disconnect(container, SIGNAL(widthChanged()), q, SLOT(_q_exposedContentRectChanged()));
            q->disconnect(container, SIGNAL(heightChanged()), q, SLOT(_q_exposedContentRectChanged()));
        }
    }

    if (q->model()->list()) {
        container = MListViewPrivateNamespace::findParentWidgetOfType<MSceneWindow>(q->model()->list());

        if (container) {
            controller->setParentItem(container);
            controller->setZValue(container->zValue() + 1);

            if (container->windowType() == MSceneWindow::ApplicationPage)
                q->connect(container, SIGNAL(exposedContentRectChanged()), q, SLOT(_q_exposedContentRectChanged()));
            else {
                q->connect(container, SIGNAL(widthChanged()), q, SLOT(_q_exposedContentRectChanged()));
                q->connect(container, SIGNAL(heightChanged()), q, SLOT(_q_exposedContentRectChanged()));
            }
            _q_exposedContentRectChanged();
        }
    }
}

qreal MListIndexViewPrivate::delayTimelineDuration()
{
    Q_Q(MListIndexView);
    return q->style()->appearDelay() / timelineDuration();
}

qreal MListIndexViewPrivate::timelineDuration()
{
    Q_Q(MListIndexView);
    return q->style()->appearDelay() * 2.f + q->style()->appearDuration();
}

void MListIndexViewPrivate::updateDisplayMode()
{
    switch(controller->displayMode()) {
    case MList::Auto:
        controller->show();
        _q_startVisibilityTimer();
        break;
    case MList::Show:
        controller->show();
        _q_stopVisibilityTimer();
        _q_showAnimated();
        break;
    default:
        controller->hide();
        break;
    };
}

void MListIndexViewPrivate::scrollToGroupHeader(int y)
{
    Q_Q(MListIndexView);
    QModelIndex scrollTo = locateShortcutIndex(y, 0);
    if (scrollTo.isValid() && scrollTo != currentIndex) {
        q->model()->list()->scrollTo(scrollTo, MList::PositionAtTopHint);
        currentIndex = scrollTo;
    }
}

void MListIndexViewPrivate::_q_listParentChanged()
{
    initLayout();
}

void MListIndexViewPrivate::_q_listPanningStarted()
{
    _q_stopVisibilityTimer();
    if (!autoVisibilityAnimation || controller->displayMode() != MList::Auto)
        return;

    if (controller->opacity() < 1.0)
        _q_showAnimated();
}

void MListIndexViewPrivate::_q_listPanningStopped()
{
    if (!autoVisibilityAnimation || controller->displayMode() != MList::Auto || down)
        return;

    if (!autoVisibilityTimer.isActive())
        _q_startVisibilityTimer();
}

void MListIndexViewPrivate::_q_visibilityTimerTimeout()
{
    _q_hideAnimated();
}

void MListIndexViewPrivate::_q_hideAnimated()
{
    if (autoVisibilityAnimation->state() != QPropertyAnimation::Running) {
        autoVisibilityAnimation->setDirection(QPropertyAnimation::Backward);
        autoVisibilityAnimation->start();
    }
}

void MListIndexViewPrivate::_q_showAnimated()
{
    autoVisibilityAnimation->setDirection(QPropertyAnimation::Forward);
    if (autoVisibilityAnimation->state() != QPropertyAnimation::Running)
        autoVisibilityAnimation->start();
}

void MListIndexViewPrivate::_q_startVisibilityTimer()
{
    Q_Q(MListIndexView);
    if (q->model()->displayMode() == MList::Auto && !down)
        autoVisibilityTimer.start();
}

void MListIndexViewPrivate::_q_stopVisibilityTimer()
{
    autoVisibilityTimer.stop();
}

void MListIndexViewPrivate::_q_exposedContentRectChanged()
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

void MListIndexViewPrivate::beginFastScrolling(const QPointF &pos)
{
    Q_Q(MListIndexView);

    if (q->model()->shortcutIndexes().isEmpty())
        return;

    fastScrollPosition = QPointF();
    updateFastScrolling(pos);
    fastScrollPosition = pos;

    down = true;
    _q_stopVisibilityTimer();
}

void MListIndexViewPrivate::updateFastScrolling(const QPointF &offset)
{
    fastScrollPosition += offset;
    int y = controller->mapFromScene(fastScrollPosition).y();
    scrollToGroupHeader(y);
    _q_stopVisibilityTimer();
}

void MListIndexViewPrivate::endFastScrolling()
{
    down = false;
    fastScrollPosition = QPointF(0,0);
    _q_startVisibilityTimer();
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

void MListIndexView::setupModel()
{
    Q_D(MListIndexView);

    MWidgetView::setupModel();
    d->initLayout();
    d->connectToList();
    d->updateDisplayMode();
}

void MListIndexView::applyStyle()
{
    Q_D(MListIndexView);

    MWidgetView::applyStyle();

    if (style()->preferredSize().isValid())
        d->controller->setPreferredWidth(style()->preferredSize().width());
    else
        d->controller->setPreferredWidth(0.0);

    if (d->autoVisibilityAnimation) {
        d->autoVisibilityAnimation->setDuration(style()->appearDuration());

        d->autoVisibilityAnimation->setStartValue(0.0);
        d->autoVisibilityAnimation->setEndValue(1.0);

        d->autoVisibilityAnimation->setKeyValueAt(d->delayTimelineDuration(), 0.0);
        d->autoVisibilityAnimation->setKeyValueAt(1.0 - d->delayTimelineDuration(), 1.0);
    }

    d->autoVisibilityTimer.setInterval(style()->fadeOutDelay());

    d->applyStyleToShortcuts();
}

void MListIndexView::updateData(const QList<const char *> &modifications)
{
    Q_D(MListIndexView);

    const char *member;
    for (int i = 0; i < modifications.count(); i++) {
        member = modifications[i];

        if (member == MListIndexModel::ShortcutLabels || member == MListIndexModel::List) {
            if (model()->list()) {
                d->initLayout();
                d->connectToList();
            }
        }
        else if (member == MListIndexModel::DisplayMode) {
            d->updateDisplayMode();
        }
        else if (member == MListIndexModel::Offset) {
            d->updateLayout();
        }
    }

    MWidgetView::updateData(modifications);
}

void MListIndexView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexView);
    d->beginFastScrolling(d->controller->mapToScene(event->pos()));
    event->accept();
}

void MListIndexView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexView);

    d->endFastScrolling();
    event->accept();
}

void MListIndexView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    event->ignore(gesture);
}

void MListIndexView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    Q_D(MListIndexView);
    if (gesture->state() == Qt::GestureStarted)
        d->beginFastScrolling(event->mapToGraphicsScene(gesture->hotSpot()));
    else if (gesture->state() == Qt::GestureFinished || gesture->state() == Qt::GestureCanceled)
        d->endFastScrolling();
    else if (gesture->state() == Qt::GestureUpdated)
        d->updateFastScrolling(gesture->delta());

    event->accept(gesture);
}

#include "moc_mlistindexview.cpp"

M_REGISTER_VIEW_NEW(MListIndexView, MListIndex)
