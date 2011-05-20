/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_mstatusbarstylenamehandler.h"
#include <mstatusbarstylenamehandler.h>
#include <mstatusbar.h>
#include <msheet.h>
#include <mdialog.h>
#include <mscenewindowevent_p.h>

Ut_MStatusBarStyleNameHandler::Ut_MStatusBarStyleNameHandler(QObject *parent)
    : QObject(parent),
      statusBarStyleNameHandler(0),
      statusBar(0),
      sheet(0)
{
}

void Ut_MStatusBarStyleNameHandler::init()
{
    statusBar = new MStatusBar;
    statusBar->setStyleName("regular");

    sheet = new MSheet;

    statusBarStyleNameHandler = new MStatusBarStyleNameHandler;
    statusBarStyleNameHandler->setRegularStyleName("regular");
    statusBarStyleNameHandler->setStyleNameForSheets("forSheets");
    statusBarStyleNameHandler->setStatusBar(statusBar);
}

void Ut_MStatusBarStyleNameHandler::cleanup()
{
    delete statusBarStyleNameHandler;
    statusBarStyleNameHandler = 0;

    delete statusBar;
    statusBar = 0;

    delete sheet;
    sheet = 0;
}

void Ut_MStatusBarStyleNameHandler::testStatusBarHasDifferentStyleWhenSheetInForeground()
{
    sendAppearEvent(statusBar);

    QCOMPARE(statusBar->styleName(), QString("regular"));

    sendAppearEvent(sheet);

    QCOMPARE(statusBar->styleName(), QString("forSheets"));

    sendDisappearEvent(sheet);

    QCOMPARE(statusBar->styleName(), QString("regular"));
}

void Ut_MStatusBarStyleNameHandler::testStatusBarHasRegularStyleWhenSheetIsBlocked()
{
    MDialog *dialog = new MDialog;

    sendAppearEvent(statusBar);
    sendAppearEvent(sheet);

    QCOMPARE(statusBar->styleName(), QString("forSheets"));

    sendAppearEvent(dialog);

    QCOMPARE(statusBar->styleName(), QString("regular"));

    sendDisappearEvent(dialog);

    QCOMPARE(statusBar->styleName(), QString("forSheets"));

    delete dialog;
}

void Ut_MStatusBarStyleNameHandler::sendAppearEvent(MSceneWindow *sceneWindow)
{
    MSceneWindowEvent *sceneWindowEvent =
            new MSceneWindowEvent(MSceneWindowEvent::eventTypeAppear(),
                                  sceneWindow, false);
    statusBarStyleNameHandler->sceneWindowAppearEvent(sceneWindowEvent);
    delete sceneWindowEvent;
}

void Ut_MStatusBarStyleNameHandler::sendDisappearEvent(MSceneWindow *sceneWindow)
{
    MSceneWindowEvent *sceneWindowEvent =
            new MSceneWindowEvent(MSceneWindowEvent::eventTypeDisappear(),
                                  sceneWindow, false);
    statusBarStyleNameHandler->sceneWindowDisappearEvent(sceneWindowEvent);
    delete sceneWindowEvent;
}


QTEST_MAIN(Ut_MStatusBarStyleNameHandler)