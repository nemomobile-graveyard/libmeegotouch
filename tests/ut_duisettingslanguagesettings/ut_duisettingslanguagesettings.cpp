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

#include <duisettingslanguagesettings.h>
#include <duisettingslanguagenode.h>
#include <duisettingslanguageselection_stub.h>

#include "ut_duisettingslanguagesettings.h"

void Ut_DuiSettingsLanguageSettings::init()
{
    m_subject = new DuiSettingsLanguageSettings;
}

void Ut_DuiSettingsLanguageSettings::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettingsLanguageSettings::initTestCase()
{
}

void Ut_DuiSettingsLanguageSettings::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageSettings::testAddChildToItem()
{
    DuiSettingsLanguageSelection *aEnum = new DuiSettingsLanguageSelection("foo");
    QCOMPARE(m_subject->numChildren(), uint(0));
    m_subject->addChild(aEnum);
    QCOMPARE(m_subject->numChildren(), uint(1));
}

void Ut_DuiSettingsLanguageSettings::testNodeType()
{
    QVERIFY((dynamic_cast<const DuiSettingsLanguageSettings *>(m_subject)));
}

QTEST_MAIN(Ut_DuiSettingsLanguageSettings)
