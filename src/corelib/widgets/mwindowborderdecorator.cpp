/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "mwindowborderdecorator.h"

#include <mborderdecoration.h>
#include <mscenemanager.h>

MWindowBorderDecorator::MWindowBorderDecorator()
{
    top = new MBorderDecoration;
    top->setStyleName("TopBorderDecoration");
    top->setObjectName(top->styleName());

    bottom = new MBorderDecoration;
    bottom->setStyleName("BottomBorderDecoration");
    bottom->setObjectName(bottom->styleName());
}

MWindowBorderDecorator::~MWindowBorderDecorator()
{
    delete top;
    delete bottom;
}

bool MWindowBorderDecorator::isDecorating() const
{
    MSceneWindow::SceneWindowState topState = top->sceneWindowState();
    Q_ASSERT(bottom->sceneWindowState() == topState);

    return (topState == MSceneWindow::Appeared || topState == MSceneWindow::Appearing);
}

void MWindowBorderDecorator::decorate(MSceneManager *sceneManager)
{
    sceneManager->appearSceneWindow(top);
    sceneManager->appearSceneWindow(bottom);
}

void MWindowBorderDecorator::removeDecorations()
{
    top->disappear();
    bottom->disappear();
}