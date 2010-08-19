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

#include <mbasiclistitem.h>
#include <mapplication.h>
#include <mimagewidget.h>

#include "ut_mbasiclistitem.h"

MApplication *app(NULL);

void Ut_MBasicListItem::init()
{
    m_subject = new MBasicListItem();
}

void Ut_MBasicListItem::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MBasicListItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbasiclistitem" };
    app = new MApplication(argc, app_name);
}

void Ut_MBasicListItem::cleanupTestCase()
{
    delete app;
    app = NULL;
}


void Ut_MBasicListItem::testConstructor()
{
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::SingleTitle );
    delete m_subject;

    m_subject = new MBasicListItem(MBasicListItem::IconWithTitleAndSubtitle);
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::IconWithTitleAndSubtitle );
    delete m_subject;

    m_subject = new MBasicListItem(MBasicListItem::IconWithTitle);
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::IconWithTitle );
    delete m_subject;

    m_subject = new MBasicListItem(MBasicListItem::IconWithTitleAndSubtitle);
    QVERIFY( m_subject != NULL );
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::IconWithTitleAndSubtitle );
}

void Ut_MBasicListItem::testTitle()
{
    QString title("title");
    m_subject->setTitle( title );
    QCOMPARE(m_subject->title(), title );
}

void Ut_MBasicListItem::testSubtitle()
{
    QString subtitle("subtitle");
    m_subject->setSubtitle( subtitle );
    QCOMPARE(m_subject->subtitle(), subtitle );
}

void Ut_MBasicListItem::testItemStyle()
{
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::SingleTitle );
    m_subject->setItemStyle(MBasicListItem::IconWithTitleAndSubtitle);
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::IconWithTitleAndSubtitle );
    m_subject->setItemStyle(MBasicListItem::TitleWithSubtitle);
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::TitleWithSubtitle );
    m_subject->setItemStyle(MBasicListItem::IconWithTitle);
    QCOMPARE( m_subject->itemStyle(), MBasicListItem::IconWithTitle );
}

void Ut_MBasicListItem::testImageWidget()
{
    MImageWidget *myImageWidget = new MImageWidget();
    m_subject->setImageWidget( myImageWidget );
    QCOMPARE( m_subject->imageWidget(), myImageWidget );
}

QTEST_APPLESS_MAIN(Ut_MBasicListItem)
