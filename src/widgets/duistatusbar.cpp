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

#include <QGraphicsSceneMouseEvent>
#include <QDBusInterface>
#include <QDebug>


#include "duistatusbar.h"
#include "duistatusbar_p.h"


#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiStatusBar)

const QString DuiStatusBarPrivate::STATUS_INDICATOR_MENU_DBUS_SERVICE = "com.meego.core.MStatusIndicatorMenu";
const QString DuiStatusBarPrivate::STATUS_INDICATOR_MENU_DBUS_PATH = "/statusindicatormenu";
const QString DuiStatusBarPrivate::STATUS_INDICATOR_MENU_DBUS_INTERFACE = "com.meego.core.MStatusIndicatorMenu";
const int DuiStatusBarPrivate::SWIPE_THRESHOLD = 30;

DuiStatusBar::DuiStatusBar()
    : DuiSceneWindow(new DuiStatusBarPrivate, new DuiSceneWindowModel, DuiSceneWindow::StatusBar)
{
    setObjectName("statusBar");
}

DuiStatusBar::~DuiStatusBar()
{
}

bool DuiStatusBar::sceneEvent(QEvent *event)
{
    Q_D(DuiStatusBar);

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *> (event);
        d->firstPos = mouseEvent->pos();
        return true;
    }
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *> (event);
        d->lastPos = mouseEvent->pos();
        return true;
    }
    case QEvent::GraphicsSceneMouseRelease: {
        if (d->firstPos.y() + DuiStatusBarPrivate::SWIPE_THRESHOLD < d->lastPos.y()) {
            d->showStatusIndicatorMenu();
            d->lastPos = QPointF();
            return true;
        }
        break;
    }
    default:
        break;
    }

    return DuiSceneWindow::sceneEvent(event);
}

DuiStatusBarPrivate::DuiStatusBarPrivate() : DuiSceneWindowPrivate()
{
}

DuiStatusBarPrivate::~DuiStatusBarPrivate()
{
}

void DuiStatusBarPrivate::showStatusIndicatorMenu()
{
    QDBusInterface interface(STATUS_INDICATOR_MENU_DBUS_SERVICE, STATUS_INDICATOR_MENU_DBUS_PATH, STATUS_INDICATOR_MENU_DBUS_INTERFACE, QDBusConnection::sessionBus());
    interface.call(QDBus::NoBlock, "open");
}
