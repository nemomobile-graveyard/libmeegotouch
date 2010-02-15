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

#include <duisettingslanguageboolean.h>

#include "ut_duisettingslanguageboolean.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_DuiSettingsLanguageBoolean::init()
{
    m_subject = new DuiSettingsLanguageBoolean("key", "title1");
}

void Ut_DuiSettingsLanguageBoolean::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettingsLanguageBoolean::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_DuiSettingsLanguageBoolean::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageBoolean::testBooleanKey()
{
    QCOMPARE(m_subject->key(), QString("key"));
}

void Ut_DuiSettingsLanguageBoolean::testBooleanTitle()
{
    QCOMPARE(m_subject->title(), QString("title1"));
}

void Ut_DuiSettingsLanguageBoolean::testNodeType()
{
    QVERIFY((dynamic_cast<const DuiSettingsLanguageBoolean *>(m_subject)));
}

void Ut_DuiSettingsLanguageBoolean::testBooleanTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("title1Localized"));
}

QTEST_MAIN(Ut_DuiSettingsLanguageBoolean)
