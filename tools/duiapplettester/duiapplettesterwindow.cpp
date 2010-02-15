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

#include "duiapplettesterwindow.h"

#include <QResizeEvent>
#include <DuiScene>
#include <DuiSceneManager>

DuiAppletTesterWindow::DuiAppletTesterWindow()
{
    setScene(new DuiScene);
    setSceneRect(QRectF(QPointF(), visibleSceneSize(Dui::Landscape)));
    centerOn(sceneRect().center());
    DuiSceneManager *manager = new DuiSceneManager(scene());
    setSceneManager(manager);
}

DuiAppletTesterWindow::~DuiAppletTesterWindow()
{
}

void DuiAppletTesterWindow::resizeEvent(QResizeEvent *event)
{
    setSceneRect(QRectF(QPointF(), event->size()));
    DuiWindow::resizeEvent(event);
    emit resized(event->size());
}
