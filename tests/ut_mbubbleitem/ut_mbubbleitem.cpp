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
#include <QPointer>
#include <QGraphicsSceneMouseEvent>
#include <mbubbleitem.h>
#include <mimagewidget.h>
#include <mwidgetview.h>
#include <mwidgetmodel.h>
#include "mbubbleitem_p.h"
#include "ut_mbubbleitem.h"
#include "mapplication.h"

void Ut_MBubbleItem::init()
{
    m_bubble = new MBubbleItem();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
}

void Ut_MBubbleItem::cleanup()
{
    delete m_bubble;
    m_bubble = 0;
}
MApplication *app;

void Ut_MBubbleItem::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbubbleitem" };
    app = new MApplication(argc, app_name);
}

void Ut_MBubbleItem::cleanupTestCase()
{
    delete app;
}


void Ut_MBubbleItem::testSenderName()
{
    QSizeF size = m_bubble->preferredSize();
    m_bubble->setSenderName("John Joe");
    QCOMPARE(m_bubble->senderName(), QString("John Joe"));
    m_bubble->setSenderName("");
    QCOMPARE(m_bubble->preferredSize(), size);
    QCOMPARE(m_bubble->senderName(), QString(""));
    QString arabicName = QString::fromUtf8("ﻊﺑﺩ ﺎﻠﻠﻫ");
    m_bubble->setSenderName(arabicName); //Test with a right-to-left unicode arabic name
    QCOMPARE(m_bubble->senderName(), arabicName);
    m_bubble->setSenderName(":-) :-/"); //Test we do not do anything funny with emoticons and <>
    QCOMPARE(m_bubble->senderName(), QString(":-) :-/"));
    size = m_bubble->preferredSize();
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
}

void Ut_MBubbleItem::testTimeStamp()
{
    QSizeF size = m_bubble->preferredSize();
    m_bubble->setTimeStamp("3:23 pm");
    QCOMPARE(m_bubble->timeStamp(), QString("3:23 pm"));
    m_bubble->setTimeStamp("");
    QCOMPARE(m_bubble->timeStamp(), QString(""));
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
}

void Ut_MBubbleItem::testMessage()
{
    QSizeF size = m_bubble->preferredSize();
    m_bubble->setMessage("Hello there");
    QCOMPARE(m_bubble->message(), QString("Hello there"));
    m_bubble->setMessage("");
    QCOMPARE(m_bubble->message(), QString(""));
    m_bubble->setMessage("Hello <b>there</b> :-) <img src='hi'> "); //make sure we don't mangle rich text and smileys
    QCOMPARE(m_bubble->message(), QString("Hello <b>there</b> :-) <img src='hi'> "));
    m_bubble->setMessage("");
    QCOMPARE(m_bubble->message(), QString(""));
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
    QVERIFY(m_bubble->informationWidgets().isEmpty());
}

void Ut_MBubbleItem::testCommentsString()
{
    QSizeF size = m_bubble->preferredSize();
    m_bubble->setCommentsString("Hello there");
    QCOMPARE(m_bubble->commentsString(), QString("Hello there"));
    m_bubble->setCommentsString("Hello <b>there</b> :-) <img src='hi'> "); //make sure we don't mangle rich text and smileys
    QCOMPARE(m_bubble->commentsString(), QString("Hello <b>there</b> :-) <img src='hi'> "));
    m_bubble->setCommentsString("");
    QCOMPARE(m_bubble->commentsString(), QString(""));
    QVERIFY(m_bubble->informationWidgets().isEmpty());
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
    QVERIFY(m_bubble->informationWidgets().isEmpty());
}

void Ut_MBubbleItem::testThumbsUpString()
{
    QSizeF size = m_bubble->preferredSize();
    m_bubble->setThumbsUpString("Hello there");
    QCOMPARE(m_bubble->thumbsUpString(), QString("Hello there"));
    m_bubble->setThumbsUpString("Hello <b>there</b> :-) <img src='hi'> "); //make sure we don't mangle rich text and smileys
    QCOMPARE(m_bubble->thumbsUpString(), QString("Hello <b>there</b> :-) <img src='hi'> "));
    m_bubble->setThumbsUpString("");
    QCOMPARE(m_bubble->thumbsUpString(), QString(""));
    QVERIFY(m_bubble->informationWidgets().isEmpty());
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
    QVERIFY(m_bubble->informationWidgets().isEmpty());
}

void Ut_MBubbleItem::testMessageType()
{
    QSizeF size = m_bubble->preferredSize();
    QCOMPARE(m_bubble->messageType(), MBubbleItem::Incoming); //Default
    m_bubble->setMessageType(MBubbleItem::Incoming);
    QCOMPARE(m_bubble->messageType(), MBubbleItem::Incoming);
    m_bubble->setMessageType(MBubbleItem::Outgoing);
    QCOMPARE(m_bubble->messageType(), MBubbleItem::Outgoing);
    m_bubble->setMessageType(MBubbleItem::Incoming);
    QCOMPARE(m_bubble->messageType(), MBubbleItem::Incoming);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
}

void Ut_MBubbleItem::testAvatar()
{
    QSizeF size = m_bubble->preferredSize();
    QPointer<MImageWidget> avatar1 = new MImageWidget;
    QPointer<MImageWidget> avatar2 = new MImageWidget;
    QPixmap pixmap("icon-m-home");
    QVERIFY(!m_bubble->avatar());
    m_bubble->setAvatar(avatar1);
    QVERIFY(m_bubble->avatar() == avatar1);
    QVERIFY(avatar1);
    QVERIFY(avatar1->parentWidget() == m_bubble);
    QVERIFY(!avatar1->parent());
    m_bubble->setAvatar(avatar1);
    QVERIFY(avatar1);
    QVERIFY(m_bubble->avatar() == avatar1);
    m_bubble->setAvatar(NULL);
    QVERIFY(!m_bubble->avatar());
    QVERIFY(avatar1);
    QVERIFY(!avatar1->parent());
    m_bubble->setAvatar(avatar1);
    QVERIFY(m_bubble->avatar() == avatar1);
    avatar1->setParent(m_bubble); //Give it memory ownership
    avatar2->setParent(m_bubble); //Give it memory ownership
    m_bubble->setAvatar(avatar2);
    QVERIFY(m_bubble->avatar() == avatar2);
    QVERIFY(avatar2->parent() == m_bubble);
    m_bubble->setAvatar(NULL);
    QVERIFY(!m_bubble->avatar());
    m_bubble->setAvatar( pixmap );
    QCOMPARE(m_bubble->avatar()->pixmap()->toImage(), pixmap.toImage());
    m_bubble->setAvatar(NULL);
    QVERIFY(!m_bubble->avatar());
    //Both should be deleted now
    QVERIFY(!avatar1);
    QVERIFY(!avatar2);
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
}
void Ut_MBubbleItem::testCentralWidget()
{
    QSizeF size = m_bubble->preferredSize();
    QPointer<QGraphicsWidget> centralWidget = new QGraphicsWidget;
    QVERIFY(m_bubble->centralWidget() == NULL);
    m_bubble->setCentralWidget(centralWidget);
    QVERIFY(m_bubble->centralWidget() == centralWidget);
//    QCOMPARE(centralWidget->parentWidget(), m_bubble);
    QVERIFY(!centralWidget->parent());
    centralWidget->setParent(m_bubble);
    m_bubble->setCentralWidget(NULL);
    QVERIFY(!centralWidget);
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
}
void Ut_MBubbleItem::testInformationWidgets()
{
    QSizeF size = m_bubble->preferredSize();
    QVERIFY(m_bubble->informationWidgets().isEmpty());
    QPointer<QGraphicsWidget> informationWidget1 = new QGraphicsWidget;
    QPointer<QGraphicsWidget> informationWidget2 = new QGraphicsWidget;
    m_bubble->addInformationWidget(informationWidget1);
    QCOMPARE(m_bubble->informationWidgets().size(), 1);
    QVERIFY(m_bubble->informationWidgets().at(0) == informationWidget1);
    m_bubble->addInformationWidget(informationWidget2);
    QCOMPARE(m_bubble->informationWidgets().size(), 2);
    QVERIFY(m_bubble->informationWidgets().at(0) == informationWidget1);
    QVERIFY(m_bubble->informationWidgets().at(1) == informationWidget2);
    m_bubble->removeInformationWidget(informationWidget1);
    QCOMPARE(m_bubble->informationWidgets().size(), 1);
    QVERIFY(m_bubble->informationWidgets().at(0) == informationWidget2);
    m_bubble->removeInformationWidget(informationWidget2);
    QVERIFY(m_bubble->informationWidgets().isEmpty());
    QCOMPARE(m_bubble->preferredSize(), size);
    m_bubble->updateGeometry();
    QCOMPARE(m_bubble->preferredSize(), size);
    
    QVERIFY(informationWidget1);
    QVERIFY(informationWidget2);
    delete informationWidget1;
    delete informationWidget2;
}

void Ut_MBubbleItem::testForMemoryLeaks()
{
    QSizeF size = m_bubble->preferredSize();
    MBubbleItem *bubble = new MBubbleItem();

    QPointer<MWidgetView> view = const_cast<MWidgetView*>(bubble->view());
    QPointer<MWidgetModel> model = bubble->model();
    
    bubble->setCommentsString("Hello");
    bubble->setThumbsUpString("Hello there");
    
    QList< QPointer<QGraphicsWidget> > widgets;
    foreach( QGraphicsWidget *widget, bubble->informationWidgets() ) {
        widgets << widget;
    }

    delete bubble;

    //The created widgets should now have been deleted
    foreach( const QPointer<QGraphicsWidget> &widget, widgets ) {
        QVERIFY(!widget);
    }
    //Make sure the view and model have been deleted as well
    QVERIFY(!view);
    QVERIFY(!model);
    
}
QTEST_APPLESS_MAIN(Ut_MBubbleItem)
