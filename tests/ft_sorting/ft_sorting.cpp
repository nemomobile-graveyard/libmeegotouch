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

#include "ft_sorting.h"
#include <DuiLocale>
#include <DuiCollator>
#include <QTextCodec>
#include <QDebug>
#include <QProcess>

void Ft_Sorting::initTestCase()
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

void Ft_Sorting::cleanup()
{
}

void Ft_Sorting::testDuiLocaleSorting_data()
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

    if (icuPackageVersion < "4.2.1-0maemo3") {
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

    if (icuPackageVersion < "4.2.1-0maemo3") {
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

void Ft_Sorting::testDuiLocaleSorting()
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

    DuiLocale loc(locale_name);
    DuiCollator comp = loc.collator();
    QStringList sl;
    sl << source1 << source2 << source3 << source4 << source5;
    // printf("%s %s %s %s %s\n", sl[0].toUtf8().data(), sl[1].toUtf8().data(), sl[2].toUtf8().data(), sl[3].toUtf8().data(), sl[4].toUtf8().data());
    qSort(sl.begin(), sl.end(), comp);
    // printf("%s %s %s %s %s\n", sl[0].toUtf8().data(), sl[1].toUtf8().data(), sl[2].toUtf8().data(), sl[3].toUtf8().data(), sl[4].toUtf8().data());

    QCOMPARE(sl[0], target1);
    QCOMPARE(sl[1], target2);
    QCOMPARE(sl[2], target3);
    QCOMPARE(sl[3], target4);
    QCOMPARE(sl[4], target5);
}

void Ft_Sorting::testDefaultCompare_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("str1");
    QTest::addColumn<QString>("str2");
    QTest::addColumn<DuiLocale::Comparison>("result");

    QTest::newRow("fi_FI-LessThan")
            << QString("fi_FI")
            << QString("AAA")
            << QString("BBB")
            << DuiLocale::LessThan;
    QTest::newRow("fi_FI-GreaterThan")
            << QString("fi_FI")
            << QString("BBB")
            << QString("AAA")
            << DuiLocale::GreaterThan;
    QTest::newRow("fi_FI-Equal")
            << QString("fi_FI")
            << QString("AAA")
            << QString("AAA")
            << DuiLocale::Equal;
    QTest::newRow("de_DE-LessThan")
            << QString("de_DE")
            << QString("oegh")
            << QString("öfgh")
            << DuiLocale::LessThan;
    QTest::newRow("de_DE@collation=phonebook-GreaterThan")
            << QString("de_DE@collation=phonebook")
            << QString("oegh")
            << QString("öfgh")
            << DuiLocale::GreaterThan;
}

void Ft_Sorting::testDefaultCompare()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, str1);
    QFETCH(QString, str2);
    QFETCH(DuiLocale::Comparison, result);

    DuiLocale loc(locale_name);
    DuiCollator duicomp = loc.collator();
    DuiLocale::setDefault(loc);
    QCOMPARE(duicomp.compare(str1, str2), result);
}

void Ft_Sorting::testCompareWithLocale_data()
{
    QTest::addColumn<QString>("locale_name1");
    QTest::addColumn<QString>("locale_name2");
    QTest::addColumn<QString>("str1");
    QTest::addColumn<QString>("str2");
    QTest::addColumn<DuiLocale::Comparison>("result");

    QTest::newRow("fi_FI-LessThan")
            << QString("en_EN")
            << QString("fi_FI")
            << QString("AAA")
            << QString("BBB")
            << DuiLocale::LessThan;
    QTest::newRow("fi_FI-GreaterThan")
            << QString("en_EN")
            << QString("fi_FI")
            << QString("BBB")
            << QString("AAA")
            << DuiLocale::GreaterThan;
    QTest::newRow("fi_FI-Equal")
            << QString("en_EN")
            << QString("fi_FI")
            << QString("AAA")
            << QString("AAA")
            << DuiLocale::Equal;
    QTest::newRow("de_DE-LessThan")
            << QString("de_DE@collation=phonebook")
            << QString("de_DE")
            << QString("oegh")
            << QString("öfgh")
            << DuiLocale::LessThan;
    QTest::newRow("de_DE@collation=phonebook-GreaterThan")
            << QString("de_DE")
            << QString("de_DE@collation=phonebook")
            << QString("oegh")
            << QString("öfgh")
            << DuiLocale::GreaterThan;
}

void Ft_Sorting::testCompareWithLocale()
{
    QFETCH(QString, locale_name1);
    QFETCH(QString, locale_name2);
    QFETCH(QString, str1);
    QFETCH(QString, str2);
    QFETCH(DuiLocale::Comparison, result);

    DuiLocale loc1(locale_name1);
    DuiCollator duicomp = loc1.collator();
    DuiLocale::setDefault(loc1);

    DuiLocale loc2(locale_name2);
    QVERIFY2(duicomp.compare(loc2, str1, str2) == result, "Compare failed");
}

QTEST_APPLESS_MAIN(Ft_Sorting);
