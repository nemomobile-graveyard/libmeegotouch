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

#ifndef FT_LOCALES_H
#define FT_LOCALES_H

#include <QtTest/QtTest>
#include <QObject>
#include <Qt>

#include <DuiLocale>
#include <DuiApplication>
#include <DuiGConfItem>

#ifdef HAVE_ICU
#include <unicode/unistr.h>
#include <unicode/ucal.h>
#include <unicode/coll.h>
#include <unicode/fieldpos.h>
#include <unicode/datefmt.h>
#include <unicode/calendar.h>
#include <unicode/smpdtfmt.h> // SimpleDateFormat
#include <unicode/numfmt.h>
#include <unicode/uloc.h>
#include <unicode/dtfmtsym.h> // date format symbols
#include <unicode/putil.h> // u_setDataDirectory
#endif

Q_DECLARE_METATYPE(DuiLocale::CalendarType);
Q_DECLARE_METATYPE(DuiLocale);


class Ft_Locales : public QObject
{
    Q_OBJECT

private:
    DuiApplication *qap;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // these from ut_misc
    void testSettingsChanged(); // Must be the first test
    void testDuiLocaleConstructor();

    void testDuiLocaleConstructorWithParams_data();
    void testDuiLocaleConstructorWithParams();

    void testDuiLocaleConstructorAndCategoryWithParams_data();
    void testDuiLocaleConstructorAndCategoryWithParams();

    void testCreateSystemLocale_data();
    void testCreateSystemLocale();
    void testDuiLocaleLanguage_data();
    void testDuiLocaleLanguage();
    void testDuiLocaleCountry_data();
    void testDuiLocaleCountry();
    void testDuiLocaleScript_data();
    void testDuiLocaleScript();

    void testDuiLocaleVariant_data();
    void testDuiLocaleVariant();

    void testDuiLocaleTextDirection_data();
    void testDuiLocaleTextDirection();

    void testDuiLocaleLanguageEndonum_data();
    void testDuiLocaleLanguageEndonum();
    void testDuiLocaleCountryEndonum_data();
    void testDuiLocaleCountryEndonum();

    void checkAvailableLocales();
};


#endif
