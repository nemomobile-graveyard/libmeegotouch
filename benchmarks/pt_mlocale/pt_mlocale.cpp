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

#include "pt_mlocale.h"

#define VERBOSE_OUTPUT

void Pt_MLocale::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QProcess process;
#ifdef HAVE_ICU
    process.start("sh -c \"dpkg -s libicu44 | grep Version | perl -pe 's/^Version:[[:space:]]*([^[[:space:]]+)$/$1/g'\"");
    if (!process.waitForFinished()) {
        qDebug() << "cannot run process to check libicu44 package version , exiting ...";
        exit(1);
    }
    icuPackageVersion = process.readAllStandardOutput();
    icuPackageVersion.replace('\n', "");
    qDebug() << "libicu44 package version is:" << icuPackageVersion;
#endif
}

void Pt_MLocale::cleanupTestCase()
{
    delete qap;
}

void Pt_MLocale::init()
{
}

void Pt_MLocale::cleanup()
{
}

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkSingleLanguageEndonym()
{
    QString language("de_CH");   // only this should matter
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // should not matter
    QString lcNumeric("en_US");  // should not matter
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);

    QString result("Schweizer Hochdeutsch");
    QCOMPARE(locale.languageEndonym(), result);

    QBENCHMARK {
        locale.languageEndonym();
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkAllLanguageEndonym()
{
    QList<QString> localeNames;
    int numberOfAvailableLocales = uloc_countAvailable();
    for (int i = 0; i < numberOfAvailableLocales; ++i)
        localeNames << QString::fromUtf8(uloc_getAvailable(i));

    QBENCHMARK {
        foreach (QString localeName, localeNames) {
            MLocale locale(localeName);
#if 0
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream << locale.name() << " " << locale.languageEndonym() << "\n";
#endif
            locale.languageEndonym();
        }
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkSingleCountryEndonym()
{
    QString language("de_CH");   // only this should matter
    QString lcMessages("en_US"); // should not matter
    QString lcTime("ar_SA");     // should not matter
    QString lcNumeric("en_US");  // should not matter
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcMessages, lcMessages);
    locale.setCategoryLocale(MLocale::MLcTime, lcTime);
    locale.setCategoryLocale(MLocale::MLcNumeric, lcNumeric);

    QString result("Schweiz");
    QCOMPARE(locale.countryEndonym(), result);

    QBENCHMARK {
        locale.countryEndonym();
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkAllCountryEndonym()
{
    QList<QString> localeNames;
    int numberOfAvailableLocales = uloc_countAvailable();
    for (int i = 0; i < numberOfAvailableLocales; ++i)
        localeNames << QString::fromUtf8(uloc_getAvailable(i));

    QBENCHMARK {
        foreach (QString localeName, localeNames) {
            MLocale locale(localeName);
#if 0
            QTextStream debugStream(stdout);
            debugStream.setCodec("UTF-8");
            debugStream << locale.name() << " " << locale.countryEndonym() << "\n";
#endif
            locale.countryEndonym();
        }
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberQLongLongArabic()
{
    QString localeName("ar");
    QString localeNameLcNumeric("ar_EG@numbers=arab");
    qlonglong number = qlonglong(-1542678073);
    QString formatted("‏١٬٥٤٢٬٦٧٨٬٠٧٣‏-");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberQLongLongWestern()
{
    QString localeName("de_CH");
    QString localeNameLcNumeric("de_CH");
    qlonglong number = qlonglong(-1542678073);
    QString formatted("-1'542'678'073");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberDoubleArabic()
{
    QString localeName("ar");
    QString localeNameLcNumeric("ar_EG@numbers=arab");
    double number = double(-1234567.1234567);
    QString formatted("‏١٬٢٣٤٬٥٦٧٫١٢٣‏-");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

#ifdef HAVE_ICU
void Pt_MLocale::benchmarkFormatNumberDoubleWestern()
{
    QString localeName("de_CH");
    QString localeNameLcNumeric("de_CH");
    double number = double(-1234567.1234567);
    QString formatted("-1'234'567.123");
    MLocale locale(localeName);
    locale.setCategoryLocale(MLocale::MLcNumeric, localeNameLcNumeric);
#if 0
    QTextStream stream(stdout);
    stream.setCodec("UTF-8");
    stream << " localeName=" << localeName
           << "\n localeNameLcNumeric=" << localeNameLcNumeric
           << "\n number=" << number
           << "\n locale.formatNumber(number)=" <<  locale.formatNumber(number)
           << "\n formatted=" << formatted;
    stream.flush();
#endif
    QCOMPARE(locale.formatNumber(number), formatted);
    QBENCHMARK {
        locale.formatNumber(number);
    }
}
#endif

QTEST_APPLESS_MAIN(Pt_MLocale);
