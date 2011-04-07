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

#ifndef FT_NUMBERS_H
#define FT_NUMBERS_H

#include <QtTest/QtTest>
#include <QObject>
#include <QTextStream>
#include <QProcess>
#include <MLocale>

#ifdef HAVE_ICU
#include <unicode/numfmt.h>
#endif

Q_DECLARE_METATYPE(MLocale);
Q_DECLARE_METATYPE(qlonglong);

#define MAX_PARAMS 10
class Ft_Numbers : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *qap;
    QString icuPackageVersion;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testQLongLongs_data();
    void testQLongLongs();

    void testToLongLong_data();
    void testToLongLong();

    void testShorts_data();
    void testShorts();

    void testToShort_data();
    void testToShort();

    void testInts_data();
    void testInts();

    void testToInt_data();
    void testToInt();

    void testDoubles_data();
    void testDoubles();

    void testToDouble_data();
    void testToDouble();

    void testFloats_data();
    void testFloats();

    void testToFloat_data();
    void testToFloat();

    void testDoublesWithFormatting_data();
    void testDoublesWithFormatting();

    void testPercents_data();
    void testPercents();

    void testCurrencies_data();
    void testCurrencies();

    void testPercentPlaceholdersInQt_data();
    void testPercentPlaceholdersInQt();
};


#endif
