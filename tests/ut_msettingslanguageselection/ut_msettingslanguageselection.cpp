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

#include <msettingslanguageselection.h>
#include <msettingslanguageoption_stub.h>
#include "ut_msettingslanguageselection.h"

void Ut_MSettingsLanguageSelection::init()
{
    m_subject = new MSettingsLanguageSelection("key", "title");
}

void Ut_MSettingsLanguageSelection::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettingsLanguageSelection::initTestCase()
{
}

void Ut_MSettingsLanguageSelection::cleanupTestCase()
{
}

void Ut_MSettingsLanguageSelection::testEnumKey()
{
    QCOMPARE(m_subject->key(), QString("key"));
}

void Ut_MSettingsLanguageSelection::testAddEnumValuesToEnum()
{
    QCOMPARE(m_subject->numOptions(), uint(0));
    MSettingsLanguageOption *value = m_subject->addOption("title1", 0);
    QVERIFY(value != NULL);
    QCOMPARE(m_subject->numOptions(), uint(1));

    MSettingsLanguageOption *eValue = new MSettingsLanguageOption("title2", 1);
    MSettingsLanguageOption *eValue2 = m_subject->addOption(eValue);
    QCOMPARE(eValue, eValue2);
    QCOMPARE(m_subject->numOptions(), uint(2));
}

void Ut_MSettingsLanguageSelection::testGetValuesFromEnum()
{
    MSettingsLanguageOption *value1 = m_subject->addOption("title1", 1);
    MSettingsLanguageOption *value2 = m_subject->addOption("title2", 2);
    QList<const MSettingsLanguageOption *> values = m_subject->options();
    QCOMPARE(values.count(), 2);
    QCOMPARE(values.at(0), value1);
    QCOMPARE(values.at(1), value2);
}

void Ut_MSettingsLanguageSelection::testNodeType()
{
    QVERIFY((dynamic_cast<const MSettingsLanguageSelection *>(m_subject)));
}

QTEST_MAIN(Ut_MSettingsLanguageSelection)
