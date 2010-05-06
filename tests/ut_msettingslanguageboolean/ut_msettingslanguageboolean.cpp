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

#include <msettingslanguageboolean.h>

#include "ut_msettingslanguageboolean.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_MSettingsLanguageBoolean::init()
{
    m_subject = new MSettingsLanguageBoolean("key", "title1");
}

void Ut_MSettingsLanguageBoolean::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettingsLanguageBoolean::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_MSettingsLanguageBoolean::cleanupTestCase()
{
}

void Ut_MSettingsLanguageBoolean::testBooleanKey()
{
    QCOMPARE(m_subject->key(), QString("key"));
}

void Ut_MSettingsLanguageBoolean::testBooleanTitle()
{
    QCOMPARE(m_subject->title(), QString("title1"));
}

void Ut_MSettingsLanguageBoolean::testNodeType()
{
    QVERIFY((dynamic_cast<const MSettingsLanguageBoolean *>(m_subject)));
}

void Ut_MSettingsLanguageBoolean::testBooleanTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("title1Localized"));
}

QTEST_MAIN(Ut_MSettingsLanguageBoolean)
