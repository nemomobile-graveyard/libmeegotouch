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

#include <QCoreApplication>
#include <QTextCodec>
#include <MLocale>
#include <unicode/uversion.h>

#include "pt_mcalendar.h"

void Pt_MCalendar::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QProcess process;
    process.start("sh -c \"dpkg -s libicu44 | grep Version | perl -pe 's/^Version:[[:space:]]*([^[[:space:]]+)$/$1/g'\"");
    if (!process.waitForFinished()) {
        qDebug() << "cannot run process to check libicu44 package version , exiting ...";
        exit(1);
    }
    icuPackageVersion = process.readAllStandardOutput();
    icuPackageVersion.replace("\n", "");
    qDebug() << "libicu44 package version is:" << icuPackageVersion;
}

void Pt_MCalendar::cleanupTestCase()
{
    delete qap;
}

void Pt_MCalendar::init()
{
}

void Pt_MCalendar::cleanup()
{
}

void Pt_MCalendar::benchmarkFormatDateTimePosixFormatString_U_QDateTime()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // this overrides language
    QString lcNumeric("ar_SA");  // does matter, overrides localized numbers in dates
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::GregorianCalendar);
    MCalendar mcal(locale);
    int year = 2008;
    int month = 7;
    int day = 21;
    int hour = 14;
    int minute = 31;
    int second = 3;
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(language);
    QString format("%U");
    QString result("٢٩");
    QCOMPARE(locale.formatDateTime(mcal, format), result);
    QCOMPARE(locale.formatDateTime(datetime, format), result);

    QBENCHMARK {
        locale.formatDateTime(datetime, format);
    }
}

void Pt_MCalendar::benchmarkFormatDateTimePosixFormatString_U_MCalendar()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // this overrides language
    QString lcNumeric("ar_SA");  // does matter, overrides localized numbers in dates
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::GregorianCalendar);
    MCalendar mcal(locale);
    int year = 2008;
    int month = 7;
    int day = 21;
    int hour = 14;
    int minute = 31;
    int second = 3;
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(language);
    QString format("%U");
    QString result("٢٩");
    QCOMPARE(locale.formatDateTime(mcal, format), result);
    QCOMPARE(locale.formatDateTime(datetime, format), result);

    QBENCHMARK {
        locale.formatDateTime(mcal, format);
    }
}

void Pt_MCalendar::benchmarkFormatDateTimePosixFormatString_V_MCalendar()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // this overrides language
    QString lcNumeric("ar_SA");  // does matter, overrides localized numbers in dates
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::GregorianCalendar);
    MCalendar mcal(locale);
    int year = 2008;
    int month = 7;
    int day = 21;
    int hour = 14;
    int minute = 31;
    int second = 3;
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(language);
    QString format("%V");
    QString result("٣٠");
    QCOMPARE(locale.formatDateTime(mcal, format), result);
    QCOMPARE(locale.formatDateTime(datetime, format), result);

    QBENCHMARK {
        locale.formatDateTime(mcal, format);
    }
}

void Pt_MCalendar::benchmarkFormatDateTimePosixFormatString_r_MCalendar()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA@mix-time-and-language=no");     // this overrides language
    QString lcNumeric("ar_SA");  // does matter, overrides localized numbers in dates
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::GregorianCalendar);
    MCalendar mcal(locale);
    int year = 2008;
    int month = 7;
    int day = 21;
    int hour = 14;
    int minute = 31;
    int second = 3;
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(language);
    QString format("%r");
    QString result("٢:٣١ م");
    QCOMPARE(locale.formatDateTime(mcal, format), result);
    QCOMPARE(locale.formatDateTime(datetime, format), result);

    QBENCHMARK {
        locale.formatDateTime(mcal, format);
    }
}

void Pt_MCalendar::benchmarkFormatDateTimePosixFormatString_R_MCalendar()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // this overrides language
    QString lcNumeric("ar_SA");  // does matter, overrides localized numbers in dates
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::GregorianCalendar);
    MCalendar mcal(locale);
    int year = 2008;
    int month = 7;
    int day = 21;
    int hour = 14;
    int minute = 31;
    int second = 3;
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(language);
    QString format("%R");
    QString result("١٤:٣١");
    QCOMPARE(locale.formatDateTime(mcal, format), result);
    QCOMPARE(locale.formatDateTime(datetime, format), result);

    QBENCHMARK {
        locale.formatDateTime(mcal, format);
    }
}

void Pt_MCalendar::benchmarkFormatDateTimePosixFormatString_t_MCalendar()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // this overrides language
    QString lcNumeric("ar_SA");  // does matter, overrides localized numbers in dates
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::GregorianCalendar);
    MCalendar mcal(locale);
    int year = 2008;
    int month = 7;
    int day = 21;
    int hour = 14;
    int minute = 31;
    int second = 3;
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(language);
    QString format("%t");
    QString result("\t");
    QCOMPARE(locale.formatDateTime(mcal, format), result);
    QCOMPARE(locale.formatDateTime(datetime, format), result);

    QBENCHMARK {
        locale.formatDateTime(mcal, format);
    }
}

void Pt_MCalendar::benchmarkIcuFormatString()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("fi_FI");     // this overrides language
    QString lcNumeric("en_US");  // should not matter, no localized numbers involved
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setCalendarType(MLocale::IslamicCalendar); // should not matter
    QCOMPARE(locale.icuFormatString(
                 MLocale::DateFull,
                 MLocale::TimeFull,
                 MLocale::GregorianCalendar),
             QString("cccc d. MMMM y H:mm:ss zzzz"));

    QBENCHMARK {
        locale.icuFormatString(
                 MLocale::DateFull,
                 MLocale::TimeFull,
                 MLocale::GregorianCalendar);
    }
}

void Pt_MCalendar::benchmarkFormatDateTime()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("fi_FI@mix-time-and-language=no");     // this overrides language
    QString lcNumeric("en_US");  // should not matter, no localized numbers involved
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    MCalendar::setSystemTimeZone("Europe/Helsinki");
    MCalendar calendar;
    calendar.setDateTime(QDateTime(QDate(2010, 7, 13),
                                   QTime(14, 51, 07, 0),
                                   Qt::LocalTime));

    QCOMPARE(locale.formatDateTime(
                 calendar, MLocale::DateFull, MLocale::TimeFull),
             QString("tiistai 13. heinäkuuta 2010 14:51:07 Itä-Euroopan kesäaika"));

    QBENCHMARK {
        locale.formatDateTime(
            calendar, MLocale::DateFull, MLocale::TimeFull);
    }
}

void Pt_MCalendar::benchmarkFormatDateTimeICU()
{
    QString language("en_US");   // will be overridden
    QString lcMessages("en_US"); // should not matter
    QString lcTime("fi_FI@mix-time-and-language=no"); // this overrides language
    QString lcNumeric("en_US");  // should not matter
    QString formatString("cccc d. MMMM y H:mm:ss zzzz");
    QString formattedResult("tiistai 13. heinäkuuta 2010 14:51:07 Itä-Euroopan kesäaika");
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    MCalendar::setSystemTimeZone("Europe/Helsinki");
    MCalendar calendar;
    calendar.setDateTime(QDateTime(QDate(2010, 7, 13),
                                   QTime(14, 51, 07, 0),
                                   Qt::LocalTime));

    QCOMPARE(locale.formatDateTimeICU(calendar, formatString), formattedResult);

    QBENCHMARK {
        locale.formatDateTimeICU(calendar, formatString);
    }
}

QTEST_APPLESS_MAIN(Pt_MCalendar);

