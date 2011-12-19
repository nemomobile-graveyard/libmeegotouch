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

//Mocking MThemePrivate, because the style object is private to library
//and we need to compile some functionality into the unittest.
#include "mtheme_p.h"
void MThemePrivate::unregisterStyleContainer(MStyleContainer*)
{
}

void MThemePrivate::registerStyleContainer(MStyleContainer *)
{
}


#include "ut_mwidgetmoveanimation.h"
#include "mwidgetmoveanimation.h"
#include "mapplication.h"
#include "mbutton.h"

void Ut_MWidgetMoveAnimation::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MWidgetMoveAnimation" };
    app = new MApplication(argc, app_name);
}

void Ut_MWidgetMoveAnimation::cleanupTestCase()
{
    delete app;
}

void Ut_MWidgetMoveAnimation::init()
{
    animation = new MWidgetMoveAnimation ();
}

void Ut_MWidgetMoveAnimation::cleanup()
{
    delete animation;
}

void Ut_MWidgetMoveAnimation::testAnimation()
{
    MButton button;
    button.setPos(QPointF(0, 0));

    QPointF finalPos(100, 100);
    animation->setWidget(&button);
    animation->setFinalPos(finalPos);
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QCOMPARE(button.pos(), finalPos);
}

QTEST_APPLESS_MAIN(Ut_MWidgetMoveAnimation)
