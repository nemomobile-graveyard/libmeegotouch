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
#include <QApplication>
#include <QStyle>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>

#include <qtmaemo6style.h>
#include "duiapplication.h"

#include "ut_qtmaemo6style.h"

/*
  NOTE: this test is only included into test package if the Qt Maemo 6 style is
        included to libdui.
*/

void Ut_QtMaemo6Style::init()
{
}

void Ut_QtMaemo6Style::cleanup()
{
}

void Ut_QtMaemo6Style::initTestCase()
{
}

void Ut_QtMaemo6Style::cleanupTestCase()
{
}

void Ut_QtMaemo6Style::testStyleLoadedByDefault() {
    int argc = 1;
    char *argv[1] = { (char *) "./ut_qtmaemo6style" };
    QApplication app(argc, argv);
    QStyle* style = app.style();
    QVERIFY(qobject_cast<QtMaemo6Style*>(style));
    //delete app;
}

void Ut_QtMaemo6Style::testLoadOtherStyle() {
    int argc = 3;
    char *argv[3] = { (char *) "./ut_qtmaemo6style", (char *) "-style", (char *) "windows"};
    QApplication app(argc, argv);
    QStyle* style = qobject_cast<QWindowsStyle*>(app.style());
    QVERIFY(style);
    //delete app;
}

void Ut_QtMaemo6Style::testWindowDecoration() {
    int argc = 3;
    char *argv[3] = { (char *) "./ut_qtmaemo6style", (char *) "-style", (char *) "maemo6"};
    m_app = new QApplication(argc, argv);
    QVERIFY(qobject_cast<QtMaemo6Style*>(m_app->style()));

    m_mw = new QMainWindow();
    m_mw->show();
    QTimer::singleShot(1000, this, SLOT(testWindowDecorationSlot()));
    //just ensure that the application quits, even if the test fails
    QTimer::singleShot(5000, m_app, SLOT(quit()));
    m_app->exec();

    delete m_mw;
    m_mw = NULL;
    delete m_app;
    m_app = NULL;
}

void Ut_QtMaemo6Style::testWindowDecorationSlot() {
    if(m_app && m_mw) {
        QWidget* pw = NULL;
        pw = m_mw->parentWidget();
        QVERIFY(pw);
        //search the toplevel widget
        while(pw->parentWidget())
            pw = pw->parentWidget();
        QVERIFY(pw);
        QVERIFY(pw->inherits("QtMaemo6WindowDecoration"));
        m_app->quit();
    }
}

QTEST_APPLESS_MAIN(Ut_QtMaemo6Style)
