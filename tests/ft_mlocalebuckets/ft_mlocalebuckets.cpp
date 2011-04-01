/****************************************************************************
**
** Copyright(C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
    return buckets.bucketContent(bucketIndex) == expectedItems;
}


void Ft_MLocaleBuckets::testEnglishGrouping()
{
    MLocale locale("en_US");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 2
    //   #0: Agnetha
    //   #1: Anna
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
    // dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.bucketCount() == Y_Bucket+1);
    QVERIFY(buckets.bucketName("Chaim") == "C");

    QVERIFY(buckets.bucketName(C_Bucket) == "C");
    QVERIFY(buckets.bucketName(H_Bucket) == "H");
    QVERIFY(buckets.bucketName(O_Bucket) == "O");
    QVERIFY(buckets.bucketName(OneTooMany_Bucket).isEmpty());
    QVERIFY(buckets.bucketName(-1).isEmpty());

    QVERIFY(buckets.bucketSize(A_Bucket) == 2);
    QVERIFY(buckets.bucketSize(H_Bucket) == 2);
    QVERIFY(buckets.bucketSize(-1) == -1);
    QVERIFY(buckets.bucketSize(OneTooMany_Bucket+17) == -1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Agnetha" << "Anna"));
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


void Ft_MLocaleBuckets::testCzechGrouping()
{
    MLocale locale("cs_CZ");
    MLocale::setDefault(locale);

    // Expecting:
    //
    // Bucket #0 [A] size 2
    //   #0: Agnetha
    //   #1: Anna
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
    // dumpBuckets(buckets, __FUNCTION__);

    QVERIFY(buckets.bucketCount() == Y_Bucket+1);
    QVERIFY(buckets.bucketName("Chaim") == "CH");

    QVERIFY(buckets.bucketName(C_Bucket) == "C");
    QVERIFY(buckets.bucketName(H_Bucket) == "H");
    QVERIFY(buckets.bucketName(CH_Bucket)== "CH");
    QVERIFY(buckets.bucketName(O_Bucket) == "O");
    QVERIFY(buckets.bucketName(Y_Bucket) == "Y");
    QVERIFY(buckets.bucketName(OneTooMany_Bucket).isEmpty());
    QVERIFY(buckets.bucketName(-1).isEmpty());

    QVERIFY(buckets.bucketSize(A_Bucket) == 2);
    QVERIFY(buckets.bucketSize(H_Bucket) == 2);
    QVERIFY(buckets.bucketSize(-1) == -1);
    QVERIFY(buckets.bucketSize(OneTooMany_Bucket+17) == -1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Agnetha" << "Anna"));
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
    // Bucket #0 [A] size 2
    //   #0: Agnetha
    //   #1: Anna
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
    QVERIFY(buckets.origItemIndex(A_Bucket, 1) == origItems.indexOf("Anna"));

    buckets.removeBucketItems(A_Bucket, 17, 5);
    buckets.removeBucketItems(OneTooMany_Bucket, 0, 1);
    buckets.removeBucketItems(-1, 0, 1);

    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Agnetha" << "Anna"));
    buckets.removeBucketItems(A_Bucket, -1, 1);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Agnetha" << "Anna"));
    buckets.removeBucketItems(A_Bucket, -1, 1);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Agnetha" << "Anna"));
    buckets.removeBucketItems(A_Bucket, 0, 0);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Agnetha" << "Anna"));
    origItems.removeAll("Agnetha");
    buckets.removeBucketItems(A_Bucket, 0, 1);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Anna"));
    QVERIFY(checkBucketContent(buckets, B_Bucket, QStringList() << "Bernardo"));

    buckets.removeEmptyBucket(A_Bucket);
    QVERIFY(buckets.bucketIndex("A") == A_Bucket);
    QVERIFY(checkBucketContent(buckets, A_Bucket, QStringList() << "Anna"));

    // dumpBuckets(buckets, __FUNCTION__);
    QVERIFY(buckets.origItemIndex(A_Bucket,   0) == origItems.indexOf("Anna"));
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
    // Bucket #0 [A] size 2
    //   #0: Agnetha
    //   #1: Anna
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

    QVERIFY(checkBucketContent(*buckets2, A_Bucket, QStringList() << "Agnetha" << "Anna"));
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

    QVERIFY(buckets3.origItemIndex(A_Bucket, 1) == inputItems.indexOf("Anna"));
    QVERIFY(buckets3.origItemIndex(H_Bucket, 0) == inputItems.indexOf("Halvar"));
    QVERIFY(buckets3.origItemIndex(H_Bucket, 1) == inputItems.indexOf("Hendrik"));
    QVERIFY(buckets3.origItemIndex(Y_Bucket, 0) == inputItems.indexOf("Yannick"));
}

QTEST_APPLESS_MAIN(Ft_MLocaleBuckets)
