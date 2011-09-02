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

#ifndef PT_LOCALE_H
#define PT_LOCALE_H

#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QTextCodec>
#include <QTextStream>
#include <QObject>
#include <MLocale>
#include <MCollator>
#ifdef HAVE_ICU
#include <unicode/uversion.h>
#include <unicode/uloc.h>
#endif

class Pt_MLocale : public QObject
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

#ifdef HAVE_ICU
    void benchmarkSingleLanguageEndonym();
    void benchmarkAllLanguageEndonym();
    void benchmarkSingleCountryEndonym();
    void benchmarkAllCountryEndonym();
    void benchmarkFormatNumberQLongLongArabic();
    void benchmarkFormatNumberQLongLongWestern();
    void benchmarkFormatNumberDoubleArabic();
    void benchmarkFormatNumberDoubleWestern();
    void benchmarkChineseSorting();
    void benchmarkCollatorStrengthSwitching();
#endif
};

#endif
