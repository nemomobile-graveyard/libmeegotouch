/****************************************************************************
**
** Copyright(C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation(directui@nokia.com)
**
** This file is part of systemui.
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

#include "ft_mlocalebuckets.h"

#include <QtTest>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <iostream>

#include "mapplication.h"
#include "mlocale.h"
#include "mlocalebuckets.h"

using std::cout;
using std::endl;

#define VERBOSE 1

MApplication *app = 0;
QStringList inputItems;


void Ft_MLocaleBuckets::initTestCase()
{
    static char *argv[] = { (char *) "./ft_mlocalebuckets" };
    static int argc = 1;
    app = new MApplication(argc, argv);

    // This is important for string constants with non-ASCII characters:
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    inputItems
        << "Ömer"        //  0
        << "Yannick"     //  1
        << "Olund"       //  2
        << "Hendrik"     //  3
        << "Halvar"      //  4
        << "Claudia"     //  5
        << "Christopher" //  6
        << "Chaim"       //  7
        << "Bernardo"    //  8
        << "Anna"        //  9
        << "Agnetha"     // 10
        << "Ágnetha"     // 11
        << "Á"           // 12
        << "A"           // 13
        ;
}


void Ft_MLocaleBuckets::cleanupTestCase()
{
    delete app;
}


void Ft_MLocaleBuckets::dumpBuckets(const MLocaleBuckets &buckets, const char *header) const
{
#if VERBOSE
    cout << endl;

    if (header)
        cout << "=== " << header << " ===\n" << endl;

    for (int b=0; b < buckets.bucketCount(); ++b)
    {
        cout << "Bucket #" << b
             << " [" << buckets.bucketName(b).toUtf8().constData() << "]"
             << " size " << buckets.bucketSize(b)
             << endl;

        QStringList items = buckets.bucketContent(b);

        for (int i=0; i < items.count(); ++i)
        {
            cout << "  #" << i << ": " << items.at(i).toUtf8().constData()
                 << "      \torig index: " << buckets.origItemIndex(b, i)
                 << endl;
        }
    }

    cout << endl;
#else
    Q_UNUSED(buckets);
    Q_UNUSED(header);
#endif
}


bool Ft_MLocaleBuckets::checkBucketContent(const MLocaleBuckets &buckets,
                                           int bucketIndex,
                                           const QStringList &expectedItems) const
{
    if (buckets.bucketContent(bucketIndex) == expectedItems) {
        return true;
    }
    else {
#if VERBOSE
        qDebug() << "bucketContent=" << buckets.bucketContent(bucketIndex)
                 << "expectedItems=" << expectedItems;
#endif
        return false;
    }
}


void Ft_MLocaleBuckets::testEnglishGrouping()
{
    MLocale locale("en_US");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 5
    //   #0: A
    //   #1: Á
    //   #2: Agnetha
    //   #3: Ágnetha
    //   #4: Anna
    // Bucket #1 [B] size 1
    //   #0: Bernardo
    // Bucket #2 [C] size 3
    //   #0: Chaim
    //   #1: Christopher
    //   #2: Claudia
    // Bucket #3 [H] size 2
    //   #0: Halvar
    //   #1: Hendrik
    // Bucket #4 [O] size 2
    //   #0: Olund
    //   #1: Ömer
    // Bucket #5 [Y] size 1
    //   #0: Yannick

    enum EnBuckets
    {
        A_Bucket = 0,
        B_Bucket,
        C_Bucket,
        H_Bucket,
        O_Bucket,
        Y_Bucket,
        OneTooMany_Bucket
    };

    MLocaleBuckets buckets;
    QVERIFY(buckets.isEmpty());
    buckets.setItems(inputItems);
    dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.bucketCount() == Y_Bucket+1);
    QVERIFY(buckets.bucketName("Chaim") == "C");

    QVERIFY(buckets.bucketName(C_Bucket) == "C");
    QVERIFY(buckets.bucketName(H_Bucket) == "H");
    QVERIFY(buckets.bucketName(O_Bucket) == "O");
    QVERIFY(buckets.bucketName(OneTooMany_Bucket).isEmpty());
    QVERIFY(buckets.bucketName(-1).isEmpty());

    QVERIFY(buckets.bucketSize(A_Bucket) == 5);
    QVERIFY(buckets.bucketSize(H_Bucket) == 2);
    QVERIFY(buckets.bucketSize(-1) == -1);
    QVERIFY(buckets.bucketSize(OneTooMany_Bucket+17) == -1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    QVERIFY(checkBucketContent(buckets, B_Bucket, QStringList() << "Bernardo"));
    QVERIFY(checkBucketContent(buckets, C_Bucket, QStringList() << "Chaim" << "Christopher" << "Claudia" ));
    QVERIFY(checkBucketContent(buckets, H_Bucket, QStringList() << "Halvar" << "Hendrik"));
    QVERIFY(checkBucketContent(buckets, O_Bucket, QStringList() << "Olund" << "Ömer"));
    QVERIFY(checkBucketContent(buckets, Y_Bucket, QStringList() << "Yannick"));
    QVERIFY(checkBucketContent(buckets, OneTooMany_Bucket, QStringList()));
    QVERIFY(checkBucketContent(buckets, -1, QStringList()));

    QVERIFY(buckets.origItemIndex(C_Bucket, 0) == inputItems.indexOf("Chaim"));
    QVERIFY(buckets.origItemIndex(Y_Bucket, 0) == inputItems.indexOf("Yannick"));
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 0) == -1);
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 7) == -1);
    QVERIFY(buckets.origItemIndex(-1, 0) == -1);

    QVERIFY(buckets.bucketIndex("A") == A_Bucket);
    QVERIFY(buckets.bucketIndex("C") == C_Bucket);
    QVERIFY(buckets.bucketIndex("CH") == -1); // exists in Czech only
    QVERIFY(buckets.bucketIndex("Y") == Y_Bucket);
    QVERIFY(buckets.bucketIndex("NoBucketName") == -1);
    QVERIFY(buckets.bucketIndex(QString()) == -1);

    buckets.clear();
    QVERIFY(buckets.isEmpty());
    QVERIFY(buckets.bucketCount() == 0);
}

void Ft_MLocaleBuckets::testChinesePinyinGrouping()
{
    MLocale locale("zh_CN");//@collation=pinyin");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 5
    //   #0: Á
    //   #1: A
    //   #2: Ágnetha
    //   #3: Agnetha
    //   #4: Anna
    // Bucket #1 [B] size 1
    //   #0: Bernardo
    // Bucket #2 [C] size 3
    //   #0: Chaim
    //   #1: Christopher
    //   #2: Claudia
    // Bucket #3 [H] size 2
    //   #0: Halvar
    //   #1: Hendrik
    // Bucket #4 [O] size 2
    //   #0: Olund
    //   #1: Ömer
    // Bucket #5 [Y] size 1
    //   #0: Yannick

    enum EnBuckets
    {
        A_Bucket = 0,
        B_Bucket,
        C_Bucket,
        H_Bucket,
        O_Bucket,
        Y_Bucket,
        OneTooMany_Bucket
    };

    MLocaleBuckets buckets;
    QVERIFY(buckets.isEmpty());
    buckets.setItems(inputItems);
    dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.bucketCount() == Y_Bucket+1);
    QVERIFY(buckets.bucketName("Chaim") == "C");

    QVERIFY(buckets.bucketName(C_Bucket) == "C");
    QVERIFY(buckets.bucketName(H_Bucket) == "H");
    QVERIFY(buckets.bucketName(O_Bucket) == "O");
    QVERIFY(buckets.bucketName(OneTooMany_Bucket).isEmpty());
    QVERIFY(buckets.bucketName(-1).isEmpty());

    QVERIFY(buckets.bucketSize(A_Bucket) == 5);
    QVERIFY(buckets.bucketSize(H_Bucket) == 2);
    QVERIFY(buckets.bucketSize(-1) == -1);
    QVERIFY(buckets.bucketSize(OneTooMany_Bucket+17) == -1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Á" << "A" << "Ágnetha" << "Agnetha" << "Anna"));
    QVERIFY(checkBucketContent(buckets, B_Bucket, QStringList() << "Bernardo"));
    QVERIFY(checkBucketContent(buckets, C_Bucket, QStringList() << "Chaim" << "Christopher" << "Claudia" ));
    QVERIFY(checkBucketContent(buckets, H_Bucket, QStringList() << "Halvar" << "Hendrik"));
    QVERIFY(checkBucketContent(buckets, O_Bucket, QStringList() << "Olund" << "Ömer"));
    QVERIFY(checkBucketContent(buckets, Y_Bucket, QStringList() << "Yannick"));
    QVERIFY(checkBucketContent(buckets, OneTooMany_Bucket, QStringList()));
    QVERIFY(checkBucketContent(buckets, -1, QStringList()));

    QVERIFY(buckets.origItemIndex(C_Bucket, 0) == inputItems.indexOf("Chaim"));
    QVERIFY(buckets.origItemIndex(Y_Bucket, 0) == inputItems.indexOf("Yannick"));
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 0) == -1);
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 7) == -1);
    QVERIFY(buckets.origItemIndex(-1, 0) == -1);

    QVERIFY(buckets.bucketIndex("A") == A_Bucket);
    QVERIFY(buckets.bucketIndex("C") == C_Bucket);
    QVERIFY(buckets.bucketIndex("CH") == -1); // exists in Czech only
    QVERIFY(buckets.bucketIndex("Y") == Y_Bucket);
    QVERIFY(buckets.bucketIndex("NoBucketName") == -1);
    QVERIFY(buckets.bucketIndex(QString()) == -1);

    buckets.clear();
    QVERIFY(buckets.isEmpty());
    QVERIFY(buckets.bucketCount() == 0);
}

void Ft_MLocaleBuckets::testGermanPhonebookGrouping()
{
    MLocale locale("de_DE@collation=phonebook");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 5
    //   #0: A
    //   #1: Á
    //   #2: Agnetha
    //   #3: Ágnetha
    //   #4: Anna
    // Bucket #1 [B] size 1
    //   #0: Bernardo
    // Bucket #2 [C] size 3
    //   #0: Chaim
    //   #1: Christopher
    //   #2: Claudia
    // Bucket #3 [H] size 2
    //   #0: Halvar
    //   #1: Hendrik
    // Bucket #4 [O] size 2
    //   #0: Ömer
    //   #1: Olund
    // Bucket #5 [Y] size 1
    //   #0: Yannick

    enum EnBuckets
    {
        A_Bucket = 0,
        B_Bucket,
        C_Bucket,
        H_Bucket,
        O_Bucket,
        Y_Bucket,
        OneTooMany_Bucket
    };

    MLocaleBuckets buckets;
    QVERIFY(buckets.isEmpty());
    buckets.setItems(inputItems);
    dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.bucketCount() == Y_Bucket+1);
    QVERIFY(buckets.bucketName("Chaim") == "C");

    QVERIFY(buckets.bucketName(C_Bucket) == "C");
    QVERIFY(buckets.bucketName(H_Bucket) == "H");
    QVERIFY(buckets.bucketName(O_Bucket) == "O");
    QVERIFY(buckets.bucketName(OneTooMany_Bucket).isEmpty());
    QVERIFY(buckets.bucketName(-1).isEmpty());

    QVERIFY(buckets.bucketSize(A_Bucket) == 5);
    QVERIFY(buckets.bucketSize(H_Bucket) == 2);
    QVERIFY(buckets.bucketSize(-1) == -1);
    QVERIFY(buckets.bucketSize(OneTooMany_Bucket+17) == -1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    QVERIFY(checkBucketContent(buckets, B_Bucket, QStringList() << "Bernardo"));
    QVERIFY(checkBucketContent(buckets, C_Bucket, QStringList() << "Chaim" << "Christopher" << "Claudia" ));
    QVERIFY(checkBucketContent(buckets, H_Bucket, QStringList() << "Halvar" << "Hendrik"));
    QVERIFY(checkBucketContent(buckets, O_Bucket, QStringList() << "Ömer" << "Olund"));
    QVERIFY(checkBucketContent(buckets, Y_Bucket, QStringList() << "Yannick"));
    QVERIFY(checkBucketContent(buckets, OneTooMany_Bucket, QStringList()));
    QVERIFY(checkBucketContent(buckets, -1, QStringList()));

    QVERIFY(buckets.origItemIndex(C_Bucket, 0) == inputItems.indexOf("Chaim"));
    QVERIFY(buckets.origItemIndex(Y_Bucket, 0) == inputItems.indexOf("Yannick"));
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 0) == -1);
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 7) == -1);
    QVERIFY(buckets.origItemIndex(-1, 0) == -1);

    QVERIFY(buckets.bucketIndex("A") == A_Bucket);
    QVERIFY(buckets.bucketIndex("C") == C_Bucket);
    QVERIFY(buckets.bucketIndex("CH") == -1); // exists in Czech only
    QVERIFY(buckets.bucketIndex("Y") == Y_Bucket);
    QVERIFY(buckets.bucketIndex("NoBucketName") == -1);
    QVERIFY(buckets.bucketIndex(QString()) == -1);

    buckets.clear();
    QVERIFY(buckets.isEmpty());
    QVERIFY(buckets.bucketCount() == 0);
}

void Ft_MLocaleBuckets::testCzechGrouping()
{
    MLocale locale("cs_CZ");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 5
    //   #0: A
    //   #1: Á
    //   #2: Agnetha
    //   #3: Ágnetha
    //   #4: Anna
    // Bucket #1 [B] size 1
    //   #0: Bernardo
    // Bucket #2 [C] size 1
    //   #0: Claudia
    // Bucket #3 [H] size 2
    //   #0: Halvar
    //   #1: Hendrik
    // Bucket #4 [CH] size 2
    //   #0: Chaim
    //   #1: Christopher
    // Bucket #5 [O] size 2
    //   #0: Olund
    //   #1: Ömer
    // Bucket #6 [Y] size 1
    //   #0: Yannick

    enum CzBuckets
    {
        A_Bucket = 0,
        B_Bucket,
        C_Bucket,
        H_Bucket,
        CH_Bucket,
        O_Bucket,
        Y_Bucket,
        OneTooMany_Bucket
    };

    MLocaleBuckets buckets(inputItems);
    dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.bucketCount() == Y_Bucket+1);
    QVERIFY(buckets.bucketName("Chaim") == "CH");

    QVERIFY(buckets.bucketName(C_Bucket) == "C");
    QVERIFY(buckets.bucketName(H_Bucket) == "H");
    QVERIFY(buckets.bucketName(CH_Bucket)== "CH");
    QVERIFY(buckets.bucketName(O_Bucket) == "O");
    QVERIFY(buckets.bucketName(Y_Bucket) == "Y");
    QVERIFY(buckets.bucketName(OneTooMany_Bucket).isEmpty());
    QVERIFY(buckets.bucketName(-1).isEmpty());

    QVERIFY(buckets.bucketSize(A_Bucket) == 5);
    QVERIFY(buckets.bucketSize(H_Bucket) == 2);
    QVERIFY(buckets.bucketSize(-1) == -1);
    QVERIFY(buckets.bucketSize(OneTooMany_Bucket+17) == -1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    QVERIFY(checkBucketContent(buckets, B_Bucket, QStringList() << "Bernardo"));
    QVERIFY(checkBucketContent(buckets, C_Bucket, QStringList() << "Claudia" ));
    QVERIFY(checkBucketContent(buckets, H_Bucket, QStringList() << "Halvar" << "Hendrik"));
    QVERIFY(checkBucketContent(buckets, CH_Bucket, QStringList() << "Chaim" << "Christopher"));
    QVERIFY(checkBucketContent(buckets, O_Bucket, QStringList() << "Olund" << "Ömer"));
    QVERIFY(checkBucketContent(buckets, Y_Bucket, QStringList() << "Yannick"));
    QVERIFY(checkBucketContent(buckets, OneTooMany_Bucket, QStringList()));
    QVERIFY(checkBucketContent(buckets, -1, QStringList()));

    QVERIFY(buckets.origItemIndex(CH_Bucket, 0) == inputItems.indexOf("Chaim"));
    QVERIFY(buckets.origItemIndex(Y_Bucket, 0) == inputItems.indexOf("Yannick"));
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 0) == -1);
    QVERIFY(buckets.origItemIndex(OneTooMany_Bucket, 7) == -1);
    QVERIFY(buckets.origItemIndex(-1, 0) == -1);

    QVERIFY(buckets.bucketIndex("C") == C_Bucket);
    QVERIFY(buckets.bucketIndex("CH") == CH_Bucket); // exists in Czech only
    QVERIFY(buckets.bucketIndex("A") == A_Bucket);
    QVERIFY(buckets.bucketIndex("Y") == Y_Bucket);
    QVERIFY(buckets.bucketIndex("NoBucketName") == -1);
    QVERIFY(buckets.bucketIndex(QString()) == -1);

    buckets.clear();
    QVERIFY(buckets.isEmpty());
    QVERIFY(buckets.bucketCount() == 0);
}


void Ft_MLocaleBuckets::testRemove()
{
    MLocale locale("en_US");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 5
    //   #0: A
    //   #0: Á
    //   #1: Agnetha
    //   #2: Ágnetha
    //   #3: Anna
    // Bucket #1 [B] size 1
    //   #0: Bernardo
    // Bucket #2 [C] size 3
    //   #0: Chaim
    //   #1: Christopher
    //   #2: Claudia
    // Bucket #3 [H] size 2
    //   #0: Halvar
    //   #1: Hendrik
    // Bucket #4 [O] size 2
    //   #0: Olund
    //   #1: Ömer
    // Bucket #5 [Y] size 1
    //   #0: Yannick

    enum Buckets
    {
        A_Bucket = 0,
        B_Bucket,
        C_Bucket,
        H_Bucket,
        O_Bucket,
        Y_Bucket,
        OneTooMany_Bucket
    };

    QStringList origItems(inputItems);
    MLocaleBuckets buckets(origItems);
    // dumpBuckets(buckets, __FUNCTION__);

    int oldBucketCount = buckets.bucketCount();
    QVERIFY(checkBucketContent(buckets, C_Bucket, QStringList() << "Chaim" << "Christopher" << "Claudia" ));
    origItems.removeAll("Christopher");
    origItems.removeAll("Claudia");
    bool bucketEmpty = buckets.removeBucketItems(C_Bucket, 1, 2); // Remove "Christopher" and "Claudia"
    // dumpBuckets(buckets, __FUNCTION__);
    QVERIFY(!bucketEmpty);
    QVERIFY(buckets.bucketIndex("C") == C_Bucket);
    QVERIFY(checkBucketContent(buckets, C_Bucket, QStringList() << "Chaim"));

    origItems.removeAll("Chaim");
    bucketEmpty = buckets.removeBucketItems(C_Bucket, 0, 1); // Remove "Chaim"
    // dumpBuckets(buckets, __FUNCTION__);
    QVERIFY(bucketEmpty);
    QVERIFY(buckets.bucketIndex("C") == C_Bucket);
    QVERIFY(buckets.bucketCount() == oldBucketCount);
    // dumpBuckets(buckets, __FUNCTION__);

    buckets.removeEmptyBucket(C_Bucket);
    QVERIFY(buckets.bucketIndex("C") == -1); // Should be removed now
    QVERIFY(buckets.bucketCount() == oldBucketCount-1);
    QVERIFY(buckets.bucketIndex("H") == H_Bucket-1); // Moved up one position after bucket "C" is gone
    // dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.origItemIndex(Y_Bucket-1, 0) == origItems.indexOf("Yannick"));
    QVERIFY(buckets.origItemIndex(H_Bucket-1, 0) == origItems.indexOf("Halvar"));
    QVERIFY(buckets.origItemIndex(H_Bucket-1, 1) == origItems.indexOf("Hendrik"));
    QVERIFY(buckets.origItemIndex(A_Bucket, 4) == origItems.indexOf("Anna"));

    buckets.removeBucketItems(A_Bucket, 17, 5);
    buckets.removeBucketItems(OneTooMany_Bucket, 0, 1);
    buckets.removeBucketItems(-1, 0, 1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    buckets.removeBucketItems(A_Bucket, -1, 1);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    buckets.removeBucketItems(A_Bucket, -1, 1);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    buckets.removeBucketItems(A_Bucket, 0, 0);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    origItems.removeAll("Agnetha");
    buckets.removeBucketItems(A_Bucket, 2, 1);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Ágnetha"  << "Anna"));
    QVERIFY(checkBucketContent(buckets, B_Bucket, QStringList() << "Bernardo"));

    buckets.removeEmptyBucket(A_Bucket);
    QVERIFY(buckets.bucketIndex("A") == A_Bucket);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "A" << "Á" << "Ágnetha" << "Anna"));

    // dumpBuckets(buckets, __FUNCTION__);
    QVERIFY(buckets.origItemIndex(A_Bucket,   3) == origItems.indexOf("Anna"));
    QVERIFY(buckets.origItemIndex(B_Bucket,   0) == origItems.indexOf("Bernardo"));
    QVERIFY(buckets.origItemIndex(H_Bucket-1, 0) == origItems.indexOf("Halvar"));
    QVERIFY(buckets.origItemIndex(H_Bucket-1, 1) == origItems.indexOf("Hendrik"));
    QVERIFY(buckets.origItemIndex(O_Bucket-1, 0) == origItems.indexOf("Olund"));
    QVERIFY(buckets.origItemIndex(O_Bucket-1, 1) == origItems.indexOf("Ömer"));
    QVERIFY(buckets.origItemIndex(Y_Bucket-1, 0) == origItems.indexOf("Yannick"));

    oldBucketCount = buckets.bucketCount();
    buckets.removeEmptyBucket(OneTooMany_Bucket);
    buckets.removeEmptyBucket(-1);
    QVERIFY(oldBucketCount == buckets.bucketCount());
}


void Ft_MLocaleBuckets::testCopy()
{
    MLocale locale("en_US");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 5
    //   #0: A
    //   #1: Á
    //   #2: Agnetha
    //   #3: Ágnetha
    //   #4: Anna
    // Bucket #1 [B] size 1
    //   #0: Bernardo
    // Bucket #2 [C] size 3
    //   #0: Chaim
    //   #1: Christopher
    //   #2: Claudia
    // Bucket #3 [H] size 2
    //   #0: Halvar
    //   #1: Hendrik
    // Bucket #4 [O] size 2
    //   #0: Olund
    //   #1: Ömer
    // Bucket #5 [Y] size 1
    //   #0: Yannick

    enum EnBuckets
    {
        A_Bucket = 0,
        B_Bucket,
        C_Bucket,
        H_Bucket,
        O_Bucket,
        Y_Bucket,
        OneTooMany_Bucket
    };


    MLocaleBuckets *buckets1 = new MLocaleBuckets(inputItems);
    // dumpBuckets(buckets1, __FUNCTION__);

    // Test operator=()
    MLocaleBuckets *buckets2 = new MLocaleBuckets;
    QVERIFY(buckets2->isEmpty());
    QVERIFY(!buckets1->isEmpty());

    *buckets2 = *buckets1;
    QVERIFY(!buckets2->isEmpty());
    QVERIFY(buckets2->bucketCount() == buckets1->bucketCount());

    buckets1->clear();
    QVERIFY(buckets1->isEmpty());
    QVERIFY(!buckets2->isEmpty());

    delete buckets1;
    buckets1 = 0;

    QVERIFY(!buckets2->isEmpty());

    QVERIFY(checkBucketContent(*buckets2, A_Bucket, QStringList() << "A" << "Á" << "Agnetha" << "Ágnetha" << "Anna"));
    QVERIFY(checkBucketContent(*buckets2, B_Bucket, QStringList() << "Bernardo"));
    QVERIFY(checkBucketContent(*buckets2, C_Bucket, QStringList() << "Chaim" << "Christopher" << "Claudia" ));
    QVERIFY(checkBucketContent(*buckets2, H_Bucket, QStringList() << "Halvar" << "Hendrik"));
    QVERIFY(checkBucketContent(*buckets2, O_Bucket, QStringList() << "Olund" << "Ömer"));
    QVERIFY(checkBucketContent(*buckets2, Y_Bucket, QStringList() << "Yannick"));

    // Test copy constructor
    MLocaleBuckets buckets3(*buckets2);
    QVERIFY(!buckets3.isEmpty());
    QVERIFY(buckets3.bucketCount() == buckets2->bucketCount());
    QVERIFY(checkBucketContent(buckets3, H_Bucket, QStringList() << "Halvar" << "Hendrik"));

    delete buckets2;

    QVERIFY(!buckets3.isEmpty());
    QVERIFY(checkBucketContent(buckets3, H_Bucket, QStringList() << "Halvar" << "Hendrik"));

    QVERIFY(buckets3.origItemIndex(A_Bucket, 4) == inputItems.indexOf("Anna"));
    QVERIFY(buckets3.origItemIndex(H_Bucket, 0) == inputItems.indexOf("Halvar"));
    QVERIFY(buckets3.origItemIndex(H_Bucket, 1) == inputItems.indexOf("Hendrik"));
    QVERIFY(buckets3.origItemIndex(Y_Bucket, 0) == inputItems.indexOf("Yannick"));
}

void Ft_MLocaleBuckets::sortTestFiles_data()
{
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QString>("fileName");

    QTest::newRow("en_US")
        << "en_US"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("ru_RU")
        << "ru_RU"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("ar_EG@numbers=arab")
        << "ar_EG@numbers=arab"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("hi_IN@numbers=deva")
        << "hi_IN@numbers=deva"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("sk_SK")
        << "sk_SK"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("zh_TW@collation=zhuyin")
        << "zh_TW@collation=zhuyin"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("zh_HK")
        << "zh_HK" // stroke count sorting
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("zh_CN@collation=pinyin")
        << "zh_CN@collation=pinyin"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("zh_CN@collation=pinyinsearch")
        << "zh_CN@collation=pinyinsearch"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("th_TH")
        << "th_TH"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("da_DK")
        << "da_DK"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("bem_ZM")
        << "bem_ZM"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("cs_CZ")
        << "cs_CZ"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("cs_CZ")
        << "cs_CZ"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("pl_PL")
        << "pl_PL"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("hu_HU")
        << "hu_HU"
        << "ft_mlocalebuckets_test-input.txt";
    QTest::newRow("kab_DZ")
        << "kab_DZ"
        << "ft_mlocalebuckets_test-input.txt";
}

void Ft_MLocaleBuckets::sortTestFiles()
{
    QFETCH(QString, localeName);
    QFETCH(QString, fileName);

    MLocale locale(localeName);
    MLocale::setDefault(locale);
    QString testInputFileName =
        qApp->applicationDirPath() + QDir::separator() + fileName;
    QFile testInputFile(testInputFileName);
    if (!testInputFile.open(QIODevice::ReadOnly))
        QFAIL(qPrintable("could not open file " + testInputFileName));
    QStringList items;
    while (!testInputFile.atEnd()) {
        QString line = QString::fromUtf8(testInputFile.readLine().constData());
        if (line.endsWith("\n"))
            line.remove(line.size() - 1, 1);
        if (!line.isEmpty())
            items << line;
    }
    testInputFile.close();

    MLocaleBuckets buckets;
    QVERIFY(buckets.isEmpty());
    buckets.setItems(items);
    // dumpBuckets(buckets1, __FUNCTION__);
    QString separator("-------------------- ");
    QString prettyResult
        = separator
        + QChar('\n')
        + QLatin1String("sort locale = ")
        + localeName
        + QChar('\n');
    for (int b = 0; b < buckets.bucketCount(); ++b) {
        prettyResult
            += separator
            + buckets.bucketName(b)
            + QChar('\n');
        QStringList bucketItems = buckets.bucketContent(b);
        for (int i = 0; i < bucketItems.count(); ++i)
            prettyResult += bucketItems[i] + QChar('\n');
    }
    prettyResult += separator + QChar('\n');
    buckets.clear();
    QVERIFY(buckets.isEmpty());
    QVERIFY(buckets.bucketCount() == 0);
    QTextStream debugStream(stdout);
    debugStream.setCodec("UTF-8");
    debugStream << "**********************************************\n";
    debugStream << prettyResult;
    debugStream.flush();
    QString testOutputFileName(
        QLatin1String("/tmp/")
        + fileName
        + QLatin1String("___")
        + localeName
        + QLatin1String("___.txt"));
    QFile testOutputFile(testOutputFileName);
    if (!testOutputFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        QFAIL(qPrintable("could not open file " + testOutputFileName));
    int bytesWritten = testOutputFile.write(prettyResult.toUtf8().constData());
    if (bytesWritten == -1)
        QFAIL(qPrintable("could not write to file" + testOutputFileName));
    QCOMPARE(uint(bytesWritten), qstrlen(prettyResult.toUtf8().constData()));
    testOutputFile.close();
}

QTEST_APPLESS_MAIN(Ft_MLocaleBuckets)
