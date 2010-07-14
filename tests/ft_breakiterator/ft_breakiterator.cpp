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

#include <QTextCodec>
#include <QDebug>

#include <MLocale>
#include <MCollator>

#include "ft_breakiterator.h"

class TestBreakIterator : public MBreakIterator
{
public:
    TestBreakIterator (const QString &text, Type type = WordIterator) : MBreakIterator(text, type) {}
    TestBreakIterator (const MLocale &locale, const QString &text, Type type = WordIterator) : MBreakIterator(locale, text, type) {}
};

void Ft_BreakIterator::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Ft_BreakIterator::cleanupTestCase()
{
    delete qap;
}

void Ft_BreakIterator::init()
{
}
void Ft_BreakIterator::cleanup()
{
}

void defaultData()
{
    QTest::addColumn<QString>("locale_name");
    QTest::addColumn<QString>("sourceString");
    QTest::addColumn<QList<int> >("correctBoundaries");

    // an empty locale_name should be equivalent to the locale_name "en_US_POSIX"
    QTest::newRow("")
            << QString("")
            << "This is a simple sentence. Täst."
            << (QList<int> ()<< 0 << 4 << 5 << 7 << 8 << 9 << 10 << 16 << 17 << 25 << 26  << 27 << 31 << 32);

    QTest::newRow("fi_FI")
            << QString("fi_FI")
            << "fiksusta boksista esiin astui despootti ges ces"
            << (QList<int> () << 0 << 8 << 9 << 17 << 18 << 23 << 24 << 29 << 30 << 39 << 40 << 43 << 44 << 47);

    // results for Japanese are apparently not completely correct
    // it breaks only between kanji and kana, i.e. はいい is counted as one word.
    // But that is better than nothing.
    QTest::newRow("ja_JP")
            << QString("ja_JP")
            << "睡眠不足はいい仕事の敵だ。"
            << (QList<int> () << 0 << 4 << 7 << 9 << 10 << 11 << 12 << 13);

    // For Thai, ICU does a "GraphemeClusterBreak". not really a word break:
    QTest::newRow("th_TH")
            << QString("th_TH")
            << "ฉันจะใช้ไดเรกทอรีของ Google แทนการค้นหาเว็บตามปกติของ Google เมื่อใด"
            << (QList<int> () << 0 << 3 << 5 << 8 << 17 << 20 << 21 << 27 << 28 << 31 << 34 << 39 << 43 << 46 << 50 << 53 << 54 << 60 << 61 << 66 << 68);

    // For Chinese, ICU breaks after every Chinese character:
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << "什么时候该选用 Google 目录查询而非网络查询呢？"
            << (QList<int> () << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23 << 24 << 25 << 26 << 27);

    // Let’s try Chinese again without spaces around the Latin characters:
    // For Chinese, ICU breaks after every Chinese character:
    QTest::newRow("zh_CN")
            << QString("zh_CN")
            << "什么时候该选用Google目录查询而非网络查询呢？"
            << (QList<int> () << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 13 << 14 << 15 << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23 << 24 << 25);

    QTest::newRow("de_DE")
            << QString("de_DE")
            << "Wann sollte ich das Google Verzeichnis anstelle der regulären Websuche von Google verwenden?"
            << (QList<int> () << 0 << 4 << 5 << 11 << 12 << 15 << 16 << 19 << 20 << 26 << 27 << 38 << 39 << 47 << 48 << 51 << 52 << 61 << 62 << 70 << 71 << 74 << 75 << 81 << 82 << 91 << 92);

    QTest::newRow("ar_SA")
            << QString("ar_SA")
            << "نريد NOKIA الهواتف لتصبح أفضل."
            << (QList<int> () << 0 << 4 << 5 << 10 << 11 << 18 << 19 << 24 << 25 << 29 << 30);

    // and now without spaces around “NOKIA”:
    // The result is most likely wrong, see NB#177492, i.e. the following
    // just “documents” the current behaviour here until we can fix it:
    QTest::newRow("ar_SA")
            << QString("ar_SA")
            << "نريدNOKIAالهواتف لتصبح أفضل."
            << (QList<int> () << 0 << 16 << 17 << 22 << 23 << 27 << 28);
}

void Ft_BreakIterator::constructors_data()
{
    defaultData();
}

void Ft_BreakIterator::constructors()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, sourceString);
    QFETCH(QList<int>, correctBoundaries);

    MLocale locale(locale_name);
    TestBreakIterator *testBreakIterator
        = new TestBreakIterator(locale, sourceString, MBreakIterator::WordIterator);
    TestBreakIterator *testBreakIteratorDefaultLocale
       = new TestBreakIterator(sourceString, MBreakIterator::WordIterator);
    delete testBreakIterator;
    delete testBreakIteratorDefaultLocale;
}

void Ft_BreakIterator::forward_data()
{
    defaultData();
}

void Ft_BreakIterator::forward()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, sourceString);
    QFETCH(QList<int>, correctBoundaries);

    MLocale locale(locale_name);
    MBreakIterator it(locale, sourceString, MBreakIterator::WordIterator);
    MLocale::setDefault(locale);
    MBreakIterator itDefault(sourceString, MBreakIterator::WordIterator);
    QListIterator<int> correctIt(correctBoundaries);

    for (int i = 0; i < 2; ++i) {
        while (it.hasNext()) {
            QVERIFY(correctIt.hasNext());
            int next = it.next();
            int nextDefault = itDefault.next();
            QCOMPARE(next, nextDefault);
            int nextCorrect = correctIt.next();
            // qDebug() << next << nextCorrect;
            QCOMPARE(next, nextCorrect);
        }
        QVERIFY(!correctIt.hasNext());
        // to cover toFront(), try whether a second pass works as well:
        it.toFront();
        itDefault.toFront();
        correctIt.toFront();
    }
}

void Ft_BreakIterator::backward_data()
{
    defaultData();
}

void Ft_BreakIterator::backward()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, sourceString);
    QFETCH(QList<int>, correctBoundaries);

    MLocale locale(locale_name);
    MBreakIterator it(locale, sourceString, MBreakIterator::WordIterator);
    MLocale::setDefault(locale);
    MBreakIterator itDefault(sourceString, MBreakIterator::WordIterator);
    QListIterator<int> correctIt(correctBoundaries);

    it.toBack();
    itDefault.toBack();
    correctIt.toBack();

    while (it.hasPrevious()) {
        QVERIFY(correctIt.hasPrevious());
        int previous = it.previous();
        int previousDefault = itDefault.previous();
        QCOMPARE(previous, previousDefault);
        QVERIFY2(it.isBoundary(), "not a boundary although it should be");
        int index = it.index();
        int previousInclusive = it.previousInclusive();
        QCOMPARE(it.index(), index - 1);
        QCOMPARE(previous, previousInclusive);
        it.setIndex(index);
        QVERIFY2(it.isBoundary(), "not a boundary although it should be");
        int previousCorrect = correctIt.previous();
        //qDebug() << "index" << index
        //         << "previousInclusive" << previousInclusive
        //         << "previous" << previous
        //         << "previousCorrect" << previousCorrect;
        QCOMPARE(previous, previousCorrect);
    }
    QVERIFY(!correctIt.hasPrevious());
}

void Ft_BreakIterator::aroundIndex_data()
{
    defaultData();
}

void Ft_BreakIterator::aroundIndex()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, sourceString);
    QFETCH(QList<int>, correctBoundaries);

    MLocale locale(locale_name);
    MBreakIterator it(locale, sourceString, MBreakIterator::WordIterator);
    QListIterator<int> correctIt(correctBoundaries);

    // look for boundaries around an explicit index "fromIndex":
    for (int fromIndex = 0; fromIndex <= sourceString.size(); ++fromIndex) {
        bool fromIndexIsBoundary = it.isBoundary(fromIndex);
        int previous = it.previous(fromIndex);
        int previousInclusive = it.previousInclusive(fromIndex);
        int next = it.next(fromIndex);
        //qDebug() << "fromIndex" << fromIndex
        //         << "previous" << previous
        //         << "previousInclusive" << previousInclusive
        //         << "next" << next;
        if (fromIndexIsBoundary) {
            QCOMPARE(previousInclusive, fromIndex);
            QVERIFY2(previous != previousInclusive,
                     "previous is equal to previousInclusive although fromIndex is a boundary");
            QVERIFY2(correctBoundaries.contains(fromIndex),
                     "fromIndex is boundary but not in list of correct boundaries");
        }
        else {
            QCOMPARE(previous, previousInclusive);
            QVERIFY2(previousInclusive != fromIndex,
                    "previousInclusive is equal to fromIndex although fromIndex is not a boundary");
        }
        if (previous != -1 && next != -1) {
            QVERIFY(previous < fromIndex);
            QVERIFY(next > fromIndex);
            correctIt.toFront();
            QVERIFY(correctIt.findNext(previous));
            if (fromIndexIsBoundary)
                correctIt.next();
            QCOMPARE(correctIt.next(), next);
        }
    }
}

void Ft_BreakIterator::peek_data()
{
    defaultData();
}

void Ft_BreakIterator::peek()
{
    QFETCH(QString, locale_name);
    QFETCH(QString, sourceString);
    QFETCH(QList<int>, correctBoundaries);

    MLocale locale(locale_name);
    MBreakIterator it(locale, sourceString, MBreakIterator::WordIterator);
    QListIterator<int> correctIt(correctBoundaries);

    // check that two peeks return the same value and that it is a correct value
    int next = it.peekNext();
    QCOMPARE(next, it.peekNext());
    QCOMPARE(next, correctIt.next());
    it.next();
    it.next();

    int prev = it.peekPrevious();
    QCOMPARE(prev, next);
}


QTEST_APPLESS_MAIN(Ft_BreakIterator);
