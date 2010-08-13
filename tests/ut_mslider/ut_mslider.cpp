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

#include "ut_mslider.h"
#include <mapplication.h>
#include <mslider.h>
#include <msliderview.h>

#include <QDebug>

void Ut_MSlider::sliderSetMinIndicatorVisibility()
{
    MSlider *s = new MSlider;

    bool maxIndicatorVisible = s->isMaxLabelVisible();
    bool handleIndicatorVisible = s->isHandleLabelVisible();

    s->setMinLabelVisible(true);
    QCOMPARE(s->isMinLabelVisible(), true);

    s->setMinLabelVisible(false);
    QCOMPARE(s->isMinLabelVisible(), false);

    QCOMPARE(s->isMaxLabelVisible(), maxIndicatorVisible);
    QCOMPARE(s->isHandleLabelVisible(), handleIndicatorVisible);

    delete s;
}

void Ut_MSlider::sliderSetMaxIndicatorVisibility()
{
    MSlider *s = new MSlider;

    bool minIndicatorVisible = s->isMinLabelVisible();
    bool handleIndicatorVisible = s->isHandleLabelVisible();

    s->setMaxLabelVisible(true);
    QCOMPARE(s->isMaxLabelVisible(), true);

    s->setMaxLabelVisible(false);
    QCOMPARE(s->isMaxLabelVisible(), false);

    QCOMPARE(s->isMinLabelVisible(), minIndicatorVisible);
    QCOMPARE(s->isHandleLabelVisible(), handleIndicatorVisible);

    delete s;
}

void Ut_MSlider::sliderSetHandleIndicatorVisibility()
{
    MSlider *s = new MSlider;

    bool minIndicatorVisible = s->isMinLabelVisible();
    bool maxIndicatorVisible = s->isMaxLabelVisible();

    s->setHandleLabelVisible(true);
    QCOMPARE(s->isHandleLabelVisible(), true);

    s->setHandleLabelVisible(false);
    QCOMPARE(s->isHandleLabelVisible(), false);

    QCOMPARE(s->isMinLabelVisible(), minIndicatorVisible);
    QCOMPARE(s->isMaxLabelVisible(), maxIndicatorVisible);

    delete s;
}

void Ut_MSlider::sliderSetMinIndicatorText()
{
    MSlider *s = new MSlider;

    QString maxLabelText = s->maxLabelText();
    QString handleLabelText = s->handleLabelText();

    s->setMinLabel("minText");

    QCOMPARE(s->minLabelText(), QString("minText"));

    QCOMPARE(s->maxLabelText(), maxLabelText);
    QCOMPARE(s->handleLabelText(), handleLabelText);

    delete s;
}

void Ut_MSlider::sliderSetMaxIndicatorText()
{
    MSlider *s = new MSlider;

    QString minLabelText = s->minLabelText();
    QString handleLabelText = s->handleLabelText();

    s->setMaxLabel("maxText");

    QCOMPARE(s->maxLabelText(), QString("maxText"));

    QCOMPARE(s->minLabelText(), minLabelText);
    QCOMPARE(s->handleLabelText(), handleLabelText);

    delete s;
}

void Ut_MSlider::sliderSetHandleIndicatorText()
{
    MSlider *s = new MSlider;

    QString minLabelText = s->minLabelText();
    QString maxLabelText = s->maxLabelText();

    s->setHandleLabel("handleText");

    QCOMPARE(s->handleLabelText(), QString("handleText"));

    QCOMPARE(s->minLabelText(), minLabelText);
    QCOMPARE(s->maxLabelText(), maxLabelText);

    delete s;
}

void Ut_MSlider::sliderSetMinImageID()
{
    MSlider *s = new MSlider;

    QString maxLabelImageID = s->maxLabelIconID();
    QString handleLabelImageID = s->handleLabelIconID();

    s->setMinLabelIconID("minIcon");

    QCOMPARE(s->minLabelIconID(), QString("minIcon"));

    QCOMPARE(s->maxLabelIconID(), maxLabelImageID);
    QCOMPARE(s->handleLabelIconID(), handleLabelImageID);

    delete s;
}

void Ut_MSlider::sliderSetMaxImageID()
{
    MSlider *s = new MSlider;

    QString minLabelImageID = s->minLabelIconID();
    QString handleLabelImageID = s->handleLabelIconID();

    s->setMaxLabelIconID("maxIcon");

    QCOMPARE(s->maxLabelIconID(), QString("maxIcon"));

    QCOMPARE(s->minLabelIconID(), minLabelImageID);
    QCOMPARE(s->handleLabelIconID(), handleLabelImageID);

    delete s;
}

void Ut_MSlider::sliderSetHandleImageID()
{
    MSlider *s = new MSlider;

    QString minLabelImageID = s->minLabelIconID();
    QString maxLabelImageID = s->maxLabelIconID();

    s->setHandleLabelIconID("handleIcon");

    QCOMPARE(s->handleLabelIconID(), QString("handleIcon"));

    QCOMPARE(s->minLabelIconID(), minLabelImageID);
    QCOMPARE(s->maxLabelIconID(), maxLabelImageID);

    delete s;
}

void Ut_MSlider::sliderSetMinimum()
{
    MSlider *s = new MSlider;

    int max = s->maximum();

    s->setMinimum(max - 1);

    QCOMPARE(s->minimum(), max - 1);

    QCOMPARE(s->maximum(), max);
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setMinimum(s->maximum() + 1);
    QVERIFY(s->minimum() <= s->maximum());
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setRange(50, 100);
    s->setSteps(10);
    s->setValue(75);
    QCOMPARE(s->value(), 75);

    s->setMinimum(0);
    QCOMPARE(s->value(), 80);

    delete s;
}

void Ut_MSlider::sliderSetMaximum()
{
    MSlider *s = new MSlider;

    int min = s->minimum();

    s->setMaximum(min + 1);

    QCOMPARE(s->maximum(), min + 1);

    QCOMPARE(s->minimum(), min);
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setMaximum(s->minimum() - 1);
    QVERIFY(s->minimum() <= s->maximum());
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setRange(0, 50);
    s->setSteps(10);
    s->setValue(25);
    QCOMPARE(s->value(), 25);

    s->setMaximum(100);
    QCOMPARE(s->value(), 30);

    delete s;
}

void Ut_MSlider::sliderSetRange()
{
    MSlider *s = new MSlider;

    s->setRange(10, 20);
    QCOMPARE(s->minimum(), 10);
    QCOMPARE(s->maximum(), 20);
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setRange(20, 10);
    QCOMPARE(s->minimum(), 20);
    QCOMPARE(s->maximum(), 20);
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setRange(20, 10);
    s->setValue(30);
    QCOMPARE(s->minimum(), 20);
    QCOMPARE(s->maximum(), 20);
    QCOMPARE(s->value(), 20);

    s->setRange(10, 5);
    s->setValue(3);
    QCOMPARE(s->minimum(), 10);
    QCOMPARE(s->maximum(), 10);
    QCOMPARE(s->value(), 10);

   s->setRange(0, 50);
   s->setSteps(10);
   s->setValue(25);
   QCOMPARE(s->value(), 25);

   s->setRange(0, 100);
   QCOMPARE(s->value(), 30);

    delete s;
}

void Ut_MSlider::sliderSetValue()
{
    MSlider *s = new MSlider;

    s->setMinimum(20);
    s->setMaximum(30);

    int min = s->minimum();
    int max = s->maximum();
    QCOMPARE(min, 20);
    QCOMPARE(max, 30);

    s->setValue((min + max) / 2);

    QCOMPARE(s->value(), (min + max) / 2);
    QCOMPARE(s->minimum(), min);
    QCOMPARE(s->maximum(), max);

    s->setValue(s->minimum() - 1);
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    QCOMPARE(s->minimum(), min);
    QCOMPARE(s->maximum(), max);

    s->setValue(s->maximum() + 1);
    QVERIFY(s->value() >= s->minimum());
    QVERIFY(s->value() <= s->maximum());

    s->setMinimum(20);
    s->setMaximum(20);
    QCOMPARE(s->minimum(), 20);
    QCOMPARE(s->maximum(), 20);
    QCOMPARE(s->value(), 20);

    s->setMinimum(20);
    s->setMaximum(10) ;
    s->setValue(30);
    QCOMPARE(s->minimum(), 10);
    QCOMPARE(s->maximum(), 10);
    QCOMPARE(s->value(), 10);

    s->setMinimum(10);
    s->setMaximum(5);
    s->setValue(3);
    QCOMPARE(s->minimum(), 5);
    QCOMPARE(s->maximum(), 5);
    QCOMPARE(s->value(), 5);

    delete s;
}

void Ut_MSlider::sliderSetSteps()
{
    MSlider *s = new MSlider;

    s->setMinimum(1);
    s->setMaximum(5);

    int min = s->minimum();
    int max = s->maximum();
    QCOMPARE(min, 1);
    QCOMPARE(max, 5);

    s->setSteps(2);

    QCOMPARE(s->steps(), 2);

    QCOMPARE(s->minimum(), min);
    QCOMPARE(s->maximum(), max);

    s->setValue(2);
    QCOMPARE(s->value(), 3);

    s->setRange(1, 7);

    min = s->minimum();
    max = s->maximum();
    QCOMPARE(min, 1);
    QCOMPARE(max, 7);

    s->setSteps(-2);

    QCOMPARE(s->steps(), 2);

    QCOMPARE(s->minimum(), min);
    QCOMPARE(s->maximum(), max);

    s->setValue(2);
    QCOMPARE(s->value(), 1);

    s->setMinimum(0);
    s->setMaximum(15);
    s->setSteps(10);

    s->setValue(1);
    QCOMPARE(s->value(), 2);

    s->setValue(3);
    QCOMPARE(s->value(), 3);

    delete s;
}

void Ut_MSlider::sliderOrientation()
{
    MSlider *s = new MSlider;

    s->setOrientation(Qt::Vertical);
    QCOMPARE(s->orientation(), Qt::Vertical);

    s->setOrientation(Qt::Horizontal);
    QCOMPARE(s->orientation(), Qt::Horizontal);

    delete s;
}

void Ut_MSlider::sliderState()
{
    MSlider *s = new MSlider;

    s->setState(MSliderModel::Pressed);
    QCOMPARE(s->state(), MSliderModel::Pressed);

    s->setState(MSliderModel::Released);
    QCOMPARE(s->state(), MSliderModel::Released);

    delete s;
}

QTEST_APPLESS_MAIN(Ut_MSlider)

