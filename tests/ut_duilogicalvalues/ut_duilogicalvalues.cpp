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

#include <QTest>
#include <QDebug>
#include <QDir>
#include "ut_duilogicalvalues.h"

Ut_DuiLogicalValues::Ut_DuiLogicalValues()
{
}

void Ut_DuiLogicalValues::init()
{
    m_subject = new DuiLogicalValues();
}

void Ut_DuiLogicalValues::cleanup()
{
    delete m_subject;
}

void Ut_DuiLogicalValues::testFontsAndColors()
{
    QCOMPARE(m_subject->append(qApp->applicationDirPath() + QDir::separator() + "test2.ini"), true);
    QCOMPARE(m_subject->append(qApp->applicationDirPath() + QDir::separator() + "test.ini"), true);

    QCOMPARE(m_subject->color("Palette", "COLOR_DEFAULT"), QColor("#000000"));
    QCOMPARE(m_subject->color("Palette", "COLOR_OVERRIDDEN"), QColor("#ffffff"));
    QCOMPARE(m_subject->color("Palette", "COLOR_ADDED"), QColor("#00ff00"));

    QFont FontArial20px("Arial"), FontArial100px("Arial"), FontArial10px("Arial");
    FontArial20px.setPixelSize(20);
    FontArial100px.setPixelSize(100);
    FontArial10px.setPixelSize(10);

    QCOMPARE(m_subject->font("Fonts", "FONT_DEFAULT"), FontArial20px);
    QCOMPARE(m_subject->font("Fonts", "FONT_OVERRIDDEN"), FontArial100px);
    QCOMPARE(m_subject->font("Fonts", "FONT_ADDED"), FontArial10px);
}

QTEST_MAIN(Ut_DuiLogicalValues)

