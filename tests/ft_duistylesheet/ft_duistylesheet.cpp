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

#include "ft_duistylesheet.h"
#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiscenemanager.h"
#include <time.h>
#include <QtTest>
#include "testobjectstyle.h"
#include "testobject2style.h"
#include "testobject3style.h"
#include "duiscalableimage.h"
#include "testwidget.h"
#include "testwidget2.h"
#include "testwidget3.h"

// include this to get theme profiling support
//#include "../../src/theme/duitheme_p.h"


Ft_DuiStyleSheet::Ft_DuiStyleSheet()
{
}

void Ft_DuiStyleSheet::init()
{
    m_subject = new DuiStyleSheet();
}

void Ft_DuiStyleSheet::cleanup()
{
    DuiStyleSheet::cleanup(false);
    delete m_subject;
}

DuiApplication *app;

void Ft_DuiStyleSheet::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ft_css" };
    app = new DuiApplication(argc, app_name);
}

void Ft_DuiStyleSheet::cleanupTestCase()
{
    delete app;
}

void Ft_DuiStyleSheet::test_supported_attribute_types()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);

    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);
    TestObjectStyle *style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "", Dui::Landscape, NULL);

    QFont font("sans");
    font.setPixelSize(10);

    // Check that attributes match to the ones in .css file
    QCOMPARE(style->attributeInteger(), 10);
    QCOMPARE(style->attributeReal(), 2.5);
    QCOMPARE(style->attributeString(), QString("string"));
    QCOMPARE(style->attributeChar(), QChar('x'));
    QCOMPARE(style->attributeBool(), true);
    QCOMPARE(style->attributePoint(), QPoint(10, 10));
    QCOMPARE(style->attributePointF(), QPointF(15.5, 21.5));
    QCOMPARE(style->attributeFont(), font);
    QCOMPARE(style->attributeSize(), QSize(12, 15));
    QCOMPARE(style->attributeSizeF(), QSizeF(22.5, 27.75));
    QCOMPARE(style->attributeColor(), QColor("#ffffff"));
#if QT_VERSION >= 0x040600
//    QEasingCurve curve;
//    curve.setType(QEasingCurve::Linear);
//    curve.setAmplitude(0.5);
//    curve.setOvershoot(1.5);
//    curve.setPeriod(1.0);
//    QCOMPARE(style.attributeEasingCurve(), curve);
#endif
    QCOMPARE(style->attributeAlignment(), Qt::AlignLeft);
    QCOMPARE(style->attributeOrientation(), Qt::Horizontal);
    QCOMPARE(style->attributeUnderlineStyle(), QTextCharFormat::NoUnderline);
    QCOMPARE(style->attributePenStyle(), Qt::NoPen);
    QCOMPARE(style->attributeAxis(), Qt::XAxis);

    QVERIFY(style->attributePixmap());
    QVERIFY(style->attributePixmap2());
    QVERIFY(style->attributePixmap3());
    QVERIFY(style->attributePixmap4());
    QVERIFY(style->attributePixmap() == style->attributePixmap2());
    QVERIFY(style->attributePixmap3() == style->attributePixmap4());
    QCOMPARE(style->attributePixmap()->size(), QSize(30, 30));

    QVERIFY(style->attributeScalable());
    QVERIFY(style->attributeScalable2());
    QVERIFY(style->attributeScalable3());
    QVERIFY(style->attributeScalable4());

    QVERIFY(style->attributeScalable() == style->attributeScalable2());
    QVERIFY(style->attributeScalable3() == style->attributeScalable4());

    int left, right, top, bottom;
    style->attributeScalable()->borders(&left, &right, &top, &bottom);
    QCOMPARE(left, 20);
    QCOMPARE(right, 20);
    QCOMPARE(top, 20);
    QCOMPARE(bottom, 20);

    DuiStyleSheet::releaseStyle(style);
}

void Ft_DuiStyleSheet::test_inheritance()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);

    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);
    TestObject2Style *style2 = (TestObject2Style *) DuiStyleSheet::style(sheets, "TestObject2Style", "", "", "", Dui::Landscape, NULL);

    QFont font("sans");
    font.setPixelSize(10);

    // Check that attributes match to the ones in .css file
    QCOMPARE(style2->attributeInteger(), 5);
    QCOMPARE(style2->attributeInteger2(), 50);
    QCOMPARE(style2->attributeReal(), 2.5);
    QCOMPARE(style2->attributeString(), QString("stringi"));
    QCOMPARE(style2->attributeString2(), QString("string2"));
    QCOMPARE(style2->attributeBool(), true);
    QCOMPARE(style2->attributePoint(), QPoint(10, 10));
    QCOMPARE(style2->attributePointF(), QPointF(15.5, 21.5));
    QCOMPARE(style2->attributeFont(), font);
    QCOMPARE(style2->attributeSize(), QSize(12, 15));
    QCOMPARE(style2->attributeSizeF(), QSizeF(22.5, 27.75));
    QCOMPARE(style2->attributeColor(), QColor("#ffffff"));

    QCOMPARE(style2->attributeAlignment(), Qt::AlignLeft);
    QCOMPARE(style2->attributeOrientation(), Qt::Horizontal);
    QCOMPARE(style2->attributeUnderlineStyle(), QTextCharFormat::NoUnderline);
    QCOMPARE(style2->attributePenStyle(), Qt::NoPen);
    QCOMPARE(style2->attributeAxis(), Qt::XAxis);

    QVERIFY(style2->attributePixmap());
    QCOMPARE(style2->attributePixmap()->size(), QSize(30, 30));

    QVERIFY(style2->attributeScalable());
    int left, right, top, bottom;
    style2->attributeScalable()->borders(&left, &right, &top, &bottom);
    QCOMPARE(left, 20);
    QCOMPARE(right, 20);
    QCOMPARE(top, 20);
    QCOMPARE(bottom, 20);
    DuiStyleSheet::releaseStyle(style2);

    TestObject3Style *style3 = (TestObject3Style *) DuiStyleSheet::style(sheets, "TestObject3Style", "", "", "", Dui::Landscape, NULL);
    QCOMPARE(style3->attributeInteger(), 3);
    QCOMPARE(style3->attributeInteger2(), 50);
    QCOMPARE(style3->attributeString2(), QString("string3"));
    QCOMPARE(style3->attributeBool(), true);
    DuiStyleSheet::releaseStyle(style3);
}

void Ft_DuiStyleSheet::test_objectnames()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);

    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "", Dui::Landscape, NULL);
    QCOMPARE(style->attributeInteger(), 100);
    QCOMPARE(style->attributeReal(), 100.0);
    QCOMPARE(style->attributeString(), QString("specialized string"));
    QCOMPARE(style->attributePixmap(), (const QPixmap*)NULL);
    QCOMPARE(style->attributePoint(), QPoint(0,0));
    DuiStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) DuiStyleSheet::style(sheets, "TestObject2Style", "Specialized", "", "", Dui::Landscape, NULL);
    QCOMPARE(style2->attributeInteger(), 100);
    QCOMPARE(style2->attributeInteger2(), 7);
    QCOMPARE(style2->attributeReal(), 12.0);
    QCOMPARE(style2->attributeString2(), QString("specialized string2"));
    DuiStyleSheet::releaseStyle(style2);
}

void Ft_DuiStyleSheet::test_orientations()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);

    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "", Dui::Portrait, NULL);
    QCOMPARE(style->attributeOrientation(), Qt::Vertical);
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "", Dui::Landscape, NULL);
    QCOMPARE(style->attributeOrientation(), Qt::Horizontal);
    DuiStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) DuiStyleSheet::style(sheets, "TestObject2Style", "", "", "", Dui::Portrait, NULL);
    QCOMPARE(style2->attributeOrientation(), Qt::Vertical);
    DuiStyleSheet::releaseStyle(style2);

    style2 = (TestObject2Style *) DuiStyleSheet::style(sheets, "TestObject2Style", "", "", "", Dui::Landscape, NULL);
    QCOMPARE(style2->attributeOrientation(), Qt::Horizontal);
    DuiStyleSheet::releaseStyle(style2);
}

void Ft_DuiStyleSheet::test_modes()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);

    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "Disabled", "", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#aaaaaa"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "Active", "", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#ff0000"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Active", "", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#00ff00"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Active", "", Dui::Portrait, NULL);
    QCOMPARE(style->attributeColor(), QColor("#ff00ff"));
    QCOMPARE(style->attributeOrientation(), Qt::Vertical);
    QCOMPARE(style->attributeString(), QString("specialiazed portrait active"));
    DuiStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) DuiStyleSheet::style(sheets, "TestObject2Style", "Specialized", "Active", "", Dui::Portrait, NULL);
    QCOMPARE(style2->attributeColor(), QColor("#f0000f"));
    QCOMPARE(style2->attributeOrientation(), Qt::Vertical);
    QCOMPARE(style2->attributeString(), QString("specialiazed portrait active 2"));
    DuiStyleSheet::releaseStyle(style2);
}

void Ft_DuiStyleSheet::test_types()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);
    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "icon", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#aaaaaa"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "fancy", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#ff0000"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "icon", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#00ff00"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "fancy", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#0abba0"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Active", "fancy", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#0f0f0f"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Disabled", "fancy", Dui::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#f0f0f0"));
    DuiStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) DuiStyleSheet::style(sheets, "TestObject2Style", "", "", "fancy", Dui::Landscape, NULL);
    QCOMPARE(style2->attributeColor(), QColor("#0000ff"));
    DuiStyleSheet::releaseStyle(style2);
}

void Ft_DuiStyleSheet::test_parent()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_duistylesheet_testobject.css"), true);
    QList<const DuiStyleSheet *> sheets;
    sheets.append(m_subject);

    TestWidget       w;
    TestWidget3      w2;
    TestWidget2      w3;

    w2.setParentItem(&w);
    w3.setParentItem(&w2);

    TestObjectStyle *style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "", Dui::Landscape, &w);
    QCOMPARE(style->attributeColor(), QColor("#000001"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "", Dui::Landscape, &w2);
    QCOMPARE(style->attributeColor(), QColor("#000010"));
    DuiStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) DuiStyleSheet::style(sheets, "TestObjectStyle", "", "", "", Dui::Landscape, &w3);
    QCOMPARE(style->attributeColor(), QColor("#000020"));
    DuiStyleSheet::releaseStyle(style);
}
/*
void Ft_CSS::test_cache_size()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // get current size of the cache, should be zero
    QCOMPARE(0, DuiThemePrivate::getProfilingInfo().cacheSize);
    qDebug() << "Cache size is:" << DuiThemePrivate::getProfilingInfo().cacheSize;

//    DuiTheme::instance()->prepareThemeChange("green");

    // load css file
    DuiTheme::loadCSS(qApp->applicationDirPath() + "/ft_duistylesheet_test.css");

//    DuiTheme::instance()->changeTheme();

    // should still be zero
    QCOMPARE(0, DuiThemePrivate::getProfilingInfo().cacheSize);

    TestObject testObject;

    // just get something from the style system
    DuiStyle* style = DuiTheme::style(&testObject, m_styleDescription);

    // ensure that it was cached
    int cacheSizeInMemory = DuiThemePrivate::getProfilingInfo().cacheSize;
    qDebug() << "Cache size is:" << DuiThemePrivate::getProfilingInfo().cacheSize;
    QVERIFY(cacheSizeInMemory > 0);

    // get something different
    testObject.setObjectName("Specialized");
    style = DuiTheme::style(&testObject, m_styleDescription);

    // check that the cache is bigger now, it should be
    int cacheSizeInMemory2 = DuiThemePrivate::getProfilingInfo().cacheSize;
    qDebug() << "Cache size is:" << DuiThemePrivate::getProfilingInfo().cacheSize;
    QVERIFY(cacheSizeInMemory2 > cacheSizeInMemory);

    // get same data once again, the cache size should remain same
    style = DuiTheme::style(&testObject, m_styleDescription);
    qDebug() << "Cache size is:" << DuiThemePrivate::getProfilingInfo().cacheSize;
    QVERIFY(cacheSizeInMemory2 == DuiThemePrivate::getProfilingInfo().cacheSize);
}
*/


QTEST_APPLESS_MAIN(Ft_DuiStyleSheet)
