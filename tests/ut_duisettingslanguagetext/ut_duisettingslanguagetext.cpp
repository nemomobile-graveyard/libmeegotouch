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

#include <duisettingslanguagetext.h>

#include "ut_duisettingslanguagetext.h"

bool gLocalizeStrings = false;
QString qtTrId(const char *id, int)
{
    if (gLocalizeStrings) {
        return QString(id) + "Localized";
    } else {
        return id;
    }
}

void Ut_DuiSettingsLanguageText::init()
{
    m_subject = new DuiSettingsLanguageText("key", "title");
}

void Ut_DuiSettingsLanguageText::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSettingsLanguageText::initTestCase()
{
    gLocalizeStrings = false;
}

void Ut_DuiSettingsLanguageText::cleanupTestCase()
{
}

void Ut_DuiSettingsLanguageText::testTextKeyTitle()
{
    QCOMPARE(m_subject->key(), QString("key"));
    QCOMPARE(m_subject->title(), QString("title"));
}

void Ut_DuiSettingsLanguageText::testNodeType()
{
    QVERIFY((dynamic_cast<const DuiSettingsLanguageText *>(m_subject)));
}

void Ut_DuiSettingsLanguageText::testTextTitleLocalized()
{
    gLocalizeStrings = true;
    QCOMPARE(m_subject->title(), QString("titleLocalized"));
}

QTEST_MAIN(Ut_DuiSettingsLanguageText)
