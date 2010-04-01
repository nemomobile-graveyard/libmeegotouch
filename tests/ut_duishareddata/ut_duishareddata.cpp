/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include <stdlib.h>
#include <time.h>
#include <QObject>
#include "ut_duishareddata.h"

void Ut_DuiSharedData::init()
{
}

void Ut_DuiSharedData::cleanup()
{
}

void Ut_DuiSharedData::testOpenClose()
{
    DuiSharedData *shm = new DuiSharedData("buffer");

    QVERIFY(shm->open(DuiSharedData::Write) == true);
    shm->close();

    // nothing yet to read
    QVERIFY(shm->open(DuiSharedData::ReadOnly) == false);
    shm->close();

    QVERIFY(shm->open(DuiSharedData::Write) == true);
    *shm << 1;
    shm->close();

    QVERIFY(shm->open(DuiSharedData::ReadOnly) == true);
    shm->close();

    delete shm;
}

void Ut_DuiSharedData::testReadWrite_1()
{
    int dataIn1 = 1, dataOut1;
    bool dataIn2 = true, dataOut2;
    qreal dataIn3 = 0.1, dataOut3;
    QFont dataIn4("Times", 10, QFont::Bold), dataOut4;
    QVariant dataIn5("qqq"), dataOut5;
    QString dataIn6("asdf"), dataOut6;

    qint64 pos1, pos2, pos3, pos4, pos5, pos6;

    DuiSharedData *shm = new DuiSharedData("buffer");

    QVERIFY(shm->open(DuiSharedData::Write) == true);

    pos1 = shm->pos();
    QVERIFY(pos1 == 0);
    *shm << dataIn1;

    pos2 = shm->pos();
    *shm << dataIn2;

    pos3 = shm->pos();
    *shm << dataIn3;

    pos4 = shm->pos();
    *shm << dataIn4;

    pos5 = shm->pos();
    *shm << dataIn5;

    pos6 = shm->pos();
    *shm << dataIn6;

    shm->close();

    QVERIFY(shm->open(DuiSharedData::ReadOnly) == true);
    *shm >> dataOut1 >> dataOut2 >> dataOut3 >> dataOut4 >> dataOut5 >> dataOut6;

    QVERIFY(dataIn1 == dataOut1);
    QVERIFY(dataIn2 == dataOut2);
    QVERIFY(dataIn3 == dataOut3);
    QVERIFY(dataIn4 == dataOut4);
    QVERIFY(dataIn5 == dataOut5);
    QVERIFY(dataIn6 == dataOut6);
    shm->close();

    QVERIFY(shm->open(DuiSharedData::ReadOnly) == true);

    shm->seek(pos6);
    *shm >> dataOut6;
    QVERIFY(dataIn6 == dataOut6);

    shm->seek(pos5);
    *shm >> dataOut5;
    QVERIFY(dataIn5 == dataOut5);

    shm->seek(pos4);
    *shm >> dataOut4;
    QVERIFY(dataIn4 == dataOut4);

    shm->seek(pos3);
    *shm >> dataOut3;
    QVERIFY(dataIn3 == dataOut3);

    shm->seek(pos2);
    *shm >> dataOut2;
    QVERIFY(dataIn2 == dataOut2);

    shm->seek(pos1);
    *shm >> dataOut1;
    QVERIFY(dataIn1 == dataOut1);

    shm->close();
    delete shm;
}

// generate random data and save it to shm
void Ut_DuiSharedData::testReadWrite_N(DuiSharedData *shm, QStringList &data, QList<qint64> &offsets)
{
    int len, i, j;

    srand(time(NULL));

    for (i = 0; i < 100; i++) {
        len = rand() % 100 + 100;
        QString newString;

        newString.resize(len);

        for (j = 0; j < len; j++) {
            newString[j] = 'a' + rand() % 26;
        }

        qint64 offset = shm->pos();
        *shm << newString;

        data << newString;
        offsets << offset;
    }
}

// verify that data is shared
void Ut_DuiSharedData::testReadWrite_N_verify(DuiSharedData *shm, QStringList &data, QList<qint64> &offsets)
{
    int i;

    for (i = 0; i < 100; i++) {
        int ind = rand() % 100;
        QString str;

        shm->seek(offsets[ind]);
        *shm >> str;
        QVERIFY(str == data[ind]);
    }
}

// test read/write of shared memory with pageSize=1000 bytes
void Ut_DuiSharedData::testReadWrite_1000()
{
    QStringList data;
    QList<qint64> offsets;

    DuiSharedData *shm1 = new DuiSharedData("buffer1K");
    shm1->setPageSize(1000);
    QVERIFY(shm1->open(DuiSharedData::Write) == true);

    testReadWrite_N(shm1, data, offsets);

    shm1->close();

    DuiSharedData *shm2 = new DuiSharedData("buffer1K");
    shm2->setPageSize(1000);
    QVERIFY(shm2->open(DuiSharedData::ReadOnly) == true);

    testReadWrite_N_verify(shm2, data, offsets);

    shm2->close();

    delete shm1;
    delete shm2;
}

// test read/write of shared memory with pageSize=2000 bytes
void Ut_DuiSharedData::testReadWrite_2000()
{
    QStringList data;
    QList<qint64> offsets;

    DuiSharedData *shm1 = new DuiSharedData("buffer2K");
    shm1->setPageSize(2000);
    QVERIFY(shm1->open(DuiSharedData::Write) == true);

    testReadWrite_N(shm1, data, offsets);

    shm1->close();

    DuiSharedData *shm2 = new DuiSharedData("buffer2K");
    shm2->setPageSize(2000);
    QVERIFY(shm2->open(DuiSharedData::ReadOnly) == true);

    testReadWrite_N_verify(shm2, data, offsets);

    shm2->close();

    delete shm1;
    delete shm2;
}

// test read/write of shared memory with pageSize=5000 bytes
void Ut_DuiSharedData::testReadWrite_5000()
{
    QStringList data;
    QList<qint64> offsets;

    DuiSharedData *shm1 = new DuiSharedData("buffer5K");
    shm1->setPageSize(5000);
    QVERIFY(shm1->open(DuiSharedData::Write) == true);

    testReadWrite_N(shm1, data, offsets);

    shm1->close();

    DuiSharedData *shm2 = new DuiSharedData("buffer5K");
    shm2->setPageSize(5000);
    QVERIFY(shm2->open(DuiSharedData::ReadOnly) == true);

    testReadWrite_N_verify(shm2, data, offsets);

    shm2->close();

    delete shm1;
    delete shm2;
}

// test read/write of shared memory with pageSize=10000 bytes
void Ut_DuiSharedData::testReadWrite_10000()
{
    QStringList data;
    QList<qint64> offsets;

    DuiSharedData *shm1 = new DuiSharedData("buffer10K");
    shm1->setPageSize(10000);
    QVERIFY(shm1->open(DuiSharedData::Write) == true);

    testReadWrite_N(shm1, data, offsets);

    shm1->close();

    DuiSharedData *shm2 = new DuiSharedData("buffer10K");
    shm2->setPageSize(10000);
    QVERIFY(shm2->open(DuiSharedData::ReadOnly) == true);

    testReadWrite_N_verify(shm2, data, offsets);

    shm2->close();

    delete shm1;
    delete shm2;
}

// test read/write of shared memory with pageSize=20000 bytes
void Ut_DuiSharedData::testReadWrite_20000()
{
    QStringList data;
    QList<qint64> offsets;

    DuiSharedData *shm1 = new DuiSharedData("buffer20K");
    shm1->setPageSize(20000);
    QVERIFY(shm1->open(DuiSharedData::Write) == true);

    testReadWrite_N(shm1, data, offsets);

    shm1->close();

    DuiSharedData *shm2 = new DuiSharedData("buffer20K");
    shm2->setPageSize(20000);
    QVERIFY(shm2->open(DuiSharedData::ReadOnly) == true);

    testReadWrite_N_verify(shm2, data, offsets);

    shm2->close();

    delete shm1;
    delete shm2;
}


QTEST_APPLESS_MAIN(Ut_DuiSharedData)
