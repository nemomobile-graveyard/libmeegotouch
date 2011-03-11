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

#ifndef FT_SORTING_H
#define FT_SORTING_H


#include <QtTest/QtTest>
#include <QObject>
#include <QCoreApplication>
#include <MLocale>

Q_DECLARE_METATYPE(MLocale);
Q_DECLARE_METATYPE(MLocale::Collation);
Q_DECLARE_METATYPE(MLocale::Comparison);

#define MAX_PARAMS 10
class Ft_Sorting : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *qap;
    QString icuPackageVersion;

private slots:
    void initTestCase();
    void cleanup();

    void testMLocaleSetAndGetCollation_data();
    void testMLocaleSetAndGetCollation();

    void testMLocaleSorting_data();
    void testMLocaleSorting();

    void testDefaultCompare_data();
    void testDefaultCompare();

    void testCompareWithLocale_data();
    void testCompareWithLocale();
};


#endif
