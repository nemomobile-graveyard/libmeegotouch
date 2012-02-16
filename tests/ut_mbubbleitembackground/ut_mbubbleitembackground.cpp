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

#include <QTest>
#include <QSignalSpy>

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <MCancelEvent>

#include "mapplication.h"
#include "mbubbleitembackground_p.h"
#include "ut_mbubbleitembackground.h"


MApplication *app;

void Ut_MBubbleItemBackground::init()
{
    m_subject = new MBubbleItemBackground;
    m_subject->setGeometry(QRectF(0, 0, 200, 100));
}

void Ut_MBubbleItemBackground::cleanup()
{
    delete m_subject;
}

void Ut_MBubbleItemBackground::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mbubbleitembackground" };
    app = new MApplication(argc, argv);
}


void Ut_MBubbleItemBackground::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MBubbleItemBackground::testClick()
{
    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));
    m_subject->click();
    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_MBubbleItemBackground::testMessageType()
{
    QSignalSpy messageTypeChangedSpy(m_subject, SIGNAL(messageTypeChanged()));
    m_subject->setMessageType(MBubbleItem::Incoming);
    QCOMPARE(m_subject->messageType(), MBubbleItem::Incoming);
    m_subject->setMessageType(MBubbleItem::Outgoing);
    QCOMPARE(m_subject->messageType(), MBubbleItem::Outgoing);
    QCOMPARE(messageTypeChangedSpy.count(), 2);
}

void Ut_MBubbleItemBackground::testClickedEvent()
{
    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(2, 2));
    m_subject->mousePressEvent(&pressEvent);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(2, 2));
    m_subject->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_MBubbleItemBackground::testMouseReleaseEvent()
{
    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(2, 2));
    m_subject->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 0);
}

void Ut_MBubbleItemBackground::testCancelEvent()
{
    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(2, 2));
    m_subject->mousePressEvent(&pressEvent);

    MCancelEvent cancelEvent;
    m_subject->cancelEvent(&cancelEvent);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(2, 2));
    m_subject->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 0);
}

void Ut_MBubbleItemBackground::testLayoutChange()
{
    MBubbleItemBackgroundView* view = const_cast<MBubbleItemBackgroundView*>(dynamic_cast<const MBubbleItemBackgroundView*>(m_subject->view()));

    QString initialObjectName = view->style().objectName();
    QString mirroredObjectName = view->style()->mirroredObjectName();

    if (initialObjectName == mirroredObjectName)
        mirroredObjectName = "";

    QEvent layoutDirectionChangeEvent(QEvent::LayoutDirectionChange);
    view->event(&layoutDirectionChangeEvent);

    QCOMPARE(view->style().objectName(), mirroredObjectName);

    view->event(&layoutDirectionChangeEvent);

    QCOMPARE(view->style().objectName(), initialObjectName);
}

QTEST_APPLESS_MAIN(Ut_MBubbleItemBackground)
