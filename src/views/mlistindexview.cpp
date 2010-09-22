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
  : controller(NULL),
    controllerModel(NULL),
    layout(NULL),
    container(NULL),
    shortcutHeight(0),
    shortcutsCount(0),
    autoVisibilityAnimation(0),
    down(false),
    q_ptr(NULL)
{
    autoVisibilityTimer.setSingleShot(true);
    autoVisibilityTimer.stop();
}

MListIndexViewPrivate::~MListIndexViewPrivate()
{
    clearVisible();
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
    
    if (controller->model()) {
        controllerModel = controller->model();
        shortcutsCount = 0;

        createContainer();

        if (controller->model()->shortcutLabels().count() > 0) {
            //create the first shortcut label and get the font metrics from it
            MLabel *firstShortcut = createLabel(0);
            layout->addItem(firstShortcut);

            shortcutHeight = firstShortcut->minimumHeight();

            shortcutsCount = controllerModel->shortcutLabels().count();
            updateVisible();
        }

        disconnect(this, SLOT(listParentChanged()));
        disconnect(this, SLOT(listPanningStarted()));
        disconnect(this, SLOT(listPanningStopped()));
        if (controllerModel && controllerModel->list()) {
            connect(controllerModel->list(), SIGNAL(parentChanged()), this, SLOT(listParentChanged()));
            connect(controllerModel->list(), SIGNAL(panningStarted()), this, SLOT(listPanningStarted()));
            connect(controllerModel->list(), SIGNAL(panningStopped()), this, SLOT(listPanningStopped()));
        }
        connect(&autoVisibilityTimer, SIGNAL(timeout()), this, SLOT(visibilityTimerTimeout()));
    }
}

void MListIndexViewPrivate::updateLayout()
{
    if(container) {
        clearVisible();
        controller->resize(controller->preferredWidth(), containerRect.height());
        controller->setPos(containerRect.x() + containerRect.width() - controller->preferredWidth(), containerRect.y());
    }
    updateVisible();
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
    Q_ASSERT(index < controllerModel->shortcutLabels().count());

    MLabel *shortcut = new MLabel();
    shortcut->setAlignment(Qt::AlignCenter);
    shortcut->setObjectName(q->style()->shortcutObjectName());
    shortcut->setText(controllerModel->shortcutLabels()[index]);

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
    if (shortcutHeight == 0)
        return;

    int fitCount = (controller->contentsRect().height()) / shortcutHeight;

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
    if (container)
        disconnect(container, SIGNAL(exposedContentRectChanged()), this, SLOT(exposedContentRectChanged()));

    if (controller->model()->list()) {
        container = MListViewPrivateNamespace::findParentWidgetOfType<MApplicationPage>(controller->model()->list());

        if (container) {
            controller->setParentItem(container);

            connect(container, SIGNAL(exposedContentRectChanged()), this, SLOT(exposedContentRectChanged()));
            exposedContentRectChanged();
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
    return q->style()->appearDelay() * 2.0 + q->style()->appearDuration();
}

void MListIndexViewPrivate::updateDisplayMode()
{
    switch(controller->displayMode()) {
    case MList::Auto:
        controller->show();
        startVisibilityTimer();
        break;
    case MList::Show:
        controller->show();
        stopVisibilityTimer();
        showAnimated();
        break;
    default:
        controller->hide();
        break;
    };
}

void MListIndexViewPrivate::scrollToGroupHeader(int y)
{
    QModelIndex scrollTo = locateShortcutIndex(y, 0);
    if (scrollTo.isValid()) {
        controllerModel->list()->scrollTo(scrollTo, MList::PositionAtTopHint);
    }
}

void MListIndexViewPrivate::listParentChanged()
{
    initLayout();
}

void MListIndexViewPrivate::listPanningStarted()
{
    stopVisibilityTimer();
    if (!autoVisibilityAnimation || controller->displayMode() != MList::Auto)
        return;

    if (controller->opacity() < 1.0)
        showAnimated();
}

void MListIndexViewPrivate::listPanningStopped()
{
    if (!autoVisibilityAnimation || controller->displayMode() != MList::Auto || down)
        return;

    if (!autoVisibilityTimer.isActive())
        startVisibilityTimer();
}

void MListIndexViewPrivate::visibilityTimerTimeout()
{
    hideAnimated();
}

void MListIndexViewPrivate::hideAnimated()
{
    if (autoVisibilityAnimation->state() != QPropertyAnimation::Running) {
        autoVisibilityAnimation->setDirection(QPropertyAnimation::Backward);
        autoVisibilityAnimation->start();
    }
}

void MListIndexViewPrivate::showAnimated()
{
    autoVisibilityAnimation->setDirection(QPropertyAnimation::Forward);
    if (autoVisibilityAnimation->state() != QPropertyAnimation::Running)
        autoVisibilityAnimation->start();
}

void MListIndexViewPrivate::startVisibilityTimer()
{
    if (controllerModel->displayMode() == MList::Auto && !down)
        autoVisibilityTimer.start();
}

void MListIndexViewPrivate::stopVisibilityTimer()
{
    autoVisibilityTimer.stop();
}

void MListIndexViewPrivate::exposedContentRectChanged()
{
    containerRect = container->exposedContentRect();
    updateLayout();
}

MListIndexView::MListIndexView(MListIndex *controller) : MWidgetView(controller), d_ptr(new MListIndexViewPrivate)
{
    Q_D(MListIndexView);

    d->q_ptr = this;
    d->controller = controller;
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
            if (model()->list())
                d->initLayout();
        }
        else if (member == MListIndexModel::DisplayMode) {
            d->updateDisplayMode();
        }
    }

    MWidgetView::updateData(modifications);
}

void MListIndexView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexView);
    MWidgetView::mousePressEvent(event);

    d->scrollToGroupHeader(event->pos().y());
    d->down = true;
    d->listPanningStarted();
    d->stopVisibilityTimer();

    event->accept();
}

void MListIndexView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexView);
    MWidgetView::mouseReleaseEvent(event);

    d->down = false;
    d->startVisibilityTimer();
}

void MListIndexView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MListIndexView);
    MWidgetView::mouseMoveEvent(event);

    d->scrollToGroupHeader(event->pos().y());

    event->accept();   
}

void MListIndexView::cancelEvent(MCancelEvent *event)
{
    Q_D(MListIndexView);
    MWidgetView::cancelEvent(event);

    d->down = false;
    d->startVisibilityTimer();
}

void MListIndexView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    event->accept(gesture);
}

M_REGISTER_VIEW_NEW(MListIndexView, MListIndex)
