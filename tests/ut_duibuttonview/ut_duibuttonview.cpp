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

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include <duibuttonview.h>
#include <duibuttoniconview.h>
#include <views/duibuttonview_p.h>

#include <duibutton.h>
#include <duibutton_p.h>
#include <duibuttonstyle.h>
#include "ut_duibuttonview.h"
#include "duiapplication.h"

#include "duicancelevent.h"

DuiApplication *app;

void Ut_DuiButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duibuttonview" };
    app = new DuiApplication(argc, app_name);

    m_button = new DuiButton();
    m_subject = new DuiButtonIconView(m_button);
    m_button->setView(m_subject);
}

void Ut_DuiButtonView::cleanupTestCase()
{
    delete m_button;
    m_button = 0;

    delete app;
}

void Ut_DuiButtonView::testDrawText_data()
{
    QTest::addColumn<QFont>("font");
    QTest::addColumn<QColor>("textColor");
    QTest::addColumn<int>("horizontalTextAlign");
    QTest::addColumn<int>("verticalTextAlign");
    QTest::addColumn<int>("textMarginLeft");
    QTest::addColumn<int>("textMarginRight");
    QTest::addColumn<int>("textMarginTop");
    QTest::addColumn<int>("textMarginBottom");

    QTest::newRow("testDrawText1") << QFont() << QColor(255, 255, 255) << (int)Qt::AlignLeft << (int)Qt::AlignTop << 0 << 0 << 0 << 0;
    //QTest::newRow("testDrawText2") << QFont() << QColor(255,255,255) << Qt::AlignLeft << Qt::AlignTop << 0 << 0 << 0 << 0;
    //QTest::newRow("testDrawText3") << QFont() << QColor(255,255,255) << Qt::AlignLeft << Qt::AlignTop << 0 << 0 << 0 << 0;
    //QTest::newRow("testDrawText4") << QFont() << QColor(255,255,255) << Qt::AlignLeft << Qt::AlignTop << 0 << 0 << 0 << 0;
}

void Ut_DuiButtonView::testDrawText()
{
    QFETCH(QFont, font);
    QFETCH(QColor, textColor);
    QFETCH(int, horizontalTextAlign);
    QFETCH(int, verticalTextAlign);
    QFETCH(int, textMarginLeft);
    QFETCH(int, textMarginRight);
    QFETCH(int, textMarginTop);
    QFETCH(int, textMarginBottom);


    m_button->setTextVisible(true);
    m_button->setIconVisible(false);
    m_button->setText("test1");

    QRectF iconRect, textRect;

    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    int buttonWidth = m_button->size().width();
    int buttonHeight = m_button->size().height();

    DuiButtonStyle *s = (DuiButtonStyle *)m_subject->style().operator->();

    s->setFont(font);
    s->setTextColor(textColor);
    s->setHorizontalTextAlign((Qt::Alignment)horizontalTextAlign);
    s->setVerticalTextAlign((Qt::Alignment)verticalTextAlign);
    s->setTextMarginLeft(textMarginLeft);
    s->setTextMarginRight(textMarginRight);
    s->setTextMarginTop(textMarginTop);
    s->setTextMarginBottom(textMarginBottom);
    m_subject->applyStyle();

    QPixmap pm(QSize(buttonWidth, buttonHeight));
    QPainter p(&pm);
    m_subject->drawContents(&p, NULL);
}

void Ut_DuiButtonView::testDrawIcon_data()
{
    QTest::addColumn<int>("iconAlign");
    QTest::addColumn<QString>("icon");
    QTest::addColumn<QString>("toggledIcon");
    QTest::addColumn<QSize>("iconSize");
    QTest::addColumn<bool>("checkable");


    QTest::newRow("testDrawIcon1") << (int)Qt::AlignLeft << "Icon-close" << "Icon-close" << QSize(16, 16) << false;
    QTest::newRow("testDrawIcon2") << (int)Qt::AlignRight << "Icon-close" << "Icon-close" << QSize(32, 32) << true;
    QTest::newRow("testDrawIcon3") << (int)Qt::AlignBottom << "Icon-close" << "Icon-close" << QSize(64, 64) << false;
    QTest::newRow("testDrawIcon4") << (int)Qt::AlignTop << "Icon-close" << "Icon-close" << QSize(128, 128) << true;
}

void Ut_DuiButtonView::testDrawIcon()
{
    QFETCH(int, iconAlign);
    QFETCH(QString, icon);
    QFETCH(QString, toggledIcon);
    QFETCH(QSize, iconSize);
    QFETCH(bool, checkable);

    m_button->setTextVisible(false);
    m_button->setIconVisible(true);
    m_button->setCheckable(checkable);
    m_button->setIconID(icon);
    m_button->setToggledIconID(toggledIcon);

    QRectF iconRect, textRect;

    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    int buttonWidth = m_button->size().width();
    int buttonHeight = m_button->size().height();

    DuiButtonStyle *s = (DuiButtonStyle *)m_subject->style().operator->();

    s->setIconAlign((Qt::Alignment)iconAlign);
    s->setIconSize(iconSize);
    s->setPreferredSize(QSize(-1, -1));
    m_subject->applyStyle();

    QPixmap pm(QSize(buttonWidth, buttonHeight));
    QPainter p(&pm);
    m_subject->drawContents(&p, NULL);

    m_button->model()->setDown(true);
    m_subject->drawContents(&p, NULL);
    m_button->model()->setDown(false);
}

void Ut_DuiButtonView::testDrawIconText_data()
{
    QTest::addColumn<int>("iconAlign");
    QTest::addColumn<QString>("icon");
    QTest::addColumn<QString>("toggledIcon");
    QTest::addColumn<QSize>("iconSize");

    QTest::newRow("testDrawIcon1") << (int)Qt::AlignLeft << "Icon-close" << "Icon-close" << QSize(16, 16);
    QTest::newRow("testDrawIcon2") << (int)Qt::AlignRight << "Icon-close" << "Icon-close" << QSize(32, 32);
    QTest::newRow("testDrawIcon3") << (int)Qt::AlignBottom << "Icon-close" << "Icon-close" << QSize(64, 64);
    QTest::newRow("testDrawIcon4") << (int)Qt::AlignTop << "Icon-close" << "Icon-close" << QSize(128, 128);
}

void Ut_DuiButtonView::testDrawIconText()
{
    QFETCH(int, iconAlign);
    QFETCH(QString, icon);
    QFETCH(QString, toggledIcon);
    QFETCH(QSize, iconSize);

    m_button->setTextVisible(true);
    m_button->setIconVisible(true);
    m_button->setIconID(icon);
    m_button->setToggledIconID(toggledIcon);
    m_button->setText("button");

    QRectF iconRect, textRect;

    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    int buttonWidth = m_button->size().width();
    int buttonHeight = m_button->size().height();

    DuiButtonStyle *s = (DuiButtonStyle *)m_subject->style().operator->();

    s->setIconAlign((Qt::Alignment)iconAlign);
    s->setIconSize(iconSize);
    s->setPreferredSize(QSize(-1, -1));
    m_subject->applyStyle();

    QPixmap pm(QSize(buttonWidth, buttonHeight));
    QPainter p(&pm);
    m_button->paint(&p, NULL, NULL);

    m_button->model()->setDown(true);
    m_button->paint(&p, NULL, NULL);
    m_button->model()->setDown(false);
}

void Ut_DuiButtonView::testMouseEvents()
{
    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    //press
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    m_subject->mousePressEvent(&pressEvent);
    QVERIFY(m_button->isDown() == true);
    m_subject->mousePressEvent(&pressEvent);

    //release
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    m_subject->mouseReleaseEvent(&releaseEvent);
    QVERIFY(m_button->isDown() == false);
    m_subject->mouseReleaseEvent(&releaseEvent);

    //cancel
    m_subject->mousePressEvent(&pressEvent);
    DuiCancelEvent event;
    m_subject->cancelEvent(&event);
    QVERIFY(m_button->isDown() == false);
    m_subject->cancelEvent(&event);
}

void Ut_DuiButtonView::testSwitchView()
{
    m_button->setCheckable(true);
    m_button->setChecked(false);
    m_button->setViewType(DuiButton::switchType);
    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    //draw unchecked
    QImage img0 = captureImage(m_button);

    //draw checked
    m_button->setChecked(true);
    QImage img1 = captureImage(m_button);

    //draw checked down
    m_button->setDown(true);
    QImage img2 = captureImage(m_button);

    //draw down
    m_button->setChecked(false);
    QImage img3 = captureImage(m_button);

    m_button->setDown(false);

    /*QVERIFY(img0 != img1);
    QVERIFY(img0 != img2);
    QVERIFY(img0 == img3);

    QVERIFY(img1 == img2);
    QVERIFY(img1 != img3);

    QVERIFY(img2 != img3);*/
}

void Ut_DuiButtonView::testCheckboxView()
{
    m_button->setCheckable(true);
    m_button->setChecked(false);
    m_button->setViewType(DuiButton::checkboxType);
    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    //draw unchecked
    QImage img0 = captureImage(m_button);

    //draw checked
    m_button->setChecked(true);
    QImage img1 = captureImage(m_button);

    //draw checked down
    m_button->setDown(true);
    QImage img2 = captureImage(m_button);

    //draw down
    m_button->setChecked(false);
    QImage img3 = captureImage(m_button);

    m_button->setDown(false);

    /*QVERIFY(img0 != img1);
    QVERIFY(img0 != img2);
    QVERIFY(img0 != img3);

    QVERIFY(img1 != img2);
    QVERIFY(img1 != img3);

    QVERIFY(img2 != img3);*/
}

QImage Ut_DuiButtonView::captureImage(DuiButton *button)
{
    QPixmap pixmap(button->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    button->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

QTEST_APPLESS_MAIN(Ut_DuiButtonView)
