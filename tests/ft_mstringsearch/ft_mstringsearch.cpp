/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ft_mstringsearch.h"

#define VERBOSE_OUTPUT

void Ft_MStringSearch::initTestCase()
{
    static int dummyArgc = 1;
    static char *dummyArgv[1] = { (char *) "ft_mstringsearch" };
    qap = new MApplication(dummyArgc, dummyArgv, "test");
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Ft_MStringSearch::cleanupTestCase()
{
    delete qap;
}

void Ft_MStringSearch::init()
{
}

void Ft_MStringSearch::cleanup()
{
}

void Ft_MStringSearch::testConstructors_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("lcCollate");
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("foo")
        << "en_US"
        << "en_US"
        << "World"
        << "Hello World!"
        << "";

}

void Ft_MStringSearch::testConstructors()
{
    QFETCH(QString, language);
    QFETCH(QString, lcCollate);
    QFETCH(QString, pattern);
    QFETCH(QString, text);
    QFETCH(QString, errorString);

    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcCollate, lcCollate);
    MStringSearch stringSearch(pattern, text, locale);

#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << "language: " << language
                << " lcCollate: " << lcCollate << "\n"
                << " pattern               : " << pattern << "\n"
                << " stringSearch.pattern(): " << stringSearch.pattern() << "\n"
                << " text               : " << text << "\n"
                << " stringSearch.text(): " << stringSearch.text() << "\n"
                << " errorString               : " << errorString << "\n"
                << " stringSearch.errorString(): " << stringSearch.errorString() << "\n";
#endif

    QCOMPARE(stringSearch.pattern(), pattern);
    QCOMPARE(stringSearch.text(), text);
    QCOMPARE(stringSearch.errorString(), QString());
}

void Ft_MStringSearch::testSearch_data()
{
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("lcCollate");
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("text");
    QTest::addColumn<MBreakIterator::Type>("breakIteratorType");
    QTest::addColumn<MLocale::CollatorStrength>("collatorStrength");
    QTest::addColumn<bool>("isAlternateHandlingShifted");
    QTest::addColumn<QList<int> >("matchStarts");
    QTest::addColumn<QList<int> >("matchLengths");
    QTest::addColumn<QStringList>("matchTexts");

    QTest::newRow("Danish da_DK")
        << "da_DK"
        << "da_DK"
        << "aa"
        << "Hello Aalto and Åse áá!"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 6 << 16)
        << (QList<int> () << 2 << 1)
        << (QStringList() << "Aa" << "Å");
    QTest::newRow("English en_GB")
        << "en_GB"
        << "en_GB"
        << "aa"
        << "Hello Aalto and Åse áá!"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 6 << 20)
        << (QList<int> () << 2 << 2)
        << (QStringList() << "Aa" << "áá");
    QTest::newRow("English en_GB")
        << "en_GB"
        << "en_GB"
        << "a a"
        << "Hello Aalto and Åse áá !"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 6 << 20)
        << (QList<int> () << 2 << 2)
        << (QStringList() << "Aa" << "áá");
    QTest::newRow("sChinese zh_CN")
        << "zh_CN"
        << "zh_CN"
        << "𠮩"
        << "one 劉 two 𠮩 three 刘 four" // 𠮩 U+20BA9 takes 2 indices! UTF-16!
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 10 << 10)
        << (QList<int> () << 3 << 3)
        << (QStringList() << "𠮩 " << "𠮩 ");
    QTest::newRow("sChinese zh_CN")
        << "zh_CN"
        << "zh_CN"
        << "刘"
        << "one 劉 two 𠮩 three 刘 four" // 𠮩 U+20BA9 takes 2 indices! UTF-16!
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 19 << 19)
        << (QList<int> () << 2 << 2)
        << (QStringList() << "刘 " << "刘 ");
    QTest::newRow("sChinese zh_CN@collation=stroke: Latin only match")
        << "zh_CN@collation=stroke"
        << "zh_CN@collation=stroke"
        << "liu"
        << "one 劉 two líu three 刘 four"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 10)
        << (QList<int> () << 3)
        << (QStringList() << "líu");
    QTest::newRow("sChinese zh_CN@collation=pinyin")
        << "zh_CN@collation=pinyin"
        << "zh_CN@collation=pinyin"
        << "刘"
        << "one 劉 two 𠮩 three 刘 four" // 𠮩 U+20BA9 takes 2 indices! UTF-16!
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 19 << 19)
        << (QList<int> () << 2 << 2)
        << (QStringList() << "刘 " << "刘 ");
    QTest::newRow("sChinese zh_CN@collation=pinyinsearch: fuzzy pinyin match")
        << "zh_CN@collation=pinyinsearch"
        << "zh_CN@collation=pinyinsearch"
        << "liu"
        << "one 劉 two líu three 刘 four"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 4 << 10 << 20)
        << (QList<int> () << 2 << 3 << 2)
        << (QStringList() << "劉 " << "líu" << "刘 ");
    QTest::newRow("sChinese zh_CN@collation=pinyin: fuzzy pinyin match")
        << "zh_CN@collation=pinyin"
        << "zh_CN@collation=pinyin"
        << "liu"
        << "one 劉 two líu three 刘 four"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 4 << 10 << 20)
        << (QList<int> () << 2 << 3 << 2)
        << (QStringList() << "劉 " << "líu" << "刘 ");
    QTest::newRow("sChinese zh_CN: fuzzy pinyin match")
        << "zh_CN"
        << "zh_CN"
        << "liu"
        << "one 劉 two líu three 刘 four"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 4 << 10 << 20)
        << (QList<int> () << 2 << 3 << 2)
        << (QStringList() << "劉 " << "líu" << "刘 ");
    QTest::newRow("sChinese zh_CN: fuzzy pinyin match")
        << "zh_CN"
        << "zh_CN"
        << "liuliu"
        << "one 劉 liu two líu liu three 刘 劉 four líu 刘"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 4 << 14 << 28 << 37)
        << (QList<int> () << 5 << 7 << 4 << 5)
        << (QStringList() << "劉 liu" << "líu liu" << "刘 劉 " << "líu 刘");
    QTest::newRow("sChinese zh_CN: fuzzy pinyin match")
        << "zh_CN"
        << "zh_CN"
        << "liu liu"
        << "one 劉 liu two líu liu three 刘 劉 four líu 刘"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 4 << 14 << 28 << 37)
        << (QList<int> () << 5 << 7 << 4 << 5)
        << (QStringList() << "劉 liu" << "líu liu" << "刘 劉 " << "líu 刘");
    QTest::newRow("sChinese zh_CN: fuzzy pinyin match")
        << "zh_CN"
        << "zh_CN"
        << "liu liu"
        << "one 劉liu two líuliuthree 刘劉 four líu刘"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 4 << 13 << 25 << 33)
        << (QList<int> () << 4 << 6 << 3 << 4)
        << (QStringList() << "劉liu" << "líuliu" << "刘劉 " << "líu刘");
    QTest::newRow("English US, accent-sensitive but case-insensitive")
        << "en_US"
        << "en_US"
        << "é"
        << "HellÉ"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthSecondary
        << true
        << (QList<int> () << 4)
        << (QList<int> () << 1)
        << (QStringList() << "É");
    QTest::newRow("English US, word iterator")
        << "en_US"
        << "en_US"
        << "éê"
        << "H ee ee, ee. aeeb."
        << MBreakIterator::WordIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 2 << 5 << 9)
        << (QList<int> () << 2 << 2 << 2)
        << (QStringList() << "ee" << "ee" << "ee");
    QTest::newRow("English US, do *not* ignore spaces and punctuation")
        << "en_US"
        << "en_US"
        << "ab"
        << "ab a b a b a,b a.b a!b a?b a-b"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << false
        << (QList<int> () << 0)
        << (QList<int> () << 2)
        << (QStringList() << "ab");
    QTest::newRow("English US, *do* ignore spaces and punctuation")
        << "en_US"
        << "en_US"
        << "ab"
        << "ab a b a b a,b a.b a!b a?b a-b"
        << MBreakIterator::CharacterIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 0    << 3     << 7     << 11    << 15    << 19    << 23    << 27)
        << (QList<int> () << 2    << 3     << 3     << 3     << 3     << 3     << 3     << 3)
        << (QStringList() << "ab" << "a b" << "a b" << "a,b" << "a.b" << "a!b" << "a?b" << "a-b");
#if 0
    // weird problem with this test, need to find out what
    // is going on here
    QTest::newRow("English US, line iterator matches only at good line breaks")
        << "en_US"
        << "en_US"
        << "é"
        << "H e e, e.  a e "
        << MBreakIterator::LineIterator
        << MLocale::CollatorStrengthPrimary
        << true
        << (QList<int> () << 2 << 4 << 7)
        << (QList<int> () << 2 << 3 << 4)
        << (QStringList() << "e " << "e, " << "e.  ");
#endif
}

void Ft_MStringSearch::testSearch()
{
    QFETCH(QString, language);
    QFETCH(QString, lcCollate);
    QFETCH(QString, pattern);
    QFETCH(QString, text);
    QFETCH(MBreakIterator::Type, breakIteratorType);
    QFETCH(MLocale::CollatorStrength, collatorStrength);
    QFETCH(bool, isAlternateHandlingShifted);
    QFETCH(QList<int>, matchStarts);
    QFETCH(QList<int>, matchLengths);
    QFETCH(QStringList, matchTexts);

#if defined(VERBOSE_OUTPUT)
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream
        << "****************************************************\n"
        << QTest::currentDataTag()
        << "\n";
    debugStream.flush();
#endif
    QCOMPARE(matchStarts.size(), matchLengths.size());
    QCOMPARE(matchStarts.size(), matchTexts.size());

    QString breakIteratorTypeDescription;
    switch(breakIteratorType) {
    case MBreakIterator::SentenceIterator:
        breakIteratorTypeDescription = "MBreakIterator::SentenceIterator";
        break;
    case MBreakIterator::TitleIterator:
        breakIteratorTypeDescription = "MBreakIterator::TitleIterator";
        break;
    case MBreakIterator::LineIterator:
        breakIteratorTypeDescription = "MBreakIterator::LineIterator";
        break;
    case MBreakIterator::WordIterator:
        breakIteratorTypeDescription = "MBreakIterator::WordIterator";
        break;
    case MBreakIterator::CharacterIterator:
        breakIteratorTypeDescription = "MBreakIterator::CharacterIterator";
        break;
    default:
        QFAIL("unsupported breakIteratorType");
        break;
    }
    QString collatorStrengthDescription;
    switch(collatorStrength) {
    case MLocale::CollatorStrengthPrimary:
        collatorStrengthDescription = "MLocale::CollatorStrengthPrimary";
        break;
    case MLocale::CollatorStrengthSecondary:
        collatorStrengthDescription = "MLocale::CollatorStrengthSecondary";
        break;
    case MLocale::CollatorStrengthTertiary:
        collatorStrengthDescription = "MLocale::CollatorStrengthTertiary";
        break;
    case MLocale::CollatorStrengthQuaternary:
        collatorStrengthDescription = "MLocale::CollatorStrengthQuaternary";
        break;
    case MLocale::CollatorStrengthIdentical:
        collatorStrengthDescription = "MLocale::CollatorStrengthIdentical";
        break;
    default:
        QFAIL("unsupported collatorStrength");
        break;
    }
    MLocale locale(language);
    locale.setCategoryLocale(MLocale::MLcCollate, lcCollate);
    MStringSearch stringSearch(pattern, text, locale, breakIteratorType);
    // default strength should be primary:
    QCOMPARE(stringSearch.collatorStrength(), MLocale::CollatorStrengthPrimary);
    // set strength manually and check:
    stringSearch.setCollatorStrength(collatorStrength);
    QCOMPARE(stringSearch.collatorStrength(), collatorStrength);
    // default alternate handling is shifted (ignore spaces and punctuation):
    QCOMPARE(stringSearch.alternateHandlingShifted(), true);
    // set alternate handling manually and check:
    stringSearch.setAlternateHandlingShifted(isAlternateHandlingShifted);
    QCOMPARE(stringSearch.alternateHandlingShifted(), isAlternateHandlingShifted);
#if defined(VERBOSE_OUTPUT)
    debugStream
        << "======================================================================\n";
    debugStream.flush();
#endif
    int first;
    int firstMatchStart;
    int firstMatchLength;
    QString firstMatchText;
    int last;
    int lastMatchStart;
    int lastMatchLength;
    QString lastMatchText;
    int next;
    int nextMatchStart;
    int nextMatchLength;
    QString nextMatchText;
    int previous;
    int previousMatchStart;
    int previousMatchLength;
    QString previousMatchText;

    first = stringSearch.first();
    firstMatchStart = stringSearch.matchedStart();
    firstMatchLength = stringSearch.matchedLength();
    firstMatchText = stringSearch.matchedText();
    next = stringSearch.next();
    nextMatchStart = stringSearch.matchedStart();
    nextMatchLength = stringSearch.matchedLength();
    nextMatchText = stringSearch.matchedText();
    previous = stringSearch.previous();
    previousMatchStart = stringSearch.matchedStart();
    previousMatchLength = stringSearch.matchedLength();
    previousMatchText = stringSearch.matchedText();
#if defined(VERBOSE_OUTPUT)
    debugStream
        << "------------------- first match:\n"
        << QTest::currentDataTag()
        << " language: " << language
        << " lcCollate: " << lcCollate << "\n"
        << " pattern: [" << pattern << "]\n"
        << " text: [" << text << "]\n"
        << " breakIteratorType: " << breakIteratorTypeDescription << "\n"
        << " collatorStrength: " << collatorStrengthDescription << "\n"
        << "\tfirstMatchStart         : \t" << firstMatchStart
        << "\tmatchStarts.first()     : \t" << matchStarts.first()
        << "\n"
        << "\tfirstMatchLength        : \t" << firstMatchLength
        << "\tmatchLenghts.first()    : \t" << matchLengths.first()
        << "\n"
        << "\tfirstMatchText          : \t[" << firstMatchText << "]"
        << "\tmatchTexts.first()      : \t[" << matchTexts.first() << "]"
        << "\n"
        << "\tnext                    : \t" << next << "\n"
        << "\tnextMatchStart          : \t" <<  nextMatchStart << "\n"
        << "\tnextMatchLength         : \t" <<  nextMatchLength << "\n"
        << "\tnextMatchText           : \t[" <<  nextMatchText << "]\n"
        << "\tprevious                : \t" << previous << "\n"
        << "\tpreviousMatchStart      : \t" <<  previousMatchStart << "\n"
        << "\tpreviousMatchLength     : \t" <<  previousMatchLength << "\n"
        << "\tpreviousMatchText       : \t[" <<  previousMatchText << "]\n";
    debugStream.flush();
#endif
    QCOMPARE(first, firstMatchStart);
    QCOMPARE(firstMatchStart, matchStarts.first());
    QCOMPARE(firstMatchLength, matchLengths.first());
    QCOMPARE(firstMatchText, matchTexts.first());

    last = stringSearch.last();
    lastMatchStart = stringSearch.matchedStart();
    lastMatchLength = stringSearch.matchedLength();
    lastMatchText = stringSearch.matchedText();
    next = stringSearch.next();
    nextMatchStart = stringSearch.matchedStart();
    nextMatchLength = stringSearch.matchedLength();
    nextMatchText = stringSearch.matchedText();
    previous = stringSearch.previous();
    previousMatchStart = stringSearch.matchedStart();
    previousMatchLength = stringSearch.matchedLength();
    previousMatchText = stringSearch.matchedText();

#if defined(VERBOSE_OUTPUT)
    debugStream
        << "------------------- last match:\n"
        << QTest::currentDataTag()
        << " language: " << language
        << " lcCollate: " << lcCollate << "\n"
        << " pattern: [" << pattern << "]\n"
        << " text: [" << text << "]\n"
        << " breakIteratorType: " << breakIteratorTypeDescription << "\n"
        << " collatorStrength: " << collatorStrengthDescription << "\n"
        << "\tlastMatchStart          : \t" << lastMatchStart
        << "\tmatchStarts.last()      : \t" << matchStarts.last()
        << "\n"
        << "\tlastMatchLength         : \t" << lastMatchLength
        << "\tmatchLenghts.last()     : \t" << matchLengths.last()
        << "\n"
        << "\tlastMatchText           : \t[" << lastMatchText << "]"
        << "\tmatchTexts.last()       : \t[" << matchTexts.last() << "]"
        << "\n"
        << "\tnextMatchStart          : \t" <<  nextMatchStart << "\n"
        << "\tnextMatchLength         : \t" <<  nextMatchLength << "\n"
        << "\tnextMatchText           : \t[" <<  nextMatchText << "]\n"
        << "\tprevious                : \t" << previous << "\n"
        << "\tpreviousMatchStart      : \t" <<  previousMatchStart << "\n"
        << "\tpreviousMatchLength     : \t" <<  previousMatchLength << "\n"
        << "\tpreviousMatchText       : \t[" <<  previousMatchText << "]\n";
    debugStream.flush();
#endif
    QCOMPARE(last, lastMatchStart);
    QCOMPARE(lastMatchStart, matchStarts.last());
    QCOMPARE(lastMatchLength, matchLengths.last());
    QCOMPARE(lastMatchText, matchTexts.last());
    QCOMPARE(next, lastMatchStart);
    QCOMPARE(nextMatchStart, lastMatchStart);
    QCOMPARE(nextMatchLength, lastMatchLength);
    QCOMPARE(nextMatchText, lastMatchText);

#if defined(VERBOSE_OUTPUT)
    debugStream
        << "------------------- matching  from the beginning:\n"
        << QTest::currentDataTag()
        << " language: " << language
        << " lcCollate: " << lcCollate << "\n"
        << " pattern: [" << pattern << "]\n"
        << " text: [" << text << "]\n"
        << " breakIteratorType: " << breakIteratorTypeDescription << "\n"
        << " collatorStrength: " << collatorStrengthDescription << "\n";
    debugStream.flush();
#endif
    stringSearch.setOffset(0);
    QCOMPARE(stringSearch.offset(), 0);
    int matchCount = 0;
    int matchStart = -1;
    int matchLength = 0;
    QString matchText = QString();
    while(stringSearch.next() != -1) {
        matchStart = stringSearch.matchedStart();
        matchLength = stringSearch.matchedLength();
        matchText = stringSearch.matchedText();
#if defined(VERBOSE_OUTPUT)
        debugStream
            << "\tmatchStart      (" << matchCount << "): \t"
            << matchStart
            << "\tmatchLength     (" << matchCount << "): \t"
            << matchLength
            << "\tmatchText       (" << matchCount << "): \t["
            << matchText << "]"
            << "\n";
        debugStream.flush();
        debugStream
            << "\tmatchStarts.at  (" << matchCount << "): \t"
            << matchStarts.at(matchCount)
            << "\tmatchLengths.at (" << matchCount << "): \t"
            << matchLengths.at(matchCount)
            << "\tmatchTexts.at   (" << matchCount << "): \t["
            << matchTexts.at(matchCount) << "]"
            << "\n";
        debugStream.flush();
#endif
        QCOMPARE(matchStart, matchStarts.at(matchCount));
        QCOMPARE(matchLength, matchLengths.at(matchCount));
        QCOMPARE(matchText, matchTexts.at(matchCount));
        // without incrementing the index, “next”
        // always matches the same match again
        stringSearch.setOffset(matchStart+1);
        matchCount++;
    }
    QCOMPARE(matchStart, lastMatchStart);
    QCOMPARE(matchLength, lastMatchLength);
    QCOMPARE(matchText, lastMatchText);

#if defined(VERBOSE_OUTPUT)
    debugStream
        << "------------------- matching  from the end:\n"
        << QTest::currentDataTag()
        << " language: " << language
        << " lcCollate: " << lcCollate << "\n"
        << " pattern: [" << pattern << "]\n"
        << " text: [" << text << "]\n"
        << " breakIteratorType: " << breakIteratorTypeDescription << "\n"
        << " collatorStrength: " << collatorStrengthDescription << "\n";
    debugStream.flush();
#endif
    stringSearch.setOffset(text.size());
    QCOMPARE(stringSearch.offset(), text.size());
    matchCount = matchTexts.size()-1;
    matchStart = -1;
    matchLength = 0;
    matchText = QString();
    while(stringSearch.previous() != -1) {
        matchStart = stringSearch.matchedStart();
        matchLength = stringSearch.matchedLength();
        matchText = stringSearch.matchedText();
#if defined(VERBOSE_OUTPUT)
        debugStream
            << "\tmatchStart      (" << matchCount << "): \t"
            << matchStart
            << "\tmatchLength     (" << matchCount << "): \t"
            << matchLength
            << "\tmatchLengths.at (" << matchCount << "): \t"
            << matchText << "]"
            << "\n";
        debugStream.flush();
        debugStream
            << "\tmatchStarts.at  (" << matchCount << "): \t"
            << matchLengths.at(matchCount)
            << "\tmatchText       (" << matchCount << "): \t["
            << matchStarts.at(matchCount)
            << "\tmatchTexts.at   (" << matchCount << "): \t["
            << matchTexts.at(matchCount) << "]"
            << "\n";
        debugStream.flush();
#endif
        QCOMPARE(matchStart, matchStarts.at(matchCount));
        QCOMPARE(matchLength, matchLengths.at(matchCount));
        QCOMPARE(matchText, matchTexts.at(matchCount));
        matchCount--;
    }
    QCOMPARE(matchStart, firstMatchStart);
    QCOMPARE(matchLength, firstMatchLength);
    QCOMPARE(matchText, firstMatchText);
}

QTEST_APPLESS_MAIN(Ft_MStringSearch);

