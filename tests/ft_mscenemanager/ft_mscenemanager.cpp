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

#include "ft_mscenemanager.h"
#include <corelib/scene/mscenemanager.h>
#include <QtTest>
#include <MApplication>
#include <MApplicationWindow>
#include <MScene>

MApplication *app;
MApplicationWindow *w;

void Ft_MSceneManager::init()
{
    m_subject = MApplication::activeApplicationWindow()->sceneManager();
    QVERIFY(m_subject != NULL);
}

void Ft_MSceneManager::cleanup()
{
}

void Ft_MSceneManager::initTestCase()
{
    static int argc = 1;
    static char *argv[1] =  { (char *) "./ft_mscenemanager" };
    app = new MApplication(argc, argv);
    w = new MApplicationWindow();
}

void Ft_MSceneManager::cleanupTestCase()
{
    delete w;
    delete app;
}

void Ft_MSceneManager::test_scene()
{
    MScene *scene = m_subject->scene();
    QVERIFY(scene != 0);
}

QTEST_APPLESS_MAIN(Ft_MSceneManager)
