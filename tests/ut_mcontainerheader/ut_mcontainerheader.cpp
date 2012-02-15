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

#include "mapplication.h"
#include "mcontainerheader_p.h"
#include "ut_mcontainerheader.h"


MApplication *app;

void Ut_MContainerHeader::init()
{
    m_subject = new MContainerHeader;
    m_subject->setGeometry(QRectF(0, 0, 200, 100));
}

void Ut_MContainerHeader::cleanup()
{
    delete m_subject;
}

void Ut_MContainerHeader::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mcontainerview" };
    app = new MApplication(argc, argv);
}


void Ut_MContainerHeader::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MContainerHeader::testClickedEvent()
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

void Ut_MContainerHeader::testClickedEventCancelledByLeavingArea()
{
    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(2, 2));
    m_subject->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(2, 200));
    m_subject->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

void Ut_MContainerHeader::testClickedEventCancelledByExceedingDistance()
{
    const MContainerHeaderView* view = dynamic_cast<const MContainerHeaderView*>(m_subject->view());
    const int maxDistanceForClick = view->style()->maxDistanceForClick();

    m_subject->setGeometry(QRectF(0, 0, 200, 100 + maxDistanceForClick));

    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(2, 2));
    m_subject->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent moveEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent.setScenePos(QPointF(2, 3 + maxDistanceForClick));
    m_subject->mouseMoveEvent(&moveEvent);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(2, 3 + maxDistanceForClick));
    m_subject->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

void Ut_MContainerHeader::testClickedEventCancelledByTimeout()
{
    const MContainerHeaderView* view = dynamic_cast<const MContainerHeaderView*>(m_subject->view());

    QSignalSpy clickedSpy(m_subject, SIGNAL(clicked()));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(2, 2));
    m_subject->mousePressEvent(&pressEvent);

    QTest::qSleep(view->style()->pressTimeout());
    QApplication::processEvents(QEventLoop::AllEvents);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(2, 2));
    m_subject->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_MContainerHeader)
