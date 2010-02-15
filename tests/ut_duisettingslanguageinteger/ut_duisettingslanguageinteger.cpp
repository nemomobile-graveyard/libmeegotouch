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

#include <duisettingslanguageinteger.h>


#include "ut_duisettingslanguageinteger.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_DuiSettingsLanguageInteger::init()
{
    m_subject = new DuiSettingsLanguageInteger("key", "title");
    m_subject->setMinValue(-100);
    m_subject->setMaxValue(100);
}

void Ut_DuiSettingsLanguageInteger::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettingsLanguageInteger::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_DuiSettingsLanguageInteger::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageInteger::testIntegerKeyTitle()
{
    QCOMPARE(m_subject->key(), QString("key"));
    QCOMPARE(m_subject->title(), QString("title"));
    int minVal;
    QVERIFY(m_subject->minValue(minVal));
    QCOMPARE(minVal, -100);
    int maxVal;
    QVERIFY(m_subject->maxValue(maxVal));
    QCOMPARE(maxVal, 100);
}

void Ut_DuiSettingsLanguageInteger::testNodeType()
{
    QVERIFY((dynamic_cast<const DuiSettingsLanguageInteger *>(m_subject)));
}

void Ut_DuiSettingsLanguageInteger::testIntegerTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("titleLocalized"));
}

QTEST_MAIN(Ut_DuiSettingsLanguageInteger)
