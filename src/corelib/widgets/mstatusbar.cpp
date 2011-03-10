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
#include "mstatusbar.h"
#include "mstatusbar_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MStatusBar)

MStatusBar::MStatusBar()
    : MSceneWindow(new MStatusBarPrivate, new MSceneWindowModel, MSceneWindow::StatusBar)
{
    setObjectName("statusBar");
}

MStatusBar::MStatusBar(QGraphicsItem *parent)
    : MSceneWindow(new MStatusBarPrivate, new MSceneWindowModel, MSceneWindow::StatusBar, QString(), parent)
{
    setObjectName("statusBar");
}

MStatusBar::~MStatusBar()
{
}

MStatusBarPrivate::MStatusBarPrivate() : MSceneWindowPrivate()
{
}

MStatusBarPrivate::~MStatusBarPrivate()
{
}

bool MStatusBar::sceneEvent(QEvent *event)
{
    return MSceneWindow::sceneEvent(event);
}
