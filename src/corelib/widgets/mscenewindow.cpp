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
#include "mscenemanager.h"
#include "mscenemanager_p.h"
#include "mapplication.h"
#include "mwidgetview.h"
#include "mwindow.h"
#include "mdialog.h"
#include "mcancelevent.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(MSceneWindow)

MSceneWindowPrivate::MSceneWindowPrivate()
        : windowType(MSceneWindow::PlainSceneWindow),
        policy(MSceneWindow::KeepWhenDone),
        sceneWindowState(MSceneWindow::Disappeared),
        managedManually(false),
        dismissed(false),
        waitingForContextMenuEvent(false),
        effect(0),
        appearanceAnimation(0),
        disappearanceAnimation(0),
        queuedTransition(0),
        sceneManager(0)
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

MSceneWindow::MSceneWindow(QGraphicsItem *parent) :
        MWidgetController(new MSceneWindowPrivate, new MSceneWindowModel, parent)
{
    Q_D(MSceneWindow);

    d->windowType = PlainSceneWindow;
    grabGesture(Qt::TapAndHoldGesture);
}


MSceneWindow::MSceneWindow(MSceneWindowPrivate *dd, MSceneWindowModel *model, MSceneWindow::WindowType windowType, const QString &viewType, QGraphicsItem *parent) :
        MWidgetController(dd, model, parent)
{
    Q_D(MSceneWindow);
    setViewType(viewType);

    d->windowType = windowType;
    grabGesture(Qt::TapAndHoldGesture);
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

    if (view()) {
        if (model()->disappearTimeout() != 0) {
            QTimer::singleShot(model()->disappearTimeout(), this, SLOT(disappear()));
        }
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
        QVariant v = view()->property("alignment");
        if (v.isValid()) {
            result = Qt::Alignment(v.toInt());
        }
        else {
        }
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
    Q_D(MSceneWindow);

    if (gesture->state() == Qt::GestureFinished) {

        QGraphicsSceneContextMenuEvent contextEvent(QEvent::GraphicsSceneContextMenu);
        contextEvent.setPos(gesture->hotSpot());
        contextEvent.setScenePos(gesture->hotSpot());
        contextEvent.setScreenPos(gesture->hotSpot().toPoint());

        d->waitingForContextMenuEvent = true;
        QApplication::sendEvent(scene(), &contextEvent);

        if (contextEvent.isAccepted() && d->waitingForContextMenuEvent) {
            //Event has been accepted by some widget on top of this scenewindow.
            if ((scene() == NULL) || (scene()->views().size() == 0)) {
                // If this widget has been removed from the scene and/or there
                // is no view, return
                return;
            }

            MCancelEvent cancelEvent;
            QList<QGraphicsItem*> affectedItems = scene()->items(gesture->hotSpot());
            QGraphicsItem *item = 0;

            foreach(item, affectedItems) {
                if (scene()->items().contains(item))
                    scene()->sendEvent(item, &cancelEvent);
            }
        }
        d->waitingForContextMenuEvent = false;

    }

    event->accept(gesture);
}

bool MSceneWindow::event(QEvent *event)
{
    Q_D(MSceneWindow);
    if (event->type() == MDismissEvent::eventType()) {
        dismissEvent(static_cast<MDismissEvent *>(event));
    } else if (event->type() == QEvent::GraphicsSceneContextMenu) {
        //Event was not accepted by any of our child widgets.
        //We need to accept it so that it doesn't propagate further and
        //clear the flag, so that the tap&hold gesture event handler
        //will know that the event wasn't delivered.
        if (d->waitingForContextMenuEvent) {
            event->accept();
            d->waitingForContextMenuEvent = false;
            return true;
        }
    } else if (event->type() == QEvent::ChildAdded) {
        QChildEvent *childEvent = static_cast<QChildEvent *>(event);
        if (childEvent->child()->objectName() == "_m_testBridge") {
            new MSceneWindowTestInterface(d, childEvent->child());
        }
    }

    return MWidgetController::event(event);
}

MSceneWindowTestInterface::MSceneWindowTestInterface(MSceneWindowPrivate *d, QObject *parent)
    : QObject(parent), d(d)
{
}

void MSceneWindowTestInterface::setSceneWindowState(MSceneWindow::SceneWindowState newState)
{
    d->setSceneWindowState(newState);
}
