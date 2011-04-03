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

#include "ft_locales.h"

#define VERBOSE_OUTPUT

class TestLocale : public MLocale
{
};

void Ft_Locales::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "ft_locales" };
    qap = new MApplication(dummyArgc, dummyArgv, "test");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QProcess process;
    process.start("sh -c \"dpkg -s libqtcore4 | grep Version | perl -pe 's/^Version:[[:space:]]*([^[[:space:]]+)$/$1/g'\"");
    if (!process.waitForFinished()) {
        qDebug() << "cannot run process to check libqtcore4 package version , exiting ...";
        exit(1);
    }
    libqtcore4PackageVersion = process.readAllStandardOutput();
    libqtcore4PackageVersion.replace("\n", "");
    qDebug() << "libqtcore4 package version is:" << libqtcore4PackageVersion;
}

void Ft_Locales::cleanupTestCase()
{
    delete qap;
}

void Ft_Locales::init()
{
}

void Ft_Locales::cleanup()
{
}

void Ft_Locales::testBug210881()
{
    // see
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=210881
    MLocale locale;
    MLocale locale1;
    locale1.formatDateTime(QDateTime(),
                           MLocale::DateFull, MLocale::TimeFull,
                           MLocale::GregorianCalendar);
    locale1 = locale;
    locale1 = locale;
}

void Ft_Locales::testBug169305()
{
    // see
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=169305
    // and also:
    // https://projects.maemo.org/bugzilla/show_bug.cgi?id=184502
    MLocale locale0("en_US@layout-direction=auto");
    locale0.installTrCatalog("foo");
    MLocale::setDefault(locale0);
    QCOMPARE(QApplication::tr("QT_LAYOUT_DIRECTION"), QString("LTR"));
    QCOMPARE(qApp->layoutDirection(), Qt::LeftToRight);
    MLocale locale1("ar_SA@layout-direction=auto");
    locale1.installTrCatalog("foo");
    MLocale::setDefault(locale1);
    QCOMPARE(QApplication::tr("QT_LAYOUT_DIRECTION"), QString("RTL"));
    QCOMPARE(qApp->layoutDirection(), Qt::RightToLeft);
    MLocale locale2;
    QCOMPARE(locale2.name(), QString("ar_SA@layout-direction=auto"));
    locale2.installTrCatalog("foo");
    MLocale::setDefault(locale2);
    QCOMPARE(QApplication::tr("QT_LAYOUT_DIRECTION"), QString("RTL"));
    QCoreApplication::processEvents();
    QCOMPARE(QApplication::tr("QT_LAYOUT_DIRECTION"), QString("RTL"));
    QCOMPARE(qApp->layoutDirection(), Qt::RightToLeft);
    MLocale::setDefault(locale0);
    QCOMPARE(QApplication::tr("QT_LAYOUT_DIRECTION"), QString("LTR"));
    QCoreApplication::processEvents();
    QCOMPARE(QApplication::tr("QT_LAYOUT_DIRECTION"), QString("LTR"));
    QCOMPARE(qApp->layoutDirection(), Qt::LeftToRight);
}

void Ft_Locales::testMLocaleConstructor()
{
    TestLocale *testLocale = new TestLocale();
    QVERIFY2(testLocale->isValid(), "testLocale is not valid");
    testLocale->setCategoryLocale(MLocale::MLcNumeric, "de_CH");

    // copy constructor
    MLocale *mLocale = new MLocale(*testLocale);
    QVERIFY2(mLocale->isValid(), "mLocale is not valid");
    QCOMPARE(mLocale->name(), testLocale->name());
    QCOMPARE(mLocale->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));
    mLocale->setCategoryLocale(MLocale::MLcNumeric, "zh_CN");
    QVERIFY2(mLocale->categoryName(MLocale::MLcNumeric)
             != testLocale->categoryName(MLocale::MLcNumeric),
             "mLocale->categoryName(MLocale::MLcNumeric) and testLocale->categoryName(MLocale::MLcNumeric) should be different");
    // assignment operator
    *mLocale = * testLocale;
    QVERIFY2(mLocale->isValid(), "mLocale is not valid");
    QCOMPARE(mLocale->name(), testLocale->name());
    QCOMPARE(mLocale->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));

    // copy constructor
    TestLocale *testLocale2 = new TestLocale(*testLocale);
    QVERIFY2(testLocale2->isValid(), "testLocale2 is not valid");
    QCOMPARE(testLocale2->name(), testLocale->name());
    QCOMPARE(testLocale2->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));
    testLocale2->setCategoryLocale(MLocale::MLcNumeric, "zh_CN");
    QVERIFY2(testLocale2->categoryName(MLocale::MLcNumeric)
             != testLocale->categoryName(MLocale::MLcNumeric),
             "testLocale2->categoryName(MLocale::MLcNumeric) and testLocale->categoryName(MLocale::MLcNumeric) should be different");
    // assignment operator
    *testLocale2 = * testLocale;
    QVERIFY2(testLocale2->isValid(), "testLocale2 is not valid");
    QCOMPARE(testLocale2->name(), testLocale->name());
    QCOMPARE(testLocale2->categoryName(MLocale::MLcNumeric),
             testLocale->categoryName(MLocale::MLcNumeric));

    delete mLocale;
    delete testLocale;
    delete testLocale2;
}

void Ft_Locales::testCreateCLocale()
{
    MLocale cLocale = MLocale::createCLocale();
    MLocale en_US_POSIX_locale("en_US_POSIX");
    QCOMPARE(cLocale.name(), QString("en_US_POSIX"));
    QCOMPARE(en_US_POSIX_locale.name(), QString("en_US_POSIX"));
}

void Ft_Locales::testLocaleNameParsingFunctions_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("localeLanguage");
    QTest::addColumn<QString>("localeScript");
    QTest::addColumn<QString>("localeCountry");
    QTest::addColumn<QString>("localeVariant");

    QTest::newRow("en")
        << "en"
        << "en"
        << ""
        << ""
        << "";
    QTest::newRow("en_US")
        << "en_US"
        << "en"
        << ""
        << "US"
        << "";
    QTest::newRow("ru_RU")
        << "ru_RU"
        << "ru"
        << ""
        << "RU"
        << "";
    QTest::newRow("ru_Cyrl_RU")
        << "ru_Cyrl_RU"
        << "ru"
        << "Cyrl"
        << "RU"
        << "";
    QTest::newRow("en_IE_PREEURO")
        << "en_IE_PREEURO"
        << "en"
        << ""
        << "IE"
        << "PREEURO";
    QTest::newRow("eo")
        << "eo"
        << "eo"
        << ""
        << ""
        << "";
    QTest::newRow("fr_FR@collation=phonebook;calendar=islamic-civil")
        << "fr_FR@collation=phonebook;calendar=islamic-civil"
        << "fr"
        << ""
        << "FR"
        << "";
    QTest::newRow("fr@collation=phonebook;calendar=islamic-civil")
        << "fr@collation=phonebook;calendar=islamic-civil"
        << "fr"
        << ""
        << ""
        << "";
    QTest::newRow("sr_RS")
        << "sr_RS"
        << "sr"
        << ""
        << "RS"
        << "";
    QTest::newRow("sr_Latn_RS_REVISED")
        << "sr_Latn_RS_REVISED"
        << "sr"
        << "Latn"
        << "RS"
        << "REVISED";
    QTest::newRow("sr_Cyrl_RS_REVISED@currency=USD")
        << "sr_Cyrl_RS_REVISED@currency=USD"
        << "sr"
        << "Cyrl"
        << "RS"
        << "REVISED";
    QTest::newRow("es__TRADITIONAL")
        << "es__TRADITIONAL"
        << "es"
        << ""
        << ""
        << "TRADITIONAL";
}

void Ft_Locales::testLocaleNameParsingFunctions()
{
    QFETCH(QString, localeName);
    QFETCH(QString, localeLanguage);
    QFETCH(QString, localeScript);
    QFETCH(QString, localeCountry);
    QFETCH(QString, localeVariant);

    MLocale::Category category = MLocale::MLcNumeric;
    MLocale locale(localeName);
    QCOMPARE(locale.name(), localeName);
    QCOMPARE(locale.language(), localeLanguage);
    QCOMPARE(locale.script(), localeScript);
    QCOMPARE(locale.country(), localeCountry);
    QCOMPARE(locale.variant(), localeVariant);
    MLocale englishLocale("en_US");
    locale = englishLocale;
    locale.setCategoryLocale(category, localeName);
    QCOMPARE(locale.categoryName(category), localeName);
    QCOMPARE(locale.categoryLanguage(category), localeLanguage);
    QCOMPARE(locale.categoryScript(category), localeScript);
    QCOMPARE(locale.categoryCountry(category), localeCountry);
    QCOMPARE(locale.categoryVariant(category), localeVariant);
}

void Ft_Locales::testCreateSystemLocale_data()
{
    QTest::addColumn<QString>("conf");
    QTest::addColumn<QString>("env");
    QTest::addColumn<QString>("locale");

    // Test the ultimate fallback to POSIX
    QTest::newRow("posix") << QString("") << QString("") << QString("en_US_POSIX");
    // Test the fallback to the LANG environment variable. If the string
    // found via gconf is empty, the value of LANG is used as a fallback
    QTest::newRow("fi") << QString("") << QString("fi") << QString("fi");
    QTest::newRow("fi") << QString("") << QString("fi.UTF-8") << QString("fi");
    QTest::newRow("fi") << QString("") << QString("fi_FI") << QString("fi_FI");
    QTest::newRow("fi") << QString("") << QString("fi_FI.UTF-8") << QString("fi_FI");
    QTest::newRow("snd") << QString("") << QString("snd@Arab") << QString("snd_Arab");
    QTest::newRow("snd") << QString("") << QString("snd_AF@Arab") << QString("snd_Arab_AF");
    QTest::newRow("snd") << QString("") << QString("snd_AF.UTF-8@Arab") << QString("snd_Arab_AF");
    // with bad data
    QTest::newRow("snd") << QString("") << QString("+2eio") << QString("en_US_POSIX");
    // Test values found via gconf and check that LANG is ignored:
    QTest::newRow("fi") << QString("fi") << QString("en") << QString("fi");
}

bool confIsDown()
{
    MGConfItem languageItem("/meegotouch/i18n/language");
    QString originalValue = languageItem.value().toString();
    int skipConf = 0;

    if (originalValue.isEmpty()) {
        languageItem.set("xx");
        //GConf is not running here, so skip it
        if (languageItem.value().toString() != "xx") {
            skipConf = 1;
        } else {
            languageItem.set(originalValue);
        }
    }

    return skipConf == 1;
}

void Ft_Locales::testSettingsChanged()
{
    if (confIsDown()) {
        QSKIP("SettingsChanged is skipped", SkipSingle);
        return;
    }
    MGConfItem languageItem("/meegotouch/i18n/language");
    QString originalValue = languageItem.value().toString();

    // Test within MLocale
    MLocale z;
    QSignalSpy spy(&z, SIGNAL(settingsChanged()));
    QCOMPARE(spy.count(), 0);

    // Changes in languageItem should not trigger the signal
    // settingsChanged() as long as the locale settings are
    // disconnected:
    languageItem.set(originalValue + "something");
    QTest::qWait(100);
    QCOMPARE(spy.count(), 0);

    // After connecting, changes in languageItem should trigger the
    // signal settingsChanged()
    z.connectSettings();
    languageItem.set("fr");
    for (int i = 0; i < 100; ++i) {
        QTest::qWait(50);
        if (spy.count() != 0)
            break;
    }
    QCOMPARE(spy.count(), 1);

    languageItem.set("fi");
    for (int i = 0; i < 100; ++i) {
        QTest::qWait(50);
        if (spy.count() != 1)
            break;
    }
    QCOMPARE(spy.count(), 2);

    // After disconnecting, the signal should not be triggered anymore:
    z.disconnectSettings();
    languageItem.set("en");
    QTest::qWait(100);
    QCOMPARE(spy.count(), 2);

    // Test the signal localeSettingsChanged in MApplication
    QSignalSpy spyapp(qap, SIGNAL(localeSettingsChanged()));
    QCOMPARE(spyapp.count(), 0);

    languageItem.set(originalValue + "something else");
    for (int i = 0; i < 100; ++i) {
        QTest::qWait(50);
        if (spyapp.count() != 0)
            break;
    }
    QCOMPARE(spyapp.count(), 1);

    languageItem.set("fi");
    for (int i = 0; i < 100; ++i) {
        QTest::qWait(50);
        if (spyapp.count() != 1)
            break;
    }
    QCOMPARE(spyapp.count(), 2);

    languageItem.set(originalValue);
}

void Ft_Locales::testCreateSystemLocale()
{
    MGConfItem languageItem("/meegotouch/i18n/language");
    QString originalValue = languageItem.value().toString();

    QFETCH(QString, conf);
    QFETCH(QString, env);
    QFETCH(QString, locale);

    setenv("LANG", env.toAscii().data(), 1);

    languageItem.set(conf);
    MLocale *z = MLocale::createSystemMLocale();

    if (!originalValue.isEmpty()) {
        languageItem.set(originalValue);
    }

    if (confIsDown() && ! conf.isEmpty()) {
        QSKIP("CreateSystemLocale is skipped", SkipSingle);
    } else {
        QCOMPARE(z->name(), locale);
    }

    delete z;
}

void Ft_Locales::testMLocaleLanguage_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("language");

    QTest::newRow("fi") << QString("fi_FI") << QString("fi");
    QTest::newRow("snd") << QString("snd_Arab_AF") << QString("snd");
    QTest::newRow("en") << QString("en") << QString("en");
}

void Ft_Locales::testMLocaleLanguage()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, language);

    MLocale *z = new MLocale(locale_name);
    QCOMPARE(z->language(), language);
    delete z;
}

void Ft_Locales::testMLocaleCountry_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("country");

    QTest::newRow("fi") << QString("fi_FI") << QString("FI");
    QTest::newRow("snd") << QString("snd_Arab_AF") << QString("AF");
    QTest::newRow("snd") << QString("snd_Arab") << QString("");
    QTest::newRow("snd") << QString("snd_Arab_EG") << QString("EG");
    QTest::newRow("en") << QString("en") << QString("");
}

void Ft_Locales::testMLocaleCountry()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, country);

    MLocale *z = new MLocale(locale_name);

    QCOMPARE(z->country(), country);
    delete z;
}

void Ft_Locales::testMLocaleScript_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QString>("script");

    QTest::newRow("fi") << QString("fi_FI") << QString("");
    QTest::newRow("snd") << QString("snd_Arab_AF") << QString("Arab");
    QTest::newRow("snd") << QString("snd_Deva") << QString("Deva");
    QTest::newRow("en") << QString("en") << QString("");
    QTest::newRow("en_Arab") << QString("en_Arab") << QString("Arab");
    QTest::newRow("es__TRADITIONAL") << QString("es__TRADITIONAL") << QString("");
}

void Ft_Locales::testMLocaleScript()
{
    QFETCH(QString, locale);
    QFETCH(QString, script);

    MLocale *z = new MLocale(locale);
    QCOMPARE(z->script(), script);
    delete z;
}

void Ft_Locales::testMLocaleVariant_data()
{
    QTest::addColumn<QString>("locale");
    QTest::addColumn<QString>("variant");

    QTest::newRow("fi_FI") << QString("fi_FI") << QString("");
    QTest::newRow("sr_Cyrl_RS_REVISED@currency=USD") << QString("sr_Cyrl_RS_REVISED@currency=USD") << QString("REVISED");
    QTest::newRow("snd_AF_XXX") << QString("snd_AF_XXX") << QString("XXX");
    QTest::newRow("snd_Arab_AF_XXX") << QString("snd_Arab_AF_XXX") << QString("XXX");
    QTest::newRow("en") << QString("en") << QString("");
}

void Ft_Locales::testMLocaleVariant()
{
    QFETCH(QString, locale);
    QFETCH(QString, variant);

    MLocale *z = new MLocale(locale);
    QCOMPARE(z->variant(), variant);
    delete z;
}

void Ft_Locales::testMLocaleTextDirection_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<int>("direction");

    QTest::newRow("fi")
        << QString("fi")
        << (int) Qt::LeftToRight;
    QTest::newRow("fi@layout-direction=auto")
        << QString("fi@layout-direction=auto")
        << (int) Qt::LeftToRight;
    QTest::newRow("fi@layout-direction=ltr")
        << QString("fi@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("fi@layout-direction=rtl")
        << QString("fi@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("am")
        << QString("am")
        << (int) Qt::LeftToRight;
    QTest::newRow("am@layout-direction=auto")
        << QString("am@layout-direction=auto")
        << (int) Qt::LeftToRight;
    QTest::newRow("am@layout-direction=ltr")
        << QString("am@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("am@layout-direction=rtl")
        << QString("am@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("ar")
        << QString("ar")
        << (int) Qt::LeftToRight;
    QTest::newRow("ar@layout-direction=auto")
        << QString("ar@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("ar@layout-direction=ltr")
        << QString("ar@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("ar@layout-direction=rtl")
        << QString("ar@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("he")
        << QString("he")
        << (int) Qt::LeftToRight;
    QTest::newRow("he@layout-direction=auto")
        << QString("he@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("he@layout-direction=ltr")
        << QString("he@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("he@layout-direction=rtl")
        << QString("he@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("ur")
        << QString("ur")
        << (int) Qt::LeftToRight;
    QTest::newRow("ur@layout-direction=auto")
        << QString("ur@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("ur@layout-direction=ltr")
        << QString("ur@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("ur@layout-direction=rtl")
        << QString("ur@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("fa")
        << QString("fa")
        << (int) Qt::LeftToRight;
    QTest::newRow("fa@layout-direction=auto")
        << QString("fa@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("fa@layout-direction=ltr")
        << QString("fa@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("fa@layout-direction=rtl")
        << QString("fa@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("ps")
        << QString("ps")
        << (int) Qt::LeftToRight;
    QTest::newRow("ps@layout-direction=auto")
        << QString("ps@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("ps@layout-direction=ltr")
        << QString("ps@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("ps@layout-direction=rtl")
        << QString("ps@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF")
        << QString("snd_Arab_AF")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF@layout-direction=auto")
        << QString("snd_Arab_AF@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF@layout-direction=ltr")
        << QString("snd_Arab_AF@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF@layout-direction=rtl")
        << QString("snd_Arab_AF@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF_XXX")
        << QString("snd_Arab_AF_XXX")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF_XXX@layout-direction=auto")
        << QString("snd_Arab_AF_XXX@layout-direction=auto")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd_Arab_AF_XXX@layout-direction=ltr")
        << QString("snd_Arab_AF_XXX@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Arab_AF_XXX@layout-direction=rtl")
        << QString("snd_Arab_AF_XXX@layout-direction=rtl")
        << (int) Qt::RightToLeft;
    QTest::newRow("snd")
        << QString("snd_Deva_AF_XXX")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Deva_AF_XXX@layout-direction=auto")
        << QString("snd_Deva_AF_XXX@layout-direction=auto")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Deva_AF_XXX@layout-direction=ltr")
        << QString("snd_Deva_AF_XXX@layout-direction=ltr")
        << (int) Qt::LeftToRight;
    QTest::newRow("snd_Deva_AF_XXX@layout-direction=rtl")
        << QString("snd_Deva_AF_XXX@layout-direction=rtl")
        << (int) Qt::RightToLeft;
}

void Ft_Locales::testMLocaleTextDirection()
{
    QFETCH(QString, localeName);
    QFETCH(int, direction);

    MLocale defaultLocale("ar");
    MLocale::setDefault(defaultLocale);
    QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LeftToRight);

    MLocale locale(localeName);
    QCOMPARE((int)locale.textDirection(), direction);
    MLocale::setDefault(locale);
    if(localeName.contains(QRegExp("@.*layout-direction=auto")))
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LayoutDirectionAuto);
    else if(localeName.contains(QRegExp("@.*layout-direction=ltr")))
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LeftToRight);
    else if(localeName.contains(QRegExp("@.*layout-direction=rtl")))
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::RightToLeft);
    else
        QCOMPARE(MLocale::defaultLayoutDirection(), Qt::LeftToRight);
}

void Ft_Locales::testMLocaleConstructorWithParams_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("country");
    QTest::addColumn<QString>("name");

    QTest::newRow("fi_FI") << QString("fi") << QString("FI") << QString("fi_FI");
    QTest::newRow("ar_SA") << QString("ar") << QString("SA") << QString("ar_SA");
    QTest::newRow("xx_YY") << QString("xx") << QString("YY") << QString("xx_YY");
}

void Ft_Locales::testMLocaleConstructorWithParams()
{
    QFETCH(QString, language);
    QFETCH(QString, country);
    QFETCH(QString, name);

    MLocale *z = new MLocale(language + '_' +  country);
    QVERIFY2(z->isValid(),
             "new MLocale(language + '_' + country)");
    QVERIFY2(z->name() == name, "z->name() differs from name");
    delete z;

    z = new MLocale(name);
    QVERIFY2(z->isValid(), "new MLocale(name)");
    QVERIFY2(z->name() == name, "z->name() differs from name");
    delete z;
}

void Ft_Locales::testMLocaleConstructorAndCategoryWithParams_data()
{
    QTest::addColumn<QString>("default_language");
    QTest::addColumn<QString>("default_country");
    QTest::addColumn<QString>("default_name");

    QTest::addColumn<QString>("category_language");
    QTest::addColumn<QString>("category_country");
    QTest::addColumn<QString>("category_name");

    QTest::newRow("fi_FI")
            << QString("fi")  << QString("FI") << QString("fi_FI")
            << QString("en")  << QString("GB") << QString("en_GB");
    QTest::newRow("ar_SA")
            << QString("ar")  << QString("SA") << QString("ar_SA")
            << QString("en")  << QString("GB") << QString("en_GB");
    QTest::newRow("xx_YY")
            << QString("xx")  << QString("YY") << QString("xx_YY")
            << QString("aa")  << QString("BB") << QString("aa_BB");
}

void Ft_Locales::testMLocaleConstructorAndCategoryWithParams()
{
    QFETCH(QString, default_language);
    QFETCH(QString, default_country);
    QFETCH(QString, default_name);

    QFETCH(QString, category_language);
    QFETCH(QString, category_country);
    QFETCH(QString, category_name);

    MLocale *z = 0;

    z = new MLocale(default_language + '_' + default_country);

    QVERIFY2(z->isValid(),
             "new MLocale(default_language + '_' + default_country)");
    QVERIFY2(z->name() == default_name, "z->name() differs from default_name");

    QVERIFY2(z->categoryName(MLocale::MLcTime) == default_name,
             "TIME: category name does not point to default category");
    z->setCategoryLocale(MLocale::MLcTime, category_language + '_' + category_country);
    QVERIFY2(z->categoryName(MLocale::MLcTime) == category_name,
             "TIME: category name does not point to category_name");

    QVERIFY2(z->categoryName(MLocale::MLcCollate) == default_name,
             "COLLATE: category name does not point to default category");

    z->setCategoryLocale(MLocale::MLcCollate, category_language + '_' + category_country);
    QVERIFY2(z->categoryName(MLocale::MLcCollate) == category_name,
             "COLLATE: category name does not point to category_name");

    QVERIFY2(z->categoryName(MLocale::MLcNumeric) == default_name,
             "NUMERIC: category name does not point to default category");

    z->setCategoryLocale(MLocale::MLcNumeric, category_language + '_' +  category_country);
    QVERIFY2(z->categoryName(MLocale::MLcNumeric) == category_name,
             "NUMERIC: category name does not point to category_name");
    delete z;
}

void Ft_Locales::testMLocaleLanguageEndonym_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("endonym_result");

    QTest::newRow("ar")
            << QString("ar")
            << QString("العربية");
    QTest::newRow("zh")
            << QString("zh")
            << QString("中文 (中国大陆)");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("中文 (中国大陆)");
    QTest::newRow("zh_HK")
            << QString("zh_HK")
            << QString("繁體中文 (香港)");
    QTest::newRow("zh_TW")
            << QString("zh_TW")
            << QString("中文 (台灣)");
    QTest::newRow("cs")
            << QString("cs")
            << QString("čeština");
    QTest::newRow("da")
            << QString("da")
            << QString("dansk");
    QTest::newRow("nl")
            << QString("nl")
            << QString("Nederlands");
    QTest::newRow("en")
            << QString("en")
            << QString("English (United Kingdom)");
    QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("English (United Kingdom)");
    QTest::newRow("en_US")
            << QString("en_US")
            << QString("English (United States)");
    QTest::newRow("en_AU")
            << QString("en_AU")
            << QString("English (Australia)");
    QTest::newRow("fi")
            << QString("fi")
            << QString("suomi");
    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("suomi");
    QTest::newRow("fr")
            << QString("fr")
            << QString("français (France)");
    QTest::newRow("fr_FR")
            << QString("fr_FR")
            << QString("français (France)");
    QTest::newRow("fr_CA")
            << QString("fr_CA")
            << QString("français (Canada)");
    QTest::newRow("de")
            << QString("de")
            << QString("Deutsch");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutsch");
    // de_CH needs a special fix because of
    // see http://site.icu-project.org/design/resbund/issues
    // make sure that the fix works:
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("Schweizer Hochdeutsch");
    QTest::newRow("de_AT")
            << QString("de_AT")
            << QString("Österreichisches Deutsch");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutsch");
    QTest::newRow("hu")
            << QString("hu")
            << QString("magyar");
    QTest::newRow("hu_HU")
            << QString("hu_HU")
            << QString("magyar");
    QTest::newRow("id")
            << QString("id")
            << QString("Indonesia");
    QTest::newRow("id_ID")
            << QString("id_ID")
            << QString("Indonesia");
    QTest::newRow("it")
            << QString("it")
            << QString("italiano");
    QTest::newRow("it_IT")
            << QString("it_IT")
            << QString("italiano");
    QTest::newRow("ms")
            << QString("ms")
            << QString("Melayu");
    QTest::newRow("ms_BN")
            << QString("ms_BN")
            << QString("Melayu");
    QTest::newRow("ms_MY")
            << QString("ms_MY")
            << QString("Melayu");
    QTest::newRow("no")
            << QString("no")
            << QString("norsk");
    QTest::newRow("no_NO")
            << QString("no_NO")
            << QString("norsk");
    QTest::newRow("nb")
            << QString("nb")
            << QString("norsk bokmål");
    QTest::newRow("nb_NO")
            << QString("nb_NO")
            << QString("norsk bokmål");
    QTest::newRow("pl")
            << QString("pl")
            << QString("polski");
    QTest::newRow("pl_PL")
            << QString("pl_PL")
            << QString("polski");
    QTest::newRow("pt")
            << QString("pt")
            << QString("português (Portugal)");
    QTest::newRow("pt_PT")
            << QString("pt_PT")
            << QString("português (Portugal)");
    QTest::newRow("pt_BR")
            << QString("pt_BR")
            << QString("português (Brasil)");
    QTest::newRow("ro")
            << QString("ro")
            << QString("română");
    QTest::newRow("ro_MD")
            << QString("ro_MD")
            << QString("română");
    QTest::newRow("ro_RO")
            << QString("ro_RO")
            << QString("română");
    QTest::newRow("ru")
            << QString("ru")
            << QString("русский");
    QTest::newRow("ru_RU")
            << QString("ru_RU")
            << QString("русский");
    QTest::newRow("ru_UA")
            << QString("ru_UA")
            << QString("русский");
    QTest::newRow("sk")
            << QString("sk")
            << QString("slovenčina");
    QTest::newRow("sk_SK")
            << QString("sk_SK")
            << QString("slovenčina");
    QTest::newRow("es")
            << QString("es")
            << QString("español (España)");
    QTest::newRow("es_419")
            << QString("es_419")
            << QString("español (América Latina)");
    QTest::newRow("sv")
            << QString("sv")
            << QString("svenska");
    QTest::newRow("sv_FI")
            << QString("sv_FI")
            << QString("svenska");
    QTest::newRow("sv_SE")
            << QString("sv_SE")
            << QString("svenska");
    QTest::newRow("tr")
            << QString("tr")
            << QString("Türkçe");
    QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("Türkçe");
    QTest::newRow("uk")
            << QString("uk")
            << QString("українська");
    QTest::newRow("uk_UA")
            << QString("uk_UA")
            << QString("українська");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("日本語");
}

void Ft_Locales::testMLocaleLanguageEndonym()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, endonym_result);
    MLocale locale(locale_name);
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "locale name: " << locale_name
        << " result: " << locale.languageEndonym()
        << " expected: " << endonym_result
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(locale.languageEndonym(), endonym_result);
}

void Ft_Locales::testMLocaleCountryEndonym_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("endonym_result");

    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("Suomi");
    // de_CH needs a special fix because of
    // see http://site.icu-project.org/design/resbund/issues
    // make sure that the fix works:
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("Schweiz");
    QTest::newRow("de_AT")
            << QString("de_AT")
            << QString("Österreich");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutschland");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("日本");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("中国");
}

void Ft_Locales::testMLocaleCountryEndonym()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, endonym_result);
    MLocale locale(locale_name);
    QCOMPARE(locale.countryEndonym(), endonym_result);
}

void Ft_Locales::testMLocaleLocaleScripts_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QStringList>("localeScripts");

    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << (QStringList () << "Latn");
    QTest::newRow("sr_RS")
            << QString("sr_RS")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_RS")
            << QString("sr_Cyrl_RS")
            << (QStringList () << "Cyrl");
    QTest::newRow("sr_Latn_RS")
            << QString("sr_Latn_RS")
            << (QStringList () << "Latn");
    QTest::newRow("zh_TW")
            << QString("zh_TW")
            << (QStringList () << "Hani" << "Bopo");
    QTest::newRow("zh_Hant_TW")
            << QString("zh_Hant_TW")
            << (QStringList () << "Hani" << "Bopo");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << (QStringList () << "Kana" << "Hira" << "Hani");
}

void Ft_Locales::testMLocaleLocaleScripts()
{
    QFETCH(QString, localeName);
    QFETCH(QStringList, localeScripts);
    MLocale locale(localeName);
    QCOMPARE(locale.localeScripts(), localeScripts);
}

void Ft_Locales::testMLocaleToLower_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("testString");
    QTest::addColumn<QString>("expectedMLocale");
    QTest::addColumn<QString>("expectedQt");

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("Größe")
        << QString("größe")
        << QString("größe")
        ;

    QTest::newRow("de_CH")
        << QString("de_CH") // no difference in behaviour to de_CH here
        << QString("Größe")
        << QString("größe")
        << QString("größe")
        ;

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("GRÖẞE")
            << QString("größe")
            << QString("größe") // works because Qt’s toLower uses libicu now
            ;
    }
    else {
        QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("GRÖẞE")
            << QString("größe")
            << QString("gröẞe") // Qt bug?
            ;
    }

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("de_CH") // no difference in behaviour to de_CH here
            << QString("de_CH")
            << QString("GRÖẞE")
            << QString("größe")
            << QString("größe") // works because Qt’s toLower uses libicu now
            ;
    }
    else {
        QTest::newRow("de_CH") // no difference in behaviour to de_CH here
            << QString("de_CH")
            << QString("GRÖẞE")
            << QString("größe")
            << QString("gröẞe") // Qt bug?
            ;
    }

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("ITEM NAÏVE İ")
        // last two chars are U+0069 (LATIN SMALL LETTER I) U+307 (COMBINING DOT ABOVE)
        // both for Qt and MLocale here
        << QString("item naïve i̇")
        << QString("item naïve i̇")
        ;

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("ITEM NAÏVE İ")
            // I lowercases to ı in Turkish locale and İ to i
            << QString("ıtem naïve i")
            // Qt’s toLower uses libicu now and thus works
            << QString("ıtem naïve i")
            ;
    }
    else {
        QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("ITEM NAÏVE İ")
            // I lowercases to ı in Turkish locale and İ to i
            << QString("ıtem naïve i")
            // Qt’s toLower is *not* locale aware
            // last two chars are U+0069 (LATIN SMALL LETTER I) U+307 (COMBINING DOT ABOVE)
            << QString("item naïve i̇")
            ;
    }

    QTest::newRow("en_GB")
        << QString("de_GB")
        << QString("ΣΙ")
        << QString("σι")
        << QString("σι")
        ;

    QTest::newRow("el_GR")
        << QString("el_GR")
        << QString("ΣΙ")
        << QString("σι")
        << QString("σι")
        ;

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("ΙΣ")
            << QString("ις")
            // Qt’s toLower is now context aware and lowercases the final Σ
            // correctly using ICU
            << QString("ις")
            ;
    }
    else {
        QTest::newRow("en_GB")
            << QString("en_GB")
            << QString("ΙΣ")
            << QString("ις")
            // Qt’s toLower is *not* context aware and lowercases the final Σ
            // the same way as a non-final one:
            << QString("ισ")
            ;
    }

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("el_GR")
            << QString("el_GR")
            << QString("ΙΣ")
            << QString("ις")
            // Qt’s toLower is now context aware and lowercases the final Σ
            // correctly using ICU
            << QString("ις")
            ;
    }
    else {
        QTest::newRow("el_GR")
            << QString("el_GR")
            << QString("ΙΣ")
            << QString("ις")
            // Qt’s toLower is *not* context aware and lowercases the final Σ
            // the same way as a non-final one:
            << QString("ισ")
            ;
    }

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("Ì") // U+00CC (LATIN CAPITAL LETTER I WITH GRAVE)
        << QString("ì") // U+00EC (LATIN SMALL LETTER I WITH GRAVE)
        << QString("ì") // U+00EC (LATIN SMALL LETTER I WITH GRAVE)
        ;

    QTest::newRow("lt_LT")
        << QString("lt_LT")
        << QString("Ì") // U+00CC (LATIN CAPITAL LETTER I WITH GRAVE)
        << QString("i̇̀") // U+0069 U+0307 U+0300
        << QString("ì") // U+00EC (LATIN SMALL LETTER I WITH GRAVE)
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("Į") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK)
        << QString("į") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        << QString("į") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        ;

    QTest::newRow("lt_LT")
        << QString("lt_LT")
        << QString("Į") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK)
        << QString("į") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        << QString("į") // U+012F (LATIN SMALL LETTER I WITH OGONEK)
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("Į̃") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK) U+0303 (COMBINING TILDE)
        << QString("į̃") // U+012F U+0303
        << QString("į̃") // U+012F U+0303
        ;

    QTest::newRow("lt_LT")
        << QString("lt_LT")
        << QString("Į̃") // U+012E (LATIN CAPITAL LETTER I WITH OGONEK) U+0303 (COMBINING TILDE)
        << QString("į̇̃") // U+012F U+0307 U+0303
        << QString("į̃") // U+012F U+0303
        ;
}

void Ft_Locales::testMLocaleToLower()
{
    QFETCH(QString, localeName);
    QFETCH(QString, testString);
    QFETCH(QString, expectedMLocale);
    QFETCH(QString, expectedQt);

    MLocale locale(localeName);
    QString resultMLocale = locale.toLower(testString);
    // MLocale::setDefault() calls QLocale::setDefault() (using McLcNumeric!)
    MLocale::setDefault(locale);
    QString resultQt = testString.toLower();

    QVector<uint> testStringUcs4 = testString.toUcs4();
    QString testStringCodePoints;
    for (int i = 0; i < testStringUcs4.size(); ++i) {
        testStringCodePoints += " U+" + QString::number(testStringUcs4[i],16);
    }
    QVector<uint> expectedMLocaleUcs4 = expectedMLocale.toUcs4();
    QString expectedMLocaleCodePoints;
    for (int i = 0; i < expectedMLocaleUcs4.size(); ++i) {
        expectedMLocaleCodePoints += " U+" + QString::number(expectedMLocaleUcs4[i],16);
    }
    QVector<uint> expectedQtUcs4 = expectedQt.toUcs4();
    QString expectedQtCodePoints;
    for (int i = 0; i < expectedQtUcs4.size(); ++i) {
        expectedQtCodePoints += " U+" + QString::number(expectedQtUcs4[i],16);
    }
    QVector<uint> resultMLocaleUcs4 = resultMLocale.toUcs4();
    QString resultMLocaleCodePoints;
    for (int i = 0; i < resultMLocaleUcs4.size(); ++i) {
        resultMLocaleCodePoints += " U+" + QString::number(resultMLocaleUcs4[i],16);
    }
    QVector<uint> resultQtUcs4 = resultQt.toUcs4();
    QString resultQtCodePoints;
    for (int i = 0; i < resultQtUcs4.size(); ++i) {
        resultQtCodePoints += " U+" + QString::number(resultQtUcs4[i],16);
    }
#if 1
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "localeName      [" << localeName      << "]\n"
        << "testString      [" << testString      << "]" << testStringCodePoints << "\n"
        << "expectedMLocale [" << expectedMLocale << "]" << expectedMLocaleCodePoints <<"\n"
        << "resultMLocale   [" << resultMLocale   << "]" << resultMLocaleCodePoints << "\n"
        << "expectedQt      [" << expectedQt      << "]" << expectedQtCodePoints << "\n"
        << "resultQt        [" << resultQt        << "]" << resultQtCodePoints << "\n"
        ;
#endif
    QCOMPARE(resultMLocale, expectedMLocale);
    QCOMPARE(resultQt, expectedQt);
}

void Ft_Locales::testMLocaleToUpper_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("testString");
    QTest::addColumn<QString>("expectedMLocale");
    QTest::addColumn<QString>("expectedQt");

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("Größe")
        << QString("GRÖSSE")
        << QString("GRÖSSE")
        ;

    QTest::newRow("de_CH")
        << QString("de_CH") // no difference in behaviour to de_CH here
        << QString("Größe")
        << QString("GRÖSSE")
        << QString("GRÖSSE")
        ;

    QTest::newRow("de_DE")
        << QString("de_DE")
        << QString("GRÖẞE")
        << QString("GRÖẞE")
        << QString("GRÖẞE")
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("item naïve ı")
        << QString("ITEM NAÏVE I")
        << QString("ITEM NAÏVE I")
        ;

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("item naïve ı")
            << QString("İTEM NAÏVE I")
            << QString("İTEM NAÏVE I") // works because Qt’s toUpper uses libicu now
            ;
    }
    else {
        QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("item naïve ı")
            << QString("İTEM NAÏVE I")
            << QString("ITEM NAÏVE I")
            ;
    }

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("i̇") // U+0069 (LATIN SMALL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        ;

    if(libqtcore4PackageVersion >= "4.7.2~git20110119") { // Qt uses libicu now
        QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("i̇") // U+0069 (LATIN SMALL LETTER I) U+0307  (COMBINING DOT ABOVE)
            << QString("İ̇") // U+0130 (LATIN CAPITAL LETTER I WITH DOT ABOVE) U+0307  (COMBINING DOT ABOVE)
            // works because Qt’s toUpper uses libicu now
            << QString("İ̇") // U+0130 (LATIN CAPITAL LETTER I WITH DOT ABOVE) U+0307  (COMBINING DOT ABOVE)
            ;
    }
    else {
        QTest::newRow("tr_TR")
            << QString("tr_TR")
            << QString("i̇") // U+0069 (LATIN SMALL LETTER I) U+0307  (COMBINING DOT ABOVE)
            << QString("İ̇") // U+0130 (LATIN CAPITAL LETTER I WITH DOT ABOVE) U+0307  (COMBINING DOT ABOVE)
            << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
            ;
    }

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("ı̇") // U+0131 (LATIN SMALL LETTER DOTLESS I) U+0307  (COMBINING DOT ABOVE)
        << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        ;

    QTest::newRow("tr_TR")
        << QString("tr_TR")
        << QString("ı̇") // U+0131 (LATIN SMALL LETTER DOTLESS I) U+0307  (COMBINING DOT ABOVE)
        << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        << QString("İ") // U+0049 (LATIN CAPITAL LETTER I) U+0307  (COMBINING DOT ABOVE)
        ;

    QTest::newRow("en_GB")
        << QString("en_GB")
        << QString("ﬃ")
        << QString("FFI")
        << QString("FFI")
        ;
}

void Ft_Locales::testMLocaleToUpper()
{
    QFETCH(QString, localeName);
    QFETCH(QString, testString);
    QFETCH(QString, expectedMLocale);
    QFETCH(QString, expectedQt);

    MLocale locale(localeName);
    QString resultMLocale = locale.toUpper(testString);
    // MLocale::setDefault() calls QLocale::setDefault() (using McLcNumeric!)
    MLocale::setDefault(locale);
    QString resultQt = testString.toUpper();

    QVector<uint> testStringUcs4 = testString.toUcs4();
    QString testStringCodePoints;
    for (int i = 0; i < testStringUcs4.size(); ++i) {
        testStringCodePoints += " U+" + QString::number(testStringUcs4[i],16);
    }
    QVector<uint> expectedMLocaleUcs4 = expectedMLocale.toUcs4();
    QString expectedMLocaleCodePoints;
    for (int i = 0; i < expectedMLocaleUcs4.size(); ++i) {
        expectedMLocaleCodePoints += " U+" + QString::number(expectedMLocaleUcs4[i],16);
    }
    QVector<uint> expectedQtUcs4 = expectedQt.toUcs4();
    QString expectedQtCodePoints;
    for (int i = 0; i < expectedQtUcs4.size(); ++i) {
        expectedQtCodePoints += " U+" + QString::number(expectedQtUcs4[i],16);
    }
    QVector<uint> resultMLocaleUcs4 = resultMLocale.toUcs4();
    QString resultMLocaleCodePoints;
    for (int i = 0; i < resultMLocaleUcs4.size(); ++i) {
        resultMLocaleCodePoints += " U+" + QString::number(resultMLocaleUcs4[i],16);
    }
    QVector<uint> resultQtUcs4 = resultQt.toUcs4();
    QString resultQtCodePoints;
    for (int i = 0; i < resultQtUcs4.size(); ++i) {
        resultQtCodePoints += " U+" + QString::number(resultQtUcs4[i],16);
    }
#if 1
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "localeName      [" << localeName      << "]\n"
        << "testString      [" << testString      << "]" << testStringCodePoints << "\n"
        << "expectedMLocale [" << expectedMLocale << "]" << expectedMLocaleCodePoints <<"\n"
        << "resultMLocale   [" << resultMLocale   << "]" << resultMLocaleCodePoints << "\n"
        << "expectedQt      [" << expectedQt      << "]" << expectedQtCodePoints << "\n"
        << "resultQt        [" << resultQt        << "]" << resultQtCodePoints << "\n"
        ;
#endif
    QCOMPARE(resultMLocale, expectedMLocale);
    QCOMPARE(resultQt, expectedQt);
}


void Ft_Locales::testMLocaleIndexBucket_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("lcCollate");
    QTest::addColumn<QStringList>("stringsSorted");
    QTest::addColumn<QStringList>("expectedBuckets");

    QTest::newRow("en_US")
        <<"ja_JP"
        <<"en_US"
        <<(QStringList()
           <<"ç"<<"Ç"<<"cote"<<"coté"<<"côte"<<"côté"
           <<"f"<<"F"<<"ff"<<"ﬀ"<<"Ff"<<"ffi"<<"ﬃ"<<"Ffi"<<"ﬄ"<<"ﬁ"<<"ﬂ"
           <<"i"<<"I"<<"ï"<<"Ï"<<"İ"<<"ı"
           <<"ö"<<"Ö"<<"öe"<<"Öe"<<"ÖE"
           <<"s"<<"S"<<"ſ"<<"ß"<<"ẞ"
           <<"test"<<"Test"
           <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
           <<"沙紀"
            )
        <<(QStringList()
           <<"C"<<"C"<<"C"<<"C"<<"C"<<"C"
           <<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"
           <<"I"<<"I"<<"I"<<"I"<<"I"<<"I"
           <<"O"<<"O"<<"O" <<"O" <<"O"
           <<"S"<<"S"<<"S"<<"S"<<"S"
           <<"T"<<"T"
           <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
           <<"沙"
            );
    QTest::newRow("de_DE")
        <<"ja_JP"
        <<"de_DE"
        <<(QStringList()
           <<"ç"<<"Ç"<<"cote"<<"coté"<<"côte"<<"côté"
           <<"f"<<"F"<<"ff"<<"ﬀ"<<"Ff"<<"ffi"<<"ﬃ"<<"Ffi"<<"ﬄ"<<"ﬁ"<<"ﬂ"
           <<"i"<<"I"<<"ï"<<"Ï"<<"İ"<<"ı"
           <<"ö"<<"Ö"<<"öe"<<"Öe"<<"ÖE"
           <<"s"<<"S"<<"ſ"<<"ß"<<"ẞ"
           <<"test"<<"Test"
           <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
           <<"沙紀"
            )
        <<(QStringList()
           <<"C"<<"C"<<"C"<<"C"<<"C"<<"C"
           <<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"
           <<"I"<<"I"<<"I"<<"I"<<"I"<<"I"
           <<"O"<<"O"<<"O" <<"O" <<"O"
           <<"S"<<"S"<<"S"<<"S"<<"S"
           <<"T"<<"T"
           <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
           <<"沙"
            );
    QTest::newRow("tr_TR")
        <<"ja_JP"
        <<"tr_TR"
        <<(QStringList()
           <<"cote"<<"coté"<<"côte"<<"côté"
           <<"ç"<<"Ç"
           <<"ı"<<"I"<<"i"<<"İ"
           <<"ö"<<"Ö"<<"öe"<<"Öe"<<"ÖE"
           <<"s"<<"S"<<"ß"<<"ẞ"
           <<"test"<<"Test"
           <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
           <<"沙紀")
        <<(QStringList()
           <<"C"<<"C"<<"C"<<"C"
           <<"Ç"<<"Ç"
           <<"I"<<"I"<<"İ"<<"İ"
           <<"Ö"<<"Ö"<<"Ö"<<"Ö"<<"Ö"
           <<"S"<<"S"<<"S"<<"S"
           <<"T"<<"T"
           <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
           <<"沙");
    QTest::newRow("fr_CA")
        <<"ja_JP"
        <<"fr_CA"
        <<(QStringList()
           <<"cote"<<"côte"<<"coté"<<"côté"
           <<"ö"<<"Ö"<<"öe"<<"Öe"<<"ÖE"
           <<"z"<<"Z"<<"zx"<<"Zx"<<"ZX"
           <<"沙紀")
        <<(QStringList()
           <<"C"<<"C"<<"C"<<"C"
           <<"O"<<"O"<<"O"<<"O"<<"O"
           <<"Z"<<"Z"<<"Z"<<"Z"<<"Z"
           <<"沙");
    QTest::newRow("cs_CZ")
        <<"ja_JP"
        <<"cs_CZ"
        <<(QStringList()
           <<"c"<<"Cc"<<"CC"
           <<"č"<<"č"<<"Č"
           << "Ěrčk" << "Etsk" << "Ézfd"
           <<"h"<<"H"
           <<"ch"<<"cH"<<"Ch"<<"CH"
           <<"i"<<"I"
           <<"ů"<<"Ů"
           <<"α"<<"Α"
           <<"ワタシ"
           <<"沙紀")
        <<(QStringList()
           <<"C"<<"C"<<"C"
           <<"Č"<<"Č"<<"Č"
           <<"E"<<"E"<<"E"
           <<"H"<<"H"
           <<"CH"<<"CH"<<"CH"<<"CH"
           <<"I"<<"I"
           <<"U"<<"U"
           <<"Α"<<"Α"
           <<"ワ"
           <<"沙");
    QTest::newRow("ru_RU")
        <<"ja_JP"
        <<"ru_RU"
        <<(QStringList()
           <<"c"<<"č"<<"č"
           <<"Č"
           <<"Cc"<<"CC"<<"ch"<<"cH"<<"Ch"<<"CH"
           <<"h"<<"H"
           <<"i"<<"I"
           <<"ů"<<"Ů"
           <<"α"<<"Α"
           <<"дерево"<<"Дерево"
           <<"еж"<<"Еж"<<"ёж"<<"Ёж"<<"ежи"<<"Ежи"<<"елка"<<"Елка"<<"ёлка"<<"Ёлка"<<"ель"<<"Ель"
           <<"живот"<<"Живот"
           <<"идти"<<"Идти"
           <<"й"<<"Й"
           <<"школа"<<"Школа"
           <<"Ъ"
           <<"Ы"<<"Ь"
           <<"ワタシ"
           <<"沙紀")
        <<(QStringList()
           <<"C"<<"C"<<"C"
           <<"Č"
           <<"C"<<"C"<<"C"<<"C"<<"C"<<"C"
           <<"H"<<"H"
           <<"I"<<"I"
           <<"Ů"<<"Ů"
           <<"Α"<<"Α"
           <<"Д"<<"Д"
           <<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"<<"Ё"
           <<"Ж"<<"Ж"
           <<"И"<<"И"
           <<"Й"<<"Й"
           <<"Ш"<<"Ш"
           <<"Щ"
           <<"Ы"<<"Ы"
           <<"ワ"
           <<"沙");
    QTest::newRow("ar_SA")
        <<"ja_JP"
        <<"ar_SA"
        <<(QStringList()
           <<"arabia"
           <<"zero"
           <<"ه")
        <<(QStringList()
           <<"A"
           <<"Z"
           <<"ه");
    QTest::newRow("fa_IR")
        <<"ja_JP"
        <<"fa_IR"
        <<(QStringList()
           <<"ه")
        <<(QStringList()
           <<"ه");
    QTest::newRow("ja_JP@collation=standard")
        <<"de_DE"
        <<"ja_JP@collation=standard" // collation=standard is default for ja_JP
        <<(QStringList()
           <<"・"<<"﹅"<<"㈠"<<"㈱"<<"※"<<"∮"<<"♨"<<"〒"<<"ー"<<"ヾ"
           <<"Ａ"<<"john"<<"John"<<"saki"<<"Saki"
           <<"あ"<<"㋐"<<"い"<<"イ"<<"ゔ"<<"ヴ"<<"ゕ"<<"ヵ"
           <<"か"<<"㋕"<<"ㇰ"<<"く"<<"ク"<<"ゖ"<<"ヶ"<<"㋘"<<"こ"<<"ご"<<"ご"<<"ヿ"
           <<"さ"<<"㋚"<<"さき"<<"サキ"<<"じょん"<<"ジョン"<<"せ"<<"セ"<<"せ1"<<"セ1"<<"せ2"<<"セ2"<<"セあセ"<<"せいせ"
           <<"た"<<"㋟"<<"っ"<<"ッ"<<"と"<<"ト"
           <<"な"<<"ㇵ"
           <<"は"<<"㋩"
           <<"ま"
           <<"や"
           <<"ら"
           <<"ヷ"<<"わたし"<<"ワタシ"<<"ゐ"<<"ヰ"<<"ヸ"<<"ヹ"<<"を"<<"ヲ"<<"ヺ"
           <<"ん"<<"ン"
           <<"ㄅ"
           <<"ㄎ"
           <<"一"<<"沙紀"<<"丁"<<"頻"<<"隆"<<"廳"<<"⼀"<<"㊀"<<"㆒"<<"龢"<<"㺃"<<"䵷")
        <<(QStringList()
           <<"・"<<"﹅"<<"㈠"<<"㈱"<<"※"<<"∮"<<"♨"<<"〒"<<"ー"<<"ヾ"
           <<"Ａ"<<"J"<<"J"<<"S"<<"S"
           <<"あ"<<"あ"<<"あ"<<"あ"<<"あ"<<"あ"<<"あ"<<"あ"
           <<"か"<<"か"<<"か"<<"か"<<"か"<<"か"<<"か"<<"か"<<"か"<<"か"<<"か"<<"か"
           <<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"<<"さ"
           <<"た"<<"た"<<"た"<<"た"<<"た"<<"た"
           <<"な"<<"な"
           <<"は"<<"は"
           <<"ま"
           <<"や"
           <<"ら"
           <<"わ"<<"わ"<<"わ"
           <<"わ"<<"わ"<<"わ"<<"わ"<<"わ"<<"わ"<<"わ"
           <<"ん"<<"ん"
           <<"ㄅ"
           <<"ㄎ"
           <<"一"<<"沙"<<"丁"<<"頻"<<"隆"<<"廳"<<"⼀"<<"㊀"<<"㆒"<<"龢"<<"㺃"<<"䵷");
    QTest::newRow("ko_KR@collation=standard")
        <<"ja_JP"
        <<"ko_KR@collation=standard"
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"
           <<"Ａ"
           <<"f"<<"F"<<"ff"<<"ﬀ"<<"Ff"<<"ffi"<<"ﬃ"<<"Ffi"<<"ﬄ"<<"ﬁ"<<"ﬂ"
           <<"john"
           <<"ｊｏｈｎ"
           <<"John"
           <<"Ｊｏｈｎ"
           <<"saki"
           <<"ｓａｋｉ"
           <<"Saki"
           <<"Ｓａｋｉ"
           <<"α"<<"Α"
           <<"дерево"<<"Дерево"
           <<"א"
           <<"ת"
           <<"ا"
           <<"ي"
           <<"अ"
           <<"ह"
           <<"ཀ"
           <<"ཧ"

           <<"ᄀ" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"교" // U+AD50 HANGUL SYLLABLE GYO
           <<"校" // Korean Pronunciation: KYO HYO
           <<"君" // Korean Pronunciation: KWUN

           <<"ᄂ" // U+1102 HANGUL CHOSEONG NIEUN

           <<"ᄃ" // U+1103 HANGUL CHOSEONG TIKEUT
           <<"대"
           <<"大"

           <<"ᄅ" // U+1105 HANGUL CHOSEONG RIEUL
           <<"柳" // Korean Pronunciation: LYU
           <<"李" // Korean Pronunciation: LI

           <<"ᄆ" // U+1106 HANGUL CHOSEONG MIEUM

           <<"ᄇ" // U+1107 HANGUL CHOSEONG PIEUP
           <<"부"
           <<"釜"

           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS
           <<"산" // U+C0B0 HANGUL SYLLABLE SAN
           <<"山" // Korean Pronunciation: SAN
           <<"峠" // Korean Pronunciation: SANG
           <<"식" // U+C2DD HANGUL SYLLABLE SIG
           <<"신" // U+C2E0 HANGUL SYLLABLE SIN

           <<"ᄋ" // U+110B HANGUL CHOSEONG IEUNG
           <<"阿" // Korean Pronunciation: A OK
           <<"威" // Korean Pronunciation: WI

           <<"ᄌ" // U+110C HANGUL CHOSEONG CIEUC
           <<"卡" // Korean Pronunciation: CAP
           <<"張" // Korean Pronunciation: CANG
           <<"宁" // Korean Pronunciation: CE
           <<"정" // U+C815 HANGUL SYLLABLE JEONG

           <<"ᄎ" // U+110E HANGUL CHOSEONG CHIEUCH
           <<"馳" // Korean Pronunciation: CHI

           <<"ᄏ" // U+110F HANGUL CHOSEONG KHIEUKH

           <<"ᄐ" // U+1110 HANGUL CHOSEONG THIEUTH

           <<"ᄑ" // U+1111 HANGUL CHOSEONG PHIEUPH

           <<"ᄒ" // U+1112 HANGUL CHOSEONG HIEUH
           <<"학" // U+D559 HANGUL SYLLABLE HAG
           <<"學" // Korean Pronunciation: HAK

           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"ꀀ"
           <<"ꆶ"
           <<"张" // No Korean Pronunciation
           <<"蘅" // No Korean Pronunciation
           <<"驰" // No Korean Pronunciation
            )
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"
           <<"Ａ"
           <<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"<<"F"
           <<"J"
           <<"Ｊ"
           <<"J"
           <<"Ｊ"
           <<"S"
           <<"Ｓ"
           <<"S"
           <<"Ｓ"
           <<"Α"<<"Α"
           <<"Д"<<"Д"
           <<"א"
           <<"ת"
           <<"ا"
           <<"ي"
           <<"अ"
           <<"ह"
           <<"ཀ"
           <<"ཧ"

           <<"ᄀ" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"ᄀ" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"ᄀ" // U+1100 HANGUL CHOSEONG KIYEOK
           <<"ᄀ" // U+1100 HANGUL CHOSEONG KIYEOK

           <<"ᄂ" // U+1102 HANGUL CHOSEONG NIEUN

           <<"ᄃ" // U+1103 HANGUL CHOSEONG TIKEUT
           <<"ᄃ" // U+1103 HANGUL CHOSEONG TIKEUT
           <<"ᄃ" // U+1103 HANGUL CHOSEONG TIKEUT

           <<"ᄅ" // U+1105 HANGUL CHOSEONG RIEUL
           <<"ᄅ" // U+1105 HANGUL CHOSEONG RIEUL
           <<"ᄅ" // U+1105 HANGUL CHOSEONG RIEUL

           <<"ᄆ" // U+1106 HANGUL CHOSEONG MIEUM

           <<"ᄇ" // U+1107 HANGUL CHOSEONG PIEUP
           <<"ᄇ" // U+1107 HANGUL CHOSEONG PIEUP
           <<"ᄇ" // U+1107 HANGUL CHOSEONG PIEUP

           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS
           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS
           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS
           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS
           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS
           <<"ᄉ" // U+1109 HANGUL CHOSEONG SIOS

           <<"ᄋ" // U+110B HANGUL CHOSEONG IEUNG
           <<"ᄋ" // U+110B HANGUL CHOSEONG IEUNG
           <<"ᄋ" // U+110B HANGUL CHOSEONG IEUNG

           <<"ᄌ" // U+110C HANGUL CHOSEONG CIEUC
           <<"ᄌ" // U+110C HANGUL CHOSEONG CIEUC
           <<"ᄌ" // U+110C HANGUL CHOSEONG CIEUC
           <<"ᄌ" // U+110C HANGUL CHOSEONG CIEUC
           <<"ᄌ" // U+110C HANGUL CHOSEONG CIEUC

           <<"ᄎ" // U+110E HANGUL CHOSEONG CHIEUCH
           <<"ᄎ" // U+110E HANGUL CHOSEONG CHIEUCH

           <<"ᄏ" // U+110F HANGUL CHOSEONG KHIEUKH

           <<"ᄐ" // U+1110 HANGUL CHOSEONG THIEUTH

           <<"ᄑ" // U+1111 HANGUL CHOSEONG PHIEUPH

           <<"ᄒ" // U+1112 HANGUL CHOSEONG HIEUH
           <<"ᄒ" // U+1112 HANGUL CHOSEONG HIEUH
           <<"ᄒ" // U+1112 HANGUL CHOSEONG HIEUH

           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"ꀀ"
           <<"ꆶ"
           <<"张"
           <<"蘅"
           <<"驰"
            );
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
    QTest::newRow("zh_TW@collation=stroke")
        <<"ja_JP"
        <<"zh_TW@collation=stroke" // collation=stroke is default for zh_TW
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"john"<<"John"<<"saki"<<"Saki"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"一" // 1 stroke
           <<"乙" // 1 stroke
           <<"丁" // 2 strokes
           <<"二" // 2 strokes
           <<"三" // 3 strokes U+4E09
           <<"川" // 3 strokes U+5DDD
           <<"丑" // 4 strokes U+4E11
           <<"木" // 4 strokes U+6728
           <<"丙" // 5 strokes, 1st stroke horizontal U+4E19
           <<"东" // dong1, 5 strokes, 1st stroke horizontal, simplified of 東 U+4E1C
           <<"冬" // dong1, 5 strokes, 1st stroke down to left U+51AC
           <<"卡" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
           <<"叫" // 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4   U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
           <<"𠮩" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
           <<"宁驰" // 5 strokes, 1st stroke vertical, 1st character U+5B81
           <<"丞" // 6 strokes U+4E1E
           <<"刘" // 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
           <<"刘 Lucy" // liu2
           <<"刘Lucy" // liu2
           <<"刘利" // liu2 li4
           <<"刘利军" // liu2 li4 jun1
           <<"动" // dong4, 6 strokes, 1st stroke horizontal, simplified of 動 U+52A8
           <<"羊" // 6 strokes U+7F8A
           <<"驰" // chi2, 6 strokes, 1st stroke bending, simplified of 馳, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
           <<"串" // 7 strokes, 1st stroke vertical U+4E32
           <<"冻" // dong4, 7 strokes, 1st stroke down to right, simplified of 凍 U+51BB
           <<"君" // 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
           <<"君蘅 柳" // 7 strokes, 1st stroke bending
           <<"张" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
           <<"李" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
           <<"沙紀" // 7 strokes, 1st stroke down to right, 1st character U+6C99
           <<"阿" // a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5 (ERROR: bucket wrong in libicu46!)
           <<"並" // 8 strokes, 1st stroke down to right U+4E26
           <<"亟" // 8 strokes! (libicu 44 counts 9 strokes, which looks like an ERROR), 1st stroke horizontal U+4E9F
           <<"侗" // dong4, 8 strokes, 1st stroke down to left U+4F97
           <<"咚" // dong1, 8 strokes, 1st stroke vertical U+549A
           <<"岽" // dong1, 8 strokes, 1st stroke vertical, simplified of 崠 U+5CBD
           <<"東" // dong1, 8 strokes, 1st stroke horizontal, traditional of 东 U+6771
           <<"苳" // dong1, 8 strokes, 1st stroke horizontal, (libicu44: 9 strokes (old, 2 part kusakanmuri gives 9 strokes)) U+82F3
           <<"威" // 9 strokes, 1st stroke horizontal U+5A01
           <<"峠" // quia3??? ９ strokes, 1st stroke vertical, (Japanese only! variant of 卡) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
           <<"恫" // dong4, 9 strokes, 1st stroke vertical U+606B
           <<"柳" // 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
           <<"柳东" // liu3 dong1; 2nd character 5 strokes, initial stroke horizontal, 2nd character U+4E1C
           <<"柳冬" // liu3 dong1; 2nd character 5 strokes, initial stroke down to left, 2nd character U+51AC
           <<"柳利军" // liu3 li4 jun1; 3rd character 5 strokes, initial stroke vertical, 3rd character U+519B
           <<"柳利軍" // liu3 li4 jun1; 3rd character traditional, 9 strokes, initial stroke vertical, 3rd character U+8ECD
           <<"柳咚" // liu3 dong1; 2nd character 8 strokes, initial stroke vertical
           <<"柳東" // liu3 dong1; 2nd character traditional, 8 strokes, initial stroke horizontal
           <<"栋" // dong4, 9 strokes, 1st stroke horizontal, simplified of 棟 U+680B
           <<"氡" // dong1, 9 strokes, 1st stroke down to left U+6C21
           <<"洞" // dong4, 9 strokes, 1st stroke down to right U+6D1E
           <<"乘" // 10 strokes, 1st stroke down to left U+4E58
           <<"凍" // dong4, 10 strokes, 1st stroke down to right, traditional of 冻 U+51CD
           <<"胴" // dong4, 10 strokes, 1st stroke down to left U+80F4
           <<"髟" // 10 strokes, 1st stroke vertical U+9ADF
           <<"鸫" // dong1, 10 strokes, 1st stroke horizontal, simplified of 鶇 U+9E2B
           <<"乾" // 11 strokes, 1st stroke horizontal U+4E7E
           <<"動" // dong4, 11 strokes, 1st stroke down to left, traditional of 动 U+52D5
           <<"崠" // dong1, dong4, 11 strokes, 1st stroke vertical, traditional of 岽 U+5D20
           <<"張" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
           <<"涷" // dong1, 11 strokes, 1st stroke down to right U+6DB7
           <<"硐" // dong4, 11 strokes, 1st stroke horizontal U+7850
           <<"雪" // 11 strokes, 1st stroke horizontal U+96EA
           <<"傢" // 12 strokes, 1st stroke down to left U+50A2
           <<"棟" // dong4, 12 strokes, 1st stroke horizontal, traditional of 栋 U+68DF
           <<"董" // dong3, 12 strokes (libicu44 13 strokes, seems to count kusakanmuri as 4) U+8463
           <<"亂" // 13 strokes, 1st stroke down to left U+4E82
           <<"働" // dong4, 13 strokes (Japanese only!) U+50CD
           <<"罪" // 13 strokes, 1st stroke vertical, 2nd stroke bending U+7F6A
           <<"馳" // chi2, 13 strokes, 1st stroke vertical, 2nd stroke horizontal, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
           <<"僧" // 14 strokes, 1st stroke down to left U+50E7
           <<"蜻" // 14 strokes, 1st stroke vertical U+873B
           <<"蝀" // dong1, 14 strokes, 1st stroke vertical U+8740
           <<"億" // 15 strokes, 1st stroke down to left U+5104
           <<"劉" // liu2; 15 strokes, 1st stroke bending, traditional of 刘 U+5289
           <<"劉 Lucy" // liu2; traditional
           <<"劉Lucy" // liu2; traditional
           <<"劉利" // liu2 li4; traditional
           <<"劉利軍" // liu2 li4 jun1; traditional
           <<"懂" // dong3, 15 strokes (ERROR in libuicu44, counts 17. If kusakanmuri counts as 4, then 16, but why 17?) U+61C2
           <<"蝙" // 15 strokes, 1st stroke vertical U+8759
           <<"儒" // 16 U+5112
           <<"頻" // 16 U+FA6A
           <<"優" // 17 U+512A
           <<"曙" // 17 U+66D9
           <<"叢" // 18 strokes, 1st stroke U+53E2
           <<"鮹" // 18 strokes, 1st stroke down to left U+9BB9
           <<"鼕" // dong1, 18 strokes, 1st stroke horizontal U+9F15
           <<"勸" // 19 strokes, 1st stroke horizontal U+52F8
           <<"嚥" // 19 strokes, 1st stroke vertical U+56A5
           <<"蘅" // 19 strokes (when kusakanmuri counts as 4, else 18), 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
           <<"鶇" // dong1, 19 strokes, 1st stroke horizontal, traditional of 鸫 U+9D87
           <<"嚷" // 20 strokes, 1st stroke vertical
           <<"儷" // 21
           <<"儼" // 22
           <<"囌" // 23
           <<"囑" // 24
           <<"廳" // 25 last official bucket
           <<"躡" // 25
           <<"鑽" // 27
           <<"鬱" // 29
           <<"驫" // 30
           <<"㺃" // U+3E83
           <<"䵷" // U+4D77
            )
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"J"<<"J"<<"S"<<"S"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"一"<<"一" //  1 stroke
           <<"丁"<<"丁" //  2
           <<"三"<<"三" //  3
           <<"丑" // 4
           <<"丑" // 4
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"丞" // 6
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"並" // 8
           <<"亟" // 8
           <<"亟" // 8
           <<"亟" // 8
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"乘" // 10
           <<"乘" // 10
           <<"乘" // 10
           <<"乘" // 10
           <<"乘" // 10
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"傢" // 12
           <<"傢" // 12
           <<"傢" // 12
           <<"亂" // 13
           <<"亂" // 13
           <<"亂" // 13
           <<"亂" // 13
           <<"僧" // 14
           <<"僧" // 14
           <<"僧" // 14
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"儒" // 16
           <<"儒" // 16
           <<"優" // 17
           <<"優" // 17
           <<"叢" // 18
           <<"叢" // 18
           <<"叢" // 18
           <<"叢" // 18
           <<"嚥" // 19
           <<"嚥" // 19
           <<"嚥" // 19
           <<"嚷" // 20
           <<"儷" // 21
           <<"儼" // 22
           <<"囌" // 23
           <<"囑" // 24
           <<"廳" // 25 last official bucket
           <<"躡" // 25
           <<"鑽" // 27
           <<"鬱" // 29
           <<"驫" // 30
           <<"㺃"
           <<"䵷"
            );
#else // libicu < 46, i.e. libicu 44
    QTest::newRow("zh_TW@collation=stroke")
        <<"ja_JP"
        <<"zh_TW@collation=stroke" // collation=stroke is default for zh_TW
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"john"<<"John"<<"saki"<<"Saki"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"一" // 1 stroke
           <<"乙" // 1 stroke
           <<"丁" // 2 strokes
           <<"二" // 2 strokes
           <<"三" // 3 strokes
           <<"川" // 3 strokes
           <<"丑" // 4 strokes
           <<"木" // 4 strokes
           <<"丙" // 5 strokes, 1st stroke horizontal
           <<"冬" // dong1, 5 strokes, 1st stroke down to left
           <<"卡" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
           <<"叫" // 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4   U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
           <<"宁驰" // 5 strokes, 1st stroke vertical
           <<"丞" // 6 strokes
           <<"羊" // 6 strokes
           <<"串" // 7 strokes
           <<"君" // 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
           <<"君蘅 柳" // 7 strokes, 1st stroke bending
           <<"李" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
           <<"沙紀" // 7 strokes, 1st stroke down to right
           <<"並" // 8 strokes, 1st stroke down to right
           <<"咚" // dong1, 8 strokes, 1st stroke vertical
           <<"東" // dong1, 8 strokes, 1st stroke horizontal, traditional of 东
           <<"阿" // a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5
           <<"侗" // dong4, 8 strokes, 1st stroke down to left
           <<"亟" // 9 strokes, 1st stroke horizontal
           <<"威" // 9 strokes, 1st stroke horizontal
           <<"恫" // dong4, 9 strokes, 1st stroke vertical
           <<"柳" // 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
           <<"柳冬" // liu3 dong1; 2nd character 5 strokes, initial stroke down to left
           <<"柳利軍" // liu3 li4 jun1; 3rd character traditional, 9 strokes, initial stroke vertical
           <<"柳利军" // liu3 li4 jun1; 3rd character 5 strokes, initial stroke vertical
           <<"柳咚" // liu3 dong1; 2nd character 8 strokes, initial stroke vertical
           <<"柳東" // liu3 dong1; 2nd character traditional, 8 strokes, initial stroke horizontal
           <<"柳东" // liu3 dong1; 2nd character 5 strokes, initial stroke horizontal
           <<"洞" // dong4, 9 strokes, 1st stroke down to right
           <<"氡" // dong1, 9 strokes, 1st stroke down to left
           <<"苳" // dong1, 8 strokes, 1st stroke horizontal, ERROR? (old, 2 part kusakanmuri gives 9 strokes)
           <<"乘" // 10 strokes, 1st stroke down to left
           <<"凍" // dong4, 10 strokes, 1st stroke down to right, traditional of 冻
           <<"胴" // dong4, 10 strokes, 1st stroke down to left
           <<"髟" // 10 strokes, 1st stroke vertical
           <<"乾" // 11 strokes, 1st stroke horizontal
           <<"動" // dong4, 11 strokes, 1st stroke down to left, traditional of 动
           <<"張" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
           <<"雪" // 11 strokes, 1st stroke horizontal
           <<"崠" // dong1, dong4, 11 strokes, 1st stroke vertical, traditional of 岽
           <<"涷" // dong1, 11 strokes, 1st stroke down to right
           <<"硐" // dong4, 11 strokes, 1st stroke horizontal
           <<"傢" // 12 strokes, 1st stroke down to left
           <<"棟" // dong4, 12 strokes, 1st stroke horizontal, traditional of 栋
           <<"亂" // 13 strokes, 1st stroke down to left
           <<"罪" // 13 strokes, 1st stroke vertical, 2nd stroke bending
           <<"董" // dong3, 12 strokes (ERROR? kusakanmuri seems to count as 4)
           <<"馳" // chi2, 13 strokes, 1st stroke vertical, 2nd stroke horizontal, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
           <<"僧" // 14 strokes, 1st stroke down to left
           <<"蜻" // 14 strokes, 1st stroke vertical
           <<"蝀" // dong1, 14 strokes, 1st stroke vertical
           <<"億" // 15 strokes, 1st stroke down to left
           <<"劉" // liu2; 15 strokes, 1st stroke bending, traditional of 刘
           <<"劉 Lucy" // liu2; traditional
           <<"劉Lucy" // liu2; traditional
           <<"劉利" // liu2 li4; traditional
           <<"劉利軍" // liu2 li4 jun1; traditional
           <<"蝙" // 15 strokes, 1st stroke vertical
           <<"儒" // 16
           <<"頻" // 16
           <<"優" // 17
           <<"懂" // dong3, 15 strokes (ERROR? if kusakanmuri counts as 4, then 16, but why 17?)
           <<"曙" // 17
           <<"叢" // 18 strokes, 1st stroke
           <<"鼕" // dong1, 18 strokes, 1st stroke horizontal
           <<"鮹" // 18 strokes, 1st stroke down to left
           <<"嚥" // 19
           <<"鶇" // dong1, 19 strokes, 1st stroke horizontal, traditional of 鸫
           <<"勸" // 19
           <<"嚷" // 20 strokes, 1st stroke vertical
           <<"蘅" // 20 strokes, 1st stroke horizontal, kHanyuPinlu heng2   U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
           <<"儷" // 21
           <<"儼" // 22
           <<"囌" // 23
           <<"囑" // 24
           <<"廳" // 25 last official bucket
           <<"躡" // 25
           <<"鑽" // 27
           <<"鬱" // 29
           <<"驫" // 30
           <<"东" // dong1, 5 strokes, 1st stroke horizontal, simplified of 東
           <<"働" // dong4, 13 strokes (Japanese only!)
           <<"冻" // dong4, 7 strokes, 1st stroke down to right, simplified of 凍
           <<"刘" // 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
           <<"刘 Lucy" // liu2
           <<"刘Lucy" // liu2
           <<"刘利" // liu2 li4
           <<"刘利军" // liu2 li4 jun1
           <<"动" // dong4, 6 strokes, 1st stroke horizontal, simplified of 動
           <<"岽" // dong1, 8 strokes, 1st stroke vertical, simplified of 崠
           <<"峠" // quia3??? ９ strokes, 1st stroke vertical, (Japanese only! variant of 卡) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
           <<"张" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
           <<"栋" // dong4, 9 strokes, 1st stroke horizontal, simplified of 棟
           <<"驰" // chi2, 6 strokes, 1st stroke bending, simplified of 馳, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
           <<"鸫" // dong1, 10 strokes, 1st stroke horizontal, simplified of 鶇
           <<"㺃"
           <<"䵷"
           <<"𠮩" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
            )
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"J"<<"J"<<"S"<<"S"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"一"<<"一" //  1 stroke
           <<"丁"<<"丁" //  2
           <<"三"<<"三" //  3
           <<"丑" // 4
           <<"丑" // 4
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丙" // 5
           <<"丞" // 6
           <<"丞" // 6
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"串" // 7
           <<"並" // 8
           <<"並" // 8
           <<"並" // 8
           <<"並" // 8
           <<"並" // 8
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"亟" // 9
           <<"乘" // 10
           <<"乘" // 10
           <<"乘" // 10
           <<"乘" // 10
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"乾" // 11
           <<"傢" // 12
           <<"傢" // 12
           <<"亂" // 13
           <<"亂" // 13
           <<"亂" // 13
           <<"亂" // 13
           <<"僧" // 14
           <<"僧" // 14
           <<"僧" // 14
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"億" // 15
           <<"儒" // 16
           <<"儒" // 16
           <<"優" // 17
           <<"優" // 17
           <<"優" // 17
           <<"叢" // 18
           <<"叢" // 18
           <<"叢" // 18
           <<"嚥" // 19
           <<"嚥" // 19
           <<"嚥" // 19
           <<"嚷" // 20
           <<"嚷" // 20
           <<"儷" // 21
           <<"儼" // 22
           <<"囌" // 23
           <<"囑" // 24
           <<"廳" // 25 last official bucket
           <<"躡" // 25
           <<"鑽" // 27
           <<"鬱" // 29
           <<"驫" // 30
           <<"东"
           <<"働"
           <<"冻"
           <<"刘"
           <<"刘"
           <<"刘"
           <<"刘"
           <<"刘"
           <<"动"
           <<"岽"
           <<"峠"
           <<"张"
           <<"栋"
           <<"驰"
           <<"鸫"
           <<"㺃"
           <<"䵷"
           <<"𠮩"
            );
#endif
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=6)
    QTest::newRow("zh_CN@collation=pinyin")
        <<"ja_JP"
        <<"zh_CN@collation=pinyin" // collation=pinyin is default for zh_CN
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"Ａ"<<"john"<<"ｊｏｈｎ"<<"John"<<"Ｊｏｈｎ"<<"saki"<<"ｓａｋｉ"<<"Saki"<<"Ｓａｋｉ"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"阿" // a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5
           <<"驰" // chi2, 6 strokes, 1st stroke bending, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
           <<"馳" // chi2, 13 strokes, 1st stroke vertical, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
           <<"东" // dong1, 5 strokes, 1st stroke horizontal, simplified of 東 U+4E1C
           <<"冬" // dong1, 5 strokes, 1st stroke down to left U+51AC
           <<"咚" // dong1, 8 strokes, 1st stroke vertical U+549A
           <<"岽" // dong1, 8 strokes, 1st stroke vertical, simplified of 崠 U+5CBD
           <<"東" // dong1, 8 strokes, 1st stroke horizontal, traditional of 东 U+6771
           <<"苳" // dong1, 8 strokes, 1st stroke horizontal U+82F3
           <<"氡" // dong1, 9 strokes, 1st stroke down to left
           <<"鸫" // dong1, 10 strokes, 1st stroke horizontal, simplified of 鶇 U+9E2B
           <<"崠" // dong1, dong4 (libicu44 sorts as dong4, libicu46 as dong1), 11 strokes, 1st stroke vertical, traditional of 岽 U+5D20
           <<"涷" // dong1, 11 strokes U+6DB7
           <<"蝀" // dong1, 14 strokes
           <<"鼕" // dong1, 18 strokes
           <<"鶇" // dong1, 19 strokes, 1st stroke horizontal, traditional of 鸫
           <<"董" // dong3, 12 strokes
           <<"懂" // dong3, 15 strokes
           <<"动" // dong4, 6 strokes, 1st stroke horizontal, simplified of 動
           <<"冻" // dong4, 7 strokes, 1st stroke down to right, simplified of 凍
           <<"侗" // dong4, 8 strokes
           <<"恫" // dong4, 9 strokes, 1st stroke vertical U+606B
           <<"栋" // dong4, 9 strokes, 1st stroke horizontal, simplified of 棟 U+680B
           <<"洞" // dong4, 9 strokes, 1st stroke down to right U+6D1E
           <<"凍" // dong4, 10 strokes, 1st stroke down to right, traditional of 冻 U+51CD
           <<"胴" // dong4, 10 strokes, 1st stroke down to left U+80F4
           <<"動" // dong4, 11 strokes, 1st stroke down to left, traditional of 动 U+52D5
           <<"硐" // dong4, 11 strokes, 1st stroke horizontal U+7850
           <<"棟" // dong4, 12 strokes, 1st stroke horizontal, traditional of 栋
           <<"働" // dong4, 13 strokes (Japanese only!)
           <<"峠" // quia3??? ９ strokes, 1st stroke vertical, (Japanese only! variant of 卡) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
           <<"蘅" // 20 strokes, 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
           <<"叫" // 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4 U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
           <<"君" // 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
           <<"卡" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
           <<"李" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
           <<"刘" // 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
           <<"刘 Lucy" // liu2
           <<"刘Lucy" // liu2
           <<"刘利" // liu2 li4
           <<"刘利军" // liu2 li4 jun1
           <<"劉" // liu2; 15 strokes, 1st stroke bending, traditional of 刘
           <<"劉 Lucy" // liu2; traditional
           <<"劉Lucy" // liu2; traditional
           <<"劉利" // liu2 li4; traditional
           <<"劉利軍" // liu2 li4 jun1; traditional
           <<"柳" // 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
           <<"柳东" // liu3 dong1; 2nd character 5 strokes, initial stroke horizontal, 2nd character U+4E1C
           <<"柳冬" // liu3 dong1; 2nd character 5 strokes, initial stroke down to left, 2nd character U+51AC
           <<"柳咚" // liu3 dong1; 2nd character 8 strokes, initial stroke vertical, 2nd character U+549A
           <<"柳東" // liu3 dong1; 2nd character traditional, 8 strokes, initial stroke horizontal, U+6771
           <<"柳利军" // liu3 li4 jun1; 3rd character 5 strokes, initial stroke vertical, 3rd character U+519B
           <<"柳利軍" // liu3 li4 jun1; 3rd character traditional, 9 strokes, initial stroke vertical, 3rd character U+8ECD
           <<"宁" // kHanyuPinlu ning2   U+5B81 kBigFive C972 kGB0 3694 kRSUnicode 40.2"
           <<"威" // 9 strokes, 1st stroke horizontal, kHanyuPinlu wei1 U+5A01 kBigFive ABC2 kGB0 4594 kRSUnicode 38.6"
           <<"张" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
           <<"張" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
           <<"𠮩" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
            )
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"A"<<"J"<<"J"<<"J"<<"J"<<"S"<<"S"<<"S"<<"S"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"阿"
           <<"驰"
           <<"馳"
           <<"东"
           <<"冬"
           <<"咚"
           <<"岽"
           <<"東"
           <<"苳"
           <<"氡"
           <<"鸫"
           <<"崠"
           <<"涷"
           <<"蝀"
           <<"鼕"
           <<"鶇"
           <<"董"
           <<"懂"
           <<"动"
           <<"冻"
           <<"侗"
           <<"恫"
           <<"栋"
           <<"洞"
           <<"凍"
           <<"胴"
           <<"動"
           <<"硐"
           <<"棟"
           <<"働"
           <<"峠"
           <<"蘅"
           <<"叫"
           <<"君"
           <<"卡"
           <<"李"
           <<"刘"<<"刘"<<"刘"<<"刘"<<"刘"
           <<"劉"<<"劉"<<"劉"<<"劉"<<"劉"
           <<"柳"<<"柳"<<"柳"<<"柳"<<"柳"<<"柳"<<"柳"
           <<"宁"
           <<"威"
           <<"张"
           <<"張"
           <<"𠮩"
            );
#else // libicu < 46, i.e. libicu 44
    QTest::newRow("zh_CN@collation=pinyin")
        <<"ja_JP"
        <<"zh_CN@collation=pinyin" // collation=pinyin is default for zh_CN
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"Ａ"<<"john"<<"ｊｏｈｎ"<<"John"<<"Ｊｏｈｎ"<<"saki"<<"ｓａｋｉ"<<"Saki"<<"Ｓａｋｉ"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"阿" // a1, 8 strokes, 1st stroke bending, kHanyuPinlu a1 U+963F kBigFive AAFC kGB0 1602 kRSUnicode 170.5
           <<"驰" // chi2, 6 strokes, 1st stroke bending, kHanyuPinlu chi2 U+9A70 kBigFive - kGB0 1959 kRSUnicode 187'.3
           <<"馳" // chi2, 13 strokes, 1st stroke vertical, kHanyuPinlu chi2    U+99B3 kBigFive B9A3 kGB0    - kRSUnicode 187.3
           <<"东" // dong1, 5 strokes, 1st stroke horizontal, simplified of 東
           <<"冬" // dong1, 5 strokes, 1st stroke down to left
           <<"咚" // dong1, 8 strokes, 1st stroke vertical
           <<"岽" // dong1, 8 strokes, 1st stroke vertical, simplified of 崠
           <<"東" // dong1, 8 strokes, 1st stroke horizontal, traditional of 东
           <<"苳" // dong1, 8 strokes, 1st stroke horizontal
           <<"氡" // dong1, 9 strokes, 1st stroke down to left
           <<"鸫" // dong1, 10 strokes, 1st stroke horizontal, simplified of 鶇
           <<"涷" // dong1, 11 strokes
           <<"蝀" // dong1, 14 strokes
           <<"鼕" // dong1, 18 strokes
           <<"鶇" // dong1, 19 strokes, 1st stroke horizontal, traditional of 鸫
           <<"董" // dong3, 12 strokes
           <<"懂" // dong3, 15 strokes
           <<"动" // dong4, 6 strokes, 1st stroke horizontal, simplified of 動
           <<"冻" // dong4, 7 strokes, 1st stroke down to right, simplified of 凍
           <<"侗" // dong4, 8 strokes
           <<"恫" // dong4, 9 strokes, 1st stroke vertical
           <<"栋" // dong4, 9 strokes, 1st stroke horizontal, simplified of 棟
           <<"洞" // dong4, 9 strokes, 1st stroke down to right
           <<"凍" // dong4, 10 strokes, 1st stroke down to right, traditional of 冻
           <<"胴" // dong4, 10 strokes, 1st stroke down to left
           <<"動" // dong4, 11 strokes, 1st stroke down to left, traditional of 动
           <<"崠" // dong1, dong4 (libicu44 sorts as dong4, libicu46 as dong1), 11 strokes, 1st stroke vertical, traditional of 岽
           <<"硐" // dong4, 11 strokes, 1st stroke horizontal
           <<"棟" // dong4, 12 strokes, 1st stroke horizontal, traditional of 栋
           <<"働" // dong4, 13 strokes (Japanese only!)
           <<"峠" // quia3??? ９ strokes, 1st stroke vertical, (Japanese only! variant of 卡) U+5CE0 kBigFive - kGB0 - kRSUnicode 46.6
           <<"蘅" // 20 strokes, 1st stroke horizontal, kHanyuPinlu heng2 U+8605 kBigFive F4C1 kGB0 6231 kRSUnicode 140.16
           <<"叫" // 5 strokes, 1st stroke vertical, kHanyuPinlu jiao4 U+53EB kBigFive A573 kGB0 2948 kRSUnicode 30.2
           <<"君" // 7 strokes, 1st stroke bending, kHanyuPinlu jun1 U+541B kBigFive A767 kGB0 3093 kRSUnicode 30.4
           <<"卡" // 5 strokes, 1st stroke vertical, kHanyuPinlu ka3 U+5361 kBigFive A564 kGB0 3108 kRSUnicode 25.3
           <<"李" // 7 strokes, 1st stroke horizontal, kHanyuPinlu li5 U+674E kBigFive A7F5 kGB0 3278 kRSUnicode 75.3
           <<"刘" // 6 strokes, 1st stroke vertical, kMandarin liu2 U+5218 kBigFive - kGB0 3385 kRSUnicode 18.4
           <<"刘 Lucy" // liu2
           <<"刘Lucy" // liu2
           <<"刘利" // liu2 li4
           <<"刘利军" // liu2 li4 jun1
           <<"劉" // liu2; 15 strokes, 1st stroke bending, traditional of 刘
           <<"劉 Lucy" // liu2; traditional
           <<"劉Lucy" // liu2; traditional
           <<"劉利" // liu2 li4; traditional
           <<"劉利軍" // liu2 li4 jun1; traditional
           <<"柳" // 9 strokes, 1st stroke horizontal, kHanyuPinlu liu3 U+67F3 kBigFive AC68 kGB0 3388 kRSUnicode 75.5"
           <<"柳东" // liu3 dong1; 2nd character 5 strokes, initial stroke horizontal
           <<"柳冬" // liu3 dong1; 2nd character 5 strokes, initial stroke down to left
           <<"柳咚" // liu3 dong1; 2nd character 8 strokes, initial stroke vertical
           <<"柳東" // liu3 dong1; 2nd character traditional, 8 strokes, initial stroke horizontal
           <<"柳利军" // liu3 li4 jun1; 3rd character 5 strokes, initial stroke vertical
           <<"柳利軍" // liu3 li4 jun1; 3rd character traditional, 9 strokes, initial stroke vertical
           <<"宁" // kHanyuPinlu ning2   U+5B81 kBigFive C972 kGB0 3694 kRSUnicode 40.2"
           <<"威" // 9 strokes, 1st stroke horizontal, kHanyuPinlu wei1 U+5A01 kBigFive ABC2 kGB0 4594 kRSUnicode 38.6"
           <<"张" // 7 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F20 kBigFive    - kGB0 5337 kRSUnicode 57.4"
           <<"張" // 11 strokes, 1st stroke bending, kHanyuPinlu zhang1  U+5F35 kBigFive B169 kGB0    - kRSUnicode 57.8"
           <<"𠮩" // 5 strokes, 1st stroke vertical, kHanyuPinlu - U+20BA9 kBigFive - kGB0- kRSUnicode 30.2"
            )
        <<(QStringList()
           <<"∮"<<"♨"
           <<"A"<<"A"<<"J"<<"J"<<"J"<<"J"<<"S"<<"S"<<"S"<<"S"
           <<"あ"
           <<"ㄅ"
           <<"ㄎ"
           <<"阿"
           <<"驰"
           <<"馳"
           <<"东"
           <<"冬"
           <<"咚"
           <<"岽"
           <<"東"
           <<"苳"
           <<"氡"
           <<"鸫"
           <<"涷"
           <<"蝀"
           <<"鼕"
           <<"鶇"
           <<"董"
           <<"懂"
           <<"动"
           <<"冻"
           <<"侗"
           <<"恫"
           <<"栋"
           <<"洞"
           <<"凍"
           <<"胴"
           <<"動"
           <<"崠"
           <<"硐"
           <<"棟"
           <<"働"
           <<"峠"
           <<"蘅"
           <<"叫"
           <<"君"
           <<"卡"
           <<"李"
           <<"刘"<<"刘"<<"刘"<<"刘"<<"刘"
           <<"劉"<<"劉"<<"劉"<<"劉"<<"劉"
           <<"柳"<<"柳"<<"柳"<<"柳"<<"柳"<<"柳"<<"柳"
           <<"宁"
           <<"威"
           <<"张"
           <<"張"
           <<"𠮩"
            );
#endif
}

void Ft_Locales::testMLocaleIndexBucket()
{
    QFETCH(QString, localeName);
    QFETCH(QString, lcCollate);
    QFETCH(QStringList, stringsSorted);
    QFETCH(QStringList, expectedBuckets);

    QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
    MLocale locale(localeName);
    MLocale localeEn("en_US");
    locale.setCategoryLocale(MLocale::MLcCollate, lcCollate);
    QStringList stringsSortedCopy = stringsSorted;
    qSort (stringsSortedCopy.begin(), stringsSortedCopy.end(),
           localeEn.collator());
    qSort (stringsSortedCopy.begin(), stringsSortedCopy.end(),
           locale.collator());
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << " stringsSorted.size():" << stringsSorted.size()
                << " expectedBuckets.size():" << expectedBuckets.size()
                << "\n";
    debugStream.flush();
    debugStream << "\tlcCollate: " << lcCollate
                << "\n\tbuckets\tbuckets\tstrings\tstrings"
                << "\n\texpect \tfound  \torig   \tsorted\n";
    for (int i = 0; i < stringsSorted.size(); ++i)
        debugStream
            << i
            << "\t" << expectedBuckets[i]
            << "\t" << locale.indexBucket(stringsSorted[i])
            << "\t" << stringsSorted[i]
            << "\t" << stringsSortedCopy[i]
            << "\n";
    debugStream.flush();
#endif
    QCOMPARE(stringsSortedCopy, stringsSorted);
    for (int i = 0; i < stringsSorted.size(); ++i) {
#if defined(VERBOSE_OUTPUT)
        if (locale.indexBucket(stringsSorted[i]) != expectedBuckets[i]) {
            debugStream << "i=" << i
                        << " locale.indexBucket(stringsSorted[i])="
                        << locale.indexBucket(stringsSorted[i])
                        << " expectedBuckets[i]="
                        << expectedBuckets[i] << "\n";
            debugStream.flush();
        }
#endif
        QCOMPARE(locale.indexBucket(stringsSorted[i]),
                 expectedBuckets[i]);
    }
}

/*
 * To reduce the size of libicu, we customize the locale data included in
 * our package of libicu and include only what needs to be there.
 *
 * This test checks whether our package of libicu still has all the
 * locales which need to be supported to make sure we did not omit too
 * much accidentally.
 *
 */
void Ft_Locales::checkAvailableLocales()
{
    // For the list of locales which need to be supported see, the list
    // in i18n.dox
    QList<QString> requiredLocaleNames;
    requiredLocaleNames << "ar";          // "Arabic"
    requiredLocaleNames << "ar_AE";       // "Arabic (United Arab Emirates)"
    requiredLocaleNames << "ar_BH";       // "Arabic (Bahrain)"
    requiredLocaleNames << "ar_DZ";       // "Arabic (Algeria)"
    requiredLocaleNames << "ar_EG";       // "Arabic (Egypt)"
    requiredLocaleNames << "ar_IQ";       // "Arabic (Iraq)"
    requiredLocaleNames << "ar_JO";       // "Arabic (Jordan)"
    requiredLocaleNames << "ar_KW";       // "Arabic (Kuwait)"
    requiredLocaleNames << "ar_LB";       // "Arabic (Lebanon)"
    requiredLocaleNames << "ar_LY";       // "Arabic (Libya)"
    requiredLocaleNames << "ar_MA";       // "Arabic (Morocco)"
    requiredLocaleNames << "ar_OM";       // "Arabic (Oman)"
    requiredLocaleNames << "ar_QA";       // "Arabic (Qatar)"
    requiredLocaleNames << "ar_SA";       // "Arabic (Saudi Arabia)"
    requiredLocaleNames << "ar_SD";       // "Arabic (Sudan)"
    requiredLocaleNames << "ar_SY";       // "Arabic (Syria)"
    requiredLocaleNames << "ar_TN";       // "Arabic (Tunisia)"
    requiredLocaleNames << "ar_YE";       // "Arabic (Yemen)"
    requiredLocaleNames << "ca";          // "Catalan"
    requiredLocaleNames << "ca_ES";       // "Catalan (Spain)"
    requiredLocaleNames << "cs";          // "Czech
    requiredLocaleNames << "cs_CZ";       // "Czech (Czech Republic)"
    requiredLocaleNames << "da";          // "Danish"
    requiredLocaleNames << "da_DK";       // "Danish (Denmark)"
    requiredLocaleNames << "de";          // "German"
    requiredLocaleNames << "de_AT";       // "German (Austria)"
    requiredLocaleNames << "de_BE";       // "German (Belgium)"
    requiredLocaleNames << "de_CH";       // "German (Switzerland)"
    requiredLocaleNames << "de_DE";       // "German (Germany)"
    requiredLocaleNames << "de_LI";       // "German (Liechtenstein)"
    requiredLocaleNames << "de_LU";       // "German (Luxembourg)"
    requiredLocaleNames << "el";          // "Greek"
    requiredLocaleNames << "el_CY";       // "Greek (Cyprus)"
    requiredLocaleNames << "el_GR";       // "Greek (Greece)"
    requiredLocaleNames << "en";          // "English"
    requiredLocaleNames << "en_AU";       // "English (Australia)"
    requiredLocaleNames << "en_BE";       // "English (Belgium)"
    requiredLocaleNames << "en_BW";       // "English (Botswana)"
    requiredLocaleNames << "en_BZ";       // "English (Belize)"
    requiredLocaleNames << "en_CA";       // "English (Canada)"
    requiredLocaleNames << "en_GB";       // "English (United Kingdom)"
    requiredLocaleNames << "en_HK";       // "English (Hong Kong SAR China)"
    requiredLocaleNames << "en_IE";       // "English (Ireland)"
    requiredLocaleNames << "en_IN";       // "English (India)"
    requiredLocaleNames << "en_JM";       // "English (Jamaica)"
    requiredLocaleNames << "en_MH";       // "English (Marshall Islands)"
    requiredLocaleNames << "en_MT";       // "English (Malta)"
    requiredLocaleNames << "en_NA";       // "English (Namibia)"
    requiredLocaleNames << "en_NZ";       // "English (New Zealand)"
    requiredLocaleNames << "en_PH";       // "English (Philippines)"
    requiredLocaleNames << "en_PK";       // "English (Pakistan)"
    requiredLocaleNames << "en_SG";       // "English (Singapore)"
    requiredLocaleNames << "en_TT";       // "English (Trinidad and Tobago)"
    requiredLocaleNames << "en_US";       // "English (United States)"
    requiredLocaleNames << "en_US_POSIX"; // "English (United States, Computer)"
    requiredLocaleNames << "en_VI";       // "English (U.S. Virgin Islands)"
    requiredLocaleNames << "en_ZA";       // "English (South Africa)"
    requiredLocaleNames << "en_ZW";       // "English (Zimbabwe)"
    requiredLocaleNames << "es";          // "Spanish"
    requiredLocaleNames << "es_AR";       // "Spanish (Argentina)"
    requiredLocaleNames << "es_BO";       // "Spanish (Bolivia)"
    requiredLocaleNames << "es_CL";       // "Spanish (Chile)"
    requiredLocaleNames << "es_CO";       // "Spanish (Colombia)"
    requiredLocaleNames << "es_CR";       // "Spanish (Costa Rica)"
    requiredLocaleNames << "es_DO";       // "Spanish (Dominican Republic)"
    requiredLocaleNames << "es_EC";       // "Spanish (Ecuador)"
    requiredLocaleNames << "es_ES";       // "Spanish (Spain)"
    requiredLocaleNames << "es_GT";       // "Spanish (Guatemala)"
    requiredLocaleNames << "es_HN";       // "Spanish (Honduras)"
    requiredLocaleNames << "es_MX";       // "Spanish (Mexico)"
    requiredLocaleNames << "es_NI";       // "Spanish (Nicaragua)"
    requiredLocaleNames << "es_PA";       // "Spanish (Panama)"
    requiredLocaleNames << "es_PE";       // "Spanish (Peru)"
    requiredLocaleNames << "es_PR";       // "Spanish (Puerto Rico)"
    requiredLocaleNames << "es_PY";       // "Spanish (Paraguay)"
    requiredLocaleNames << "es_SV";       // "Spanish (El Salvador)"
    requiredLocaleNames << "es_US";       // "Spanish (United States)"
    requiredLocaleNames << "es_UY";       // "Spanish (Uruguay)"
    requiredLocaleNames << "es_VE";       // "Spanish (Venezuela)"
    requiredLocaleNames << "eu";          // "Basque"
    requiredLocaleNames << "eu_ES";       // "Basque (Spain)"
    requiredLocaleNames << "fa";          // "Persian"
    requiredLocaleNames << "fa_AF";       // "Persian (Afghanistan)"
    requiredLocaleNames << "fa_IR";       // "Persian (Iran)"
    requiredLocaleNames << "fi";          // "Finnish"
    requiredLocaleNames << "fi_FI";       // "Finnish (Finland)"
    requiredLocaleNames << "fr";          // "French"
    requiredLocaleNames << "fr_BE";       // "French (Belgium)"
    requiredLocaleNames << "fr_CA";       // "French (Canada)"
    requiredLocaleNames << "fr_CH";       // "French (Switzerland)"
    requiredLocaleNames << "fr_FR";       // "French (France)"
    requiredLocaleNames << "fr_LU";       // "French (Luxembourg)"
    requiredLocaleNames << "fr_MC";       // "French (Monaco)"
    requiredLocaleNames << "fr_SN";       // "French (Senegal)"
    requiredLocaleNames << "gl";          // "Galician"
    requiredLocaleNames << "gl_ES";       // "Galician (Spain)"
    requiredLocaleNames << "he";          // "Hebrew"
    requiredLocaleNames << "he_IL";       // "Hebrew (Israel)"
    requiredLocaleNames << "hi";          // "Hindi"
    requiredLocaleNames << "hi_IN";       // "Hindi (India)"
    requiredLocaleNames << "hu";          // "Hungarian"
    requiredLocaleNames << "hu_HU";       // "Hungarian (Hungary)"
    requiredLocaleNames << "id";          // "Indonesian"
    requiredLocaleNames << "id_ID";       // "Indonesian (Indonesia)"
    requiredLocaleNames << "it";          // "Italian"
    requiredLocaleNames << "it_CH";       // "Italian (Switzerland)"
    requiredLocaleNames << "it_IT";       // "Italian (Italy)"
    requiredLocaleNames << "ja";          // "Japanese"
    requiredLocaleNames << "ja_JP";       // "Japanese (Japan)"
    requiredLocaleNames << "ms";          // "Malay"
    requiredLocaleNames << "ms_BN";       // "Malay (Brunei)"
    requiredLocaleNames << "ms_MY";       // "Malay (Malaysia)"
    requiredLocaleNames << "nb";          // "Norwegian Bokml"
    requiredLocaleNames << "nb_NO";       // "Norwegian Bokml (Norway)"
    requiredLocaleNames << "nl";          // "Dutch"
    requiredLocaleNames << "nl_BE";       // "Dutch (Belgium)"
    requiredLocaleNames << "nl_NL";       // "Dutch (Netherlands)"
    requiredLocaleNames << "nn";          // "Norwegian Nynorsk"
    requiredLocaleNames << "nn_NO";       // "Norwegian Nynorsk (Norway)"
    requiredLocaleNames << "pl";          // "Polish"
    requiredLocaleNames << "pl_PL";       // "Polish (Poland)"
    requiredLocaleNames << "pt";          // "Portuguese"
    requiredLocaleNames << "pt_BR";       // "Portuguese (Brazil)"
    requiredLocaleNames << "pt_PT";       // "Portuguese (Portugal)"
    requiredLocaleNames << "ro";          // "Romanian"
    requiredLocaleNames << "ro_MD";       // "Romanian (Moldova)"
    requiredLocaleNames << "ro_RO";       // "Romanian (Romania)"
    requiredLocaleNames << "ru";          // "Russian"
    requiredLocaleNames << "ru_RU";       // "Russian (Russia)"
    requiredLocaleNames << "ru_UA";       // "Russian (Ukraine)"
    requiredLocaleNames << "sk";          // "Slovak"
    requiredLocaleNames << "sk_SK";       // "Slovak (Slovakia)"
    requiredLocaleNames << "sv";          // "Swedish"
    requiredLocaleNames << "sv_FI";       // "Swedish (Finland)"
    requiredLocaleNames << "sv_SE";       // "Swedish (Sweden)"
    requiredLocaleNames << "th";          // "Thai"
    requiredLocaleNames << "th_TH";       // "Thai (Thailand)"
    requiredLocaleNames << "tr";          // "Turkish"
    requiredLocaleNames << "tr_TR";       // "Turkish (Turkey)"
    requiredLocaleNames << "uk";          // "Ukrainian"
    requiredLocaleNames << "uk_UA";       // "Ukrainian (Ukraine)"
    requiredLocaleNames << "ur";          // "Urdu"
    requiredLocaleNames << "ur_IN";       // "Urdu (India)"
    requiredLocaleNames << "ur_PK";       // "Urdu (Pakistan)"
    requiredLocaleNames << "zh";          // "Chinese"
    requiredLocaleNames << "zh_Hans" ;    // "Chinese (Simplified Han)"
    requiredLocaleNames << "zh_Hans_CN";  // "Chinese (Simplified Han, China)"
    requiredLocaleNames << "zh_Hans_HK";  // "Chinese (Simplified Han, Hong Kong SAR China)"
    requiredLocaleNames << "zh_Hans_MO";  // "Chinese (Simplified Han, Macau SAR China)"
    requiredLocaleNames << "zh_Hans_SG";  // "Chinese (Simplified Han, Singapore)"
    requiredLocaleNames << "zh_Hant";     // "Chinese (Traditional Han)"
    requiredLocaleNames << "zh_Hant_HK";  // "Chinese (Traditional Han, Hong Kong SAR China)"
    requiredLocaleNames << "zh_Hant_MO";  // "Chinese (Traditional Han, Macau SAR China)"
    requiredLocaleNames << "zh_Hant_TW";  // "Chinese (Traditional Han, Taiwan)"

    QList<QString> availableLocaleNames;
    QList<QString> availableDisplayNames;
    int numberOfAvailableLocales = uloc_countAvailable();

    for (int i = 0; i < numberOfAvailableLocales; ++i) {
        int maxresultsize = 100;
        UChar result[maxresultsize];
        UErrorCode status = U_ZERO_ERROR;
        availableLocaleNames << QString::fromUtf8(uloc_getAvailable(i));
        uloc_getDisplayName(availableLocaleNames[i].toUtf8().constData(),
                            "en_US", result, maxresultsize, &status);
        if (U_SUCCESS(status))
            availableDisplayNames << QString::fromUtf16(result);
        else
            availableDisplayNames << QString("What kind of locale is this?");
    }
    // for (int i = 0; i < numberOfAvailableLocales; ++i) {
    //     qDebug() << "available:" << availableLocaleNames[i] << availableDisplayNames[i];
    // }
    foreach(const QString &requiredLocaleName, requiredLocaleNames) {
        // if (availableLocaleNames.contains(requiredLocaleName))
        //     qDebug() << "required and available: "
        //              << requiredLocaleName
        //              << availableDisplayNames[availableLocaleNames.indexOf(requiredLocaleName)];
        // else {
        //     qDebug() << "required but missing: " << requiredLocaleName;
        // }
        QVERIFY2(availableLocaleNames.contains(requiredLocaleName),
                 QString("The following required locale is missing: "
                         + requiredLocaleName).toUtf8().constData());
    }
    // add supported aliases here to check that these work as well:
    QList<QString> supportedLocaleNames = requiredLocaleNames;
    supportedLocaleNames << "no";
    supportedLocaleNames << "no_NO";
    supportedLocaleNames << "es_419";
    // add some variants with different sort rules:
    supportedLocaleNames << "de_DE@collation=phonebook";
    supportedLocaleNames << "zh_CN";
    supportedLocaleNames << "zh_SG";
    supportedLocaleNames << "zh_HK";
    supportedLocaleNames << "zh_MO";
    supportedLocaleNames << "zh_TW";
    supportedLocaleNames << "zh_Hans_TW";
    supportedLocaleNames << "zh_TW@collation=pinyin";
    supportedLocaleNames << "zh_TW@collation=stroke";
    supportedLocaleNames << "zh_CN@collation=pinyin";
    supportedLocaleNames << "zh_CN@collation=stroke";
    supportedLocaleNames << "ko_KR";
    bool runFullTest = false;
    if (!runFullTest) {
        supportedLocaleNames.clear();
        supportedLocaleNames << "ar_SA";       // "Arabic (Saudi Arabia)"
        supportedLocaleNames << "ca_ES";       // "Catalan (Spain)"
        supportedLocaleNames << "da_DK";       // "Danish (Denmark)"
        supportedLocaleNames << "de_CH";       // "German (Switzerland)"
        supportedLocaleNames << "de_DE";       // "German (Germany)"
        supportedLocaleNames << "el_GR";       // "Greek (Greece)"
        supportedLocaleNames << "en";          // "English"
        supportedLocaleNames << "en_BE";       // "English (Belgium)"
        supportedLocaleNames << "en_GB";       // "English (United Kingdom)"
        supportedLocaleNames << "en_US";       // "English (United States)"
        supportedLocaleNames << "en_US_POSIX"; // "English (United States, Computer)"
        supportedLocaleNames << "eu_ES";       // "Basque (Spain)"
        supportedLocaleNames << "fa_IR";       // "Persian (Iran)"
        supportedLocaleNames << "fi_FI";       // "Finnish (Finland)"
        supportedLocaleNames << "fr_FR";       // "French (France)"
        supportedLocaleNames << "gl_ES";       // "Galician (Spain)"
        supportedLocaleNames << "he_IL";       // "Hebrew (Israel)"
        supportedLocaleNames << "hi_IN";       // "Hindi (India)"
        supportedLocaleNames << "id_ID";       // "Indonesian (Indonesia)"
        supportedLocaleNames << "it_IT";       // "Italian (Italy)"
        supportedLocaleNames << "ja_JP";       // "Japanese (Japan)"
        supportedLocaleNames << "ko_KR";       // "Korean (Korea)"
        supportedLocaleNames << "ms_MY";       // "Malay (Malaysia)"
        supportedLocaleNames << "nb_NO";       // "Norwegian Bokml (Norway)"
        supportedLocaleNames << "nl_NL";       // "Dutch (Netherlands)"
        supportedLocaleNames << "nn_NO";       // "Norwegian Nynorsk (Norway)"
        supportedLocaleNames << "pt_BR";       // "Portuguese (Brazil)"
        supportedLocaleNames << "pt_PT";       // "Portuguese (Portugal)"
        supportedLocaleNames << "ru_RU";       // "Russian (Russia)"
        supportedLocaleNames << "sv_SE";       // "Swedish (Sweden)"
        supportedLocaleNames << "th_TH";       // "Thai (Thailand)"
        supportedLocaleNames << "tr_TR";       // "Turkish (Turkey)"
        supportedLocaleNames << "uk_UA";       // "Ukrainian (Ukraine)"
        supportedLocaleNames << "ur_PK";       // "Urdu (Pakistan)"
        supportedLocaleNames << "zh_Hans_CN";  // "Chinese (Simplified Han, China)"
        supportedLocaleNames << "zh_Hant_TW";  // "Chinese (Traditional Han, Taiwan)"
        supportedLocaleNames << "no_NO";
        supportedLocaleNames << "es_419";
        supportedLocaleNames << "de_DE@collation=phonebook";
        supportedLocaleNames << "zh_CN@collation=pinyin";
        supportedLocaleNames << "zh_CN@collation=stroke";
    }
    // sort the list for easier comparison in the output
    // (i.e. es_419 should be near es, not at the end of the list):
    qSort(supportedLocaleNames.begin(), supportedLocaleNames.end());
    MCalendar::setSystemTimeZone("GMT+0");
    QDateTime dateTime(QDate(2008, 7, 21), QTime(14, 31, 0, 0), Qt::LocalTime);
    MCalendar gregorianCalendar(MLocale::GregorianCalendar);
    MCalendar islamicCalendar(MLocale::IslamicCalendar);
    gregorianCalendar.setDateTime(dateTime);
    islamicCalendar.setDateTime(dateTime);
    QStringList sortingTestList;
    sortingTestList
        // Han:
        << "あ" << "内山" << "うちやま"
        << "か" << "小中" << "こなか"
        << "さ" << "沙紀" << "さき" << "ジョン"
        << "た" << "竹山" << "竹山" << "ティロー"
        << "な" << "中村" << "なかむら" << "ナターシャ"
        << "は" << "鳩山" << "はとやま" << "ヘンリー"
        << "ま" << "松下" << "まつした" << "マイケル"
        << "や" << "山田" << "やまだ" << "ヤングマン"
        << "ら" << "楽谷" << "らくたに" << "ラースロ"
        << "わ" << "若山" << "わかやま" << "ワトソン"
        << "一" << "二" << "三" << "中" << "正"
        << "爱" << "播" << "此" << "得" << "俄" << "攀"
        // Cyrillic:
        << "Ѐ" << "А" << "а" << "Б" << "б" << "В" << "в" << "Г" << "г" << "Я" << "я"
        // Greek:
        << "Α" << "α" << "Β" << "β" << "Ω" << "ω"
        // Arabic and Farsi:
        << "ا" << "ب" << "ت" << "ث" << "ج" << "ح" << "خ" << "د" << "ذ"
        << "ر" << "ز" << "س" << "ش" << "ص" << "ض" << "ط" << "ظ" << "ع" << "غ"
        << "ف" << "ق" << "ک" << "گ" << "ك" << "ل" << "م" << "ن" << "و" << "ه" << "ي"
        // Hebrew:
        << "א" << "ב" << "ג" << "ש" << "ת"
        // Latin:
        << "Ă" << "ă" << "A" << "a" << "Ä" << "ä" << "Å" << "å" << "Á" << "á"
        << "b" << "ch" << "Ch" << "cH" << "CH" << "h" << "H" << "i" << "I"
        << "c" << "ca" << "cz" << "č" << "ča" << "čc" << "čz"
        << "cote" << "côte" << "coté" << "côté"
        << "CÔTE" << "CÔTÉ" << "COTÉ" << "COTE"
        << "O" << "o" << "Ö" << "ö" << "öfgh" << "oegh"
        << "d" << "da" << "dz" << "ď" << "ďa" << "ďz"
        << "e" << "ea" << "ez" << "ě" << "ěa" << "ěz"
        << "n" << "na" << "nz" << "ň" << "ňa" << "ňz"
        << "r" << "ra" << "rz" << "ř" << "řa" << "řz"
        << "s" << "sa" << "sz" << "š" << "ša" << "šz"
        << "t" << "ta" << "tz" << "ť" << "ťa" << "ťz"
        << "U" << "ú" << "Ů" << "ů" << "N" << "Ň" << "ň"
        << "z" << "za" << "zz" << "ž" << "ža"<< "žz"
        ;
    QString ft_localesTestOutput = "";
    foreach(QString supportedLocaleName, supportedLocaleNames) {
        MLocale locale(supportedLocaleName);
        QCOMPARE(MLocale::dataPaths(), (QStringList() << "/usr/share/meegotouch/icu"));
        locale.setTimeFormat24h(MLocale::LocaleDefaultTimeFormat24h);
        QCOMPARE(locale.timeFormat24h(), MLocale::LocaleDefaultTimeFormat24h);
        qSort(sortingTestList.begin(), sortingTestList.end(), locale.collator());
        QString newLinePlusSupportedLocaleName('\n' + supportedLocaleName);
        ft_localesTestOutput
            += supportedLocaleName + "\tLanguage endonym\t"
            + locale.languageEndonym()
            + newLinePlusSupportedLocaleName + "\tCountry endonym\t"
            + locale.countryEndonym()
            + newLinePlusSupportedLocaleName + "\tFloating Point Number\t"
            + locale.formatNumber(-123456789.01234)
            + newLinePlusSupportedLocaleName + "\tPercent value\t"
            + locale.formatPercent(-12.3456789, 4)
            + newLinePlusSupportedLocaleName + "\tCurrency value\t"
            + locale.formatCurrency(1234.56, "EUR")
            + newLinePlusSupportedLocaleName + "\tNegative Currency value\t"
            + locale.formatCurrency(-1234.56, "EUR");
        for (unsigned i = 0; i < 2; ++i) {
            MLocale::CalendarType calendarType;
            MCalendar calendar;
            QString calendarTypeString;
            switch (i) {
            case 0:
                calendarType = MLocale::GregorianCalendar;
                calendarTypeString = "Gregorian calendar";
                calendar = gregorianCalendar;
                break;
            case 1:
                calendarType = MLocale::IslamicCalendar;
                calendarTypeString = "Islamic calendar";
                calendar = islamicCalendar;
                break;
            }
            for (unsigned i = 0; i < 2; ++i) {
                MLocale::DateSymbolContext dateSymbolContext;
                QString dateSymbolContextString;
                switch (i) {
                case 0:
                    dateSymbolContext = MLocale::DateSymbolStandalone;
                    dateSymbolContextString = "stand-alone";
                    break;
                case 1:
                    dateSymbolContext = MLocale::DateSymbolFormat;
                    dateSymbolContextString = "format";
                    break;
                }
                for (unsigned j = 0; j < 3; ++j) {
                    MLocale::DateSymbolLength dateSymbolLength;
                    QString dateSymbolLengthString;
                    switch (j) {
                    case 0:
                        dateSymbolLength = MLocale::DateSymbolWide;
                        dateSymbolLengthString = "wide";
                        break;
                    case 1:
                        dateSymbolLength = MLocale::DateSymbolAbbreviated;
                        dateSymbolLengthString = "abbreviated";
                        break;
                    case 2:
                        dateSymbolLength = MLocale::DateSymbolNarrow;
                        dateSymbolLengthString = "narrow";
                        break;
                    }
                    for (int monthNumber = 1; monthNumber <= 12; ++monthNumber) {
                        ft_localesTestOutput
                            += newLinePlusSupportedLocaleName
                            + "\tName of month " + QString("%1").arg(monthNumber, int(2), int(10), QLatin1Char('0'))
                            + " (" + calendarTypeString + "), "
                            + dateSymbolContextString + ", "
                            + dateSymbolLengthString + QLatin1Char('\t')
                            + locale.monthName(calendar, monthNumber, dateSymbolContext, dateSymbolLength);
                    }
                }
            }
        }
        for (unsigned i = 0; i < 2; ++i) {
            MLocale::CalendarType calendarType;
            MCalendar calendar;
            QString calendarTypeString;
            switch (i) {
            case 0:
                calendarType = MLocale::GregorianCalendar;
                calendarTypeString = "Gregorian calendar";
                calendar = gregorianCalendar;
                break;
            case 1:
                calendarType = MLocale::IslamicCalendar;
                calendarTypeString = "Islamic calendar";
                calendar = islamicCalendar;
                break;
            }
            for (unsigned i = 0; i < 2; ++i) {
                MLocale::DateSymbolContext dateSymbolContext;
                QString dateSymbolContextString;
                switch (i) {
                case 0:
                    dateSymbolContext = MLocale::DateSymbolStandalone;
                    dateSymbolContextString = "stand-alone";
                    break;
                case 1:
                    dateSymbolContext = MLocale::DateSymbolFormat;
                    dateSymbolContextString = "format";
                    break;
                }
                for (unsigned j = 0; j < 3; ++j) {
                    MLocale::DateSymbolLength dateSymbolLength;
                    QString dateSymbolLengthString;
                    switch (j) {
                    case 0:
                        dateSymbolLength = MLocale::DateSymbolWide;
                        dateSymbolLengthString = "wide";
                        break;
                    case 1:
                        dateSymbolLength = MLocale::DateSymbolAbbreviated;
                        dateSymbolLengthString = "abbreviated";
                        break;
                    case 2:
                        dateSymbolLength = MLocale::DateSymbolNarrow;
                        dateSymbolLengthString = "narrow";
                        break;
                    }
                    for (int weekDayNumber = 1; weekDayNumber <= 7; ++weekDayNumber) {
                        ft_localesTestOutput
                            += newLinePlusSupportedLocaleName
                            + "\tName of weekday " + QString("%1").arg(weekDayNumber, int(2), int(10), QLatin1Char('0'))
                            + " (" + calendarTypeString + "), "
                            + dateSymbolContextString + ", "
                            + dateSymbolLengthString + QLatin1Char('\t')
                            + locale.weekdayName(calendar, weekDayNumber, dateSymbolContext, dateSymbolLength);
                    }
                }
            }
        }
        ft_localesTestOutput
            += newLinePlusSupportedLocaleName + "\tCollation sample\t"
            + sortingTestList.join(" ")
            + newLinePlusSupportedLocaleName + "\tExemplar characters index\t"
            + '[' + locale.exemplarCharactersIndex().join(" ") + ']'
            + newLinePlusSupportedLocaleName + "\tScripts used\t"
            + locale.localeScripts().join(" ")
            + newLinePlusSupportedLocaleName + "\t%r (locale specific 12 hour “TimeShort”)\t"
            + locale.formatDateTime(gregorianCalendar, "%r")
            + newLinePlusSupportedLocaleName + "\t%R (locale specific 24 hour “TimeShort”)\t"
            + locale.formatDateTime(gregorianCalendar, "%R");
        for (unsigned i = 0; i < 2; ++i) {
            MLocale::CalendarType calendarType;
            MCalendar calendar;
            QString calendarTypeString;
            switch (i) {
            case 0:
                calendarType = MLocale::GregorianCalendar;
                calendarTypeString = "Gregorian calendar";
                calendar = gregorianCalendar;
                break;
            case 1:
                calendarType = MLocale::IslamicCalendar;
                calendarTypeString = "Islamic calendar";
                calendar = islamicCalendar;
                break;
            }
            locale.setCalendarType(calendarType);
            for (unsigned dateType = MLocale::DateNone;
                 dateType <= MLocale::DateFull;
                 ++dateType) {
                QString dateTypeString;
                switch (dateType) {
                case 0:
                    dateTypeString = "dateNone";
                    break;
                case 1:
                    dateTypeString = "dateShort";
                    break;
                case 2:
                    dateTypeString = "dateMedium";
                    break;
                case 3:
                    dateTypeString = "dateLong";
                    break;
                case 4:
                    dateTypeString = "dateFull";
                    break;
                }
                for (unsigned timeType = MLocale::TimeNone;
                     timeType <= MLocale::TimeFull;
                     ++timeType) {
                    QString timeTypeString;
                    switch (timeType) {
                    case 0:
                        timeTypeString = "timeNone";
                        break;
                    case 1:
                        timeTypeString = "timeShort";
                        break;
                    case 2:
                        timeTypeString = "timeMedium";
                        break;
                    case 3:
                        timeTypeString = "timeLong";
                        break;
                    case 4:
                        timeTypeString = "timeFull";
                        break;
                    }
                    if (dateType == MLocale::DateNone && timeType == MLocale::TimeNone)
                        continue;
                    for (int hourMode = 0; hourMode < 2; ++hourMode) {
                        MLocale::TimeFormat24h timeFormat24h = MLocale::LocaleDefaultTimeFormat24h;
                        locale.setTimeFormat24h(timeFormat24h);
                        QCOMPARE(locale.timeFormat24h(), timeFormat24h);
                        QString icuFormatStringDefault
                            = locale.icuFormatString(static_cast<MLocale::DateType>(dateType),
                                                     static_cast<MLocale::TimeType>(timeType),
                                                     calendarType);
                        QString formattedDateTimeDefault
                            = locale.formatDateTime(calendar,
                                                    static_cast<MLocale::DateType>(dateType),
                                                    static_cast<MLocale::TimeType>(timeType));
                        QString icuFormatStringForced;
                        QString formattedDateTimeForced;
                        QString timeFormat24hString;
                        switch (hourMode) {
                        case 0:
                            timeFormat24h = MLocale::TwelveHourTimeFormat24h;
                            locale.setTimeFormat24h(timeFormat24h);
                            QCOMPARE(locale.timeFormat24h(), timeFormat24h);
                            icuFormatStringForced
                                = locale.icuFormatString(static_cast<MLocale::DateType>(dateType),
                                                         static_cast<MLocale::TimeType>(timeType),
                                                         calendarType);
                            formattedDateTimeForced
                                = locale.formatDateTime(calendar,
                                                        static_cast<MLocale::DateType>(dateType),
                                                        static_cast<MLocale::TimeType>(timeType));
                            if (locale.defaultTimeFormat24h() == MLocale::TwelveHourTimeFormat24h) {
                                QCOMPARE(icuFormatStringDefault, icuFormatStringForced);
                                timeFormat24hString = "24/12 hour mode: 12 (default)";
                            }
                            else {
                                timeFormat24hString = "24/12 hour mode: 12          ";
                            }
                            break;
                        case 1:
                            timeFormat24h = MLocale::TwentyFourHourTimeFormat24h;
                            locale.setTimeFormat24h(timeFormat24h);
                            QCOMPARE(locale.timeFormat24h(), timeFormat24h);
                            icuFormatStringForced
                                = locale.icuFormatString(static_cast<MLocale::DateType>(dateType),
                                                         static_cast<MLocale::TimeType>(timeType),
                                                         calendarType);
                            formattedDateTimeForced
                                = locale.formatDateTime(calendar,
                                                        static_cast<MLocale::DateType>(dateType),
                                                        static_cast<MLocale::TimeType>(timeType));
                            if (locale.defaultTimeFormat24h() == MLocale::TwentyFourHourTimeFormat24h) {
                                QCOMPARE(icuFormatStringDefault, icuFormatStringForced);
                                timeFormat24hString = "24/12 hour mode: 24 (default)";
                            }
                            else {
                                timeFormat24hString = "24/12 hour mode: 24          ";
                            }
                            break;
                        }
                        ft_localesTestOutput +=
                            newLinePlusSupportedLocaleName
                            + QLatin1Char('\t')
                            + dateTypeString + ", " + timeTypeString + ", "
                            + calendarTypeString + ", " + timeFormat24hString
                            + QLatin1Char('\t')
                            + icuFormatStringForced
                            + " -> "
                            + formattedDateTimeForced;
                    }
                }
            }
        }
        ft_localesTestOutput += '\n';
    }
    QString ft_localesTestOutputFileName =
        "/tmp/ft_locales-test-output.txt";
    QFile ft_localesTestOutputFile(ft_localesTestOutputFileName);
    if (!ft_localesTestOutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        QFAIL(qPrintable("could not open file " + ft_localesTestOutputFileName));
    int bytesWritten = ft_localesTestOutputFile.write(ft_localesTestOutput.toUtf8().constData());
    if (bytesWritten == -1)
        QFAIL(qPrintable("could not write to file" + ft_localesTestOutputFileName));
    QCOMPARE(uint(bytesWritten), qstrlen(ft_localesTestOutput.toUtf8().constData()));
    ft_localesTestOutputFile.close();

    QString ft_localesTestInputFileName;
    if (runFullTest)
        ft_localesTestInputFileName =
            qApp->applicationDirPath() + QDir::separator() + "ft_locales-test-input-full-icu-" + U_ICU_VERSION_SHORT + ".txt";
    else
        ft_localesTestInputFileName =
            qApp->applicationDirPath() + QDir::separator() + "ft_locales-test-input-icu-" + U_ICU_VERSION_SHORT + ".txt";
    QFile ft_localesTestInputFile(ft_localesTestInputFileName);
    if (!ft_localesTestInputFile.open(QIODevice::ReadOnly))
        QFAIL(qPrintable("could not open file " + ft_localesTestInputFileName));
    QString ft_localesTestInput = QString::fromUtf8(ft_localesTestInputFile.readAll().constData());
    ft_localesTestInputFile.close();
    
    // QTextStream debugStream(stdout);
    // debugStream.setCodec("UTF-8");
    // debugStream << ft_localesTestInput;

    if (ft_localesTestOutput != ft_localesTestInput) {
        QProcess::execute("diff -u " + ft_localesTestInputFileName + ' ' + ft_localesTestOutputFileName);
//        QFAIL(qPrintable("files " + ft_localesTestOutputFileName + " and " + ft_localesTestInputFileName + " differ."));
    }
}

QTEST_APPLESS_MAIN(Ft_Locales);

