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

#include <QTest>
#include <QSignalSpy>
#include <QObject>
#include <QGraphicsSceneMouseEvent>

#include "mapplication.h"
#include "mbanner.h"
#include "ut_mbanner.h"



MApplication *app;

void Ut_MBanner::init()
{
    m_subject = new MBanner();
    QApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    m_tmp = 0;
}

void Ut_MBanner::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MBanner::initTestCase()
{    
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbanner" };
    app = new MApplication(argc, app_name);
}

void Ut_MBanner::cleanupTestCase()
{
    delete app;
}

void Ut_MBanner::testTitle()
{
    QString title("Title 1");
    m_subject->setTitle(title);
    QCOMPARE(m_subject->title(), title);
}

void Ut_MBanner::testSubTitle()
{
    QString subtitle("Title 1");
    m_subject->setSubtitle(subtitle);
    QCOMPARE(m_subject->subtitle(), subtitle);
}


void Ut_MBanner::testIcon()
{
    QString icon("icon-image");
    m_subject->setIconID(icon);;
    QCOMPARE(m_subject->iconID(), icon);
}

void Ut_MBanner::testTimeStamp()
{
    QDate tempdate;
    tempdate.setDate(2010,10,22);
    QTime temptime;
    temptime.setHMS(20,32,0,0);
    QDateTime time= QDateTime(tempdate,temptime);
    m_subject->setBannerTimeStamp(time);
    QCOMPARE(m_subject->bannerTimeStamp(),time);
}

void Ut_MBanner::testPrefixTimeStamp()
{
    QString tempPrefix= "Yesterday";
    m_subject->setPrefixTimeStamp(tempPrefix);
    QCOMPARE(m_subject->prefixTimeStamp(), tempPrefix);

    tempPrefix= "Two days ago";
    m_subject->setPrefixTimeStamp(tempPrefix);
    QCOMPARE(m_subject->prefixTimeStamp(), tempPrefix);
}

void Ut_MBanner::testUpdatingTimeStamp()
{
    QDateTime time = QDateTime::fromString("01:02:03", "hhmmss");
    m_subject->setBannerTimeStamp(time);
    QCOMPARE(m_subject->bannerTimeStamp(), time);

    time = QDateTime::fromString("02:03:04", "hhmmss");
    m_subject->setBannerTimeStamp(time);
    QCOMPARE(m_subject->bannerTimeStamp(), time);

    time = QDateTime::fromString("05:06:07", "hhmmss");
    m_subject->setBannerTimeStamp(time);
    QCOMPARE(m_subject->bannerTimeStamp(), time);
}

void Ut_MBanner::testBannerCreation()
{
    /* Style configured as Event Banner */

    MBanner *bannerEvent = new MBanner();

    bannerEvent->setTitle("MBanner the new title");
    bannerEvent->setSubtitle("New subtitle");
    bannerEvent->setIconID("icon-1");

    QCOMPARE(bannerEvent->title(),  QString("MBanner the new title"));
    QCOMPARE(bannerEvent->subtitle(),  QString("New subtitle"));
    QCOMPARE(bannerEvent->iconID(),  QString("icon-1"));

    /* Style configured as Information Banner */

    MBanner *bannerInformation = new MBanner();

    bannerInformation->setSubtitle("New subtitle");

    QCOMPARE(bannerInformation->subtitle(),  QString("New subtitle"));

    /* Style configured as System Banner */

    MBanner *bannerSystem = new MBanner();

    bannerSystem->setSubtitle("New subtitle");
    bannerSystem->setIconID("icon-1");

    QCOMPARE(bannerSystem->iconID(),  QString("icon-1"));
    QCOMPARE(bannerSystem->subtitle(),  QString("New subtitle"));

    delete bannerEvent;
    delete bannerInformation;
    delete bannerSystem;
}

void Ut_MBanner::testBannerClicking()
{
    MBanner *banner = new MBanner();

    // click the banner

    QSignalSpy clickedSpy(banner, SIGNAL(clicked()));
    banner->click();
    QCOMPARE(clickedSpy.count(), 1);

    delete banner;
}

void Ut_MBanner::testPixmap()
{
    QPixmap pixmapTest("bannerpixmap.png");
    m_subject->setPixmap(pixmapTest);
    QCOMPARE(pixmapTest, m_subject->pixmap());
}

QTEST_APPLESS_MAIN(Ut_MBanner)
