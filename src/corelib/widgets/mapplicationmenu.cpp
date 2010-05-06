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
#include <MSceneManager>

#include "mapplicationmenu.h"
#include "mapplicationmenu_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MApplicationMenu)

MApplicationMenuPrivate::MApplicationMenuPrivate()
{
}

MApplicationMenuPrivate::~MApplicationMenuPrivate()
{
}

MApplicationMenu::MApplicationMenu(const QString &viewType)
    : MSceneWindow(new MApplicationMenuPrivate(),
                     new MApplicationMenuModel(),
                     MSceneWindow::ApplicationMenu,
                     viewType)
{
}

MApplicationMenu::~MApplicationMenu()
{
}

void MApplicationMenu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MSceneWindow::mousePressEvent(event);
    event->accept();
}

void MApplicationMenu::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MSceneWindow::mouseReleaseEvent(event);
    if (isVisible()) {
        dismiss();
    }
}

void MApplicationMenu::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}
