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

#include "ft_numbers.h"

void Ft_Numbers::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Ft_Numbers::cleanup()
{
}

void Ft_Numbers::testQLongLongs_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("localeNameLcNumeric");
    QTest::addColumn<qlonglong>("val");
    QTest::addColumn<QString>("formatted");

    QTest::newRow("en_US")
            << QString("en_US")
            << QString("en_US")
            << (qlonglong) 1542678073
            << QString("1,542,678,073");
    QTest::newRow("en_US")
            << QString("en_US")
            << QString("de_DE")
            << (qlonglong) 1542678073
            << QString("1.542.678.073");
    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("fi_FI")
            << (qlonglong) 1542678073
            << QString("1 542 678 073");
    QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("en_GB")
            << (qlonglong) 1542678073
            << QString("1,542,678,073");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("de_DE")
            << (qlonglong) 1542678073
            << QString("1.542.678.073");
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("de_CH")
            << (qlonglong) 1542678073
            << QString("1'542'678'073");
    QTest::newRow("fr_FR 0")
            << QString("fr_FR")
            << QString("fr_FR")
            << (qlonglong) 0
            << QString("0");
    QTest::newRow("fr_FR")
            << QString("fr_FR")
            << QString("fr_FR")
            << (qlonglong) 1542678073
            << QString("1 542 678 073");
    QTest::newRow("ar_EG 0")
            << QString("ar_EG")
            << QString("ar_EG")
            << (qlonglong) 0
            << QString("٠");
    QTest::newRow("ar_EG")
            << QString("ar_EG")
            << QString("ar_EG")
            << (qlonglong) 1542678073
            << QString("١٬٥٤٢٬٦٧٨٬٠٧٣");
    QTest::newRow("ar_EG")
            << QString("ar_EG")
            << QString("en_US")
            << (qlonglong) 1542678073
            << QString("1,542,678,073");
    QTest::newRow("ur_PK")
            << QString("ur_PK")
            << QString("ur_PK")
            << (qlonglong) 1542678073
            << QString("1,542,678,073");
    QTest::newRow("ur_IN")
            << QString("ur_IN")
            << QString("ur_IN")
            << (qlonglong) 1542678073
            << QString("1,54,26,78,073");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("zh_CN")
            << (qlonglong) 1542678073
            << QString("1,542,678,073");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("ja_JP")
            << (qlonglong) 1542678073
            << QString("1,542,678,073");
    QTest::newRow("hi_IN")
            << QString("hi_IN")
            << QString("hi_IN")
            << (qlonglong) 1542678073
            << QString("१,५४,२६,७८,०७३");
}

void Ft_Numbers::testQLongLongs()
{
    QFETCH(QString, localeName);
    QFETCH(QString, localeNameLcNumeric);
    QFETCH(qlonglong, val);
    QFETCH(QString, formatted);
    MLocale loc(localeName);
    loc.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
    qDebug() << "localeName:" << localeName << "localeNameLcNumeric" << localeNameLcNumeric;
    printf("printf %s\n", loc.formatNumber(val).toUtf8().constData());
    QCOMPARE(loc.formatNumber(val), formatted);
}

void Ft_Numbers::testShorts_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<short>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("0_fi")
            << QString("fi_FI")
            << (short) 632
            << QString("632");
    QTest::newRow("0_en")
            << QString("en_GB")
            << (short) 632
            << QString("632");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << (short) 21131
            << QString("21 131");
    QTest::newRow("1_en")
            << QString("en_GB")
            << (short) 21131
            << QString("21,131");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << (short) 28515
            << QString("28 515");
    QTest::newRow("2_en")
            << QString("en_GB")
            << (short) 28515
            << QString("28,515");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << (short) 6822
            << QString("6 822");
    QTest::newRow("3_en")
            << QString("en_GB")
            << (short) 6822
            << QString("6,822");
    QTest::newRow("4_fi")
            << QString("fi_FI")
            << (short) - 1824
            << QString("-1 824");
    QTest::newRow("4_en")
            << QString("en_GB")
            << (short) - 1824
            << QString("-1,824");
    QTest::newRow("5_fi")
            << QString("fi_FI")
            << (short) 11741
            << QString("11 741");
    QTest::newRow("5_en")
            << QString("en_GB")
            << (short) 11741
            << QString("11,741");
    QTest::newRow("6_fi")
            << QString("fi_FI")
            << (short) 13490
            << QString("13 490");
    QTest::newRow("6_en")
            << QString("en_GB")
            << (short) 13490
            << QString("13,490");
    QTest::newRow("7_fi")
            << QString("fi_FI")
            << (short) - 7886
            << QString("-7 886");
    QTest::newRow("7_en")
            << QString("en_GB")
            << (short) - 7886
            << QString("-7,886");
    QTest::newRow("8_fi")
            << QString("fi_FI")
            << (short) - 27117
            << QString("-27 117");
    QTest::newRow("8_en")
            << QString("en_GB")
            << (short) - 27117
            << QString("-27,117");
    QTest::newRow("9_fi")
            << QString("fi_FI")
            << (short) 27627
            << QString("27 627");
    QTest::newRow("9_en")
            << QString("en_GB")
            << (short) 27627
            << QString("27,627");
}

void Ft_Numbers::testShorts()
{
    QFETCH(QString, localeName);
    QFETCH(short, val);
    MLocale loc(localeName);
    QTEST(loc.formatNumber(val), "formatted");
}

void Ft_Numbers::testInts_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<int>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("0_fi")
            << QString("fi_FI")
            << (int) 1369430476
            << QString("1 369 430 476");
    QTest::newRow("0_en")
            << QString("en_GB")
            << (int) 1369430476
            << QString("1,369,430,476");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << (int) 1536207176
            << QString("1 536 207 176");
    QTest::newRow("1_en")
            << QString("en_GB")
            << (int) 1536207176
            << QString("1,536,207,176");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << (int) 1320475240
            << QString("1 320 475 240");
    QTest::newRow("2_en")
            << QString("en_GB")
            << (int) 1320475240
            << QString("1,320,475,240");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << (int) 2135623067
            << QString("2 135 623 067");
    QTest::newRow("3_en")
            << QString("en_GB")
            << (int) 2135623067
            << QString("2,135,623,067");
    QTest::newRow("4_fi")
            << QString("fi_FI")
            << (int) 1868779941
            << QString("1 868 779 941");
    QTest::newRow("4_en")
            << QString("en_GB")
            << (int) 1868779941
            << QString("1,868,779,941");
    QTest::newRow("5_fi")
            << QString("fi_FI")
            << (int) 885469168
            << QString("885 469 168");
    QTest::newRow("5_en")
            << QString("en_GB")
            << (int) 885469168
            << QString("885,469,168");
    QTest::newRow("6_fi")
            << QString("fi_FI")
            << (int) 948672826
            << QString("948 672 826");
    QTest::newRow("6_en")
            << QString("en_GB")
            << (int) 948672826
            << QString("948,672,826");
    QTest::newRow("7_fi")
            << QString("fi_FI")
            << (int) 242845572
            << QString("242 845 572");
    QTest::newRow("7_en")
            << QString("en_GB")
            << (int) 242845572
            << QString("242,845,572");
    QTest::newRow("8_fi")
            << QString("fi_FI")
            << (int) 1469851943
            << QString("1 469 851 943");
    QTest::newRow("8_en")
            << QString("en_GB")
            << (int) 1469851943
            << QString("1,469,851,943");
    QTest::newRow("9_fi")
            << QString("fi_FI")
            << (int) 1970611277
            << QString("1 970 611 277");
    QTest::newRow("9_en")
            << QString("en_GB")
            << (int) 1970611277
            << QString("1,970,611,277");
}

void Ft_Numbers::testInts()
{
    QFETCH(QString, localeName);
    QFETCH(int, val);
    QFETCH(QString, formatted);
    MLocale loc(localeName);
    QCOMPARE(loc.formatNumber(val), formatted);
}

void Ft_Numbers::testDoubles_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<double>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("0_fi")
            << QString("fi_FI")
            << (double) 0
            << QString("0");
    QTest::newRow("0_en")
            << QString("en_GB")
            << (double) 0
            << QString("0");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << (double) 0.1
            << QString("0,1");
    QTest::newRow("1_en")
            << QString("en_GB")
            << (double) 0.1
            << QString("0.1");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << (double) 0.12
            << QString("0,12");
    QTest::newRow("2_en")
            << QString("en_GB")
            << (double) 0.12
            << QString("0.12");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << (double) 0.123
            << QString("0,123");
    QTest::newRow("3_en")
            << QString("en_GB")
            << (double) 0.123
            << QString("0.123");
    QTest::newRow("4_fi")
            << QString("fi_FI")
            << (double) 0.1234
            << QString("0,123");
    QTest::newRow("4_en")
            << QString("en_GB")
            << (double) 0.1234
            << QString("0.123");
    QTest::newRow("5_fi")
            << QString("fi_FI")
            << (double) 0.12345
            << QString("0,123");
    QTest::newRow("5_en")
            << QString("en_GB")
            << (double) 0.12345
            << QString("0.123");
    QTest::newRow("6_fi")
            << QString("fi_FI")
            << (double) 0.123456
            << QString("0,123");
    QTest::newRow("6_en")
            << QString("en_GB")
            << (double) 0.123456
            << QString("0.123");
    QTest::newRow("7_fi")
            << QString("fi_FI")
            << (double) 0.1234567
            << QString("0,123");
    QTest::newRow("7_en")
            << QString("en_GB")
            << (double) 0.1234567
            << QString("0.123");
    QTest::newRow("8_fi")
            << QString("fi_FI")
            << (double) 1.1234567
            << QString("1,123");
    QTest::newRow("8_en")
            << QString("en_GB")
            << (double) 1.1234567
            << QString("1.123");
    QTest::newRow("9_fi")
            << QString("fi_FI")
            << (double) 12.1234567
            << QString("12,123");
    QTest::newRow("9_en")
            << QString("en_GB")
            << (double) 12.1234567
            << QString("12.123");
    QTest::newRow("10_fi")
            << QString("fi_FI")
            << (double) 123.1234567
            << QString("123,123");
    QTest::newRow("10_en")
            << QString("en_GB")
            << (double) 123.1234567
            << QString("123.123");
    QTest::newRow("11_fi")
            << QString("fi_FI")
            << (double) 1234.1234567
            << QString("1 234,123");
    QTest::newRow("11_en")
            << QString("en_GB")
            << (double) 1234.1234567
            << QString("1,234.123");
    QTest::newRow("12_fi")
            << QString("fi_FI")
            << (double) 12345.1234567
            << QString("12 345,123");
    QTest::newRow("12_en")
            << QString("en_GB")
            << (double) 12345.1234567
            << QString("12,345.123");
    QTest::newRow("13_fi")
            << QString("fi_FI")
            << (double) 123456.1234567
            << QString("123 456,123");
    QTest::newRow("13_en")
            << QString("en_GB")
            << (double) 123456.1234567
            << QString("123,456.123");
    QTest::newRow("14_fi")
            << QString("fi_FI")
            << (double) 1234567.1234567
            << QString("1 234 567,123");
    QTest::newRow("14_en")
            << QString("en_GB")
            << (double) 1234567.1234567
            << QString("1,234,567.123");
}

void Ft_Numbers::testDoubles()
{
    QFETCH(QString, localeName);
    QFETCH(double, val);
    QFETCH(QString, formatted);
    MLocale loc(localeName);
    QCOMPARE(loc.formatNumber(val), formatted);
}

void Ft_Numbers::testFloats_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<float>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("0_fi")
            << QString("fi_FI")
            << (float) 0
            << QString("0");
    QTest::newRow("0_en")
            << QString("en_GB")
            << (float) 0
            << QString("0");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << (float) 0.1
            << QString("0,1");
    QTest::newRow("1_en")
            << QString("en_GB")
            << (float) 0.1
            << QString("0.1");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << (float) 0.12
            << QString("0,12");
    QTest::newRow("2_en")
            << QString("en_GB")
            << (float) 0.12
            << QString("0.12");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << (float) 0.123
            << QString("0,123");
    QTest::newRow("3_en")
            << QString("en_GB")
            << (float) 0.123
            << QString("0.123");
    QTest::newRow("4_fi")
            << QString("fi_FI")
            << (float) 0.1234
            << QString("0,123");
    QTest::newRow("4_en")
            << QString("en_GB")
            << (float) 0.1234
            << QString("0.123");
    QTest::newRow("5_fi")
            << QString("fi_FI")
            << (float) 0.12345
            << QString("0,123");
    QTest::newRow("5_en")
            << QString("en_GB")
            << (float) 0.12345
            << QString("0.123");
    QTest::newRow("6_fi")
            << QString("fi_FI")
            << (float) 0.123456
            << QString("0,123");
    QTest::newRow("6_en")
            << QString("en_GB")
            << (float) 0.123456
            << QString("0.123");
    QTest::newRow("7_fi")
            << QString("fi_FI")
            << (float) 0.1234567
            << QString("0,123");
    QTest::newRow("7_en")
            << QString("en_GB")
            << (float) 0.1234567
            << QString("0.123");
    QTest::newRow("8_fi")
            << QString("fi_FI")
            << (float) 1.1234567
            << QString("1,123");
    QTest::newRow("8_en")
            << QString("en_GB")
            << (float) 1.1234567
            << QString("1.123");
    QTest::newRow("9_fi")
            << QString("fi_FI")
            << (float) 12.1234567
            << QString("12,123");
    QTest::newRow("9_en")
            << QString("en_GB")
            << (float) 12.1234567
            << QString("12.123");
    QTest::newRow("10_fi")
            << QString("fi_FI")
            << (float) 123.1234567
            << QString("123,123");
    QTest::newRow("10_en")
            << QString("en_GB")
            << (float) 123.1234567
            << QString("123.123");
    QTest::newRow("11_fi")
            << QString("fi_FI")
            << (float) 1234.1234567
            << QString("1 234,123");
    QTest::newRow("11_en")
            << QString("en_GB")
            << (float) 1234.1234567
            << QString("1,234.123");
    QTest::newRow("12_fi")
            << QString("fi_FI")
            << (float) 12345.1234567
            << QString("12 345,123");
    QTest::newRow("12_en")
            << QString("en_GB")
            << (float) 12345.1234567
            << QString("12,345.123");
    QTest::newRow("13_fi")
            << QString("fi_FI")
            << (float) 123456.1234567
            << QString("123 456,125");
    QTest::newRow("13_en")
            << QString("en_GB")
            << (float) 123456.1234567
            << QString("123,456.125");
    QTest::newRow("14_fi")
            << QString("fi_FI")
            << (float) 1234567.1234567
            << QString("1 234 567,125");
    QTest::newRow("14_en")
            << QString("en_GB")
            << (float) 1234567.1234567
            << QString("1,234,567.125");
}

void Ft_Numbers::testFloats()
{
    QFETCH(QString, localeName);
    QFETCH(float, val);
    QFETCH(QString, formatted);
    MLocale loc(localeName);
    QCOMPARE(loc.formatNumber(val), formatted);
}

void Ft_Numbers::testDoublesWithFormatting_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<double>("val");
    QTest::addColumn<int>("prec");
    QTest::addColumn<QString>("formatted");

    double defaultNumber = 1234567.1234567;

    QTest::newRow("0_fi")
            << QString("fi_FI")
            << defaultNumber
            << 0 << QString("1 234 567");
    QTest::newRow("0_en")
            << QString("en_GB")
            << defaultNumber
            << 0 << QString("1,234,567");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << defaultNumber
            << 1 << QString("1 234 567,1");
    QTest::newRow("1_en")
            << QString("en_GB")
            << defaultNumber
            << 1 << QString("1,234,567.1");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << defaultNumber
            << 2 << QString("1 234 567,12");
    QTest::newRow("2_en")
            << QString("en_GB")
            << defaultNumber
            << 2 << QString("1,234,567.12");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << defaultNumber
            << 3 << QString("1 234 567,123");
    QTest::newRow("3_en")
            << QString("en_GB")
            << defaultNumber
            << 3 << QString("1,234,567.123");
    QTest::newRow("4_fi")
            << QString("fi_FI")
            << defaultNumber
            << 4 << QString("1 234 567,1235");
    QTest::newRow("4_en")
            << QString("en_GB")
            << defaultNumber
            << 4 << QString("1,234,567.1235");
    QTest::newRow("5_fi")
            << QString("fi_FI")
            << defaultNumber
            << 5 << QString("1 234 567,12346");
    QTest::newRow("5_en")
            << QString("en_GB")
            << defaultNumber
            << 5 << QString("1,234,567.12346");
    QTest::newRow("6_fi")
            << QString("fi_FI")
            << defaultNumber
            << 6 << QString("1 234 567,123457");
    QTest::newRow("6_en")
            << QString("en_GB")
            << defaultNumber
            << 6 << QString("1,234,567.123457");
}

void Ft_Numbers::testDoublesWithFormatting()
{
    QFETCH(QString, localeName);
    QFETCH(double, val);
    QFETCH(int, prec);
    MLocale loc(localeName);
    QTEST(loc.formatNumber(val, prec), "formatted");
}

void Ft_Numbers::testPercents_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<double>("val");
    QTest::addColumn<int>("decimals");
    QTest::addColumn<QString>("formatted");

    QTest::newRow("0_fi")
            << QString("fi_FI")
            << 0.123456
            << 0
            << QString("12 %");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << 0.123456
            << 1
            << QString("12,3 %");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << 0.123456
            << 2
            << QString("12,35 %");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << 0.123456
            << 3
            << QString("12,346 %");
    QTest::newRow("4_fi")
            << QString("fi_FI")
            << 0.123456
            << 4
            << QString("12,3456 %");
    QTest::newRow("0_en")
            << QString("en_GB")
            << 0.123456
            << 0
            << QString("12%");
    QTest::newRow("1_en")
            << QString("en_GB")
            << 0.123456
            << 1
            << QString("12.3%");
    QTest::newRow("2_en")
            << QString("en_GB")
            << 0.123456
            << 2
            << QString("12.35%");
    QTest::newRow("3_en")
            << QString("en_GB")
            << 0.123456
            << 3
            << QString("12.346%");
    QTest::newRow("4_en")
            << QString("en_GB")
            << 0.123456
            << 4
            << QString("12.3456%");
}

void Ft_Numbers::testPercents()
{
    QFETCH(QString, localeName);
    QFETCH(int, decimals);
    QFETCH(double, val);
    MLocale loc(localeName);
    QTEST(loc.formatPercent(val, decimals), "formatted");
}

void Ft_Numbers::testCurrencies_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<double>("val");
    QTest::addColumn<QString>("currency");
    QTest::addColumn<QString>("formatted");

    QTest::newRow("0_fi")
            << QString("fi_FI")
            << 1234.56
            << "EUR"
            << QString("1 234,56 €");
    QTest::newRow("1_fi")
            << QString("fi_FI")
            << 1234.56
            << "USD"
            << QString("1 234,56 $");
    QTest::newRow("2_fi")
            << QString("fi_FI")
            << 1234.56
            << "GBP"
            << QString("1 234,56 £");
    QTest::newRow("3_fi")
            << QString("fi_FI")
            << 1234.56
            << "XYZ"
            << QString("1 234,56 XYZ");

    QTest::newRow("0_en")
            << QString("en_GB")
            << 1234.56
            << "EUR"
            << QString("€1,234.56");
    QTest::newRow("1_en")
            << QString("en_GB")
            << 1234.56
            << "USD"
            << QString("$1,234.56");
    QTest::newRow("2_en")
            << QString("en_GB")
            << 1234.56
            << "GBP"
            << QString("£1,234.56");
    QTest::newRow("3_en")
            << QString("en_GB")
            << 1234.56
            << "XYZ"
            << QString("XYZ1,234.56");

    QTest::newRow("0_de")
            << QString("de_DE")
            << 1234.56
            << "EUR"
            << QString("1.234,56 €");
    QTest::newRow("1_de")
            << QString("de_DE")
            << 1234.56
            << "USD"
            << QString("1.234,56 $");
    QTest::newRow("2_de")
            << QString("de_DE")
            << 1234.56
            << "GBP"
            << QString("1.234,56 £");
    QTest::newRow("3_de")
            << QString("de_DE")
            << 1234.56
            << "XYZ"
            << QString("1.234,56 XYZ");
}

void Ft_Numbers::testCurrencies()
{
    QFETCH(QString, localeName);
    QFETCH(double, val);
    QFETCH(QString, currency);
    MLocale loc(localeName);
    QTEST(loc.formatCurrency(val, currency), "formatted");
}

QTEST_APPLESS_MAIN(Ft_Numbers);

