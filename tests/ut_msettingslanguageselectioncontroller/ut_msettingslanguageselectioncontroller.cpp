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

#include "ut_msettingslanguageselectioncontroller.h"
#include <msettingslanguageselectioncontroller.h>

#include <QtTest/QtTest>
#include <MButton>
#include <MDataStore>
#include "../stubs/mockdatastore.h"

void Ut_MSettingsLanguageSelectionController::init()
{
    m_subject = new MSettingsLanguageSelectionController();
}

void Ut_MSettingsLanguageSelectionController::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSettingsLanguageSelectionController::initTestCase()
{

}

void Ut_MSettingsLanguageSelectionController::cleanupTestCase()
{
}

void Ut_MSettingsLanguageSelectionController::testButtonClicked()
{
    MockDataStore dataStore;
    MButton button;
    button.setProperty("key", QString("buttonKey"));
    button.setProperty("value", 1);
    button.setProperty("dataStore", qVariantFromValue(static_cast<void *>(&dataStore)));
    m_subject->buttonClicked(&button);
    QCOMPARE(dataStore.value("buttonKey").toInt(), 1);
}

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageSelectionController)
