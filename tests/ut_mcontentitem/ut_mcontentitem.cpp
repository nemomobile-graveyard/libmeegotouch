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
#include <QGraphicsSceneMouseEvent>
#include <mapplication.h>
#include <qpixmap.h>
#include <mbutton.h>
#include <mlabel.h>

#include <mcontentitem.h>

#include "ut_mcontentitem.h"

MApplication *app(NULL);

void Ut_MContentItem::init()
{
    m_subject = new MContentItem();
}

void Ut_MContentItem::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MContentItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mcontentitem" };
    app = new MApplication(argc, app_name);
}

void Ut_MContentItem::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MContentItem::testSetPixmap_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testSetPixmap()
{
    QFETCH(QString, fname);

    const QPixmap pixmap(fname);
    m_subject->setPixmap( pixmap );

    QCOMPARE( m_subject->pixmap().toImage(), pixmap.toImage() );
}

void Ut_MContentItem::testSetImage_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testSetImage()
{
    QFETCH(QString, fname);
    const QImage image(fname);
    m_subject->setImage( image );

    QCOMPARE( m_subject->image(), image );
    QCOMPARE( m_subject->model()->itemImage(), image );
}

void Ut_MContentItem::testSetImageID()
{
    QString iconId ("icon-m-home");
    m_subject->setImageID(iconId);
    QCOMPARE( m_subject->imageID(), iconId );
}

void Ut_MContentItem::testSetOptionalPixmap_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testSetOptionalPixmap()
{
    QFETCH(QString, fname);

    const QPixmap pixmap(fname);
    m_subject->setOptionalPixmap( pixmap );

    QCOMPARE( m_subject->optionalPixmap().toImage(), pixmap.toImage() );
}

void Ut_MContentItem::testSetOptionalImage_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testSetOptionalImage()
{
    QFETCH(QString, fname);
    const QImage image(fname);
    m_subject->setOptionalImage( image );

    QCOMPARE( m_subject->optionalImage(), image );
}

void Ut_MContentItem::testSetOptionalImageID()
{
    QString iconId ("icon-m-home");
    m_subject->setOptionalImageID(iconId);
    QCOMPARE( m_subject->optionalImageID(), iconId );
}

void Ut_MContentItem::testSetSmallItem()
{
    //testing with MButton usage, since it is deriving from MWidget
    MButton *smallItem = new MButton("testing smallItem");
    m_subject->setSmallItem(smallItem);
    QCOMPARE( m_subject->smallItem(), smallItem );
}

void Ut_MContentItem::testSetSmallText()
{
    QString smallText("smallText");
    m_subject->setSmallText(smallText);

    MWidget *smallItem = m_subject->smallItem();
    MLabel *smallLabel = static_cast<MLabel*>(smallItem);
    QCOMPARE( smallLabel->text(), smallText );
}

void Ut_MContentItem::testAdditionalItem()
{
    //testing with MButton usage, since it is deriving from MWidget
    MButton *additionalItem = new MButton("testing additionalItem");
    m_subject->setAdditionalItem(additionalItem);
    QCOMPARE( m_subject->additionalItem(), additionalItem );
}

void Ut_MContentItem::testModelSetItemPixmap_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testModelSetItemPixmap()
{
    QFETCH(QString, fname);

    const QPixmap pixmap(fname);
    m_subject->model()->setItemPixmap(pixmap);
    QCOMPARE( m_subject->model()->itemPixmap().toImage(), pixmap.toImage());
}

void Ut_MContentItem::testModelSetOptionalPixmap_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testModelSetOptionalPixmap()
{
    QFETCH(QString, fname);

    const QPixmap pixmap(fname);
    m_subject->model()->setOptionalPixmap( pixmap );
    QCOMPARE( m_subject->model()->optionalPixmap().toImage(), pixmap.toImage());
}

void Ut_MContentItem::testModelSetOptionalImage_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testModelSetOptionalImage()
{
    QFETCH(QString, fname);
    const QImage image(fname);
    m_subject->setOptionalImage( image );

    QCOMPARE( m_subject->model()->optionalImage(), image );
}

void Ut_MContentItem::testModelSetItemQImage_data()
{
    QTest::addColumn<QString>("fname");
    QTest::newRow("png") << qApp->applicationDirPath() + "/ut_mcontentitem_test.png";
}

void Ut_MContentItem::testModelSetItemQImage()
{
    QFETCH(QString, fname);
    const QImage image(fname);
    m_subject->model()->setItemQImage( image );

    QCOMPARE( m_subject->model()->itemQImage(), image );
}

void Ut_MContentItem::testTitle()
{
    QString title ("Title");
    m_subject->setTitle( title );
    QCOMPARE( m_subject->title(), title );
}

void Ut_MContentItem::testSubtitle()
{
    QString subtitle("Subitle");
    m_subject->enableProgressBar(); //setting additional item
    m_subject->setSubtitle( subtitle );
    QCOMPARE( m_subject->subtitle(), subtitle );
}

void Ut_MContentItem::testItemMode()
{
    m_subject->setItemMode(MContentItem::TopLeft);
    QCOMPARE( m_subject->itemMode(), MContentItem::TopLeft );
}

void Ut_MContentItem::testProgressBar()
{
    QVERIFY( m_subject->additionalItem() == NULL );
    m_subject->enableProgressBar();
    QVERIFY( m_subject->additionalItem() != NULL );
}


QTEST_APPLESS_MAIN(Ut_MContentItem)
