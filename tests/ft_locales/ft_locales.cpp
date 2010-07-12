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

class TestLocale : public MLocale
{
};

void Ft_Locales::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "ft_locales" };
    qap = new MApplication(dummyArgc, dummyArgv, "test");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
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

void Ft_Locales::testBug169305()
{
    MLocale locale1("ar_SA");
    locale1.installTrCatalog("foo");
    MLocale::setDefault(locale1);
    QCOMPARE(qApp->layoutDirection(), Qt::RightToLeft);
    MLocale locale;
    locale.installTrCatalog("foo");
    MLocale::setDefault(locale);
    QCoreApplication::processEvents();
    QCOMPARE(qApp->layoutDirection(), Qt::RightToLeft);
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
    QTest::newRow("snd") << QString("snd_AF_Arab") << QString("snd");
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
    // this isn't a valid country, or is it?:
    // should we refuse to accept something like this?
    QTest::newRow("snd") << QString("snd_Arab_Egypt") << QString("Egypt");
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
    QTest::newRow("en") << QString("en_Arab") << QString("Arab");
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

    QTest::newRow("fi") << QString("fi_FI") << QString("");
    QTest::newRow("snd") << QString("snd_AF_XXX") << QString("XXX");
    QTest::newRow("snd") << QString("snd_Arab_AF_XXX") << QString("XXX");
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
    QTest::addColumn<QString>("locale");
    QTest::addColumn<int>("direction");

    QTest::newRow("fi") << QString("fi") << (int) Qt::LeftToRight;
    QTest::newRow("ar") << QString("ar") << (int) Qt::RightToLeft;
    QTest::newRow("he") << QString("he") << (int) Qt::RightToLeft;
    QTest::newRow("ur") << QString("ur") << (int) Qt::RightToLeft;
    QTest::newRow("fa") << QString("fa") << (int) Qt::RightToLeft;
    QTest::newRow("am") << QString("am") << (int) Qt::RightToLeft;
    QTest::newRow("ps") << QString("ps") << (int) Qt::RightToLeft;
    QTest::newRow("snd") << QString("snd_Arab_AF") << (int) Qt::RightToLeft;
    QTest::newRow("snd") << QString("snd_Arab_AF_XXX") << (int) Qt::RightToLeft;
    QTest::newRow("snd") << QString("snd_Deva_AF_XXX") << (int) Qt::LeftToRight;
}

void Ft_Locales::testMLocaleTextDirection()
{
    QFETCH(QString, locale);
    QFETCH(int, direction);

    MLocale *z = new MLocale(locale);
    QCOMPARE((int)z->textDirection(), direction);
    delete z;
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

    QTest::newRow("fi")
            << QString("fi")
            << QString("suomi");
    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("suomi");
    QTest::newRow("de")
            << QString("de")
            << QString("Deutsch");
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    // de_CH needs a special fix because of
    // see http://site.icu-project.org/design/resbund/issues
    // make sure that the fix works:
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("Deutsch");
#endif
    QTest::newRow("de_AT")
            << QString("de_AT")
            << QString("Deutsch");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("Deutsch");
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << QString("日本語");
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << QString("中文");
}

void Ft_Locales::testMLocaleLanguageEndonym()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, endonym_result);
    MLocale locale(locale_name);
    QCOMPARE(locale.languageEndonym(), endonym_result);
}

void Ft_Locales::testMLocaleCountryEndonym_data()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("endonym_result");

    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << QString("Suomi");
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    // de_CH needs a special fix because of
    // see http://site.icu-project.org/design/resbund/issues
    // make sure that the fix works:
    QTest::newRow("de_CH")
            << QString("de_CH")
            << QString("Schweiz");
#endif
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
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    requiredLocaleNames << "he";          // "Hebrew"
    requiredLocaleNames << "he_IL";       // "Hebrew (Israel)"
#endif
    requiredLocaleNames << "hi";          // "Hindi"
    requiredLocaleNames << "hi_IN";       // "Hindi (India)"
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    requiredLocaleNames << "id";          // "Indonesian"
    requiredLocaleNames << "id_ID";       // "Indonesian (Indonesia)"
#endif
    requiredLocaleNames << "it";          // "Italian"
    requiredLocaleNames << "it_CH";       // "Italian (Switzerland)"
    requiredLocaleNames << "it_IT";       // "Italian (Italy)"
    requiredLocaleNames << "ja";          // "Japanese"
    requiredLocaleNames << "ja_JP";       // "Japanese (Japan)"
#if (U_ICU_VERSION_MAJOR_NUM > 4) || (U_ICU_VERSION_MAJOR_NUM == 4 && U_ICU_VERSION_MINOR_NUM >=4)
    requiredLocaleNames << "ms";          //"Malay"
    requiredLocaleNames << "ms_BN";       // "Malay (Brunei)"
    requiredLocaleNames << "ms_MY";       // "Malay (Malaysia)"
#endif
    requiredLocaleNames << "nb";          // "Norwegian Bokml"
    requiredLocaleNames << "nb_NO";       // "Norwegian Bokml (Norway)"
    requiredLocaleNames << "nl";          // "Dutch"
    requiredLocaleNames << "nl_BE";       // "Dutch (Belgium)"
    requiredLocaleNames << "nl_NL";       // "Dutch (Netherlands)"
    requiredLocaleNames << "nn";          // "Norwegian Nynorsk"
    requiredLocaleNames << "nn_NO";       // "Norwegian Nynorsk (Norway)"
    requiredLocaleNames << "pt";          // "Portuguese"
    requiredLocaleNames << "pt_BR";       // "Portuguese (Brazil)"
    requiredLocaleNames << "pt_PT";       // "Portuguese (Portugal)"
    requiredLocaleNames << "ru";          // "Russian"
    requiredLocaleNames << "ru_RU";       // "Russian (Russia)"
    requiredLocaleNames << "ru_UA";       // "Russian (Ukraine)"
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
    foreach(QString requiredLocaleName, requiredLocaleNames) {
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
    supportedLocaleNames << "zh_CN@collation=pinyin";
    supportedLocaleNames << "zh_CN@collation=stroke";
    // sort the list for easier comparision in the output
    // (i.e. es_419 should be near es, not at the end of the list):
    qSort(supportedLocaleNames.begin(), supportedLocaleNames.end());
    QDateTime dateTime(QDate(2008, 7, 21), QTime(14, 31, 0, 0), Qt::LocalTime);
    MCalendar gregorianCalendar(MLocale::GregorianCalendar);
    MCalendar islamicCalendar(MLocale::IslamicCalendar);
    gregorianCalendar.setDateTime(dateTime);
    islamicCalendar.setDateTime(dateTime);
    QStringList sortingTestList;
    sortingTestList
        // Han:
        << "一" << "二" << "三" << "中" << "正"
        << "爱" << "播" << "此" << "得" << "俄" << "攀"
        // Cyrillic:
        << "Ѐ" << "А" << "а" << "Б" << "б" << "В" << "в" << "Г" << "г" << "Я" << "я"
        // Greek:
        << "Α" << "α" << "Β" << "β" << "Ω" << "ω"
        // Arabic:
        << "ا" << "ب" << "ة" << "ظ" << "ع" << "غ"
        // Hebrew:
        << "א" << "ב" << "ג" << "ש" << "ת"
        // Latin:
        << "a" << "ä" << "å" << "á" << "b" << "öfgh" << "oegh" << "z";
    QString ft_localesTestOutput = "";
    foreach(QString supportedLocaleName, supportedLocaleNames) {
        MLocale locale(supportedLocaleName);
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
            + locale.formatCurrency(-1234.56, "EUR")
            + newLinePlusSupportedLocaleName + "\t%r (locale specific 12 hour “TimeShort”)\t"
            + locale.formatDateTime(gregorianCalendar, "%r")
            + newLinePlusSupportedLocaleName + "\t%R (locale specific 24 hour “TimeShort”)\t"
            + locale.formatDateTime(gregorianCalendar, "%R")
            + newLinePlusSupportedLocaleName + "\tDate and time short (Gregorian Calendar)\t"
            + locale.formatDateTime(gregorianCalendar,
                                    MLocale::DateShort, MLocale::TimeShort)
            + newLinePlusSupportedLocaleName + "\tDate and time medium (Gregorian Calendar)\t"
            + locale.formatDateTime(gregorianCalendar,
                                    MLocale::DateMedium, MLocale::TimeMedium)
            + newLinePlusSupportedLocaleName + "\tDate and time long (Gregorian Calendar)\t"
            + locale.formatDateTime(gregorianCalendar,
                                    MLocale::DateLong, MLocale::TimeLong)
            + newLinePlusSupportedLocaleName + "\tDate and time full (Gregorian Calendar)\t"
            + locale.formatDateTime(gregorianCalendar,
                                    MLocale::DateFull, MLocale::TimeFull)
            + newLinePlusSupportedLocaleName + "\tDate and time short (Islamic Calendar)\t"
            + locale.formatDateTime(islamicCalendar,
                                    MLocale::DateShort, MLocale::TimeShort)
            + newLinePlusSupportedLocaleName + "\tDate and time medium (Islamic Calendar)\t"
            + locale.formatDateTime(islamicCalendar,
                                    MLocale::DateMedium, MLocale::TimeMedium)
            + newLinePlusSupportedLocaleName + "\tDate and time long (Islamic Calendar)\t"
            + locale.formatDateTime(islamicCalendar,
                                    MLocale::DateLong, MLocale::TimeLong)
            + newLinePlusSupportedLocaleName + "\tDate and time full (Islamic Calendar)\t"
            + locale.formatDateTime(islamicCalendar,
                                    MLocale::DateFull, MLocale::TimeFull)
            + newLinePlusSupportedLocaleName + "\tName of month 01 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 1)
            + newLinePlusSupportedLocaleName + "\tName of month 02 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 2)
            + newLinePlusSupportedLocaleName + "\tName of month 03 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 3)
            + newLinePlusSupportedLocaleName + "\tName of month 04 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 4)
            + newLinePlusSupportedLocaleName + "\tName of month 05 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 5)
            + newLinePlusSupportedLocaleName + "\tName of month 06 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 6)
            + newLinePlusSupportedLocaleName + "\tName of month 07 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 7)
            + newLinePlusSupportedLocaleName + "\tName of month 08 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 8)
            + newLinePlusSupportedLocaleName + "\tName of month 09 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 9)
            + newLinePlusSupportedLocaleName + "\tName of month 10 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 10)
            + newLinePlusSupportedLocaleName + "\tName of month 11 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 11)
            + newLinePlusSupportedLocaleName + "\tName of month 12 (Gregorian Calendar)\t"
            + locale.monthName(gregorianCalendar, 12)
            + newLinePlusSupportedLocaleName + "\tName of month 01 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 1)
            + newLinePlusSupportedLocaleName + "\tName of month 02 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 2)
            + newLinePlusSupportedLocaleName + "\tName of month 03 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 3)
            + newLinePlusSupportedLocaleName + "\tName of month 04 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 4)
            + newLinePlusSupportedLocaleName + "\tName of month 05 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 5)
            + newLinePlusSupportedLocaleName + "\tName of month 06 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 6)
            + newLinePlusSupportedLocaleName + "\tName of month 07 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 7)
            + newLinePlusSupportedLocaleName + "\tName of month 08 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 8)
            + newLinePlusSupportedLocaleName + "\tName of month 09 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 9)
            + newLinePlusSupportedLocaleName + "\tName of month 10 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 10)
            + newLinePlusSupportedLocaleName + "\tName of month 11 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 11)
            + newLinePlusSupportedLocaleName + "\tName of month 12 (Islamic Calendar)\t"
            + locale.monthName(islamicCalendar, 12)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 1, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 2, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 3, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 4, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 5, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 6, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Gregorian Calendar), stand-alone, wide\t"
            + locale.weekdayName(gregorianCalendar, 7, MLocale::DateSymbolStandalone, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 1, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 2, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 3, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 4, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 5, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 6, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Gregorian Calendar), stand-alone, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 7, MLocale::DateSymbolStandalone, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 1, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 2, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 3, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 4, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 5, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 6, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Gregorian Calendar), stand-alone, narrow\t"
            + locale.weekdayName(gregorianCalendar, 7, MLocale::DateSymbolStandalone, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 1, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 2, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 3, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 4, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 5, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 6, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Gregorian Calendar), format, wide\t"
            + locale.weekdayName(gregorianCalendar, 7, MLocale::DateSymbolFormat, MLocale::DateSymbolWide)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 1, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 2, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 3, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 4, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 5, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 6, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Gregorian Calendar), format, abbreviated\t"
            + locale.weekdayName(gregorianCalendar, 7, MLocale::DateSymbolFormat, MLocale::DateSymbolAbbreviated)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 1, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 2, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 3, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 4, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 5, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 6, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Gregorian Calendar), format, narrow\t"
            + locale.weekdayName(gregorianCalendar, 7, MLocale::DateSymbolFormat, MLocale::DateSymbolNarrow)
            + newLinePlusSupportedLocaleName + "\tName of weekday 01 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 1)
            + newLinePlusSupportedLocaleName + "\tName of weekday 02 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 2)
            + newLinePlusSupportedLocaleName + "\tName of weekday 03 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 3)
            + newLinePlusSupportedLocaleName + "\tName of weekday 04 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 4)
            + newLinePlusSupportedLocaleName + "\tName of weekday 05 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 5)
            + newLinePlusSupportedLocaleName + "\tName of weekday 06 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 6)
            + newLinePlusSupportedLocaleName + "\tName of weekday 07 (Islamic Calendar)\t"
            + locale.weekdayName(islamicCalendar, 7)
            + newLinePlusSupportedLocaleName + "\tCollation sample\t"
            + sortingTestList.join(" ")
            + newLinePlusSupportedLocaleName + "\tScripts used\t"
            + locale.localeScripts().join(" ")
            + '\n';
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

    QString ft_localesTestInputFileName =
        qApp->applicationDirPath() + QDir::separator() + "ft_locales-test-input-icu-" + U_ICU_VERSION + ".txt";
    QFile ft_localesTestInputFile(ft_localesTestInputFileName);
    if (!ft_localesTestInputFile.open(QIODevice::ReadOnly))
        QFAIL(qPrintable("could not open file " + ft_localesTestInputFileName));
    QString ft_localesTestInput = QString::fromUtf8(ft_localesTestInputFile.readAll().constData());
    ft_localesTestInputFile.close();
    
    // QTextStream debugStream(stderr);
    // debugStream.setCodec("UTF-8");
    // debugStream << ft_localesTestInput;

    if (ft_localesTestOutput != ft_localesTestInput) {
        QProcess::execute("diff -u " + ft_localesTestInputFileName + ' ' + ft_localesTestOutputFileName);
//        QFAIL(qPrintable("files " + ft_localesTestOutputFileName + " and " + ft_localesTestInputFileName + " differ."));
    }
}

QTEST_APPLESS_MAIN(Ft_Locales);

