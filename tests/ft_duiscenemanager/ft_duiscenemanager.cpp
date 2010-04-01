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

#include "ft_duiscenemanager.h"
#include <corelib/scene/duiscenemanager.h>
#include <QtTest>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiScene>

DuiApplication *app;
DuiApplicationWindow *w;

void Ft_DuiSceneManager::init()
{
    m_subject = DuiApplication::activeApplicationWindow()->sceneManager();
    QVERIFY(m_subject != NULL);
}

void Ft_DuiSceneManager::cleanup()
{
}

void Ft_DuiSceneManager::initTestCase()
{
    static int argc = 1;
    static char *argv[1] =  { (char *) "./ft_duiscenemanager" };
    app = new DuiApplication(argc, argv);
    w = new DuiApplicationWindow();
}

void Ft_DuiSceneManager::cleanupTestCase()
{
    delete w;
    delete app;
}

void Ft_DuiSceneManager::test_scene()
{
    DuiScene *scene = m_subject->scene();
    QVERIFY(scene != 0);
}

QTEST_APPLESS_MAIN(Ft_DuiSceneManager)
