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

#ifndef UT_MSTATUSBARSTYLENAMEHANDLER_H
#define UT_MSTATUSBARSTYLENAMEHANDLER_H

#include <QtTest/QtTest>
#include <QObject>

class MSceneWindow;
class MStatusBarStyleNameHandler;

class Ut_MStatusBarStyleNameHandler : public QObject
{
    Q_OBJECT

public:
    Ut_MStatusBarStyleNameHandler(QObject *parent = 0);

private slots:
    void init();
    void cleanup();

    void testStatusBarHasDifferentStyleWhenSheetInForeground();
    void testStatusBarHasRegularStyleWhenSheetIsBlocked();

private:
    void sendAppearEvent(MSceneWindow *sceneWindow);
    void sendDisappearEvent(MSceneWindow *sceneWindow);

    MStatusBarStyleNameHandler *statusBarStyleNameHandler;
    MSceneWindow *statusBar;
    MSceneWindow *sheet;
};

#endif
