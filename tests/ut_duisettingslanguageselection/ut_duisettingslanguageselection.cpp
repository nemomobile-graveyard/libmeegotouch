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

#include <duisettingslanguageselection.h>
#include <duisettingslanguageoption_stub.h>
#include "ut_duisettingslanguageselection.h"

void Ut_DuiSettingsLanguageSelection::init()
{
    m_subject = new DuiSettingsLanguageSelection("key");
}

void Ut_DuiSettingsLanguageSelection::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettingsLanguageSelection::initTestCase()
{
}

void Ut_DuiSettingsLanguageSelection::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageSelection::testEnumKey()
{
    QCOMPARE(m_subject->key(), QString("key"));
}

void Ut_DuiSettingsLanguageSelection::testAddEnumValuesToEnum()
{
    QCOMPARE(m_subject->numOptions(), uint(0));
    DuiSettingsLanguageOption *value = m_subject->addOption("title1", 0);
    QVERIFY(value != NULL);
    QCOMPARE(m_subject->numOptions(), uint(1));

    DuiSettingsLanguageOption *eValue = new DuiSettingsLanguageOption("title2", 1);
    DuiSettingsLanguageOption *eValue2 = m_subject->addOption(eValue);
    QCOMPARE(eValue, eValue2);
    QCOMPARE(m_subject->numOptions(), uint(2));
}

void Ut_DuiSettingsLanguageSelection::testGetValuesFromEnum()
{
    DuiSettingsLanguageOption *value1 = m_subject->addOption("title1", 1);
    DuiSettingsLanguageOption *value2 = m_subject->addOption("title2", 2);
    QList<const DuiSettingsLanguageOption *> values = m_subject->options();
    QCOMPARE(values.count(), 2);
    QCOMPARE(values.at(0), value1);
    QCOMPARE(values.at(1), value2);
}

void Ut_DuiSettingsLanguageSelection::testNodeType()
{
    QVERIFY((dynamic_cast<const DuiSettingsLanguageSelection *>(m_subject)));
}

QTEST_MAIN(Ut_DuiSettingsLanguageSelection)
