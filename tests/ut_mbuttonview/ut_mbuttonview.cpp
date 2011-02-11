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

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include <mbuttonview.h>
#include <mbuttoniconview.h>
#include <views/mbuttonview_p.h>

#include <mbutton.h>
#include <mbutton_p.h>
#include <mbuttonstyle.h>
#include "ut_mbuttonview.h"
#include "mapplication.h"

#include "mcancelevent.h"

MApplication *app;

void Ut_MButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbuttonview" };
    app = new MApplication(argc, app_name);
}

void Ut_MButtonView::cleanupTestCase()
{
    delete app;
}

void Ut_MButtonView::init()
{
    m_button = new MButton();
    m_subject = new MButtonIconView(m_button);
    m_button->setView(m_subject);
}

void Ut_MButtonView::cleanup()
{
    delete m_button;
}

void Ut_MButtonView::testDrawText_data()
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

void Ut_MButtonView::testDrawText()
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

    MButtonStyle *s = (MButtonStyle *)m_subject->style().operator->();

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

void Ut_MButtonView::testDrawIcon_data()
{
    QTest::addColumn<int>("iconAlign");
    QTest::addColumn<QString>("icon");
    QTest::addColumn<QString>("toggledIcon");
    QTest::addColumn<QSize>("iconSize");
    QTest::addColumn<bool>("checkable");


    QTest::newRow("testDrawIcon1") << (int)Qt::AlignLeft << "icon-m-framework-close" << "icon-m-framework-close" << QSize(16, 16) << false;
    QTest::newRow("testDrawIcon2") << (int)Qt::AlignRight << "icon-m-framework-close" << "icon-m-framework-close" << QSize(32, 32) << true;
    QTest::newRow("testDrawIcon3") << (int)Qt::AlignBottom << "icon-m-framework-close" << "icon-m-framework-close" << QSize(64, 64) << false;
    QTest::newRow("testDrawIcon4") << (int)Qt::AlignTop << "icon-m-framework-close" << "icon-m-framework-close" << QSize(128, 128) << true;
}

void Ut_MButtonView::testDrawIcon()
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

    MButtonStyle *s = (MButtonStyle *)m_subject->style().operator->();

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

void Ut_MButtonView::testDrawIconText_data()
{
    QTest::addColumn<int>("iconAlign");
    QTest::addColumn<QString>("icon");
    QTest::addColumn<QString>("toggledIcon");
    QTest::addColumn<QSize>("iconSize");

    QTest::newRow("testDrawIcon1") << (int)Qt::AlignLeft << "icon-m-framework-close" << "icon-m-framework-close" << QSize(16, 16);
    QTest::newRow("testDrawIcon2") << (int)Qt::AlignRight << "icon-m-framework-close" << "icon-m-framework-close" << QSize(32, 32);
    QTest::newRow("testDrawIcon3") << (int)Qt::AlignBottom << "icon-m-framework-close" << "icon-m-framework-close" << QSize(64, 64);
    QTest::newRow("testDrawIcon4") << (int)Qt::AlignTop << "icon-m-framework-close" << "icon-m-framework-close" << QSize(128, 128);
}

void Ut_MButtonView::testDrawIconText()
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

    MButtonStyle *s = (MButtonStyle *)m_subject->style().operator->();

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

void Ut_MButtonView::testMouseEvents()
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
    MCancelEvent event;
    m_subject->cancelEvent(&event);
    QVERIFY(m_button->isDown() == false);
    m_subject->cancelEvent(&event);

    //move
    m_subject->mousePressEvent(&pressEvent);
   
    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setScenePos(QPointF(300,300));
    
    m_subject->mouseMoveEvent(&moveEvent);
    QVERIFY(m_button->isDown() == false);
    m_subject->mouseReleaseEvent(&releaseEvent);
}

void Ut_MButtonView::testSwitchView()
{
    m_button->setCheckable(true);
    m_button->setChecked(false);
    m_button->setViewType(MButton::switchType);
    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    //draw unchecked
    QImage img0 = captureImage(m_button);

    //draw checked
    m_button->setChecked(true);
    QImage img1= captureImage(m_button);

    //disabled
    m_button->setChecked(false);
    m_button->setEnabled(false);
    QImage img2= captureImage(m_button);

    QVERIFY(img0 != img1);
    QVERIFY(img0 != img2);
    
    QVERIFY(img1 != img2);
}

void Ut_MButtonView::testCheckboxView()
{
    m_button->setCheckable(true);
    m_button->setChecked(false);
    m_button->setViewType(MButton::checkboxType);
    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    //draw unchecked
    QImage img0 = captureImage(m_button);

    //draw checked
    m_button->setChecked(true);
    QImage img1 = captureImage(m_button);

    //disabled
    m_button->setChecked(false);
    m_button->setEnabled(false);
    QImage img2 = captureImage(m_button);

    QVERIFY(img0 != img1);
    QVERIFY(img0 != img2);

    QVERIFY(img1 != img2);
}

void Ut_MButtonView::testSizeHint()
{
    m_button->setText("H");
    QSizeF s1 = m_subject->sizeHint(Qt::PreferredSize);
    m_button->setText("<u></u>H");
    QSizeF s2 = m_subject->sizeHint(Qt::PreferredSize);

    QCOMPARE(s1.width(), s2.width());
    QCOMPARE(s1.height(), s2.height());
}

void Ut_MButtonView::testDisabledSelected()
{
    m_button->setCheckable(true);
    m_button->setChecked(false);
    m_button->setViewType(MButton::checkboxType);
    QRectF rect = QRectF(0, 0, 200, 100);
    m_button->setGeometry(rect);

    //draw disabled selected
    m_button->setChecked(true);
    m_button->setEnabled(false);
    QImage img0 = captureImage(m_button);

    //draw disabled
    m_button->setEnabled(true);
    m_button->setChecked(false);
    m_button->setEnabled(false);
    QImage img1 = captureImage(m_button);

    QVERIFY(img0 != img1);
}

QImage Ut_MButtonView::captureImage(MButton *button)
{
    QPixmap pixmap(button->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    button->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

QTEST_APPLESS_MAIN(Ut_MButtonView)
