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
            cout << "  #" << i << ": " << items.at(i).toUtf8().constData() << endl;
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
    dumpBuckets(buckets, __FUNCTION__);

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

    buckets.clear();
    QVERIFY(buckets.isEmpty());
    QVERIFY(buckets.bucketCount() == 0);
}


QTEST_APPLESS_MAIN(Ft_MLocaleBuckets)
