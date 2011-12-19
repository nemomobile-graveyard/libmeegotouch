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


#include "ut_mwidgetzoomanimation.h"
#include "mwidgetzoomanimation.h"
#include "mapplication.h"
#include "mbutton.h"

void Ut_MWidgetZoomAnimation::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MWidgetZoomAnimation" };
    app = new MApplication(argc, app_name);
}

void Ut_MWidgetZoomAnimation::cleanupTestCase()
{
    delete app;
}

void Ut_MWidgetZoomAnimation::init()
{
    animation = new MWidgetZoomAnimation ();
}

void Ut_MWidgetZoomAnimation::cleanup()
{
    delete animation;
}

void Ut_MWidgetZoomAnimation::testOrigin()
{
    QPointF origin(0, 0);
    animation->setOrigin(origin);

    QCOMPARE(origin, animation->origin());
}

void Ut_MWidgetZoomAnimation::testAnimationIn()
{
    MButton button;
    QPointF buttonPos(0, 0);
    button.setPos(buttonPos);
    button.setOpacity(1);
    button.setScale(10);

    animation->setTargetWidget(&button);
    animation->setTransitionDirection(MAbstractWidgetAnimation::In);
    animation->setOrigin(QPointF(100, 100));
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QCOMPARE(button.opacity(), animation->style()->opacity());
    QCOMPARE(button.scale(), 1.0);
    QCOMPARE(buttonPos, button.pos());
}

void Ut_MWidgetZoomAnimation::testAnimationOut()
{
    MButton button;
    QPointF buttonPos(0, 0);
    button.setPos(buttonPos);
    button.setOpacity(1);
    button.setScale(1);

    animation->setTargetWidget(&button);
    animation->setTransitionDirection(MAbstractWidgetAnimation::Out);
    QPointF origin(100, 100);
    animation->setOrigin(origin);
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QPointF distance = origin - buttonPos;
    QPointF finalPos = buttonPos + distance * (1 - animation->style()->positionAnimationDistanceFactor());

    QCOMPARE(button.opacity(), 0.0);
    QCOMPARE(button.scale(), animation->style()->scale());
    QCOMPARE(button.pos(), finalPos);
}

QTEST_APPLESS_MAIN(Ut_MWidgetZoomAnimation)
