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

#include "ut_msettingslanguagebinary.h"

#include <msettingslanguagebinary.h>
#include <msettingslanguagesettings.h>
#include <msettingslanguageselection_stub.h>
#include <msettingslanguagetext_stub.h>
#include <msettingslanguageboolean_stub.h>
#include <msettingslanguageinteger_stub.h>

// Called before the first testfunction is executed
void Ut_MSettingsLanguageBinary::initTestCase()
{
}

// Called after the last testfunction was executed
void Ut_MSettingsLanguageBinary::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_MSettingsLanguageBinary::init()
{
    m_subject = new MSettingsLanguageBinary();
}

// Called after every testfunction
void Ut_MSettingsLanguageBinary::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_MSettingsLanguageBinary::testKeys()
{
    QCOMPARE(m_subject->numChildren(), uint(0));

    // Create some test nodes with keys
    gMSettingsLanguageSelectionStub->stubSetReturnValue("key", QString("enumKey"));
    gMSettingsLanguageTextStub->stubSetReturnValue("key", QString("textKey"));
    gMSettingsLanguageBooleanStub->stubSetReturnValue("key", QString("boolKey"));
    gMSettingsLanguageIntegerStub->stubSetReturnValue("key", QString("intKey"));
    MSettingsLanguageSettings *child = new MSettingsLanguageSettings();
    MSettingsLanguageSelection *aEnum = new MSettingsLanguageSelection("enumKey", "foobar");
    MSettingsLanguageText *aText = new MSettingsLanguageText("textKey", "bar");
    MSettingsLanguageBoolean *aBool = new MSettingsLanguageBoolean("boolKey", "boolBar");
    MSettingsLanguageInteger *aInteger = new MSettingsLanguageInteger("intKey", "foo");
    m_subject->addChild(child);
    child->addChild(aEnum);
    child->addChild(aText);
    child->addChild(aBool);
    child->addChild(aInteger);
    QList<MSettingsLanguageNode *> children = m_subject->children();
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

QTEST_MAIN(Ut_MSettingsLanguageBinary)
