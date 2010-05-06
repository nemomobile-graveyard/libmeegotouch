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

#include "ut_msettingslanguagetextcontroller.h"
#include <msettingslanguagetextcontroller.h>

#include <QtTest/QtTest>
#include <MApplication>
#include <MApplicationWindow>
#include <MTextEdit>
#include <MDataStore>
#include "../stubs/mockdatastore.h"

void TestTextEdit::focusOut()
{
    QFocusEvent event(QEvent::FocusOut);
    focusOutEvent(&event);
}

void Ut_MSettingsLanguageTextController::init()
{
    m_subject = new MSettingsLanguageTextController();
}

void Ut_MSettingsLanguageTextController::cleanup()
{
    delete m_subject;
}

void Ut_MSettingsLanguageTextController::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_msettingslanguagetextcontroller" };
    app = new MApplication(argc, argv);
    appWin = new MApplicationWindow;
}

void Ut_MSettingsLanguageTextController::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MSettingsLanguageTextController::testTextChanged()
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

QTEST_APPLESS_MAIN(Ut_MSettingsLanguageTextController)
