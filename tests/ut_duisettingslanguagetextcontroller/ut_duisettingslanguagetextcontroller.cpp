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

#include "ut_duisettingslanguagetextcontroller.h"
#include <duisettingslanguagetextcontroller.h>

#include <QtTest/QtTest>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiTextEdit>
#include <DuiDataStore>
#include "../stubs/mockdatastore.h"

void TestTextEdit::focusOut()
{
    QFocusEvent event(QEvent::FocusOut);
    focusOutEvent(&event);
}

void Ut_DuiSettingsLanguageTextController::init()
{
    m_subject = new DuiSettingsLanguageTextController();
}

void Ut_DuiSettingsLanguageTextController::cleanup()
{
    delete m_subject;
}

void Ut_DuiSettingsLanguageTextController::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_duisettingslanguagetextcontroller" };
    app = new DuiApplication(argc, argv);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiSettingsLanguageTextController::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiSettingsLanguageTextController::testTextChanged()
{
    MockDataStore dataStore;
    TestTextEdit textEdit;
    textEdit.setProperty("key", QString("textKey"));
    textEdit.setText("Test");
    textEdit.setProperty("dataStore", qVariantFromValue(static_cast<void *>(&dataStore)));

    connect(&textEdit, SIGNAL(lostFocus(Qt::FocusReason)), m_subject, SLOT(textEditLostFocus(Qt::FocusReason)));
    textEdit.focusOut();
    QCOMPARE(dataStore.value("textKey").toString(), QString("Test"));
}

QTEST_APPLESS_MAIN(Ut_DuiSettingsLanguageTextController)
