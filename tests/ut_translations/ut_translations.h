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

#ifndef UT_TRANSLATIONS_H
#define UT_TRANSLATIONS_H


#include <QtTest/QtTest>
#include <QObject>
#define private public
#include <MLocale>
#undef private

Q_DECLARE_METATYPE(MLocale::Category);
Q_DECLARE_METATYPE(MLocale);

class Ut_Translations : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *qap;

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    void testTranslationPathModificationMethods();
    void testinstallTrCatalogThenUseQtTr_data();
    void testinstallTrCatalogThenUseQtTr();
    void testOriginalQtTr_data();
    void testOriginalQtTr();
    void testQtTrId_data();
    void testQtTrId();
    void testQtTrIdLocalizedNumbers_data();
    void testQtTrIdLocalizedNumbers();
    void testQtTrIdLocalizedNumbersPlural_data();
    void testQtTrIdLocalizedNumbersPlural();
    void testQtTrIdMultipleVariable_data();
    void testQtTrIdMultipleVariable();
    void testQtTrIdMultipleVariableWithPlural_data();
    void testQtTrIdMultipleVariableWithPlural();
    void testGettingTheDefaultLocaleFromTheEnvironment_data();
    void testGettingTheDefaultLocaleFromTheEnvironment();
    void testCreatingAndDestroyingLocales();
    void testInstallTrCatalogMultipleTimes();
    void benchmarkMLocaleConstructorAndDelete();
    void benchmarkMLocaleConstructorAndDeleteStack();
    void benchmarkQLocaleConstructorAndDelete();
    void benchmarkQLocaleConstructorAndDeleteStack();
    void benchmarkMLocaleAssignmentOperator();
    void benchmarkMLocaleAssignmentOperatorStack();
    void benchmarkQLocaleAssignmentOperator();
    void benchmarkQLocaleAssignmentOperatorStack();
    void benchmarkMLocaleCopyConstructorAndDelete();
    void benchmarkMLocaleCopyConstructorAndDeleteStack();
    void benchmarkQLocaleCopyConstructorAndDelete();
    void benchmarkQLocaleCopyConstructorAndDeleteStack();
};


#endif
