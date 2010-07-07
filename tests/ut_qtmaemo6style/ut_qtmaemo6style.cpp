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
#include <QApplication>
#include <QStyle>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QDebug>
#include <QPainter>

#include <qtmaemo6style.h>
#include "mapplication.h"

#include "ut_qtmaemo6style.h"

/*
  NOTE: this test is only included into test package if the Qt Maemo 6 style is
        included to libmeegotouch.
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
    //only test it, if at least there is a plugin, otherwise we asume the
    // style is intentionally not installed
    if(QFile::exists("/usr/lib/qt4/plugins/styles/libmeegotouchqtstyleplugin.so")) {
        static int argc = 1;
        static char *argv[1] = { (char *) "./ut_qtmaemo6style" };
        QApplication app(argc, argv);
        QStyle* style = app.style();
        QVERIFY(style->inherits("QtMaemo6Style"));
    } else {
        qDebug() << "Assuming style plugin not installed attentionally";
    }
}

void Ut_QtMaemo6Style::testLoadOtherStyle() {
    //only test it, if at least there is a plugin, otherwise we asume the
    // style is intentionally not installed
    if(QFile::exists("/usr/lib/qt4/plugins/styles/libmeegotouchqtstyleplugin.so")) {
        static int argc = 3;
        static char *argv[3] = { (char *) "./ut_qtmaemo6style", (char *) "-style", (char *) "windows"};
        QApplication app(argc, argv);
        QStyle* style = app.style();
        QVERIFY(style->inherits("QWindowsStyle"));
    }
}

void Ut_QtMaemo6Style::testWindowDecoration() {
    //only test it, if at least there is a plugin, otherwise we asume the
    // style is intentionally not installed
    if(QFile::exists("/usr/lib/qt4/plugins/styles/libmeegotouchqtstyleplugin.so")) {
        static int argc = 3;
        static char *argv[3] = { (char *) "./ut_qtmaemo6style", (char *) "-style", (char *) "maemo6"};
        m_app = new QApplication(argc, argv);
        QVERIFY(m_app->style()->inherits("QtMaemo6Style"));

        m_mw = new QMainWindow();
        m_mw->show();
        QTimer::singleShot(1000, this, SLOT(testWindowDecorationSlot()));
        //just ensure that the application quits, even if the test fails
        QTimer::singleShot(5000, m_app, SLOT(quit()));
        m_app->exec();

        delete m_mw;
        m_mw = 0;
        delete m_app;
        m_app = 0;
    }
}

void Ut_QtMaemo6Style::testWindowDecorationSlot() {
    //only test it, if at least there is a plugin, otherwise we asume the
    // style is intentionally not installed
    if(QFile::exists("/usr/lib/qt4/plugins/styles/libmeegotouchqtstyleplugin.so")) {
        //this is called by testWindowDecoration(), but also called by the
        //testframework but needs testWindowDecoration() as precondition
        if(m_app && m_mw) {
            QWidget* pw = 0;
            pw = m_mw->parentWidget();
            QVERIFY(pw != 0);
            //search the toplevel widget
            while(pw->parentWidget())
                pw = pw->parentWidget();
            QVERIFY(pw != 0);
            QVERIFY(pw->inherits("QtMaemo6WindowDecoration"));
            m_app->quit();
        }
    }
}

void Ut_QtMaemo6Style::testFonts() {
    //only test it, if at least there is a plugin, otherwise we asume the
    // style is intentionally not installed
    if(QFile::exists("/usr/lib/qt4/plugins/styles/libmeegotouchqtstyleplugin.so")) {
        static int argc = 1;
        static char *argv[1] = { (char *) "./ut_qtmaemo6style" };
        QApplication app(argc, argv);
        Q_UNUSED(app);
        //font settings on a button should be ignored
        // the font of m style is used
        QPushButton button;
        button.setText("Test");
        button.setFont(QFont("Arial"));
        QSize size1 = button.sizeHint();

        QPixmap pix1(size1);
        pix1.fill(Qt::white);
        QPainter p1(&pix1);
        button.render(&p1);

        button.setFont(QFont("Helvetica"));
        QSize size2 = button.sizeHint();
        //changing the font shouldn't effect the size
        QCOMPARE(size1, size2);

        QPixmap pix2(size2);
        pix2.fill(Qt::white);
        QPainter p2(&pix2);
        button.render(&p2);
        QCOMPARE(pix1.toImage(), pix2.toImage());
    }
}

QTEST_APPLESS_MAIN(Ut_QtMaemo6Style)
