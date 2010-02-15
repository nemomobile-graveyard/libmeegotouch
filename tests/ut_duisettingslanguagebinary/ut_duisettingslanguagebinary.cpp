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

#include "ut_duisettingslanguagebinary.h"

#include <duisettingslanguagebinary.h>
#include <duisettingslanguagesettings.h>
#include <duisettingslanguageselection_stub.h>
#include <duisettingslanguagetext_stub.h>
#include <duisettingslanguageboolean_stub.h>
#include <duisettingslanguageinteger_stub.h>

// Called before the first testfunction is executed
void Ut_DuiSettingsLanguageBinary::initTestCase()
{
}

// Called after the last testfunction was executed
void Ut_DuiSettingsLanguageBinary::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_DuiSettingsLanguageBinary::init()
{
    m_subject = new DuiSettingsLanguageBinary();
}

// Called after every testfunction
void Ut_DuiSettingsLanguageBinary::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_DuiSettingsLanguageBinary::testKeys()
{
    // Create some test nodes with keys
    gDuiSettingsLanguageSelectionStub->stubSetReturnValue("key", QString("enumKey"));
    gDuiSettingsLanguageTextStub->stubSetReturnValue("key", QString("textKey"));
    gDuiSettingsLanguageBooleanStub->stubSetReturnValue("key", QString("boolKey"));
    gDuiSettingsLanguageIntegerStub->stubSetReturnValue("key", QString("intKey"));
    DuiSettingsLanguageSettings *child = new DuiSettingsLanguageSettings();
    DuiSettingsLanguageSelection *aEnum = new DuiSettingsLanguageSelection("enumKey");
    DuiSettingsLanguageText *aText = new DuiSettingsLanguageText("textKey", "bar");
    DuiSettingsLanguageBoolean *aBool = new DuiSettingsLanguageBoolean("boolKey", "boolBar");
    DuiSettingsLanguageInteger *aInteger = new DuiSettingsLanguageInteger("intKey", "foo");
    QCOMPARE(m_subject->numChildren(), uint(0));
    m_subject->addChild(child);
    child->addChild(aEnum);
    child->addChild(aText);
    child->addChild(aBool);
    child->addChild(aInteger);
    QList<DuiSettingsLanguageNode *> children = m_subject->children();
    QCOMPARE(children.count(), 1);
    QCOMPARE(children.at(0), child);
    QCOMPARE(child->numChildren(), uint(4));

    // Get the keys
    QList<QString> keys = m_subject->keys();
    QCOMPARE(keys.count(), 4);
    QVERIFY(keys.contains(QString("enumKey")));
    QVERIFY(keys.contains(QString("textKey")));
    QVERIFY(keys.contains(QString("boolKey")));
    QVERIFY(keys.contains(QString("intKey")));
}

QTEST_MAIN(Ut_DuiSettingsLanguageBinary)
