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

#include "pt_mstylesheet.h"

#include <mbenchmark.h>
#include <MComponentData>
#include <mlogicalvalues.h>
#include <mstylesheet.h>

#include <QtTest/QtTest>

void Pt_MStyleSheet::constructor()
{
    MStyleSheet *styleSheet = 0;
    MBENCHMARK_ONCE (
        styleSheet = new MStyleSheet();
    )
    delete styleSheet;
}

void Pt_MStyleSheet::loadThemeStyles()
{
    QFETCH(int, repeatCount);

    MLogicalValues logicalValues;
    initLogicalValues(logicalValues);

    QStringList files;
    files.append("/usr/share/themes/base/meegotouch/libmeegotouchextensions/style/libmeegotouchextensions.css");
    files.append("/usr/share/themes/base/meegotouch/libmeegotouchcore/style/libmeegotouchcore.css");
    files.append("/usr/share/themes/base/meegotouch/libmeegotouchviews/style/libmeegotouchviews.css");

    MBENCHMARK_ONCE (
        for (int i = 0; i < repeatCount; ++i) {
            foreach (const QString &file, files) {
                MStyleSheet styleSheet(&logicalValues);
                styleSheet.load(file);
            }
        }
    )
}

void Pt_MStyleSheet::loadThemeStyles_data()
{
    QTest::addColumn<int>("repeatCount");
    QTest::newRow("Read once") << 1;
    QTest::newRow("Read 2 times") << 2;
    QTest::newRow("Read 5 times") << 5;
    QTest::newRow("Read 10 times") << 10;
}

void Pt_MStyleSheet::loadCoreStyles()
{
    QFETCH(QString, style);
    QFETCH(int, repeatCount);

    MLogicalValues logicalValues;
    initLogicalValues(logicalValues);

    MBENCHMARK_ONCE (
        for (int i = 0; i < repeatCount; ++i) {
            MStyleSheet styleSheet(&logicalValues);
            styleSheet.load("/usr/share/themes/base/meegotouch/libmeegotouchcore/style/" + style);
        }
    )
}

void Pt_MStyleSheet::loadCoreStyles_data()
{
    QTest::addColumn<QString>("style");
    QTest::addColumn<int>("repeatCount");

    QTest::newRow("commonlayouts (read once)")  << "../../libmeegotouchviews/style/commonlayouts/commonlayouts.css" << 1;
    QTest::newRow("commonlayouts (read twice)") << "../../libmeegotouchviews/style/commonlayouts/commonlayouts.css" << 2;
    QTest::newRow("mwidgetstyle (read once)")   << "mwidgetstyle.css"  << 1;
    QTest::newRow("mwidgetstyles (read twice)") << "mwidgetstyle.css"  << 2;
    QTest::newRow("deprecated (read once)")     << "deprecated.css"    << 1;
    QTest::newRow("deprecated (read twice)")    << "deprecated.css"    << 2;
}

void Pt_MStyleSheet::initLogicalValues(MLogicalValues &values)
{
    QStringList themeInheritance;
    themeInheritance.append("/usr/share/themes/base/");

    values.load(themeInheritance, QString());
}

void Pt_MStyleSheet::testBinarySpeed()
{
    // create binary file, if it doesn't exist
    MStyleSheetParser tmp;
    tmp.setBinaryFileGenerationEnabled(true);
    QVERIFY(tmp.load(qApp->applicationDirPath() + "/pt_mstylesheet_test.css"));

    QBENCHMARK {
        MStyleSheetParser p;
        p.setBinaryFileGenerationEnabled(true);

        // Open test file
        QVERIFY(p.load(qApp->applicationDirPath() + "/pt_mstylesheet_test.css"));
    }
}

QTEST_MAIN(Pt_MStyleSheet)
