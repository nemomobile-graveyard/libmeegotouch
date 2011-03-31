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

#include <msettingslanguagesettings.h>
#include <msettingslanguagenode.h>
#include <msettingslanguageselection_stub.h>

#include "ut_msettingslanguagesettings.h"

void Ut_MSettingsLanguageSettings::init()
{
    m_subject = new MSettingsLanguageSettings;
}

void Ut_MSettingsLanguageSettings::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettingsLanguageSettings::initTestCase()
{
}

void Ut_MSettingsLanguageSettings::cleanupTestCase()
{
}

void Ut_MSettingsLanguageSettings::testAddChildToItem()
{
    QCOMPARE(m_subject->numChildren(), uint(0));
    MSettingsLanguageSelection *aEnum = new MSettingsLanguageSelection("foo", "bar");
    m_subject->addChild(aEnum);
    QCOMPARE(m_subject->numChildren(), uint(1));
}

void Ut_MSettingsLanguageSettings::testNodeType()
{
    QVERIFY((dynamic_cast<const MSettingsLanguageSettings *>(m_subject)));
}

QTEST_MAIN(Ut_MSettingsLanguageSettings)
