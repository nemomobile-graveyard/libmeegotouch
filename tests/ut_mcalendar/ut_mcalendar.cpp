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

#include <QCoreApplication>
#include <QTextCodec>
#include <QTextStream>
#include <MLocale>
#include <unicode/uversion.h>

#include "ut_mcalendar.h"

#define VERBOSE_OUTPUT

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

    QString fiResourceName = QString(M_ICUEXTRADATA_DIR) + QDir::separator() +  QString(ICUUSRDATA) + QDir::separator() +  QString("fi.res");
    qDebug() << "Finnish resource file:" << fiResourceName; //"/usr/share/meegotouch/icu/usrdt44l/fi.res"

    QFile fiResource(fiResourceName);
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
        << (QStringList() << "Asia/Tokyo");
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
        << "de_DE@mix-time-and-language=no" // only lc_time matters
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
        << "de_DE@mix-time-and-language=no" // only lc_time matters
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
        << "de_DE@mix-time-and-language=no" // only lc_time matters
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
        << "fi_FI@mix-time-and-language=no" // only lc_time matters
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
        << "fi_FI@mix-time-and-language=no" // language does not matter unless lc_time is empty
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
        << "fi_FI@mix-time-and-language=no" // only lc_time matters
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
        << "fi_FI@mix-time-and-language=no" // only lc_time matters
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
        << "fi_FI@mix-time-and-language=no" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::IslamicCalendar
        << "d.M.y G"
        << "d.M.y G"
        << "d. MMMM y G"
        << "cccc d. MMMM y G"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Islamic calendar, TwentyFourHourTimeFormat24h")
        << "fi_FI@mix-time-and-language=no" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::IslamicCalendar
        << "d.M.y G"
        << "d.M.y G"
        << "d. MMMM y G"
        << "cccc d. MMMM y G"
        << "H:mm"
        << "H:mm:ss"
        << "H:mm:ss z"
        << "H:mm:ss zzzz";
    QTest::newRow("fi_FI, Islamic calendar, TwelveHourTimeFormat24h")
        << "fi_FI@mix-time-and-language=no" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "" // only lc_time matters
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::IslamicCalendar
        << "d.M.y G"
        << "d.M.y G"
        << "d. MMMM y G"
        << "cccc d. MMMM y G"
        << "h:mm a"
        << "h:mm:ss a"
        << "h:mm:ss a z"
        << "h:mm:ss a zzzz";
    //--------------------------------------------------
    QTest::newRow("Arabic, Gregorian calendar, LocaleDefaultTimeFormat24h")
        << "fi_FI" // language does not matter unless lc_time is empty
        << "de_DE" // lc_messages does not matter
        << "ar_EG@mix-time-and-language=no" // only lc_time matters
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
        << "ar_EG@mix-time-and-language=no" // only lc_time matters
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
        << "ar_EG@mix-time-and-language=no" // only lc_time matters
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
        << "ar_EG@mix-time-and-language=no" // only lc_time matters
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
        << "ar_EG@mix-time-and-language=no" // only lc_time matters
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
        << "ar_EG@mix-time-and-language=no" // only lc_time matters
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
        << "ja_JP@mix-time-and-language=no" // only lc_time matters
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
        << "ja_JP@mix-time-and-language=no" // only lc_time matters
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
        << "ja_JP@mix-time-and-language=no" // only lc_time matters
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
        << "ja_JP@mix-time-and-language=no" // only lc_time matters
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
        << "zh_CN@mix-time-and-language=no" // only lc_time matters
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
        << "zh_CN@mix-time-and-language=no" // only lc_time matters
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
        << "zh_CN@mix-time-and-language=no" // only lc_time matters
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
        << "zh_CN@mix-time-and-language=no" // only lc_time matters
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
        << "zh_CN@mix-time-and-language=no" // only lc_time matters
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
        << "zh_CN@mix-time-and-language=no" // only lc_time matters
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
        << "th_TH@mix-time-and-language=no" // only lc_time matters
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
        << "th_TH@mix-time-and-language=no" // only lc_time matters
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
        << "th_TH@mix-time-and-language=no" // only lc_time matters
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
        << "th_TH@mix-time-and-language=no" // only lc_time matters
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
        << "th_TH@mix-time-and-language=no" // only lc_time matters
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
        << "th_TH@mix-time-and-language=no" // only lc_time matters
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
        << "de_BE@mix-time-and-language=no" // only lc_time matters
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
        << "de_BE@mix-time-and-language=no" // only lc_time matters
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
        << "de_BE@mix-time-and-language=no" // only lc_time matters
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
        << "en_BE@mix-time-and-language=no" // only lc_time matters
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
        << "en_BE@mix-time-and-language=no" // only lc_time matters
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
        << "en_BE@mix-time-and-language=no" // only lc_time matters
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
        << "eo@mix-time-and-language=no" // only lc_time matters
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
        << "fi_FI"
        << "fi_FI"
        << "eo@mix-time-and-language=no" // lc_time
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
        << "fi_FI"
        << "fi_FI"
        << "eo@mix-time-and-language=no" // lc_time
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
    QTest::newRow("eo, Gregorian, TwelveHourTimeFormat24h")
        << "fi_FI"
        << "fi_FI"
        << "eo@mix-time-and-language=yes" // lc_time
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yy-MM-dd"
        << "y-LLL-dd"
        << "y-LLLL-dd"
        << "cccc, d LLLL y"
        << "hh:mm a"
        << "hh:mm:ss a"
        << "hh:mm:ss a z"
        << "hh:mm:ss a zzzz";
    QTest::newRow("it_IT, Gregorian, TwentyFourHourTimeFormat24h")
        << "fi_FI"
        << "fi_FI"
        << "it_IT@mix-time-and-language=no" // lc_time
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "dd/MM/yy"
        << "dd/MMM/y"
        << "dd MMMM y"
        << "cccc d LLLL y"
        << "HH:mm"
        << "HH:mm:ss"
        << "HH:mm:ss z"
        << "HH:mm:ss zzzz";
    QTest::newRow("zh_HK, Gregorian, TwelveHourTimeFormat24h")
        << "fi_FI"
        << "fi_FI"
        << "zh_HK@mix-time-and-language=no" // lc_time
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::GregorianCalendar
        << "yyyy/MM/dd"
        << "yyyy/MM/dd"
        << "y年M月d日"
        << "y年M月d日EEEE"
        << "ah:mm"
        << "ahh:mm:ss"
        << "zah時mm分ss秒"
        << "zzzzah時mm分ss秒";
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
            else if(locale.categoryName(MLocale::MLcTime).startsWith("ja")
                    && calendarType == MLocale::GregorianCalendar
                    && (dateType == MLocale::DateLong
                        || dateType == MLocale::DateFull))
                expectedResult = dateResults[dateType] + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime).startsWith("zh")
                    && calendarType == MLocale::GregorianCalendar
                    && (dateType == MLocale::DateLong
                        || dateType == MLocale::DateFull))
                expectedResult = dateResults[dateType] + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime).startsWith("th_TH"))
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

void Ut_MCalendar::testMLocaleSetAndGetCalendar_data()
{
    QTest::addColumn<QString>("localeNameOrig");
    QTest::addColumn<QString>("lcTimeOrig");
    QTest::addColumn<MLocale::CalendarType>("calendarTypeOrig");
    QTest::addColumn<MLocale::CalendarType>("calendarTypeNew");
    QTest::addColumn<QString>("localeNameNew");
    QTest::addColumn<QString>("lcTimeNew");

    QTest::newRow("default->default")
        << "fi"
        << "fi"
        << MLocale::DefaultCalendar
        << MLocale::DefaultCalendar
        << "fi"
        << "fi";
    QTest::newRow("default->default")
        << "fi@calendar=gregorian"
        << "fi"
        << MLocale::DefaultCalendar
        << MLocale::DefaultCalendar
        << "fi@calendar=gregorian"
        << "fi";
    QTest::newRow("gregorian->default")
        << "fi@calendar=islamic"
        << "fi@calendar=gregorian"
        << MLocale::GregorianCalendar
        << MLocale::DefaultCalendar
        << "fi@calendar=islamic"
        << "fi";
    QTest::newRow("gregorian->default")
        << "fi@collation=phonebook;calendar=islamic;foo=bar"
        << "fi@collation=phonebook;calendar=gregorian;foo=bar"
        << MLocale::GregorianCalendar
        << MLocale::DefaultCalendar
        << "fi@collation=phonebook;calendar=islamic;foo=bar"
        << "fi@collation=phonebook;foo=bar";
    QTest::newRow("gregorian->default")
        << "fi@collation=phonebook;calendar=gregorian;foo=bar"
        << ""
        << MLocale::GregorianCalendar
        << MLocale::DefaultCalendar
        << "fi@collation=phonebook;foo=bar"
        << "fi@collation=phonebook;foo=bar";
    QTest::newRow("gregorian->islamic")
        << "fi@collation=phonebook;calendar=gregorian;foo=bar"
        << ""
        << MLocale::GregorianCalendar
        << MLocale::IslamicCalendar
        << "fi@collation=phonebook;calendar=islamic;foo=bar"
        << "fi@collation=phonebook;calendar=islamic;foo=bar";
    QTest::newRow("gregorian->islamic")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=gregorian;foo=bar"
        << MLocale::GregorianCalendar
        << MLocale::IslamicCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic;foo=bar";
    QTest::newRow("islamic->islamic-civil")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic;foo=bar"
        << MLocale::IslamicCalendar
        << MLocale::IslamicCivilCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar";
    QTest::newRow("islamic-civil->islamic")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::IslamicCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic;foo=bar";
    QTest::newRow("default->islamic-civil")
        << "zh_CN"
        << "fi@collation=phonebook;foo=bar"
        << MLocale::DefaultCalendar
        << MLocale::IslamicCivilCalendar
        << "zh_CN"
        << "fi@collation=phonebook;foo=bar;calendar=islamic-civil";
    QTest::newRow("islamic-civil->chinese")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::ChineseCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=chinese;foo=bar";
    QTest::newRow("islamic-civil->hebrew")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::HebrewCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=hebrew;foo=bar";
    QTest::newRow("islamic-civil->japanese")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::JapaneseCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=japanese;foo=bar";
    QTest::newRow("islamic-civil->buddhist")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::BuddhistCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=buddhist;foo=bar";
    QTest::newRow("islamic-civil->persian")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::PersianCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=persian;foo=bar";
    QTest::newRow("islamic-civil->coptic")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::CopticCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=coptic;foo=bar";
    QTest::newRow("islamic-civil->ethiopic")
        << "zh_CN"
        << "fi@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::IslamicCivilCalendar
        << MLocale::EthiopicCalendar
        << "zh_CN"
        << "fi@collation=phonebook;calendar=ethiopic;foo=bar";
}

void Ut_MCalendar::testMLocaleSetAndGetCalendar()
{
    QFETCH(QString, localeNameOrig);
    QFETCH(QString, lcTimeOrig);
    QFETCH(MLocale::CalendarType, calendarTypeOrig);
    QFETCH(MLocale::CalendarType, calendarTypeNew);
    QFETCH(QString, localeNameNew);
    QFETCH(QString, lcTimeNew);
    MLocale locale(localeNameOrig);
    QVERIFY2(locale.isValid(), "constructor did not create a valid locale");
    locale.setCategoryLocale(MLocale::MLcTime, lcTimeOrig);
    QCOMPARE(locale.calendarType(), calendarTypeOrig);
    locale.setCalendarType(calendarTypeNew);
    QCOMPARE(locale.name(), localeNameNew);
    QCOMPARE(locale.categoryName(MLocale::MLcTime), lcTimeNew);
    QCOMPARE(locale.calendarType(), calendarTypeNew);
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromLocaltimeQDateTime_data()
{
    QTest::addColumn<QDateTime>("datetime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<QString>("timeZone");
    QTest::addColumn<MLocale::TimeFormat24h>("timeFormat24h");
    QTest::addColumn<MLocale::CalendarType>("calType");
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
            << QString("fi_FI") // language
            << QString("fi_FI") // lc_messages
            << QString("fi_FI") // lc_time
            << QString("fi_FI") // lc_numeric
            << QString("Europe/Helsinki")
            << MLocale::LocaleDefaultTimeFormat24h
            << MLocale::GregorianCalendar
            << QString("21.7.2008")
            << QString("21.7.2008")
            << QString("21. heinäkuuta 2008")
            << QString("Maanantai 21. heinäkuuta 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 UTC+3.00")
            << QString("12:31:00 Itä-Euroopan kesäaika");
    QTest::newRow("21.7.2008_en_GB_Gregorian")
            << datetime
            << QString("en_GB") // language
            << QString("en_GB") // lc_messages
            << QString("en_GB") // lc_time
            << QString("en_GB") // lc_numeric
            << QString("Europe/Helsinki")
            << MLocale::LocaleDefaultTimeFormat24h
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
            << QString("de_DE") // language
            << QString("de_DE") // lc_messages
            << QString("de_DE") // lc_time
            << QString("de_DE") // lc_numeric
            << QString("Europe/Helsinki")
            << MLocale::LocaleDefaultTimeFormat24h
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
            << QString("nn_NO") // language
            << QString("nn_NO") // lc_messages
            << QString("nn_NO") // lc_time
            << QString("nn_NO") // lc_numeric
            << QString("Europe/Helsinki")
            << MLocale::LocaleDefaultTimeFormat24h
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
            << QString("nb_NO") // language
            << QString("nb_NO") // lc_messages
            << QString("nb_NO") // lc_time
            << QString("nb_NO") // lc_numeric
            << QString("Europe/Helsinki")
            << MLocale::LocaleDefaultTimeFormat24h
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
            << QString("no_NO") // language
            << QString("no_NO") // lc_messages
            << QString("no_NO") // lc_time
            << QString("no_NO") // lc_numeric
            << QString("Europe/Helsinki")
            << MLocale::LocaleDefaultTimeFormat24h
            << MLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. juli 2008")
            << QString("21. juli 2008")
            << QString("mandag 21. juli 2008")
            << QString("12:31")
            << QString("12:31:00")
            << QString("12:31:00 GMT+03:00")
            << QString("kl. 12:31:00 østeuropeisk sommertid");
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@mix-time-and-language=no") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@collation=phonebook;calendar=islamic-civil;numbers=latn;foo=bar;mix-time-and-language=no") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@collation=phonebook;calendar=islamic-civil;numbers=latn;foo=bar") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@collation=phonebook;calendar=islamic-civil;numbers=arab;foo=bar;mix-time-and-language=no") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@collation=phonebook;calendar=islamic-civil;numbers=arab;foo=bar") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ru_RU_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ru_RU@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15.06.04"
        << "15.06.2004"
        << "15 июня 2004 г."
        << "вторник, 15 июня 2004 г."
        << "14:31"
        << "14:31:00"
        << "14:31:00 Восточноевропейское летнее время"
        << "14:31:00 Восточноевропейское летнее время";
    QTest::newRow("15.6.2004_ru_RU_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ru_RU@mix-time-and-language=yes") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15.06.04"
        << "15.06.2004"
        << "15 Kesäkuu 2004"
        << "Tiistai, 15 Kesäkuu 2004"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("15.6.2004_en_US_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_US@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "6/15/04"
        << "Jun 15, 2004"
        << "June 15, 2004"
        << "Tuesday, June 15, 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM GMT+03:00"
        << "2:31:00 PM Eastern European Summer Time";
    QTest::newRow("15.6.2004_en_US_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_US@mix-time-and-language=yes") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "6/15/04"
        << "Kesä 15, 2004"
        << "Kesäkuu 15, 2004"
        << "Tiistai, Kesäkuu 15, 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM UTC+3.00"
        << "2:31:00 PM Itä-Euroopan kesäaika";
    QTest::newRow("15.6.2004_he_IL_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("he_IL@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15/06/04"
        << "15 ביונ 2004"
        << "15 ביוני 2004"
        << "יום שלישי, 15 ביוני 2004"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "14:31:00 GMT+03:00";
    QTest::newRow("15.6.2004_he_IL_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("he_IL@mix-time-and-language=yes") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "15/06/04"
        << "15 Kesä 2004"
        << "15 Kesäkuu 2004"
        << "Tiistai, 15 Kesäkuu 2004"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("15.6.2004_hu_HU_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("hu_HU@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "2004.06.15."
        << "2004.06.15."
        << "2004. június 15."
        << "2004. június 15., kedd"
        << "14:31"
        << "14:31:00"
        << "14:31:00 EEST"
        << "14:31:00 kelet-európai nyári idő";
    QTest::newRow("15.6.2004_hu_HU_Gregorian")
        << QDateTime(QDate(2004, 6, 15), QTime(14, 31))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("hu_HU@mix-time-and-language=yes") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "2004.06.15."
        << "2004.06.15."
        << "2004. Kesäkuu 15."
        << "2004. Kesäkuu 15., Tiistai"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("2010-02-28_en_GB_Gregorian Helsinki Summertime switch")
        << QDateTime(QDate(2011, 3, 27), QTime(3, 01, 00))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_GB@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "27/03/2011"
        << "27 Mar 2011"
        << "27 March 2011"
        << "Sunday, 27 March 2011"
        << "4:01"
        << "4:01:00"
        << "4:01:00 EEST"
        << "4:01:00 Eastern European Summer Time";
    QTest::newRow("2010-02-28_en_GB_Gregorian Berlin Summertime switch")
        << QDateTime(QDate(2011, 3, 27), QTime(2, 01, 00))
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_GB@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Berlin"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << "27/03/2011"
        << "27 Mar 2011"
        << "27 March 2011"
        << "Sunday, 27 March 2011"
        << "3:01"
        << "3:01:00"
        << "3:01:00 CEST"
        << "3:01:00 Central European Summer Time";
}

void Ut_MCalendar::testMLocaleCalendarConversionsFromLocaltimeQDateTime()
{
    QFETCH(QDateTime, datetime);
    QFETCH(QString, localeName);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, lcNumeric);
    QFETCH(QString, timeZone);
    QFETCH(MLocale::TimeFormat24h, timeFormat24h);
    QFETCH(MLocale::CalendarType, calType);
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
    MCalendar::setSystemTimeZone(timeZone);

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
            else
                expectedResult = dateResults[dateType] + ' ' + timeResults[timeType];
#if defined(VERBOSE_OUTPUT)
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream
                << "language " << localeName
                << " lc_time " << lcTime
                << " lc_numeric " << lcNumeric
                << " dateType " << dateType << " timeType " << timeType
                << " format string |"
                << locale.icuFormatString(
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType),
                    calType)
                << "| actual result |"
                << locale.formatDateTime(
                    datetime,
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType),
                    calType)
                << "| expected result |" << expectedResult << "|\n";
            debugStream.flush();
#endif
            QCOMPARE(
                locale.formatDateTime(datetime,
                                      static_cast<MLocale::DateType>(dateType),
                                      static_cast<MLocale::TimeType>(timeType),
                                      calType),
                expectedResult);
            // see whether the result can be parsed back into a QDateTime:
            QDateTime dateTimeParsedFromFormattedResult;
            dateTimeParsedFromFormattedResult
                = locale.parseDateTime(expectedResult,
                                       static_cast<MLocale::DateType>(dateType),
                                       static_cast<MLocale::TimeType>(timeType),
                                       calType);
            if (dateType == MLocale::DateNone && timeType == MLocale::TimeNone)
                QVERIFY2(!dateTimeParsedFromFormattedResult.isValid(),
                         "an invalid datetime should have been returned");
            else if (dateType == MLocale::DateNone) {
                QVERIFY2(dateTimeParsedFromFormattedResult.isValid(),
                         "this datetime should have been valid.");
                // parsing the time without the date may cause offsets
                // due to daylight savings time. Therefore don’t
                // compare the result here.
            }
            else if (timeType == MLocale::TimeNone) {
                QVERIFY2(dateTimeParsedFromFormattedResult.isValid(),
                         "this datetime should have been valid.");
                dateTimeParsedFromFormattedResult.setTime(datetime.time());
                QCOMPARE(dateTimeParsedFromFormattedResult, datetime);
            }
            else {
                QVERIFY2(dateTimeParsedFromFormattedResult.isValid(),
                         "this datetime should have been valid.");
                QCOMPARE(dateTimeParsedFromFormattedResult, datetime);
            }
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
            << QString("Maanantai 21. heinäkuuta 2008 15:31:00 Itä-Euroopan kesäaika");
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

    QTest::newRow("21.7.2008_en_US_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_US@mix-time-and-language=no") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "7/21/08"
        << "Jul 21, 2008"
        << "July 21, 2008"
        << "Monday, July 21, 2008"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM GMT+03:00"
        << "2:31:00 PM Eastern European Summer Time";
    QTest::newRow("21.7.2008_en_US_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_US@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "7/21/08"
        << "Heinä 21, 2008"
        << "Heinäkuu 21, 2008"
        << "Maanantai, Heinäkuu 21, 2008"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM UTC+3.00"
        << "2:31:00 PM Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_de_DE_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("de_DE@mix-time-and-language=no") // lc_time
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
    QTest::newRow("21.7.2008_de_DE_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("de_DE@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21.07.2008"
        << "21. Heinäkuu 2008"
        << "Maanantai, 21. Heinäkuu 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_fi_FI_Gregorian")
        << QString("de_DE")
        << QString("de_DE")
        << QString("fi_FI@mix-time-and-language=no") // lc_time
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
        << "Maanantai 21. heinäkuuta 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_fi_FI_Gregorian")
        << QString("de_DE")
        << QString("de_DE")
        << QString("fi_FI@mix-time-and-language=yes") // lc_time
        << QString("de_DE")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.7.2008"
        << "21.7.2008"
        << "21. Juli 2008"
        << "Montag 21. Juli 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "14:31:00 Osteuropäische Sommerzeit";
    QTest::newRow("21.7.2008_en_GB_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_GB@mix-time-and-language=no") // lc_time
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
    QTest::newRow("21.7.2008_en_GB_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("en_GB@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21/07/2008"
        << "21 Heinä 2008"
        << "21 Heinäkuu 2008"
        << "Maanantai, 21 Heinäkuu 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_nn_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("nn_NO@mix-time-and-language=no") // lc_time
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
    QTest::newRow("21.7.2008_nn_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("nn_NO@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21. Heinä. 2008" // bad, look at the dot!
        << "21. Heinäkuu 2008"
        << "Maanantai 21. Heinäkuu 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("nb_NO@mix-time-and-language=no") // lc_time
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
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("nb_NO@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21. Heinä 2008"
        << "21. Heinäkuu 2008"
        << "Maanantai 21. Heinäkuu 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_no_NO_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("no_NO@mix-time-and-language=no") // lc_time
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
        << QString("no_NO@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21.07.08"
        << "21. Heinä 2008"
        << "21. Heinäkuu 2008"
        << "Maanantai 21. Heinäkuu 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_ja_JP_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ja_JP@mix-time-and-language=no") // lc_time
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
        << QString("ja_JP@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "08/07/21"
        << "2008/07/21"
        << "2008-7-21"
        << "2008-7-21 Maanantai"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_zh_CN_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("zh_CN@mix-time-and-language=no") // lc_time
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
        << "格林尼治标准时间+03:00下午2时31分00秒"
        << "东欧夏令时间下午2时31分00秒";
    QTest::newRow("21.7.2008_zh_CN_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("zh_CN@mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "08-7-21"
        << "2008-7-21"
        << "2008-7-21"
        << "2008-7-21 Maanantai"
        << "PM 2:31"
        << "PM 2:31:00"
        << "UTC+3.00 PM 2:31:00"
        << "Itä-Euroopan kesäaika PM 2:31:00";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@mix-time-and-language=no") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@numbers=latn;mix-time-and-language=no") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@numbers=latn") // lc_time
        << QString("ar_EG") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@numbers=arab;mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@numbers=arab") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "١٥‏/٦‏/٢٠٠٤"
        << "١٥‏/٠٦‏/٢٠٠٤"
        << "١٥ يونيو، ٢٠٠٤"
        << "الثلاثاء، ١٥ يونيو، ٢٠٠٤"
        << "٢:٣١ م"
        << "٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م"
        << "جرينتش+٠٣:٠٠ ٢:٣١:٠٠ م";
    QTest::newRow("15.6.2004_ar_EG_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15‏/6‏/2004"
        << "15‏/06‏/2004"
        << "15 يونيو، 2004"
        << "الثلاثاء، 15 يونيو، 2004"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_ar_EG_Islamic")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ar_EG@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::IslamicCalendar
        << 2004
        << 6
        << 15
        << "27‏/4‏/1425"
        << "27‏/04‏/1425"
        << "27 ربيع الآخر، 1425"
        << "الثلاثاء، 27 ربيع الآخر، 1425"
        << "2:31 م"
        << "2:31:00 م"
        << "جرينتش+03:00 2:31:00 م"
        << "جرينتش+03:00 2:31:00 م";
    QTest::newRow("15.6.2004_fa_IR_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fa_IR@mix-time-and-language=no") // lc_time
        << QString("fa_IR") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "۶/۱۵/۰۴"
        << "جون ۱۵, ۲۰۰۴"
        << "جون ۱۵, ۲۰۰۴"
        << "سه‌شنبه, جون ۱۵, ۲۰۰۴"
        << "۱۴:۳۱"
        << "۱۴:۳۱:۰۰"
        << "۱۴:۳۱:۰۰ (GMT+۰۳:۰۰)"
        << "۱۴:۳۱:۰۰ (وقت تابستانی شرق اروپا)";
    QTest::newRow("15.6.2004_fa_IR_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fa_IR@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "6/15/04"
        << "جون 15, 2004"
        << "جون 15, 2004"
        << "سه‌شنبه, جون 15, 2004"
        << "14:31"
        << "14:31:00"
        << "14:31:00 (GMT+03:00)"
        << "14:31:00 (وقت تابستانی شرق اروپا)";
    QTest::newRow("15.6.2004_fa_IR_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fa_IR@numbers=arabext;mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "۶/۱۵/۰۴"
        << "جون ۱۵, ۲۰۰۴"
        << "جون ۱۵, ۲۰۰۴"
        << "سه‌شنبه, جون ۱۵, ۲۰۰۴"
        << "۱۴:۳۱"
        << "۱۴:۳۱:۰۰"
        << "۱۴:۳۱:۰۰ (GMT+۰۳:۰۰)"
        << "۱۴:۳۱:۰۰ (وقت تابستانی شرق اروپا)";
    QTest::newRow("15.6.2004_fa_IR_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fa_IR@numbers=arabext") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "۶/۱۵/۰۴"
        << "جون ۱۵, ۲۰۰۴"
        << "جون ۱۵, ۲۰۰۴"
        << "سه‌شنبه, جون ۱۵, ۲۰۰۴"
        << "۱۴:۳۱"
        << "۱۴:۳۱:۰۰"
        << "۱۴:۳۱:۰۰ (GMT+۰۳:۰۰)"
        << "۱۴:۳۱:۰۰ (وقت تابستانی شرق اروپا)";
    QTest::newRow("15.6.2004_fa_IR_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fa_IR@numbers=latn;mix-time-and-language=no") // lc_time
        << QString("fa_IR") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "6/15/04"
        << "جون 15, 2004"
        << "جون 15, 2004"
        << "سه‌شنبه, جون 15, 2004"
        << "14:31"
        << "14:31:00"
        << "14:31:00 (GMT+03:00)"
        << "14:31:00 (وقت تابستانی شرق اروپا)";
    QTest::newRow("15.6.2004_fa_IR_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fa_IR@numbers=latn") // lc_time
        << QString("fa_IR") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "6/15/04"
        << "جون 15, 2004"
        << "جون 15, 2004"
        << "سه‌شنبه, جون 15, 2004"
        << "14:31"
        << "14:31:00"
        << "14:31:00 (GMT+03:00)"
        << "14:31:00 (وقت تابستانی شرق اروپا)";
    QTest::newRow("15.6.2004_ur_PK_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ur_PK@mix-time-and-language=no") // lc_time
        << QString("ur_PK") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15/6/04"
        << "15, جون 2004"
        << "15, جون 2004"
        << "منگل, 15, جون 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM GMT+03:00"
        << "2:31:00 PM GMT+03:00";
    QTest::newRow("15.6.2004_ur_PK_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ur_PK@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15/6/04"
        << "15, جون 2004"
        << "15, جون 2004"
        << "منگل, 15, جون 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM GMT+03:00"
        << "2:31:00 PM GMT+03:00";
    QTest::newRow("15.6.2004_ur_PK_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("ur_PK@mix-time-and-language=yes") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15/6/04"
        << "15, Kesä 2004"
        << "15, Kesäkuu 2004"
        << "Tiistai, 15, Kesäkuu 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM UTC+3.00"
        << "2:31:00 PM Itä-Euroopan kesäaika";
    QTest::newRow("15.6.2004_hi_HI_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("hi_HI@mix-time-and-language=no") // lc_time
        << QString("hi_HI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "१५-६-०४"
        << "१५-०६-२००४"
        << "१५ जून २००४"
        << "मंगलवार, १५ जून २००४"
        << "२:३१ PM"
        << "२:३१:०० PM"
        << "२:३१:०० PM GMT+०३:००"
        << "२:३१:०० PM GMT+०३:००";
    QTest::newRow("15.6.2004_hi_HI_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("hi_HI@mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15-6-04"
        << "15-06-2004"
        << "15 जून 2004"
        << "मंगलवार, 15 जून 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM GMT+03:00"
        << "2:31:00 PM GMT+03:00";
    QTest::newRow("15.6.2004_hi_HI_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("hi_HI@numbers=deva;mix-time-and-language=no") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "१५-६-०४"
        << "१५-०६-२००४"
        << "१५ जून २००४"
        << "मंगलवार, १५ जून २००४"
        << "२:३१ PM"
        << "२:३१:०० PM"
        << "२:३१:०० PM GMT+०३:००"
        << "२:३१:०० PM GMT+०३:००";
    QTest::newRow("15.6.2004_hi_HI_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("hi_HI@numbers=deva;mix-time-and-language=yes") // lc_time
        << QString("fi_FI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "१५-६-०४"
        << "१५-०६-२००४"
        << "१५ Kesäkuu २००४"
        << "Tiistai, १५ Kesäkuu २००४"
        << "२:३१ PM"
        << "२:३१:०० PM"
        << "२:३१:०० PM UTC+३.००"
        << "२:३१:०० PM Itä-Euroopan kesäaika";
    QTest::newRow("15.6.2004_hi_HI_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("hi_HI@numbers=latn;mix-time-and-language=no") // lc_time
        << QString("hi_HI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15-6-04"
        << "15-06-2004"
        << "15 जून 2004"
        << "मंगलवार, 15 जून 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM GMT+03:00"
        << "2:31:00 PM GMT+03:00";
    QTest::newRow("15.6.2004_hi_HI_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("hi_HI@numbers=latn;mix-time-and-language=yes") // lc_time
        << QString("hi_HI") // lc_numeric
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2004
        << 6
        << 15
        << "15-6-04"
        << "15-06-2004"
        << "15 Kesäkuu 2004"
        << "Tiistai, 15 Kesäkuu 2004"
        << "2:31 PM"
        << "2:31:00 PM"
        << "2:31:00 PM UTC+3.00"
        << "2:31:00 PM Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_th_TH_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("th_TH@calendar=gregorian;mix-time-and-language=no") // lc_time
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
    QTest::newRow("21.7.2008_th_TH_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("th_TH@calendar=gregorian;mix-time-and-language=yes") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::GregorianCalendar
        << 2008
        << 7
        << 21
        << "21/7/2008"
        << "21 Heinä 2008"
        << "21 Heinäkuu 2008"
        << "Maanantai 21 Heinäkuu jKr. 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 UTC+3.00"
        << "14:31:00 Itä-Euroopan kesäaika";
    QTest::newRow("21.7.2008_th_TH_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("th_TH@calendar=gregorian;mix-time-and-language=no") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::BuddhistCalendar
        << 2008
        << 7
        << 21
        << "21/7/2551"
        << "21 ก.ค. 2551"
        << "21 กรกฎาคม 2551"
        << "วันจันทร์ที่ 21 กรกฎาคม พ.ศ. 2551"
        << "14:31"
        << "14:31:00"
        << "14 นาฬิกา 31 นาที 00 วินาที GMT+03:00"
        << "14 นาฬิกา 31 นาที 00 วินาที GMT+03:00";
    QTest::newRow("21.7.2008_th_TH_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("th_TH@calendar=gregorian;mix-time-and-language=no") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::DefaultCalendar
        << 2008
        << 7
        << 21
        << "21/7/2551"
        << "21 ก.ค. 2551"
        << "21 กรกฎาคม 2551"
        << "วันจันทร์ที่ 21 กรกฎาคม พ.ศ. 2551"
        << "14:31"
        << "14:31:00"
        << "14 นาฬิกา 31 นาที 00 วินาที GMT+03:00"
        << "14 นาฬิกา 31 นาที 00 วินาที GMT+03:00";
    QTest::newRow("21.7.2008_vi_VN_Gregorian")
        << QString("fi_FI")
        << QString("fi_FI") // lc_messages
        << QString("vi_VN@calendar=gregorian;mix-time-and-language=no") // lc_time
        << QString("fi_FI")
        << "Europe/Helsinki"
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::DefaultCalendar
        << 2008
        << 7
        << 21
        << "21/07/2008"
        << "21-07-2008"
        << "21 tháng 7 năm 2008"
        << "Thứ hai, ngày 21 tháng bảy năm 2008"
        << "14:31"
        << "14:31:00"
        << "14:31:00 GMT+03:00"
        << "14:31:00 GMT+03:00";
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
    locale.setCalendarType(calType);
    MCalendar::setSystemTimeZone("Europe/Helsinki");
    MCalendar mcal(calType);
    MCalendar mcalConstructedFromLocale(locale);
    QCOMPARE(mcal.type(), calType);
    QCOMPARE(mcalConstructedFromLocale.type(), calType);
    QDateTime dateTime(QDate(year, month, day), QTime(14, 31, 0, 0));
    mcal.setDateTime(dateTime);
    mcalConstructedFromLocale.setDateTime(dateTime);

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
            else if (locale.categoryName(MLocale::MLcTime).startsWith("ja_JP")
                     && calType == MLocale::GregorianCalendar
                     && (dateType == MLocale::DateLong
                         || dateType == MLocale::DateFull))
                    if (MLocale::localeScript(locale.categoryName(MLocale::MLcMessages)) != QLatin1String("Kana")
                        && MLocale::localeScript(locale.categoryName(MLocale::MLcMessages)) != QLatin1String("Hira")
                        && MLocale::localeScript(locale.categoryName(MLocale::MLcMessages)) != QLatin1String("Hani")
                        && !lcTime.contains("mix-time-and-language=no"))
                        expectedResult = dateResults[dateType] + ' ' + timeResults[timeType];
                    else
                        expectedResult = dateResults[dateType] + timeResults[timeType];
            else if (locale.categoryName(MLocale::MLcTime).startsWith("zh_CN")
                     && calType == MLocale::GregorianCalendar
                     && (dateType == MLocale::DateLong
                         || dateType == MLocale::DateFull))
                    if (MLocale::localeScript(locale.categoryName(MLocale::MLcMessages)) != QLatin1String("Hani")
                        && !lcTime.contains("mix-time-and-language=no"))
                        expectedResult = dateResults[dateType] + ' ' + timeResults[timeType];
                    else
                        expectedResult = dateResults[dateType] + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime).startsWith("th_TH"))
                expectedResult = dateResults[dateType] + ", " + timeResults[timeType];
            else if(locale.categoryName(MLocale::MLcTime).startsWith("fa_IR")) {
                if (dateType == MLocale::DateShort || dateType == MLocale::DateMedium)
                        expectedResult = dateResults[dateType] + "،‏ " + timeResults[timeType];
                    else
                        expectedResult = dateResults[dateType] + "، ساعت " + timeResults[timeType];
            }
            else if(locale.categoryName(MLocale::MLcTime).startsWith("vi")) {
                expectedResult = timeResults[timeType] + " " + dateResults[dateType];
            }
            else
                expectedResult = dateResults[dateType] + ' ' + timeResults[timeType];
#if defined(VERBOSE_OUTPUT)
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream
                << "language " << localeName
                << " lc_time " << lcTime
                << " lc_numeric " << lcNumeric
                << " dateType " << dateType << " timeType " << timeType
                << " format string |"
                << locale.icuFormatString(
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType),
                    calType)
                << "| actual result mcalendar |"
                << locale.formatDateTime(
                    mcal,
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType))
                << "| actual result qdatetime |"
                << locale.formatDateTime(
                    dateTime,
                    static_cast<MLocale::DateType>(dateType),
                    static_cast<MLocale::TimeType>(timeType),
                    calType)
                << "| expected result |" << expectedResult << "|\n";
            debugStream.flush();
#endif
#if 1
            QCOMPARE(
                locale.formatDateTime(mcal,
                                      static_cast<MLocale::DateType>(dateType),
                                      static_cast<MLocale::TimeType>(timeType)),
                expectedResult);
            QCOMPARE(
                locale.formatDateTime(mcalConstructedFromLocale,
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

#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "result format: " << format
        << "\n";
    debugStream.flush();
#endif

    // we allow off-by-one conversion because the dates can be a bit ambiguous
    // e.g. islamic day starts from sunset
    if (format.contains("22.12.1428")
        || format.contains("23.12.1428")
        || format.contains("21.12.1428")) {
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
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("format");
    QTest::addColumn<QString>("expectedResult");

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
            << "fi_FI"
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
            << "fi_FI"
            << "fi_FI"
            << "%b" // locale's abbreviated month name (e.g., Jan)
            << "Helmi";
    QTest::newRow("fi_FI %c")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
            << "fi_FI"
            << "%h" // %h same as %b, locale's abbreviated month name (e.g., Jan)
            << "Helmi";
    QTest::newRow("fi_FI %j")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
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
            << "fi_FI"
            << "fi_FI"
            << "%m" // month (01..12)
            << "02";
    QTest::newRow("fi_FI %r")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
            << "fi_FI"
            << "%r" // locale specific 12-hour clock time
            << "12:25 PM";
    QTest::newRow("fi_FI %t")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
            << "fi_FI"
            << "%A" // locale's full weekday name (e.g., Sunday)
            << "Sunnuntai";
    QTest::newRow("fi_FI %B")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
            << "fi_FI"
            << "%B" // locale's full month name (e.g., January)
            << "Helmikuu";
    QTest::newRow("fi_FI %C")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "fi_FI"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
            << "en_GB"
            << "%m" // month (01..12)
            << "02";
    QTest::newRow("en_GB %r")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "ar_EG"
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
            << "en_GB"
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
            << "ar_EG"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "en_GB"
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
            << "de_DE"
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
            << "fi_FI"
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
            << "en_GB"
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
            << "de_DE"
            << "de_DE"
            << "%R %Z"
            << "12:25 (Finnland)";
    QTest::newRow("fi_FI %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "fi_FI"
            << "fi_FI"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "PM";
    QTest::newRow("en_GB %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "en_GB"
            << "en_GB"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "PM";
    QTest::newRow("zh_CN zh_CN zh_CN %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "zh_CN"
            << "zh_CN"
            << "zh_CN"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "下午";
    QTest::newRow("zh_CN zh_CN zh_CN %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "zh_CN"
            << "zh_CN"
            << "fi_FI" // no mixing by default for Chinese messages
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "PM";
    QTest::newRow("zh_CN zh_CN zh_CN %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "zh_CN"
            << "zh_CN"
            << "fi_FI@mix-time-and-language=no"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "PM";
    QTest::newRow("zh_CN zh_CN zh_CN %p")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "zh_CN"
            << "zh_CN"
            << "fi_FI@mix-time-and-language=yes"
            << "%p" // %p locale's equivalent of either AM or PM; blank if not known
            << "下午";
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
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, format);
    QFETCH(QString, expectedResult);

    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCalendarType(calendarType);
    MCalendar mcal(locale);
    mcal.setDate(year, month, day);
    mcal.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(localeName);

#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << localeName
        << " lc_messages " << lcMessages
        << " lc_time " << lcTime
        << " format: " << format
        << " expectedResult: " << expectedResult
        << " result: " << locale.formatDateTime(mcal, format)
        << "\n";
//    debugStream << "QLocale time ShortFormat " << qlocale.toString(time, QLocale::ShortFormat) << "QLocale time LongFormat " << qlocale.toString(time, QLocale::LongFormat) << "\n";
//    debugStream << "QLocale format" << qlocale.toString(time, "hh:mm") << "\n";
//    debugStream << "QLocale date ShortFormat" << qlocale.toString(date, QLocale::ShortFormat) << "QLocale date LongFormat" << qlocale.toString(date, QLocale::LongFormat) << "\n";
    debugStream << "DateNone TimeNone " << locale.formatDateTime(datetime, MLocale::DateNone, MLocale::TimeNone, calendarType) << "\n";
    debugStream << "DateNone TimeShort " << locale.formatDateTime(datetime, MLocale::DateNone, MLocale::TimeShort, calendarType) << "\n";
    debugStream << "DateShort TimeNone " << locale.formatDateTime(datetime, MLocale::DateShort, MLocale::TimeNone, calendarType) << "\n";
//    debugStream << "DateShort TimeShort" << locale.formatDateTime(datetime, MLocale::DateShort, MLocale::TimeShort, calendarType) << "\n";
//    debugStream << "DateMedium TimeMedium" << locale.formatDateTime(datetime, MLocale::DateMedium, MLocale::TimeMedium, calendarType) << "\n";
//    debugStream << "DateLong TimeLong" << locale.formatDateTime(datetime, MLocale::DateLong, MLocale::TimeLong, calendarType) << "\n";
//    debugStream << "DateFull TimeFull" << locale.formatDateTime(datetime, MLocale::DateFull, MLocale::TimeFull, calendarType) << "\n";
    debugStream.flush();
#endif
    QCOMPARE(locale.formatDateTime(mcal, format), expectedResult);
    QCOMPARE(locale.formatDateTime(datetime, format), expectedResult);
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
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<QString>("timeZone");
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
            << "fi_FI" // language
            << "fi_FI" // lc_messages
            << "fi_FI" // lc_time
            << "fi_FI" // lc_numeric
            << "Europe/Helsinki"
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
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "de_DE" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
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
            << "zh_CN" // language
            << "zh_CN" // lc_messages
            << "zh_CN" // lc_time
            << "zh_CN" // lc_numeric
            << "Europe/Helsinki"
            << "G" // era designator
            << "公元";
    QTest::newRow("ja_JP “G”")
            << MLocale::DefaultCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ja_JP" // language
            << "ja_JP" // lc_messages
            << "ja_JP@calendar=japanese;collation=standard" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "G" // era designator
            << "AD";
    QTest::newRow("ja_JP “G”")
            << MLocale::DefaultCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ja_JP@calendar=japanese;collation=standard" // language
            << "ja_JP" // lc_messages
            << "" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "G" // era designator
            << "AD";
    QTest::newRow("ja_JP “G”")
            << MLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "ja_JP" // language
            << "ja_JP" // lc_messages
            << "ja_JP@calendar=japanese" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
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
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=no" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "G" // era designator
            << "平成";
    QTest::newRow("ja_JP “G”")
            << MLocale::JapaneseCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=yes" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "G" // era designator
            << "Heisei";
    QTest::newRow("ja_JP “G”")
            << MLocale::JapaneseCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE" // language
            << "he_IL" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=yes" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "G" // era designator
            << "Heisei";
    QTest::newRow("ja_JP “Gy年M月d日EEEE時mm分ss秒 zzzz”")
            << MLocale::JapaneseCalendar
            << 2008
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ja_JP" // language
            << "he_IL" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=no" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "Gy年M月d日EEEE時mm分ss秒 zzzz"
            << "平成20年7月22日火曜日時31分03秒 東欧夏時間";
    QTest::newRow("ja_JP “Gy年M月d日EEEE時mm分ss秒 zzzz”")
            << MLocale::JapaneseCalendar
            << 2008
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ja_JP" // language
            << "he_IL" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=yes" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "Gy年M月d日EEEE時mm分ss秒 zzzz"
            << "Heisei20年7月22日יום שלישי時31分03秒 GMT+03:00";
    QTest::newRow("ja_JP “Gy年M月d日EEEE時mm分ss秒 zzzz”")
            << MLocale::JapaneseCalendar
            << 2008
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ja_JP" // language
            << "ar_SA" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=yes" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "Gy年M月d日EEEE時mm分ss秒 zzzz"
            << "هيسي20年7月22日الثلاثاء時31分03秒 جرينتش+03:00";
    QTest::newRow("ja_JP “Gy年M月d日EEEE時mm分ss秒 zzzz”")
            << MLocale::JapaneseCalendar
            << 646 // http://en.wikipedia.org/wiki/Taika_%28era%29
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ja_JP" // language
            << "he_IL" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=no" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "Gy年M月d日EEEE時mm分ss秒 zzzz"
            << "大化2年7月22日土曜日時31分03秒 GMT+01:39:52"; // strange tz offset!
    QTest::newRow("ja_JP “Gy年M月d日EEEE時mm分ss秒 zzzz”")
            << MLocale::JapaneseCalendar
            << 646 // http://en.wikipedia.org/wiki/Taika_%28era%29
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ja_JP" // language
            << "he_IL" // lc_messages
            << "ja_JP@calendar=gregorian;mix-time-and-language=yes" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "Gy年M月d日EEEE時mm分ss秒 zzzz"
            << "טאיקה2年7月22日יום שבת時31分03秒 GMT+01:39:52"; // strange tz offset!
    QTest::newRow("ja_JP “y年M月d日EEEEH時mm分ss秒 zzzz”")
            << MLocale::GregorianCalendar
            << 2008
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ja_JP" // language
            << "ja_JP" // lc_messages
            << "ja_JP" // lc_time
            << "ja_JP" // lc_numeric
            << "Europe/Helsinki"
            << "y年M月d日EEEEH時mm分ss秒 zzzz"
            << "2008年7月22日火曜日14時31分03秒 東欧夏時間";
    QTest::newRow("ar_SA “EEEE، d MMMM، y zzzz H:mm:ss”")
            << MLocale::GregorianCalendar
            << 2008
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ar_SA" // language
            << "ar_SA" // lc_messages
            << "ar_SA" // lc_time
            << "ar_SA" // lc_numeric
            << "Europe/Helsinki"
            << "EEEE، d MMMM، y zzzz H:mm:ss"
            << "الثلاثاء، ٢٢ يوليو، ٢٠٠٨ جرينتش+٠٣:٠٠ ١٤:٣١:٠٣";
    QTest::newRow("ar_SA “EEEE، d MMMM، y zzzz H:mm:ss”")
            << MLocale::GregorianCalendar
            << 2008
            << 7
            << 22
            << 14
            << 31
            << 3
            << "ar_SA" // language
            << "ar_SA" // lc_messages
            << "ar_SA" // lc_time
            << "fi_FI" // lc_numeric: force unlocalized digits
            << "Europe/Helsinki"
            << "EEEE، d MMMM، y zzzz H:mm:ss"
            << "الثلاثاء، 22 يوليو، 2008 جرينتش+03:00 14:31:03";
    QTest::newRow("id_ID “EEEE”")
            << MLocale::GregorianCalendar
            << 2011
            << 2
            << 17
            << 14
            << 31
            << 3
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "id_ID@mix-time-and-language=no" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "EEEE"
            << "Kamis";
    QTest::newRow("id_ID “EEEE”")
            << MLocale::GregorianCalendar
            << 2011
            << 2
            << 17
            << 14
            << 31
            << 3
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "id_ID@mix-time-and-language=yes" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "EEEE"
            << "Donnerstag";
    QTest::newRow("fi_FI “ccc”")
            << MLocale::GregorianCalendar
            << 2011
            << 2
            << 17
            << 14
            << 31
            << 3
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "fi_FI@mix-time-and-language=no" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "ccc"
            << "to";
    QTest::newRow("de_DE “EEE”")
            << MLocale::GregorianCalendar
            << 2011
            << 2
            << 17
            << 14
            << 31
            << 3
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "de_DE@mix-time-and-language=no" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "EEE"
            << "Do.";
    QTest::newRow("fi_FI “MMM”")
            << MLocale::GregorianCalendar
            << 2011
            << 2
            << 17
            << 14
            << 31
            << 3
            << "de_DE" // language
            << "de_DE" // lc_messages
            << "fi_FI@mix-time-and-language=no" // lc_time
            << "de_DE" // lc_numeric
            << "Europe/Helsinki"
            << "MMM"
            << "helmi";
    QTest::newRow("de_DE “MMM”")
            << MLocale::GregorianCalendar
            << 2011
            << 2
            << 17
            << 14
            << 31
            << 3
            << "fi_FI" // language
            << "fi_FI" // lc_messages
            << "de_DE@mix-time-and-language=no" // lc_time
            << "fi_FI" // lc_numeric
            << "Europe/Helsinki"
            << "MMM"
            << "Feb";
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
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, lcNumeric);
    QFETCH(QString, timeZone);
    QFETCH(QString, format);
    QFETCH(QString, result);

    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
    MCalendar::setSystemTimeZone(timeZone);
    locale.setCalendarType(calendarType);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    MCalendar mcal(locale);
    mcal.setDateTime(datetime);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "--------------------\n"
        << " localeName: " << localeName
        << " lcMessages: " << lcMessages
        << " lcTime: " << lcTime
        << " lcNumeric: " << lcNumeric
        << "\n"
        << " after locale.setCalendarType(" << calendarType << ")"
        << "\n"
        << " localeName: " << locale.name()
        << " lcMessages: " << locale.categoryName(MLocale::MLcMessages)
        << " lcTime: " << locale.categoryName(MLocale::MLcTime)
        << " lcNumeric: " << locale.categoryName(MLocale::MLcNumeric)
        << "\n"
        << " format: " << format
        << " actual result mcal: " << locale.formatDateTimeICU(mcal, format)
        << " actual result datetime: " << locale.formatDateTimeICU(datetime, format)
        << " expected result: " << result << "\n";
    debugStream.flush();
#endif
    QCOMPARE(locale.formatDateTimeICU(mcal, format), result);
    QCOMPARE(locale.formatDateTimeICU(datetime, format), result);
}

void Ut_MCalendar::testWeekdaySymbols_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<QStringList>("symbols");

    QStringList fi_FI_Symbols = QStringList() << "Maanantai"
                                              << "Tiistai"
                                              << "Keskiviikko"
                                              << "Torstai"
                                              << "Perjantai"
                                              << "Lauantai"
                                              << "Sunnuntai";
    QStringList en_GB_Symbols = QStringList() << "Monday"
                                              << "Tuesday"
                                              << "Wednesday"
                                              << "Thursday"
                                              << "Friday"
                                              << "Saturday"
                                              << "Sunday";
    QStringList nn_NO_Symbols = QStringList() << "Måndag"
                                              << "Tysdag"
                                              << "Onsdag"
                                              << "Torsdag"
                                              << "Fredag"
                                              << "Laurdag"
                                              << "Søndag";
    QStringList nb_NO_Symbols = QStringList() << "Mandag"
                                              << "Tirsdag"
                                              << "Onsdag"
                                              << "Torsdag"
                                              << "Fredag"
                                              << "Lørdag"
                                              << "Søndag";
    QStringList no_NO_Symbols = nb_NO_Symbols;
    QStringList zh_CN_Symbols = QStringList() << "星期一"
                                              << "星期二"
                                              << "星期三"
                                              << "星期四"
                                              << "星期五"
                                              << "星期六"
                                              << "星期日";
    QTest::newRow("weekday_symbols fi_FI fi_FI fi_FI")
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << MLocale::GregorianCalendar
        << fi_FI_Symbols;
    QTest::newRow("weekday_symbols en_GB en_GB en_GB")
        << "en_GB"
        << "en_GB"
        << "en_GB"
        << MLocale::GregorianCalendar
        << en_GB_Symbols;
    QTest::newRow("weekday_symbols nn_NO nn_NO nn_NO")
        << "nn_NO"
        << "nn_NO"
        << "nn_NO"
        << MLocale::GregorianCalendar
        << nn_NO_Symbols;
    QTest::newRow("weekday_symbols nb_NO nb_NO nb_NO")
        << "nb_NO"
        << "nb_NO"
        << "nb_NO"
        << MLocale::GregorianCalendar
        << nb_NO_Symbols;
    QTest::newRow("weekday_symbols no_NO no_NO no_NO")
        << "no_NO"
        << "no_NO"
        << "no_NO"
        << MLocale::GregorianCalendar
        << no_NO_Symbols;
    QTest::newRow("weekday_symbols zh_CN zh_CN zh_CN")
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << MLocale::GregorianCalendar
        << zh_CN_Symbols;
    QTest::newRow("weekday_symbols zh_CN zh_CN zh_CN")
        << "zh_CN"
        << "zh_CN"
        << "fi_FI@mix-time-and-language=no"
        << MLocale::GregorianCalendar
        << fi_FI_Symbols;
    QTest::newRow("weekday_symbols zh_CN zh_CN zh_CN")
        << "zh_CN"
        << "zh_CN"
        << "fi_FI" // no mixing by default for Chinese messages
        << MLocale::GregorianCalendar
        << fi_FI_Symbols;
    QTest::newRow("weekday_symbols zh_CN zh_CN zh_CN")
        << "zh_CN"
        << "zh_CN"
        << "fi_FI@mix-time-and-language=yes"
        << MLocale::GregorianCalendar
        << zh_CN_Symbols;
}

void Ut_MCalendar::testWeekdaySymbols()
{
    QFETCH(QString, language);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(QStringList, symbols);

    MLocale locale(language);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    MCalendar mcal(calendarType);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << language
        << " lc_messages " << lcMessages
        << " lc_time " << lcTime
        << " calendarType " << calendarType << "\n";
    for (int i = 1; i <= 7; ++i) {
        debugStream
            << " i " << i
            << " weekday name " << locale.weekdayName(mcal, i)
            << " expected " << symbols.at(i-1)
            << "\n";
    }
    debugStream.flush();
#endif
    for (int i = 1; i <= 7; ++i)
        QCOMPARE(locale.weekdayName(mcal, i), symbols.at(i-1));
}

void Ut_MCalendar::testMonthSymbols_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<QStringList>("symbols");

    QStringList fi_FI_Symbols = QStringList() << "Tammikuu"
                                              << "Helmikuu"
                                              << "Maaliskuu"
                                              << "Huhtikuu"
                                              << "Toukokuu"
                                              << "Kesäkuu"
                                              << "Heinäkuu"
                                              << "Elokuu"
                                              << "Syyskuu"
                                              << "Lokakuu"
                                              << "Marraskuu"
                                              << "Joulukuu";
    QStringList en_GB_Symbols = QStringList() << "January"
                                              << "February"
                                              << "March"
                                              << "April"
                                              << "May"
                                              << "June"
                                              << "July"
                                              << "August"
                                              << "September"
                                              << "October"
                                              << "November"
                                              << "December";
    QStringList nn_NO_Symbols = QStringList() << "Januar"
                                              << "Februar"
                                              << "Mars"
                                              << "April"
                                              << "Mai"
                                              << "Juni"
                                              << "Juli"
                                              << "August"
                                              << "September"
                                              << "Oktober"
                                              << "November"
                                              << "Desember";
    QStringList nb_NO_Symbols = QStringList() << "Januar"
                                              << "Februar"
                                              << "Mars"
                                              << "April"
                                              << "Mai"
                                              << "Juni"
                                              << "Juli"
                                              << "August"
                                              << "September"
                                              << "Oktober"
                                              << "November"
                                              << "Desember";
    QStringList no_NO_Symbols = nb_NO_Symbols;
    QStringList zh_CN_Symbols = QStringList() << "一月"
                                              << "二月"
                                              << "三月"
                                              << "四月"
                                              << "五月"
                                              << "六月"
                                              << "七月"
                                              << "八月"
                                              << "九月"
                                              << "十月"
                                              << "十一月"
                                              << "十二月";

    QTest::newRow("month_symbols de_DE fi_FI fi_FI")
            << "de_DE"
            << "fi_FI"
            << "fi_FI"
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;
    QTest::newRow("month_symbols de_DE en_GB fi_FI")
            << "de_DE"
            << "en_GB"
            << "fi_FI"
            << MLocale::GregorianCalendar
            << en_GB_Symbols;
    QTest::newRow("month_symbols fi_FI en_GB fi_FI@mix-time-and-language=no")
            << "fi_FI"
            << "en_GB"
            << "fi_FI@mix-time-and-language=no"
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;
    QTest::newRow("month_symbols de_DE en_GB de_DE")
            << "de_DE"
            << "en_GB"
            << "de_DE"
            << MLocale::GregorianCalendar
            << en_GB_Symbols;
    QTest::newRow("month_symbols de_DE nb_NO de_DE")
            << "de_DE"
            << "nb_NO"
            << "de_DE"
            << MLocale::GregorianCalendar
            << nb_NO_Symbols;
    QTest::newRow("month_symbols nn_NO nn_NO nn_NO")
            << "nn_NO"
            << "nn_NO"
            << "nn_NO"
            << MLocale::GregorianCalendar
            << nn_NO_Symbols;
    QTest::newRow("month_symbols nb_NO nb_NO nb_NO")
            << "nb_NO"
            << "nb_NO"
            << "nb_NO"
            << MLocale::GregorianCalendar
            << no_NO_Symbols;
    QTest::newRow("month_symbols no_NO no_NO no_NO")
            << "no_NO"
            << "no_NO"
            << "no_NO"
            << MLocale::GregorianCalendar
            << no_NO_Symbols;
    QTest::newRow("month_symbols zh_CN zh_CN zh_CN")
            << "zh_CN"
            << "zh_CN"
            << "zh_CN"
            << MLocale::GregorianCalendar
            << zh_CN_Symbols;
    QTest::newRow("month_symbols zh_CN zh_CN zh_CN")
            << "zh_CN"
            << "fi_FI"
            << "zh_CN"
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;
    QTest::newRow("month_symbols zh_CN zh_CN zh_CN")
            << "zh_CN"
            << "fi_FI"
            << "zh_CN@mix-time-and-language=no"
            << MLocale::GregorianCalendar
            << zh_CN_Symbols;
    QTest::newRow("month_symbols zh_CN zh_CN zh_CN")
            << "zh_CN"
            << "zh_CN"
            << "fi_FI" // no mixing by default for Chinese messages
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;
    QTest::newRow("month_symbols zh_CN zh_CN zh_CN")
            << "zh_CN"
            << "zh_CN"
            << "fi_FI@mix-time-and-language=no"
            << MLocale::GregorianCalendar
            << fi_FI_Symbols;
    QTest::newRow("month_symbols zh_CN zh_CN zh_CN")
            << "zh_CN"
            << "zh_CN"
            << "fi_FI@mix-time-and-language=yes"
            << MLocale::GregorianCalendar
            << zh_CN_Symbols;
}

void Ut_MCalendar::testMonthSymbols()
{
    QFETCH(QString, language);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(QStringList, symbols);

    MLocale locale(language);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    MCalendar mcal(calendarType);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << language
        << " lc_messages " << lcMessages
        << " lc_time " << lcTime
        << " calendarType " << calendarType << "\n";
    for (int i = 1; i <= 12; ++i) {
        debugStream
            << " i " << i
            << " month name " << locale.monthName(mcal, i)
            << " expected " << symbols.at(i-1)
            << "\n";
    }
    debugStream.flush();
#endif
    for (int i = 1; i <= 12; ++i)
        QCOMPARE(locale.monthName(mcal, i), symbols.at(i-1));
}

void Ut_MCalendar::testDateYearAndMonth_data()
{
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<QDateTime>("qDateTime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<QString>("expectedFormatString");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("en_US")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "en_US"
        << "en_US"
        << "en_US"
        << "en_US"
        << "LLLL yyyy"
        << "December 2011";
    QTest::newRow("cs_CZ")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "cs_CZ"
        << "cs_CZ"
        << "cs_CZ"
        << "cs_CZ"
        << "LLLL yyyy"
        << "Prosinec 2011";
    QTest::newRow("fi_FI")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << "LLLL yyyy"
        << "Joulukuu 2011";
    QTest::newRow("zh_CN")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << "yyyy年 LLLL"
        << "2011年 十二月";
    QTest::newRow("fi_FI fi_FI zh_CN zh_CN")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "zh_CN@mix-time-and-language=yes"
        << "zh_CN"
        << "yyyy LLLL"
        << "2011 Joulukuu";
    QTest::newRow("zh_TW")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "zh_TW"
        << "zh_TW"
        << "zh_TW"
        << "zh_TW"
        << "yyyy年 LLLL"
        << "2011年 十二月";
    QTest::newRow("fi_FI fi_FI zh_TW zh_TW")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "zh_TW@mix-time-and-language=yes"
        << "zh_TW"
        << "yyyy LLLL"
        << "2011 Joulukuu";
    QTest::newRow("ja_JP")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ja_JP"
        << "ja_JP"
        << "ja_JP"
        << "ja_JP"
        << "yyyy年M月"
        << "2011年12月";
    QTest::newRow("fi_FI fi_FI ja_JP ja_JP")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "ja_JP@mix-time-and-language=yes"
        << "ja_JP"
        << "yyyy LLLL"
        << "2011 Joulukuu";
    QTest::newRow("ko_KR")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ko_KR"
        << "ko_KR"
        << "ko_KR"
        << "ko_KR"
        << "yyyy년 M월"
        << "2011년 12월";
    QTest::newRow("hu_HU")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "hu_HU"
        << "hu_HU"
        << "hu_HU"
        << "hu_HU"
        << "yyyy LLLL"
        << "2011 December";
    QTest::newRow("ms_MY")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ms_MY"
        << "ms_MY"
        << "ms_MY"
        << "ms_MY"
        << "yyyy LLLL"
        << "2011 Disember";
}

void Ut_MCalendar::testDateYearAndMonth()
{
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(QDateTime, qDateTime);
    QFETCH(QString, localeName);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, lcNumeric);
    QFETCH(QString, expectedFormatString);
    QFETCH(QString, expectedResult);

    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcTime, lcNumeric);
    locale.setCalendarType(calendarType);
    MCalendar mcal(locale);
    mcal.setDateTime(qDateTime);
    QString formatString = locale.icuFormatString(MLocale::DateYearAndMonth, MLocale::TimeNone, calendarType);
    QString result = locale.formatDateTime(mcal, MLocale::DateYearAndMonth, MLocale::TimeNone);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << localeName
        << " lc_messages " << lcMessages
        << " lc_time " << lcTime
        << " lc_numeric " << lcNumeric
        << " expectedFormatString: " << expectedFormatString
        << " formatString: " << formatString
        << " expectedResult: " << expectedResult
        << " result: " << result
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(formatString, expectedFormatString);
    QCOMPARE(result, expectedResult);
}

void Ut_MCalendar::testDateWeekdayAbbreviatedAndDayOfMonth_data()
{
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<QDateTime>("qDateTime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<QString>("expectedFormatString");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("en_US")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "en_US"
        << "en_US"
        << "en_US"
        << "en_US"
        << "ccc d"
        << "Fri 16";
    QTest::newRow("cs_CZ")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "cs_CZ"
        << "cs_CZ"
        << "cs_CZ"
        << "cs_CZ"
        << "ccc d"
        << "pá 16";
    QTest::newRow("fi_FI")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << "ccc d"
        << "pe 16";
    QTest::newRow("zh_CN")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << "d日ccc"
        << "16日周五";
    QTest::newRow("fi_FI fi_FI zh_CN zh_CN")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "zh_CN@mix-time-and-language=yes"
        << "zh_CN"
        << "d ccc"
        << "16 pe";
    QTest::newRow("zh_TW")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "zh_TW"
        << "zh_TW"
        << "zh_TW"
        << "zh_TW"
        << "d日ccc"
        << "16日週五";
    QTest::newRow("fi_FI fi_FI zh_TW zh_TW")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "zh_TW@mix-time-and-language=yes"
        << "zh_TW"
        << "d ccc"
        << "16 pe";
    QTest::newRow("ja_JP")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ja_JP"
        << "ja_JP"
        << "ja_JP"
        << "ja_JP"
        << "d日(ccc)"
        << "16日(金)";
    QTest::newRow("fi_FI fi_FI ja_JP ja_JP")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "ja_JP@mix-time-and-language=yes"
        << "ja_JP"
        << "d ccc"
        << "16 pe";
    QTest::newRow("ko_KR")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ko_KR"
        << "ko_KR"
        << "ko_KR"
        << "ko_KR"
        << "d일 ccc"
        << "16일 금";
}

void Ut_MCalendar::testDateWeekdayAbbreviatedAndDayOfMonth()
{
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(QDateTime, qDateTime);
    QFETCH(QString, localeName);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, lcNumeric);
    QFETCH(QString, expectedFormatString);
    QFETCH(QString, expectedResult);

    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcTime, lcNumeric);
    locale.setCalendarType(calendarType);
    MCalendar mcal(locale);
    mcal.setDateTime(qDateTime);
    QString formatString = locale.icuFormatString(MLocale::DateWeekdayAbbreviatedAndDayOfMonth, MLocale::TimeNone, calendarType);
    QString result = locale.formatDateTime(mcal, MLocale::DateWeekdayAbbreviatedAndDayOfMonth, MLocale::TimeNone);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << localeName
        << " lc_messages " << lcMessages
        << " lc_time " << lcTime
        << " lc_numeric " << lcNumeric
        << " expectedFormatString: " << expectedFormatString
        << " formatString: " << formatString
        << " expectedResult: " << expectedResult
        << " result: " << result
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(formatString, expectedFormatString);
    QCOMPARE(result, expectedResult);
}

void Ut_MCalendar::testDateWeekdayWideAndDayOfMonth_data()
{
    QTest::addColumn<MLocale::CalendarType>("calendarType");
    QTest::addColumn<QDateTime>("qDateTime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcMessages");
    QTest::addColumn<QString>("lcTime");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<QString>("expectedFormatString");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("en_US")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "en_US"
        << "en_US"
        << "en_US"
        << "en_US"
        << "cccc d"
        << "Friday 16";
    QTest::newRow("cs_CZ")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "cs_CZ"
        << "cs_CZ"
        << "cs_CZ"
        << "cs_CZ"
        << "cccc d"
        << "pátek 16";
    QTest::newRow("fi_FI")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << "fi_FI"
        << "cccc d"
        << "Perjantai 16";
    QTest::newRow("zh_CN")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << "zh_CN"
        << "d日cccc"
        << "16日星期五";
    QTest::newRow("fi_FI fi_FI zh_CN zh_CN")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "zh_CN@mix-time-and-language=yes"
        << "zh_CN"
        << "d cccc"
        << "16 Perjantai";
    QTest::newRow("zh_TW")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "zh_TW"
        << "zh_TW"
        << "zh_TW"
        << "zh_TW"
        << "d日cccc"
        << "16日星期五";
    QTest::newRow("fi_FI fi_FI zh_TW zh_TW")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "zh_TW@mix-time-and-language=yes"
        << "zh_TW"
        << "d cccc"
        << "16 Perjantai";
    QTest::newRow("ja_JP")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ja_JP"
        << "ja_JP"
        << "ja_JP"
        << "ja_JP"
        << "d日(cccc)"
        << "16日(金曜日)";
    QTest::newRow("fi_FI fi_FI ja_JP ja_JP")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "fi_FI"
        << "fi_FI"
        << "ja_JP@mix-time-and-language=yes"
        << "ja_JP"
        << "d cccc"
        << "16 Perjantai";
    QTest::newRow("ko_KR")
        << MLocale::GregorianCalendar
        << QDateTime(QDate(2011,12,16),QTime(18,42,5))
        << "ko_KR"
        << "ko_KR"
        << "ko_KR"
        << "ko_KR"
        << "d일 cccc"
        << "16일 금요일";
}

void Ut_MCalendar::testDateWeekdayWideAndDayOfMonth()
{
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(QDateTime, qDateTime);
    QFETCH(QString, localeName);
    QFETCH(QString, lcMessages);
    QFETCH(QString, lcTime);
    QFETCH(QString, lcNumeric);
    QFETCH(QString, expectedFormatString);
    QFETCH(QString, expectedResult);

    MLocale locale(localeName);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcTime, lcNumeric);
    locale.setCalendarType(calendarType);
    MCalendar mcal(locale);
    mcal.setDateTime(qDateTime);
    QString formatString = locale.icuFormatString(MLocale::DateWeekdayWideAndDayOfMonth, MLocale::TimeNone, calendarType);
    QString result = locale.formatDateTime(mcal, MLocale::DateWeekdayWideAndDayOfMonth, MLocale::TimeNone);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << localeName
        << " lc_messages " << lcMessages
        << " lc_time " << lcTime
        << " lc_numeric " << lcNumeric
        << " expectedFormatString: " << expectedFormatString
        << " formatString: " << formatString
        << " expectedResult: " << expectedResult
        << " result: " << result
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(formatString, expectedFormatString);
    QCOMPARE(result, expectedResult);
}

void Ut_MCalendar::testWeekdayType_data()
{
   QTest::addColumn<QString>("language");
   QTest::addColumn<QString>("lcTime");
   QTest::addColumn<MLocale::CalendarType>("calendarType");
   QTest::addColumn<int>("firstDayOfWeek");
   QTest::addColumn<int>("mondayType");
   QTest::addColumn<int>("tuesdayType");
   QTest::addColumn<int>("wednesdayType");
   QTest::addColumn<int>("thursdayType");
   QTest::addColumn<int>("fridayType");
   QTest::addColumn<int>("saturdayType");
   QTest::addColumn<int>("sundayType");
   QTest::addColumn<int>("mondayTransition");
   QTest::addColumn<int>("tuesdayTransition");
   QTest::addColumn<int>("wednesdayTransition");
   QTest::addColumn<int>("thursdayTransition");
   QTest::addColumn<int>("fridayTransition");
   QTest::addColumn<int>("saturdayTransition");
   QTest::addColumn<int>("sundayTransition");

   QTest::newRow("de_DE")
       << "de_DE"
       << "de_DE"
       << MLocale::DefaultCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("de_DE")
       << "de_DE"
       << "de_DE"
       << MLocale::GregorianCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("de_DE")
       << "de_DE"
       << "de_DE"
       << MLocale::IslamicCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ar")
       << "ar"
       << "ar"
       << MLocale::DefaultCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ar")
       << "ar"
       << "ar"
       << MLocale::GregorianCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ar")
       << "ar"
       << "ar"
       << MLocale::IslamicCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ar ar_EG")
       << "ar"
       << "ar_EG"
       << MLocale::IslamicCalendar
       << 6
       <<  0 <<  0 <<  0 <<  0 <<  1 <<        3 <<  0
       << -1 << -1 << -1 << -1 <<  0 << 86400000 << -1;
   QTest::newRow("de_DE ar_EG")
       << "de_DE"
       << "ar_EG"
       << MLocale::IslamicCalendar
       << 6
       <<  0 <<  0 <<  0 <<  0 <<  1 <<        3 <<  0
       << -1 << -1 << -1 << -1 <<  0 << 86400000 << -1;
   QTest::newRow("ar_EG de_DE")
       << "ar_EG"
       << "de_DE"
       << MLocale::IslamicCalendar
       << 1
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 << 3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ar_EG")
       << "ar_EG"
       << "ar_EG"
       << MLocale::DefaultCalendar
       << 6
       <<  0 <<  0 <<  0 <<  0 <<  1 <<        3 <<  0
       << -1 << -1 << -1 << -1 <<  0 << 86400000 << -1;
   QTest::newRow("ar_EG")
       << "ar_EG"
       << "ar_EG"
       << MLocale::GregorianCalendar
       << 6
       <<  0 <<  0 <<  0 <<  0 <<  1 <<        3 <<  0
       << -1 << -1 << -1 << -1 <<  0 << 86400000 << -1;
   QTest::newRow("ar_EG")
       << "ar_EG"
       << "ar_EG"
       << MLocale::IslamicCalendar
       << 6
       <<  0 <<  0 <<  0 <<  0 <<  1 <<        3 <<  0
       << -1 << -1 << -1 << -1 <<  0 << 86400000 << -1;
   QTest::newRow("ar_SA")
       << "ar_SA"
       << "ar_SA"
       << MLocale::DefaultCalendar
       << 6
       <<  0 <<  0 <<  0 <<  1 <<        3 <<  0 <<  0
       << -1 << -1 << -1 <<  0 << 86400000 << -1 << -1;
   QTest::newRow("ar_SA")
       << "ar_SA"
       << "ar_SA"
       << MLocale::GregorianCalendar
       << 6
       <<  0 <<  0 <<  0 <<  1 <<        3 <<  0 <<  0
       << -1 << -1 << -1 <<  0 << 86400000 << -1 << -1;
   QTest::newRow("ar_SA")
       << "ar_SA"
       << "ar_SA"
       << MLocale::IslamicCalendar
       << 6
       <<  0 <<  0 <<  0 <<  1 <<        3 <<  0 <<  0
       << -1 << -1 << -1 <<  0 << 86400000 << -1 << -1;
   QTest::newRow("ar_AE")
       << "ar_AE"
       << "ar_AE"
       << MLocale::GregorianCalendar
       << 6
       <<  0 <<  0 <<  0 <<   0 << 1 <<        3 <<  0
       << -1 << -1 << -1 <<  -1 << 0 << 86400000 << -1;
   QTest::newRow("ar_AE")
       << "ar_AE"
       << "ar_AE"
       << MLocale::IslamicCalendar
       << 6
       <<  0 <<  0 <<  0 <<   0 << 1 <<        3 <<  0
       << -1 << -1 << -1 <<  -1 << 0 << 86400000 << -1;
   QTest::newRow("ar_SY")
       << "ar_SY"
       << "ar_SY"
       << MLocale::GregorianCalendar
       << 6
       <<  0 <<  0 <<  0 <<   0 << 1 <<        3 <<  0
       << -1 << -1 << -1 <<  -1 << 0 << 86400000 << -1;
   QTest::newRow("ar_SY")
       << "ar_SY"
       << "ar_SY"
       << MLocale::IslamicCalendar
       << 6
       <<  0 <<  0 <<  0 <<   0 << 1 <<        3 <<  0
       << -1 << -1 << -1 <<  -1 << 0 << 86400000 << -1;
   QTest::newRow("he_IL")
       << "he_IL"
       << "he_IL"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  1 <<        3 <<  0
       << -1 << -1 << -1 << -1 <<  0 << 86400000 << -1;
   QTest::newRow("en_US")
       << "en_US"
       << "en_US"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("as_IN")
       << "as_IN"
       << "as_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("bn_IN")
       << "bn_IN"
       << "bn_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("bo_IN")
       << "bo_IN"
       << "bo_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("en_IN")
       << "en_IN"
       << "en_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("gu_IN")
       << "gu_IN"
       << "gu_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("hi_IN")
       << "hi_IN"
       << "bn_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("kn_IN")
       << "kn_IN"
       << "kn_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("kok_IN")
       << "kok_IN"
       << "kok_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ml_IN")
       << "ml_IN"
       << "ml_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("mr_IN")
       << "mr_IN"
       << "mr_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ne_IN")
       << "ne_IN"
       << "ne_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("or_IN")
       << "or_IN"
       << "or_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("pa_Guru_IN")
       << "pa_Guru_IN"
       << "pa_Guru_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("pa_IN")
       << "pa_IN"
       << "pa_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("ta_IN")
       << "ta_IN"
       << "ta_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
   QTest::newRow("te_IN")
       << "te_IN"
       << "te_IN"
       << MLocale::GregorianCalendar
       << 7
       <<  0 <<  0 <<  0 <<  0 <<  0 << 1 <<        3
       << -1 << -1 << -1 << -1 << -1 << 0 << 86400000;
}

void Ut_MCalendar::testWeekdayType()
{
    QFETCH(QString, language);
    QFETCH(QString, lcTime);
    QFETCH(MLocale::CalendarType, calendarType);
    QFETCH(int, firstDayOfWeek);
    QFETCH(int, mondayType);
    QFETCH(int, tuesdayType);
    QFETCH(int, wednesdayType);
    QFETCH(int, thursdayType);
    QFETCH(int, fridayType);
    QFETCH(int, saturdayType);
    QFETCH(int, sundayType);
    QFETCH(int, mondayTransition);
    QFETCH(int, tuesdayTransition);
    QFETCH(int, wednesdayTransition);
    QFETCH(int, thursdayTransition);
    QFETCH(int, fridayTransition);
    QFETCH(int, saturdayTransition);
    QFETCH(int, sundayTransition);

    MLocale locale(language);
    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCalendarType(calendarType);
    MCalendar calendar(locale);
    QVERIFY(calendar.isValid());
    QCOMPARE(calendar.type(), calendarType);
    calendar.setDate(2011, 1, 31);
    calendar.setTime(19, 23, 0);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "language " << language << " lc_time " << lcTime
        << " calendar type " << calendar.type()
        << "\n   firstDayOfWeek " << calendar.firstDayOfWeek()
        << "\tmon "
        << calendar.getDayOfWeekType(MLocale::Monday)
        << " "
        << calendar.getWeekendTransition(MLocale::Monday)
        << "\ttue "
        << calendar.getDayOfWeekType(MLocale::Tuesday)
        << " "
        << calendar.getWeekendTransition(MLocale::Tuesday)
        << "\twed "
        << calendar.getDayOfWeekType(MLocale::Wednesday)
        << " "
        << calendar.getWeekendTransition(MLocale::Wednesday)
        << "\tthu "
        << calendar.getDayOfWeekType(MLocale::Thursday)
        << " "
        << calendar.getWeekendTransition(MLocale::Thursday)
        << "\tfri "
        << calendar.getDayOfWeekType(MLocale::Friday)
        << " "
        << calendar.getWeekendTransition(MLocale::Friday)
        << "\tsat "
        << calendar.getDayOfWeekType(MLocale::Saturday)
        << " "
        << calendar.getWeekendTransition(MLocale::Saturday)
        << "\tsun "
        << calendar.getDayOfWeekType(MLocale::Sunday)
        << " "
        << calendar.getWeekendTransition(MLocale::Sunday)
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(firstDayOfWeek, (int) calendar.firstDayOfWeek());
    QCOMPARE(mondayType, (int) calendar.getDayOfWeekType(MLocale::Monday));
    QCOMPARE(tuesdayType, (int) calendar.getDayOfWeekType(MLocale::Tuesday));
    QCOMPARE(wednesdayType, (int) calendar.getDayOfWeekType(MLocale::Wednesday));
    QCOMPARE(thursdayType, (int) calendar.getDayOfWeekType(MLocale::Thursday));
    QCOMPARE(fridayType, (int) calendar.getDayOfWeekType(MLocale::Friday));
    QCOMPARE(saturdayType, (int) calendar.getDayOfWeekType(MLocale::Saturday));
    QCOMPARE(sundayType, (int) calendar.getDayOfWeekType(MLocale::Sunday));
    QCOMPARE(mondayTransition, calendar.getWeekendTransition(MLocale::Monday));
    QCOMPARE(tuesdayTransition, calendar.getWeekendTransition(MLocale::Tuesday));
    QCOMPARE(wednesdayTransition, calendar.getWeekendTransition(MLocale::Wednesday));
    QCOMPARE(thursdayTransition, calendar.getWeekendTransition(MLocale::Thursday));
    QCOMPARE(fridayTransition, calendar.getWeekendTransition(MLocale::Friday));
    QCOMPARE(saturdayTransition, calendar.getWeekendTransition(MLocale::Saturday));
    QCOMPARE(sundayTransition, calendar.getWeekendTransition(MLocale::Sunday));
}
QTEST_APPLESS_MAIN(Ut_MCalendar);

