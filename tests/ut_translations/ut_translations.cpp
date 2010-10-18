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

#include "ut_translations.h"
#include <MGConfItem>
#include <MApplication>

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

void Ut_Translations::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "ut_translations" };
    qap = new MApplication(argc, argv, "test");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Ut_Translations::cleanupTestCase()
{
    delete qap;
}

void Ut_Translations::init()
{
    // could also use: QCoreApplication::applicationDirPath()
    // but it seems to have some problems under scratchbox
    MLocale::setTranslationPaths(
        (QStringList()
         << qApp->applicationDirPath() + "/translations-tr"
         << qApp->applicationDirPath() + "/translations-qttrid"));
}

void Ut_Translations::cleanup()
{
}

void Ut_Translations::testTranslationPathModificationMethods()
{
    MLocale::setTranslationPaths(QStringList());
    QCOMPARE(MLocale::translationPaths().size(), 0);
    MLocale::setTranslationPaths((QStringList() << "/foo" << "/bar"));
    QCOMPARE(MLocale::translationPaths().size(), 2);
    QCOMPARE(MLocale::translationPaths(),
             (QStringList() << "/foo" << "/bar"));
    MLocale::addTranslationPath("/baz");
    QCOMPARE(MLocale::translationPaths().size(), 3);
    QCOMPARE(MLocale::translationPaths(),
             (QStringList() << "/foo" << "/bar" << "/baz"));
    MLocale::addTranslationPath("/baz");
    QCOMPARE(MLocale::translationPaths().size(), 3);
    QCOMPARE(MLocale::translationPaths(),
             (QStringList() << "/foo" << "/bar" << "/baz"));
    MLocale::removeTranslationPath("/baz");
    QCOMPARE(MLocale::translationPaths().size(), 2);
    QCOMPARE(MLocale::translationPaths(),
             (QStringList() << "/foo" << "/bar"));
}

void Ut_Translations::testinstallTrCatalogThenUseQtTr_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("catalog");
    QTest::addColumn<QString>("string_id");
    QTest::addColumn<QString>("translation");

    QTest::newRow("fi_FI") << QString("fi_FI")
                           << QString("ut_translations-tr")
                           << QString("hello_str")
                           << QString("Moi maailma");
    QTest::newRow("en_GB") << QString("en_GB")
                           << QString("ut_translations-tr")
                           << QString("hello_str")
                           << QString("Hello world");
}

void Ut_Translations::testinstallTrCatalogThenUseQtTr()
{
    QFETCH(QString, localeName);
    QFETCH(QString, catalog);
    QFETCH(QString, string_id);
    QFETCH(QString, translation);

    QString context("Ut_Translations");
    MLocale locale(localeName);
    locale.removeTrCatalog(catalog);
    MLocale::setDefault(locale);
    // Everything should be untranslated now:
    QCOMPARE(locale.translate(qPrintable(context), qPrintable(string_id)),
             string_id);
    QCOMPARE(tr(qPrintable(string_id)), string_id);
    locale.installTrCatalog(catalog);
    // The .ts file we are using here uses
    // <context>
    //    <name>Ut_Translations</name>
    // ...
    // therefore we have to give "Ut_Translations" as context
    // to locale.translate():
    QCOMPARE(locale.translate(qPrintable(context), qPrintable(string_id)),
             translation);
    // Make the locale the default to enable the message catalog also for
    // tr():
    MLocale::setDefault(locale);
    // Note that the context for tr() is automatically
    // "Ut_Translations"
    QCOMPARE(tr(qPrintable(string_id)), translation);

    locale.removeTrCatalog(catalog);
    MLocale::setDefault(locale);
    // Everything should be untranslated again now:
    QCOMPARE(locale.translate(qPrintable(context), qPrintable(string_id)),
             string_id);
    QCOMPARE(tr(qPrintable(string_id)), string_id);
}

void Ut_Translations::testOriginalQtTr_data()
{
    QTest::addColumn<QString>("catalog");
    QTest::addColumn<QString>("string_id");
    QTest::addColumn<QString>("translation");

    QTest::newRow("fi_FI") << QString(qApp->applicationDirPath() + "/translations-tr/ut_translations-tr_fi_FI.qm")
                           << QString("hello_str")
                           << QString("Moi maailma");
    QTest::newRow("en_GB") << QString(qApp->applicationDirPath() + "/translations-tr/ut_translations-tr_en_GB.qm")
                           << QString("hello_str")
                           << QString("Hello world");
}

void Ut_Translations::testOriginalQtTr()
{
    QFETCH(QString, catalog);
    QFETCH(QString, string_id);
    QFETCH(QString, translation);

    // Note that the context for tr() is "Ut_Translations", i.e.
    // the section
    //    <context>
    //      <name>Ut_Translations</name>
    //         ...
    //    </context>
    // from the .ts file is used here.

    // Everything should be untranslated now:
    QCOMPARE(tr(qPrintable(string_id)), string_id);

    QTranslator translator;
    translator.load(catalog);
    qap->installTranslator(&translator);
    QCOMPARE(tr(qPrintable(string_id)), translation);

    qap->removeTranslator(&translator);
    // Everything should be untranslated again now:
    QCOMPARE(tr(qPrintable(string_id)), string_id);
}

void Ut_Translations::testQtTrId_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<QString>("engEnglish");
    QTest::addColumn<QString>("translation");

    QTest::newRow("en_GB untranslated")
            << QString("en_GB")
            //% "Untranslated message (engineering English)"
            << QString(QT_TRID_NOOP("xx_untranslated_message"))
            << QString("!! Untranslated message (engineering English)")
            << QString("!! Untranslated message (engineering English)");

    QTest::newRow("en_GB translated")
            << QString("en_GB")
            //% "Translated message (engineering English)"
            << QString(QT_TRID_NOOP("xx_translated_message"))
            << QString("!! Translated message (engineering English)")
            << QString("Translated message");

    QTest::newRow("de_DE untranslated")
            << QString("de_DE")
            //% "Untranslated message (engineering English)"
            << QString(QT_TRID_NOOP("xx_untranslated_message"))
            << QString("!! Untranslated message (engineering English)")
            << QString("!! Untranslated message (engineering English)");

    QTest::newRow("de_DE translated")
            << QString("de_DE")
            //% "Translated message (engineering English)"
            << QString(QT_TRID_NOOP("xx_translated_message"))
            << QString("!! Translated message (engineering English)")
            << QString("Übersetzte Meldung");
}

void Ut_Translations::testQtTrId()
{
    QFETCH(QString, localeName);
    QFETCH(QString, messageId);
    QFETCH(QString, engEnglish);
    QFETCH(QString, translation);

    MLocale locale(localeName);
    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated now:
    QCOMPARE(qtTrId(qPrintable(messageId)), messageId);

    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("ut_translations-qttrid.qm");
    QCOMPARE(locale.translate("", qPrintable(messageId)), engEnglish);

    MLocale::setDefault(locale);
    // now qtTrId should return the engineering English:
    QCOMPARE(qtTrId(qPrintable(messageId)), engEnglish);

    // installs the catalog for the real translation:
    locale.installTrCatalog("ut_translations-qttrid");
    QCOMPARE(locale.translate("", qPrintable(messageId)), translation);

    MLocale::setDefault(locale);
    // now qtTrId should return the real translation, unless
    // the message is not translated at all:
    QCOMPARE(qtTrId(qPrintable(messageId)), translation);

    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated again now:
    QCOMPARE(locale.translate("", qPrintable(messageId)), messageId);
    QCOMPARE(qtTrId(qPrintable(messageId)), messageId);
}

void Ut_Translations::testQtTrIdLocalizedNumbers_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<double>("number");
    QTest::addColumn<QString>("engEnglish");
    QTest::addColumn<QString>("translation");

    QTest::newRow("en_GB 4711")
            << QString("en_GB")
            //% "Big localized number: %L1 (engineering English)"
            << QString(QT_TRID_NOOP("id_%L1_big_number"))
            << 4711.0
            << QString("!! Big localized number: %L1 (engineering English)")
            << QString("Big localized number: 4,711");

    QTest::newRow("en_GB 4711 nonlocal")
            << QString("en_GB")
            //% "Big non-localized number: %L1 (engineering English)"
            << QString(QT_TRID_NOOP("id_%L1_big_nonlocal_number"))
            << 4711.0
            << QString("!! Big non-localized number: %L1 (engineering English)")
            << QString("Big non-localized number: 4711");

    QTest::newRow("ar_EG 4711")
            << QString("ar_EG")
            //% "Big localized number: %L1 (engineering English)"
            << QString(QT_TRID_NOOP("id_%L1_big_number"))
            << 4711.0
            << QString("!! Big localized number: %L1 (engineering English)")
            << QString("Big localized number: ٤٬٧١١");

    QTest::newRow("ar_EG 4711 nonlocal")
            << QString("ar_EG")
            //% "Big non-localized number: %L1 (engineering English)"
            << QString(QT_TRID_NOOP("id_%L1_big_nonlocal_number"))
            << 4711.0
            << QString("!! Big non-localized number: %L1 (engineering English)")
            << QString("Big non-localized number: 4711");
}

void Ut_Translations::testQtTrIdLocalizedNumbers()
{
    QFETCH(QString, localeName);
    QFETCH(QString, messageId);
    QFETCH(double, number);
    QFETCH(QString, engEnglish);
    QFETCH(QString, translation);

    MLocale locale(localeName);
    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated now:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(number), messageId.arg(number));

    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("ut_translations-qttrid.qm");
    QCOMPARE(locale.translate("", qPrintable(messageId)).arg(number),
             engEnglish.arg(number));

    MLocale::setDefault(locale);
    // now qtTrId should return the engineering English:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(number), engEnglish.arg(number));

    // installs the catalog for the real translation:
    locale.installTrCatalog("ut_translations-qttrid");
    QCOMPARE(locale.translate("", qPrintable(messageId)).arg(number), translation);

    MLocale::setDefault(locale);
    // now qtTrId should return the real translation, unless
    // the message is not translated at all:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(number), translation);

    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated again now:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(number), messageId.arg(number));
}

void Ut_Translations::testQtTrIdLocalizedNumbersPlural_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<int>("number");
    QTest::addColumn<QString>("engEnglish");
    QTest::addColumn<QString>("engEnglishLocalizedNumber");
    QTest::addColumn<QString>("translation");

    // The following 2 qtTrId() lines are dummies to make lupdate work
    // (QT_TRID_NOOP has no support for plural apparently):

    //% "%Ln messages received."
    qtTrId("id_%Ln_messages_received.", 0);
    //% "%Ln non-local messages received."
    qtTrId("id_%Ln_nonlocal_messages_received.", 0);

    QTest::newRow("en_GB 0")
            << QString("en_GB")
            << QString("id_%Ln_messages_received.")
            << 0
            << QString("!! 0 messages received.")
            << QString("!! 0 messages received.")
            << QString("0 messages received.");
    QTest::newRow("en_GB 1")
            << QString("en_GB")
            << QString("id_%Ln_messages_received.")
            << 1
            << QString("!! 1 messages received.")
            << QString("!! 1 messages received.")
            << QString("1 message received.");
    QTest::newRow("en_GB 2")
            << QString("en_GB")
            << QString("id_%Ln_messages_received.")
            << 2
            << QString("!! 2 messages received.")
            << QString("!! 2 messages received.")
            << QString("2 messages received.");

    QTest::newRow("en_GB 0 nonlocal")
            << QString("en_GB")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 0
            << QString("!! 0 non-local messages received.")
            << QString("!! 0 non-local messages received.")
            << QString("0 non-local messages received.");
    QTest::newRow("en_GB 1 nonlocal")
            << QString("en_GB")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 1
            << QString("!! 1 non-local messages received.")
            << QString("!! 1 non-local messages received.")
            << QString("1 non-local message received.");
    QTest::newRow("en_GB 2 nonlocal")
            << QString("en_GB")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 2
            << QString("!! 2 non-local messages received.")
            << QString("!! 2 non-local messages received.")
            << QString("2 non-local messages received.");

    QTest::newRow("ar_EG 0")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 0
            << QString("!! 0 messages received.")
            << QString("!! ٠ messages received.")
            << QString("1st translation: ٠");
    QTest::newRow("ar_EG 1")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 1
            << QString("!! 1 messages received.")
            << QString("!! ١ messages received.")
            << QString("2nd translation: ١");
    QTest::newRow("ar_EG 2")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 2
            << QString("!! 2 messages received.")
            << QString("!! ٢ messages received.")
            << QString("3rd translation: ٢");
    QTest::newRow("ar_EG 3")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 3
            << QString("!! 3 messages received.")
            << QString("!! ٣ messages received.")
            << QString("4th translation: ٣");
    QTest::newRow("ar_EG 4")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 4
            << QString("!! 4 messages received.")
            << QString("!! ٤ messages received.")
            << QString("4th translation: ٤");
    QTest::newRow("ar_EG 5")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 5
            << QString("!! 5 messages received.")
            << QString("!! ٥ messages received.")
            << QString("4th translation: ٥");
    QTest::newRow("ar_EG 6")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 6
            << QString("!! 6 messages received.")
            << QString("!! ٦ messages received.")
            << QString("4th translation: ٦");
    QTest::newRow("ar_EG 7")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 7
            << QString("!! 7 messages received.")
            << QString("!! ٧ messages received.")
            << QString("4th translation: ٧");
    QTest::newRow("ar_EG 8")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 8
            << QString("!! 8 messages received.")
            << QString("!! ٨ messages received.")
            << QString("4th translation: ٨");
    QTest::newRow("ar_EG 9")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 9
            << QString("!! 9 messages received.")
            << QString("!! ٩ messages received.")
            << QString("4th translation: ٩");
    QTest::newRow("ar_EG 10")
            << QString("ar_EG")
            << QString("id_%Ln_messages_received.")
            << 10
            << QString("!! 10 messages received.")
            << QString("!! ١٠ messages received.")
            << QString("4th translation: ١٠");

    QTest::newRow("ar_EG 0 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 0
            << QString("!! 0 non-local messages received.")
            << QString("!! ٠ non-local messages received.")
            << QString("1st non-local translation: 0");
    QTest::newRow("ar_EG 1 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 1
            << QString("!! 1 non-local messages received.")
            << QString("!! ١ non-local messages received.")
            << QString("2nd non-local translation: 1");
    QTest::newRow("ar_EG 2 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 2
            << QString("!! 2 non-local messages received.")
            << QString("!! ٢ non-local messages received.")
            << QString("3rd non-local translation: 2");
    QTest::newRow("ar_EG 3 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 3
            << QString("!! 3 non-local messages received.")
            << QString("!! ٣ non-local messages received.")
            << QString("4th non-local translation: 3");
    QTest::newRow("ar_EG 4 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 4
            << QString("!! 4 non-local messages received.")
            << QString("!! ٤ non-local messages received.")
            << QString("4th non-local translation: 4");
    QTest::newRow("ar_EG 5 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 5
            << QString("!! 5 non-local messages received.")
            << QString("!! ٥ non-local messages received.")
            << QString("4th non-local translation: 5");
    QTest::newRow("ar_EG 6 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 6
            << QString("!! 6 non-local messages received.")
            << QString("!! ٦ non-local messages received.")
            << QString("4th non-local translation: 6");
    QTest::newRow("ar_EG 7 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 7
            << QString("!! 7 non-local messages received.")
            << QString("!! ٧ non-local messages received.")
            << QString("4th non-local translation: 7");
    QTest::newRow("ar_EG 8 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 8
            << QString("!! 8 non-local messages received.")
            << QString("!! ٨ non-local messages received.")
            << QString("4th non-local translation: 8");
    QTest::newRow("ar_EG 9 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 9
            << QString("!! 9 non-local messages received.")
            << QString("!! ٩ non-local messages received.")
            << QString("4th non-local translation: 9");
    QTest::newRow("ar_EG 10 nonlocal")
            << QString("ar_EG")
            << QString("id_%Ln_nonlocal_messages_received.")
            << 10
            << QString("!! 10 non-local messages received.")
            << QString("!! ١٠ non-local messages received.")
            << QString("4th non-local translation: 10");
}

void Ut_Translations::testQtTrIdLocalizedNumbersPlural()
{
    QFETCH(QString, localeName);
    QFETCH(QString, messageId);
    QFETCH(int, number);
    QFETCH(QString, engEnglish);
    QFETCH(QString, engEnglishLocalizedNumber);
    QFETCH(QString, translation);

    QString messageIdNumberReplaced = messageId;
    messageIdNumberReplaced.replace("%Ln", QString::number(number));

    MLocale enLocaleWithoutTranslations("en_GB");
    // removes the catalogs for both the engineering English and the
    // real translation:
    enLocaleWithoutTranslations.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(enLocaleWithoutTranslations);
    // Everything should be untranslated now:
    QCOMPARE(qtTrId(qPrintable(messageId), number), messageIdNumberReplaced);

    MLocale locale(localeName);
    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("ut_translations-qttrid.qm");
    QCOMPARE(locale.translate("", qPrintable(messageId), "", number), engEnglish);
    MLocale::setDefault(locale);

    // now qtTrId should return the engineering English:
    QCOMPARE(qtTrId(qPrintable(messageId), number), engEnglishLocalizedNumber);

    // installs the catalog for the real translation:
    locale.installTrCatalog("ut_translations-qttrid");
    QCOMPARE(locale.translate("", qPrintable(messageId), "", number), translation);

    MLocale::setDefault(locale);
    // now qtTrId should return the real translation, unless
    // the message is not translated at all:
    QCOMPARE(qtTrId(qPrintable(messageId), number), translation);

    // removes the catalogs for both the engineering English and the
    // real translation again:
    MLocale::setDefault(enLocaleWithoutTranslations);
    // Everything should be untranslated again now:
    QCOMPARE(qtTrId(qPrintable(messageId), number), messageIdNumberReplaced);
}

void Ut_Translations::testQtTrIdMultipleVariable_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<QString>("variable1");
    QTest::addColumn<QString>("variable2");
    QTest::addColumn<QString>("engEnglish");
    QTest::addColumn<QString>("translation");

    QTest::newRow("de_DE")
            << QString("de_DE")
            //% "Name: %1 City: %2"
            << QString(QT_TRID_NOOP("id_name_%1_and_%2_city"))
            << QString("Müller")
            << QString("Köln")
            << QString("!! Name: %1 City: %2")
            << QString("Name: Müller Stadt: Köln");
}

void Ut_Translations::testQtTrIdMultipleVariable()
{
    QFETCH(QString, localeName);
    QFETCH(QString, messageId);
    QFETCH(QString, variable1);
    QFETCH(QString, variable2);
    QFETCH(QString, engEnglish);
    QFETCH(QString, translation);

    MLocale locale(localeName);
    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated now:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(variable1).arg(variable2),
             messageId.arg(variable1).arg(variable2));

    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("ut_translations-qttrid.qm");
    QCOMPARE(locale.translate("", qPrintable(messageId))
             .arg(variable1).arg(variable2),
             engEnglish.arg(variable1).arg(variable2));

    MLocale::setDefault(locale);
    // now qtTrId should return the engineering English:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(variable1).arg(variable2),
             engEnglish.arg(variable1).arg(variable2));

    // installs the catalog for the real translation:
    locale.installTrCatalog("ut_translations-qttrid");
    QCOMPARE(locale.translate(
                 "", qPrintable(messageId)).arg(variable1).arg(variable2),
             translation);

    MLocale::setDefault(locale);
    // now qtTrId should return the real translation, unless
    // the message is not translated at all:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(variable1).arg(variable2),
             translation);

    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated again now:
    QCOMPARE(qtTrId(qPrintable(messageId)).arg(variable1).arg(variable2),
             messageId.arg(variable1).arg(variable2));
}

void Ut_Translations::testQtTrIdMultipleVariableWithPlural_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("messageId");
    QTest::addColumn<int>("number");
    QTest::addColumn<QString>("variable");
    QTest::addColumn<QString>("engEnglish");
    QTest::addColumn<QString>("translation");

    // The following qtTrId() line is a dummy to make lupdate work
    // (QT_TRID_NOOP has no support for plural apparently):

    //% "%Ln messages received from %1."
    qtTrId("id_%Ln_messages_received_from_%1.", 0);

    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("id_%Ln_messages_received_from_%1.")
            << 0
            << QString("Björn Müller")
            << QString("!! 0 messages received from Björn Müller.")
            << QString("0 Nachrichten erhalten von Björn Müller.");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("id_%Ln_messages_received_from_%1.")
            << 1
            << QString("Björn Müller")
            << QString("!! 1 messages received from Björn Müller.")
            << QString("1 Nachricht erhalten von Björn Müller.");
    QTest::newRow("de_DE")
            << QString("de_DE")
            << QString("id_%Ln_messages_received_from_%1.")
            << 2
            << QString("Björn Müller")
            << QString("!! 2 messages received from Björn Müller.")
            << QString("2 Nachrichten erhalten von Björn Müller.");
}

void Ut_Translations::testQtTrIdMultipleVariableWithPlural()
{
    QFETCH(QString, localeName);
    QFETCH(QString, messageId);
    QFETCH(int, number);
    QFETCH(QString, variable);
    QFETCH(QString, engEnglish);
    QFETCH(QString, translation);

    QString messageIdNumberReplaced = messageId;
    messageIdNumberReplaced.replace("%Ln", QString::number(number));

    MLocale locale(localeName);
    // removes the catalogs for both the engineering English and the
    // real translation:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated now:
    QCOMPARE(qtTrId(qPrintable(messageId), number).arg(variable),
             messageIdNumberReplaced.arg(variable));

    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("ut_translations-qttrid.qm");
    QCOMPARE(locale.translate("", qPrintable(messageId), "", number).arg(variable),
             engEnglish);

    MLocale::setDefault(locale);
    // now qtTrId should return the engineering English:
    QCOMPARE(qtTrId(qPrintable(messageId), number).arg(variable), engEnglish);

    // installs the catalog for the real translation:
    locale.installTrCatalog("ut_translations-qttrid");
    QCOMPARE(locale.translate("", qPrintable(messageId), "", number).arg(variable),
             translation);

    MLocale::setDefault(locale);
    // now qtTrId should return the real translation, unless
    // the message is not translated at all:
    QCOMPARE(qtTrId(qPrintable(messageId), number).arg(variable),
             translation);

    // removes the catalogs for both the engineering English and the
    // real translation again:
    locale.removeTrCatalog("ut_translations-qttrid");
    MLocale::setDefault(locale);
    // Everything should be untranslated now:
    QCOMPARE(qtTrId(qPrintable(messageId), number).arg(variable),
             messageIdNumberReplaced.arg(variable));
}

void Ut_Translations::testGettingTheDefaultLocaleFromTheEnvironment_data()
{
    QTest::addColumn<QString>("localeName");

    QTest::newRow("en_GB") << "en_GB";
    QTest::newRow("fi_FI") << "fi_FI";
    QTest::newRow("ru_RU") << "ru_RU";
}

void Ut_Translations::testGettingTheDefaultLocaleFromTheEnvironment()
{
    QFETCH(QString, localeName);

    MLocale::s_systemDefault = NULL;
    qputenv("LANG", qPrintable(localeName));
    QCOMPARE(QString(qgetenv("LANG")), QString(localeName));

    QString languageItemOriginalValue("");
    MLocale *locale;
    if (confIsDown()) {
        // gconf is not available. Therefore,
        // MLocale::createSystemMLocale() will get the language
        // from the LANG variable.
        locale = new MLocale();
    } else {
        // gconf is available. Therefore,
        // MLocale::createSystemMLocale() would get the language
        // from gconf. But if the language string it gets from gconf
        // is empty, it falls back to getting the language from the
        // LANG variable, as in the case when gconf is not available
        // at all. Therefore, we set the language in gconf to the
        // empty string temporarily to test whether getting the locale
        // from the LANG variable works:
        MGConfItem languageItem("/meegotouch/i18n/language");
        QString languageItemOriginalValue = languageItem.value().toString();
        languageItem.set("");
        QCOMPARE(languageItem.value().toString(), QString(""));
        locale = new MLocale();
        languageItem.set(languageItemOriginalValue);
    }

    QCOMPARE(locale->name(), localeName);
}

void Ut_Translations::testCreatingAndDestroyingLocales()
{
    MLocale locale1;
    MLocale locale2("fi_FI");
    QCOMPARE(locale2.name(), QString("fi_FI"));
    MLocale::setDefault(locale2);
    locale1.formatNumber(13);
    MLocale locale3;
    MLocale locale4("en_GB");
    QCOMPARE(locale4.name(), QString("en_GB"));
    {
        MLocale locale1;
    }
    {
        MLocale locale1;
        {
            MLocale locale2("fi");
        }
        {
            MLocale locale3;
        }
        MLocale::setDefault(locale1);
    }
}

void Ut_Translations::testInstallTrCatalogMultipleTimes()
{
    // see https://projects.maemo.org/bugzilla/show_bug.cgi?id=198551
    MLocale locale;
    QElapsedTimer timer;
    int loopCount = 100;

    timer.start();
    for (int i = 0; i < loopCount; i++) {
        locale.installTrCatalog("ut_translations-qttrid");
        MLocale::setDefault(locale);
    }
    qint64 firstLoopTime = timer.restart();
    qDebug() << "first loop time" << firstLoopTime << "milliseconds";
    for (int i = 0; i < loopCount; i++) {
        locale.installTrCatalog("ut_translations-qttrid");
        MLocale::setDefault(locale);
    }
    qint64 secondLoopTime = timer.restart();
    qDebug() << "second loop time" << secondLoopTime << "milliseconds";
    qDebug() << "second loop time / first loop time" << double(secondLoopTime)/firstLoopTime;
    QVERIFY2(double(secondLoopTime)/firstLoopTime < double(2.0),
             "second loop took much longer than the first, this should not happen!");

}

void Ut_Translations::benchmarkMLocaleConstructorAndDelete()
{
    QBENCHMARK {
        MLocale *locale = new MLocale("de_DE");
        delete locale;
    }
}

void Ut_Translations::benchmarkMLocaleConstructorAndDeleteStack()
{
    QBENCHMARK {
        MLocale locale("de_DE");
    }
}

void Ut_Translations::benchmarkQLocaleConstructorAndDelete()
{
    QBENCHMARK {
        QLocale *locale = new QLocale("de_DE");
        delete locale;
    }
}

void Ut_Translations::benchmarkQLocaleConstructorAndDeleteStack()
{
    QBENCHMARK {
        QLocale locale("de_DE");
    }
}

void Ut_Translations::benchmarkMLocaleAssignmentOperator()
{
    MLocale *locale1 = new MLocale("de_DE");
    MLocale *locale2 = new MLocale();
    QBENCHMARK {
        *locale2 = *locale1;
    }
    delete locale1;
    delete locale2;
}

void Ut_Translations::benchmarkMLocaleAssignmentOperatorStack()
{
    MLocale locale1("de_DE");
    MLocale locale2;
    QBENCHMARK {
        locale2 = locale1;
    }
}

void Ut_Translations::benchmarkQLocaleAssignmentOperator()
{
    QLocale *locale1 = new QLocale("de_DE");
    QLocale *locale2 = new QLocale();
    QBENCHMARK {
        *locale2 = *locale1;
    }
    delete locale1;
    delete locale2;
}

void Ut_Translations::benchmarkQLocaleAssignmentOperatorStack()
{
    QLocale locale1("de_DE");
    QLocale locale2;
    QBENCHMARK {
        locale2 = locale1;
    }
}

void Ut_Translations::benchmarkMLocaleCopyConstructorAndDelete()
{
    MLocale *locale1 = new MLocale("de_DE");
    QBENCHMARK {
        MLocale *locale2 = new MLocale(*locale1);
        delete locale2;
    }
    delete locale1;
}

void Ut_Translations::benchmarkMLocaleCopyConstructorAndDeleteStack()
{
    MLocale locale1("de_DE");
    QBENCHMARK {
        MLocale locale2(locale1);
    }
}

void Ut_Translations::benchmarkQLocaleCopyConstructorAndDelete()
{
    QLocale *locale1 = new QLocale("de_DE");
    QBENCHMARK {
        QLocale *locale2 = new QLocale(*locale1);
        delete locale2;
    }
    delete locale1;
}

void Ut_Translations::benchmarkQLocaleCopyConstructorAndDeleteStack()
{
    QLocale locale1("de_DE");
    QBENCHMARK {
        QLocale locale2(locale1);
    }
}

QTEST_APPLESS_MAIN(Ut_Translations);

