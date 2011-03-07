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
#include <QtTest/QtTest>
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MWindow>
#include <MSceneManager>
#include <MScene>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>
#include <MLabel>
#include <MScene>
#include <MBanner>
#include <MLocale>
#include <MCancelEvent>
#include <MImageWidget>

#include "mbannerview.h"
#include "mbanner.h"
#include "mbannerview_p.h"
#include "mscenewindowview_p.h"
#include "ut_mbannerview.h"

MApplication *app;
MApplicationWindow *win;
MApplicationPage *page;

void Ut_MBannerView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbannerview" };
    app = new MApplication(argc, app_name);
    win = new MApplicationWindow();
    page = new MApplicationPage();

    m_banner = new MBanner();
    m_subject = new MBannerView(m_banner);
    m_banner->setView(m_subject);

    page->setCentralWidget(m_banner);
    page->appear(win);
}

void Ut_MBannerView::cleanupTestCase()
{
    delete page;
    page = NULL;
    delete win;
    win = NULL;
    delete app;
    app = NULL;
}

void Ut_MBannerView::testBannerModelDatainViews()
{
    m_banner->setIconID("icon-l-browser");
    m_banner->setTitle("Hello");
    m_banner->setSubtitle("World");
    m_banner->setPrefixTimeStamp("Latest:");

    QDateTime tempDate (QDateTime::currentDateTime());
    m_banner->setBannerTimeStamp(tempDate);

    QString dateLocaleReady = MLocale().formatDateTime(tempDate, MLocale::DateNone, MLocale::TimeShort);

    QCOMPARE(m_banner->title(), m_subject->d_func()->title()->text());
    QCOMPARE(m_banner->subtitle(), m_subject->d_func()->subtitle()->text());
    QCOMPARE(m_banner->prefixTimeStamp(), m_subject->d_func()->prefixTimeStamp()->text());
    QCOMPARE(dateLocaleReady, m_subject->d_func()->bannerTimeStamp()->text());
    QCOMPARE(m_banner->iconID(), m_subject->d_func()->icon()->imageId());
}

void Ut_MBannerView::testBannerPixmapAndIcon()
{
    const QPixmap pixmap("bannerpixmap.png");
    m_banner->setPixmap(pixmap);
    QCOMPARE(pixmap.toImage(), m_subject->d_func()->pixmap()->pixmap()->toImage());
}

void Ut_MBannerView::testBannerDateFormats()
{
    QDate tempdate;
    tempdate.setDate(2010,10,22);
    QTime temptime;
    temptime.setHMS(20,32,0,0);
    QDateTime time= QDateTime(tempdate,temptime);

    //Date in banner is old it should use DateShort + TimeNone
    m_banner->setBannerTimeStamp(time);
    //Banner should detect that the date is older and it will convert the date
    QString dateLocaleShort = MLocale().formatDateTime(time, MLocale::DateShort, MLocale::TimeNone);
    QCOMPARE(dateLocaleShort, m_subject->d_func()->bannerTimeStamp()->text());
    //Now  we put the date in current date  == banner must change the date format again
    time.setDate(QDate::currentDate());
    time.setTime(QTime::currentTime());
    m_banner->setBannerTimeStamp(time);
    dateLocaleShort = MLocale().formatDateTime(time, MLocale::DateNone, MLocale::TimeShort);
    QCOMPARE(dateLocaleShort, m_subject->d_func()->bannerTimeStamp()->text());
}

void Ut_MBannerView::testBannerMouseEvents()
{
    QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
    QGraphicsSceneMouseEvent releaseEvent(QEvent::GraphicsSceneMouseRelease);
    MCancelEvent event;

    m_subject->mousePressEvent(&pressEvent);
    QVERIFY(m_banner->isDown());
    m_subject->mouseReleaseEvent(&releaseEvent);
    QVERIFY(!m_banner->isDown());

    m_subject->cancelEvent(&event);
}

QTEST_APPLESS_MAIN(Ut_MBannerView)
