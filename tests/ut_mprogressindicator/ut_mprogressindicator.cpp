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

#include "ut_mprogressindicator.h"

#include <QDebug>

void Ut_MProgressIndicator::progressIndicatorSetMinimum()
{
    MProgressIndicator *s = new MProgressIndicator();
    int max = s->maximum();

    s->setMinimum(max - 1);
    QVERIFY(s->minimum() <= s->maximum());

    s->setMinimum(max + 1);
    QVERIFY(s->minimum() <= s->maximum());

    s->setMinimum(s->maximum());

    QVERIFY(s->minimum() == s->maximum());
}

void Ut_MProgressIndicator::progressIndicatorSetMaximum()
{
    MProgressIndicator *s = new MProgressIndicator();
    int min = s->minimum();

    s->setMaximum(min + 1);
    QVERIFY(s->minimum() <= s->maximum());

    s->setMinimum(min - 1);
    QVERIFY(s->minimum() <= s->maximum());

    s->setMaximum(s->minimum());

    QVERIFY(s->minimum() == s->maximum());
}

void Ut_MProgressIndicator::progressIndicatorSetRange()
{
    MProgressIndicator *s = new MProgressIndicator();

    s->setRange(10, 20);

    QCOMPARE(s->minimum(), 10);
    QCOMPARE(s->maximum(), 20);

    s->setRange(3, 1);

    QCOMPARE(s->minimum(), 3);
    QCOMPARE(s->maximum(), 3);

    delete s;
}

void Ut_MProgressIndicator::progressIndicatorSetRangeNegative()
{
    MProgressIndicator *s = new MProgressIndicator();

    s->setRange(-10, -20);

    QCOMPARE(s->minimum(), -10);
    QCOMPARE(s->maximum(), -10);


    s->setRange(-3, -1);

    QCOMPARE(s->minimum(), -3);
    QCOMPARE(s->maximum(), -1);

    delete s;
}

void Ut_MProgressIndicator::progressIndicatorOutOfRange()
{
    MProgressIndicator *s = new MProgressIndicator();

    s->setRange(1, 5);

    QSignalSpy spy(s, SIGNAL(valueChanged(int)));
    s->setValue((s->minimum() + s->maximum()) / 2);
    QCOMPARE(spy.count(), 1);

    QVERIFY(s->minimum() <= s->value());
    QVERIFY(s->maximum() >= s->value());

    spy.clear();

    s->setMinimum(s->value() + 1);
    QCOMPARE(spy.count(), 1);

    //when value falls out of range, progress indicator
    //is reset ie its value is set to its minimum
    QVERIFY(s->minimum() == s->value());
    QVERIFY(s->maximum() >= s->value());

    spy.clear();

    s->setRange(s->value() - 3, s->value() - 2);
    QCOMPARE(spy.count(), 1);

    //mprogressindicator reset
    QVERIFY(s->minimum() == s->value());
    QVERIFY(s->maximum() >= s->value());

    spy.clear();

    s->setRange(s->value() + 27, s->value() + 23);
    QCOMPARE(spy.count(), 1);

    //mprogressindicator reset
    QVERIFY(s->minimum() == s->value());
    QVERIFY(s->maximum() >= s->value());
    QVERIFY(s->minimum() <= s->maximum());
}

/*void Ut_MSeekBar::seekbarTestOutOfLoadedContentRange()
{
    MSeekBar *s = new MSeekBar();

    QSignalSpy spy(s, SIGNAL(outOfLoadedContentRange()));

    s->setRange(10, 20);
    s->setValue(15);
    s->setLoadedContentMinimum(1);
    s->setLoadedContentMaximum(3);

    QCOMPARE(s->minimum(), 10);
    QCOMPARE(s->maximum(), 20);
    QCOMPARE(s->value(), 15);
    QCOMPARE(s->loadedContentMinimum(), 1);
    QCOMPARE(s->loadedContentMaximum(), 3);

    QCOMPARE(spy.count(), 3);

    spy.clear();

    s->setRange(0, 20);
    QCOMPARE(s->minimum(), 0);
    QCOMPARE(s->maximum(), 20);

    s->setValue(2);
    QCOMPARE(s->value(), 2);
    QCOMPARE(spy.count(), 0);

    s->setLoadedContentMinimum(2);
    QCOMPARE(s->loadedContentMinimum(), 2);
    QCOMPARE(spy.count(), 0);

    s->setLoadedContentMaximum(2);
    QCOMPARE(s->loadedContentMaximum(), 2);
    QCOMPARE(spy.count(), 0);

    s->setValue(4);
    QCOMPARE(s->value(), 4);
    QCOMPARE(spy.count(), 1);

    spy.clear();

    s->setLoadedContentRange(2, 7);
    QCOMPARE(s->loadedContentMinimum(), 2);
    QCOMPARE(s->loadedContentMaximum(), 7);
    QCOMPARE(spy.count(), 0);

    s->setLoadedContentMinimum(5);
    QCOMPARE(s->loadedContentMinimum(), 5);
    QCOMPARE(spy.count(), 1);

    spy.clear();

    s->setLoadedContentMinimum(2);
    QCOMPARE(s->loadedContentMinimum(), 2);
    QCOMPARE(spy.count(), 0);

    s->setLoadedContentMaximum(3);
    QCOMPARE(s->loadedContentMaximum(), 3);
    QCOMPARE(spy.count(), 1);

    spy.clear();

    s->setLoadedContentMaximum(6);
    QCOMPARE(s->loadedContentMaximum(), 6);
    QCOMPARE(spy.count(), 0);

    delete s;
}*/

QTEST_APPLESS_MAIN(Ut_MProgressIndicator)

