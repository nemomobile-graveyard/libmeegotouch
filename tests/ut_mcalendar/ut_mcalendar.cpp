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
#include <QTextStream>
#include <MLocale>
#include <unicode/uversion.h>

#include "ut_mcalendar.h"

void Ut_MCalendar::initTestCase()
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
    QFile fiResource("/usr/share/meegotouch/icu/usrdt44l/fi.res");
    QVERIFY2(fiResource.exists(), "File /usr/share/meegotouch/icu/usrdt44l/fi.res does not exist. The files in /usr/share/meegotouch/icu/usrdt44l/ are needed for this test, please check why they are missing.");
}

void Ut_MCalendar::cleanupTestCase()
{
    delete qap;
}

void Ut_MCalendar::init()
{
    // most tests use the Finnish timezone:
    MCalendar::setSystemTimeZone("Europe/Helsinki");
}

void Ut_MCalendar::cleanup()
{
}

void Ut_MCalendar::testTimeZones()
{
    qDebug() << "systemTimeZone() =" << MCalendar::systemTimeZone();
    MCalendar::setSystemTimeZone("Europe/Helsinki");
    QCOMPARE(MCalendar::systemTimeZone(), QString("Europe/Helsinki"));
    foreach (QString timeZone, MCalendar::supportedTimeZones()) {
        MCalendar::setSystemTimeZone(timeZone);
        QCOMPARE(MCalendar::systemTimeZone(), timeZone);
    }
}

void Ut_MCalendar::testTimeZonesInCountry_data()
{
    QTest::addColumn<QString>("countryCode");
    QTest::addColumn<QStringList>("timeZoneList");

    QTest::newRow("FI")
        << "FI"
        << (QStringList() << "Europe/Helsinki");
    QTest::newRow("JP")
        << "JP"
        << (QStringList() << "Asia/Tokyo" << "JST" << "Japan");
}

void Ut_MCalendar::testTimeZonesInCountry()
{
    QFETCH(QString, countryCode);
    QFETCH(QStringList, timeZoneList);
    QCOMPARE(MCalendar::supportedTimeZones(countryCode), timeZoneList);
}

void Ut_MCalendar::testConversionFromAndToQDateTime_data()
{
    QTest::addColumn<QDate>("qDate");
    QTest::addColumn<QTime>("qTime");
    QTest::addColumn<int>("qTimeSpec");
    QTest::addColumn<QString>("timeZone");

    QTest::newRow("1945-07-21 Qt::Localtime UTC")
            << QDate(1945, 7, 31)
            << QTime(14, 31, 0, 0)
            << (int) Qt::LocalTime
            << "UTC";

    QTest::newRow("1963-07-21 Qt::Localtime UTC")
            << QDate(1963, 7, 31)
            << QTime(14, 31, 0, 0)
            << (int) Qt::LocalTime
            << "UTC";

    QTest::newRow("2040-07-21 Qt::Localtime UTC")
            << QDate(2040, 7, 31)
            << QTime(14, 31, 0, 0)
            << (int) Qt::LocalTime
            << "UTC";

    QTest::newRow("2050-07-21 Qt::Localtime UTC")
            << QDate(2050, 7, 31)
            << QTime(14, 31, 0, 0)
            << (int) Qt::LocalTime
            << "UTC";

    QTest::newRow("2068-07-21 Qt::Localtime UTC")
            << QDate(2068, 7, 31)
            << QTime(14, 31, 0, 0)
            << (int) Qt::LocalTime
            << "UTC";

    QTest::newRow("2008-07-21 Qt::Localtime UTC")
            << QDate(2008, 7, 21)
            << QTime(12, 31, 0, 0)
            << (int) Qt::LocalTime
            << "UTC";

    QTest::newRow("2008-07-21 Qt::UTC UTC")
            << QDate(2008, 7, 21)
            << QTime(12, 31, 0, 0)
            << (int) Qt::UTC
            << "UTC";

    QTest::newRow("2008-07-21 Qt::LocalTime Europe/Helsinki")
            << QDate(2008, 7, 21)
            << QTime(12, 31, 0, 0)
            << (int) Qt::LocalTime
            << "Europe/Helsinki";

    QTest::newRow("2008-07-21 Qt::UTC Europe/Helsinki")
            << QDate(2008, 7, 21)
            << QTime(12, 31, 0, 0)
            << (int) Qt::UTC
            << "Europe/Helsinki";
}

void Ut_MCalendar::testConversionFromAndToQDateTime()
{
    QFETCH(QDate, qDate);
    QFETCH(QTime, qTime);
    QFETCH(int, qTimeSpec);
    QFETCH(QString, timeZone);

    MCalendar::setSystemTimeZone(timeZone);

    MCalendar mcal;
    QDateTime datetime(qDate, qTime, (Qt::TimeSpec) qTimeSpec);
    mcal.setDateTime(datetime);
    QCOMPARE(mcal.qDateTime((Qt::TimeSpec) qTimeSpec), datetime);
}

void Ut_MCalendar::testIcuFormatString_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<MLocale::TimeFormat24h>("timeFormat24h");
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<QString>("dateShortResult");
    QTest::addColumn<QString>("dateMediumResult");
    QTest::addColumn<QString>("dateLongResult");
    QTest::addColumn<QString>("dateFullResult");
    QTest::addColumn<QString>("timeShortResult");
    QTest::addColumn<QString>("timeMediumResult");
    QTest::addColumn<QString>("timeLongResult");
    QTest::addColumn<QString>("timeFullResult");
    //--------------------------------------------------
    QTest::newRow("de_DE, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "de_DE" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd.MM.yy"
        << "dd.MM.yyyy"
        << "d. MMMM y"
        << "EEEE, d. MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH:mm:ss zzzz";
    QTest::newRow("de_DE, Gregorian calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "de_DE" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd.MM.yy"
        << "dd.MM.yyyy"
        << "d. MMMM y"
        << "EEEE, d. MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH:mm:ss zzzz";
    QTest::newRow("de_DE, Gregorian calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "de_DE" // only lc_time matters
        // de_DE has 24 hours by default, override it here:
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd.MM.yy"
        << "dd.MM.yyyy"
        << "d. MMMM y"
        << "EEEE, d. MMMM y"
        << "hh:mm a"
        << "hh:mm:ss a"
        << "hh:mm:ss a z"
        << "hh:mm:ss a zzzz";
    //--------------------------------------------------
    QTest::newRow("fi_FI, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "de_DE" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "fi_FI" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Gregorian calendar, TwentyFourHourTimeFormat24h")
        << "de_DE" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "fi_FI" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Gregorian calendar, TwelveHourTimeFormat24h")
        << "de_DE" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "fi_FI" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "h:mm a"
        << "h:mm:ss a"
        << "h:mm:ss a z"
        << "h:mm:ss a zzzz";
    //--------------------------------------------------
    QTest::newRow("fi_FI, Islamic calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::IslamicCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Islamic calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::IslamicCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Islamic calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::IslamicCalendar
        << "d.M.yyyy"
        << "d.M.yyyy"
        << "d. MMMM y"
        << "cccc d. MMMM y"
        << "h:mm a"
        << "h:mm:ss a"
        << "h:mm:ss a z"
        << "h:mm:ss a zzzz";
    //--------------------------------------------------
    QTest::newRow("Arabic, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "d‏/M‏/yyyy"   // contains U+200F RIGHT-TO-LEFT MARK
        << "dd‏/MM‏/yyyy" // contains U+200F RIGHT-TO-LEFT MARK
        << "d MMMM، y"
        << "EEEE، d MMMM، y"
        << "h:mm a"
        << "h:mm:ss a"
        << "z h:mm:ss a"
        << "zzzz h:mm:ss a";
    QTest::newRow("Arabic, Gregorian calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d‏/M‏/yyyy"   // contains U+200F RIGHT-TO-LEFT MARK
        << "dd‏/MM‏/yyyy" // contains U+200F RIGHT-TO-LEFT MARK
        << "d MMMM، y"
        << "EEEE، d MMMM، y"
        << "h:mm a"
        << "h:mm:ss a"
        << "z h:mm:ss a"
        << "zzzz h:mm:ss a";
    QTest::newRow("Arabic, Gregorian calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d‏/M‏/yyyy"   // contains U+200F RIGHT-TO-LEFT MARK
        << "dd‏/MM‏/yyyy" // contains U+200F RIGHT-TO-LEFT MARK
        << "d MMMM، y"
        << "EEEE، d MMMM، y"
        << "H:mm"
        << "H:mm:ss"
        << "z H:mm:ss"
        << "zzzz H:mm:ss";
    //--------------------------------------------------
    QTest::newRow("Arabic, Islamic calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::IslamicCalendar
        << "d‏/M‏/yyyy"   // contains U+200F RIGHT-TO-LEFT MARK
        << "dd‏/MM‏/yyyy" // contains U+200F RIGHT-TO-LEFT MARK
        << "d MMMM، y"
        << "EEEE، d MMMM، y"
        << "h:mm a"
        << "h:mm:ss a"
        << "z h:mm:ss a"
        << "zzzz h:mm:ss a";
    QTest::newRow("Arabic, Islamic calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::IslamicCalendar
        << "d‏/M‏/yyyy"   // contains U+200F RIGHT-TO-LEFT MARK
        << "dd‏/MM‏/yyyy" // contains U+200F RIGHT-TO-LEFT MARK
        << "d MMMM، y"
        << "EEEE، d MMMM، y"
        << "h:mm a"
        << "h:mm:ss a"
        << "z h:mm:ss a"
        << "zzzz h:mm:ss a";
    QTest::newRow("Arabic, Islamic calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::IslamicCalendar
        << "d‏/M‏/yyyy"   // contains U+200F RIGHT-TO-LEFT MARK
        << "dd‏/MM‏/yyyy" // contains U+200F RIGHT-TO-LEFT MARK
        << "d MMMM، y"
        << "EEEE، d MMMM، y"
        << "H:mm"
        << "H:mm:ss"
        << "z H:mm:ss"
        << "zzzz H:mm:ss";
    //--------------------------------------------------
    QTest::newRow("ja_JP, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "ja_JP" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy/MM/dd"
        << "yyyy/MM/dd"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H時mm分ss秒 zzzz";
    QTest::newRow("ja_JP, Gregorian calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "ja_JP" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy/MM/dd"
        << "yyyy/MM/dd"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H時mm分ss秒 zzzz";
    QTest::newRow("ja_JP, Gregorian calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "ja_JP" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy/MM/dd"
        << "yyyy/MM/dd"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "ah:mm"
        << "ah:mm:ss"
        << "ah:mm:ss z"
        << "ah時mm分ss秒 zzzz";
    //--------------------------------------------------
    QTest::newRow("ja_JP, Japanese calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "ja_JP" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::JapaneseCalendar
        << "Gyy/MM/dd"
        << "Gyy/MM/dd"
        << "Gy年M月d日"
        << "Gy年M月d日EEEE"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H時mm分ss秒 zzzz";
    //--------------------------------------------------
    QTest::newRow("zh_CN, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "zh_CN" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-M-d"
        << "yyyy-M-d"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "ah:mm"
        << "ah:mm:ss"
        << "zah时mm分ss秒"
        << "zzzzah时mm分ss秒";
    QTest::newRow("zh_CN, Gregorian calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "zh_CN" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-M-d"
        << "yyyy-M-d"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "ah:mm"
        << "ah:mm:ss"
        << "zah时mm分ss秒"
        << "zzzzah时mm分ss秒";
    QTest::newRow("zh_CN, Gregorian calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "zh_CN" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-M-d"
        << "yyyy-M-d"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "H:mm"
        << "H:mm:ss"
        << "zH时mm分ss秒"
        << "zzzzH时mm分ss秒";
    //--------------------------------------------------
    QTest::newRow("zh_CN, Chinese calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "zh_CN" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::ChineseCalendar
        << "y'x'G-Ml-d"
        << "y'x'G-Ml-d"
        << "y'x'G-Ml-d"
        << "EEEE y'x'G-Ml-d"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH:mm:ss zzzz";
    QTest::newRow("zh_CN, Chinese calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "zh_CN" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::ChineseCalendar
        << "y'x'G-Ml-d"
        << "y'x'G-Ml-d"
        << "y'x'G-Ml-d"
        << "EEEE y'x'G-Ml-d"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH:mm:ss zzzz";
    QTest::newRow("zh_CN, Chinese calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "zh_CN" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::ChineseCalendar
        << "y'x'G-Ml-d"
        << "y'x'G-Ml-d"
        << "y'x'G-Ml-d"
        << "EEEE y'x'G-Ml-d"
        << "ahh:mm"
        << "ahh:mm:ss"
        << "ahh:mm:ss z"
        << "ahh:mm:ss zzzz";
    //--------------------------------------------------
    QTest::newRow("th_TH, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "th_TH" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "d/M/yyyy"
        << "d MMM y"
        << "d MMMM y"
        << "EEEEที่ d MMMM G y"
        << "H:mm"
        << "H:mm:ss"
        << "H นาฬิกา m นาที ss วินาที z"
        << "H นาฬิกา m นาที ss วินาที zzzz";
    QTest::newRow("th_TH, Gregorian calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "th_TH" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d/M/yyyy"
        << "d MMM y"
        << "d MMMM y"
        << "EEEEที่ d MMMM G y"
        << "H:mm"
        << "H:mm:ss"
        << "H นาฬิกา m นาที ss วินาที z"
        << "H นาฬิกา m นาที ss วินาที zzzz";
    QTest::newRow("th_TH, Gregorian calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "th_TH" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d/M/yyyy"
        << "d MMM y"
        << "d MMMM y"
        << "EEEEที่ d MMMM G y"
        << "h:mm a"
        << "h:mm:ss a"
        << "h นาฬิกา m นาที ss วินาที a z"
        << "h นาฬิกา m นาที ss วินาที a zzzz";
    //--------------------------------------------------
    QTest::newRow("th_TH, Buddhist calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "th_TH" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::BuddhistCalendar
        << "d/M/yyyy"
        << "d MMM y"
        << "d MMMM y"
        << "EEEEที่ d MMMM G y"
        << "H:mm"
        << "H:mm:ss"
        << "H นาฬิกา m นาที ss วินาที z"
        << "H นาฬิกา m นาที ss วินาที zzzz";
    QTest::newRow("th_TH, Buddhist calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "th_TH" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::BuddhistCalendar
        << "d/M/yyyy"
        << "d MMM y"
        << "d MMMM y"
        << "EEEEที่ d MMMM G y"
        << "H:mm"
        << "H:mm:ss"
        << "H นาฬิกา m นาที ss วินาที z"
        << "H นาฬิกา m นาที ss วินาที zzzz";
    QTest::newRow("th_TH, Buddhist calendar, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "th_TH" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::BuddhistCalendar
        << "d/M/yyyy"
        << "d MMM y"
        << "d MMMM y"
        << "EEEEที่ d MMMM G y"
        << "h:mm a"
        << "h:mm:ss a"
        << "h นาฬิกา m นาที ss วินาที a z"
        << "h นาฬิกา m นาที ss วินาที a zzzz";
    //--------------------------------------------------
    QTest::newRow("de_BE, Gregorian, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "de_BE" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "d/MM/yy"
        << "dd.MM.yyyy"
        << "d MMMM y"
        << "EEEE d MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH 'h' mm 'min' ss 's' zzzz";
    QTest::newRow("de_BE, Gregorian, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "de_BE" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d/MM/yy"
        << "dd.MM.yyyy"
        << "d MMMM y"
        << "EEEE d MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH 'h' mm 'min' ss 's' zzzz";
    QTest::newRow("de_BE, Gregorian, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "de_BE" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "d/MM/yy"
        << "dd.MM.yyyy"
        << "d MMMM y"
        << "EEEE d MMMM y"
        << "hh:mm a"
        << "hh:mm:ss a"
        << "hh:mm:ss a z"
        << "hh 'h' mm 'min' ss 's' a zzzz";
    //--------------------------------------------------
    QTest::newRow("en_BE, Gregorian, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "en_BE" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd/MM/yy"
        << "dd MMM y"
        << "d MMM y"
        << "EEEE d MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH 'h' mm 'min' ss 's' zzzz";
    QTest::newRow("en_BE, Gregorian, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "en_BE" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd/MM/yy"
        << "dd MMM y"
        << "d MMM y"
        << "EEEE d MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH 'h' mm 'min' ss 's' zzzz";
    QTest::newRow("en_BE, Gregorian, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "en_BE" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd/MM/yy"
        << "dd MMM y"
        << "d MMM y"
        << "EEEE d MMMM y"
        << "hh:mm a"
        << "hh:mm:ss a"
        << "hh:mm:ss a z"
        << "hh 'h' mm 'min' ss 's' a zzzz";
    //--------------------------------------------------
    QTest::newRow("eo, Gregorian, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "eo" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-MM-dd"
        << "y-MMM-dd"
        << "y-MMMM-dd"
        << "EEEE, d-'a' 'de' MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "H-'a' 'horo' 'kaj' m:ss zzzz";
    QTest::newRow("eo, Gregorian, TwentyFourHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "eo" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-MM-dd"
        << "y-MMM-dd"
        << "y-MMMM-dd"
        << "EEEE, d-'a' 'de' MMMM y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "H-'a' 'horo' 'kaj' m:ss zzzz";
    QTest::newRow("eo, Gregorian, TwelveHourTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "fi_FI" // lc_messages does not matter
        << "eo" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-MM-dd"
        << "y-MMM-dd"
        << "y-MMMM-dd"
        << "EEEE, d-'a' 'de' MMMM y"
        << "hh:mm a"
        << "hh:mm:ss a"
        << "hh:mm:ss a z"
        << "h-'a' 'horo' 'kaj' m:ss a zzzz";
}

void Ut_MCalendar::testIcuFormatString()
{
    QFETCH(QString, language);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(MLocale::TimeFormat24h, timeFormat24h);
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(QString, dateShortResult);
    QFETCH(QString, dateMediumResult);
    QFETCH(QString, dateLongResult);
    QFETCH(QString, dateFullResult);
    QFETCH(QString, timeShortResult);
    QFETCH(QString, timeMediumResult);
    QFETCH(QString, timeLongResult);
    QFETCH(QString, timeFullResult);

    MLocale locale(language);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setTimeFormat24h(timeFormat24h);

    QList<QString> dateResults;
    dateResults << QString("")
                <<  dateShortResult
                <<  dateMediumResult
                <<  dateLongResult
                <<  dateFullResult;
    QList<QString> timeResults;
    timeResults << QString("")
                <<  timeShortResult
                <<  timeMediumResult
                <<  timeLongResult
                <<  timeFullResult;

    for (unsigned dateType = MLocale::DateNone; dateType <= MLocale::DateFull;
            ++dateType) {
        for (unsigned timeType = MLocale::TimeNone; timeType <= MLocale::TimeFull;
                ++timeType) {
            QString expectedResult;
            if (dateType == MLocale::DateNone && timeType == MLocale::TimeNone)
                expectedResult = QString("irrelevant");
            else if (dateType == MLocale::DateNone)
                expectedResult = timeResults[timeType];
            else if (timeType == MLocale::TimeNone)
                expectedResult = dateResults[dateType];
            else if(locale.categoryName(MLocale::MLcTime) == "ja_JP"
                    && calendarType == MLocale::GregorianCalendar
                    && (dateType == MLocale::DateLong
                        || dateType == MLocale::DateFull))
                expectedResult = dateResults[dateType] + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime) == "zh_CN"
                    && calendarType == MLocale::GregorianCalendar
                    && (dateType == MLocale::DateLong
                        || dateType == MLocale::DateFull))
                expectedResult = dateResults[dateType] + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime) == "th_TH")
                expectedResult = dateResults[dateType] + ", " + timeResults[timeType];
            else
                expectedResult = dateResults[dateType] + ' ' + timeResults[timeType];
            QString result = locale.icuFormatString(
                static_cast<MLocale::DateType>(dateType),
                static_cast<MLocale::TimeType>(timeType),
                calendarType);
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream << lcTime
                        << " timeFormat24h: " << timeFormat24h
                        << " dateType: " << dateType << " timeType: " << timeType
                        << " expectedResult: " << expectedResult
                        << " result: " << result
                        << "\n";
            if (expectedResult != "irrelevant")
                QCOMPARE(result, expectedResult);
        }
    }
}

void Ut_MCalendar::testMLocaleCalendar_data()
{
    QTest::addColumn<MLocale::CalendarType>("cal");
    QTest::newRow("default") << MLocale::DefaultCalendar;
    QTest::newRow("gregorian") << MLocale::GregorianCalendar;
    QTest::newRow("islamic") << MLocale::IslamicCalendar;
    QTest::newRow("chinese") << MLocale::ChineseCalendar;
    QTest::newRow("islamiccivil") << MLocale::IslamicCivilCalendar;
    QTest::newRow("hebrew") << MLocale::HebrewCalendar;
    QTest::newRow("japanese") << MLocale::JapaneseCalendar;
    QTest::newRow("buddhist") << MLocale::BuddhistCalendar;
    QTest::newRow("persian") << MLocale::PersianCalendar;
    QTest::newRow("coptic") << MLocale::CopticCalendar;
    QTest::newRow("ethiopian") << MLocale::EthiopicCalendar;
}

void Ut_MCalendar::testMLocaleCalendar()
{
    MLocale *z = 0;
    // We can't test with default locale because the s_systemDefault
    // is persistent and whatever we set to default locale will be
    // kept there.
    z = new MLocale("fi");
    QVERIFY2(z->isValid(), "new MLocale() did not create a valid locale");

    QFETCH(MLocale::CalendarType, cal);

    QVERIFY2(z->calendarType() == MLocale::DefaultCalendar,
             "Constructor didn't set the default calendar");
    z->setCalendarType(cal);
    QVERIFY2(z->calendarType() == cal, "Calendar was not set properly");
    delete z;
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromLocaltimeQDateTime_data()
{
    QTest::addColumn<QDateTime>("datetime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<MLocale::CalendarType>("cal");
    QTest::addColumn<QString>("dateShortResult");
    QTest::addColumn<QString>("dateMediumResult");
    QTest::addColumn<QString>("dateLongResult");
    QTest::addColumn<QString>("dateFullResult");
    QTest::addColumn<QString>("timeShortResult");
    QTest::addColumn<QString>("timeMediumResult");
    QTest::addColumn<QString>("timeLongResult");
    QTest::addColumn<QString>("timeFullResult");

    QDate date(2008, 7, 21);
    QTime time(12, 31, 0, 0);
    QDateTime datetime(date, time, Qt::LocalTime);

    QTest::newRow("21.7.2008_fi_FI_Gregorian")
            << datetime
            << QString("fi_FI")
            << MLocale::GregorianCalendar
            << QString("21.7.2008")
            << QString("21.7.2008")
            << QString("21. heinäkuuta 2008")
            << QString("maanantai 21. heinäkuuta 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 UTC+3.00")
            << QString("12:31:00 Itä-Euroopan kesäaika");
    QTest::newRow("21.7.2008_en_GB_Gregorian")
            << datetime
            << QString("en_GB")
            << MLocale::GregorianCalendar
            << QString("21/07/2008")
            << QString("21 Jul 2008")
            << QString("21 July 2008")
            << QString("Monday, 21 July 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 EEST")
            << QString("12:31:00 Eastern European Summer Time");
    QTest::newRow("21.7.2008_de_DE_Gregorian")
            << datetime
            << QString("de_DE")
            << MLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21.07.2008")
            << QString("21. Juli 2008")
            << QString("Montag, 21. Juli 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 GMT+03:00")
            << QString("12:31:00 Osteuropäische Sommerzeit");
    QTest::newRow("21.7.2008_nn_NO_Gregorian")
            << datetime
            << QString("nn_NO")
            << MLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. jul. 2008")
            << QString("21. juli 2008")
            << QString("måndag 21. juli 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 GMT+03:00")
            << QString("kl. 12:31:00 austeuropeisk sommartid");
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
            << datetime
            << QString("nb_NO")
            << MLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. juli 2008")
            << QString("21. juli 2008")
            << QString("mandag 21. juli 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 GMT+03:00")
            << QString("kl. 12:31:00 østeuropeisk sommertid");
    QTest::newRow("21.7.2008_no_NO_Gregorian")
            << datetime
            << QString("no_NO")
            << MLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. juli 2008")
            << QString("21. juli 2008")
            << QString("mandag 21. juli 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 GMT+03:00")
            << QString("kl. 12:31:00 østeuropeisk sommertid");
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromLocaltimeQDateTime()
{
    QFETCH(QDateTime, datetime);
    QFETCH(QString, localeName);
    QFETCH(MLocale::CalendarType, cal);
    QFETCH(QString, dateShortResult);
    QFETCH(QString, dateMediumResult);
    QFETCH(QString, dateLongResult);
    QFETCH(QString, dateFullResult);
    QFETCH(QString, timeShortResult);
    QFETCH(QString, timeMediumResult);
    QFETCH(QString, timeLongResult);
    QFETCH(QString, timeFullResult);

    MLocale locale(localeName);

    QList<QString> dateResults;
    dateResults << QString("")
                <<  dateShortResult
                <<  dateMediumResult
                <<  dateLongResult
                <<  dateFullResult;
    QList<QString> timeResults;
    timeResults << QString("")
                <<  timeShortResult
                <<  timeMediumResult
                <<  timeLongResult
                <<  timeFullResult;

    for (unsigned dateType = MLocale::DateNone; dateType <= MLocale::DateFull;
            ++dateType) {
        for (unsigned timeType = MLocale::TimeNone; timeType <= MLocale::TimeFull;
                ++timeType) {
            QString result;
            if (dateType == MLocale::DateNone)
                result = timeResults[timeType];
            else if (timeType == MLocale::TimeNone)
                result = dateResults[dateType];
            else
                result = dateResults[dateType] + ' ' + timeResults[timeType];
            qDebug() << "dateType" << dateType << "timeType" << timeType
                     << "result" << result;
            QCOMPARE(
                locale.formatDateTime(datetime,
                                      static_cast<MLocale::DateType>(dateType),
                                      static_cast<MLocale::TimeType>(timeType),
                                      cal),
                result);
        }
    }
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromUTCQDateTime_data()
{
    QTest::addColumn<QDateTime>("datetime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<MLocale::CalendarType>("calType");
    QTest::addColumn<QString>("short_result");
    QTest::addColumn<QString>("long_result");
    QTest::addColumn<QString>("full_result");

    QDate date(2008, 7, 21);
    QTime time(12, 31, 0, 0);
    QDateTime datetime(date, time, Qt::UTC); // finland is utc+3 so maps to 15:31

    QTest::newRow("21.7.2008_fi_FI_Gregorian")
            << datetime
            << QString("fi_FI")
            << MLocale::GregorianCalendar
            << QString("21.7.2008 15:31")
            << QString("21. heinäkuuta 2008 15:31:00 UTC+3.00")
            << QString("maanantai 21. heinäkuuta 2008 15:31:00 Itä-Euroopan kesäaika");
    QTest::newRow("21.7.2008_en_GB_Gregorian")
            << datetime
            << QString("en_GB")
            << MLocale::GregorianCalendar
            << QString("21/07/2008 15:31")
            << QString("21 July 2008 15:31:00 EEST")
            << QString("Monday, 21 July 2008 15:31:00 Eastern European Summer Time");
    QTest::newRow("21.7.2008_nn_NO_Gregorian")
            << datetime
            << QString("nn_NO")
            << MLocale::GregorianCalendar
            << QString("21.07.08 15:31")
            << QString("21. juli 2008 15:31:00 GMT+03:00")
            << QString("måndag 21. juli 2008 kl. 15:31:00 austeuropeisk sommartid");
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
            << datetime
            << QString("nb_NO")
            << MLocale::GregorianCalendar
            << QString("21.07.08 15:31")
            << QString("21. juli 2008 15:31:00 GMT+03:00")
            << QString("mandag 21. juli 2008 kl. 15:31:00 østeuropeisk sommertid");
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromUTCQDateTime()
{
    QFETCH(QDateTime, datetime);
    QFETCH(QString, localeName);
    QFETCH(MLocale::CalendarType, calType);
    QFETCH(QString, short_result);
    QFETCH(QString, long_result);
    QFETCH(QString, full_result);

    MLocale locale(localeName);

    QCOMPARE(locale.formatDateTime(datetime, MLocale::DateShort,
                                   MLocale::TimeShort, calType),
             short_result);
    QCOMPARE(locale.formatDateTime(datetime, MLocale::DateLong,
                                   MLocale::TimeLong, calType),
             long_result);
    QCOMPARE(locale.formatDateTime(datetime, MLocale::DateFull,
                                   MLocale::TimeFull, calType),
             full_result);
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromMCalendar_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<QString>("timeZone");
    QTest::addColumn<MLocale::TimeFormat24h>("timeFormat24h");
    QTest::addColumn<MLocale::CalendarType>("calType");
    QTest::addColumn<int>("year");
    QTest::addColumn<int>("month");
    QTest::addColumn<int>("day");
    QTest::addColumn<QString>("dateShortResult");
    QTest::addColumn<QString>("dateMediumResult");
    QTest::addColumn<QString>("dateLongResult");
    QTest::addColumn<QString>("dateFullResult");
    QTest::addColumn<QString>("timeShortResult");
    QTest::addColumn<QString>("timeMediumResult");
    QTest::addColumn<QString>("timeLongResult");
    QTest::addColumn<QString>("timeFullResult");

    QTest::newRow("21.7.2008_de_DE_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("de_DE") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21.07.2008"
        << "21. Juli 2008"
        << "Montag, 21. Juli 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "14:31:00 Osteuropäische Sommerzeit";
    QTest::newRow("21.7.2008_fi_FI_Gregorian")
        << QString("de_DE")
        << QString("de_DE")
        << QString("fi_FI") // only this matters
        << QString("de_DE")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.7.2008"
        << "21.7.2008"
        << "21. heinäkuuta 2008"
        << "maanantai 21. heinäkuuta 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_en_GB_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_GB") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21/07/2008"
        << "21 Jul 2008"
        << "21 July 2008"
        << "Monday, 21 July 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 EEST"
        << "14:31:00 Eastern European Summer Time";
    QTest::newRow("21.7.2008_nn_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("nn_NO") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21. jul. 2008"
        << "21. juli 2008"
        << "måndag 21. juli 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "kl. 14:31:00 austeuropeisk sommartid";
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("nb_NO") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21. juli 2008"
        << "21. juli 2008"
        << "mandag 21. juli 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "kl. 14:31:00 østeuropeisk sommertid";
    QTest::newRow("21.7.2008_no_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("no_NO") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21. juli 2008"
        << "21. juli 2008"
        << "mandag 21. juli 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "kl. 14:31:00 østeuropeisk sommertid";
    QTest::newRow("21.7.2008_ja_JP_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ja_JP") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "08/07/21"
        << "2008/07/21"
        << "2008年7月21日"
        << "2008年7月21日月曜日"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "14時31分00秒 東欧夏時間";
    QTest::newRow("21.7.2008_ja_JP_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ja_JP") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "08/07/21"
        << "2008/07/21"
        << "2008年7月21日"
        << "2008年7月21日月曜日"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "14時31分00秒 東欧夏時間";
    QTest::newRow("21.7.2008_zh_CN_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("zh_CN") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "08-7-21"
        << "2008-7-21"
        << "2008年7月21日"
        << "2008年7月21日星期一"
        << "下午2:31"
        << "下午2:31:00"
        << "格林尼治标准时间+0300下午2时31分00秒"
        << "东欧夏令时间下午2时31分00秒";
    QTest::newRow("21.7.2008_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "٢١‏/٧‏/٢٠٠٨"
        << "٢١‏/٠٧‏/٢٠٠٨"
        << "٢١ يوليو، ٢٠٠٨"
        << "الإثنين، ٢١ يوليو، ٢٠٠٨"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("21.7.2008_th_TH_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("th_TH") // only this matters
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21/7/2008"
        << "21 ก.ค. 2008"
        << "21 กรกฎาคม 2008"
        << "วันจันทร์ที่ 21 กรกฎาคม ค.ศ. 2008"
        << "14:31"
        << "14:31:00"
        << "14 นาฬิกา 31 นาที 00 วินาที GMT+03:00"
        << "14 นาฬิกา 31 นาที 00 วินาที GMT+03:00";
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromMCalendar()
{
    QFETCH(QString, localeName);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, lcNumeric);
    QFETCH(QString, timeZone);
    QFETCH(MLocale::TimeFormat24h, timeFormat24h);
    QFETCH(MLocale::CalendarType, calType);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(QString, dateShortResult);
    QFETCH(QString, dateMediumResult);
    QFETCH(QString, dateLongResult);
    QFETCH(QString, dateFullResult);
    QFETCH(QString, timeShortResult);
    QFETCH(QString, timeMediumResult);
    QFETCH(QString, timeLongResult);
    QFETCH(QString, timeFullResult);

    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    locale.setTimeFormat24h(timeFormat24h);
    MCalendar::setSystemTimeZone("Europe/Helsinki");
    MCalendar mcal(calType);
    mcal.setDate(year, month, day);
    mcal.setTime(14, 31, 0);
    QDateTime dateTime(QDate(year, month, day), QTime(14, 31, 0, 0));

    QList<QString> dateResults;
    dateResults << QString("")
                <<  dateShortResult
                <<  dateMediumResult
                <<  dateLongResult
                <<  dateFullResult;
    QList<QString> timeResults;
    timeResults << QString("")
                <<  timeShortResult
                <<  timeMediumResult
                <<  timeLongResult
                <<  timeFullResult;

    for (unsigned dateType = MLocale::DateNone; dateType <= MLocale::DateFull;
            ++dateType) {
        for (unsigned timeType = MLocale::TimeNone; timeType <= MLocale::TimeFull;
                ++timeType) {
            QString expectedResult;
            if (dateType == MLocale::DateNone)
                expectedResult = timeResults[timeType];
            else if (timeType == MLocale::TimeNone)
                expectedResult = dateResults[dateType];
            else if (locale.categoryName(MLocale::MLcTime) == "ja_JP"
                     && calType == MLocale::GregorianCalendar
                     && (dateType == MLocale::DateLong
                         || dateType == MLocale::DateFull))
                expectedResult = dateResults[dateType] + timeResults[timeType];
            else if (locale.categoryName(MLocale::MLcTime) == "zh_CN"
                     && calType == MLocale::GregorianCalendar
                     && (dateType == MLocale::DateLong
                         || dateType == MLocale::DateFull))
                expectedResult = dateResults[dateType] + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime) == "th_TH")
                expectedResult = dateResults[dateType] + ", " + timeResults[timeType];
            else
                expectedResult = dateResults[dateType] + ' ' + timeResults[timeType];
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream
                << "language " << localeName
                << " lc_time " << lcTime
                << " dateType " << dateType << " timeType " << timeType
                << " format string |"
                << locale.icuFormatString(
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType),
                    calType)
                << "| actual result |"
                << locale.formatDateTime(
                    mcal,
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType))
                << "| expected result |" << expectedResult << "|\n";
#if 1
            QCOMPARE(
                locale.formatDateTime(mcal,
                                      static_cast<MLocale::DateType>(dateType),
                                      static_cast<MLocale::TimeType>(timeType)),
                expectedResult);
            QCOMPARE(
                locale.formatDateTime(dateTime,
                                      static_cast<MLocale::DateType>(dateType),
                                      static_cast<MLocale::TimeType>(timeType),
                                      calType),
                expectedResult);
            if (dateType == MLocale::DateLong
                && timeType == MLocale::TimeLong) {
                QCOMPARE(locale.formatDateTime(dateTime, calType), expectedResult);
            }
#endif
        }
    }
}

void Ut_MCalendar::testVariousSetDateTimeMethods_data()
{
    QTest::addColumn<QDateTime>("qDateTime");
    QTest::addColumn<QString>("timeZoneId");
    QTest::addColumn<int>("qDateTimeOffsetToLocalInHours");

    QTest::newRow("foo")
        << QDateTime(QDate(2010, 7, 13), QTime(14, 51, 07, 0), Qt::LocalTime)
        << "Europe/Helsinki"
        << 0;
    QTest::newRow("bar")
        << QDateTime(QDate(2010, 7, 13), QTime(14, 51, 07, 0), Qt::UTC)
        << "Europe/Helsinki"
        << 3;
}

void Ut_MCalendar::testVariousSetDateTimeMethods()
{
    QFETCH(QDateTime, qDateTime);
    QFETCH(QString, timeZoneId);
    QFETCH(int, qDateTimeOffsetToLocalInHours);

    MCalendar::setSystemTimeZone(timeZoneId);

    MCalendar mCalendarYmdHms;
    mCalendarYmdHms.setYear(qDateTime.date().year());
    mCalendarYmdHms.setMonth(qDateTime.date().month());
    mCalendarYmdHms.setDay(qDateTime.date().day());
    mCalendarYmdHms.setHours(qDateTime.time().hour());
    mCalendarYmdHms.setMinutes(qDateTime.time().minute());
    mCalendarYmdHms.setSeconds(qDateTime.time().second());
    MCalendar mCalendarQdHms;
    mCalendarQdHms.setDate(qDateTime.date());
    mCalendarQdHms.setTime(qDateTime.time().hour(),
                           qDateTime.time().minute(),
                           qDateTime.time().second());
    MCalendar mCalendarQdt;
    mCalendarQdt.setDateTime(qDateTime);

    MLocale locale("en_US");
    qDebug()
        << "\nmCalendarYmdHms"
        << locale.formatDateTime(
            mCalendarYmdHms, MLocale::DateLong, MLocale::TimeLong)
        << "\nmCalendarQdHms "
        << locale.formatDateTime(
            mCalendarQdHms, MLocale::DateLong, MLocale::TimeLong)
        << "\nmCalendarQdt   "
        << locale.formatDateTime(
            mCalendarQdt, MLocale::DateLong, MLocale::TimeLong);

    QCOMPARE(mCalendarYmdHms.year(), qDateTime.date().year());
    QCOMPARE(mCalendarYmdHms.month(), qDateTime.date().month());
    QCOMPARE(mCalendarYmdHms.dayOfMonth(), qDateTime.date().day());
    QCOMPARE(mCalendarYmdHms.hour(), qDateTime.time().hour());
    QCOMPARE(mCalendarYmdHms.minute(), qDateTime.time().minute());
    QCOMPARE(mCalendarYmdHms.second(), qDateTime.time().second());

    QCOMPARE(mCalendarQdHms.year(), qDateTime.date().year());
    QCOMPARE(mCalendarQdHms.month(), qDateTime.date().month());
    QCOMPARE(mCalendarQdHms.dayOfMonth(), qDateTime.date().day());
    QCOMPARE(mCalendarQdHms.hour(), qDateTime.time().hour());
    QCOMPARE(mCalendarQdHms.minute(), qDateTime.time().minute());
    QCOMPARE(mCalendarQdHms.second(), qDateTime.time().second());

    QCOMPARE(mCalendarQdt.year(), qDateTime.date().year());
    QCOMPARE(mCalendarQdt.month(), qDateTime.date().month());
    QCOMPARE(mCalendarQdt.dayOfMonth(), qDateTime.date().day());
    QCOMPARE(mCalendarQdt.hour(),
             qDateTime.time().hour()
             + qDateTimeOffsetToLocalInHours);
    QCOMPARE(mCalendarQdt.minute(), qDateTime.time().minute());
    QCOMPARE(mCalendarQdt.second(), qDateTime.time().second());
}

void Ut_MCalendar::testMCalendarAdditions()
{
    MLocale fi_FI("fi_FI");
    MCalendar cal(fi_FI);
    QVERIFY(cal.isValid());
    cal.setDate(2008, 1, 31);
    cal.setTime(19, 23, 0);
    cal.addMonths(1);

    QCOMPARE(cal.year(), 2008);
    QCOMPARE(cal.month(), 2);
    QCOMPARE(cal.dayOfMonth(), 29);
    QCOMPARE(cal.dayOfYear(), 60);
    QCOMPARE(cal.lastDayOfMonth(), 29);
    QCOMPARE(cal.weekNumber(), 9);
    QCOMPARE(cal.hour(), 19);
    QCOMPARE(cal.minute(), 23);
    QCOMPARE(cal.second(), 0);

    QCOMPARE(cal.firstDayOfMonth(), 1);
    QCOMPARE(cal.firstDayOfWeek(), 1);
    QCOMPARE(cal.maximumWeeksInMonth(), 6);
    QCOMPARE(cal.daysInWeek(), 7);

    // add some hours so calendar changes to 1.3.2008
    cal.addHours(5);
    QCOMPARE(cal.year(), 2008);
    QCOMPARE(cal.month(), 3);
    QCOMPARE(cal.dayOfMonth(), 1);
    QCOMPARE(cal.dayOfYear(), 61);
    QCOMPARE(cal.lastDayOfMonth(), 31);
    QCOMPARE(cal.weekNumber(), 9);
    QCOMPARE(cal.hour(), 0);
    QCOMPARE(cal.minute(), 23);
    QCOMPARE(cal.second(), 0);

    // add some minutes so calendar changes to 2.3.2008
    cal.addMinutes(1445);
    QCOMPARE(cal.year(), 2008);
    QCOMPARE(cal.month(), 3);
    QCOMPARE(cal.dayOfMonth(), 2);
    QCOMPARE(cal.dayOfYear(), 62);
    QCOMPARE(cal.lastDayOfMonth(), 31);
    QCOMPARE(cal.weekNumber(), 9);
    QCOMPARE(cal.hour(), 0);
    QCOMPARE(cal.minute(), 28);
    QCOMPARE(cal.second(), 0);

    // add some seconds so calendar changes to 3.3.2008
    cal.addSeconds(86427);
    QCOMPARE(cal.year(), 2008);
    QCOMPARE(cal.month(), 3);
    QCOMPARE(cal.dayOfMonth(), 3);
    QCOMPARE(cal.dayOfYear(), 63);
    QCOMPARE(cal.lastDayOfMonth(), 31);
    QCOMPARE(cal.weekNumber(), 10);
    QCOMPARE(cal.hour(), 0);
    QCOMPARE(cal.minute(), 28);
    QCOMPARE(cal.second(), 27);

    // add 2 days so calendar changes to 5.3.2008
    cal.addDays(2);
    QCOMPARE(cal.year(), 2008);
    QCOMPARE(cal.month(), 3);
    QCOMPARE(cal.dayOfMonth(), 5);
    QCOMPARE(cal.dayOfYear(), 65);
    QCOMPARE(cal.lastDayOfMonth(), 31);
    QCOMPARE(cal.weekNumber(), 10);
    QCOMPARE(cal.hour(), 0);
    QCOMPARE(cal.minute(), 28);
    QCOMPARE(cal.second(), 27);

    // add a year so calendar changes to 5.3.2009
    cal.addYears(1);
    QCOMPARE(cal.year(), 2009);
    QCOMPARE(cal.month(), 3);
    QCOMPARE(cal.dayOfMonth(), 5);
    QCOMPARE(cal.dayOfYear(), 64);
    QCOMPARE(cal.lastDayOfMonth(), 31);
    QCOMPARE(cal.weekNumber(), 10);
    QCOMPARE(cal.hour(), 0);
    QCOMPARE(cal.minute(), 28);
    QCOMPARE(cal.second(), 27);
}

void Ut_MCalendar::testWeekNumbers()
{
    // 31.12.2007 belongs to week 1 of year 2008, check it really is so
    MLocale fi_FI("fi_FI");
    MCalendar cal(fi_FI);
    cal.setDate(2007, 12, 31);
    QVERIFY2(cal.yearOfWeek() == 2008, "year of week is incorrect");

    cal.setDate(2005, 1, 1);
    QVERIFY2(cal.weekOfYear() == 53, "year of week 1.1.2005 is incorrect2");

    cal.setMinimalDaysInFirstWeek(1);
    QVERIFY2(cal.minimalDaysInFirstWeek() == 1, "setMinimalDaysInFirstWeek() failed");
    QVERIFY2(cal.weekOfYear() == 1, "year of week 1.1.2005 with overridden week data failed");

}

void Ut_MCalendar::testComparisons()
{
    MCalendar cal1;
    MCalendar cal2;
    cal1.setDate(2008, 3, 4);
    cal2.setDate(2007, 7, 23);

    QVERIFY2(cal1.after(cal2) == true, "after() comparison failed");
    QVERIFY2(cal2.before(cal2) == false, "before() comparison failed");
    QVERIFY2(cal1.equals(cal2) == false, "equals() comparison failed");

    QVERIFY2((cal1 < cal2) == false, "< operator failed");
    QVERIFY2((cal2 < cal1) == true, "< operator failed");
    QVERIFY2((cal1 > cal2) == true, "> operator failed");
    QVERIFY2((cal2 > cal1) == false, "> operator failed");
    QVERIFY2((cal1 == cal2) == false, "== operator failed");
    QVERIFY2((cal1 == cal1) == true, "== operator failed");
    QVERIFY2((cal1 <= cal2) == false, "<= operator failed");
    QVERIFY2((cal1 <= cal1) == true, "<= operator failed");
    QVERIFY2((cal1 >= cal1) == true, ">= operator failed");
    QVERIFY2((cal1 >= cal2) == true, ">= operator failed");
    QVERIFY2((cal1 != cal1) == false, "!= operator failed");
    QVERIFY2((cal1 != cal2) == true, "!= operator failed");
}

void Ut_MCalendar::testIslamicCalendar()
{
    MLocale loc("fi_FI");
    loc.setCalendarType(MLocale::IslamicCalendar);
    MCalendar cal(loc);
    QDate date(2008, 1, 1); // use qdate to set date as gregorian
    cal.setDate(date);
    QString format = loc.formatDateTime(cal, MLocale::DateShort, MLocale::TimeNone);
    bool gotCorrect = false;

    // we allow off-by-one conversion because the dates can be a bit ambiguous
    // e.g. islamic day starts from sunset
    if (format == "22.12.1428"
            || format == "23.12.1428"
            || format == "21.12.1428") {
        gotCorrect = true;
    }

    QVERIFY2(gotCorrect == true, "gregorian -> islamic conversion failed");
}

void Ut_MCalendar::testPosixFormatPattern_data()
{
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<int>("year");
    QTest::addColumn<int>("month");
    QTest::addColumn<int>("day");
    QTest::addColumn<int>("hour");
    QTest::addColumn<int>("minute");
    QTest::addColumn<int>("second");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("result");

    // ************************************************************
    QTest::newRow("fi_FI %a")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%a" // locale's abbreviated weekday name (e.g., Sun)
            << "su";
    QTest::newRow("fi_FI %b")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%b" // locale's abbreviated month name (e.g., Jan)
            << "helmi"; // probably wrong! FIXME!!
    QTest::newRow("fi_FI %c")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%c" // locale's date and time (e.g., Thu Mar  3 23:05:25 2005)
            << "3.2.2008 12:25:03";
    QTest::newRow("fi_FI %d")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%d" // %d day of month (e.g, 01)
            << "03";
    QTest::newRow("fi_FI %e")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%e" // %e day of month, space padded; same as %_d
            << "3"; // padding missing, FIXME!!!
    QTest::newRow("fi_FI %g")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%g" // %g last two digits of year of ISO week number (see %G)
            << "08";
    QTest::newRow("fi_FI %h")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%h" // %h same as %b, locale's abbreviated month name (e.g., Jan)
            << "helmi"; // probably wrong! FIXME!!!
    QTest::newRow("fi_FI %j")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%j" // day of year (001..366)
            << "034";
    QTest::newRow("fi_FI %m")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%m" // month (01..12)
            << "02";
    QTest::newRow("fi_FI %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "ip.";
    QTest::newRow("fi_FI %r")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%r" // locale specific 12-hour clock time
            << "12:25 ip.";
    QTest::newRow("fi_FI %t")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%t" // a tab
            << "\t";
    QTest::newRow("fi_FI %u")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%u" // %u day of week (1..7); 1 is Monday
            << "7";
    QTest::newRow("fi_FI %v")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%v" // week number of the year in two digits (missing in POSIX?)
            << "05";
    QTest::newRow("fi_FI %w")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%w" // day of week (0..6); 0 is Sunday
            << "0";
    QTest::newRow("fi_FI %x")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%x" // locale's date representation (e.g., 12/31/99)
            << "3.2.2008";
    QTest::newRow("fi_FI %y")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%y" // last two digits of year (00..99)
            << "08";
    QTest::newRow("fi_FI %z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%z" // +hhmm numeric timezone (e.g., -0400)
            << "+0200";
    QTest::newRow("fi_FI %A")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%A" // locale's full weekday name (e.g., Sunday)
            << "sunnuntaina";
    QTest::newRow("fi_FI %B")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%B" // locale's full month name (e.g., January)
            << "helmikuuta";
    QTest::newRow("fi_FI %C")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%C" // century; like %Y, except omit last two digits (e.g., 21)
            << "20";
    QTest::newRow("fi_FI %D")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%D" // date; same as %m/%d/%y
            << "02/03/08";
    QTest::newRow("fi_FI %F")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%F" // full date; same as %Y-%m-%d"
            << "2008-02-03";
    QTest::newRow("fi_FI %G")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%G" // year of ISO week number (see %V); normally useful only with %V
            << "2008";
    QTest::newRow("fi_FI %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 00
            << 25
            << 3
            << "fi_FI"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "00";
    QTest::newRow("fi_FI %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 01
            << 25
            << 3
            << "fi_FI"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "01";
    QTest::newRow("fi_FI %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 23
            << 25
            << 3
            << "fi_FI"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "23";
    QTest::newRow("fi_FI %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 24 // for the “date” command this would be invalid input but ICU wraps around
            << 25
            << 3
            << "fi_FI"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "00";
    QTest::newRow("fi_FI %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 00
            << 25
            << 3
            << "fi_FI"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "12";
    QTest::newRow("fi_FI %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 01
            << 25
            << 3
            << "fi_FI"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "01";
    QTest::newRow("fi_FI %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 11
            << 25
            << 3
            << "fi_FI"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "11";
    QTest::newRow("fi_FI %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "12";
    QTest::newRow("fi_FI %M")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%M" // minute (00..59)"
            << "25";
    QTest::newRow("fi_FI %R")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%R" // locale specific 24-hour clock time
            << "12:25";
    QTest::newRow("fi_FI %S")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%S" // second (00..60)
            << "03";
    QTest::newRow("fi_FI %T")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%T" // time; same as %H:%M:%S
            << "12:25:03";
    QTest::newRow("fi_FI %U")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%U" // week number of year, with Sunday as first day of week (00..53)
            << "05";
    QTest::newRow("fi_FI %V")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%V" // ISO week number, with Monday as first day of week (01..53)"
            << "05";
    QTest::newRow("fi_FI %W")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%W" // week number of year, with Monday as first day of week (00..53)
            << "4";
    QTest::newRow("fi_FI %X")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%X" // locale's time representation (e.g., 23:13:48)
            << "12:25:03";
    QTest::newRow("fi_FI %Y")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%Y" // year
            << "2008";
    QTest::newRow("fi_FI %Z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%Z" // Time-zone name, or no characters if no time zone is determinable
            << "(Suomi)";

    // ************************************************************
    QTest::newRow("en_GB %a")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%a" // locale's abbreviated weekday name (e.g., Sun)
            << "Sun";
    QTest::newRow("en_GB %b")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%b" // locale's abbreviated month name (e.g., Jan)
            << "Feb";
    QTest::newRow("en_GB %c")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%c" // locale's date and time (e.g., Thu Mar  3 23:05:25 2005)
            << "3 Feb 2008 12:25:03";
    QTest::newRow("en_GB %d")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%d" // %d day of month (e.g, 01)
            << "03";
    QTest::newRow("en_GB %e")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%e" // %e day of month, space padded; same as %_d
            << "3"; // padding missing, FIXME!!!
    QTest::newRow("en_GB %g")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%g" // %g last two digits of year of ISO week number (see %G)
            << "08";
    QTest::newRow("en_GB %h")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%h" // %h same as %b, locale's abbreviated month name (e.g., Jan)
            << "Feb";
    QTest::newRow("en_GB %j")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%j" // day of year (001..366)
            << "034";
    QTest::newRow("en_GB %m")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%m" // month (01..12)
            << "02";
    QTest::newRow("en_GB %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "PM";
    QTest::newRow("en_GB %r")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%r" // locale specific 12-hour clock time
            << "12:25 PM";
    QTest::newRow("en_GB %t")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%t" // a tab
            << "\t";
    QTest::newRow("en_GB %u")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%u" // %u day of week (1..7); 1 is Monday
            << "7";
    QTest::newRow("en_GB %v")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%v" // week number of the year in two digits (missing in POSIX?)
            << "05";
    QTest::newRow("en_GB %w")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%w" // day of week (0..6); 0 is Sunday
            << "0";
    QTest::newRow("en_GB %x")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%x" // locale's date representation (e.g., 12/31/99)
            << "3 Feb 2008";
    QTest::newRow("en_GB %y")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%y" // last two digits of year (00..99)
            << "08";
    QTest::newRow("en_GB %z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%z" // +hhmm numeric timezone (e.g., -0400)
            << "+0200";
    QTest::newRow("en_GB %A")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%A" // locale's full weekday name (e.g., Sunday)
            << "Sunday";
    QTest::newRow("en_GB %B")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%B" // locale's full month name (e.g., January)
            << "February";
    QTest::newRow("en_GB %C")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%C" // century; like %Y, except omit last two digits (e.g., 21)
            << "20";
    QTest::newRow("en_GB %D")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%D" // date; same as %m/%d/%y
            << "02/03/08";
    QTest::newRow("en_GB %F")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%F" // full date; same as %Y-%m-%d"
            << "2008-02-03";
    QTest::newRow("en_GB %G")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%G" // year of ISO week number (see %V); normally useful only with %V
            << "2008";
    QTest::newRow("en_GB %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 00
            << 25
            << 3
            << "en_GB"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "00";
    QTest::newRow("en_GB %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 01
            << 25
            << 3
            << "en_GB"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "01";
    QTest::newRow("en_GB %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 23
            << 25
            << 3
            << "en_GB"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "23";
    QTest::newRow("en_GB %H")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 24 // for the “date” command this would be invalid input but ICU wraps around
            << 25
            << 3
            << "en_GB"
            << "%H" // Hour (24-hour clock), as a decimal number (00-23)
            << "00";
    QTest::newRow("en_GB %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 00
            << 25
            << 3
            << "en_GB"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "12";
    QTest::newRow("en_GB %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 01
            << 25
            << 3
            << "en_GB"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "01";
    QTest::newRow("en_GB %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 11
            << 25
            << 3
            << "en_GB"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "11";
    QTest::newRow("en_GB %I")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%I" // Hour (12-hour clock), as a decimal number (01-12)
            << "12";
    QTest::newRow("en_GB %M")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%M" // minute (00..59)"
            << "25";
    QTest::newRow("en_GB %R")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%R" // locale specific 24-hour clock time
            << "12:25";
    QTest::newRow("en_GB %S")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%S" // second (00..60)
            << "03";
    QTest::newRow("en_GB %T")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%T" // time; same as %H:%M:%S
            << "12:25:03";
    QTest::newRow("en_GB %U")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%U" // week number of year, with Sunday as first day of week (00..53)
            << "05";
    QTest::newRow("ar_EG %U")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ar_EG"
            << "%U" // week number of year, with Sunday as first day of week (00..53)
            << "٠٥";
    QTest::newRow("en_GB %V")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%V" // ISO week number, with Monday as first day of week (01..53)"
            << "05";
    QTest::newRow("ar_EG %V")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ar_EG"
            << "%V" // ISO week number, with Monday as first day of week (01..53)"
            << "٠٥";
    QTest::newRow("en_GB %W")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%W" // week number of year, with Monday as first day of week (00..53)
            << "4";
    QTest::newRow("en_GB %X")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%X" // locale's time representation (e.g., 23:13:48)
            << "12:25:03";
    QTest::newRow("en_GB %Y")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%Y" // year
            << "2008";
    QTest::newRow("en_GB %Z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%Z" // Time-zone name, or no characters if no time zone is determinable
            << "Finland Time";

    // ************************************************************
    QTest::newRow("de_DE %Z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE"
            << "%Z" // Time-zone name, or no characters if no time zone is determinable
            << "(Finnland)";
    // ************************************************************
    QTest::newRow("fi_FI %R %Z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%R %Z"
            << "12:25 (Suomi)";

    QTest::newRow("en_GB %R %Z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%R %Z"
            << "12:25 Finland Time";

    QTest::newRow("de_DE %R %Z")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE"
            << "%R %Z"
            << "12:25 (Finnland)";
}

void Ut_MCalendar::testPosixFormatPattern()
{
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);
    QFETCH(QString, localeName);
    QFETCH(QString, format);
    QFETCH(QString, result);

    MLocale locale(localeName);
    locale.setCalendarType(calendarType);
    MCalendar mcal(locale);
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(localeName);

    qDebug() << "format:" << format << "result: " << result;
//    qDebug() << "QLocale time ShortFormat" << qlocale.toString(time, QLocale::ShortFormat) << "QLocale time LongFormat" << qlocale.toString(time, QLocale::LongFormat);
//    qDebug() << "QLocale format" << qlocale.toString(time, "hh:mm");
//    qDebug() << "QLocale date ShortFormat" << qlocale.toString(date, QLocale::ShortFormat) << "QLocale date LongFormat" << qlocale.toString(date, QLocale::LongFormat);
    qDebug() << "DateNone TimeNone" << locale.formatDateTime(datetime, MLocale::DateNone, MLocale::TimeNone, calendarType);
    qDebug() << "DateNone TimeShort" << locale.formatDateTime(datetime, MLocale::DateNone, MLocale::TimeShort, calendarType);
    qDebug() << "DateShort TimeNone" << locale.formatDateTime(datetime, MLocale::DateShort, MLocale::TimeNone, calendarType);
//    qDebug() << "DateShort TimeShort" << locale.formatDateTime(datetime, MLocale::DateShort, MLocale::TimeShort, calendarType);
//    qDebug() << "DateMedium TimeMedium" << locale.formatDateTime(datetime, MLocale::DateMedium, MLocale::TimeMedium, calendarType);
//    qDebug() << "DateLong TimeLong" << locale.formatDateTime(datetime, MLocale::DateLong, MLocale::TimeLong, calendarType);
//    qDebug() << "DateFull TimeFull" << locale.formatDateTime(datetime, MLocale::DateFull, MLocale::TimeFull, calendarType);
    if ((format == "%b" || format == "%h") && result == "helmi") {
        QEXPECT_FAIL("", "FIXME: ICU does %b and %h wrong for Finnish", Continue);
        QVERIFY(false);
    } else {
        QCOMPARE(locale.formatDateTime(mcal, format), result);
        QCOMPARE(locale.formatDateTime(datetime, format), result);
    }
}

void Ut_MCalendar::testFormatDateTimeICU_data()
{
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<int>("year");
    QTest::addColumn<int>("month");
    QTest::addColumn<int>("day");
    QTest::addColumn<int>("hour");
    QTest::addColumn<int>("minute");
    QTest::addColumn<int>("second");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("result");

    QTest::newRow("fi_FI “G”")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "G" // era designator
            << "jKr.";
    QTest::newRow("de_DE “G”")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE"
            << "G" // era designator
            << "n. Chr.";
    QTest::newRow("zh_CN “G”")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "zh_CN"
            << "G" // era designator
            << "公元";
    QTest::newRow("ja_JP “G”")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ja_JP"
            << "G" // era designator
            << "AD";
    QTest::newRow("ja_JP “G”")
            << MLocale::JapaneseCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ja_JP"
            << "G" // era designator
            << "平成";
}

void Ut_MCalendar::testFormatDateTimeICU()
{
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);
    QFETCH(QString, localeName);
    QFETCH(QString, format);
    QFETCH(QString, result);

    MLocale locale(localeName);
    locale.setCalendarType(calendarType);
    MCalendar mcal(locale);
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(localeName);

    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << "format: " << format
                << " actual result: " << locale.formatDateTimeICU(mcal, format)
                << " expected result: " << result << "\n";

    QCOMPARE(locale.formatDateTimeICU(mcal, format), result);
    QCOMPARE(locale.formatDateTimeICU(datetime, format), result);
}

void Ut_MCalendar::testWeekdaySymbols_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<MLocale::CalendarType>("cal");
    QTest::addColumn<QStringList>("symbols");

    QStringList fi_FI_Symbols;
    fi_FI_Symbols
            << QString("maanantai")
            << QString("tiistai")
            << QString("keskiviikko")
            << QString("torstai")
            << QString("perjantai")
            << QString("lauantai")
            << QString("sunnuntai");

    QTest::newRow("weekday_symbols_fi_FI_Gregorian")
            << QString("fi_FI")
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;

    QStringList en_GB_Symbols;
    en_GB_Symbols
            << QString("Monday")
            << QString("Tuesday")
            << QString("Wednesday")
            << QString("Thursday")
            << QString("Friday")
            << QString("Saturday")
            << QString("Sunday");

    QTest::newRow("weekday_symbols_en_GB_Gregorian")
            << QString("en_GB")
            << MLocale::GregorianCalendar
            << en_GB_Symbols;

    QStringList nn_NO_Symbols;
    nn_NO_Symbols
            << QString("måndag")
            << QString("tysdag")
            << QString("onsdag")
            << QString("torsdag")
            << QString("fredag")
            << QString("laurdag")
            << QString("søndag");

    QTest::newRow("weekday_symbols_nn_NO_Gregorian")
            << QString("nn_NO")
            << MLocale::GregorianCalendar
            << nn_NO_Symbols;

    QStringList nb_NO_Symbols;
    nb_NO_Symbols
            << QString("mandag")
            << QString("tirsdag")
            << QString("onsdag")
            << QString("torsdag")
            << QString("fredag")
            << QString("lørdag")
            << QString("søndag");

    QTest::newRow("weekday_symbols_nb_NO_Gregorian")
            << QString("nb_NO")
            << MLocale::GregorianCalendar
            << nb_NO_Symbols;

    QStringList no_NO_Symbols;
    no_NO_Symbols = nb_NO_Symbols;

    QTest::newRow("weekday_symbols_no_NO_Gregorian")
            << QString("no_NO")
            << MLocale::GregorianCalendar
            << no_NO_Symbols;
}

void Ut_MCalendar::testWeekdaySymbols()
{
    QFETCH(QString, localeName);
    QFETCH(MLocale::CalendarType, cal);
    QFETCH(QStringList, symbols);

    MLocale locale(localeName);
    MCalendar mcal(cal);
    for (int i = 1; i <= 7; ++i) {
        QString symbol = symbols.at(i - 1);
        QCOMPARE(locale.weekdayName(mcal, i), symbol);
    }
}

void Ut_MCalendar::testMonthSymbols_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<MLocale::CalendarType>("cal");
    QTest::addColumn<QStringList>("symbols");

    QStringList fi_FI_Symbols;
    fi_FI_Symbols
            << QString("tammikuu")
            << QString("helmikuu")
            << QString("maaliskuu")
            << QString("huhtikuu")
            << QString("toukokuu")
            << QString("kesäkuu")
            << QString("heinäkuu")
            << QString("elokuu")
            << QString("syyskuu")
            << QString("lokakuu")
            << QString("marraskuu")
            << QString("joulukuu");

    QTest::newRow("month_symbols_fi_FI_Gregorian")
            << QString("fi_FI")
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;

    QStringList en_GB_Symbols;
    en_GB_Symbols
            << QString("January")
            << QString("February")
            << QString("March")
            << QString("April")
            << QString("May")
            << QString("June")
            << QString("July")
            << QString("August")
            << QString("September")
            << QString("October")
            << QString("November")
            << QString("December");

    QTest::newRow("month_symbols_en_GB_Gregorian")
            << QString("en_GB")
            << MLocale::GregorianCalendar
            << en_GB_Symbols;

    QStringList nn_NO_Symbols;
    nn_NO_Symbols
            << QString("januar")
            << QString("februar")
            << QString("mars")
            << QString("april")
            << QString("mai")
            << QString("juni")
            << QString("juli")
            << QString("august")
            << QString("september")
            << QString("oktober")
            << QString("november")
            << QString("desember");

    QTest::newRow("month_symbols_nn_NO_Gregorian")
            << QString("nn_NO")
            << MLocale::GregorianCalendar
            << nn_NO_Symbols;

    QStringList nb_NO_Symbols;
    nb_NO_Symbols
            << QString("januar")
            << QString("februar")
            << QString("mars")
            << QString("april")
            << QString("mai")
            << QString("juni")
            << QString("juli")
            << QString("august")
            << QString("september")
            << QString("oktober")
            << QString("november")
            << QString("desember");

    QTest::newRow("month_symbols_nb_NO_Gregorian")
            << QString("nb_NO")
            << MLocale::GregorianCalendar
            << nb_NO_Symbols;

    QStringList no_NO_Symbols;
    no_NO_Symbols = nb_NO_Symbols;

    QTest::newRow("month_symbols_no_NO_Gregorian")
            << QString("no_NO")
            << MLocale::GregorianCalendar
            << no_NO_Symbols;
}

void Ut_MCalendar::testMonthSymbols()
{
    QFETCH(QString, localeName);
    QFETCH(MLocale::CalendarType, cal);

    MLocale locale(localeName);
    MCalendar mcal(cal);
    QFETCH(QStringList, symbols);

    for (int i = 1; i <= 12; ++i) {
        QString symbol = symbols.at(i - 1);
        QCOMPARE(locale.monthName(mcal, i), symbol);
    }
}

QTEST_APPLESS_MAIN(Ut_MCalendar);

