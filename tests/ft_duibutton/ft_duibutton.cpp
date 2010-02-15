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

#include "ft_duibutton.h"
#include "duitheme.h"
#include <QtTest>
#include <QImage>
#include "duitheme.h"
#include "duibutton.h"
#include "duibuttonview.h"
#include "duiapplication.h"





Ft_DuiButton::Ft_DuiButton()
{
}

void Ft_DuiButton::initTestCase()
{
    int argc = 1;
    char *argv = (char *) "./ft_duibutton";
    app = new DuiApplication(argc, &argv);
    qDebug() << "loadCSS " <<  DuiTheme::loadCSS("ft_duibutton.css");
}


void Ft_DuiButton::cleanupTestCase()
{
    delete app;
}

void Ft_DuiButton::buttonPaint()
{
    // Allocate button, rename, resize and check.
    DuiButton *b = new DuiButton();
    setObjectName("ft_test_button");
    b->resize(125, 85);
    QVERIFY(b->size() == QSizeF(125, 85));

    // Allocate two pixmaps.
    QPixmap *p = new QPixmap(500, 500);
    p->fill(QColor(0, 0, 0, 0));
    QPixmap *orig = new QPixmap(500, 500);
    orig->fill(QColor(0, 0, 0, 0));

    // Allocate painter, convert pixmaps to image and check are they the same.
    QPainter *painter = new QPainter(p);
    b->paint(painter, 0, 0);
    QImage pi = p->toImage();
    QImage origi = orig->toImage();
    QVERIFY(pi != origi);

    for (int x = 125; x < 500; x++) {
        for (int y = 0; y < 500; y++) {
            QVERIFY(pi.pixel(x, y) == origi.pixel(x, y));
        }
    }

    for (int y = 85; y < 500; y++) {
        for (int x = 0; x < 500; x++) {
            QVERIFY(pi.pixel(x, y) == origi.pixel(x, y));
        }
    }

    // Clean.
    delete orig;
    delete b;
    delete p;
}

void Ft_DuiButton::buttonSetObjectName()
{
    // Allocate button, resize and check.
    DuiButton *b1 = new DuiButton();
    b1->resize(125, 85);
    QVERIFY(b1->size() == QSizeF(125, 85));

    // Allocate button, rename, resize and check.
    DuiButton *b2 = new DuiButton();
    b2->setObjectName("button2");
    QVERIFY(b2->objectName() == "button2");
    b2->resize(125, 85);
    QVERIFY(b2->size() == QSizeF(125, 85));

    // Allocate two pixmaps.
    QPixmap *p1 = new QPixmap(125, 85);
    p1->fill(QColor(0, 0, 0, 0));
    QPixmap *p2 = new QPixmap(125, 85);
    p2->fill(QColor(0, 0, 0, 0));

    // Init painters and paint.
    QPainter painter1(p1);
    QPainter painter2(p2);
    b1->paint(&painter1, 0, 0);
    b2->paint(&painter2, 0, 0);

    // Convert pixmaps to QT image. Check that images are not the same.
    QImage i1 = p1->toImage();
    QImage i2 = p2->toImage();
    QVERIFY(i1 != i2);

    // Clean.
    delete b1;
    delete b2;
}


void Ft_DuiButton::buttonSetValues()
{
    // Set some values and check that they are ok.
    DuiButton b;
    b.setIconID(QString("test_button_id"));
    QVERIFY(b.iconID() == QString("test_button_id"));

    b.setToggledIconID(QString("test_button_toggled_id"));
    QVERIFY(b.toggledIconID() == QString("test_button_toggled_id"));

    b.setText(QString("test_button_text"));
    QVERIFY(b.text() == QString("test_button_text"));

    b.setTextVisible(true);
    QVERIFY(b.isTextVisible() == true);
    b.setTextVisible(false);
    QVERIFY(b.isTextVisible() == false);

    b.setIconVisible(true);
    QVERIFY(b.isIconVisible() == true);
    b.setIconVisible(false);
    QVERIFY(b.isIconVisible() == false);

    b.setCheckable(true);
    QVERIFY(b.isCheckable() == true);
    b.setCheckable(false);
    QVERIFY(b.isCheckable() == false);

    b.setState(DuiButtonModel::Pressed);
    QVERIFY(b.state() == DuiButtonModel::Pressed);
    b.setState(DuiButtonModel::Released);
    QVERIFY(b.state() == DuiButtonModel::Released);

    b.setCheckable(true);
    b.setChecked(true);
    QVERIFY(b.isChecked() == true);
    b.setChecked(false);
    QVERIFY(b.isChecked() == false);
}


void Ft_DuiButton::testSignalClicked()
{
    DuiButton b;
    const int CLICK_COUNT = 50;
    b.setCheckable(false);
    b.setChecked(false);
    QSignalSpy spy(&b, SIGNAL(clicked(bool)));
    for (int i = 0; i < CLICK_COUNT; i++)
        b.click();
    QVERIFY(spy.count() == CLICK_COUNT);
}


void Ft_DuiButton::testSignalToggled()
{
    DuiButton b;
    const int TOGGLE_COUNT = 55;
    b.setCheckable(true);
    b.setChecked(false);
    QSignalSpy spy(&b, SIGNAL(toggled(bool)));
    for (int i = 0; i < TOGGLE_COUNT; i++)
        b.toggle();
    QVERIFY(spy.count() == TOGGLE_COUNT);
}

void Ft_DuiButton::testSignalPressed()
{
    DuiButton b;
    const int PRESS_COUNT = 60;
    QSignalSpy spy(&b, SIGNAL(pressed()));
    for (int i = 0; i < PRESS_COUNT; i++) {
        b.setState(DuiButtonModel::Released);
        b.setState(DuiButtonModel::Pressed);
    }
    QVERIFY(spy.count() == PRESS_COUNT);
}

void Ft_DuiButton::testSignalReleased()
{
    DuiButton b;
    const int RELEASE_COUNT = 65;
    QSignalSpy spy(&b, SIGNAL(released()));
    for (int i = 0; i < RELEASE_COUNT; i++) {
        b.setState(DuiButtonModel::Pressed);
        b.setState(DuiButtonModel::Released);
    }
    QVERIFY(spy.count() == RELEASE_COUNT);
}



// Horizontal box edge percent no more in css style sheet. Implemented in scalable image.
//
// void Ft_DuiButton::buttonBoxed()
// {
//  // Allocate and name a new button.
//     DuiButton* b1 = new DuiButton();
//     b1->setObjectName("buttonNotBoxed");

//  // Resize and check.
//     qDebug() << b1->effectiveSizeHint(Qt::MaximumSize);
//     b1->resize(150, 95);
//     QVERIFY(b1->size() == QSizeF(150, 95));

//  // Allocate and name a new button.
//     DuiButton* b2 = new DuiButton();
//     b2->setObjectName("buttonBoxed");

//  // Resize and check.
//     b2->resize(150, 95);
//     QVERIFY(b2->size() == QSizeF(150, 95));

//  // Allocate two pixmaps.
//     QPixmap* p1 = new QPixmap(150, 95);
//     p1->fill(QColor(0, 0, 0, 0));
//     QPixmap* p2 = new QPixmap(150, 95);
//     p2->fill(QColor(0, 0, 0, 0));

//  // Init painters and paint.
//     QPainter painter1(p1);
//     QPainter painter2(p2);
//     b1->paint(&painter1, 0, 0);
//     b2->paint(&painter2, 0, 0);

//  // Convert pixmaps to QT image. Check that images are not the same.
//     QImage i1 = p1->toImage();
//     QImage i2 = p2->toImage();
//     QVERIFY(i1 != i2);

//  // Check that pixels on one vertical line are the same.
//     for(int y = 0; y < 95; y++) {
//      QVERIFY(i1.pixel(75, y) == i2.pixel(75, y));
//  }

//  // Clean.
//     delete b1;
//     delete b2;
//  delete p1;
//  delete p2;
// }


QTEST_APPLESS_MAIN(Ft_DuiButton)


