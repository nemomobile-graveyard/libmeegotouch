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

#include <MLabel>
#include <MImageWidget>
#include "mapplication.h"
#include "minfobanner.h"
#include "minfobannereventview.h"
#include "minfobannereventview_p.h"
#include "ut_minfobannerview.h"

MApplication *app;

void Ut_MInfoBannerView::init()
{
    m_infoBanner = new MInfoBanner;
    m_subject = new MInfoBannerEventView(m_infoBanner);
    m_infoBanner->setView(m_subject);
}

void Ut_MInfoBannerView::cleanup()
{
    delete m_infoBanner;
}

void Ut_MInfoBannerView::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_minfobannerview" };
    app = new MApplication(argc, argv);
}


void Ut_MInfoBannerView::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MInfoBannerView::testBodyText()
{
    QString bodyText("testing");
    m_infoBanner->setBodyText(bodyText);
    QCOMPARE(m_infoBanner->bodyText(), bodyText);
    QCOMPARE(m_subject->d_func()->label->text(), bodyText);
}

void Ut_MInfoBannerView::testImageId()
{
    QString imageID("icon-l-contacts");
    m_infoBanner->setImageID(imageID);
    QCOMPARE(m_infoBanner->imageID(), imageID);
    QCOMPARE(m_subject->d_func()->image->imageId(), imageID);
}

void Ut_MInfoBannerView::testIconId()
{
    QString iconID("icon-l-contacts");
    m_infoBanner->setIconID(iconID);
    QCOMPARE(m_infoBanner->iconID(), iconID);
    QCOMPARE(m_subject->d_func()->icon->imageId(), iconID);
}

void Ut_MInfoBannerView::testClickedEvent()
{
    m_infoBanner->setGeometry(QRectF(0, 0, 200, 100));
    QSignalSpy clickedSpy(m_infoBanner, SIGNAL(clicked()));
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(10, 10));
    m_subject->mousePressEvent(&pressEvent);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(10, 10));
    m_subject->mouseReleaseEvent(&releaseEvent);
    QCOMPARE(clickedSpy.count(), 1);
}

void Ut_MInfoBannerView::testClickedEventCancelledByLeavingArea()
{
    m_infoBanner->setGeometry(QRectF(0, 0, 200, 100));
    QSignalSpy clickedSpy(m_infoBanner, SIGNAL(clicked()));

    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    pressEvent.setScenePos(QPointF(10, 10));
    m_subject->mousePressEvent(&pressEvent);

    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    releaseEvent.setScenePos(QPointF(100, 200));
    m_subject->mouseReleaseEvent(&releaseEvent);

    QCOMPARE(clickedSpy.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_MInfoBannerView)
