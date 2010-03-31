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
#include "duiwidgetview.h"
#include "duiwindow.h"
#include "duidialog.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(DuiSceneWindow)

Q_DECLARE_METATYPE(Qt::Alignment)

DuiSceneWindowPrivate::DuiSceneWindowPrivate()
{
    managedManually = false;
    shown = false;
    dismissed = false;
    policy = DuiSceneWindow::KeepWhenDone;
    effect = NULL;
    windowType = DuiSceneWindow::PlainSceneWindow;
    appearanceAnimation = 0;
    disappearanceAnimation = 0;
}

DuiSceneWindow::DuiSceneWindow(QGraphicsItem *parent) :
    DuiWidgetController(new DuiSceneWindowPrivate, new DuiSceneWindowModel, parent)
{
    Q_D(DuiSceneWindow);

    d->windowType = PlainSceneWindow;
}


DuiSceneWindow::DuiSceneWindow(DuiSceneWindowPrivate *dd, DuiSceneWindowModel *model, DuiSceneWindow::WindowType windowType, const QString &viewType, QGraphicsItem *parent) :
    DuiWidgetController(dd, model, parent)
{
    Q_D(DuiSceneWindow);
    setViewType(viewType);

    d->windowType = windowType;
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
    if (!window) {
        window = DuiApplication::activeWindow();
        if (!window) {
            // TODO: Create and show() a Dui[Application]Window on the fly?
            duiWarning("DuiSceneWindow")
                    << "Construct and show DuiWindow before showing a scene window";
            return;
        }
    }

    window->sceneManager()->appearSceneWindow(this, policy);
}

void DuiSceneWindow::appear(DuiSceneWindow::DeletionPolicy policy)
{
    appear(0, policy);
}

void DuiSceneWindow::disappear()
{
    if (sceneManager())
        sceneManager()->disappearSceneWindow(this);
}

Qt::Alignment DuiSceneWindow::alignment() const
{
    Qt::Alignment result = 0;

    // There is an implicit property interface between the controller and the views
    // The controller expects the view to provide an "alignment" property.
    if (view()) {
        QVariant v = view()->property("alignment");
        if (v.isValid()) {
            result = qvariant_cast<Qt::Alignment>(v);
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

QPointF DuiSceneWindow::offset() const
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

bool DuiSceneWindow::dismiss()
{
    DuiDismissEvent dismissEvent;
    QApplication::sendEvent(this, &dismissEvent);

    if (!dismissEvent.isAccepted()) {
        return false;
    }

    if (sceneManager()) {
        sceneManager()->dismissSceneWindow(this);
    }

    return true;
}

void DuiSceneWindow::dismissEvent(DuiDismissEvent *event)
{
    event->accept();
}

void DuiSceneWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    dismiss();
}

bool DuiSceneWindow::event(QEvent *event)
{
    if (event->type() == DuiDismissEvent::eventType()) {
        dismissEvent(static_cast<DuiDismissEvent *>(event));
    }

    return DuiWidgetController::event(event);
}
