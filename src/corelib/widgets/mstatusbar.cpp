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

#include <QGraphicsSceneMouseEvent>
#include <QDBusInterface>

#include "mstatusbar.h"
#include "mstatusbar_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MStatusBar)

const QString MStatusBarPrivate::STATUS_INDICATOR_MENU_DBUS_SERVICE = "com.meego.core.MStatusIndicatorMenu";
const QString MStatusBarPrivate::STATUS_INDICATOR_MENU_DBUS_PATH = "/statusindicatormenu";
const QString MStatusBarPrivate::STATUS_INDICATOR_MENU_DBUS_INTERFACE = "com.meego.core.MStatusIndicatorMenu";
const int MStatusBarPrivate::SWIPE_THRESHOLD = 30;

MStatusBar::MStatusBar() : MSceneWindow(new MStatusBarPrivate, new MSceneWindowModel, MSceneWindow::StatusBar)
{
    setObjectName("statusBar");
}

MStatusBar::~MStatusBar()
{
}

bool MStatusBar::sceneEvent(QEvent *event)
{
    Q_D(MStatusBar);

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *> (event);
        if (mouseEvent != NULL) {
            d->firstPos = mouseEvent->pos();
        }
        return true;
    }
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *> (event);
        if (mouseEvent != NULL) {
            d->lastPos = mouseEvent->pos();
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease: {
        if (d->firstPos.y() + MStatusBarPrivate::SWIPE_THRESHOLD < d->lastPos.y()) {
            d->showStatusIndicatorMenu();
            d->lastPos = QPointF();
            return true;
        }
        break;
    }
    default:
        break;
    }

    return MSceneWindow::sceneEvent(event);
}

MStatusBarPrivate::MStatusBarPrivate() : MSceneWindowPrivate()
{
}

MStatusBarPrivate::~MStatusBarPrivate()
{
}

void MStatusBarPrivate::showStatusIndicatorMenu()
{
    QDBusInterface interface(STATUS_INDICATOR_MENU_DBUS_SERVICE, STATUS_INDICATOR_MENU_DBUS_PATH, STATUS_INDICATOR_MENU_DBUS_INTERFACE, QDBusConnection::sessionBus());
    interface.call(QDBus::NoBlock, "open");
}
