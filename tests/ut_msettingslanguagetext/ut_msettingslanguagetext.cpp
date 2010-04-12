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

#include <msettingslanguagetext.h>

#include "ut_msettingslanguagetext.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_MSettingsLanguageText::init()
{
    m_subject = new MSettingsLanguageText("key", "title");
}

void Ut_MSettingsLanguageText::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettingsLanguageText::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_MSettingsLanguageText::cleanupTestCase()
{
}

void Ut_MSettingsLanguageText::testTextKeyTitle()
{
    QCOMPARE(m_subject->key(), QString("key"));
    QCOMPARE(m_subject->title(), QString("title"));
}

void Ut_MSettingsLanguageText::testNodeType()
{
    QVERIFY((dynamic_cast<const MSettingsLanguageText *>(m_subject)));
}

void Ut_MSettingsLanguageText::testTextTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("titleLocalized"));
}

QTEST_MAIN(Ut_MSettingsLanguageText)
