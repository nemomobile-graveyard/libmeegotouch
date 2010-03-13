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

#include "duistatusbar.h"
#include "duiscenewindow_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiStatusBar)

DuiStatusBar::DuiStatusBar()
    : DuiSceneWindow(new DuiSceneWindowPrivate, new DuiSceneWindowModel, DuiSceneWindow::StatusBar)
{
    setObjectName("statusBar");
}

DuiStatusBar::~DuiStatusBar()
{
}
