/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QGraphicsGridLayout>
#include <QGraphicsItem>

#include <mdetailedlistitem.h>
#include <mlabel.h>
#include <mapplication.h>
#include <mlist.h>
#include <mimagewidget.h>
#include <mapplication.h>

#include "ut_mdetailedlistitem.h"
#include "mdetailedlistitem_p.h"

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
    m_subject->clearLayout();
    m_subject->setItemStyle(MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel);
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::IconTitleSubtitleAndSideIconWithLabel );

    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->imageWidget()) );
    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->titleLabelWidget()) );
    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->subtitleLabelWidget()) );

    m_subject->clearLayout();
    m_subject->setItemStyle(MDetailedListItem::ThumbnailTitleAndTwoSideIcons);
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::ThumbnailTitleAndTwoSideIcons );

    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->imageWidget()) );
    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->titleLabelWidget()) );
    QVERIFY( !findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->subtitleLabelWidget()) );

    m_subject->clearLayout();
    m_subject->setItemStyle(MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons );
    QCOMPARE( m_subject->itemStyle(), MDetailedListItem::ThumbnailTitleSubtitleAndTwoSideIcons );

    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->imageWidget()) );
    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->titleLabelWidget()) );
    QVERIFY( findItemInGridLayout(m_subject->d_ptr->layout(), m_subject->subtitleLabelWidget()) );
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
    m_subject->createLayout();
    MImageWidget *myImageWidget = new MImageWidget();
    m_subject->setImageWidget( myImageWidget );
    QCOMPARE( m_subject->d_ptr->image, myImageWidget );
    QCOMPARE( m_subject->imageWidget(), myImageWidget );

    //now there is an image widget already set, test if it is deleted from layout
    MImageWidget *myImageWidget2 = new MImageWidget();
    m_subject->setImageWidget(myImageWidget2);

    bool foundRemovedInLayout = false;
    bool foundNewInLayout = false;

    for (int i = 0; i < m_subject->d_ptr->layout()->count(); i++)
    {
        if (m_subject->d_ptr->layout()->itemAt(i) == myImageWidget)
        {
            foundRemovedInLayout = true;
        }
        if(m_subject->d_ptr->layout()->itemAt(i) == myImageWidget2)
        {
            foundNewInLayout = true;
        }
    }

    QCOMPARE(foundRemovedInLayout, false);
    QCOMPARE(foundNewInLayout, true);
    QVERIFY(!m_subject->d_ptr->image->objectName().isEmpty());
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

void Ut_mdetailedlistitem::testResizeEvent()
{
    QGraphicsSceneResizeEvent event;
    event.setOldSize(QSizeF(100, 100));

    QSizeF newSize(200, 200);
    event.setNewSize(newSize);
    m_subject->resizeEvent(&event);

    QVERIFY(m_subject->d_ptr->layout());
}

bool Ut_mdetailedlistitem::findItemInGridLayout(QGraphicsGridLayout *layout,
        QGraphicsLayoutItem *item)
{
    bool found = false;
    for(int i = 0; i < layout->rowCount(); i++) {
        for(int j = 0; j < layout->columnCount(); j++) {
            if (layout->itemAt(i, j) == item) {
                found = true;
                break;
            }
        }
    }
    return found;
}

QTEST_APPLESS_MAIN(Ut_mdetailedlistitem)
