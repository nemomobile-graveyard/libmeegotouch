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

#include "mhomebuttonpanel.h"
#include "mscenewindowmodel.h"
#include "mscenewindow_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MHomeButtonPanel)

MHomeButtonPanel::MHomeButtonPanel(QGraphicsItem *parent) :
    MSceneWindow(new MSceneWindowPrivate,
                   new MSceneWindowModel,
                   MSceneWindow::HomeButtonPanel, QString(""), parent)
{
    setObjectName("MHomeButtonPanel");
}

MHomeButtonPanel::~MHomeButtonPanel()
{
}

MHomeButtonPanel::MHomeButtonPanel(MSceneWindowPrivate *dd, MSceneWindowModel *model,
                                       MSceneWindow::WindowType windowType,
                                       const QString &viewType, QGraphicsItem *parent) :
    MSceneWindow(dd, model, windowType, viewType, parent)
{
}
