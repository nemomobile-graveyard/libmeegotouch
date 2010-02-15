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

#ifndef FT_NUMBERS_H
#define FT_NUMBERS_H


#include <QtTest/QtTest>
#include <QObject>
#include <DuiLocale>


Q_DECLARE_METATYPE(DuiLocale);
Q_DECLARE_METATYPE(qlonglong);

#define MAX_PARAMS 10
class Ft_Numbers : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *qap;

private slots:
    void initTestCase();
    void cleanup();

    void testFloats_data();
    void testFloats();

    void testShorts_data();
    void testShorts();

    void testQLongLongs_data();
    void testQLongLongs();

    void testInts_data();
    void testInts();

    void testDoubles_data();
    void testDoubles();

    void testDoublesWithFormatting_data();

    void testDoublesWithFormatting();

    void testPercents_data();
    void testPercents();

    void testCurrencies_data();
    void testCurrencies();
};


#endif
