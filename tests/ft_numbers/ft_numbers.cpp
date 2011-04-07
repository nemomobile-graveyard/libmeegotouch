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

#define VERBOSE_OUTPUT

void Ft_Numbers::initTestCase()
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

void Ft_Numbers::cleanupTestCase()
{
    delete qap;
}

void Ft_Numbers::init()
{
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
            << qlonglong(1542678073)
            << QString("1,542,678,073");
    QTest::newRow("en_US")
            << QString("en_US")
            << QString("de_DE")
            << qlonglong(1542678073)
            << QString("1.542.678.073");
    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("fi_FI")
            << qlonglong(1542678073)
            << QString("1 542 678 073");
    QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("en_GB")
            << qlonglong(1542678073)
            << QString("1,542,678,073");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("de_DE")
            << qlonglong(1542678073)
            << QString("1.542.678.073");
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("de_CH")
            << qlonglong(1542678073)
            << QString("1'542'678'073");
    QTest::newRow("fr_FR 0")
            << QString("fr_FR")
            << QString("fr_FR")
            << qlonglong(0)
            << QString("0");
    QTest::newRow("fr_FR")
            << QString("fr_FR")
            << QString("fr_FR")
            << qlonglong(1542678073)
            << QString("1 542 678 073");
    QTest::newRow("ar_EG 0")
            << QString("ar_EG")
            << QString("ar_EG")
            << qlonglong(0)
            << QString("‏٠‏");
    QTest::newRow("ar_EG")
            << QString("ar_EG")
            << QString("ar_EG")
            << qlonglong(1542678073)
            << QString("‏١٬٥٤٢٬٦٧٨٬٠٧٣‏");
    QTest::newRow("ar_EG")
            << QString("ar_EG")
            << QString("ar_EG")
            << qlonglong(-1542678073)
            << QString("‏١٬٥٤٢٬٦٧٨٬٠٧٣‏-");
    QTest::newRow("ar_EG")
            << QString("ar_EG")
            << QString("en_US")
            << qlonglong(1542678073)
            << QString("‏1,542,678,073‏");
    QTest::newRow("ar_EG")
            << QString("ar_EG@numbers=arab")
            << QString("ar_EG@numbers=latn")
            << qlonglong(-1542678073)
        // with RLM markers because language is ar!:
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
            << QString("‏1.542.678.073‏-");
#else
            << QString("‏1٬542٬678٬073‏-");
#endif
    QTest::newRow("ar_EG")
            << QString("fa")
            << QString("ar_EG@numbers=latn")
            << qlonglong(-1542678073)
        // with RLM markers because language is fa!:
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
            << QString("‏1.542.678.073‏-");
#else
            << QString("‏1٬542٬678٬073‏-");
#endif
    QTest::newRow("ar_EG")
            << QString("en_US")
            << QString("ar_EG@numbers=latn")
            << qlonglong(-1542678073)
        // without RLM markers because language is en_US!:
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
            << QString("1.542.678.073-");
#else
            << QString("1٬542٬678٬073-");
#endif
    QTest::newRow("ar_TN")
            << QString("ar_TN@numbers=arab")
            << QString("ar_TN@numbers=latn")
            << qlonglong(-1542678073)
        // with RLM markers because language is ar!:
            << QString("‏1542678073‏-");
    QTest::newRow("ar_TN")
            << QString("ar_TN@numbers=arab")
            << QString("ar_TN@numbers=arab")
            << qlonglong(-1542678073)
        // with RLM markers because language is ar!:
            << QString("‏١٥٤٢٦٧٨٠٧٣‏-");
    QTest::newRow("ar_TN")
            << QString("en_US")
            << QString("ar_TN@numbers=arab")
            << qlonglong(-1542678073)
        // without RLM markers because language is en_US!:
            << QString("١٥٤٢٦٧٨٠٧٣-");
    QTest::newRow("ar_TN")
            << QString("en_US")
            << QString("ar_TN")
            << qlonglong(-1542678073)
        // without RLM markers because language is en_US!:
            << QString("١٥٤٢٦٧٨٠٧٣-");
    QTest::newRow("ar_TN")
            << QString("ar_TN")
            << QString("ar_TN")
            << qlonglong(-1542678073)
        // with RLM markers because language is en_US!:
            << QString("‏١٥٤٢٦٧٨٠٧٣‏-");
    QTest::newRow("ur_PK")
            << QString("ur_PK")
            << QString("ur_PK")
            << qlonglong(1542678073)
            << QString("1,542,678,073");
    QTest::newRow("ur_IN")
            << QString("ur_IN")
            << QString("ur_IN")
            << qlonglong(1542678073)
            << QString("1,54,26,78,073");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("zh_CN")
            << qlonglong(1542678073)
            << QString("1,542,678,073");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("ja_JP")
            << qlonglong(1542678073)
            << QString("1,542,678,073");
    QTest::newRow("hi_IN")
            << QString("hi_IN")
            << QString("hi_IN")
            << qlonglong(1542678073)
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
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "localeName: " << localeName
        << " localeNameLcNumeric: " << localeNameLcNumeric
        << " number: " << val
        << " expected: " << formatted
        << " formatNumber result: " << loc.formatNumber(val)
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(loc.formatNumber(val), formatted);
}

void Ft_Numbers::testToLongLong_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("formattedLongLong");
    QTest::addColumn<bool>("parsable");
    QTest::addColumn<qlonglong>("parsedLongLong");
    QTest::addColumn<QString>("formattedAgainLongLong");

    QTest::newRow("fi_FI \"\"")
        << QString("fi_FI")
        << QString("")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("fi_FI 0")
        << QString("fi_FI")
        << QString("0")
        << true
        << qlonglong(0)
        << QString("0");
    QTest::newRow("en_GB 0")
        << QString("en_GB")
        << QString("0")
        << true
        << qlonglong(0)
        << QString("0");
    QTest::newRow("fi_FI 0,1")
        << QString("fi_FI")
        << QString("0,1")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("en_GB 0.1")
        << QString("en_GB")
        << QString("0.1")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("fi_FI 1 234,1")
        << QString("fi_FI")
        << QString("1 234,1")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("en_GB 1,234.1")
        << QString("en_GB")
        << QString("1,234.1")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("fi_FI 1 234 567,12")
        << QString("fi_FI")
        << QString("1 234 567,12")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("en_GB 1,234,567.12")
        << QString("en_GB")
        << QString("1,234,567.12")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("en_GB 8,446,744,073,709,551,615")
        << QString("en_GB")
        << QString("8,446,744,073,709,551,615")
        << true
        << qlonglong(8446744073709551615LL)
        << QString("8,446,744,073,709,551,615");
    // The following string gives a number which is
    // too big for a 64 bit integer:
    QTest::newRow("en_GB 318,446,744,073,709,551,615")
        << QString("en_GB")
        << QString("318,446,744,073,709,551,615")
        << false
        << qlonglong(0LL)
        << QString("0");
    QTest::newRow("de_DE 1234.56")
        << QString("de_DE")
        << QString("1234.56")
        << true
        << qlonglong(123456)
        << QString("123.456");
    QTest::newRow("de_DE 1'234,56")
        << QString("de_DE")
        << QString("1'234,56")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("de_CH 1'234,56")
        << QString("de_CH")
        << QString("1'234,56")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("de_CH 1'234.56")
        << QString("de_CH")
        << QString("1'234.56")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("de_DE 1.234,56X")
        << QString("de_DE")
        << QString("1.234,56X")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("de_DE 1.23X4,56")
        << QString("de_DE")
        << QString("1.23X4,56X")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("de_DE X1.234,56")
        << QString("de_DE")
        << QString("X1.234,56")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("de_DE Nonsense")
        << QString("de_DE")
        << QString("Nonsense")
        << false
        << qlonglong(0)
        << QString("0");
    QTest::newRow("ar_SA ١٢٣٤") // ar_SA does not use thousands separators
        << QString("ar_SA")
        << QString("١٢٣٤")
        << true
        << qlonglong(1234)
        << QString("‏١٢٣٤‏");
    QTest::newRow("ar_EG ١٢٣٤") // ar_EG uses thousands separators
        << QString("ar_EG")
        << QString("١٢٣٤")
        << true
        << qlonglong(1234)
        << QString("‏١٬٢٣٤‏");
    QTest::newRow("ar_EG ١٬٢٣٤") // ar_EG uses thousands separators
        << QString("ar_EG")
        << QString("١٬٢٣٤")
        << true
        << qlonglong(1234)
        << QString("‏١٬٢٣٤‏");
    QTest::newRow("fa_IR ۱٬۰۰۰")
        << QString("fa_IR")
        << QString("۱٬۰۰۰")
        << true
        << qlonglong(1000)
        << QString("‏۱٬۰۰۰‏");
    QTest::newRow("ar_SA ١٢٣٤٫١٢٣")
        << QString("ar_SA")
        << QString("١٢٣٤٫١٢٣")
        << false
        << qlonglong(0)
        << QString("‏٠‏");
    QTest::newRow("hi_HI ४,५६७")
        << QString("hi_HI")
        << QString("४,५६७")
        << true
        << qlonglong(4567)
        << QString("४,५६७");
    QTest::newRow("hi_HI ४,५६७.१२३")
        << QString("hi_HI")
        << QString("४,५६७.१२३")
        << false
        << qlonglong(0)
        << QString("०");
}

void Ft_Numbers::testToLongLong()
{
    QFETCH(QString, localeName);
    QFETCH(QString, formattedLongLong);
    QFETCH(bool, parsable);
    QFETCH(qlonglong, parsedLongLong);
    QFETCH(QString, formattedAgainLongLong);

    MLocale locale(localeName);
    qlonglong result = locale.toLongLong(formattedLongLong);
    QCOMPARE(result, parsedLongLong);
    QCOMPARE(locale.toLongLong(formattedLongLong, NULL), parsedLongLong);
    bool ok;
    QCOMPARE(locale.toLongLong(formattedLongLong, &ok), parsedLongLong);
    QCOMPARE(ok, parsable);
    QCOMPARE(locale.formatNumber(result), formattedAgainLongLong);
}

void Ft_Numbers::testShorts_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<short>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("fi_FI 632")
            << QString("fi_FI")
            << short(632)
            << QString("632");
    QTest::newRow("en_GB 632")
            << QString("en_GB")
            << short(632)
            << QString("632");
    QTest::newRow("fi_FI 21131")
            << QString("fi_FI")
            << short(21131)
            << QString("21 131");
    QTest::newRow("en_GB 21131")
            << QString("en_GB")
            << short(21131)
            << QString("21,131");
    QTest::newRow("fi_FI 28515")
            << QString("fi_FI")
            << short(28515)
            << QString("28 515");
    QTest::newRow("en_GB 28515")
            << QString("en_GB")
            << short(28515)
            << QString("28,515");
    QTest::newRow("fi_FI 6822")
            << QString("fi_FI")
            << short(6822)
            << QString("6 822");
    QTest::newRow("en_GB 6822")
            << QString("en_GB")
            << short(6822)
            << QString("6,822");
    QTest::newRow("fi_FI -1824")
            << QString("fi_FI")
            << short(-1824)
            << QString("-1 824");
    QTest::newRow("en_GB -1824")
            << QString("en_GB")
            << short(-1824)
            << QString("-1,824");
    QTest::newRow("fi_FI 11741")
            << QString("fi_FI")
            << short(11741)
            << QString("11 741");
    QTest::newRow("en_GB 11741")
            << QString("en_GB")
            << short(11741)
            << QString("11,741");
    QTest::newRow("fi_FI 13490")
            << QString("fi_FI")
            << short(13490)
            << QString("13 490");
    QTest::newRow("en_GB 13490")
            << QString("en_GB")
            << short(13490)
            << QString("13,490");
    QTest::newRow("fi_FI -7886")
            << QString("fi_FI")
            << short(-7886)
            << QString("-7 886");
    QTest::newRow("en_GB -7886")
            << QString("en_GB")
            << short(-7886)
            << QString("-7,886");
    QTest::newRow("fi_FI -27117")
            << QString("fi_FI")
            << short(-27117)
            << QString("-27 117");
    QTest::newRow("en_GB -27117")
            << QString("en_GB")
            << short(-27117)
            << QString("-27,117");
    QTest::newRow("fi_FI 27627")
            << QString("fi_FI")
            << short(27627)
            << QString("27 627");
    QTest::newRow("en_GB 27627")
            << QString("en_GB")
            << short(27627)
            << QString("27,627");
}

void Ft_Numbers::testShorts()
{
    QFETCH(QString, localeName);
    QFETCH(short, val);
    MLocale loc(localeName);
    QTEST(loc.formatNumber(val), "formatted");
}

void Ft_Numbers::testToShort_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("formattedShort");
    QTest::addColumn<bool>("parsable");
    QTest::addColumn<short>("parsedShort");
    QTest::addColumn<QString>("formattedAgainShort");

    QTest::newRow("fi_FI \"\"")
        << QString("fi_FI")
        << QString("")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("fi_FI 0")
        << QString("fi_FI")
        << QString("0")
        << true
        << short(0)
        << QString("0");
    QTest::newRow("en_GB 0")
        << QString("en_GB")
        << QString("0")
        << true
        << short(0)
        << QString("0");
    QTest::newRow("fi_FI 0,1")
        << QString("fi_FI")
        << QString("0,1")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("en_GB 0.1")
        << QString("en_GB")
        << QString("0.1")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("fi_FI 1 234,1")
        << QString("fi_FI")
        << QString("1 234,1")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("en_GB 1,234.1")
        << QString("en_GB")
        << QString("1,234.1")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("fi_FI 1 234 567,12")
        << QString("fi_FI")
        << QString("1 234 567,12")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("en_GB 1,234,567.12")
        << QString("en_GB")
        << QString("1,234,567.12")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("en_GB 8,446,744,073,709,551,615")
        << QString("en_GB")
        << QString("8,446,744,073,709,551,615")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_DE 1234.56")
        << QString("de_DE")
        << QString("1234.56")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_DE 1'234,56")
        << QString("de_DE")
        << QString("1'234,56")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_CH 1'234,56")
        << QString("de_CH")
        << QString("1'234,56")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_CH 1'234.56")
        << QString("de_CH")
        << QString("1'234.56")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_DE 1.234,56X")
        << QString("de_DE")
        << QString("1.234,56X")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_DE 1.23X4,56")
        << QString("de_DE")
        << QString("1.23X4,56X")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_DE X1.234,56")
        << QString("de_DE")
        << QString("X1.234,56")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("de_DE Nonsense")
        << QString("de_DE")
        << QString("Nonsense")
        << false
        << short(0)
        << QString("0");
    QTest::newRow("ar_SA ١٢٣٤")
        << QString("ar_SA")
        << QString("١٢٣٤")
        << true
        << short(1234)
        << QString("‏١٢٣٤‏");
    QTest::newRow("ar_SA ١٢٣٤٫١٢٣")
        << QString("ar_SA")
        << QString("١٢٣٤٫١٢٣")
        << false
        << short(0)
        << QString("‏٠‏");
    QTest::newRow("ar_SA ١٢٣٤-")
        << QString("ar_SA")
        << QString("١٢٣٤-")
        << true
        << short(-1234)
        << QString("‏١٢٣٤‏-");
    QTest::newRow("ar_SA ١٢٣٤٫١٢٣-")
        << QString("ar_SA")
        << QString("١٢٣٤٫١٢٣-")
        << false
        << short(0)
        << QString("‏٠‏");
    QTest::newRow("hi_HI ४,५६७")
        << QString("hi_HI")
        << QString("४,५६७")
        << true
        << short(4567)
        << QString("४,५६७");
    QTest::newRow("hi_HI ४,५६७.१२३")
        << QString("hi_HI")
        << QString("४,५६७.१२३")
        << false
        << short(0)
        << QString("०");
}

void Ft_Numbers::testToShort()
{
    QFETCH(QString, localeName);
    QFETCH(QString, formattedShort);
    QFETCH(bool, parsable);
    QFETCH(short, parsedShort);
    QFETCH(QString, formattedAgainShort);

    MLocale locale(localeName);
    short result = locale.toShort(formattedShort);
    QCOMPARE(result, parsedShort);
    QCOMPARE(locale.toShort(formattedShort, NULL), parsedShort);
    bool ok;
    QCOMPARE(locale.toShort(formattedShort, &ok), parsedShort);
    QCOMPARE(ok, parsable);
    QCOMPARE(locale.formatNumber(result), formattedAgainShort);
}

void Ft_Numbers::testInts_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<int>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("fi_FI 1369430476")
            << QString("fi_FI")
            << int(1369430476)
            << QString("1 369 430 476");
    QTest::newRow("en_GB 1369430476")
            << QString("en_GB")
            << int(1369430476)
            << QString("1,369,430,476");
    QTest::newRow("fi_FI 1536207176")
            << QString("fi_FI")
            << int(1536207176)
            << QString("1 536 207 176");
    QTest::newRow("en_GB 1536207176")
            << QString("en_GB")
            << int(1536207176)
            << QString("1,536,207,176");
    QTest::newRow("fi_FI 1320475240")
            << QString("fi_FI")
            << int(1320475240)
            << QString("1 320 475 240");
    QTest::newRow("en_GB 1320475240")
            << QString("en_GB")
            << int(1320475240)
            << QString("1,320,475,240");
    QTest::newRow("fi_FI 2135623067")
            << QString("fi_FI")
            << int(2135623067)
            << QString("2 135 623 067");
    QTest::newRow("en_GB 2135623067")
            << QString("en_GB")
            << int(2135623067)
            << QString("2,135,623,067");
    QTest::newRow("fi_FI 1868779941")
            << QString("fi_FI")
            << int(1868779941)
            << QString("1 868 779 941");
    QTest::newRow("en_GB 1868779941")
            << QString("en_GB")
            << int(1868779941)
            << QString("1,868,779,941");
    QTest::newRow("fi_FI 885469168")
            << QString("fi_FI")
            << int(885469168)
            << QString("885 469 168");
    QTest::newRow("en_GB 885469168")
            << QString("en_GB")
            << int(885469168)
            << QString("885,469,168");
    QTest::newRow("fi_FI 948672826")
            << QString("fi_FI")
            << int(948672826)
            << QString("948 672 826");
    QTest::newRow("en_GB 948672826")
            << QString("en_GB")
            << int(948672826)
            << QString("948,672,826");
    QTest::newRow("fi_FI 242845572")
            << QString("fi_FI")
            << int(242845572)
            << QString("242 845 572");
    QTest::newRow("en_GB 242845572")
            << QString("en_GB")
            << int(242845572)
            << QString("242,845,572");
    QTest::newRow("fi_FI 1469851943")
            << QString("fi_FI")
            << int(1469851943)
            << QString("1 469 851 943");
    QTest::newRow("en_GB 1469851943")
            << QString("en_GB")
            << int(1469851943)
            << QString("1,469,851,943");
    QTest::newRow("fi_FI 1970611277")
            << QString("fi_FI")
            << int(1970611277)
            << QString("1 970 611 277");
    QTest::newRow("en_GB 1970611277")
            << QString("en_GB")
            << int(1970611277)
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

void Ft_Numbers::testToInt_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("formattedInt");
    QTest::addColumn<bool>("parsable");
    QTest::addColumn<int>("parsedInt");
    QTest::addColumn<QString>("formattedAgainInt");

    QTest::newRow("fi_FI \"\"")
        << QString("fi_FI")
        << QString("")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("fi_FI 0")
        << QString("fi_FI")
        << QString("0")
        << true
        << int(0)
        << QString("0");
    QTest::newRow("en_GB 0")
        << QString("en_GB")
        << QString("0")
        << true
        << int(0)
        << QString("0");
    QTest::newRow("fi_FI 0,1")
        << QString("fi_FI")
        << QString("0,1")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("en_GB 0.1")
        << QString("en_GB")
        << QString("0.1")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("fi_FI 1 234,1")
        << QString("fi_FI")
        << QString("1 234,1")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("en_GB 1,234.1")
        << QString("en_GB")
        << QString("1,234.1")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("en_GB 8,446,744,073,709,551,615")
        << QString("en_GB")
        << QString("8,446,744,073,709,551,615")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_DE 1234.56")
        << QString("de_DE")
        << QString("1234.56")
        << true
        << int(123456)
        << QString("123.456");
    QTest::newRow("de_DE 1'234,56")
        << QString("de_DE")
        << QString("1'234,56")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_CH 1'234,56")
        << QString("de_CH")
        << QString("1'234,56")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_CH 1'234.56")
        << QString("de_CH")
        << QString("1'234.56")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_DE 1.234,56X")
        << QString("de_DE")
        << QString("1.234,56X")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_DE 1.23X4,56")
        << QString("de_DE")
        << QString("1.23X4,56X")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_DE X1.234,56")
        << QString("de_DE")
        << QString("X1.234,56")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("de_DE Nonsense")
        << QString("de_DE")
        << QString("Nonsense")
        << false
        << int(0)
        << QString("0");
    QTest::newRow("ar_SA ١٢٣٤")
        << QString("ar_SA")
        << QString("١٢٣٤")
        << true
        << int(1234)
        << QString("‏١٢٣٤‏");
    QTest::newRow("ar_SA ١٢٣٤٫١٢٣")
        << QString("ar_SA")
        << QString("١٢٣٤٫١٢٣")
        << false
        << int(0)
        << QString("‏٠‏");
    QTest::newRow("hi_HI ४,५६७")
        << QString("hi_HI")
        << QString("४,५६७")
        << true
        << int(4567)
        << QString("४,५६७");
    QTest::newRow("hi_HI ४,५६७.१२३")
        << QString("hi_HI")
        << QString("४,५६७.१२३")
        << false
        << int(0)
        << QString("०");
}

void Ft_Numbers::testToInt()
{
    QFETCH(QString, localeName);
    QFETCH(QString, formattedInt);
    QFETCH(bool, parsable);
    QFETCH(int, parsedInt);
    QFETCH(QString, formattedAgainInt);

    MLocale locale(localeName);
    int result = locale.toInt(formattedInt);
    QCOMPARE(result, parsedInt);
    QCOMPARE(locale.toInt(formattedInt, NULL), parsedInt);
    bool ok;
    QCOMPARE(locale.toInt(formattedInt, &ok), parsedInt);
    QCOMPARE(ok, parsable);
    QCOMPARE(locale.formatNumber(result), formattedAgainInt);
}

void Ft_Numbers::testDoubles_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<double>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("fi_FI 0")
            << QString("fi_FI")
            << double(0)
            << QString("0");
    QTest::newRow("en_GB 0")
            << QString("en_GB")
            << double(0)
            << QString("0");
    QTest::newRow("fi_FI 0.1")
            << QString("fi_FI")
            << double(0.1)
            << QString("0,1");
    QTest::newRow("en_GB 0.1")
            << QString("en_GB")
            << double(0.1)
            << QString("0.1");
    QTest::newRow("fi_FI 0.12")
            << QString("fi_FI")
            << double(0.12)
            << QString("0,12");
    QTest::newRow("en_GB 0.12")
            << QString("en_GB")
            << double(0.12)
            << QString("0.12");
    QTest::newRow("fi_FI 0.123")
            << QString("fi_FI")
            << double(0.123)
            << QString("0,123");
    QTest::newRow("en_GB 0.123")
            << QString("en_GB")
            << double(0.123)
            << QString("0.123");
    QTest::newRow("fi_FI 0.1234")
            << QString("fi_FI")
            << double(0.1234)
            << QString("0,123");
    QTest::newRow("en_GB 0.1234")
            << QString("en_GB")
            << double(0.1234)
            << QString("0.123");
    QTest::newRow("fi_FI 0.12345")
            << QString("fi_FI")
            << double(0.12345)
            << QString("0,123");
    QTest::newRow("en_GB 0.12345")
            << QString("en_GB")
            << double(0.12345)
            << QString("0.123");
    QTest::newRow("fi_FI 0.123456")
            << QString("fi_FI")
            << double(0.123456)
            << QString("0,123");
    QTest::newRow("en_GB 0.123456")
            << QString("en_GB")
            << double(0.123456)
            << QString("0.123");
    QTest::newRow("fi_FI 0.1234567")
            << QString("fi_FI")
            << double(0.1234567)
            << QString("0,123");
    QTest::newRow("en_GB 0.1234567")
            << QString("en_GB")
            << double(0.1234567)
            << QString("0.123");
    QTest::newRow("fi_FI 1.1234567")
            << QString("fi_FI")
            << double(1.1234567)
            << QString("1,123");
    QTest::newRow("en_GB 1.1234567")
            << QString("en_GB")
            << double(1.1234567)
            << QString("1.123");
    QTest::newRow("fi_FI 12.1234567")
            << QString("fi_FI")
            << double(12.1234567)
            << QString("12,123");
    QTest::newRow("en_GB 12.1234567")
            << QString("en_GB")
            << double(12.1234567)
            << QString("12.123");
    QTest::newRow("fi_FI 123.1234567")
            << QString("fi_FI")
            << double(123.1234567)
            << QString("123,123");
    QTest::newRow("en_GB 123.1234567")
            << QString("en_GB")
            << double(123.1234567)
            << QString("123.123");
    QTest::newRow("fi_FI 1234.1234567")
            << QString("fi_FI")
            << double(1234.1234567)
            << QString("1 234,123");
    QTest::newRow("en_GB 1234.1234567")
            << QString("en_GB")
            << double(1234.1234567)
            << QString("1,234.123");
    QTest::newRow("fi_FI 12345.1234567")
            << QString("fi_FI")
            << double(12345.1234567)
            << QString("12 345,123");
    QTest::newRow("en_GB 12345.1234567")
            << QString("en_GB")
            << double(12345.1234567)
            << QString("12,345.123");
    QTest::newRow("fi_FI 123456.1234567")
            << QString("fi_FI")
            << double(123456.1234567)
            << QString("123 456,123");
    QTest::newRow("en_GB 123456.1234567")
            << QString("en_GB")
            << double(123456.1234567)
            << QString("123,456.123");
    QTest::newRow("fi_FI 1234567.1234567")
            << QString("fi_FI")
            << double(1234567.1234567)
            << QString("1 234 567,123");
    QTest::newRow("en_GB 1234567.1234567")
            << QString("en_GB")
            << double(1234567.1234567)
            << QString("1,234,567.123");
    QTest::newRow("ar_SA 1234567.1234567")
            << QString("ar_SA")
            << double(1234567.1234567)
            << QString("‏١٢٣٤٥٦٧٫١٢٣‏");
    QTest::newRow("hi_HI 1234567.1234567")
            << QString("hi_HI")
            << double(1234567.1234567)
            << QString("१२,३४,५६७.१२३");
}

void Ft_Numbers::testDoubles()
{
    QFETCH(QString, localeName);
    QFETCH(double, val);
    QFETCH(QString, formatted);
    MLocale loc(localeName);
    QString result = loc.formatNumber(val);
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << "result: " << result << " expected: " << formatted << "\n";
    QCOMPARE(result, formatted);
}

void Ft_Numbers::testToDouble_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("formattedDouble");
    QTest::addColumn<bool>("parsable");
    QTest::addColumn<double>("parsedDouble");
    QTest::addColumn<QString>("formattedAgainDouble");

    QTest::newRow("fi_FI \"\"")
        << QString("fi_FI")
        << QString("")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("fi_FI 0")
        << QString("fi_FI")
        << QString("0")
        << true
        << double(0)
        << QString("0");
    QTest::newRow("en_GB 0")
        << QString("en_GB")
        << QString("0")
        << true
        << double(0)
        << QString("0");
    QTest::newRow("fi_FI 0,1")
        << QString("fi_FI")
        << QString("0,1")
        << true
        << double(0.1)
        << QString("0,1");
    QTest::newRow("en_GB 0.1")
        << QString("en_GB")
        << QString("0.1")
        << true
        << double(0.1)
        << QString("0.1");
    QTest::newRow("fi_FI 0,12")
        << QString("fi_FI")
        << QString("0,12")
        << true
        << double(0.12)
        << QString("0,12");
    QTest::newRow("en_GB 0.12")
        << QString("en_GB")
        << QString("0.12")
        << true
        << double(0.12)
        << QString("0.12");
    QTest::newRow("fi_FI 0,123")
        << QString("fi_FI")
        << QString("0,123")
        << true
        << double(0.123)
        << QString("0,123");
    QTest::newRow("en_GB 0.123")
        << QString("en_GB")
        << QString("0.123")
        << true
        << double(0.123)
        << QString("0.123");
    QTest::newRow("fi_FI 0,1234")
        << QString("fi_FI")
        << QString("0,1234")
        << true
        << double(0.1234)
        << QString("0,1234");
    QTest::newRow("en_GB 0.1234")
        << QString("en_GB")
        << QString("0.1234")
        << true
        << double(0.1234)
        << QString("0.1234");
    QTest::newRow("fi_FI 0,12345")
        << QString("fi_FI")
        << QString("0,12345")
        << true
        << double(0.12345)
        << QString("0,12345");
    QTest::newRow("en_GB 0.12345")
        << QString("en_GB")
        << QString("0.12345")
        << true
        << double(0.12345)
        << QString("0.12345");
    QTest::newRow("fi_FI 0,123456")
        << QString("fi_FI")
        << QString("0,123456")
        << true
        << double(0.123456)
        << QString("0,123456");
    QTest::newRow("en_GB 0.123456")
        << QString("en_GB")
        << QString("0.123456")
        << true
        << double(0.123456)
        << QString("0.123456");
    QTest::newRow("fi_FI 0,1234567")
        << QString("fi_FI")
        << QString("0,1234567")
        << true
        << double(0.1234567)
        << QString("0,1234567");
    QTest::newRow("en_GB 0.1234567")
        << QString("en_GB")
        << QString("0.1234567")
        << true
        << double(0.1234567)
        << QString("0.1234567");
    QTest::newRow("fi_FI 1,1234567")
        << QString("fi_FI")
        << QString("1,1234567")
        << true
        << double(1.1234567)
        << QString("1,1234567");
    QTest::newRow("en_GB 1.1234567")
        << QString("en_GB")
        << QString("1.1234567")
        << true
        << double(1.1234567)
        << QString("1.1234567");
    QTest::newRow("fi_FI 12,1234567")
        << QString("fi_FI")
        << QString("12,1234567")
        << true
        << double(12.1234567)
        << QString("12,1234567");
    QTest::newRow("en_GB 12.1234567")
        << QString("en_GB")
        << QString("12.1234567")
        << true
        << double(12.1234567)
        << QString("12.1234567");
    QTest::newRow("fi_FI 123,1234567")
        << QString("fi_FI")
        << QString("123,1234567")
        << true
        << double(123.1234567)
        << QString("123,1234567");
    QTest::newRow("en_GB 123.1234567")
        << QString("en_GB")
        << QString("123.1234567")
        << true
        << double(123.1234567)
        << QString("123.1234567");
    QTest::newRow("fi_FI 1 234,1234567")
        << QString("fi_FI")
        << QString("1 234,1234567")
        << true
        << double(1234.1234567)
        << QString("1 234,1234567");
    QTest::newRow("en_GB 1,234.1234567")
        << QString("en_GB")
        << QString("1,234.1234567")
        << true
        << double(1234.1234567)
        << QString("1,234.1234567");
    QTest::newRow("fi_FI 12 345,1234567")
        << QString("fi_FI")
        << QString("12 345,1234567")
        << true
        << double(12345.1234567)
        << QString("12 345,1234567");
    QTest::newRow("en_GB 12,345.1234567")
        << QString("en_GB")
        << QString("12,345.1234567")
        << true
        << double(12345.1234567)
        << QString("12,345.1234567");
    QTest::newRow("fi_FI 123 456,1234567")
        << QString("fi_FI")
        << QString("123 456,1234567")
        << true
        << double(123456.1234567)
        << QString("123 456,1234567");
    QTest::newRow("en_GB 123,456.1234567")
        << QString("en_GB")
        << QString("123,456.1234567")
        << true
        << double(123456.1234567)
        << QString("123,456.1234567");
    QTest::newRow("fi_FI 1 234 567,1234567")
        << QString("fi_FI")
        << QString("1 234 567,1234567")
        << true
        << double(1234567.1234567)
        << QString("1 234 567,1234567");
    QTest::newRow("en_GB 1,234,567.1234567")
        << QString("en_GB")
        << QString("1,234,567.1234567")
        << true
        << double(1234567.1234567)
        << QString("1,234,567.1234567");
    QTest::newRow("en_GB 1,234,567.1234567")
        << QString("en_GB")
        << QString("1,234,567.1234567")
        << true
        << double(1234567.1234567)
        << QString("1,234,567.1234567");
    QTest::newRow("en_GB f4")
        << QString("en_GB")
        << QString("f4")
        << false
        << double(0)
        << QString("0");

    QTest::newRow("en_GB 4f")
        << QString("en_GB")
        << QString("4f")
        << false
        << double(0)
        << QString("0");

    QTest::newRow("en_GB 1E+9")
        << QString("en_GB")
        << QString("1E+9")
        << true
        << double(1.0E+9)
        << QString("1,000,000,000");

    if (!icuPackageVersion.isEmpty() && icuPackageVersion < "4.4.2-0maemo3") {
        qDebug() << "NB#206085 not yet fixed, some exponents parsed wrong.";
        QTest::newRow("en_GB 1E+10")
            << QString("en_GB")
            << QString("1E+10")
            << true
            << double(0)
            << QString("0");
    }
    else {
        qDebug() << "NB#206085 fixed, exponent parsing corrected.";
        QTest::newRow("en_GB 1E+10")
            << QString("en_GB")
            << QString("1E+10")
            << true
            << double(1.0E+10)
            << QString("10,000,000,000");

        QTest::newRow("en_GB 1E+10")
            << QString("en_GB")
            << QString("1E+10")
            << true // exponential symbol is case insensitive now.
            << double(1E+10)
            << QString("10,000,000,000");

        QTest::newRow("en_GB 1e+10")
            << QString("en_GB")
            << QString("1e+10")
            << true // exponential symbol is case insensitive now.
            << double(1e+10)
            << QString("10,000,000,000");

        QTest::newRow("el_GR 1E+10")
            << QString("el_GR")
            << QString("1E+10")
            << true // exponential symbol is case insensitive now.
            << double(1e+10)
            << QString("10.000.000.000");

        QTest::newRow("el_GR 1e+10")
            << QString("el_GR")
            << QString("1e+10")
            << true // exponential symbol is case insensitive now.
            << double(1E+10)
            << QString("10.000.000.000");

        QTest::newRow("sv_SE 1E+10")
            << QString("sv_SE")
            << QString("1E+10")
            << false // OK not to parse this in Swedish?
            << double(0)
            << QString("0");

        QTest::newRow("sv_SE 1e+10")
            << QString("sv_SE")
            << QString("1e+10")
            << false // OK not to parse this in Swedish?
            << double(0)
            << QString("0");

        QTest::newRow("sv_SE 1×10^10")
            << QString("sv_SE")
            << QString("1×10^10")
            << true
            << double(1E+10)
            << QString("10 000 000 000");

        QTest::newRow("ar_SA ١اس+١٠") // ar_SA does not use thousands separators
            << QString("ar_SA")
            << QString("١اس+١٠")
            << true
            << double(1E+10)
             << QString("‏١٠٠٠٠٠٠٠٠٠٠‏");

        QTest::newRow("ar_SA ١٠٠٠") // ar_SA does not use thousands separators
            << QString("ar_SA")
            << QString("١٠٠٠")
            << true
            << double(1000.0)
             << QString("‏١٠٠٠‏");

        QTest::newRow("ar_SA ١٬٠٠٠") // ar_SA does not use thousands separators
            << QString("ar_SA")
            << QString("١٬٠٠٠")
            << false
            << double(0.0)
            << QString("‏٠‏");

        QTest::newRow("ar_EG ١اس+١٠") // ar_EG does not use thousands separators
            << QString("ar_EG")
            << QString("١اس+١٠")
            << true
            << double(1E+10)
            << QString("‏١٠٬٠٠٠٬٠٠٠٬٠٠٠‏");

        QTest::newRow("ar_EG ١٠٠٠") // ar_EG does not use thousands separators
            << QString("ar_EG")
            << QString("١٠٠٠")
            << true
            << double(1000.0)
            << QString("‏١٬٠٠٠‏");

        QTest::newRow("ar_EG ١٬٠٠٠") // ar_EG does not use thousands separators
            << QString("ar_EG")
            << QString("١٬٠٠٠")
            << true
            << double(1000.0)
            << QString("‏١٬٠٠٠‏");

        QTest::newRow("fa_IR ١×۱۰^١۰")
            << QString("fa_IR")
            << QString("۱×۱۰^۱۰")
            << true
            << double(1E+10)
            << QString("‏۱۰٬۰۰۰٬۰۰۰٬۰۰۰‏");

        QTest::newRow("fa_IR ۱۰۰۰")
            << QString("fa_IR")
            << QString("۱۰۰۰")
            << true
            << double(1000.0)
            << QString("‏۱٬۰۰۰‏");

        QTest::newRow("fa_IR ۱٬۰۰۰")
            << QString("fa_IR")
            << QString("۱٬۰۰۰")
            << true
            << double(1000.0)
            << QString("‏۱٬۰۰۰‏");
    }

    QTest::newRow("de_DE 1234.56")
        << QString("de_DE")
        << QString("1234.56")
        << true           // With QLocale this fails
        << double(123456) // ICU gives a weird result
        << QString("123.456");
    QTest::newRow("de_DE 1'234,56")
        << QString("de_DE")
        << QString("1'234,56")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("de_CH 1'234,56")
        << QString("de_CH")
        << QString("1'234,56")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("de_CH 1'234.56")
        << QString("de_CH")
        << QString("1'234.56")
        << true
        << double(1234.56)
        << QString("1'234.56");
    QTest::newRow("de_DE 1.234,56X")
        << QString("de_DE")
        << QString("1.234,56X")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("de_DE 1.23X4,56")
        << QString("de_DE")
        << QString("1.23X4,56X")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("de_DE X1.234,56")
        << QString("de_DE")
        << QString("X1.234,56")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("de_DE Nonsense")
        << QString("de_DE")
        << QString("Nonsense")
        << false
        << double(0)
        << QString("0");
    QTest::newRow("ar_SA ١٢٣٤٥٦٧٫١٢٣٤٥٦٧")
        << QString("ar_SA")
        << QString("١٢٣٤٥٦٧٫١٢٣٤٥٦٧")
        << true
        << double(1234567.1234567)
        << QString("‏١٢٣٤٥٦٧٫١٢٣٤٥٦٧‏");
    QTest::newRow("ar_SA ١٢٣٤٥٦٧٫١٢٣٤٥٦٧")
        << QString("ar_SA")
        << QString("‏١٢٣٤٥٦٧٫١٢٣٤٥٦٧‏")
        << true
        << double(1234567.1234567)
        << QString("‏١٢٣٤٥٦٧٫١٢٣٤٥٦٧‏");
    QTest::newRow("hi_HI १२,३४,५६७.१२३४५६७")
        << QString("hi_HI")
        << QString("१२,३४,५६७.१२३४५६७")
        << true
        << double(1234567.1234567)
        << QString("१२,३४,५६७.१२३४५६७");
}

void Ft_Numbers::testToDouble()
{
    QFETCH(QString, localeName);
    QFETCH(QString, formattedDouble);
    QFETCH(bool, parsable);
    QFETCH(double, parsedDouble);
    QFETCH(QString, formattedAgainDouble);

    MLocale locale(localeName);
    double result = locale.toDouble(formattedDouble);
    QCOMPARE(result, parsedDouble);
    QCOMPARE(locale.toDouble(formattedDouble, NULL), parsedDouble);
    bool ok;
    QCOMPARE(locale.toDouble(formattedDouble, &ok), parsedDouble);
    QCOMPARE(ok, parsable);
    QCOMPARE(locale.formatNumber(result, 1000), formattedAgainDouble);
}

void Ft_Numbers::testFloats_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<float>("val");
    QTest::addColumn<QString>("formatted");
    QTest::newRow("fi_FI 0")
            << QString("fi_FI")
            << float(0)
            << QString("0");
    QTest::newRow("en_GB 0")
            << QString("en_GB")
            << float(0)
            << QString("0");
    QTest::newRow("fi_FI 0.1")
            << QString("fi_FI")
            << float(0.1)
            << QString("0,1");
    QTest::newRow("en_GB 0.1")
            << QString("en_GB")
            << float(0.1)
            << QString("0.1");
    QTest::newRow("fi_FI 0.12")
            << QString("fi_FI")
            << float(0.12)
            << QString("0,12");
    QTest::newRow("en_GB 0.12")
            << QString("en_GB")
            << float(0.12)
            << QString("0.12");
    QTest::newRow("fi_FI 0.123")
            << QString("fi_FI")
            << float(0.123)
            << QString("0,123");
    QTest::newRow("en_GB 0.123")
            << QString("en_GB")
            << float(0.123)
            << QString("0.123");
    QTest::newRow("fi_FI 0.1234")
            << QString("fi_FI")
            << float(0.1234)
            << QString("0,123");
    QTest::newRow("en_GB 0.1234")
            << QString("en_GB")
            << float(0.1234)
            << QString("0.123");
    QTest::newRow("fi_FI 0.12345")
            << QString("fi_FI")
            << float(0.12345)
            << QString("0,123");
    QTest::newRow("en_GB 0.12345")
            << QString("en_GB")
            << float(0.12345)
            << QString("0.123");
    QTest::newRow("fi_FI 0.123456")
            << QString("fi_FI")
            << float(0.123456)
            << QString("0,123");
    QTest::newRow("en_GB 0.123456")
            << QString("en_GB")
            << float(0.123456)
            << QString("0.123");
    QTest::newRow("fi_FI 0.1234567")
            << QString("fi_FI")
            << float(0.1234567)
            << QString("0,123");
    QTest::newRow("en_GB 0.1234567")
            << QString("en_GB")
            << float(0.1234567)
            << QString("0.123");
    QTest::newRow("fi_FI 1.1234567")
            << QString("fi_FI")
            << float(1.1234567)
            << QString("1,123");
    QTest::newRow("en_GB 1.1234567")
            << QString("en_GB")
            << float(1.1234567)
            << QString("1.123");
    QTest::newRow("fi_FI 12.1234567")
            << QString("fi_FI")
            << float(12.1234567)
            << QString("12,123");
    QTest::newRow("en_GB 12.1234567")
            << QString("en_GB")
            << float(12.1234567)
            << QString("12.123");
    QTest::newRow("fi_FI 123.1234567")
            << QString("fi_FI")
            << float(123.1234567)
            << QString("123,123");
    QTest::newRow("en_GB 123.1234567")
            << QString("en_GB")
            << float(123.1234567)
            << QString("123.123");
    QTest::newRow("fi_FI 1234.1234567")
            << QString("fi_FI")
            << float(1234.1234567)
            << QString("1 234,123");
    QTest::newRow("en_GB 1234.1234567")
            << QString("en_GB")
            << float(1234.1234567)
            << QString("1,234.123");
    QTest::newRow("fi_FI 12345.1234567")
            << QString("fi_FI")
            << float(12345.1234567)
            << QString("12 345,123");
    QTest::newRow("en_GB 12345.1234567")
            << QString("en_GB")
            << float(12345.1234567)
            << QString("12,345.123");
    QTest::newRow("fi_FI 123456.1234567")
            << QString("fi_FI")
            << float(123456.1234567)
            << QString("123 456,125");
    QTest::newRow("en_GB 123456.1234567")
            << QString("en_GB")
            << float(123456.1234567)
            << QString("123,456.125");
    QTest::newRow("fi_FI 1234567.1234567")
            << QString("fi_FI")
            << float(1234567.1234567)
            << QString("1 234 567,125");
    QTest::newRow("en_GB 1234567.1234567")
            << QString("en_GB")
            << float(1234567.1234567)
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

void Ft_Numbers::testToFloat_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("formattedFloat");
    QTest::addColumn<bool>("parsable");
    QTest::addColumn<float>("parsedFloat");
    QTest::addColumn<QString>("formattedAgainFloat");

    QTest::newRow("fi_FI \"\"")
        << QString("fi_FI")
        << QString("")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("fi_FI 0")
        << QString("fi_FI")
        << QString("0")
        << true
        << float(0)
        << QString("0");
    QTest::newRow("en_GB 0")
        << QString("en_GB")
        << QString("0")
        << true
        << float(0)
        << QString("0");
    QTest::newRow("fi_FI 0,1")
        << QString("fi_FI")
        << QString("0,1")
        << true
        << float(0.1)
        << QString("0,1");
    QTest::newRow("en_GB 0.1")
        << QString("en_GB")
        << QString("0.1")
        << true
        << float(0.1)
        << QString("0.1");
    QTest::newRow("fi_FI 0,12")
        << QString("fi_FI")
        << QString("0,12")
        << true
        << float(0.12)
        << QString("0,12");
    QTest::newRow("en_GB 0.12")
        << QString("en_GB")
        << QString("0.12")
        << true
        << float(0.12)
        << QString("0.12");
    QTest::newRow("fi_FI 0,123")
        << QString("fi_FI")
        << QString("0,123")
        << true
        << float(0.123)
        << QString("0,123");
    QTest::newRow("en_GB 0.123")
        << QString("en_GB")
        << QString("0.123")
        << true
        << float(0.123)
        << QString("0.123");
    QTest::newRow("fi_FI 0,1234")
        << QString("fi_FI")
        << QString("0,1234")
        << true
        << float(0.1234)
        << QString("0,123");
    QTest::newRow("en_GB 0.1234")
        << QString("en_GB")
        << QString("0.1234")
        << true
        << float(0.1234)
        << QString("0.123");
    QTest::newRow("fi_FI 0,12345")
        << QString("fi_FI")
        << QString("0,12345")
        << true
        << float(0.12345)
        << QString("0,123");
    QTest::newRow("en_GB 0.12345")
        << QString("en_GB")
        << QString("0.12345")
        << true
        << float(0.12345)
        << QString("0.123");
    QTest::newRow("fi_FI 0,123456")
        << QString("fi_FI")
        << QString("0,123456")
        << true
        << float(0.123456)
        << QString("0,123");
    QTest::newRow("en_GB 0.123456")
        << QString("en_GB")
        << QString("0.123456")
        << true
        << float(0.123456)
        << QString("0.123");
    QTest::newRow("fi_FI 0,1234567")
        << QString("fi_FI")
        << QString("0,1234567")
        << true
        << float(0.1234567)
        << QString("0,123");
    QTest::newRow("en_GB 0.1234567")
        << QString("en_GB")
        << QString("0.1234567")
        << true
        << float(0.1234567)
        << QString("0.123");
    QTest::newRow("fi_FI 1,1234567")
        << QString("fi_FI")
        << QString("1,1234567")
        << true
        << float(1.1234567)
        << QString("1,123");
    QTest::newRow("en_GB 1.1234567")
        << QString("en_GB")
        << QString("1.1234567")
        << true
        << float(1.1234567)
        << QString("1.123");
    QTest::newRow("fi_FI 12,1234567")
        << QString("fi_FI")
        << QString("12,1234567")
        << true
        << float(12.1234567)
        << QString("12,123");
    QTest::newRow("en_GB 12.1234567")
        << QString("en_GB")
        << QString("12.1234567")
        << true
        << float(12.1234567)
        << QString("12.123");
    QTest::newRow("fi_FI 123,1234567")
        << QString("fi_FI")
        << QString("123,1234567")
        << true
        << float(123.1234567)
        << QString("123,123");
    QTest::newRow("en_GB 123.1234567")
        << QString("en_GB")
        << QString("123.1234567")
        << true
        << float(123.1234567)
        << QString("123.123");
    QTest::newRow("fi_FI 1 234,1234567")
        << QString("fi_FI")
        << QString("1 234,1234567")
        << true
        << float(1234.1234567)
        << QString("1 234,123");
    QTest::newRow("en_GB 1,234.1234567")
        << QString("en_GB")
        << QString("1,234.1234567")
        << true
        << float(1234.1234567)
        << QString("1,234.123");
    QTest::newRow("fi_FI 12 345,1234567")
        << QString("fi_FI")
        << QString("12 345,1234567")
        << true
        << float(12345.1234567)
        << QString("12 345,123");
    QTest::newRow("en_GB 12,345.1234567")
        << QString("en_GB")
        << QString("12,345.1234567")
        << true
        << float(12345.1234567)
        << QString("12,345.123");
    QTest::newRow("fi_FI 123 456,1234567")
        << QString("fi_FI")
        << QString("123 456,1234567")
        << true
        << float(123456.1234567)
        << QString("123 456,125");
    QTest::newRow("en_GB 123,456.1234567")
        << QString("en_GB")
        << QString("123,456.1234567")
        << true
        << float(123456.1234567)
        << QString("123,456.125");
    QTest::newRow("fi_FI 1 234 567,1234567")
        << QString("fi_FI")
        << QString("1 234 567,1234567")
        << true
        << float(1234567.1234567)
        << QString("1 234 567,125");
    QTest::newRow("en_GB 1,234,567.1234567")
        << QString("en_GB")
        << QString("1,234,567.1234567")
        << true
        << float(1234567.1234567)
        << QString("1,234,567.125");
    QTest::newRow("en_GB 1,234,567.1234567")
        << QString("en_GB")
        << QString("1,234,567.1234567")
        << true
        << float(1234567.1234567)
        << QString("1,234,567.125");

    QTest::newRow("en_GB f4")
        << QString("en_GB")
        << QString("f4")
        << false
        << float(0)
        << QString("0");

    QTest::newRow("en_GB 4f")
        << QString("en_GB")
        << QString("4f")
        << false
        << float(0)
        << QString("0");

    QTest::newRow("en_GB 1E+9")
        << QString("en_GB")
        << QString("1E+9")
        << true
        << float(1.0E+9)
        << QString("1,000,000,000");

    if (!icuPackageVersion.isEmpty() && icuPackageVersion < "4.4.2-0maemo3") {
        qDebug() << "NB#206085 not yet fixed, some exponents parsed wrong.";
        QTest::newRow("en_GB 1E+10")
            << QString("en_GB")
            << QString("1E+10")
            << true
            << float(0)
            << QString("0");
    }
    else {
        qDebug() << "NB#206085 fixed, exponent parsing corrected.";
        QTest::newRow("en_GB 1E+10")
            << QString("en_GB")
            << QString("1E+10")
            << true
            << float(1.0E+10)
            << QString("10,000,000,000");

        QTest::newRow("en_GB 1E+10")
            << QString("en_GB")
            << QString("1E+10")
            << true // exponential symbol is case insensitive now.
            << float(1E+10)
            << QString("10,000,000,000");

        QTest::newRow("en_GB 1e+10")
            << QString("en_GB")
            << QString("1e+10")
            << true // exponential symbol is case insensitive now.
            << float(1e+10)
            << QString("10,000,000,000");

        QTest::newRow("el_GR 1E+10")
            << QString("el_GR")
            << QString("1E+10")
            << true // exponential symbol is case insensitive now.
            << float(1e+10)
            << QString("10.000.000.000");

        QTest::newRow("el_GR 1e+10")
            << QString("el_GR")
            << QString("1e+10")
            << true // exponential symbol is case insensitive now.
            << float(1E+10)
            << QString("10.000.000.000");

        QTest::newRow("sv_SE 1E+10")
            << QString("sv_SE")
            << QString("1E+10")
            << false // OK not to parse this in Swedish?
            << float(0)
            << QString("0");

        QTest::newRow("sv_SE 1e+10")
            << QString("sv_SE")
            << QString("1e+10")
            << false // OK not to parse this in Swedish?
            << float(0)
            << QString("0");

        QTest::newRow("sv_SE 1×10^10")
            << QString("sv_SE")
            << QString("1×10^10")
            << true
            << float(1E+10)
            << QString("10 000 000 000");

        QTest::newRow("ar_SA ١اس+١٠")
            << QString("ar_SA")
            << QString("١اس+١٠")
            << true
            << float(1E+10)
            << QString("‏١٠٠٠٠٠٠٠٠٠٠‏");
        QTest::newRow("ar_SA ١اس+١٠")
            << QString("ar_SA")
            << QString("‏١اس+١٠‏")
            << true
            << float(1E+10)
            << QString("‏١٠٠٠٠٠٠٠٠٠٠‏");

        QTest::newRow("ar_SA ١٠٠٠")
            << QString("ar_SA")
            << QString("١٠٠٠")
            << true
            << float(1000.0)
            << QString("‏١٠٠٠‏");
        QTest::newRow("ar_SA ١٠٠٠")
            << QString("ar_SA")
            << QString("‏١٠٠٠‏")
            << true
            << float(1000.0)
            << QString("‏١٠٠٠‏");

        QTest::newRow("ar_SA ١٬٠٠٠")
            << QString("ar_SA")
            << QString("١٬٠٠٠")
            << false
            << float(0.0)
            << QString("‏٠‏");
        QTest::newRow("ar_SA ١٬٠٠٠")
            << QString("ar_SA")
            << QString("‏١٬٠٠٠‏")
            << false
            << float(0.0)
            << QString("‏٠‏");

        QTest::newRow("ar_EG ١اس+١٠")
            << QString("ar_EG")
            << QString("١اس+١٠")
            << true
            << float(1E+10)
            << QString("‏١٠٬٠٠٠٬٠٠٠٬٠٠٠‏");
        QTest::newRow("ar_EG ١اس+١٠")
            << QString("ar_EG")
            << QString("‏١اس+١٠‏")
            << true
            << float(1E+10)
            << QString("‏١٠٬٠٠٠٬٠٠٠٬٠٠٠‏");

        QTest::newRow("ar_EG ١٠٠٠")
            << QString("ar_EG")
            << QString("١٠٠٠")
            << true
            << float(1000.0)
            << QString("‏١٬٠٠٠‏");
        QTest::newRow("ar_EG ١٠٠٠")
            << QString("ar_EG")
            << QString("‏١٠٠٠‏")
            << true
            << float(1000.0)
            << QString("‏١٬٠٠٠‏");

        QTest::newRow("ar_EG ١٬٠٠٠")
            << QString("ar_EG")
            << QString("١٬٠٠٠")
            << true
            << float(1000.0)
            << QString("‏١٬٠٠٠‏");
        QTest::newRow("ar_EG ١٬٠٠٠")
            << QString("ar_EG")
            << QString("‏١٬٠٠٠‏")
            << true
            << float(1000.0)
            << QString("‏١٬٠٠٠‏");

        QTest::newRow("fa_IR ١×۱۰^١۰")
            << QString("fa_IR")
            << QString("۱×۱۰^۱۰")
            << true
            << float(1E+10)
            << QString("‏۱۰٬۰۰۰٬۰۰۰٬۰۰۰‏");
        QTest::newRow("fa_IR ١×۱۰^١۰")
            << QString("fa_IR")
            << QString("‏۱×۱۰^۱۰‏")
            << true
            << float(1E+10)
            << QString("‏۱۰٬۰۰۰٬۰۰۰٬۰۰۰‏");

        QTest::newRow("fa_IR ۱۰۰۰")
            << QString("fa_IR")
            << QString("۱۰۰۰")
            << true
            << float(1000.0)
            << QString("‏۱٬۰۰۰‏");
        QTest::newRow("fa_IR ۱۰۰۰")
            << QString("fa_IR")
            << QString("‏۱۰۰۰‏")
            << true
            << float(1000.0)
            << QString("‏۱٬۰۰۰‏");

        QTest::newRow("fa_IR ۱٬۰۰۰")
            << QString("fa_IR")
            << QString("۱٬۰۰۰")
            << true
            << float(1000.0)
            << QString("‏۱٬۰۰۰‏");
        QTest::newRow("fa_IR ۱٬۰۰۰")
            << QString("fa_IR")
            << QString("‏۱٬۰۰۰‏")
            << true
            << float(1000.0)
            << QString("‏۱٬۰۰۰‏");
    }

    QTest::newRow("en_GB 1E+38")
        << QString("en_GB")
        << QString("1E+38")
        << true
        << float(1.0E+38)
        << QString("99,999,996,802,856,900,000,000,000,000,000,000,000");

    // FLT_MAX:
    QTest::newRow("en_GB 3.40282E+38")
        << QString("en_GB")
        << QString("3.40282E+38")
        << true
        << float(3.40282E+38)
        << QString("340,282,001,837,566,000,000,000,000,000,000,000,000");
    // FLT_MAX * 10:
    QTest::newRow("en_GB 3.40282E+39")
        << QString("en_GB")
        << QString("3.40282E+39")
        << false
        << float(0)
        << QString("0");

    QTest::newRow("fr_FR 1234 56")
        << QString("fr_FR")
        << QString("1234 56")
        << true // With QLocale this fails
        << float(123456) // ICU gives a weird result
        << QString("123 456");
    QTest::newRow("de_DE 1234.56")
        << QString("de_DE")
        << QString("1234.56")
        << true           // With QLocale this fails
        << float(123456) // ICU gives a weird result
        << QString("123.456");
    QTest::newRow("de_DE 1'234,56")
        << QString("de_DE")
        << QString("1'234,56")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("de_CH 1'234,56")
        << QString("de_CH")
        << QString("1'234,56")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("de_CH 1'234.56")
        << QString("de_CH")
        << QString("1'234.56")
        << true
        << float(1234.56)
        << QString("1'234.56");
    QTest::newRow("de_DE 1.234,56X")
        << QString("de_DE")
        << QString("1.234,56X")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("de_DE 1.23X4,56")
        << QString("de_DE")
        << QString("1.23X4,56X")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("de_DE X1.234,56")
        << QString("de_DE")
        << QString("X1.234,56")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("de_DE Nonsense")
        << QString("de_DE")
        << QString("Nonsense")
        << false
        << float(0)
        << QString("0");
    QTest::newRow("ar_SA ١٢٣٤٫١٢٣")
        << QString("ar_SA")
        << QString("‏١٢٣٤٫١٢٣‏")
        << true
        << float(1234.123)
        << QString("‏١٢٣٤٫١٢٣‏");
    QTest::newRow("ar_SA ١٢٣٤٫١٢٣")
        << QString("ar_SA")
        << QString("١٢٣٤٫١٢٣")
        << true
        << float(1234.123)
        << QString("‏١٢٣٤٫١٢٣‏");
    QTest::newRow("hi_HI ४,५६७.१२३")
        << QString("hi_HI")
        << QString("४,५६७.१२३")
        << true
        << float(4567.123)
        << QString("४,५६७.१२३");
}

void Ft_Numbers::testToFloat()
{
    QFETCH(QString, localeName);
    QFETCH(QString, formattedFloat);
    QFETCH(bool, parsable);
    QFETCH(float, parsedFloat);
    QFETCH(QString, formattedAgainFloat);

    MLocale locale(localeName);
    float result = locale.toFloat(formattedFloat);
#if defined(VERBOSE_OUTPUT)
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << localeName
           << " formattedFloat: " << formattedFloat
           << " parsable: " << parsable
           << " result: " << result
           << " parsedFloat: " << parsedFloat
           << " locale.formatNumber(result): " << locale.formatNumber(result)
           << " formattedAgainFloat " << formattedAgainFloat
           << "\n";
    stream.flush();
#endif
    QCOMPARE(result, parsedFloat);
    QCOMPARE(locale.toFloat(formattedFloat, NULL), parsedFloat);
    bool ok;
    QCOMPARE(locale.toFloat(formattedFloat, &ok), parsedFloat);
    QCOMPARE(ok, parsable);
    QCOMPARE(locale.formatNumber(result), formattedAgainFloat);
}

void Ft_Numbers::testDoublesWithFormatting_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<double>("val");
    QTest::addColumn<int>("prec");
    QTest::addColumn<QString>("formatted");

    double defaultNumber = 1234567.1234567;

    QTest::newRow("fi_FI 0")
            << QString("fi_FI")
            << defaultNumber
            << 0 << QString("1 234 567");
    QTest::newRow("en_GB 0")
            << QString("en_GB")
            << defaultNumber
            << 0 << QString("1,234,567");
    QTest::newRow("fi_FI 1")
            << QString("fi_FI")
            << defaultNumber
            << 1 << QString("1 234 567,1");
    QTest::newRow("en_GB 1")
            << QString("en_GB")
            << defaultNumber
            << 1 << QString("1,234,567.1");
    QTest::newRow("fi_FI 2")
            << QString("fi_FI")
            << defaultNumber
            << 2 << QString("1 234 567,12");
    QTest::newRow("en_GB 2")
            << QString("en_GB")
            << defaultNumber
            << 2 << QString("1,234,567.12");
    QTest::newRow("fi_FI 3")
            << QString("fi_FI")
            << defaultNumber
            << 3 << QString("1 234 567,123");
    QTest::newRow("en_GB 3")
            << QString("en_GB")
            << defaultNumber
            << 3 << QString("1,234,567.123");
    QTest::newRow("fi_FI 4")
            << QString("fi_FI")
            << defaultNumber
            << 4 << QString("1 234 567,1235");
    QTest::newRow("en_GB 4")
            << QString("en_GB")
            << defaultNumber
            << 4 << QString("1,234,567.1235");
    QTest::newRow("fi_FI 5")
            << QString("fi_FI")
            << defaultNumber
            << 5 << QString("1 234 567,12346");
    QTest::newRow("en_GB 5")
            << QString("en_GB")
            << defaultNumber
            << 5 << QString("1,234,567.12346");
    QTest::newRow("fi_FI 6")
            << QString("fi_FI")
            << defaultNumber
            << 6 << QString("1 234 567,123457");
    QTest::newRow("en_GB 6")
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

    QTest::newRow("fi_FI")
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
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("lcMonetary");
    QTest::addColumn<QString>("lcNumeric");
    QTest::addColumn<double>("val");
    QTest::addColumn<QString>("currency");
    QTest::addColumn<QString>("formatted");

    QTest::newRow("0_fi")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fi_FI")
        << 1234.56
        << "EUR"
        << QString("1 234,56 €");
    QTest::newRow("1_fi")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fi_FI")
        << 1234.56
        << "USD"
        << QString("1 234,56 $");
    QTest::newRow("2_fi")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fi_FI")
        << 1234.56
        << "GBP"
        << QString("1 234,56 £");
    QTest::newRow("3_fi")
        << QString("fi_FI")
        << QString("fi_FI")
        << QString("fi_FI")
        << 1234.56
        << "XYZ"
        << QString("1 234,56 XYZ");
    QTest::newRow("0_en")
        << QString("en_GB")
        << QString("en_GB")
        << QString("en_GB")
        << 1234.56
        << "EUR"
        << QString("€1,234.56");
    QTest::newRow("1_en")
        << QString("en_GB")
        << QString("en_GB")
        << QString("en_GB")
        << 1234.56
        << "USD"
        << QString("$1,234.56");
    QTest::newRow("2_en")
        << QString("en_GB")
        << QString("en_GB")
        << QString("en_GB")
        << 1234.56
        << "GBP"
        << QString("£1,234.56");
    QTest::newRow("3_en")
        << QString("en_GB")
        << QString("en_GB")
        << QString("en_GB")
        << 1234.56
        << "XYZ"
        << QString("XYZ1,234.56");
    QTest::newRow("0_de")
        << QString("de_DE")
        << QString("de_DE")
        << QString("de_DE")
        << 1234.56
        << "EUR"
        << QString("1.234,56 €");
    QTest::newRow("1_de")
        << QString("de_DE")
        << QString("de_DE")
        << QString("de_DE")
        << 1234.56
        << "USD"
        << QString("1.234,56 $");
    QTest::newRow("2_de")
        << QString("de_DE")
        << QString("de_DE")
        << QString("de_DE")
        << 1234.56
        << "GBP"
        << QString("1.234,56 £");
    QTest::newRow("3_de")
        << QString("de_DE")
        << QString("de_DE")
        << QString("de_DE")
        << 1234.56
        << "XYZ"
        << QString("1.234,56 XYZ");
    QTest::newRow("ar_EG")
        << QString("de_DE")
        << QString("ar_EG")
        << QString("ar_EG")
        << 1234.56
        << "USD"
        << QString("US$ ١٬٢٣٤٫٥٦");
    QTest::newRow("ar_EG")
        << QString("de_DE") // language
        << QString("ar_EG") // lc_monetary
        << QString("en_US") // lc_numeric
        << 1234.56
        << "USD"
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
        << QString("US$ 1.234,56");
#else
        << QString("US$ 1٬234٫56");
#endif
    QTest::newRow("ar_SA")
        << QString("de_DE")
        << QString("ar_SA")
        << QString("ar_SA")
        << 1234.56
        << "USD"
        << QString("US$١٢٣٤٫٥٦");
    QTest::newRow("ar_SA")
        << QString("de_DE")
        << QString("ar_SA")
        << QString("en_US")
        << 1234.56
        << "USD"
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
        << QString("US$1234,56");
#else
        << QString("US$1234٫56");
#endif
    QTest::newRow("hi_IN")
        << QString("de_DE")
        << QString("hi_IN")
        << QString("hi_IN")
        << 1234.56
        << "USD"
        << QString("US$ १,२३४.५६");
    QTest::newRow("hi_IN")
        << QString("de_DE")
        << QString("hi_IN")
        << QString("en_US")
        << 1234.56
        << "USD"
        << QString("US$ 1,234.56");
}

void Ft_Numbers::testCurrencies()
{
    QFETCH(QString, language);
    QFETCH(QString, lcMonetary);
    QFETCH(QString, lcNumeric);
    QFETCH(double, val);
    QFETCH(QString, currency);
    QFETCH(QString, formatted);
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMonetary, lcMonetary);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << " language: " << language
        << " lcMonetary: " << lcMonetary
        << " lcNumeric: " << lcNumeric
        << " result: " << locale.formatCurrency(val, currency)
        << " expected: " << formatted
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(locale.formatCurrency(val, currency), formatted);
}

void Ft_Numbers::testPercentPlaceholdersInQt_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("localeNameLcNumeric");
    QTest::addColumn<QString>("formatString");
    QTest::addColumn<double>("number");
    QTest::addColumn<QString>("formatted");

    QTest::newRow("zh_CN")
        << QString("zh_CN")
        << QString("zh_CN")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("ar_EG")
        << QString("ar_EG")
        << QString("en_US")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("ar_EG")
        << QString("ar_EG")
        << QString("de_DE")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1,2345");
    QTest::newRow("ar_EG")
        << QString("en_US")
        << QString("ar_EG")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_TN")
        << QString("en_US")
        << QString("ar_TN")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_MA")
        << QString("en_US")
        << QString("ar_MA")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_DZ")
        << QString("en_US")
        << QString("ar_DZ")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_EG")
        << QString("en_US")
        << QString("ar_EG@numbers=latn")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("ar_EG")
        << QString("en_US")
        << QString("ar_EG@numbers=arab")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_MA")
        << QString("en_US")
        << QString("ar_MA@numbers=latn")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("ar_MA")
        << QString("en_US")
        << QString("ar_MA@numbers=arab")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_DZ")
        << QString("en_US")
        << QString("ar_DZ@numbers=latn")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("ar_DZ")
        << QString("en_US")
        << QString("ar_DZ@numbers=arab")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("ar_TN")
        << QString("en_US")
        << QString("ar_TN@numbers=latn")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("ar_TN")
        << QString("en_US")
        << QString("ar_TN@numbers=arab")
        << QString("%L1")
        << double(-1.2345)
        << QString("-١٫٢٣٤٥");
    QTest::newRow("fa_IR")
        << QString("en_US")
        << QString("fa_IR@numbers=latn")
        << QString("%L1")
        << double(-1.2345)
        << QString("-1.2345");
    QTest::newRow("fa_IR")
        << QString("en_US")
        << QString("fa_IR@numbers=arab")
        << QString("%L1")
        << double(-1.2345)
        << QString("−۱٫۲۳۴۵");
    QTest::newRow("fa_IR")
        << QString("en_US")
        << QString("fa_IR")
        << QString("%L1")
        << double(-1.2345)
        << QString("−۱٫۲۳۴۵");
}

void Ft_Numbers::testPercentPlaceholdersInQt()
{
    QFETCH(QString, localeName);
    QFETCH(QString, localeNameLcNumeric);
    QFETCH(QString, formatString);
    QFETCH(double, number);
    QFETCH(QString, formatted);

    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
    MLocale::setDefault(locale);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << " localeName: " << localeName
        << " localeNameLcNumeric: " << localeNameLcNumeric
        << " number: " << number
        << " result: " << formatString.arg(number)
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(formatString.arg(number), formatted);

}

QTEST_APPLESS_MAIN(Ft_Numbers);

