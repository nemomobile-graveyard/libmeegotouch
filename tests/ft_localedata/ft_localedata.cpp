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

#include "ft_localedata.h"

void Ft_LocaleData::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    app = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    MLocale::setDataPath(qApp->applicationDirPath());
}

void Ft_LocaleData::cleanup()
{
}

/*
 * Test whether standard number formatting of ICU can be overridden
 * for a certain language by loading a special resource file.
 *
 * The path to the resource file loaded here is of the form
 *
 *    <prefix>/icudt<version><flag>/<basename>.res
 *
 * <prefix> is set above in initTestCase() by MLocale::setDataPath("."),
 * i.e. this test application looks for the file starting from the current
 * directory.
 *
 * <version> indicates the icu version, for libicu42 this is 42
 *
 * <flag> is 'l' for Little Endian, ASCII, 'b' for Big Endian, ASCII,
 * 'e' for Big Endian, EBCDIC.
 *
 * <basename> is the name of the locale this resource file is intended for.
 * For the test below we use "fi_FI" as the locale name.
 *
 * Therefore, the full path name of the file loaded by the test below is
 * ./icudt42l/fi_FI.res  (for libicu42).
 *
 * fi_FI.res is generated from a the plain text file fi_FI.txt using
 * the "genrb" tool (See the rules in the ft_localedata.pro file).
 *
 * For more details see  http://userguide.icu-project.org/icudata
 */

void Ft_LocaleData::testNumberData_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<double>("value");
    QTest::addColumn<QString>("result");

    // for en_US_POSIX locale, we don’t override anything, i.e.
    // the expected result below is what ICU does by default:
    QTest::newRow("en_US_POSIX")
            << QString("en_US_POSIX")
            << (double) 1234567.123456789
            <<  QString("1234567.123457");
    // For fi_FI locale, we override the formatting of a double number using the
    // the file ./icudt42l/fi_FI.res as explained above.
    // Therefore we expect a weird result below, the point here
    // is only to check whether we can override the default if this should
    // be necessary:
    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << (double) 1234567.123456789
            <<  QString("1 234 567--123");
}

void Ft_LocaleData::testNumberData()
{
    QFETCH(QString, locale_name);
    QFETCH(double, value);
    QFETCH(QString, result);

    MLocale locale(locale_name);
    QVERIFY(locale.isValid());
    QCOMPARE(locale.formatNumber(value), result);
}

QTEST_APPLESS_MAIN(Ft_LocaleData);
