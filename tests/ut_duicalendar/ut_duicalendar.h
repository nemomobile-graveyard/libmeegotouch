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

#ifndef UT_CALENDAR_H
#define UT_CALENDAR_H


#include <QtTest/QtTest>
#include <QObject>
#include <DuiLocale>
#include <DuiCalendar>

Q_DECLARE_METATYPE(DuiLocale::CalendarType);
Q_DECLARE_METATYPE(DuiLocale);


#define MAX_PARAMS 10
class Ut_DuiCalendar : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *qap;
    QString icuPackageVersion;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDuiLocaleCalendar_data();
    void testDuiLocaleCalendar();

    void testDuiLocaleCalendarConversionsFromLocaltimeQDateTime_data();
    void testDuiLocaleCalendarConversionsFromLocaltimeQDateTime();

    void testDuiLocaleCalendarConversionsFromUTCQDateTime_data();
    void testDuiLocaleCalendarConversionsFromUTCQDateTime();

    void testDuiLocaleCalendarConversionsFromDuiCalendar_data();
    void testDuiLocaleCalendarConversionsFromDuiCalendar();

    void testDuiCalendarAdditions();
    void testWeekNumbers();
    void testComparisons();

    void testIslamicCalendar();

    void testPosixFormatPattern_data();
    void testPosixFormatPattern();

    void testWeekdaySymbols_data();
    void testWeekdaySymbols();

    void testMonthSymbols_data();
    void testMonthSymbols();
};


#endif
