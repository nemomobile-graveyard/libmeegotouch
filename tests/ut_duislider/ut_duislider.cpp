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

#include "ut_duislider.h"
#include <duiapplication.h>
#include <duislider.h>
#include <duisliderview.h>

#include <QDebug>

void Ut_DuiSlider::sliderSetMinIndicatorVisibility()
{
    DuiSlider *s = new DuiSlider;

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

void Ut_DuiSlider::sliderSetMaxIndicatorVisibility()
{
    DuiSlider *s = new DuiSlider;

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

void Ut_DuiSlider::sliderSetHandleIndicatorVisibility()
{
    DuiSlider *s = new DuiSlider;

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

void Ut_DuiSlider::sliderSetMinIndicatorText()
{
    DuiSlider *s = new DuiSlider;

    QString maxLabelText = s->maxLabelText();
    QString handleLabelText = s->handleLabelText();

    s->setMinLabel("minText");

    QCOMPARE(s->minLabelText(), QString("minText"));

    QCOMPARE(s->maxLabelText(), maxLabelText);
    QCOMPARE(s->handleLabelText(), handleLabelText);

    delete s;
}

void Ut_DuiSlider::sliderSetMaxIndicatorText()
{
    DuiSlider *s = new DuiSlider;

    QString minLabelText = s->minLabelText();
    QString handleLabelText = s->handleLabelText();

    s->setMaxLabel("maxText");

    QCOMPARE(s->maxLabelText(), QString("maxText"));

    QCOMPARE(s->minLabelText(), minLabelText);
    QCOMPARE(s->handleLabelText(), handleLabelText);

    delete s;
}

void Ut_DuiSlider::sliderSetHandleIndicatorText()
{
    DuiSlider *s = new DuiSlider;

    QString minLabelText = s->minLabelText();
    QString maxLabelText = s->maxLabelText();

    s->setHandleLabel("handleText");

    QCOMPARE(s->handleLabelText(), QString("handleText"));

    QCOMPARE(s->minLabelText(), minLabelText);
    QCOMPARE(s->maxLabelText(), maxLabelText);

    delete s;
}

void Ut_DuiSlider::sliderSetMinImageID()
{
    DuiSlider *s = new DuiSlider;

    QString maxLabelImageID = s->maxLabelIconID();
    QString handleLabelImageID = s->handleLabelIconID();

    s->setMinLabelIconID("minIcon");

    QCOMPARE(s->minLabelIconID(), QString("minIcon"));

    QCOMPARE(s->maxLabelIconID(), maxLabelImageID);
    QCOMPARE(s->handleLabelIconID(), handleLabelImageID);

    delete s;
}

void Ut_DuiSlider::sliderSetMaxImageID()
{
    DuiSlider *s = new DuiSlider;

    QString minLabelImageID = s->minLabelIconID();
    QString handleLabelImageID = s->handleLabelIconID();

    s->setMaxLabelIconID("maxIcon");

    QCOMPARE(s->maxLabelIconID(), QString("maxIcon"));

    QCOMPARE(s->minLabelIconID(), minLabelImageID);
    QCOMPARE(s->handleLabelIconID(), handleLabelImageID);

    delete s;
}

void Ut_DuiSlider::sliderSetHandleImageID()
{
    DuiSlider *s = new DuiSlider;

    QString minLabelImageID = s->minLabelIconID();
    QString maxLabelImageID = s->maxLabelIconID();

    s->setHandleLabelIconID("handleIcon");

    QCOMPARE(s->handleLabelIconID(), QString("handleIcon"));

    QCOMPARE(s->minLabelIconID(), minLabelImageID);
    QCOMPARE(s->maxLabelIconID(), maxLabelImageID);

    delete s;
}

void Ut_DuiSlider::sliderSetMinimum()
{
    DuiSlider *s = new DuiSlider;

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

    delete s;
}

void Ut_DuiSlider::sliderSetMaximum()
{
    DuiSlider *s = new DuiSlider;

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

    delete s;
}

void Ut_DuiSlider::sliderSetRange()
{
    DuiSlider *s = new DuiSlider;

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

    delete s;
}

void Ut_DuiSlider::sliderSetValue()
{
    DuiSlider *s = new DuiSlider;

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

void Ut_DuiSlider::sliderSetSteps()
{
    DuiSlider *s = new DuiSlider;

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

    delete s;
}

void Ut_DuiSlider::sliderOrientation()
{
    DuiSlider *s = new DuiSlider;

    s->setOrientation(Qt::Vertical);
    QCOMPARE(s->orientation(), Qt::Vertical);

    s->setOrientation(Qt::Horizontal);
    QCOMPARE(s->orientation(), Qt::Horizontal);

    delete s;
}

void Ut_DuiSlider::sliderState()
{
    DuiSlider *s = new DuiSlider;

    s->setState(DuiSliderModel::Pressed);
    QCOMPARE(s->state(), DuiSliderModel::Pressed);

    s->setState(DuiSliderModel::Released);
    QCOMPARE(s->state(), DuiSliderModel::Released);

    delete s;
}

QTEST_APPLESS_MAIN(Ut_DuiSlider)

