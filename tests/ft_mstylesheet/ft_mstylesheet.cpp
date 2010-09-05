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

#include "ft_mstylesheet.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscenemanager.h"
#include <time.h>
#include <QtTest>
#include "testobjectstyle.h"
#include "testobject2style.h"
#include "testobject3style.h"
#include "mscalableimage.h"
#include "testwidget.h"
#include "testwidget2.h"
#include "testwidget3.h"

// include this to get theme profiling support
//#include "../../src/corelib/theme/mtheme_p.h"

const MStyle *MTheme::style(const char *styleClassName,
                                const QString &objectName,
                                const QString &mode,
                                const QString &type,
                                M::Orientation orientation,
                                const MWidgetController *parent)
{
    MStyleSheet sheet;
    sheet.load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css");
    QList<const MStyleSheet *> sheets;
    sheets.append(&sheet);
    return MStyleSheet::style(sheets, styleClassName, objectName, mode, type, orientation, parent);
}


Ft_MStyleSheet::Ft_MStyleSheet()
  : m_subject(NULL)
{
}

void Ft_MStyleSheet::init()
{
    m_subject = new MStyleSheet();
}

void Ft_MStyleSheet::cleanup()
{
    MStyleSheet::cleanup(false);
    delete m_subject;
}

MApplication *app;

void Ft_MStyleSheet::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ft_css" };
    app = new MApplication(argc, app_name);
}

void Ft_MStyleSheet::cleanupTestCase()
{
    delete app;
}

void Ft_MStyleSheet::test_supported_attribute_types()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);

    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);
    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, NULL);

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

    MStyleSheet::releaseStyle(style);
}

void Ft_MStyleSheet::test_inheritance()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);

    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);
    TestObject2Style *style2 = (TestObject2Style *) MStyleSheet::style(sheets, "TestObject2Style", "", "", "", M::Landscape, NULL);

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

    QVERIFY(style2->attributeScalable());
    int left, right, top, bottom;
    style2->attributeScalable()->borders(&left, &right, &top, &bottom);
    QCOMPARE(left, 20);
    QCOMPARE(right, 20);
    QCOMPARE(top, 20);
    QCOMPARE(bottom, 20);
    MStyleSheet::releaseStyle(style2);

    TestObject3Style *style3 = (TestObject3Style *) MStyleSheet::style(sheets, "TestObject3Style", "", "", "", M::Landscape, NULL);
    QCOMPARE(style3->attributeInteger(), 3);
    QCOMPARE(style3->attributeInteger2(), 50);
    QCOMPARE(style3->attributeString2(), QString("string3"));
    QCOMPARE(style3->attributeBool(), true);
    MStyleSheet::releaseStyle(style3);
}

void Ft_MStyleSheet::test_objectnames()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);

    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "", M::Landscape, NULL);
    QCOMPARE(style->attributeInteger(), 100);
    QCOMPARE(style->attributeReal(), 100.0);
    QCOMPARE(style->attributeString(), QString("specialized string"));
    QCOMPARE(style->attributePixmap(), (const QPixmap*)NULL);
    QCOMPARE(style->attributeColor(), QColor());
    MStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) MStyleSheet::style(sheets, "TestObject2Style", "Specialized", "", "", M::Landscape, NULL);
    QCOMPARE(style2->attributeInteger(), 100);
    QCOMPARE(style2->attributeInteger2(), 7);
    QCOMPARE(style2->attributeReal(), 12.0);
    QCOMPARE(style2->attributeString2(), QString("specialized string2"));
    MStyleSheet::releaseStyle(style2);
}

void Ft_MStyleSheet::test_orientations()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);

    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Portrait, NULL);
    QCOMPARE(style->attributeOrientation(), Qt::Vertical);
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, NULL);
    QCOMPARE(style->attributeOrientation(), Qt::Horizontal);
    MStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) MStyleSheet::style(sheets, "TestObject2Style", "", "", "", M::Portrait, NULL);
    QCOMPARE(style2->attributeOrientation(), Qt::Vertical);
    MStyleSheet::releaseStyle(style2);

    style2 = (TestObject2Style *) MStyleSheet::style(sheets, "TestObject2Style", "", "", "", M::Landscape, NULL);
    QCOMPARE(style2->attributeOrientation(), Qt::Horizontal);
    MStyleSheet::releaseStyle(style2);
}

void Ft_MStyleSheet::test_modes()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);

    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "Disabled", "", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#aaaaaa"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "Active", "", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#ff0000"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Active", "", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#00ff00"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Active", "", M::Portrait, NULL);
    QCOMPARE(style->attributeColor(), QColor("#ff00ff"));
    QCOMPARE(style->attributeOrientation(), Qt::Vertical);
    QCOMPARE(style->attributeString(), QString("specialiazed portrait active"));
    MStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) MStyleSheet::style(sheets, "TestObject2Style", "Specialized", "Active", "", M::Portrait, NULL);
    QCOMPARE(style2->attributeColor(), QColor("#f0000f"));
    QCOMPARE(style2->attributeOrientation(), Qt::Vertical);
    QCOMPARE(style2->attributeString(), QString("specialiazed portrait active 2"));
    MStyleSheet::releaseStyle(style2);

    TestObject3StyleContainer styleContainer;
    styleContainer.initialize("","",NULL);
    QCOMPARE(styleContainer->attributeInteger(), 3);
    QCOMPARE(styleContainer->attributeInteger2(), 50);
    QCOMPARE(styleContainer->attributeString2(), QString("string3"));
    QCOMPARE(styleContainer->attributeBool(), true);
    styleContainer.setModeSomeMode();
    QCOMPARE(styleContainer->attributeInteger(), 163);
}

void Ft_MStyleSheet::test_types()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);
    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "icon", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#aaaaaa"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "fancy", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#ff0000"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "icon", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#00ff00"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "fancy", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#0abba0"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Active", "fancy", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#0f0f0f"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "Disabled", "fancy", M::Landscape, NULL);
    QCOMPARE(style->attributeColor(), QColor("#f0f0f0"));
    MStyleSheet::releaseStyle(style);

    TestObject2Style *style2 = (TestObject2Style *) MStyleSheet::style(sheets, "TestObject2Style", "", "", "fancy", M::Landscape, NULL);
    QCOMPARE(style2->attributeColor(), QColor("#0000ff"));
    MStyleSheet::releaseStyle(style2);
}

void Ft_MStyleSheet::test_parent()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);
    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);

    TestWidget       w;
    TestWidget3      w2;
    TestWidget2      w3;

    w2.setParentItem(&w);
    w3.setParentItem(&w2);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, &w);
    QCOMPARE(style->attributeColor(), QColor("#000001"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, &w2);
    QCOMPARE(style->attributeColor(), QColor("#000010"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, &w3);
    QCOMPARE(style->attributeColor(), QColor("#000020"));
    MStyleSheet::releaseStyle(style);
}

void Ft_MStyleSheet::test_parent_stylenames()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // Open test file
    QCOMPARE(m_subject->load(qApp->applicationDirPath() + "/ft_mstylesheet_testobject.css"), true);
    QList<const MStyleSheet *> sheets;
    sheets.append(m_subject);

    TestWidget       w;
    TestWidget3      w2;
    TestWidget2      w3;

    w.setStyleName("Specialized");
    w2.setParentItem(&w);
    w2.setStyleName("Specialized");
    w3.setParentItem(&w2);

    TestObjectStyle *style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, &w);
    QCOMPARE(style->attributeColor(), QColor("#000100"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "Specialized", "", "", M::Landscape, &w2);
    QCOMPARE(style->attributeColor(), QColor("#000110"));
    MStyleSheet::releaseStyle(style);

    style = (TestObjectStyle *) MStyleSheet::style(sheets, "TestObjectStyle", "", "", "", M::Landscape, &w3);
    QCOMPARE(style->attributeColor(), QColor("#000020"));
    MStyleSheet::releaseStyle(style);
}

/*
void Ft_CSS::test_cache_size()
{
    m_subject->setBinaryFileGenerationEnabled(false);

    // get current size of the cache, should be zero
    QCOMPARE(0, MThemePrivate::getProfilingInfo().cacheSize);
    qDebug() << "Cache size is:" << MThemePrivate::getProfilingInfo().cacheSize;

//    MTheme::instance()->prepareThemeChange("green");

    // load css file
    MTheme::loadCSS(qApp->applicationDirPath() + "/ft_mstylesheet_test.css");

//    MTheme::instance()->changeTheme();

    // should still be zero
    QCOMPARE(0, MThemePrivate::getProfilingInfo().cacheSize);

    TestObject testObject;

    // just get something from the style system
    MStyle* style = MTheme::style(&testObject, m_styleDescription);

    // ensure that it was cached
    int cacheSizeInMemory = MThemePrivate::getProfilingInfo().cacheSize;
    qDebug() << "Cache size is:" << MThemePrivate::getProfilingInfo().cacheSize;
    QVERIFY(cacheSizeInMemory > 0);

    // get something different
    testObject.setObjectName("Specialized");
    style = MTheme::style(&testObject, m_styleDescription);

    // check that the cache is bigger now, it should be
    int cacheSizeInMemory2 = MThemePrivate::getProfilingInfo().cacheSize;
    qDebug() << "Cache size is:" << MThemePrivate::getProfilingInfo().cacheSize;
    QVERIFY(cacheSizeInMemory2 > cacheSizeInMemory);

    // get same data once again, the cache size should remain same
    style = MTheme::style(&testObject, m_styleDescription);
    qDebug() << "Cache size is:" << MThemePrivate::getProfilingInfo().cacheSize;
    QVERIFY(cacheSizeInMemory2 == MThemePrivate::getProfilingInfo().cacheSize);
}
*/


QTEST_APPLESS_MAIN(Ft_MStyleSheet)
