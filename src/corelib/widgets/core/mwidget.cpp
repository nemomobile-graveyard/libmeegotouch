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

#include "mwidget.h"
#include "mwidget_p.h"

#include <QPainter>
#include <QObject>
#include <QGraphicsSceneContextMenuEvent>
#include <MDebug>
#include <QApplication>
#include <QGraphicsLayout>
#include <QApplication>
#include <QGraphicsItem>

#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QPanGesture>
#include <QPinchGesture>
#include <QTapGesture>
#include <QSwipeGesture>

#include <mapplicationwindow.h>
#include <mpannableviewport.h>
#include <maction.h>
#include <mcancelevent.h>
#include <mondisplaychangeevent.h>
#include <morientationchangeevent.h>
#include "mscene.h"
#include "mscene_p.h"
#include <mscenemanager.h>

#include "mobjectmenu.h"


MWidgetPrivate::MWidgetPrivate() :
    explicitlyHidden(false),
    layoutHidden(false),
    q_ptr(NULL),
    onDisplay(false),
    selected(false)
{
}

MWidgetPrivate::~MWidgetPrivate()
{
}

/*
 * protected constructor
 */
MWidget::MWidget(MWidgetPrivate &dd, QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , d_ptr(& dd) // this is a special case, since we start our shared d-pointer hierarchy within m right here
{
    Q_D(MWidget);
    if (d)
        d->q_ptr = this;
}

/*
 * public constructor
 */
MWidget::MWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent)
    , d_ptr(new MWidgetPrivate())
{
    Q_D(MWidget);
    d->q_ptr = this;
}

/*
 * destructor
 */
MWidget::~MWidget()
{
    delete d_ptr;
}

void MWidgetPrivate::doEnterDisplayEvent()
{
    Q_Q(MWidget);

    if (!onDisplay && q->isVisible()) {

        onDisplay = true;

        q->enterDisplayEvent();
        emit q->displayEntered();
    }
}

void MWidgetPrivate::doExitDisplayEvent()
{
    Q_Q(MWidget);

    if (onDisplay) {

        onDisplay = false;

        q->exitDisplayEvent();
        emit q->displayExited();
    }
}

void MWidgetPrivate::sendOnDisplayChangeEvent(MWidget *widget, const QRectF *visibleSceneRect)
{
    Q_Q(MWidget);
    MOnDisplayChangeEvent *event = 0;

    QRectF widgetSceneRect = widget->sceneBoundingRect();

    if (visibleSceneRect) {
        if (visibleSceneRect->intersects(widgetSceneRect)) {
            if (visibleSceneRect->contains(widgetSceneRect)) {
                event = new MOnDisplayChangeEvent(MOnDisplayChangeEvent::FullyOnDisplay, *visibleSceneRect);
            } else {
                event = new MOnDisplayChangeEvent(MOnDisplayChangeEvent::PartiallyOnDisplay, *visibleSceneRect);
            }
        } else {
            event = new MOnDisplayChangeEvent(MOnDisplayChangeEvent::FullyOffDisplay, *visibleSceneRect);
        }

        q->scene()->sendEvent(widget, event);
        delete event;
    }
}

void MWidgetPrivate::resolveIsOnDisplay(QGraphicsItem *item, const QRectF *visibleSceneRect,
                                        MOnDisplayChangeEvent *event)
{
    Q_Q(MWidget);
    MWidget *mWidget;

    if (item->isWidget()) {
        mWidget = qobject_cast<MWidget *>(static_cast<QGraphicsWidget *>(item));
        if (mWidget) {
            // Check if event was defined explicitly. If so then send it, otherwise
            // let sendOnDisplayChangeEvent() resolve visibility and send
            // the corresponding event.
            if (event) {
                q->scene()->sendEvent(mWidget, event);
            } else {
                sendOnDisplayChangeEvent(mWidget, visibleSceneRect);
            }
        }
    }

    QList<QGraphicsItem *> childItemsList = item->childItems();
    int childItemsCount = childItemsList.count();

    for (int i = 0; i < childItemsCount; i++) {
        resolveIsOnDisplay(childItemsList.at(i), visibleSceneRect, event);
    }
}

QGraphicsView *MWidgetPrivate::fetchGraphicsView()
{
    Q_Q(MWidget);
    QGraphicsView *graphicsView = 0;

    QGraphicsScene *scene = q->scene();
    if (scene) {
        QList<QGraphicsView *> viewsList = scene->views();
        // We assume that there's only one QGraphicsView rendering our scene
        if (viewsList.count() > 0) {
            if (viewsList.count() > 1) {
                mWarning("MWidget") << "There's more than one QGraphicsView rendering a MScene.";
            }

            graphicsView = viewsList.at(0);
        }
    }

    return graphicsView;
}

void MWidgetPrivate::setVisible(bool visible)
{
    Q_Q(MWidget);

    // FIXME: Workaround for bug NB#186278.
    QGraphicsItem *focused = q->focusItem();
    if (visible && focused && !focused->hasFocus()) {
        focused->setFlag(QGraphicsItem::ItemIsFocusable, false);
        q->QGraphicsWidget::setVisible(true);
        focused->setFlag(QGraphicsItem::ItemIsFocusable, true);
    } else {
        q->QGraphicsWidget::setVisible(visible);
    }

    // Propagate visibility events
    QGraphicsView *graphicsView = fetchGraphicsView();
    if (graphicsView) {
        QRectF visibleSceneRect = graphicsView->sceneRect();

        // show() called: resolve visibility
        if (visible) {
            if (q->isOnDisplay()) {
                MOnDisplayChangeEvent event(MOnDisplayChangeEvent::FullyOnDisplay,
                                            visibleSceneRect);
                resolveIsOnDisplay(q, &visibleSceneRect, &event);
            }
            // hide() called: explicitly send FullyOffDisplay
            } else {
                MOnDisplayChangeEvent event(MOnDisplayChangeEvent::FullyOffDisplay,
                                            visibleSceneRect);
                resolveIsOnDisplay(q, &visibleSceneRect, &event);
        }
    }
 }

void MWidget::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    Q_D(MWidget);

    switch (event->state()) {
    case MOnDisplayChangeEvent::FullyOnDisplay:
    case MOnDisplayChangeEvent::PartiallyOnDisplay:
        // If the widget has not previously gotten enterDisplayEvent
        // and it hasn't been hidden by hide() (setVisible(false)),
        // consider this event as entering the display.
        d->doEnterDisplayEvent();
        break;

    case MOnDisplayChangeEvent::FullyOffDisplay:
        d->doExitDisplayEvent();
        break;

    case MOnDisplayChangeEvent::MustBeResolved: {
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

void MWidget::enterDisplayEvent()
{
}

void MWidget::exitDisplayEvent()
{
}

bool MWidget::isOnDisplay() const
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
    MWindow *mWindow;
    QRect viewportRect; // viewport rectangle. (0, 0, width, height)
    QRect widgetViewportRect; // widget bounding rect in viewport coordinates
    int i = 0;

    while (result == false && i < viewCount) {

        graphicsView = viewsList.at(i);
        ++i;

        if (!graphicsView->isVisible()) {
            continue;
        }

        mWindow = qobject_cast<MWindow *>(graphicsView);
        if (mWindow && !mWindow->isOnDisplay()) {
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

MSceneManager *MWidget::sceneManager() const
{
    MScene *mScene = qobject_cast<MScene *>(scene());
    if (mScene)
        return mScene->sceneManager();
    return 0;
}

QPainterPath MWidget::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}


void MWidget::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void MWidget::setObjectName(const QString &name)
{
    QGraphicsWidget::setObjectName(name);
}

void MWidget::clearActions()
{
    QList<QAction *> acts = actions();
    const int actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        removeAction(acts[i]);
    }
}

void MWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList<QAction *> actionList = actions();
    bool hasObjectMenuActions = false;
    const int actionListCount = actionList.count();
    for (int i = 0; i < actionListCount; ++i) {
        MAction *action = qobject_cast<MAction *>(actionList.at(i));
        if (action && action->isVisible() && (action->location() & MAction::ObjectMenuLocation)) {
            hasObjectMenuActions = true;
            break;
        }
    }

    if (hasObjectMenuActions) {

        MScene *mScene = qobject_cast<MScene *>(scene());
        if (mScene && event->reason() == QGraphicsSceneContextMenuEvent::Mouse)
            mScene->d_func()->notifyChildRequestedMouseCancel();

        MObjectMenu *menu = new MObjectMenu(this);
        menu->setCursorPosition(event->scenePos());
        sceneManager()->appearSceneWindow(menu, MSceneWindow::DestroyWhenDone);
    } else {
        event->ignore();
    }
}

void MWidget::grabGestureWithCancelPolicy(Qt::GestureType type, Qt::GestureFlags flags, MouseEventCancelPolicy cancelPolicy)
{
    Q_D(MWidget);
    QGraphicsWidget::grabGesture(type, flags);
    if (cancelPolicy != MouseEventNoCancel)
        d->cancelPolicies.insert(type,cancelPolicy);
}

void MWidget::gestureEvent(QGestureEvent *event)
{
    Q_D(MWidget);

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

        if (gesture->state() != Qt::GestureStarted || event->isAccepted(gesture)) {
            // Gesture was accepted, let's see if we should cancel mouse event at this time.

            bool initiateMouseCancel = false;

            if (gesture->state() == Qt::GestureStarted &&
                d->cancelPolicies.value(gesture->gestureType()) == MouseEventCancelOnGestureStarted)
                initiateMouseCancel = true;

            if (gesture->state() == Qt::GestureFinished &&
                d->cancelPolicies.value(gesture->gestureType()) == MouseEventCancelOnGestureFinished)
                initiateMouseCancel = true;

            if (initiateMouseCancel)    {
                MScene *mScene = qobject_cast<MScene *>(scene());
                if (mScene)
                    mScene->d_func()->notifyChildRequestedMouseCancel();
            }
        }
    }
}

void MWidget::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    event->ignore(gesture);
}

void MWidget::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
    event->ignore(gesture);
}

void MWidget::pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture)
{
    event->ignore(gesture);
}

void MWidget::tapGestureEvent(QGestureEvent *event, QTapGesture* gesture)
{
    event->ignore(gesture);
}

void MWidget::swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture)
{
    event->ignore(gesture);
}

bool MWidget::event(QEvent *event)
{
    QEvent::Type type = event->type();
    switch(type) {
        case QEvent::ActionAdded:
        case QEvent::ActionChanged:
        case QEvent::ActionRemoved:
            actionEvent(static_cast<QActionEvent *>(event));
            break;
        case QEvent::LanguageChange:
            // retranslate the ui strings of this MWidget:
            retranslateUi();
            break;
        case QEvent::Gesture:
            gestureEvent(static_cast<QGestureEvent*>(event));
            break;
        case QEvent::LayoutRequest: {
            //When you have a widget inside a layout in a widget inside a layout in a widget...
            //and the innermost widget or layout's sizehint is invalidated only the parent is notified in the next
            //event loop. And that will notify its parent in the event loop after, and so on.  It can thus take many iterations
            //of the event loop before the layout has settled down, leading to a visual 'jumping around' effect.
            //So we manually invalidate the sizehint for all the parents immediately.
            QGraphicsLayoutItem *item = this;
            do {
                item->updateGeometry();
            } while( (item = item->parentLayoutItem()) );
            break;
        }
        default:
           if(type >= QEvent::User) {
               if (type == MCancelEvent::eventNumber())
                   cancelEvent((MCancelEvent *)event);
               else if (type == MOrientationChangeEvent::eventNumber())
                   orientationChangeEvent( static_cast<MOrientationChangeEvent *>(event) );
               else if (type == MOnDisplayChangeEvent::eventNumber())
                   onDisplayChangeEvent( static_cast<MOnDisplayChangeEvent *>(event) );
           }
    }
    return QGraphicsWidget::event(event);
}

bool MWidget::sceneEvent(QEvent *event)
{
    QEvent::Type type = event->type();
    switch(type) {
        case QEvent::TouchBegin:
           if(acceptTouchEvents()) {
               event->setAccepted(true);
               return true;
           }
           break;
        default:
           break;
    }
    return QGraphicsWidget::sceneEvent(event);
}

void MWidget::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
}

void MWidget::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_UNUSED(event);
}

void MWidget::actionEvent(QActionEvent *)
{
}

void MWidget::retranslateUi()
{
    // do nothing
}

void MWidget::setSelected(bool selected)
{
    Q_D(MWidget);

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

bool MWidget::isSelected() const
{
    Q_D(const MWidget);

    return d->selected;
}

void MWidget::setVisible(bool visible)
{
    Q_D(MWidget);
    // FIXME: The proper way of handling this would be the listening to QGraphicsItem::ItemVisibleChange
    // instead of overriding the non-virtual QGraphicsItem::setVisible(), but doing that would cause an ABI
    // break for those who inherit directly from MWidget.

    // If we are already explicitly hidden, early out.
    if (!visible && d->explicitlyHidden)
        return;

    d->explicitlyHidden = !visible;

    // Only show if the layout is not hiding this
    if (!d->layoutHidden) {
        // If current visibility has changed or we're explicitly hidden
        // we must change the visibility.
        // d->explicitlyHidden is hit when QGraphicsItem::setVisible(false) has been
        // called since the code in MWidget::setVisible() won’t be executed.
        if (visible != isVisible() || d->explicitlyHidden)
            d->setVisible(visible);
    }
}

QPointF MWidget::paintOffset() const
{
    return QPointF(transform().dx(), transform().dy());
}

void MWidget::setPaintOffset(const QPointF &newOffset)
{
    setTransform(QTransform::fromTranslate(newOffset.x(), newOffset.y()));
}
