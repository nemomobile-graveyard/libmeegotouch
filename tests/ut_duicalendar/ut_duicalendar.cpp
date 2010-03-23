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

#include <QCoreApplication>
#include <QTextCodec>
#include <DuiLocale>
#include <unicode/uversion.h>

#include "ut_duicalendar.h"

void Ut_DuiCalendar::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QProcess process;
    process.start("sh -c \"dpkg -s libicu42 | grep Version | perl -pe 's/^Version:[[:space:]]*([^[[:space:]]+)$/$1/g'\"");
    if (!process.waitForFinished()) {
        qDebug() << "cannot run process to check libicu42 package version , exiting ...";
        exit(1);
    }
    icuPackageVersion = process.readAllStandardOutput();
    icuPackageVersion.replace("\n", "");
    qDebug() << "libicu42 package version is:" << icuPackageVersion;
}

void Ut_DuiCalendar::cleanupTestCase()
{
    delete qap;
}

void Ut_DuiCalendar::init()
{
    // most tests use the Finnish timezone:
    DuiCalendar::setSystemTimeZone("Europe/Helsinki");
}

void Ut_DuiCalendar::cleanup()
{
}

void Ut_DuiCalendar::testConversionFromAndToQDateTime_data()
{
    QTest::addColumn<QDate>("qDate");
    QTest::addColumn<QTime>("qTime");
    QTest::addColumn<int>("qTimeSpec");
    QTest::addColumn<QString>("timeZone");

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

void Ut_DuiCalendar::testConversionFromAndToQDateTime()
{
    QFETCH(QDate, qDate);
    QFETCH(QTime, qTime);
    QFETCH(int, qTimeSpec);
    QFETCH(QString, timeZone);

    DuiCalendar::setSystemTimeZone(timeZone);

    DuiCalendar duical;
    QDateTime datetime(qDate, qTime, (Qt::TimeSpec) qTimeSpec);
    duical.setDateTime(datetime);
    QCOMPARE(datetime, duical.qDateTime((Qt::TimeSpec) qTimeSpec));
}

void Ut_DuiCalendar::testDuiLocaleCalendar_data()
{
    QTest::addColumn<DuiLocale::Calendar>("cal");
    QTest::newRow("default") << DuiLocale::DefaultCalendar;
    QTest::newRow("gregorian") << DuiLocale::GregorianCalendar;
    QTest::newRow("islamic") << DuiLocale::IslamicCalendar;
    QTest::newRow("chinese") << DuiLocale::ChineseCalendar;
    QTest::newRow("islamiccivil") << DuiLocale::IslamicCivilCalendar;
    QTest::newRow("hebrew") << DuiLocale::HebrewCalendar;
    QTest::newRow("japanese") << DuiLocale::JapaneseCalendar;
    QTest::newRow("buddhist") << DuiLocale::BuddhistCalendar;
    QTest::newRow("persian") << DuiLocale::PersianCalendar;
    QTest::newRow("coptic") << DuiLocale::CopticCalendar;
    QTest::newRow("ethiopian") << DuiLocale::EthiopicCalendar;
}

void Ut_DuiCalendar::testDuiLocaleCalendar()
{
    DuiLocale *z = 0;
    // We can't test with default locale because the s_systemDefault
    // is persistent and whatever we set to default locale will be
    // kept there.
    z = new DuiLocale("fi");
    QVERIFY2(z->isValid(), "new DuiLocale() did not create a valid locale");

    QFETCH(DuiLocale::Calendar, cal);

    QVERIFY2(z->calendar() == DuiLocale::DefaultCalendar,
             "Constructor didn't set the default calendar");
    z->setCalendar(cal);
    QVERIFY2(z->calendar() == cal, "Calendar was not set properly");
    delete z;
}

void Ut_DuiCalendar::testDuiLocaleCalendarConversionsFromLocaltimeQDateTime_data()
{
    QTest::addColumn<QDateTime>("datetime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<DuiLocale::Calendar>("cal");
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
            << DuiLocale::GregorianCalendar
            << QString("21.7.2008")
            << QString("21.7.2008")
            << QString("21. heinäkuuta 2008")
            << QString("maanantaina 21. heinäkuuta 2008")
            << QString("12.31")
            << QString("12.31.00")
            << QString("12.31.00 UTC+3.00")
            << QString("12.31.00 Itä-Euroopan kesäaika");
    QTest::newRow("21.7.2008_en_GB_Gregorian")
            << datetime
            << QString("en_GB")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. jul. 2008")
            << QString("21. juli 2008")
            << QString("måndag 21. juli 2008")
            << QString("12.31")
            << QString("12.31.00")
            << QString("12.31.00 GMT+03.00")
            << QString("kl. 12.31.00 austeuropeisk sommartid");
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
            << datetime
            << QString("nb_NO")
            << DuiLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. juli 2008")
            << QString("21. juli 2008")
            << QString("mandag 21. juli 2008")
            << QString("12.31")
            << QString("12.31.00")
            << QString("12.31.00 GMT+03.00")
            << QString("kl. 12.31.00 østeuropeisk sommertid");
    QTest::newRow("21.7.2008_no_NO_Gregorian")
            << datetime
            << QString("no_NO")
            << DuiLocale::GregorianCalendar
            << QString("21.07.08")
            << QString("21. juli 2008")
            << QString("21. juli 2008")
            << QString("mandag 21. juli 2008")
            << QString("12.31")
            << QString("12.31.00")
            << QString("12.31.00 GMT+03.00")
            << QString("kl. 12.31.00 østeuropeisk sommertid");
}

void Ut_DuiCalendar::testDuiLocaleCalendarConversionsFromLocaltimeQDateTime()
{
    QFETCH(QDateTime, datetime);
    QFETCH(QString, localeName);
    QFETCH(DuiLocale::Calendar, cal);
    QFETCH(QString, dateShortResult);
    QFETCH(QString, dateMediumResult);
    QFETCH(QString, dateLongResult);
    QFETCH(QString, dateFullResult);
    QFETCH(QString, timeShortResult);
    QFETCH(QString, timeMediumResult);
    QFETCH(QString, timeLongResult);
    QFETCH(QString, timeFullResult);

    DuiLocale locale(localeName);

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

    for (unsigned dateType = DuiLocale::DateNone; dateType <= DuiLocale::DateFull;
            ++dateType) {
        for (unsigned timeType = DuiLocale::TimeNone; timeType <= DuiLocale::TimeFull;
                ++timeType) {
            QString result;
            if (dateType == DuiLocale::DateNone)
                result = timeResults[timeType];
            else if (timeType == DuiLocale::TimeNone)
                result = dateResults[dateType];
            else
                result = dateResults[dateType] + ' ' + timeResults[timeType];
            qDebug() << "dateType" << dateType << "timeType" << timeType
                     << "result" << result;
            QCOMPARE(
                locale.formatDateTime(datetime,
                                      static_cast<DuiLocale::DateType>(dateType),
                                      static_cast<DuiLocale::TimeType>(timeType),
                                      cal),
                result);
        }
    }
}

void Ut_DuiCalendar::testDuiLocaleCalendarConversionsFromUTCQDateTime_data()
{
    QTest::addColumn<QDateTime>("datetime");
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<DuiLocale::Calendar>("cal");
    QTest::addColumn<QString>("short_result");
    QTest::addColumn<QString>("long_result");
    QTest::addColumn<QString>("full_result");

    QDate date(2008, 7, 21);
    QTime time(12, 31, 0, 0);
    QDateTime datetime(date, time, Qt::UTC); // finland is utc+3 so maps to 15:31

    QTest::newRow("21.7.2008_fi_FI_Gregorian")
            << datetime
            << QString("fi_FI")
            << DuiLocale::GregorianCalendar
            << QString("21.7.2008 15.31")
            << QString("21. heinäkuuta 2008 15.31.00 UTC+3.00")
            << QString("maanantaina 21. heinäkuuta 2008 15.31.00 Itä-Euroopan kesäaika");
    QTest::newRow("21.7.2008_en_GB_Gregorian")
            << datetime
            << QString("en_GB")
            << DuiLocale::GregorianCalendar
            << QString("21/07/2008 15:31")
            << QString("21 July 2008 15:31:00 EEST")
            << QString("Monday, 21 July 2008 15:31:00 Eastern European Summer Time");
    QTest::newRow("21.7.2008_nn_NO_Gregorian")
            << datetime
            << QString("nn_NO")
            << DuiLocale::GregorianCalendar
            << QString("21.07.08 15.31")
            << QString("21. juli 2008 15.31.00 GMT+03.00")
            << QString("måndag 21. juli 2008 kl. 15.31.00 austeuropeisk sommartid");
    QTest::newRow("21.7.2008_nb_NO_Gregorian")
            << datetime
            << QString("nb_NO")
            << DuiLocale::GregorianCalendar
            << QString("21.07.08 15.31")
            << QString("21. juli 2008 15.31.00 GMT+03.00")
            << QString("mandag 21. juli 2008 kl. 15.31.00 østeuropeisk sommertid");
}

void Ut_DuiCalendar::testDuiLocaleCalendarConversionsFromUTCQDateTime()
{
    QFETCH(QDateTime, datetime);
    QFETCH(QString, localeName);
    QFETCH(DuiLocale::Calendar, cal);
    QFETCH(QString, short_result);
    QFETCH(QString, long_result);
    QFETCH(QString, full_result);

    DuiLocale locale(localeName);

    QCOMPARE(locale.formatDateTime(datetime, DuiLocale::DateShort,
                                   DuiLocale::TimeShort, cal),
             short_result);
    QCOMPARE(locale.formatDateTime(datetime, DuiLocale::DateLong,
                                   DuiLocale::TimeLong, cal),
             long_result);
    QCOMPARE(locale.formatDateTime(datetime, DuiLocale::DateFull,
                                   DuiLocale::TimeFull, cal),
             full_result);
}

void Ut_DuiCalendar::testDuiLocaleCalendarConversionsFromDuiCalendar_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<DuiLocale::Calendar>("cal");
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

    QTest::newRow("21.7.2008_fi_FI_Gregorian")
            << QString("fi_FI")
            << DuiLocale::GregorianCalendar
            << 2008
            << 7
            << 21
            << "21.7.2008"
            << "21.7.2008"
            << "21. heinäkuuta 2008"
            << "maanantaina 21. heinäkuuta 2008"
            << "14.31"
            << "14.31.00"
            << "14.31.00 UTC+3.00"
            << "14.31.00 Itä-Euroopan kesäaika";

    QTest::newRow("21.7.2008_en_GB_Gregorian")
            << QString("en_GB")
            << DuiLocale::GregorianCalendar
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
            << QString("nn_NO")
            << DuiLocale::GregorianCalendar
            << 2008
            << 7
            << 21
            << "21.07.08"
            << "21. jul. 2008"
            << "21. juli 2008"
            << "måndag 21. juli 2008"
            << "14.31"
            << "14.31.00"
            << "14.31.00 GMT+03.00"
            << "kl. 14.31.00 austeuropeisk sommartid";

    QTest::newRow("21.7.2008_nb_NO_Gregorian")
            << QString("nb_NO")
            << DuiLocale::GregorianCalendar
            << 2008
            << 7
            << 21
            << "21.07.08"
            << "21. juli 2008"
            << "21. juli 2008"
            << "mandag 21. juli 2008"
            << "14.31"
            << "14.31.00"
            << "14.31.00 GMT+03.00"
            << "kl. 14.31.00 østeuropeisk sommertid";

    QTest::newRow("21.7.2008_no_NO_Gregorian")
            << QString("no_NO")
            << DuiLocale::GregorianCalendar
            << 2008
            << 7
            << 21
            << "21.07.08"
            << "21. juli 2008"
            << "21. juli 2008"
            << "mandag 21. juli 2008"
            << "14.31"
            << "14.31.00"
            << "14.31.00 GMT+03.00"
            << "kl. 14.31.00 østeuropeisk sommertid";
}

void Ut_DuiCalendar::testDuiLocaleCalendarConversionsFromDuiCalendar()
{
    QFETCH(QString, localeName);
    QFETCH(DuiLocale::Calendar, cal);
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

    DuiLocale locale(localeName);
    DuiCalendar duical(cal);
    duical.setDate(year, month, day);
    duical.setTime(14, 31, 0);

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

    for (unsigned dateType = DuiLocale::DateNone; dateType <= DuiLocale::DateFull;
            ++dateType) {
        for (unsigned timeType = DuiLocale::TimeNone; timeType <= DuiLocale::TimeFull;
                ++timeType) {
            QString result;
            if (dateType == DuiLocale::DateNone)
                result = timeResults[timeType];
            else if (timeType == DuiLocale::TimeNone)
                result = dateResults[dateType];
            else
                result = dateResults[dateType] + ' ' + timeResults[timeType];
            qDebug() << "dateType" << dateType << "timeType" << timeType
                     << "result" << result;
            QCOMPARE(
                locale.formatDateTime(duical,
                                      static_cast<DuiLocale::DateType>(dateType),
                                      static_cast<DuiLocale::TimeType>(timeType)),
                result);
        }
    }
}

void Ut_DuiCalendar::testDuiCalendarAdditions()
{
    DuiLocale fi_FI("fi_FI");
    DuiCalendar cal(fi_FI);
    cal.setDate(2008, 1, 31);
    cal.setTime(19, 23, 0);
    cal.addMonths(1);

    QVERIFY2(cal.year() == 2008, "year is incorrect");
    QVERIFY2(cal.month() == 2, "month is incorrect");
    QVERIFY2(cal.dayOfMonth() == 29, "day of month is incorrect");
    QVERIFY2(cal.dayOfYear() == 60, "day of year is incorrect");
    QVERIFY2(cal.firstDayOfMonth() == 1, "first day of month is incorrect");
    QVERIFY2(cal.lastDayOfMonth() == 29, "last day of month is incorrect");
    QVERIFY2(cal.firstDayOfWeek() == 1, "first day of week is incorrect");
    QVERIFY2(cal.weekNumber() == 9, "week number is incorrect");
    QVERIFY2(cal.maximumWeeksInMonth() == 6, "maximum weeks in month is incorrect");
    QVERIFY2(cal.daysInWeek() == 7, "days is week is incorrect");

    // add some hours so calendar changes to 1.3.2008
    cal.addHours(5);
    QVERIFY2(cal.hour() == 0, "hour failed");
    QVERIFY2(cal.minute() == 23, "minute failed");
    QVERIFY2(cal.second() == 0, "second failed");
    QVERIFY2(cal.month() == 3, "month failed");
    QVERIFY2(cal.dayOfMonth() == 1, "day of month failed");
}

void Ut_DuiCalendar::testWeekNumbers()
{
    // 31.12.2007 belongs to week 1 of year 2008, check it really is so
    DuiLocale fi_FI("fi_FI");
    DuiCalendar cal(fi_FI);
    cal.setDate(2007, 12, 31);
    QVERIFY2(cal.yearOfWeek() == 2008, "year of week is incorrect");

    cal.setDate(2005, 1, 1);
    QVERIFY2(cal.weekOfYear() == 53, "year of week 1.1.2005 is incorrect2");

    cal.setMinimalDaysInFirstWeek(1);
    QVERIFY2(cal.minimalDaysInFirstWeek() == 1, "setMinimalDaysInFirstWeek() failed");
    QVERIFY2(cal.weekOfYear() == 1, "year of week 1.1.2005 with overridden week data failed");

}

void Ut_DuiCalendar::testComparisons()
{
    DuiCalendar cal1;
    DuiCalendar cal2;
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

void Ut_DuiCalendar::testIslamicCalendar()
{
    DuiLocale loc("fi_FI");
    loc.setCalendar(DuiLocale::IslamicCalendar);
    DuiCalendar cal(loc);
    QDate date(2008, 1, 1); // use qdate to set date as gregorian
    cal.setDate(date);
    QString format = loc.formatDateTime(cal, DuiLocale::DateShort, DuiLocale::TimeNone);
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

void Ut_DuiCalendar::testPosixFormatPattern_data()
{
    QTest::addColumn<DuiLocale::Calendar>("calendarType");
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%c" // locale's date and time (e.g., Thu Mar  3 23:05:25 2005)
            << "3.2.2008 12.25.03";
    QTest::newRow("fi_FI %d")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%r" // locale's 12-hour clock time (e.g., 11:11:04 PM)
            << "00 ip.";
    QTest::newRow("fi_FI %t")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%H" // hour (00..23)
            << "12";
    QTest::newRow("fi_FI %I")
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%I" // hour (01..12)
            << "00";
    QTest::newRow("fi_FI %M")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%R" // 24-hour hour and minute; same as %H:%M
            << "12:25";
    QTest::newRow("fi_FI %S")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%U" // week number of year, with Sunday as first day of week (00..53)
            << "5";
    QTest::newRow("fi_FI %V")
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%V" // ISO week number, with Monday as first day of week (01..53)"
            << "5";
    QTest::newRow("fi_FI %W")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%X" // locale's time representation (e.g., 23:13:48)
            << "12.25.03";
    QTest::newRow("fi_FI %Y")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%Z" // Time-zone name, or no characters if no time zone is determinable
            << "Suomi";

    // ************************************************************
    QTest::newRow("en_GB %a")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%r" // locale's 12-hour clock time (e.g., 11:11:04 PM)
            << "00 PM";
    QTest::newRow("en_GB %t")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%H" // hour (00..23)
            << "12";
    QTest::newRow("en_GB %I")
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%I" // hour (01..12)
            << "00";
    QTest::newRow("en_GB %M")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%R" // 24-hour hour and minute; same as %H:%M
            << "12:25";
    QTest::newRow("en_GB %S")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%U" // week number of year, with Sunday as first day of week (00..53)
            << "5";
    QTest::newRow("en_GB %V")
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "en_GB"
            << "%V" // ISO week number, with Monday as first day of week (01..53)"
            << "5";
    QTest::newRow("en_GB %W")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE"
            << "%Z" // Time-zone name, or no characters if no time zone is determinable
            << "Finnland";
    // ************************************************************
    QTest::newRow("fi_FI %R %Z")
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "fi_FI"
            << "%R %Z"
            << "12:25 Suomi";
    QTest::newRow("en_GB %R %Z")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << 2008
            << 2
            << 3
            << 12
            << 25
            << 3
            << "de_DE"
            << "%R %Z"
            << "12:25 Finnland";
}

void Ut_DuiCalendar::testPosixFormatPattern()
{
    QFETCH(DuiLocale::Calendar, calendarType);
    QFETCH(int, year);
    QFETCH(int, month);
    QFETCH(int, day);
    QFETCH(int, hour);
    QFETCH(int, minute);
    QFETCH(int, second);
    QFETCH(QString, localeName);
    QFETCH(QString, format);
    QFETCH(QString, result);

    DuiLocale locale(localeName);
    locale.setCalendar(calendarType);
    DuiCalendar duical(locale);
    duical.setDate(year, month, day);
    duical.setTime(hour, minute, second);
    QDate date(year, month, day);
    QTime time(hour, minute, second);
    QDateTime datetime(date, time, Qt::LocalTime);
    QLocale qlocale(localeName);

    qDebug() << "format:" << format << "result: " << result;
//    qDebug() << "QLocale time ShortFormat" << qlocale.toString(time, QLocale::ShortFormat) << "QLocale time LongFormat" << qlocale.toString(time, QLocale::LongFormat);
//    qDebug() << "QLocale format" << qlocale.toString(time, "hh:mm");
//    qDebug() << "QLocale date ShortFormat" << qlocale.toString(date, QLocale::ShortFormat) << "QLocale date LongFormat" << qlocale.toString(date, QLocale::LongFormat);
    qDebug() << "DateNone TimeNone" << locale.formatDateTime(datetime, DuiLocale::DateNone, DuiLocale::TimeNone, calendarType);
    qDebug() << "DateNone TimeShort" << locale.formatDateTime(datetime, DuiLocale::DateNone, DuiLocale::TimeShort, calendarType);
    qDebug() << "DateShort TimeNone" << locale.formatDateTime(datetime, DuiLocale::DateShort, DuiLocale::TimeNone, calendarType);
//    qDebug() << "DateShort TimeShort" << locale.formatDateTime(datetime, DuiLocale::DateShort, DuiLocale::TimeShort, calendarType);
//    qDebug() << "DateMedium TimeMedium" << locale.formatDateTime(datetime, DuiLocale::DateMedium, DuiLocale::TimeMedium, calendarType);
//    qDebug() << "DateLong TimeLong" << locale.formatDateTime(datetime, DuiLocale::DateLong, DuiLocale::TimeLong, calendarType);
//    qDebug() << "DateFull TimeFull" << locale.formatDateTime(datetime, DuiLocale::DateFull, DuiLocale::TimeFull, calendarType);
    if ((format == "%b" || format == "%h") && result == "helmi") {
        QEXPECT_FAIL("", "FIXME: ICU does %b and %h wrong for Finnish", Continue);
        QVERIFY(false);
    } else {
        QCOMPARE(locale.formatDateTime(duical, format), result);
        QCOMPARE(locale.formatDateTime(datetime, format), result);
    }
}

void Ut_DuiCalendar::testWeekdaySymbols_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<DuiLocale::Calendar>("cal");
    QTest::addColumn<QStringList>("symbols");

    // FIXME: better translations would be without -na suffix
    QStringList fi_FI_Symbols;
    fi_FI_Symbols
            << QString("maanantaina")
            << QString("tiistaina")
            << QString("keskiviikkona")
            << QString("torstaina")
            << QString("perjantaina")
            << QString("lauantaina")
            << QString("sunnuntaina");

    QTest::newRow("weekday_symbols_fi_FI_Gregorian")
            << QString("fi_FI")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << nb_NO_Symbols;

    QStringList no_NO_Symbols;
    no_NO_Symbols = nb_NO_Symbols;

    QTest::newRow("weekday_symbols_no_NO_Gregorian")
            << QString("no_NO")
            << DuiLocale::GregorianCalendar
            << no_NO_Symbols;
}

void Ut_DuiCalendar::testWeekdaySymbols()
{
    QFETCH(QString, localeName);
    QFETCH(DuiLocale::Calendar, cal);
    QFETCH(QStringList, symbols);

    DuiLocale locale(localeName);
    DuiCalendar duical(cal);
    for (int i = 1; i <= 7; ++i) {
        QString symbol = symbols.at(i - 1);
        QCOMPARE(locale.weekdayName(duical, i), symbol);
    }
}

void Ut_DuiCalendar::testMonthSymbols_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<DuiLocale::Calendar>("cal");
    QTest::addColumn<QStringList>("symbols");

    // FIXME: again better translations without -ta
    QStringList fi_FI_Symbols;
    fi_FI_Symbols
            << QString("tammikuuta")
            << QString("helmikuuta")
            << QString("maaliskuuta")
            << QString("huhtikuuta")
            << QString("toukokuuta")
            << QString("kesäkuuta")
            << QString("heinäkuuta")
            << QString("elokuuta")
            << QString("syyskuuta")
            << QString("lokakuuta")
            << QString("marraskuuta")
            << QString("joulukuuta");

    QTest::newRow("month_symbols_fi_FI_Gregorian")
            << QString("fi_FI")
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
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
            << DuiLocale::GregorianCalendar
            << nb_NO_Symbols;

    QStringList no_NO_Symbols;
    no_NO_Symbols = nb_NO_Symbols;

    QTest::newRow("month_symbols_no_NO_Gregorian")
            << QString("no_NO")
            << DuiLocale::GregorianCalendar
            << no_NO_Symbols;
}

void Ut_DuiCalendar::testMonthSymbols()
{
    QFETCH(QString, localeName);
    QFETCH(DuiLocale::Calendar, cal);

    DuiLocale locale(localeName);
    DuiCalendar duical(cal);
    QFETCH(QStringList, symbols);

    for (int i = 1; i <= 12; ++i) {
        QString symbol = symbols.at(i - 1);
        QCOMPARE(locale.monthName(duical, i), symbol);
    }
}

QTEST_APPLESS_MAIN(Ut_DuiCalendar);

