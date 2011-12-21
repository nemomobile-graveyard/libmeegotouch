/***************************************************************************
**
** Copyright) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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


#include "ut_mwidgetscalefadeanimation.h"
#include "mwidgetscalefadeanimation.h"
#include "mapplication.h"
#include "mbutton.h"

void Ut_MWidgetScaleFadeAnimation::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MWidgetScaleFadeAnimation" };
    app = new MApplication(argc, app_name);
}

void Ut_MWidgetScaleFadeAnimation::cleanupTestCase()
{
    delete app;
}

void Ut_MWidgetScaleFadeAnimation::init()
{
    animation = new MWidgetScaleFadeAnimation ();
}

void Ut_MWidgetScaleFadeAnimation::cleanup()
{
    delete animation;
}

void Ut_MWidgetScaleFadeAnimation::testAnimationIn()
{
    MButton button;
    button.setOpacity(1);
    button.setScale(10);

    animation->setTargetWidget(&button);
    animation->setTransitionDirection(MAbstractWidgetAnimation::In);
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QCOMPARE(button.opacity(), animation->style()->opacity());
    QCOMPARE(button.scale(), 1.0);
}

void Ut_MWidgetScaleFadeAnimation::testAnimationOut()
{
    MButton button;
    button.setOpacity(1);
    button.setScale(1);

    animation->setTargetWidget(&button);
    animation->setTransitionDirection(MAbstractWidgetAnimation::Out);
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QCOMPARE(button.opacity(), 0.0);
    QCOMPARE(button.scale(), animation->style()->scale());
}

QTEST_APPLESS_MAIN(Ut_MWidgetScaleFadeAnimation)
