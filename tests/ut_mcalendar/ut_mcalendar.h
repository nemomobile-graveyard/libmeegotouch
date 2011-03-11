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

#ifndef UT_CALENDAR_H
#define UT_CALENDAR_H


#include <QtTest/QtTest>
#include <QObject>
#include <MLocale>
#include <MCalendar>

#ifdef HAVE_ICU
#include <unicode/unistr.h>
#include <unicode/ucal.h>
#include <unicode/coll.h>
#include <unicode/fieldpos.h>
#include <unicode/datefmt.h>
#include <unicode/calendar.h>
#include <unicode/smpdtfmt.h> // SimpleDateFormat
#include <unicode/numfmt.h>
#include <unicode/uloc.h>
#include <unicode/dtfmtsym.h> // date format symbols
#include <unicode/putil.h> // u_setDataDirectory
#endif

Q_DECLARE_METATYPE(MLocale::TimeFormat24h);
Q_DECLARE_METATYPE(MLocale::CalendarType);
Q_DECLARE_METATYPE(MLocale);


#define MAX_PARAMS 10
class Ut_MCalendar : public QObject
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

    void testTimeZones();

    void testTimeZonesInCountry_data();
    void testTimeZonesInCountry();

    void testConversionFromAndToQDateTime_data();
    void testConversionFromAndToQDateTime();

    void testIcuFormatString_data();
    void testIcuFormatString();

    void testMLocaleSetAndGetCalendar_data();
    void testMLocaleSetAndGetCalendar();

    void testMLocaleCalendarConversionsFromLocaltimeQDateTime_data();
    void testMLocaleCalendarConversionsFromLocaltimeQDateTime();

    void testMLocaleCalendarConversionsFromUTCQDateTime_data();
    void testMLocaleCalendarConversionsFromUTCQDateTime();

    void testMLocaleCalendarConversionsFromMCalendar_data();
    void testMLocaleCalendarConversionsFromMCalendar();

    void testVariousSetDateTimeMethods_data();
    void testVariousSetDateTimeMethods();

    void testMCalendarAdditions();
    void testWeekNumbers();
    void testComparisons();

    void testIslamicCalendar();

    void testPosixFormatPattern_data();
    void testPosixFormatPattern();

    void testFormatDateTimeICU_data();
    void testFormatDateTimeICU();

    void testWeekdaySymbols_data();
    void testWeekdaySymbols();

    void testMonthSymbols_data();
    void testMonthSymbols();
};

#endif
