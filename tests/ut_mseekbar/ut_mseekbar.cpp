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

#include "ut_mseekbar.h"

#include <QDebug>

void Ut_MSeekBar::seekbarSetLoadedContentMinimum()
{
    MSeekBar *s = new MSeekBar();

    int max = s->loadedContentMaximum();

    s->setLoadedContentMinimum(max - 1);

    QCOMPARE(s->loadedContentMinimum(), max - 1);
    QCOMPARE(s->loadedContentMaximum(), max);

    s->setLoadedContentMinimum(s->loadedContentMaximum() + 1);
    QVERIFY(s->loadedContentMinimum() <= s->loadedContentMaximum());

    s->setLoadedContentMaximum(1);
    s->setLoadedContentMinimum(2);
    QCOMPARE(s->loadedContentMinimum(), 2);
    QCOMPARE(s->loadedContentMaximum(), 2);

    delete s;
}

void Ut_MSeekBar::seekbarSetLoadedContentMaximum()
{
    MSeekBar *s = new MSeekBar();

    int min = s->loadedContentMinimum();

    s->setLoadedContentMaximum(min + 1);

    QCOMPARE(s->loadedContentMaximum(), min + 1);
    QCOMPARE(s->loadedContentMinimum(), min);

    s->setLoadedContentMaximum(s->loadedContentMinimum() - 1);
    QVERIFY(s->loadedContentMinimum() <= s->loadedContentMaximum());

    s->setLoadedContentMinimum(1);
    s->setLoadedContentMaximum(1);
    QCOMPARE(s->loadedContentMinimum(), 1);
    QCOMPARE(s->loadedContentMaximum(), 1);

    delete s;
}

void Ut_MSeekBar::seekbarSetLoadedContentRange()
{
    MSeekBar *s = new MSeekBar();

    s->setRange(10, 20);
    s->setLoadedContentRange(1, 3);

    QCOMPARE(s->minimum(), 10);
    QCOMPARE(s->maximum(), 20);

    QCOMPARE(s->loadedContentMinimum(), 1);
    QCOMPARE(s->loadedContentMaximum(), 3);

    s->setLoadedContentRange(3, 1);

    QCOMPARE(s->loadedContentMinimum(), 3);
    QCOMPARE(s->loadedContentMaximum(), 3);

    delete s;
}

void Ut_MSeekBar::seekbarTestOutOfLoadedContentRange()
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
}

QTEST_APPLESS_MAIN(Ut_MSeekBar)

