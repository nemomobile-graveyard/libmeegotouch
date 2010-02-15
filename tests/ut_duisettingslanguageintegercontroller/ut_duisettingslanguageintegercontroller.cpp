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

#include "ut_duisettingslanguageintegercontroller.h"
#include <duisettingslanguageintegercontroller.h>

#include <QtTest/QtTest>
#include <DuiApplication>
#include <DuiDataStore>
#include "../stubs/mockdatastore.h"

void TestSlider::changeValue(int newValue)
{
    emit valueChanged(newValue);
}

void Ut_DuiSettingsLanguageIntegerController::init()
{
    m_subject = new DuiSettingsLanguageIntegerController();
}

void Ut_DuiSettingsLanguageIntegerController::cleanup()
{
    delete m_subject;
}

void Ut_DuiSettingsLanguageIntegerController::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_duisettingslanguageintegercontroller" };
    app = new DuiApplication(argc, argv);
}

void Ut_DuiSettingsLanguageIntegerController::cleanupTestCase()
{
    delete app;
}

void Ut_DuiSettingsLanguageIntegerController::testValueChanged()
{
    MockDataStore dataStore;
    dataStore.createValue(QString("integerKey"), 12);

    TestSlider slider;
    slider.setValue(10);

    m_subject->setProperty("key", QString("integerKey"));
    m_subject->setProperty("dataStore", qVariantFromValue(static_cast<void *>(&dataStore)));

    connect(&slider, SIGNAL(valueChanged(int)), m_subject, SLOT(changeValue(int)));

    QCOMPARE(dataStore.value("integerKey").toInt(), 12);
    slider.changeValue(25);
    QCOMPARE(dataStore.value("integerKey").toInt(), 25);
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageIntegerController)
