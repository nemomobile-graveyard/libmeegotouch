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

#include <mdetailedlistitem.h>
#include <mapplication.h>
#include <mlist.h>
#include <mimagewidget.h>
#include <mapplication.h>

#include "ut_mdetailedlistitem.h"

MApplication *app(NULL);

void Ut_mdetailedlistitem::init()
{
        m_subject = new MDetailedListItem();
}

void Ut_mdetailedlistitem::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_mdetailedlistitem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mdetailedlistitem" };
    app = new MApplication(argc, app_name);
}

void Ut_mdetailedlistitem::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_mdetailedlistitem::testConstructor()
{
    //m_subject was created in init() so verify if it has been created properly:)
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::IconTitleSubtitleAndTwoSideIcons );
    delete m_subject;

    m_subject = new MDetailedListItem( MDetailedListItem::IconTitleSubtitleAndTwoSideIcons );
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::IconTitleSubtitleAndTwoSideIcons );

    delete m_subject;
    m_subject = new MDetailedListItem( MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel );
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel );

    delete m_subject;
    m_subject = new MDetailedListItem( MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons );
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons );

    delete m_subject;
    m_subject = new MDetailedListItem( MDetailedListItem::ThumbnailTitleAndTwoSideIcons );
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::ThumbnailTitleAndTwoSideIcons );
}

void Ut_mdetailedlistitem::testSetItemStyle()
{
    //check if m_subject was created with proper style
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::IconTitleSubtitleAndTwoSideIcons );
    m_subject->setItemStyle(MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel);
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel );
}

void Ut_mdetailedlistitem::testSetIconStyle()
{
    m_subject->setIconStyle(MDetailedListItem::Icon);
    QCOMPARE( m_subject->iconStyle() , MDetailedListItem::Icon);

    m_subject->setIconStyle(MDetailedListItem::Thumbnail);
    QCOMPARE( m_subject->iconStyle() , MDetailedListItem::Thumbnail);
}

void Ut_mdetailedlistitem::testSetImageWidget()
{
    MImageWidget *myImageWidget = new MImageWidget();
    m_subject->setImageWidget( myImageWidget );
    QCOMPARE( m_subject->imageWidget(), myImageWidget );
}

void Ut_mdetailedlistitem::testSetTitle()
{
    QString title("Title");
    m_subject->setTitle( title );
    QCOMPARE( m_subject->title(), title );
}

void Ut_mdetailedlistitem::testSetSubtitle()
{
    QString subtitle("Subtitle");
    m_subject->setSubtitle(subtitle);
    QCOMPARE( m_subject->subtitle(), subtitle );
}

void Ut_mdetailedlistitem::testSetSideBottomTitle()
{
    QString sidebottomtitle("SideBottomTitle");
    m_subject->setSideBottomTitle(sidebottomtitle);
    QCOMPARE( m_subject->sideBottomTitle(), sidebottomtitle );
}

void Ut_mdetailedlistitem::testSideBottomImageWidget()
{
    QCOMPARE(m_subject->sideBottomImageWidget()->objectName(), QString("CommonSubIconBottom"));
}



QTEST_APPLESS_MAIN(Ut_mdetailedlistitem)
