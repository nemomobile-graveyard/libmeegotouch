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

#include <MDebug>
#include <MDismissEvent>
#include <QChildEvent>
#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QGraphicsSceneContextMenuEvent>
#include <QTimer>
#include "mscenewindow.h"
#include "mscenewindowmodel.h"
#include "mscenewindow_p.h"
#include "mscene.h"
#include "mscene_p.h"
#include "mscenemanager.h"
#include "mscenemanager_p.h"
#include "mapplication.h"
#include "mwidgetview.h"
#include "mwindow.h"
#include "mdialog.h"
#include "mcancelevent.h"

#include "mmetatypes.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(MSceneWindow)


MSceneWindowPrivate::MSceneWindowPrivate()
        : windowType(MSceneWindow::PlainSceneWindow),
        policy(MSceneWindow::KeepWhenDone),
        sceneWindowState(MSceneWindow::Disappeared),
        managedManually(false),
        dismissed(false),
        effect(0),
        displacementItem(0),
        appearanceAnimation(0),
        disappearanceAnimation(0),
        queuedTransition(0),
        sceneManager(0),
        focusItemBeforeWindowBlocked(0)
{
}

void MSceneWindowPrivate::setSceneWindowState(MSceneWindow::SceneWindowState newState)
{
    Q_Q(MSceneWindow);

    if (newState == sceneWindowState)
        return;

    MSceneWindow::SceneWindowState oldState = sceneWindowState;

    sceneWindowState = newState;

    emit q->sceneWindowStateChanged(newState, oldState);

    switch (newState) {
        case MSceneWindow::Appearing:
            emit q->appearing();
            break;

        case MSceneWindow::Appeared:
            startDisappearTimeout();
            emit q->appeared();
            break;

        case MSceneWindow::Disappearing:
            emit q->disappearing();
            break;

        case MSceneWindow::Disappeared:
            emit q->disappeared();
            break;

        default:
            break;
    }
}

void MSceneWindowPrivate::startDisappearTimeout()
{
    Q_Q(MSceneWindow);

    if (q->view()) {
        if (q->model()->disappearTimeout() != 0) {
            QTimer::singleShot(q->model()->disappearTimeout(), q, SLOT(disappear()));
        }
    }
}

MSceneWindow::MSceneWindow(QGraphicsItem *parent) :
        MWidgetController(new MSceneWindowPrivate, new MSceneWindowModel, parent)
{
    Q_D(MSceneWindow);

    d->windowType = PlainSceneWindow;
    setAcceptTouchEvents(true);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    setFlag(QGraphicsItem::ItemStopsClickFocusPropagation);
}


MSceneWindow::MSceneWindow(MSceneWindowPrivate *dd, MSceneWindowModel *model, MSceneWindow::WindowType windowType, const QString &viewType, QGraphicsItem *parent) :
        MWidgetController(dd, model, parent)
{
    Q_D(MSceneWindow);
    setViewType(viewType);

    d->windowType = windowType;
    setAcceptTouchEvents(true);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    setFlag(QGraphicsItem::ItemStopsClickFocusPropagation);
}

MSceneWindow::~MSceneWindow()
{
    if (sceneManager())
        sceneManager()->d_func()->removeSceneWindow(this);
}

MSceneWindow::WindowType MSceneWindow::windowType() const
{
    Q_D(const MSceneWindow);
    return d->windowType;
}

MSceneWindow::DeletionPolicy MSceneWindow::deletionPolicy() const
{
    Q_D(const MSceneWindow);
    return d->policy;
}

bool MSceneWindow::isManagedManually() const
{
    Q_D(const MSceneWindow);
    return d->managedManually;
}

void MSceneWindow::setManagedManually(bool managedManually)
{
    Q_D(MSceneWindow);
    d->managedManually = managedManually;
}

MSceneWindow::SceneWindowState MSceneWindow::sceneWindowState() const
{
    Q_D(const MSceneWindow);
    return d->sceneWindowState;
}

void MSceneWindow::appear(QGraphicsScene *scene, MSceneWindow::DeletionPolicy policy)
{
    if (!scene) {
        mWarning("MSceneWindow") << Q_FUNC_INFO << "NULL scene.";
        return;
    }

    MScene *mScene = qobject_cast<MScene *>(scene);
    if (!mScene || !mScene->sceneManager()) {
        mWarning("MSceneWindow") << Q_FUNC_INFO << "scene has no scene manager.";
        return;
    }

    mScene->sceneManager()->appearSceneWindow(this, policy);
}

void MSceneWindow::appear(MWindow *window, MSceneWindow::DeletionPolicy policy)
{
    if (!window) {
        // Remove this behavior along with the deprecated
        // MSceneWindow::appear(DeletionPolicy);
        window = MApplication::activeWindow();
        if (!window) {
            mWarning("MSceneWindow")
                    << "Construct and show MWindow before showing a scene window";
            return;
        }
    }

    // Force the creation of a scene manager (and scene) if none was
    // set to this window yet.
    //
    // Therefore:
    //    sceneWindow->appear(window);
    // Will yield a different result than:
    //    sceneWindow->appear(window->scene());
    // If window didn't have a scene and scene manager.
    window->sceneManager();

    appear(window->scene(), policy);
}

void MSceneWindow::appear(MSceneWindow::DeletionPolicy policy)
{
    mWarning("MSceneWindow") << Q_FUNC_INFO << "is deprecated."
             "Use appear(QGraphicsScene*) or appear(MWindow*) instead.";
    appear((MWindow *)0, policy);
}

void MSceneWindow::disappear()
{
    if (sceneManager())
        sceneManager()->disappearSceneWindow(this);
}

Qt::Alignment MSceneWindow::alignment() const
{
    Qt::Alignment result = 0;

    // There is an implicit property interface between the controller and the views
    // The controller expects the view to provide an "alignment" property.
    if (view()) {
        result = view()->property("alignment").value<Qt::Alignment>();
    }

    if (layoutDirection() == Qt::RightToLeft) {
        if (result.testFlag(Qt::AlignLeft)) {
            result &= ~Qt::AlignLeft;
            result |= Qt::AlignRight;
        } else if (result.testFlag(Qt::AlignRight)) {
            result &= ~Qt::AlignRight;
            result |= Qt::AlignLeft;
        }
    }

    return result;
}

QPointF MSceneWindow::offset() const
{
    QPointF result;

    // There is an implicit property interface between the controller and the views
    // The controller expects the view to provide an "offset" property.
    if (view()) {
        QVariant v = view()->property("offset");
        if (v.isValid()) {
            result = v.toPointF();
        }
    } else {
        result = QPointF(0, 0);
    }

    return result;
}

bool MSceneWindow::dismiss()
{
    MDismissEvent dismissEvent;
    QApplication::sendEvent(this, &dismissEvent);

    if (!dismissEvent.isAccepted()) {
        return false;
    }

    if (sceneManager()) {
        sceneManager()->dismissSceneWindow(this);
    }

    return true;
}

void MSceneWindow::dismissEvent(MDismissEvent *event)
{
    event->accept();
}

void MSceneWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    dismiss();
}

void MSceneWindow::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
    if (gesture->state() == Qt::GestureFinished) {

        QGraphicsSceneContextMenuEvent contextEvent(QEvent::GraphicsSceneContextMenu);
        contextEvent.setPos(gesture->position());
        contextEvent.setScenePos(gesture->position());
        contextEvent.setScreenPos(gesture->position().toPoint());
        contextEvent.setReason(QGraphicsSceneContextMenuEvent::Mouse);

        QApplication::sendEvent(scene(), &contextEvent);
    }

    event->accept(gesture);
}

bool MSceneWindow::event(QEvent *event)
{
    Q_D(MSceneWindow);
    if (event->type() == MDismissEvent::eventType()) {
        dismissEvent(static_cast<MDismissEvent *>(event));
    } else if (event->type() == QEvent::GraphicsSceneContextMenu) {

        //Event was not accepted by any of our child widgets. We want to
        //react to it, if we have actions added to us, but in the end
        //we always need to accept it so that it doesn't propagate.
        MWidgetController::event(event);
        event->accept();
        return true;

    } else if (event->type() == QEvent::ChildAdded) {
        QChildEvent *childEvent = static_cast<QChildEvent *>(event);
        if (childEvent->child()->objectName() == "_m_testBridge") {
            new MSceneWindowTestInterface(d, childEvent->child());
        }
    } else if (event->type() == QEvent::WindowBlocked) {
        // blocked scene windows must lose focus
        if (focusItem()) {
            if (focusItem()->isWidget())
                d->focusItemBeforeWindowBlocked = static_cast<QGraphicsWidget*>(focusItem());
            focusItem()->clearFocus();
        }
    } else if (event->type() == QEvent::WindowUnblocked) {
        // Unblocked scene window must restore the lost focus.
        if (d->focusItemBeforeWindowBlocked && !focusItem()) {
            d->focusItemBeforeWindowBlocked->setFocus(Qt::ActiveWindowFocusReason);
        }
        d->focusItemBeforeWindowBlocked = 0;
    }

    return MWidgetController::event(event);
}

void MSceneWindow::gestureEvent(QGestureEvent *event)
{
    MWidgetController::gestureEvent(event);

    foreach(QGesture* gesture, event->gestures()) {
        if (gesture->state() == Qt::GestureStarted && !event->isAccepted(gesture->gestureType())) {
            event->accept(gesture);
        }
    }
}

MSceneWindowTestInterface::MSceneWindowTestInterface(MSceneWindowPrivate *d, QObject *parent)
    : QObject(parent), d(d)
{
}

void MSceneWindowTestInterface::setSceneWindowState(MSceneWindow::SceneWindowState newState)
{
    d->setSceneWindowState(newState);
}
