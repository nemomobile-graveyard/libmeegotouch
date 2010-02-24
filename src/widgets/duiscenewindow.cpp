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

#include <DuiDebug>
#include <DuiDismissEvent>
#include "duiscenewindow.h"
#include "duiscenewindowmodel.h"
#include "duiscenewindow_p.h"
#include "duiscene.h"
#include "duiscenemanager.h"
#include "duiscenemanager_p.h"
#include "duiapplication.h"
#include "duiscenewindowview.h"
#include "duiwindow.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(DuiSceneWindow)

DuiSceneWindowPrivate::DuiSceneWindowPrivate()
{
    managedManually = false;
    shown = false;
    dismissed = false;
    policy = DuiSceneWindow::KeepWhenDone;
    effect = NULL;
}

void DuiSceneWindowPrivate::appear(bool now, DuiWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_Q(DuiSceneWindow);

    if (!window) {
        window = DuiApplication::activeWindow();
        if (!window) {
            // TODO: Create and show() a Dui[Application]Window on the fly?
            duiWarning("DuiSceneWindow")
                    << "Construct and show DuiWindow before showing a scene window";
            return;
        }
    }

    if (now) {
        window->sceneManager()->showWindowNow(q, policy);
    } else {
        window->sceneManager()->showWindow(q, policy);
    }
}

DuiSceneWindow::DuiSceneWindow(QGraphicsItem *parent) :
    DuiWidgetController(new DuiSceneWindowPrivate, new DuiSceneWindowModel, parent)
{
    Q_D(DuiSceneWindow);

    d->windowType = PlainSceneWindow;

    // TODO: Remove this along with deprecated windowShown() and windowHidden()
    connect(this, SIGNAL(appeared()), SIGNAL(windowShown()));
    connect(this, SIGNAL(disappeared()), SIGNAL(windowHidden()));
}

bool DuiSceneWindowPrivate::dismiss(bool now)
{
    Q_Q(DuiSceneWindow);

    /* ABI FREEZE: Release this
    DuiDismissEvent dismissEvent;
    QApplication::sendEvent(q, &dismissEvent);

    if (!dismissEvent.isAccepted()) {
        return false;
    }
    */

    if (q->sceneManager()) {
        if (now) {
            q->sceneManager()->closeWindowNow(q);
        } else {
            q->sceneManager()->closeWindow(q);
        }
    }

    return true;
}


DuiSceneWindow::DuiSceneWindow(DuiSceneWindowPrivate *dd, DuiSceneWindowModel *model, DuiSceneWindow::WindowType windowType, const QString &viewType, QGraphicsItem *parent) :
    DuiWidgetController(dd, model, parent)
{
    Q_D(DuiSceneWindow);
    setViewType(viewType);

    d->windowType = windowType;

    // TODO: Remove this along with deprecated windowShown() and windowHidden()
    connect(this, SIGNAL(appeared()), SIGNAL(windowShown()));
    connect(this, SIGNAL(disappeared()), SIGNAL(windowHidden()));
}

DuiSceneWindow::~DuiSceneWindow()
{
    if (sceneManager())
        sceneManager()->d_func()->detachWindow(this);
}

DuiSceneWindow::WindowType DuiSceneWindow::windowType() const
{
    Q_D(const DuiSceneWindow);
    return d->windowType;
}

DuiSceneWindow::DeletionPolicy DuiSceneWindow::deletionPolicy() const
{
    Q_D(const DuiSceneWindow);
    return d->policy;
}

bool DuiSceneWindow::isManagedManually() const
{
    Q_D(const DuiSceneWindow);
    return d->managedManually;
}

void DuiSceneWindow::setManagedManually(bool managedManually)
{
    Q_D(DuiSceneWindow);
    d->managedManually = managedManually;
}

void DuiSceneWindow::appear(DuiWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiSceneWindow);
    d->appear(false, window, policy);
}

void DuiSceneWindow::appearNow(DuiWindow *window, DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiSceneWindow);
    d->appear(true, window, policy);
}

void DuiSceneWindow::appear(DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiSceneWindow);
    d->appear(false, 0, policy);
}

void DuiSceneWindow::appearNow(DuiSceneWindow::DeletionPolicy policy)
{
    Q_D(DuiSceneWindow);
    d->appear(true, 0, policy);
}

void DuiSceneWindow::disappear()
{
    if (sceneManager())
        sceneManager()->hideWindow(this);
}

void DuiSceneWindow::disappearNow()
{
    if (sceneManager())
        sceneManager()->hideWindowNow(this);
}

Qt::Alignment DuiSceneWindow::alignment() const
{
    Qt::Alignment result = 0;

    if (view()) {
        const DuiSceneWindowView *sceneWindowView =
            qobject_cast<const DuiSceneWindowView *>(view());

        if (sceneWindowView) {
            result = sceneWindowView->alignment();
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

QPointF DuiSceneWindow::offset() const
{
    QPointF result;

    if (view()) {
        const DuiSceneWindowView *sceneWindowView =
            qobject_cast<const DuiSceneWindowView *>(view());

        if (sceneWindowView) {
            result = sceneWindowView->offset();
        }
    } else {
        result = QPointF(0, 0);
    }

    return result;
}

bool DuiSceneWindow::dismiss()
{
    Q_D(DuiSceneWindow);
    return d->dismiss(false);
}

bool DuiSceneWindow::dismissNow()
{
    Q_D(DuiSceneWindow);
    return d->dismiss(true);
}

/* ABI FREEZE: Release this
void DuiSceneWindow::dismissEvent(DuiDismissEvent *event)
{
    event->accept();
}
*/

void DuiSceneWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    dismiss();
}

bool DuiSceneWindow::event(QEvent *event)
{
    /* ABI FREEZE: Release this
    if (event->type() == DuiDismissEvent::eventType()) {
        dismissEvent(static_cast<DuiDismissEvent *>(event));
    }
    */

    return DuiWidgetController::event(event);
}
