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
#include <DuiSceneManager>

#include "duiapplicationmenu.h"
#include "duiapplicationmenu_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiApplicationMenu)

DuiApplicationMenuPrivate::DuiApplicationMenuPrivate()
{
}

DuiApplicationMenuPrivate::~DuiApplicationMenuPrivate()
{
}

DuiApplicationMenu::DuiApplicationMenu(const QString &viewType)
    : DuiSceneWindow(new DuiApplicationMenuPrivate(),
                     new DuiApplicationMenuModel(),
                     DuiSceneWindow::ApplicationMenu,
                     viewType)
{
}

DuiApplicationMenu::~DuiApplicationMenu()
{
}

void DuiApplicationMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    DuiSceneWindow::mousePressEvent(event);
    event->accept();
}

void DuiApplicationMenu::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    DuiSceneWindow::mouseReleaseEvent(event);
    if (isVisible()) {
        dismiss();
    }
}

void DuiApplicationMenu::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}
