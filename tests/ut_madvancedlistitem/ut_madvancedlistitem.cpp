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

#include <madvancedlistitem.h>
#include <mapplication.h>
#include <mimagewidget.h>
#include <mprogressindicator.h>

#include "ut_madvancedlistitem.h"

MApplication *app(NULL);

void Ut_MAdvancedListItem::init()
{
    m_subject = new MAdvancedListItem();
}

void Ut_MAdvancedListItem::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MAdvancedListItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_madvancedlistitem" };
    app = new MApplication(argc, app_name);
}

void Ut_MAdvancedListItem::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MAdvancedListItem::testConstructors()
{
    //m_subject was created in init() so verify if it has been created properly:)
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons );
    delete m_subject;

    m_subject = new MAdvancedListItem(MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon );
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon );
    //no need to delete m_subject here, because it will be deleted in cleanup()
}

void Ut_MAdvancedListItem::testTitle()
{
    QString title ("Title");
    m_subject->setTitle( title );
    QCOMPARE( m_subject->title(), title );
}

void Ut_MAdvancedListItem::testImageWidget()
{
    MImageWidget *myImageWidget = new MImageWidget();
    m_subject->setImageWidget( myImageWidget );
    QCOMPARE( m_subject->imageWidget(), myImageWidget );
}

void Ut_MAdvancedListItem::testSideTopImage()
{
    QCOMPARE(m_subject->sideTopImageWidget()->objectName(), QString("CommonSubIconTop"));
}

void Ut_MAdvancedListItem::testSideBottomImage()
{
    QCOMPARE(m_subject->sideBottomImageWidget()->objectName(), QString("CommonSubIconBottom"));
}

void Ut_MAdvancedListItem::testProgressIndicator()
{
    QCOMPARE(m_subject->progressIndicator()->objectName(), QString("CommonProgressBar"));
    QCOMPARE(m_subject->progressIndicator()->viewType(), MProgressIndicator::barType);
}

void Ut_MAdvancedListItem::testItemStyle()
{
    QCOMPARE( m_subject->itemStyle(), MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons );
    m_subject->setItemStyle(MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon);
    QCOMPARE( m_subject->itemStyle(), MAdvancedListItem::IconWithTitleProgressIndicatorAndTopSideIcon );
    m_subject->setItemStyle(MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons);
    QCOMPARE( m_subject->itemStyle(), MAdvancedListItem::IconWithTitleProgressIndicatorAndTwoSideIcons );
}

QTEST_APPLESS_MAIN(Ut_MAdvancedListItem)
