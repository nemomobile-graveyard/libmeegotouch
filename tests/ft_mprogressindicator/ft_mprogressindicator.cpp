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

#include "ft_mprogressindicator.h"
#include <QtTest>
#include <QImage>
#include "mapplication.h"
#include "mprogressindicator.h"

#define DEBUG

MApplication *app;

void Ft_MProgressIndicator::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ft_mslider" };
    app = new MApplication(argc, argv);
}

void Ft_MProgressIndicator::cleanupTestCase()
{
    delete app;
}

void Ft_MProgressIndicator::init()
{
    m_subject = new MProgressIndicator();
}

void Ft_MProgressIndicator::cleanup()
{
    delete m_subject;
}

void Ft_MProgressIndicator::setValue()
{
    // init
    m_subject->setMinimum(2);
    m_subject->setMaximum(7);
    m_subject->setValue(5);

    // set same value, it shouln't change and it shouldn't emit signal
    QSignalSpy spy(m_subject, SIGNAL(valueChanged(int)));
    m_subject->setValue(5);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(m_subject->value(), 5);

    // set different value, it should change and it should emit signal
    m_subject->setValue(6);
    QCOMPARE(m_subject->value(), 6);
    QCOMPARE(spy.count(), 1);

    // set value over the range, it shouldn't change and it shouldn't emit signal
    m_subject->setValue(8);
    QCOMPARE(m_subject->value(), 6);
    QCOMPARE(spy.count(), 1);

    // set value below the range, it shouldn't change and it shouldn't emit signal
    m_subject->setValue(1);
    QCOMPARE(m_subject->value(), 6);
    QCOMPARE(spy.count(), 1);
}

void Ft_MProgressIndicator::setMinimum()
{
    // init
    m_subject->setMinimum(2);
    m_subject->setMaximum(7);
    m_subject->setValue(5);

    // set minimum range so, that the value is not in the range anymore
    // it should reset it to minimum value
    m_subject->setMinimum(6);
    QCOMPARE(m_subject->minimum(), 6);
    QCOMPARE(m_subject->value(), m_subject->minimum());

    // set minimum so, that it's over the maximum
    // it should set the maximum so that the range is valid and it should reset the indicator
    m_subject->setMinimum(8);
    QCOMPARE(m_subject->minimum(), 8);
    QCOMPARE(m_subject->maximum(), m_subject->minimum());
    QCOMPARE(m_subject->value(), m_subject->minimum());
}

void Ft_MProgressIndicator::setMaximum()
{
    // init
    m_subject->setMinimum(2);
    m_subject->setMaximum(7);
    m_subject->setValue(5);

    // set maximum range so, that the value is not in the range anymore
    // it should reset it to maximum value
    m_subject->setMaximum(4);
    QCOMPARE(m_subject->maximum(), 4);
    QCOMPARE(m_subject->value(), m_subject->minimum());

    // set maximum so, that it's above minimum
    // it should set the minimum so that the range is valid and it should reset the indicator
    m_subject->setMaximum(1);
    QCOMPARE(m_subject->maximum(), 1);
    QCOMPARE(m_subject->minimum(), m_subject->maximum());
    QCOMPARE(m_subject->value(), m_subject->minimum());
}

void Ft_MProgressIndicator::unknown()
{
    // enable unknown mode and check that it was enabled
    m_subject->setUnknownDuration(true);
    QCOMPARE(m_subject->unknownDuration(), true);

    // disable unknown mode and check that it was disable
    m_subject->setUnknownDuration(false);
    QCOMPARE(m_subject->unknownDuration(), false);
}

void Ft_MProgressIndicator::reset()
{
    // set some values to the progress indicator
    m_subject->setMinimum(2);
    m_subject->setMaximum(7);
    m_subject->setValue(5);
    m_subject->setUnknownDuration(true);

    // reset it
    m_subject->reset();

    // check that it was reset properly, value should equal minimum
    QCOMPARE(m_subject->minimum(), 2);
    QCOMPARE(m_subject->maximum(), 7);
    QCOMPARE(m_subject->value(), m_subject->minimum());
    QCOMPARE(m_subject->unknownDuration(), false);
}


QTEST_APPLESS_MAIN(Ft_MProgressIndicator)

