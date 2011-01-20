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

#include "pt_mcharsetdetector.h"

void Pt_MCharsetDetector::initTestCase()
{
    static int argc = 0;
    static char *argv[1] = { (char *) "" };
    qap = new QCoreApplication(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

void Pt_MCharsetDetector::cleanupTestCase()
{
    delete qap;
}

void Pt_MCharsetDetector::init()
{
}

void Pt_MCharsetDetector::cleanup()
{
}

static QString makeStringLonger(const QString &str, int n)
{
    QString ret;
    for (int i = 0; i < n; ++i)
        ret += str;
    return ret;
}

void Pt_MCharsetDetector::benchmarkDetection_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("declaredLocale");
    QTest::addColumn<QString>("declaredEncoding");
    QTest::addColumn<bool>("enableInputFilter");
    QTest::addColumn<QString>("inputEncoding");
    QTest::addColumn<QString>("bestMatchName");
    QTest::addColumn<QString>("bestMatchLanguage");

    QString traditionalChinese("中華電信");
    traditionalChinese = makeStringLonger(traditionalChinese, 10);
    QTest::newRow("Traditional Chinese UTF-8")
        << traditionalChinese
        << ""
        << ""
        << false
        << "UTF-8"
        << "UTF-8"
        << "";
    QTest::newRow("Traditional Chinese Big5")
        << traditionalChinese
        << ""
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Traditional Chinese Big5 zh_TW")
        << traditionalChinese
        << "zh_TW"
        << ""
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Traditional Chinese Big5 Big5")
        << traditionalChinese
        << ""
        << "Big5"
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Traditional Chinese Big5 zh_TW Big5")
        << traditionalChinese
        << "zh_TW"
        << "Big5"
        << false
        << "Big5"
        << "Big5"
        << "zh";
    QTest::newRow("Traditional Chinese Big5 zh_TW Big5 input filter")
        << traditionalChinese
        << "zh_TW"
        << "Big5"
        << true
        << "Big5"
        << "Big5"
        << "zh";
}

void Pt_MCharsetDetector::benchmarkDetection()
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
    MCharsetDetector charsetDetector(encodedString);
    charsetDetector.setDeclaredLocale(declaredLocale);
    charsetDetector.setDeclaredEncoding(declaredEncoding);
    charsetDetector.enableInputFilter(enableInputFilter);
    QCOMPARE(charsetDetector.isInputFilterEnabled(), enableInputFilter);
    QList<MCharsetMatch> mCharsetMatchList;

    QBENCHMARK {
        mCharsetMatchList = charsetDetector.detectAll();
    }

    int numberOfMatches = mCharsetMatchList.size();
    if (numberOfMatches > 0) {
        QCOMPARE(bestMatchName, mCharsetMatchList[0].name());
        QCOMPARE(bestMatchLanguage, mCharsetMatchList[0].language());
    }
}

QTEST_APPLESS_MAIN(Pt_MCharsetDetector);
