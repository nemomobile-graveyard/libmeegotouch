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

#include "ut_mwidgetanimation.h"
#include <MLabel>
#include <MButton>
#include <QEasingCurve>
#include <QRectF>

void Ut_MWidgetAnimation::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MWidgetAnimation" };
    app = new MApplication(argc, app_name);
}

void Ut_MWidgetAnimation::cleanupTestCase()
{
    delete app;
}

void Ut_MWidgetAnimation::init()
{
    animation = new MWidgetAnimation ();
}

void Ut_MWidgetAnimation::cleanup()
{
    delete animation;
}

void Ut_MWidgetAnimation::testAddWidget()
{
    QList <QGraphicsWidget *> list;

    MLabel *label1 = new MLabel ("test1");
    MLabel *label2 = new MLabel ("test2");
    MButton *button1 = new MButton ("button1");

    list << label1 << button1 << label2;

    animation->addWidget(label1);
    animation->addWidget(button1);
    animation->addWidget(label2);

    //check number of elements
    QCOMPARE (((MWidgetAnimationPrivate *)(animation->d_ptr))->animations.count(), list.count());

    QList <QGraphicsWidget *> tmpList  = ((MWidgetAnimationPrivate *)(animation->d_ptr))->animations.keys ();

    //check if elements in the list are correct
    while (list.count() > 0)
    {
        QVERIFY (tmpList.contains(list.takeFirst()));
    };

    list.clear ();
    tmpList.clear ();

    delete label1;
    delete label2;
    delete button1;
}

void Ut_MWidgetAnimation::testRemoveWidget()
{
    QList <QGraphicsWidget *> list;

    MLabel *label1 = new MLabel ("test1");
    MLabel *label2 = new MLabel ("test2");
    MButton *button1 = new MButton ("button1");

    list << label1 << button1 << label2;

    animation->addWidget(label1);
    animation->addWidget(button1);
    animation->addWidget(label2);

    //check number of elements
    QCOMPARE (((MWidgetAnimationPrivate *)(animation->d_ptr))->animations.count(), list.count());

    animation->removeWidget(list.takeAt(1));

    //check the number of animations
    QCOMPARE (((MWidgetAnimationPrivate *)(animation->d_ptr))->animations.count(), list.count());

    QList <QGraphicsWidget *> tmpList  = ((MWidgetAnimationPrivate *)(animation->d_ptr))->animations.keys ();

    //check if elements in the list are correct
    while (list.count() > 0)
    {
        QVERIFY (tmpList.contains(list.takeFirst()));
    };

    list.clear ();
    tmpList.clear ();

    delete label1;
    delete label2;
    delete button1;
}

void Ut_MWidgetAnimation::testSetDuration()
{
    animation->setDuration(DURATION);

    QCOMPARE (((MWidgetAnimationPrivate *)(animation->d_ptr))->duration, DURATION);
}


void Ut_MWidgetAnimation::testSetEasingCurve ()
{
    QEasingCurve curve;
    animation->setEasingCurve (curve);

    QCOMPARE (((MWidgetAnimationPrivate *)(animation->d_ptr))->easingCurve, curve);

    //not checking easing curves for individual animations because, it should be done in different unit tests
}


void Ut_MWidgetAnimation::testSetTargetValue ()
{
    MLabel label ("test");

    animation->setTargetValue(&label, "opacity", 1);
}

void Ut_MWidgetAnimation::testSetTargetGeometry ()
{
    MLabel label ("test");

    animation->setTargetGeometry(&label, QRectF (QPoint (1,1), QPoint (50,50)));
}

void Ut_MWidgetAnimation::testSetTargetOpacity ()
{
    MLabel label ("test");

    animation->setTargetOpacity (&label, 0.5);
}

void Ut_MWidgetAnimation::testSetTargetSize ()
{
    MLabel label ("test");

    animation->setTargetSize (&label, QSizeF (10,10));
}

void Ut_MWidgetAnimation::testSetTargetPosition ()
{
    MLabel label ("test");

    animation->setTargetPosition(&label, QPointF (50, 50));
}


QTEST_APPLESS_MAIN(Ut_MWidgetAnimation)
