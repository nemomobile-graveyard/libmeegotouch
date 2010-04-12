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

#include "mmodalscenewindow.h"
#include "mmodalscenewindow_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MModalSceneWindow)

MModalSceneWindowPrivate::MModalSceneWindowPrivate()
{
}

MModalSceneWindowPrivate::~MModalSceneWindowPrivate()
{
}

void MModalSceneWindowPrivate::init()
{
}

MModalSceneWindow::MModalSceneWindow() :
    MSceneWindow(new MModalSceneWindowPrivate(), new MModalSceneWindowModel(), MSceneWindow::ModalSceneWindow, QString())
{
    Q_D(MModalSceneWindow);

    d->init();
}

MModalSceneWindow::MModalSceneWindow(MModalSceneWindowPrivate *dd, MModalSceneWindowModel *model, MSceneWindow::WindowType windowType) :
    MSceneWindow(dd, model, windowType, QString())
{
    Q_D(MModalSceneWindow);

    d->init();
}

MModalSceneWindow::~MModalSceneWindow()
{
}

#include "moc_mmodalscenewindow.cpp"
