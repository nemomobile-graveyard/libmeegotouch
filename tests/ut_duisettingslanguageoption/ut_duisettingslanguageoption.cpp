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

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include "ut_duisettingslanguageoption.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_DuiSettingsLanguageOption::init()
{
    m_subject = new DuiSettingsLanguageOption("foo", 1);
}

void Ut_DuiSettingsLanguageOption::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettingsLanguageOption::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_DuiSettingsLanguageOption::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageOption::testOptionInitialization()
{
    QCOMPARE(m_subject->title(), QString("foo"));
    QCOMPARE(m_subject->value(), 1);
}

void Ut_DuiSettingsLanguageOption::testNodeType()
{
    QVERIFY((dynamic_cast<const DuiSettingsLanguageOption *>(m_subject)));
}

void Ut_DuiSettingsLanguageOption::testOptionTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("fooLocalized"));
}

QTEST_MAIN(Ut_DuiSettingsLanguageOption)
