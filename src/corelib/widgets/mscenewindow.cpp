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
#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QGraphicsSceneContextMenuEvent>
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
      managedManually(false),
      shown(false),
      dismissed(false),
      effect(0),
      appearanceAnimation(0),
      disappearanceAnimation(0)
{
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
        sceneManager()->d_func()->detachWindow(this);
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

void MSceneWindow::appear(MWindow *window, MSceneWindow::DeletionPolicy policy)
{
    if (!window) {
        window = MApplication::activeWindow();
        if (!window) {
            // TODO: Create and show() a M[Application]Window on the fly?
            mWarning("MSceneWindow")
                    << "Construct and show MWindow before showing a scene window";
            return;
        }
    }

    window->sceneManager()->appearSceneWindow(this, policy);
}

void MSceneWindow::appear(MSceneWindow::DeletionPolicy policy)
{
    appear(0, policy);
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
    if (gesture->state() == Qt::GestureFinished) {

        QGraphicsSceneContextMenuEvent contextEvent(QEvent::GraphicsSceneContextMenu);
        contextEvent.setPos(gesture->hotSpot());
        contextEvent.setScenePos(gesture->hotSpot());
        contextEvent.setScreenPos(gesture->hotSpot().toPoint());

        QApplication::sendEvent(scene(), &contextEvent);

        if (contextEvent.isAccepted()) {
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
    }

    event->accept(gesture);
}

bool MSceneWindow::event(QEvent *event)
{
    if (event->type() == MDismissEvent::eventType()) {
        dismissEvent(static_cast<MDismissEvent *>(event));
    }

    return MWidgetController::event(event);
}
