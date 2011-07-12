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

#ifndef UT_MSCENEWINDOWSCROLLER_H
#define UT_MSCENEWINDOWSCROLLER_H

#include <QtTest/QtTest>
#include <QObject>

class MSceneWindowScroller;
class MSceneWindow;
class MApplication;
class MApplicationWindow;

class Ut_MSceneWindowScroller : public QObject
{
    Q_OBJECT

public:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testQueryScrollingAmount();
    void testApplyScrolling();
    void testRestoreScrolling();

private:
    MApplication *app;
    MSceneWindowScroller *scroller;
    MApplicationWindow *appWindow;
    MSceneWindow *sceneWindow;
};

#endif
