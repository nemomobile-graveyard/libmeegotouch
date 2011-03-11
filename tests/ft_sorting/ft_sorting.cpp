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

#include "ft_sorting.h"
#include <MLocale>
#include <MCollator>
#include <QTextCodec>
#include <QDebug>
#include <QProcess>

#define VERBOSE_OUTPUT

class TestCollator : public MCollator
{
public:
    TestCollator () : MCollator() {}
    TestCollator (const MLocale &locale) : MCollator(locale) {}
};

void Ft_Sorting::initTestCase()
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

void Ft_Sorting::cleanup()
{
}

void Ft_Sorting::testMLocaleSetAndGetCollation_data()
{
    QTest::addColumn<QString>("localeNameOrig");
    QTest::addColumn<QString>("lcCollateOrig");
    QTest::addColumn<MLocale::Collation>("collationOrig");
    QTest::addColumn<MLocale::Collation>("collationNew");
    QTest::addColumn<QString>("localeNameNew");
    QTest::addColumn<QString>("lcCollateNew");

    QTest::newRow("default->default")
        << "fi"
        << "fi"
        << MLocale::DefaultCollation
        << MLocale::DefaultCollation
        << "fi"
        << "fi";
    QTest::newRow("default->default")
        << "fi@collation=phonebook"
        << "fi"
        << MLocale::DefaultCollation
        << MLocale::DefaultCollation
        << "fi@collation=phonebook"
        << "fi";
    QTest::newRow("phonebook->default")
        << "fi@collation=standard"
        << "fi@collation=phonebook"
        << MLocale::PhonebookCollation
        << MLocale::DefaultCollation
        << "fi@collation=standard"
        << "fi";
    QTest::newRow("phonebook->phonebook")
        << "ja_JP"
        << "zh_CN@collation=phonebook;calendar=islamic-civil;foo=bar"
        << MLocale::PhonebookCollation
        << MLocale::PhonebookCollation
        << "ja_JP"
        << "zh_CN@collation=phonebook;calendar=islamic-civil;foo=bar";
    QTest::newRow("stroke->default")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::DefaultCollation
        << "ja_JP"
        << "zh_CN@calendar=islamic-civil;foo=bar";
    QTest::newRow("default->stroke")
        << "ja_JP"
        << "zh_CN@calendar=islamic-civil;foo=bar"
        << MLocale::DefaultCollation
        << MLocale::StrokeCollation
        << "ja_JP"
        << "zh_CN@calendar=islamic-civil;foo=bar;collation=stroke";
    QTest::newRow("stroke->pinyin")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::PinyinCollation
        << "ja_JP"
        << "zh_CN@collation=pinyin;calendar=islamic-civil;foo=bar";
    QTest::newRow("stroke->traditional")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::TraditionalCollation
        << "ja_JP"
        << "zh_CN@collation=traditional;calendar=islamic-civil;foo=bar";
    QTest::newRow("stroke->direct")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::DirectCollation
        << "ja_JP"
        << "zh_CN@collation=direct;calendar=islamic-civil;foo=bar";
    QTest::newRow("stroke->posix")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::PosixCollation
        << "ja_JP"
        << "zh_CN@collation=posix;calendar=islamic-civil;foo=bar";
    QTest::newRow("stroke->big5han")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::Big5hanCollation
        << "ja_JP"
        << "zh_CN@collation=big5han;calendar=islamic-civil;foo=bar";
    QTest::newRow("stroke->gb2312han")
        << "ja_JP"
        << "zh_CN@collation=stroke;calendar=islamic-civil;foo=bar"
        << MLocale::StrokeCollation
        << MLocale::Gb2312hanCollation
        << "ja_JP"
        << "zh_CN@collation=gb2312han;calendar=islamic-civil;foo=bar";
}

void Ft_Sorting::testMLocaleSetAndGetCollation()
{
    QFETCH(QString, localeNameOrig);
    QFETCH(QString, lcCollateOrig);
    QFETCH(MLocale::Collation, collationOrig);
    QFETCH(MLocale::Collation, collationNew);
    QFETCH(QString, localeNameNew);
    QFETCH(QString, lcCollateNew);
    MLocale locale(localeNameOrig);
    QVERIFY2(locale.isValid(), "constructor did not create a valid locale");
    locale.setCategoryLocale(MLocale::MLcCollate, lcCollateOrig);
    QCOMPARE(locale.collation(), collationOrig);
    locale.setCollation(collationNew);
    QCOMPARE(locale.name(), localeNameNew);
    QCOMPARE(locale.categoryName(MLocale::MLcCollate), lcCollateNew);
    QCOMPARE(locale.collation(), collationNew);
}

void Ft_Sorting::testMLocaleSorting_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("source1");
    QTest::addColumn<QString>("source2");
    QTest::addColumn<QString>("source3");
    QTest::addColumn<QString>("source4");
    QTest::addColumn<QString>("source5");
    QTest::addColumn<QString>("target1");
    QTest::addColumn<QString>("target2");
    QTest::addColumn<QString>("target3");
    QTest::addColumn<QString>("target4");
    QTest::addColumn<QString>("target5");

    QTest::newRow("Sorting_fi_FI")
            << QString("fi_FI")
            << QString("z3zz")
            << QString("åtgh")
            << QString("b2bb")
            << QString("ähjj")
            << QString("abcd")

            << QString("abcd")
            << QString("b2bb")
            << QString("z3zz")
            << QString("åtgh")
            << QString("ähjj");

    QTest::newRow("Sorting_nn_NO")
            << QString("nn_NO")
            << QString("å")
            << QString("ø")
            << QString("z")
            << QString("a")
            << QString("f")

            << QString("a")
            << QString("f")
            << QString("z")
            << QString("ø")
            << QString("å");

    if (!icuPackageVersion.isEmpty() &&  icuPackageVersion < "4.2.1-0maemo3") {
        qDebug() << "NB#154449 not yet fixed, sort order must be wrong.";
        QTest::newRow("Sorting_nb_NO")
                << QString("nb_NO")
                << QString("å")
                << QString("ø")
                << QString("z")
                << QString("a")
                << QString("f")

                << QString("a")
                << QString("å")
                << QString("f")
                << QString("ø")
                << QString("z");
    } else {
        qDebug() << "NB#154449 fixed, sort order must be correct.";
        QTest::newRow("Sorting_nb_NO")
                << QString("nb_NO")
                << QString("å")
                << QString("ø")
                << QString("z")
                << QString("a")
                << QString("f")

                << QString("a")
                << QString("f")
                << QString("z")
                << QString("ø")
                << QString("å");
    }

    if (!icuPackageVersion.isEmpty() &&  icuPackageVersion < "4.2.1-0maemo3") {
        qDebug() << "NB#154449 not yet fixed, sort order must be wrong.";
        QTest::newRow("Sorting_no_NO")
                << QString("no_NO")
                << QString("å")
                << QString("ø")
                << QString("z")
                << QString("a")
                << QString("f")

                << QString("a")
                << QString("å")
                << QString("f")
                << QString("ø")
                << QString("z");
    } else {
        qDebug() << "NB#154449 fixed, sort order must be correct.";
        QTest::newRow("Sorting_no_NO")
                << QString("no_NO")
                << QString("å")
                << QString("ø")
                << QString("z")
                << QString("a")
                << QString("f")

                << QString("a")
                << QString("f")
                << QString("z")
                << QString("ø")
                << QString("å");
    }

    QTest::newRow("Sorting_en_GB")
            << QString("en_EN")
            << QString("z3zz")
            << QString("åtgh")
            << QString("b2bb")
            << QString("ähjj")
            << QString("abcd")

            << QString("abcd")
            << QString("ähjj")
            << QString("åtgh")
            << QString("b2bb")
            << QString("z3zz");

    QTest::newRow("Sorting_de_DE")
            << QString("de_DE")
            << QString("z3zz")
            << QString("oegh")
            << QString("b2bb")
            << QString("öfgh")
            << QString("abcd")

            << QString("abcd")
            << QString("b2bb")
            << QString("oegh")
            << QString("öfgh")
            << QString("z3zz");

    QTest::newRow("Sorting_de_DE@collation=phonebook")
            << QString("de_DE@collation=phonebook")
            << QString("z3zz")
            << QString("oegh")
            << QString("b2bb")
            << QString("öfgh")
            << QString("abcd")

            << QString("abcd")
            << QString("b2bb")
            << QString("öfgh")
            << QString("oegh")
            << QString("z3zz");

    QTest::newRow("Sorting_ja_JP@collation=standard")
            << QString("ja_JP@collation=standard")
            << QString("哀")  // U+54C0
            << QString("娃")  // U+5A03
            << QString("唖")  // U+5516
            << QString("阿")  // U+963F
            << QString("亜")  // U+4E9C

            << QString("亜")  // U+4E9C
            << QString("唖")  // U+5516
            << QString("娃")  // U+5A03
            << QString("阿")  // U+963F
            << QString("哀"); // U+54C0

    QTest::newRow("Sorting_ja_JP@collation=unihan")
            << QString("ja_JP@collation=unihan")
            << QString("哀")  // U+54C0
            << QString("娃")  // U+5A03
            << QString("唖")  // U+5516
            << QString("阿")  // U+963F
            << QString("亜")  // U+4E9C

            << QString("亜")  // U+4E9C
            << QString("哀")  // U+54C0
            << QString("唖")  // U+5516
            << QString("娃")  // U+5A03
            << QString("阿"); // U+963F

    QTest::newRow("Sorting_zh_CN@collation=pinyin")
            << QString("zh_CN@collation=pinyin")
            << QString("俄")
            << QString("得")
            << QString("此")
            << QString("播")
            << QString("爱")

            << QString("爱")
            << QString("播")
            << QString("此")
            << QString("得")
            << QString("俄");

    QTest::newRow("Sorting_zh_CN@collation=stroke")
            << QString("zh_CN@collation=stroke")
            << QString("二")
            << QString("三")
            << QString("一")
            << QString("正")
            << QString("中")

            << QString("一")
            << QString("二")
            << QString("三")
            << QString("中")
            << QString("正");
}

void Ft_Sorting::testMLocaleSorting()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, source1);
    QFETCH(QString, source2);
    QFETCH(QString, source3);
    QFETCH(QString, source4);
    QFETCH(QString, source5);

    QFETCH(QString, target1);
    QFETCH(QString, target2);
    QFETCH(QString, target3);
    QFETCH(QString, target4);
    QFETCH(QString, target5);

    MLocale locale(locale_name);
    MLocale::setDefault(locale);
    MCollator mCollator = locale.collator();
    QStringList stringListOrig;
    stringListOrig << source1 << source2 << source3 << source4 << source5;
    QStringList sl = stringListOrig;
#if defined(VERBOSE_OUTPUT)
    printf("%s %s %s %s %s\n", sl[0].toUtf8().data(), sl[1].toUtf8().data(), sl[2].toUtf8().data(), sl[3].toUtf8().data(), sl[4].toUtf8().data());
#endif
    qSort(sl.begin(), sl.end(), mCollator);
#if defined(VERBOSE_OUTPUT)
    printf("%s %s %s %s %s\n", sl[0].toUtf8().data(), sl[1].toUtf8().data(), sl[2].toUtf8().data(), sl[3].toUtf8().data(), sl[4].toUtf8().data());
#endif
    QCOMPARE(sl[0], target1);
    QCOMPARE(sl[1], target2);
    QCOMPARE(sl[2], target3);
    QCOMPARE(sl[3], target4);
    QCOMPARE(sl[4], target5);

    MCollator mCollatorDefaultLocale;
    QStringList sl2 = stringListOrig;
    qSort(sl2.begin(), sl2.end(), mCollatorDefaultLocale);
    QCOMPARE(sl2, sl);

    TestCollator *testCollator = new TestCollator(locale);
    sl2 = stringListOrig;
    qSort(sl2.begin(), sl2.end(), *testCollator);
    QCOMPARE(sl2, sl);
    delete testCollator;

    TestCollator *testCollatorDefaultLocale = new TestCollator();
    sl2 = stringListOrig;
    qSort(sl2.begin(), sl2.end(), *testCollatorDefaultLocale);
    QCOMPARE(sl2, sl);
    delete testCollatorDefaultLocale;
}

void Ft_Sorting::testDefaultCompare_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("str1");
    QTest::addColumn<QString>("str2");
    QTest::addColumn<MLocale::Comparison>("result");

    QTest::newRow("fi_FI-LessThan")
            << QString("fi_FI")
            << QString("AAA")
            << QString("BBB")
            << MLocale::LessThan;
    QTest::newRow("fi_FI-GreaterThan")
            << QString("fi_FI")
            << QString("BBB")
            << QString("AAA")
            << MLocale::GreaterThan;
    QTest::newRow("fi_FI-Equal")
            << QString("fi_FI")
            << QString("AAA")
            << QString("AAA")
            << MLocale::Equal;
    QTest::newRow("de_DE-LessThan")
            << QString("de_DE")
            << QString("oegh")
            << QString("öfgh")
            << MLocale::LessThan;
    QTest::newRow("de_DE@collation=phonebook-GreaterThan")
            << QString("de_DE@collation=phonebook")
            << QString("oegh")
            << QString("öfgh")
            << MLocale::GreaterThan;
}

void Ft_Sorting::testDefaultCompare()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, str1);
    QFETCH(QString, str2);
    QFETCH(MLocale::Comparison, result);

    MLocale locale(locale_name);
    MLocale::setDefault(locale);
    MCollator mCollator(locale);
    MCollator mCollator2 = locale.collator();
    MCollator mCollatorDefaultLocale;
    QCOMPARE(mCollator.compare(str1, str2), result);
    QCOMPARE(mCollator2.compare(str1, str2), result);
    QCOMPARE(mCollatorDefaultLocale.compare(str1, str2), result);
}

void Ft_Sorting::testCompareWithLocale_data()
{
    QTest::addColumn<QString>("locale_name1");
    QTest::addColumn<QString>("locale_name2");
    QTest::addColumn<QString>("str1");
    QTest::addColumn<QString>("str2");
    QTest::addColumn<MLocale::Comparison>("result");

    QTest::newRow("fi_FI-LessThan")
            << QString("en_EN")
            << QString("fi_FI")
            << QString("AAA")
            << QString("BBB")
            << MLocale::LessThan;
    QTest::newRow("fi_FI-GreaterThan")
            << QString("en_EN")
            << QString("fi_FI")
            << QString("BBB")
            << QString("AAA")
            << MLocale::GreaterThan;
    QTest::newRow("fi_FI-Equal")
            << QString("en_EN")
            << QString("fi_FI")
            << QString("AAA")
            << QString("AAA")
            << MLocale::Equal;
    QTest::newRow("de_DE-LessThan")
            << QString("de_DE@collation=phonebook")
            << QString("de_DE")
            << QString("oegh")
            << QString("öfgh")
            << MLocale::LessThan;
    QTest::newRow("de_DE@collation=phonebook-GreaterThan")
            << QString("de_DE")
            << QString("de_DE@collation=phonebook")
            << QString("oegh")
            << QString("öfgh")
            << MLocale::GreaterThan;
}

void Ft_Sorting::testCompareWithLocale()
{
    QFETCH(QString, locale_name1);
    QFETCH(QString, locale_name2);
    QFETCH(QString, str1);
    QFETCH(QString, str2);
    QFETCH(MLocale::Comparison, result);

    MLocale loc1(locale_name1);
    MCollator mcomp = loc1.collator();
    MLocale::setDefault(loc1);

    MLocale loc2(locale_name2);
    QVERIFY2(mcomp.compare(loc2, str1, str2) == result, "Compare failed");
}

QTEST_APPLESS_MAIN(Ft_Sorting);
