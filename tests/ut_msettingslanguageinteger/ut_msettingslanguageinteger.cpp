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

#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include <msettingslanguageinteger.h>


#include "ut_msettingslanguageinteger.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_MSettingsLanguageInteger::init()
{
    m_subject = new MSettingsLanguageInteger("key", "title");
    m_subject->setMinValue(-100);
    m_subject->setMaxValue(100);
}

void Ut_MSettingsLanguageInteger::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettingsLanguageInteger::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_MSettingsLanguageInteger::cleanupTestCase()
{
}

void Ut_MSettingsLanguageInteger::testIntegerKeyTitle()
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

void Ut_MSettingsLanguageInteger::testNodeType()
{
    QVERIFY((dynamic_cast<const MSettingsLanguageInteger *>(m_subject)));
}

void Ut_MSettingsLanguageInteger::testIntegerTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("titleLocalized"));
}

QTEST_MAIN(Ut_MSettingsLanguageInteger)
