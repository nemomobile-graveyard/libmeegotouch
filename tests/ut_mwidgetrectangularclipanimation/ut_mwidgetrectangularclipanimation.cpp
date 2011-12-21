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


#include "ut_mwidgetrectangularclipanimation.h"
#include "mwidgetrectangularclipanimation.h"
#include "mwidgetrectangularclipanimation_p.h"
#include "mapplication.h"
#include "mbutton.h"
#include "mgraphicsclipeffect.h"

void Ut_MWidgetRectangularClipAnimation::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MWidgetRectangularClipAnimation" };
    app = new MApplication(argc, app_name);
}

void Ut_MWidgetRectangularClipAnimation::cleanupTestCase()
{
    delete app;
}

void Ut_MWidgetRectangularClipAnimation::init()
{
    animation = new MWidgetRectangularClipAnimation ();
}

void Ut_MWidgetRectangularClipAnimation::cleanup()
{
    delete animation;
}

void Ut_MWidgetRectangularClipAnimation::testAnimationIn()
{
    MButton button;

    animation->setTargetWidget(&button);
    animation->setTransitionDirection(MAbstractWidgetAnimation::In);
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QRect startRect = QRect(0.0, button.size().height() / 2.f, button.size().width(), 0.0);
    QRect endRect = QRect(0.0, 0.0, button.size().width(), button.size().height());

    QCOMPARE(button.graphicsEffect(), animation->d_func()->clipEffect);
    QCOMPARE(startRect, animation->d_func()->clipAnimation->startValue().toRect());
    QCOMPARE(endRect, animation->d_func()->clipAnimation->endValue().toRect());
    QCOMPARE(endRect, animation->d_func()->clipEffect->drawRect());

    animation->restoreTargetWidgetState();
    QVERIFY(!button.graphicsEffect());
}

void Ut_MWidgetRectangularClipAnimation::testAnimationOut()
{
    MButton button;

    animation->setTargetWidget(&button);
    animation->setTransitionDirection(MAbstractWidgetAnimation::Out);
    animation->start();
    animation->setCurrentTime(animation->totalDuration());

    QRect startRect = QRect(0.0, 0.0, button.size().width(), button.size().height());
    QRect endRect = QRect(0.0, button.size().height() / 2.f, button.size().width(), 0.0);

    QCOMPARE(button.graphicsEffect(), animation->d_func()->clipEffect);
    QCOMPARE(startRect, animation->d_func()->clipAnimation->startValue().toRect());
    QCOMPARE(endRect, animation->d_func()->clipAnimation->endValue().toRect());
    QCOMPARE(endRect, animation->d_func()->clipEffect->drawRect());

    animation->restoreTargetWidgetState();
    QVERIFY(!button.graphicsEffect());
}

QTEST_APPLESS_MAIN(Ut_MWidgetRectangularClipAnimation)
