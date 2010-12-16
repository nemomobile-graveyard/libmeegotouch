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

#include "ft_mcharsetdetector.h"

#define VERBOSE_OUTPUT

void Ft_MCharsetDetector::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "ft_mcharsetdetector" };
    qap = new MApplication(dummyArgc, dummyArgv, "test");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Ft_MCharsetDetector::cleanupTestCase()
{
    delete qap;
}

void Ft_MCharsetDetector::init()
{
}

void Ft_MCharsetDetector::cleanup()
{
}

void Ft_MCharsetDetector::testMCharsetMatch_data()
{
    QTest::addColumn<QStringList>("names");
    QTest::addColumn<QStringList>("languages");
    QTest::addColumn<QStringList>("confidences");
    QTest::addColumn<QStringList>("namesSorted");
    QTest::addColumn<QStringList>("languagesSorted");
    QTest::addColumn<QStringList>("confidencesSorted");

    QTest::newRow("matches")

        << (QStringList()
            << "ISO-8859-1" << "ISO-8859-2" << "UTF-8" << "Shift_JIS")
        << (QStringList()
            << "de"         << "cs"         << ""      << "ja")
        << (QStringList()
            << "85"         << "97"         << "99"    << "10")

        << (QStringList()
            << "UTF-8" << "ISO-8859-2" << "ISO-8859-1" << "Shift_JIS")
        << (QStringList()
            << ""      << "cs"         << "de"         << "ja")
        << (QStringList()
            << "99"    << "97"         << "85"         << "10")
        ;
}

void Ft_MCharsetDetector::testMCharsetMatch()
{
    QFETCH(QStringList, names);
    QFETCH(QStringList, languages);
    QFETCH(QStringList, confidences);
    QFETCH(QStringList, namesSorted);
    QFETCH(QStringList, languagesSorted);
    QFETCH(QStringList, confidencesSorted);

    QList<MCharsetMatch> mCharsetMatchList;
    for(int i = 0; i < names.size(); ++i) {
        MCharsetMatch match(names[i], languages[i], confidences[i].toInt());
        mCharsetMatchList << match;
    }
    QList<MCharsetMatch> mCharsetMatchListSorted;
    for(int i = 0; i < names.size(); ++i) {
        MCharsetMatch match;
        // use the setters here to get more test coverage
        match.setName(namesSorted[i]);
        match.setLanguage(languagesSorted[i]);
        match.setConfidence(confidencesSorted[i].toInt());
        mCharsetMatchListSorted << match;
    }
    qSort(mCharsetMatchList.begin(), mCharsetMatchList.end(),
          qGreater<MCharsetMatch>());
#if defined(VERBOSE_OUTPUT)
    for(int i = 0; i < names.size(); ++i) {
        qDebug() << mCharsetMatchList[i].name()
                 << mCharsetMatchListSorted[i].name();
        qDebug() << mCharsetMatchList[i].language()
                 << mCharsetMatchListSorted[i].language();
        qDebug() << mCharsetMatchList[i].confidence()
                 << mCharsetMatchListSorted[i].confidence();
    }
#endif
    for(int i = 0; i < names.size(); ++i) {
        QCOMPARE(mCharsetMatchList[i].name(),
                 mCharsetMatchListSorted[i].name());
        QCOMPARE(mCharsetMatchList[i].language(),
                 mCharsetMatchListSorted[i].language());
        QCOMPARE(mCharsetMatchList[i].confidence(),
                 mCharsetMatchListSorted[i].confidence());
    }
}

void Ft_MCharsetDetector::testConstructors_data()
{
    QTest::addColumn<QString>("charsetName");
    QTest::addColumn<QByteArray>("byteArray");
    QTest::addColumn<QString>("textResult");
    QTest::addColumn<bool>("hasError");

    QTest::newRow("UTF-8")
        << "UTF-8"
        << QString::fromUtf8("Hello Wörld, こんにちは日本。").toUtf8()
        << QString::fromUtf8("Hello Wörld, こんにちは日本。")
        << false;

    QTest::newRow("ISO-8859-1")
        << "ISO-8859-1"
        << QString::fromUtf8("Hello Wörld, täst, Grüße.").toLatin1()
        << QString::fromUtf8("Hello Wörld, täst, Grüße.")
        << false;

    QTest::newRow("NONSENSE")
        << "NONSENSE"
        << QString::fromUtf8("Hello Wörld, こんにちは日本。").toUtf8()
        << QString()
        << true;

    QTest::newRow("UTF-8, but force detection as ISO-8859-1")
        << "ISO-8859-1"
        << QString::fromUtf8("täst本").toUtf8()
        << QString::fromUtf8("tÃ¤stæ") + QChar(0x9c) + QString::fromUtf8("¬")
        << false;

    QTest::newRow("Invalid EUC-JP, it is really Latin1")
        << "EUC-JP"
        << QString::fromUtf8("Hello Wörld, täst, Grüße.").toLatin1()
        << QString::fromUtf8("Hello W�ld, t�t, Gr�e.")
        << true;
}

void Ft_MCharsetDetector::testConstructors()
{
    QFETCH(QString, charsetName);
    QFETCH(QByteArray, byteArray);
    QFETCH(QString, textResult);
    QFETCH(bool, hasError);

    MCharsetMatch charsetMatch;
    charsetMatch.setName(charsetName);

    MCharsetDetector charsetDetector1;
    charsetDetector1.setText(byteArray);
    MCharsetDetector charsetDetector2(byteArray);
    MCharsetDetector charsetDetector3(byteArray.constData());
    MCharsetDetector charsetDetector4(byteArray.constData(), byteArray.size());

    QString result1 = charsetDetector1.text(charsetMatch);
    bool hasError1 = charsetDetector1.hasError();
    QString result2 = charsetDetector2.text(charsetMatch);
    bool hasError2 = charsetDetector2.hasError();
    QString result3 = charsetDetector3.text(charsetMatch);
    bool hasError3 = charsetDetector3.hasError();
    QString result4 = charsetDetector4.text(charsetMatch);
    bool hasError4 = charsetDetector4.hasError();

#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << "result1:    " << result1
                << " size: " << result1.size() << "\n"
                << "textResult: " << textResult
                << " size: " << textResult.size() << "\n"
                << "hasError1: " << hasError1
                << " errorString: " << charsetDetector1.errorString() << "\n";
#endif

    QCOMPARE(result1, textResult);
    QCOMPARE(result2, textResult);
    QCOMPARE(result3, textResult);
    QCOMPARE(result4, textResult);
    QCOMPARE(hasError1, hasError);
    QCOMPARE(hasError2, hasError);
    QCOMPARE(hasError3, hasError);
    QCOMPARE(hasError4, hasError);
}

void Ft_MCharsetDetector::testInputFilterEnabledDefaultValue()
{
    MCharsetDetector charsetDetector;
    // The input filter is disabled by default:
    QCOMPARE(charsetDetector.isInputFilterEnabled(), false);
}

void Ft_MCharsetDetector::testDetectableCharsets_data()
{
    QTest::addColumn<QStringList>("expectedCharsets");

    QTest::newRow("at least these charsets should be detectable")
        << (QStringList()
            << "UTF-8"
            << "UTF-16BE"
            << "UTF-16LE"
            << "UTF-32BE"
            << "UTF-32LE"
            << "ISO-8859-1"
            << "ISO-8859-2"
            << "ISO-8859-5"
            << "ISO-8859-6"
            << "ISO-8859-7"
            << "ISO-8859-8"
            << "ISO-8859-9"
            << "KOI8-R"
            << "Shift_JIS"
            << "GB18030"
            << "EUC-JP"
            << "EUC-KR"
            << "Big5"
            << "ISO-2022-JP"
            << "windows-1250"
            << "windows-1251"
            << "windows-1252"
            << "windows-1253"
            << "windows-1255"
            << "windows-1256"
            << "windows-1254")
        ;
}

void Ft_MCharsetDetector::testDetectableCharsets()
{
    QFETCH(QStringList, expectedCharsets);

    MCharsetDetector charsetDetector;
    QStringList detectableCharsets
        = charsetDetector.getAllDetectableCharsets();
#if defined(VERBOSE_OUTPUT)
    qDebug() << "detectable charsets" << detectableCharsets;
#endif
    foreach(QString cs, expectedCharsets)
        QVERIFY2(detectableCharsets.contains(cs),
                 QString("charset %1 is missing in the list of detectable charset")
                 .arg(cs).toUtf8().constData());
}

static QString makeStringLonger(const QString &str, int n)
{
    QString ret;
    for (int i = 0; i < n; ++i)
        ret += str;
    return ret;
}


void Ft_MCharsetDetector::testDetection_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("declaredLocale");
    QTest::addColumn<QString>("declaredEncoding");
    QTest::addColumn<bool>("enableInputFilter");
    QTest::addColumn<QString>("inputEncoding");
    QTest::addColumn<QString>("bestMatchName");
    QTest::addColumn<QString>("bestMatchLanguage");

    QString longGerman("Als Menschenrechte werden subjektive Rechte bezeichnet, die jedem Menschen gleichermaßen zustehen. Das Konzept der Menschenrechte geht davon aus, dass alle Menschen allein aufgrund ihres Menschseins[1] mit gleichen Rechten ausgestattet und dass diese egalitär begründeten Rechte universell, unveräußerlich und unteilbar sind. Die Idee der Menschenrechte ist eng verbunden mit dem Humanismus und der im Zeitalter der Aufklärung entwickelten Idee des Naturrechts.");
    QTest::newRow("Long German UTF-8")
        << longGerman
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long German Latin1")
        << longGerman
        << ""
        << ""
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "de";
    QTest::newRow("Short German UTF-8")
        << "Grüße"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short German UTF-8")
        << "Grüße"
        << "ja"        // declared locale, should not break it
        << "Shift_JIS" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short German Latin1")
        << "Grüße"
        << ""
        << ""
        << false
        << "ISO-8859-1"
        << "Shift_JIS" // not fixable
        << "ja";       // not fixable
    QTest::newRow("Short German Latin1")
        << "Grüße"
        << ""
        << "ISO-8859-1" // declared encoding, makes it work
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "";
    QTest::newRow("Short German Latin1")
        << "Grüße"
        << "de"         // declared locale, makes it work
        << ""
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "de";
    QTest::newRow("Short German Latin1")
        << "Grüße"
        << "de_DE"     // declared locale, makes it work
        << "" 
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "de";
    QTest::newRow("Short German Latin1")
        << "Grüße"
        << "de"         // declared locale
        << "ISO-8859-1" // declared encoding, makes it work
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "de";

    QString longFrench("Le français est une langue romane parlée comme langue maternelle principalement en France (y compris outre-mer), dont elle est originaire (la « langue d’oïl »), en Belgique (en Wallonie et à Bruxelles), dans plusieurs provinces et territoires du Canada (principalement au Québec, mais aussi en Ontario et au Nouveau-Brunswick) et en Suisse romande (le français est l'une des quatre langues officielles de la Suisse). On trouve aussi des îlots de francophones natifs aux États-Unis (notamment en Louisiane et au Maine), à Haïti, aux Seychelles, à l'île Maurice, au Vanuatu, dans certaines vallées italiennes, etc...");
    QTest::newRow("Long French ISO-8859-1")
        << longFrench
        << ""
        << ""
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "fr";

    QString htmlTest("<p></p><p></p><p></p><p style='font-family: "
                     + longFrench
                     + ";'>"
                     + longGerman
                     + "</p>");
    QTest::newRow("HTML test ISO-8859-1")
        << htmlTest
        << ""
        << ""
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "fr"; // detects as French without filtering
    QTest::newRow("HTML test ISO-8859-1")
        << htmlTest
        << ""
        << ""
        << true
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "de"; // detects as German with filtering

    QString longCzech("Český jazyk neboli čeština je západoslovanský jazyk, nejvíce příbuzný se slovenštinou, poté polštinou a lužickou srbštinou. Patří mezi slovanské jazyky, do rodiny jazyků indoevropských. Čeština se vyvinula ze západních nářečí praslovanštiny na konci 10. století. Česky psaná literatura se objevuje od 14. století. První písemné památky jsou však již z 12. století. Dělí se na spisovnou č., určenou pro oficiální styk (je kodifikována v mluvnicích a slovnících), a nespisovnou č., která zahrnuje dialekty (nářečí) a sociolekty (slangy) včetně vulgarismů a argotu. Spisovná čeština má dvě podoby: vypjatě spisovnou a hovorovou. Hovorovou češtinu je třeba odlišovat od češtiny obecné.");
    QTest::newRow("Long Czech UTF-8")
        << longCzech
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Czech Latin2")
        << longCzech
        << ""
        << ""
        << false
        << "ISO-8859-2"
        << "ISO-8859-2"
        << "cs";
    QTest::newRow("Long Czech windows-1250")
        << longCzech
        << ""
        << ""
        << false
        << "windows-1250"
        << "windows-1250"
        << "cs";
    QString shortCzech("Český");
    QTest::newRow("Short Czech UTF-8")
        << shortCzech
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Czech UTF-8")
        << shortCzech
        << "ja"        // declared locale, should not break it
        << "Shift_JIS" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Czech UTF-8")
        << shortCzech
        << "de"         // declared locale, should not break it
        << "ISO-8859-1" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Czech Latin2")
        << shortCzech
        << ""
        << ""
        << false
        << "ISO-8859-2"
        << "" // not fixable
        << "";
    QTest::newRow("Short Czech Latin2")
        << shortCzech
        << ""
        << "ISO-8859-2" // declared encoding, makes it work
        << false
        << "ISO-8859-2"
        << "ISO-8859-2"
        << "";

    QString longHebrew("השם עֵבֶר מופיע במקרא כשמו של אחד מאבותיו של אברהם אבינו. המושג \"עברי\" נזכר במקרא פעמים רבות ככינוי לבני ישראל, אולם שמה של שפתם של העברים אינו מוזכר במקרא. עם זאת, במלכים ב' ב' י\"ח,כו ובישעיהו ל\"ו,יא מסופר כי שליחי חזקיהו המלך מבקשים מרבשקה, שליחו של סנחריב מלך אשור, לדבר עמם ב\"ארמית\" ולא ב\"יהודית\", כדי שהעם (שכנראה לא דיבר ארמית) לא יבין את דבריהם; לפי זה נראה כי זה היה שמה של השפה, או לפחות שמו של הניב שדובר באזור ירושלים אשר בממלכת \"יהודה\".");
    QTest::newRow("Long Hebrew UTF-8")
        << longHebrew
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Hebrew ISO-8859-8")
        << longHebrew
        << ""
        << ""
        << false
        << "ISO-8859-8"
        << "ISO-8859-8"
        << "he";
    QTest::newRow("Short Hebrew UTF-8")
        << "עִבְרִית"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Hebrew UTF-8")
        << "עִבְרִית"
        << "de"         // declared locale, should not break it
        << "ISO-8859-1" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Hebrew ISO-8859-8")
        << "עִבְרִית"
        << ""
        << ""
        << false
        << "ISO-8859-8"
        << "ISO-8859-7" // not fixable
        << "el";        // not fixable
    QTest::newRow("Short Hebrew ISO-8859-8")
        << "עִבְרִית"
        << ""
        << "ISO-8859-8" // declared encoding, makes it work
        << false
        << "ISO-8859-8"
        << "ISO-8859-8"
        << "";
    QTest::newRow("Short Hebrew ISO-8859-8")
        << "עִבְרִית"
        << "he"          // declared locale, makes it work
        << ""
        << false
        << "ISO-8859-8"
        << "ISO-8859-8"
        << "he";
    QTest::newRow("Short Hebrew ISO-8859-8")
        << "עִבְרִית"
        << "he"         // declared locale
        << "ISO-8859-8" // declared encoding, makes it work
        << false
        << "ISO-8859-8"
        << "ISO-8859-8"
        << "he";

    QString shortJapanese("日");
    // "攀" in EUC-JP happens to be a valid UTF-8 sequence.
    QTest::newRow("Short Japanese UTF-8")
        << shortJapanese
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Japanese UTF-16")
        << shortJapanese
        << ""
        << ""
        << false
        << "UTF-16LE"
        << "UTF-16LE"
        << "";
    QTest::newRow("Short Japanese UTF-16BE")
        << shortJapanese
        << ""
        << ""
        << false
        << "UTF-16BE"
        << "UTF-16BE"
        << "";
    QTest::newRow("Short Japanese UTF-32")
        << shortJapanese
        << ""
        << ""
        << false
        << "UTF-32LE"
        << "UTF-32LE"
        << "";
    QTest::newRow("Short Japanese UTF-32BE")
        << shortJapanese
        << ""
        << ""
        << false
        << "UTF-32BE"
        << "UTF-32BE"
        << "";
    QTest::newRow("Short Japanese EUC-JP")
        << shortJapanese
        << ""
        << ""
        << false
        << "EUC-JP"
        << "EUC-JP"
        << "ja";
    QTest::newRow("Short Japanese Shift_JIS")
        << shortJapanese
        << ""
        << ""
        << false
        << "Shift_JIS"
        << "Shift_JIS"
        << "ja";
    QTest::newRow("Short Japanese ISO-2022-JP")
        << shortJapanese
        << ""
        << ""
        << false
        << "ISO-2022-JP"
        << "ISO-2022-JP"
        << "ja";

    QTest::newRow("Short Thai UTF-8")
        << "ภาษาไทย"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";

    QTest::newRow("Short Tamil UTF-8")
        << "தமிழ்"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";

    QString longGreek("Η ελληνική γλώσσα είναι μία από τις ινδοευρωπαϊκές γλώσσες, για την οποία έχουμε γραπτά κείμενα από τον 15ο αιώνα π.Χ. μέχρι σήμερα. Αποτελεί το μοναδικό μέλος ενός κλάδου της ινδοευρωπαϊκής οικογένειας γλωσσών. Ανήκει επίσης στον βαλκανικό γλωσσικό δεσμό.");
    QTest::newRow("Long Greek UTF-8")
        << longGreek
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Greek ISO-8859-7")
        << longGreek
        << ""
        << ""
        << false
        << "ISO-8859-7"
        << "ISO-8859-7"
        << "el";
    QTest::newRow("Short Greek UTF-8")
        << "ελληνικά"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Greek UTF-8")
        << "ελληνικά"
        << "el"         // declared locale, short not break it
        << "ISO-8859-7" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Greek ISO-8859-7")
        << "ελληνικά"
        << ""
        << ""
        << false
        << "ISO-8859-7"
        << "ISO-8859-6" // not fixable
        << "ar";
    QTest::newRow("Short Greek ISO-8859-7")
        << "ελληνικά"
        << ""
        << "ISO-8859-7" // declared encoding, makes it work
        << false
        << "ISO-8859-7"
        << "ISO-8859-7"
        << "";
    QTest::newRow("Short Greek ISO-8859-7")
        << "ελληνικά"
        << "el"         // declared locale, makes it work
        << ""
        << false
        << "ISO-8859-7"
        << "ISO-8859-7"
        << "el";
    QTest::newRow("Short Greek ISO-8859-7")
        << "ελληνικά"
        << "el"         // declared locale
        << "ISO-8859-7" // declared encoding, makes it work
        << false
        << "ISO-8859-7"
        << "ISO-8859-7"
        << "el";

    QString longTurkish("Türkçe veya Türkiye Türkçesi, varlığı tam olarak ispatlanamamış ortak Altay dil ailesine bağlı Türk dillerinin Oğuz öbeğine üye bir dildir. Türk dilleri ailesi bünyesindeki Oğuz öbeğinde bulunur.[5] Türkçe dünyada en fazla konuşulan 15. dildir.");
    QTest::newRow("Long Turkish UTF-8")
        << longTurkish
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Turkish ISO-8859-9")
        << longTurkish
        << ""
        << ""
        << false
        << "ISO-8859-9"
        << "ISO-8859-9"
        << "tr";
    QString shortTurkish("Türkçe");
    QTest::newRow("Short Turkish UTF-8")
        << shortTurkish
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Turkish UTF-8")
        << shortTurkish
        << "tr"         // declared language, should not break it
        << "ISO-8859-9" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Turkish ISO-8859-9")
        << shortTurkish
        << ""
        << ""
        << false
        << "ISO-8859-9"
        << "Shift_JIS" // not fixable
        << "ja";
    QTest::newRow("Short Turkish ISO-8859-9")
        << shortTurkish
        << ""
        << "ISO-8859-9" // declared encoding, makes it work
        << false
        << "ISO-8859-9"
        << "ISO-8859-9"
        << "";
    QTest::newRow("Short Turkish ISO-8859-9")
        << shortTurkish
        << "tr" // declared locale
        << ""
        << false
        << "ISO-8859-9"
        << "ISO-8859-9"
        << "tr";
    QTest::newRow("Short Turkish ISO-8859-9")
        << shortTurkish
        << "tr"         // declared locale
        << "ISO-8859-9" // declared encoding
        << false
        << "ISO-8859-9"
        << "ISO-8859-9"
        << "tr";

    QString longEnglish("English is a West Germanic language that arose in the Anglo-Saxon kingdoms of England and spread into what was to become south-east Scotland under the influence of the Anglian medieval kingdom of Northumbria.");
    QTest::newRow("Long English ASCII")
        << longEnglish
        << ""
        << ""
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "en";

    QString longEnglishWithNonAscii(
        makeStringLonger("naïve ", 3)
        + // order does not matter here
        makeStringLonger(longEnglish, 3)
        );
    QTest::newRow("Long English with non-ASCII Latin1")
        << longEnglishWithNonAscii
        << ""
        << ""
        << false
        << "ISO-8859-1"
        << "ISO-8859-1"
        << "en";
    // UTF-8 detection may fail if there are very few
    // UTF-8 characters in a text which is otherwise mostly ASCII
    // The following input is rather long ASCII with very few
    // UTF-8 characters, I added a hack to make this work as well
    // and verify that it works here.
    QTest::newRow("Long English with non-ASCII UTF-8")
        << longEnglishWithNonAscii
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";

    QString longRussian("Ру́сский язы́к — один из восточнославянских языков, один из крупнейших языков мира, национальный язык русского народа. Является самым распространённым из славянских языков и самым распространённым языком Европы как географически, так и по числу носителей языка как родного (хотя также значительная и географически бо́льшая часть русского языкового ареала находится в Азии).");
    QTest::newRow("Long Russian UTF-8")
        << longRussian
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Russian windows-1251")
        << longRussian
        << ""
        << ""
        << false
        << "windows-1251"
        << "windows-1251"
        << "ru";
    QTest::newRow("Long Russian KOI8-R")
        << longRussian
        << ""
        << ""
        << false
        << "KOI8-R"
        << "KOI8-R"
        << "ru";
    QTest::newRow("Long Russian ISO-8859-5")
        << longRussian
        << ""
        << ""
        << false
        << "ISO-8859-5"
        << "ISO-8859-5"
        << "ru";
    QTest::newRow("Short Russian UTF-8")
        << "русский"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Russian windows-1251")
        << "русский"
        << ""
        << ""
        << false
        << "windows-1251"
        << "Shift_JIS" // not fixable
        << "ja";
    QTest::newRow("Short Russian windows-1251")
        << "русский"
        << ""
        << "windows-1251" // declared encoding, makes it work
        << false
        << "windows-1251"
        << "windows-1251"
        << "";
    QTest::newRow("Short Russian windows-1251 declared locale")
        << "русский"
        << "ru" // declared locale, makes it work (accidentally!)
        << ""
        << false
        << "windows-1251"
        << "windows-1251"
        << "ru";
    QTest::newRow("Short Russian KOI8-R")
        << "русский"
        << ""
        << ""
        << false
        << "KOI8-R"
        << "Shift_JIS" // not fixable
        << "ja";
    QTest::newRow("Short Russian KOI8-R")
        << "русский"
        << ""
        << "KOI8-R" // delared encoding, makes it work
        << false
        << "KOI8-R"
        << "KOI8-R"
        << "";
    QTest::newRow("Short Russian KOI8-R declared locale")
        << "русский"
        << "ru" // declared locale, does not help, not fixable
        << ""
        << false
        << "KOI8-R"
        << "windows-1251"
        << "ru";
    QTest::newRow("Short Russian ISO-8859-5")
        << "русский"
        << ""
        << ""
        << false
        << "ISO-8859-5"
        << "Shift_JIS" // not fixable
        << "ja";
    QTest::newRow("Short Russian ISO-8859-5")
        << "русский"
        << ""
        << "ISO-8859-5" // declared encoding, makes it work
        << false
        << "ISO-8859-5"
        << "ISO-8859-5"
        << "";
    QTest::newRow("Short Russian ISO-8859-5 declared locale")
        << "русский"
        << "ru" // declared locale, does not help, not fixable
        << ""
        << false
        << "ISO-8859-5"
        << "windows-1251"
        << "ru";
    QTest::newRow("Short Russian ISO-8859-5 declared locale")
        << "русский"
        << "ru"         // declared locale
        << "ISO-8859-5" // declared encoding
        << false
        << "ISO-8859-5"
        << "ISO-8859-5"
        << "ru";

    QString longKorean("한국어(韓國語)는 한국에서 사용하는 언어로, 대한민국에서는 한국어, 한국말이라고 부른다. 조선민주주의인민공화국, 중국(조선족 위주)을 비롯한 등지에서는 조선말, 조선어(朝鮮語)로, 카자흐스탄을 비롯 중앙아시아의 고려인들 사이에서는 고려말(高麗─)로 불린다.");
    QTest::newRow("Long Korean UTF-8")
        << longKorean
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Korean EUC-KR")
        << longKorean
        << ""
        << ""
        << false
        << "EUC-KR"
        << "EUC-KR"
        << "ko";

    QTest::newRow("Short Korean UTF-8")
        << "한국말"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Korean UTF-8")
        << "한국말"
        << "de"         // declared locale, should not break it
        << "ISO-8859-1" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Korean EUC-KR")
        << "한국말"
        << ""
        << ""
        << false
        << "EUC-KR"
        << "EUC-KR"
        << "ko";
    QTest::newRow("Very short Korean UTF-8")
        << "한"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Very short Korean EUC-KR")
        << "한"
        << ""
        << ""
        << false
        << "EUC-KR"
        << "EUC-KR"
        << "ko";
    QTest::newRow("Very short Korean UTF-8")
        << "국"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Very short Korean EUC-KR")
        << "국"
        << ""
        << ""
        << false
        << "EUC-KR"
        << "EUC-KR"
        << "ko";
    QTest::newRow("Very short Korean UTF-8")
        << "말"
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Very short Korean EUC-KR")
        << "말"
        << ""
        << ""
        << false
        << "EUC-KR"
        << "EUC-KR"
        << "ko";

    QString shortArabic("مكيلع");
    QTest::newRow("Short Arabic UTF-8")
        << shortArabic
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Arabic UTF-8")
        << shortArabic
        << "de"         // declared locale, should not break it
        << "ISO-8859-1" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Arabic ISO-8859-6")
        << shortArabic
        << ""
        << ""
        << false
        << "ISO-8859-6"
        << "Shift_JIS" // not fixable
        << "ja";
    QTest::newRow("Short Arabic ISO-8859-6")
        << shortArabic
        << ""
        << "ISO-8859-6" // declared encoding, makes it work
        << false
        << "ISO-8859-6"
        << "ISO-8859-6"
        << "";
    QTest::newRow("Short Arabic ISO-8859-6")
        << shortArabic
        << "ar"         // declared locale, makes it work
        << ""
        << false
        << "ISO-8859-6"
        << "ISO-8859-6"
        << "ar";
    QTest::newRow("Short Arabic ISO-8859-6")
        << shortArabic
        << "ar"         // declared locale
        << "ISO-8859-6" // declared encoding, makes it work
        << false
        << "ISO-8859-6"
        << "ISO-8859-6"
        << "ar";

    QString longUrdu("اُردو کو پاکستان کے تمام صوبوں میں سرکاری زبان کی حیثیت حاصل ہے۔ یہ مدرسوں میں اعلٰی ثانوی جماعتوں تک لازمی مضمون کی طور پر پڑھائی جاتی ہے۔ اِس نے کروڑوں اُردو بولنے والے پیدا کردیئے ہیں جن کی زبان پنجابی، پشتو، سندھی، بلوچی، کشمیری، براہوی، چترالی وغیرہ میں سے کوئی ایک ہوتی ہے. اُردو پاکستان کی مُشترکہ زبان ہے اور یہ علاقائی زبانوں سے کئی الفاظ ضم کررہی ہے۔ اُردو کا یہ لہجہ اب پاکستانی اُردو کہلاتی ہے. یہ اَمر زبان کے بارے میں رائے تبدیل کررہی ہے جیسے اُردو بولنے والا وہ ہے جو اُردو بولتا ہے گو کہ اُس کی مادری زبان کوئی اَور زبان ہی کیوں نہ ہو. علاقائی زبانیں بھی اُردو کے الفاظ سے اثر پارہی ہیں. پاکستان میں کروڑوں افراد ایسے ہیں جن کی مادری زبان کوئی اَور ہے لیکن وہ اُردو کو بولتے اور سمجھ سکتے ہیں. پانچ ملین افغان مہاجرین، جنہوں نے پاکستان میں پچیس برس گزارے، میں سے زیادہ تر اُردو روانی سے بول سکتے ہیں. وہ تمام اُردو بولنے والے کہلائیں گے۔ پاکستان میں اُردو اخباروں کی ایک بڑی تعداد چھپتی ہے جن میں روزنامۂ جنگ، نوائے وقت اور ملّت شامل ہیں۔");
    QTest::newRow("Long Urdu UTF-8")
        << longUrdu
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Urdu UTF-8")
        << longUrdu
        << "de"         // declared locale, should not break it
        << "ISO-8859-1" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long Urdu ISO-8859-6")
        << longUrdu
        << ""
        << ""
        << false
        << "ISO-8859-6"
        << "ISO-8859-6"
        << "ar";
    QTest::newRow("Long Urdu ISO-8859-6")
        << longUrdu
        << "ja"         // declared locale, should not break it
        << "Shift_JIS"  // declared encoding, should not break it
        << false
        << "ISO-8859-6"
        << "ISO-8859-6"
        << "ar";
    QTest::newRow("Long Urdu ISO-8859-6")
        << longUrdu
        << "ar"         // declared locale, should not break it
        << "UTF-8"      // declared encoding, should not break it
        << false
        << "ISO-8859-6"
        << "ISO-8859-6"
        << "ar";
    QTest::newRow("Long Urdu ISO-8859-6")
        << longUrdu
        << "de"         // declared locale, breaks it, not fixable
        << ""
        << false
        << "ISO-8859-6"
        << "ISO-8859-1"
        << "de";
    QTest::newRow("Long Urdu ISO-8859-6")
        << longUrdu
        << ""
        << "ISO-8859-1" // declared encoding, breaks it, not fixable
        << false
        << "ISO-8859-6"
        << "ISO-8859-1"
        << "";

    QString shortUrdu("اردو");
    QTest::newRow("Short Urdu UTF-8")
        << shortUrdu
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Urdu UTF-8")
        << shortUrdu
        << "de"         // declared locale, should not break it
        << "ISO-8859-1" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Short Urdu ISO-8859-6")
        << shortUrdu
        << ""
        << ""
        << false
        << "ISO-8859-6"
        << "EUC-KR" // not fixable
        << "ko";
    QTest::newRow("Short Urdu ISO-8859-6")
        << shortUrdu
        << "ur" // declared locale, does not help, not fixable
        << ""
        << false
        << "ISO-8859-6"
        << "EUC-KR"
        << "ko";

    QTest::newRow("ChineseHK.txt from bug#215942")
        << "榱朔奖W}R的朋友，特此把商I件《Follow Me}R字典》部份功能放上W，提供I的}R字典上查。《Follow Me}R字典》上查版提供：6200常用h字的}R拆a的B演示，首同步@示h字的}R字形Y、取a原t、取a                                                                                  方法。令初W者更容易正_掌握}R拆a，能M足一般W用}R人士之需要。"
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    QString longTraditionalChinese("春節期間 中華電信帳單繳費順延 發布日期：2011/01/14 中華電信北區分公司表示：為配合春節連續假期暨2月份只有28天，避免造成客戶繳費不及而被催費、停話，100年1月份第3計費週期至2月份第2計費週期之電信費帳單繳費期限往後順延，以利客戶繳費。茲將100年1及2月份繳費期限調整情形說明如下：");
    QTest::newRow("Long traditional Chinese UTF-8")
        << longTraditionalChinese
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long traditional Chinese Big5")
        << longTraditionalChinese
        << ""
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Long traditional Chinese GB18030")
        << longTraditionalChinese
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    QString longSimplifiedChinese("中国人民银行14日晚间宣布，从2011年1月20日起，上调存款类金融机构人民币存款准备金率0.5个百分点。业内人士认为，央行在春节前上调存款准备金率在预料之内，鉴于春节期间物价上行压力较大，节后流动性回笼依然任重道远。");
    QTest::newRow("Long simplified Chinese UTF-8")
        << longSimplifiedChinese
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Long simplified Chinese GB2312")
        << longSimplifiedChinese
        << ""
        << ""
        << false
        << "GB2312"
        << "GB18030" // OK!!
        << "zh";
    QTest::newRow("Long simplified Chinese GB18030")
        << longSimplifiedChinese
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    // Chunghwa Telecom, 中華電信, http://www.cht.com.tw/
    QString chungwaTelecom("中華電信");
    QTest::newRow("Chungwa Telecom UTF-8")
        << chungwaTelecom
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Chungwa Telecom UTF-8")
        << chungwaTelecom
        << "zh"   // declared locale, should not break it
        << "Big5" // declared encoding, should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Chungwa Telecom Big5")
        << chungwaTelecom
        << ""
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Chungwa Telecom Big5")
        << chungwaTelecom
        << "zh"         // declared locale, should not break it
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Chungwa Telecom Big5")
        << chungwaTelecom
        << "zh"         // declared locale, should not break it
        << "GB18030"    // declared encoding, should not break it
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Chungwa Telecom Big5")
        << chungwaTelecom
        << ""
        << "GB18030"    // declared encoding, should not break it
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Chungwa Telecom Big5")
        << chungwaTelecom
        << ""
        << "GB2312"    // declared encoding, should not break it
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Chungwa Telecom Big5")
        << chungwaTelecom
        << ""
        << "silly nonsense ☺" // declared encoding, should not break it
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Chungwa Telecom GB18030")
        << chungwaTelecom
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    // Taiwan Mobile, 台灣大哥大, http://www.taiwanmobile.com/index.html
    QString taiwanMobile("台灣大哥大");
    QTest::newRow("Taiwan Mobile UTF-8")
        << taiwanMobile
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Taiwan Mobile Big5")
        << taiwanMobile
        << ""
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Taiwan Mobile Big5")
        << taiwanMobile
        << ""
        << "GB18030" // declared encoding, should not break it
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Taiwan Mobile Big5")
        << taiwanMobile
        << "zh_CN"   // declared locale, should not break it
        << "GB18030" // declared encoding, should not break it
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Taiwan Mobile GB18030")
        << taiwanMobile
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";
    QTest::newRow("Taiwan Mobile GB18030")
        << taiwanMobile
        << "zh_CN" // declared locale, should not break it
        << "Big5"  // declared encoding, should not break it
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    // 電訊盈科流動通訊, PCCW Mobile,
    // http://www2.pccwmobile.com/portal/index.jsp
    QString pccwMobile("電訊盈科流動通訊");
    QTest::newRow("PCCW Mobile (Hongkong) UTF-8")
        << pccwMobile
        << "zh"   // declared locale, this should not break it
        << "Big5" // declared encoding, this should not break it
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << ""
        << ""
        << false
        << "Big5"
        << "ISO-8859-1" // not fixable
        << "es";        // not fixable
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh" // declared locale
        << ""
        << false
        << "Big5"
        // declared locale "zh" gives GB18030 instead of ISO-8859-1.
        // That’s still wrong but unless “zh_TW” or “zh_HK” is declared
        // that is not fixable.
        << "GB18030" 
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh_TW" // declared locale, makes it work
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh_HK" // declared locale, makes it work
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh_MO" // declared locale, makes it work
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << ""
        << "Big5" // declared encoding, works
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh"   // declared locale, should not break it
        << "Big5" // declared encoding, works
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh_CN" // declared locale, should not break it
        << "Big5"  // declared encoding, works
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh_CN" // declared locale, should not break it
        << "Big5"  // declared encoding, works
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) Big5")
        << pccwMobile
        << "zh_CN" // declared locale, should not break it
        << "Big5"  // declared encoding, works
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) GB18030")
        << pccwMobile
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";
    QTest::newRow("PCCW Mobile (Hongkong) GB18030")
        << pccwMobile
        << "ja"    // declared language, should not break it
        << "UTF-8" // declared encoding, should not break it
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    // 中國移動香港有限公司, China   Mobile Hong Kong Company Limited,
    // http://www.hk.chinamobile.com/p_homepage_tc.jsp
    QString mobileHongKong("中國移動香港有限公司");
    QTest::newRow("Mobile Hong Kong UTF-8")
        << mobileHongKong
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Mobile Hong Kong Big5")
        << mobileHongKong
        << ""
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Mobile Hong Kong GB18030")
        << mobileHongKong
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";

    // Operator in  Mainland China: 中国移动 CMCC in English
    QString chnMobile("中国移动");
    QTest::newRow("CMCC UTF-8")
        << chnMobile
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("CMCC GB18030")
        << chnMobile
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";
    QTest::newRow("CMCC GB2312")
        << chnMobile
        << ""
        << ""
        << false
        << "GB2312"
        << "GB18030"
        << "zh";
    QTest::newRow("CMCC GB18030")
        << makeStringLonger(chnMobile, 10)
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";
    QTest::newRow("CMCC GB2312")
        << makeStringLonger(chnMobile, 10)
        << ""
        << ""
        << false
        << "GB2312"
        << "GB18030"
        << "zh";

    // Operator in  Mainland China: 中国联通  CHN-UNICOM in English）
    QString chnUnicom("中国联通");
    QTest::newRow("CHN-UNICOM UTF-8")
        << chnUnicom
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("CHN-UNICOM GB18030")
        << chnUnicom
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";
    QTest::newRow("CHN-UNICOM GB2312")
        << chnUnicom
        << ""
        << ""
        << false
        << "GB2312"
        << "GB18030"
        << "zh";
    QTest::newRow("CHN-UNICOM Big5")
        << chnUnicom
        << ""
        << ""
        << false
        // this cannot be converted to Big5, therefore, when converted
        // back it is broken. But it still detects correctly.
        << "Big5"
        << "Big5"
        << "zh";

    // Operator in  Mainland China: China Telecom (CDMA only) 中国电信
    QString chinaTelecom("中国电信");
    QTest::newRow("China Telecom UTF-8")
        << chinaTelecom
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("China Telecom GB18030")
        << chinaTelecom
        << ""
        << ""
        << false
        << "GB18030"
        << "GB18030"
        << "zh";
    QTest::newRow("China Telecom GB2312")
        << chinaTelecom
        << ""
        << ""
        << false
        << "GB2312"
        << "GB18030"
        << "zh";
    QTest::newRow("China Telecom Big5")
        << chinaTelecom
        << ""
        << ""
        << false
        // this cannot be converted to Big5, therefore, when converted
        // back it is broken. But it still detects correctly.
        << "Big5"
        << "Big5"
        << "zh";
}

void Ft_MCharsetDetector::testDetection()
{
    QFETCH(QString, text);
    QFETCH(QString, declaredLocale);
    QFETCH(QString, declaredEncoding);
    QFETCH(bool, enableInputFilter);
    QFETCH(QString, inputEncoding);
    QFETCH(QString, bestMatchName);
    QFETCH(QString, bestMatchLanguage);

    QTextCodec *codec = QTextCodec::codecForName(inputEncoding.toAscii());
    if (codec == NULL) // there is no codec matching the name
        QFAIL(QString("no such codec: " + inputEncoding).toAscii().constData());

    QByteArray encodedString = codec->fromUnicode(text);
    // add Latin1 junk:
    // encodedString = QByteArray(QString("ï").toLatin1()) + encodedString;
    MCharsetDetector charsetDetector(encodedString);
    charsetDetector.setDeclaredLocale(declaredLocale);
    charsetDetector.setDeclaredEncoding(declaredEncoding);
    charsetDetector.enableInputFilter(enableInputFilter);
    QCOMPARE(charsetDetector.isInputFilterEnabled(), enableInputFilter);
    MCharsetMatch bestMatch = charsetDetector.detect();

    QList<MCharsetMatch> mCharsetMatchList = charsetDetector.detectAll();
    int numberOfMatches = mCharsetMatchList.size();
#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << "======================================================================\n";
    debugStream << QTest::currentDataTag() << "\n";
    debugStream << "-------input text in UTF-8:\n";
    debugStream << text << "\n";
    debugStream << "-------input text converted to " << inputEncoding << ":\n";
    debugStream << encodedString << "\n";
    debugStream << "------ converted back to UTF-8 using the best detected encoding:\n";
    debugStream << charsetDetector.text(bestMatch) << "\n";
    debugStream << "----------------------------------------------------------------------\n";
    debugStream << QTest::currentDataTag() << "\n";
    debugStream << "declaredEncoding=" << declaredEncoding << "\n";
    debugStream << "declaredLocale=" << declaredLocale << "\n";
    debugStream << "match count = " << numberOfMatches << "\n";
    for(int i = 0; i < mCharsetMatchList.size(); ++i) {
        debugStream << "match " << i << ": "
                    << mCharsetMatchList[i].name()
                    << "\tlanguage="<<mCharsetMatchList[i].language()
                    << "\tconfidence=" << mCharsetMatchList[i].confidence()
                    << "\n";
    }
    debugStream.flush();
#endif
    QCOMPARE(bestMatch.name(), bestMatchName);
    QCOMPARE(bestMatch.language(), bestMatchLanguage);
    if (numberOfMatches > 0) {
        QCOMPARE(bestMatch.name(), mCharsetMatchList[0].name());
        QCOMPARE(bestMatch.language(), mCharsetMatchList[0].language());
        QCOMPARE(bestMatch.confidence(), mCharsetMatchList[0].confidence());
    }
}

QTEST_APPLESS_MAIN(Ft_MCharsetDetector);

