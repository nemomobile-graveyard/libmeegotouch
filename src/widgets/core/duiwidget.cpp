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

#include "duiwidget.h"
#include "duiwidget_p.h"

#include <QPainter>
#include <QObject>
#include <QGraphicsSceneContextMenuEvent>
#include <DuiDebug>
#include <QApplication>
#include <QGraphicsLayout>
#include <QApplication>
#include <QGraphicsItem>
#include <QTapAndHoldGesture>

#include <duiapplicationwindow.h>
#include <duipannableviewport.h>
#include <duiaction.h>
#include <duicancelevent.h>
#include <duiondisplaychangeevent.h>
#include <duiorientationchangeevent.h>
#include <duiscene.h>
#include <duiscenemanager.h>

#include "duiobjectmenu.h"


DuiWidgetPrivate::DuiWidgetPrivate() :
    explicitlyHidden(false),
    layoutHidden(false),
    q_ptr(NULL),
    onDisplay(false),
    onDisplaySet(false),
    selected(false)
{
}

DuiWidgetPrivate::~DuiWidgetPrivate()
{
}

/*
 * protected constructor
 */
DuiWidget::DuiWidget(DuiWidgetPrivate &dd, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , d_ptr(& dd) // this is a special case, since we start our shared d-pointer hierarchy within dui right here
{
    Q_D(DuiWidget);
    if (d)
        d->q_ptr = this;
}

/*
 * public constructor
 */
DuiWidget::DuiWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , d_ptr(new DuiWidgetPrivate())
{
    Q_D(DuiWidget);
    d->q_ptr = this;
}

/*
 * destructor
 */
DuiWidget::~DuiWidget()
{
    delete d_ptr;
}

void DuiWidgetPrivate::doEnterDisplayEvent()
{
    Q_Q(DuiWidget);

    onDisplay = true;
    onDisplaySet = true;

    q->enterDisplayEvent();
    emit q->displayEntered();
}

void DuiWidgetPrivate::doExitDisplayEvent()
{
    Q_Q(DuiWidget);

    onDisplay = false;
    onDisplaySet = true;

    q->exitDisplayEvent();
    emit q->displayExited();
}

void DuiWidgetPrivate::sendOnDisplayChangeEvent(DuiWidget *widget, const QRectF *visibleSceneRect)
{
    Q_Q(DuiWidget);
    DuiOnDisplayChangeEvent *event = 0;

    QRectF widgetSceneRect = widget->sceneBoundingRect();

    if (visibleSceneRect && visibleSceneRect->intersects(widgetSceneRect)) {
        if (visibleSceneRect->contains(widgetSceneRect)) {
            event = new DuiOnDisplayChangeEvent(DuiOnDisplayChangeEvent::FullyOnDisplay, *visibleSceneRect);
        } else {
            event = new DuiOnDisplayChangeEvent(DuiOnDisplayChangeEvent::PartiallyOnDisplay, *visibleSceneRect);
        }
    } else {
        event = new DuiOnDisplayChangeEvent(DuiOnDisplayChangeEvent::FullyOffDisplay, *visibleSceneRect);
    }

    q->scene()->sendEvent(widget, event);
    delete event;
}

void DuiWidgetPrivate::resolveIsOnDisplay(QGraphicsItem *item, const QRectF *visibleSceneRect)
{
    DuiWidget *duiWidget;

    if (item->isWidget()) {
        duiWidget = qobject_cast<DuiWidget *>(static_cast<QGraphicsWidget *>(item));
        if (duiWidget) {
            sendOnDisplayChangeEvent(duiWidget, visibleSceneRect);
        }
    }

    QList<QGraphicsItem *> childItemsList = item->childItems();
    int childItemsCount = childItemsList.count();

    for (int i = 0; i < childItemsCount; i++) {
        resolveIsOnDisplay(childItemsList.at(i), visibleSceneRect);
    }
}

QGraphicsView *DuiWidgetPrivate::fetchGraphicsView()
{
    Q_Q(DuiWidget);
    QGraphicsView *graphicsView = 0;

    QGraphicsScene *scene = q->scene();
    if (scene) {
        QList<QGraphicsView *> viewsList = scene->views();
        // We assume that there's only one QGraphicsView rendering our scene
        if (viewsList.count() > 0) {
            if (viewsList.count() > 1) {
                duiWarning("DuiWidget") << "There's more than one QGraphicsView rendering a DuiScene.";
            }

            graphicsView = viewsList.at(0);
        }
    }

    return graphicsView;
}

void DuiWidget::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    Q_D(DuiWidget);

    switch (event->state()) {
    case DuiOnDisplayChangeEvent::FullyOnDisplay:
    case DuiOnDisplayChangeEvent::PartiallyOnDisplay:
        if (!d->onDisplay || !d->onDisplaySet) {
            d->doEnterDisplayEvent();
        }
        break;

    case DuiOnDisplayChangeEvent::FullyOffDisplay:
        if (d->onDisplay || !d->onDisplaySet) {
            d->doExitDisplayEvent();
        }
        break;

    case DuiOnDisplayChangeEvent::MustBeResolved: {
        // We assume that there's only one QGraphicsView rendering our scene
        QGraphicsView *graphicsView = d->fetchGraphicsView();
        if (graphicsView) {
            QRectF visibleSceneRect = graphicsView->sceneRect();
            d->resolveIsOnDisplay(this, &visibleSceneRect);
        } else {
            d->resolveIsOnDisplay(this, 0);
        }
    }
    break;

    default:
        break;
    }
}

void DuiWidget::enterDisplayEvent()
{
}

void DuiWidget::exitDisplayEvent()
{
}

bool DuiWidget::isOnDisplay() const
{
    bool result = false;

    if (!isVisible()) {
        return false;
    }

    if (!scene()) {
        return false;
    }

    QList<QGraphicsView *> viewsList = scene()->views();
    int viewCount = viewsList.count();
    QGraphicsView *graphicsView;
    DuiWindow *duiWindow;
    QRect viewportRect; // viewport rectangle. (0, 0, width, height)
    QRect widgetViewportRect; // widget bounding rect in viewport coordinates
    int i = 0;

    while (result == false && i < viewCount) {

        graphicsView = viewsList.at(i);
        ++i;

        if (!graphicsView->isVisible() || graphicsView->isMinimized()) {
            continue;
        }

        duiWindow = qobject_cast<DuiWindow *>(graphicsView);
        if (duiWindow && !duiWindow->isOnDisplay()) {
            continue;
        }

        widgetViewportRect = graphicsView->mapFromScene(sceneBoundingRect()).boundingRect();

        viewportRect.setWidth(graphicsView->viewport()->width());
        viewportRect.setHeight(graphicsView->viewport()->height());

        if (widgetViewportRect.intersects(viewportRect)) {
            result = true;
        }
    }

    return result;
}

DuiSceneManager *DuiWidget::sceneManager() const
{
    DuiScene *duiScene = qobject_cast<DuiScene *>(scene());
    if (duiScene)
        return duiScene->sceneManager();
    return 0;
}

QPainterPath DuiWidget::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


void DuiWidget::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void DuiWidget::setObjectName(const QString &name)
{
    QGraphicsWidget::setObjectName(name);
}

void DuiWidget::clearActions()
{
    QList<QAction *> acts = actions();
    const int actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        removeAction(acts[i]);
    }
}

QVariant DuiWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemVisibleHasChanged:
        emit visibilityChanged(value.toBool());
        break;

    default:
        break;
    }

    return QGraphicsWidget::itemChange(change, value);
}

void DuiWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList<QAction *> actionList = actions();
    bool hasObjectMenuActions = false;
    const int actionListCount = actionList.count();
    for (int i = 0; i < actionListCount; ++i) {
        DuiAction *action = qobject_cast<DuiAction *>(actionList.at(i));
        if (action && action->isVisible() && (action->location() & DuiAction::ObjectMenuLocation)) {
            hasObjectMenuActions = true;
            break;
        }
    }

    if (hasObjectMenuActions) {
        DuiObjectMenu *menu = new DuiObjectMenu(this);
        sceneManager()->appearSceneWindow(menu, DuiSceneWindow::DestroyWhenDone);
    } else {
        event->ignore();
    }
}

void DuiWidget::gestureEvent(QGestureEvent *event)
{
    foreach(QGesture* gesture, event->gestures()) {
        if (Qt::TapAndHoldGesture == gesture->gestureType()) {
            QTapAndHoldGesture* tapAndHoldState = static_cast<QTapAndHoldGesture *>(gesture);
            tapAndHoldGestureEvent(event,tapAndHoldState);
        } else if (Qt::PanGesture == gesture->gestureType()) {
            QPanGesture* panState = static_cast<QPanGesture *>(gesture);
            panGestureEvent(event,panState);
        } else if (Qt::PinchGesture == gesture->gestureType()) {
            QPinchGesture* pinchState = static_cast<QPinchGesture *>(gesture);
            pinchGestureEvent(event,pinchState);
        } else if (Qt::TapGesture == gesture->gestureType()) {
            QTapGesture* tapState = static_cast<QTapGesture *>(gesture);
            tapGestureEvent(event,tapState);
        } else if (Qt::SwipeGesture == gesture->gestureType()) {
            QSwipeGesture* swipeState = static_cast<QSwipeGesture *>(gesture);
            swipeGestureEvent(event,swipeState);
        }
    }
}

void DuiWidget::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    event->ignore(gesture);
}

void DuiWidget::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    event->ignore(gesture);
}

void DuiWidget::pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture)
{
    event->ignore(gesture);
}

void DuiWidget::tapGestureEvent(QGestureEvent *event, QTapGesture* gesture)
{
    event->ignore(gesture);
}

void DuiWidget::swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture)
{
    event->ignore(gesture);
}

bool DuiWidget::event(QEvent *event)
{

//    if (DuiTouchEvent::cast2TouchEvent(event))
//        duiDebug("DuiWidget") << "touchevent in DuiWidget:" << event;
//    if (DuiGestureEvent::cast2GestureEvent(event))
//        duiDebug("DuiWidget) << "gesture in DuiWidget:" << event;
    QEvent::Type type = event->type();
    if (type == QEvent::ActionAdded || type == QEvent::ActionChanged ||
            type == QEvent::ActionRemoved) {

        // now I add ActionEvent here for Qt 4.5 not provide this in QGraphicsWidget, maybe changed later
        actionEvent((QActionEvent *)event);
    } else if (type == DuiCancelEvent::eventNumber()) {
        cancelEvent((DuiCancelEvent *)event);
    } else if (type == DuiOrientationChangeEvent::eventNumber()) {
        DuiOrientationChangeEvent *oce = static_cast<DuiOrientationChangeEvent *>(event);
        orientationChangeEvent(oce);
    } else if (type == QEvent::LanguageChange) {
        // retranslate the ui strings of this DuiWidget:
        retranslateUi();
    } else if (type == DuiOnDisplayChangeEvent::eventNumber()) {
        onDisplayChangeEvent(static_cast<DuiOnDisplayChangeEvent *>(event));
    } else if (type == QEvent::Gesture) {
        gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QGraphicsWidget::event(event);
}

void DuiWidget::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
}

void DuiWidget::orientationChangeEvent(DuiOrientationChangeEvent *event)
{
    Q_UNUSED(event);
}

void DuiWidget::actionEvent(QActionEvent *)
{
}

void DuiWidget::retranslateUi()
{
    // do nothing
}

void DuiWidget::setSelected(bool selected)
{
    Q_D(DuiWidget);

    if (d->selected == selected)
        return;

    const QVariant newSelectedVariant(itemChange(QGraphicsItem::ItemSelectedChange, quint32(selected)));

    bool newSelected = newSelectedVariant.toBool();
    if (d->selected == newSelected)
        return;
    d->selected = newSelected;

    update();

    // Deliver post-change notification.
    itemChange(QGraphicsItem::ItemSelectedHasChanged, newSelectedVariant);
}

bool DuiWidget::isSelected() const
{
    Q_D(const DuiWidget);

    return d->selected;
}

void DuiWidget::setVisible(bool visible)
{
    Q_D(DuiWidget);
    d->explicitlyHidden = !visible;
    if (!d->layoutHidden) //Only show if the layout is not showing this
        QGraphicsWidget::setVisible(visible);
}

