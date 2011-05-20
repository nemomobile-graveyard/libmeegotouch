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

#include "mstatusbarstylenamehandler.h"
#include <mscenewindow.h>
#include <mscenewindowevent_p.h>

MStatusBarStyleNameHandler::MStatusBarStyleNameHandler()
    : sheetsBeingShown(0), sceneWindowsBlockingSheets(0)
{
}

void MStatusBarStyleNameHandler::setStatusBar(MSceneWindow *statusBar)
{
    statusBarPointer = statusBar;
}

void MStatusBarStyleNameHandler::setRegularStyleName(const QString &regularStyleName)
{
    this->regularStyleName = regularStyleName;
}

void MStatusBarStyleNameHandler::setStyleNameForSheets(const QString &styleNameForSheets)
{
    this->styleNameForSheets = styleNameForSheets;
}

void MStatusBarStyleNameHandler::sceneWindowAppearEvent(MSceneWindowEvent *ev)
{
    if (statusBarPointer.isNull())
        return;

    switch (ev->sceneWindow()->windowType()) {
    case MSceneWindow::Sheet:
        ++sheetsBeingShown;
        updateStatusBarStyleName();
        break;
    case MSceneWindow::Dialog:
    case MSceneWindow::MessageBox:
    case MSceneWindow::ObjectMenu:
    case MSceneWindow::PopupList:
        ++sceneWindowsBlockingSheets;
        updateStatusBarStyleName();
        break;
    default:
        // do nothing
        break;
    }
}

void MStatusBarStyleNameHandler::sceneWindowDisappearEvent(MSceneWindowEvent *ev)
{
    if (statusBarPointer.isNull())
        return;

    switch (ev->sceneWindow()->windowType()) {
    case MSceneWindow::Sheet:
        --sheetsBeingShown;
        updateStatusBarStyleName();
        break;
    case MSceneWindow::Dialog:
    case MSceneWindow::MessageBox:
    case MSceneWindow::ObjectMenu:
    case MSceneWindow::PopupList:
        --sceneWindowsBlockingSheets;
        updateStatusBarStyleName();
        break;
    default:
        // do nothing
        break;
    }
}

void MStatusBarStyleNameHandler::updateStatusBarStyleName()
{
    Q_ASSERT(!statusBarPointer.isNull());
    Q_ASSERT(sheetsBeingShown >= 0);
    Q_ASSERT(sceneWindowsBlockingSheets >= 0);

    MSceneWindow *statusBar = statusBarPointer.data();

    if (sheetsBeingShown > 0 && sceneWindowsBlockingSheets == 0
            && statusBar->styleName() != styleNameForSheets) {
        statusBar->setStyleName(styleNameForSheets);

    } else if ((sheetsBeingShown == 0 || sceneWindowsBlockingSheets > 0)
               && statusBar->styleName() != regularStyleName) {
        statusBar->setStyleName(regularStyleName);
    }
}