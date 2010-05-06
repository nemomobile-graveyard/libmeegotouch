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

#include "mapplettesterwindow.h"

#include <QResizeEvent>
#include <MScene>
#include <MSceneManager>

MAppletTesterWindow::MAppletTesterWindow()
{
    setScene(new MScene);
    setSceneRect(QRectF(QPointF(), visibleSceneSize(M::Landscape)));
    centerOn(sceneRect().center());
    MSceneManager *manager = new MSceneManager(scene());
    setSceneManager(manager);
}

MAppletTesterWindow::~MAppletTesterWindow()
{
}

void MAppletTesterWindow::resizeEvent(QResizeEvent *event)
{
    setSceneRect(QRectF(QPointF(), event->size()));
    MWindow::resizeEvent(event);
    emit resized(event->size());
}
