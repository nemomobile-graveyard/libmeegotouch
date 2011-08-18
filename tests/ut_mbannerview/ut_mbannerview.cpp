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
#include <MLinearLayoutPolicy>
#include <MLayout>

#include "mbannerview.h"
#include "mbanner.h"
#include "mbannerview_p.h"
#include "mscenewindowview_p.h"
#include "ut_mbannerview.h"
#include "mviewconstants.h"

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

void Ut_MBannerView::testInstantiateBannerView()
{
    MBanner *banner = new MBanner();
    MBannerViewPrivate *bannerViewPrivate = new MBannerViewPrivate();
    MBannerView *bannerView = new MBannerView(*bannerViewPrivate, banner);
    banner->setView(bannerView);

    delete banner;
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
    const QPixmap pixmap("ut_mbannerview_pixmap.png");
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
    //See NB#246264
    QString dateLocaleShort = QString("%1 %2 %3").arg(MLocale().formatDateTime(time.toLocalTime(), MLocale::DateShort, MLocale::TimeNone))
            .arg(m_subject->style()->timestampSeparator())
            .arg(MLocale().formatDateTime(time.toLocalTime(), MLocale::DateNone, MLocale::TimeShort));

    QCOMPARE(dateLocaleShort, m_subject->d_func()->bannerTimeStamp()->text());
    //Now  we put the date in current date  == banner must change the date format again
    time.setDate(QDate::currentDate());
    time.setTime(QTime::currentTime());
    m_banner->setBannerTimeStamp(time);
    dateLocaleShort = MLocale().formatDateTime(time, MLocale::DateNone, MLocale::TimeShort);
    QCOMPARE(dateLocaleShort, m_subject->d_func()->bannerTimeStamp()->text());
}

// The model contains now this information
// Title: Hello
// Subtitle: World
// Prefixtimemstamp: Latest
// Timestamp: 20:32 22-10-2010

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

void Ut_MBannerView::testMouseMoveEvent()
{
    QGraphicsSceneMouseEvent *event = new QGraphicsSceneMouseEvent(
                QGraphicsSceneMouseEvent::MouseMove);

    MBannerViewPrivate *d = m_subject->d_ptr;
    QVERIFY2(d, "Private implementation is NULL");

    event->setScenePos(QPointF(d->controller->sceneBoundingRect().x() + 1,
                               d->controller->sceneBoundingRect().y() + 1));

    m_subject->mouseMoveEvent(event);
    QVERIFY2(m_subject->model()->down(), "Model supposed to be down!");

    event->setScenePos(QPointF(
                           d->controller->sceneBoundingRect().x() - M_RELEASE_MISS_DELTA - 1,
                           d->controller->sceneBoundingRect().y() - M_RELEASE_MISS_DELTA - 1));

    m_subject->mouseMoveEvent(event);
    QVERIFY2(!m_subject->model()->down(), "Model supposed not to be down!");
}

void Ut_MBannerView::testDrawForeground()
{
    m_subject->drawForeground(NULL, NULL);
}

void Ut_MBannerView::testDrawContents()
{
    m_subject->drawContents(NULL, NULL);
}


void Ut_MBannerView::testBannerStyles()
{
    //When we change the stylename the different layouts
    //must change and re-organize the elements
    //according with the stylename set.

    m_banner->setStyleName(MBannerType::ShortEventBanner);
    testPrivateLayoutShortEventBanner();
    m_banner->setStyleName(MBannerType::InformationBanner);
    testPrivateLayoutInformationBanner();
    m_banner->setStyleName(MBannerType::SystemBanner);
    testPrivateLayoutSystemBanner();
    m_banner->setStyleName(MBannerType::FullEventBanner);
    testPrivateLayoutFullEventBanner();
    m_banner->setStyleName(MBannerType::LockScreenEventBanner);
    testPrivateLayoutLockScreenEventBanner();
    m_banner->setStyleName(MBannerType::PrivateEventBanner);
    testPrivateLayoutPrivateEventBanner();
}

void Ut_MBannerView::testPrivateManageOpacities()
{
    m_subject->d_ptr->isDownOpacityEnabled = true;

    m_subject->d_ptr->manageOpacities();

    m_subject->model()->setDown(true);

    m_subject->d_ptr->manageOpacities();
}

void Ut_MBannerView::testPrivateLayoutShortEventBanner()
{
    QVERIFY2(m_subject->d_ptr->layout, "NULL returned");

    testLayoutSetup();

    testPixmapOrIconLayout();
    testTitleLabel();
    testSubtitleLabel();

}

void Ut_MBannerView::testPrivateLayoutInformationBanner()
{
    QVERIFY2(m_subject->d_ptr->layout, "NULL returned");

    testLayoutSetup();

    testTitleLabel();
}

void Ut_MBannerView::testPrivateLayoutSystemBanner()
{
    QVERIFY2(m_subject->d_ptr->layout, "NULL returned");

    testLayoutSetup();

    testPixmapOrIconLayout();
    testTitleLabel();
}

void Ut_MBannerView::testPrivateLayoutFullEventBanner()
{
    QVERIFY2(m_subject->d_ptr->gridBanner, "NULL returned");

    testGridItemsExist();

    testLayoutTimeStamp();
}

void Ut_MBannerView::testPrivateLayoutLockScreenEventBanner()
{
    QVERIFY2(m_subject->d_ptr->gridBanner, "NULL returned");

    testGridItemsExist();

    testLayoutTimeStamp();
}


void Ut_MBannerView::testPrivateLayoutPrivateEventBanner()
{
    QVERIFY2(m_subject->d_ptr->controller->layout(), "layout not set in controller");
    QVERIFY2(m_subject->d_ptr->gridBanner, "NULL returned");

    testLayoutTimeStamp();
}

void Ut_MBannerView::testLayoutSetup()
{
    QVERIFY2(m_subject->d_ptr->controller->layout(), "layout not set in controller");
    QVERIFY2(m_subject->d_ptr->landscapePolicy->layout(), "Landscape policy layout not set");
    QVERIFY2(m_subject->d_ptr->portraitPolicy->layout(), "Portrait policy layout not set");
}

void Ut_MBannerView::testGridItemsExist()
{
    testPixmapOrIconGrid();

    QVERIFY2(findItemInGridLayout(m_subject->d_ptr->gridBanner, m_subject->d_ptr->title()), "Title not found");

    QVERIFY2(findItemInGridLayout(m_subject->d_ptr->gridBanner, m_subject->d_ptr->subtitle()), "Subtitle not found");

}

void Ut_MBannerView::testTitleLabel()
{
    QCOMPARE(m_subject->model()->title().isEmpty(),
             -1 == m_subject->d_ptr->landscapePolicy->indexOf(m_subject->d_ptr->title()));

    QCOMPARE(m_subject->model()->title().isEmpty(),
             -1 == m_subject->d_ptr->portraitPolicy->indexOf(m_subject->d_ptr->title()));
}

void Ut_MBannerView::testSubtitleLabel()
{
    QCOMPARE(m_subject->model()->subtitle().isEmpty(),
             -1 == m_subject->d_ptr->landscapePolicy->indexOf(m_subject->d_ptr->subtitle()));

    QCOMPARE(m_subject->model()->subtitle().isEmpty(),
             -1 == m_subject->d_ptr->portraitPolicy->indexOf(m_subject->d_ptr->subtitle()));
}

void Ut_MBannerView::testPixmapOrIconGrid()
{

    QGraphicsLayoutItem *expected = NULL;

    if (!m_subject->model()->pixmap().isNull() && m_subject->model()->iconID().isEmpty()) {
        expected = m_subject->d_ptr->pixmap();
    } else {
        expected = m_subject->d_ptr->icon();
    }

    QVERIFY2(findItemInGridLayout(m_subject->d_ptr->gridBanner, expected), "Pixmap or icon not found");
}

void Ut_MBannerView::testPixmapOrIconLayout()
{
    if (!m_subject->model()->iconID().isEmpty() && m_subject->model()->pixmap().isNull()) {
        QCOMPARE(m_subject->d_ptr->layout->itemAt(0), m_subject->d_ptr->icon());
    } else if (!m_subject->model()->pixmap().isNull()){
        QCOMPARE(m_subject->d_ptr->layout->itemAt(0), m_subject->d_ptr->pixmap());
    }
}

bool Ut_MBannerView::findItemInGridLayout(QGraphicsGridLayout *layout, QGraphicsLayoutItem *item)
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

void Ut_MBannerView::testLayoutTimeStamp()
{
    if (!m_subject->model()->prefixTimeStamp().isEmpty() && m_subject->model()->bannerTimeStamp().isValid()) {
        QGraphicsLinearLayout *layoutStamp = NULL;
        if ((m_subject->model()->subtitle().isEmpty() && !m_subject->d_ptr->subtitle()->isVisible() )
                || m_banner->styleName() == MBannerType::PrivateEventBanner) {
            layoutStamp = dynamic_cast<QGraphicsLinearLayout*>(m_subject->d_ptr->gridBanner->itemAt(1, 1));

        } else {
            layoutStamp = dynamic_cast<QGraphicsLinearLayout*>(m_subject->d_ptr->gridBanner->itemAt(2, 1));
        }

        QVERIFY2(layoutStamp, "Layout stamp not found");
        QCOMPARE(layoutStamp->count(), 2);
        QCOMPARE(layoutStamp->itemAt(0), m_subject->d_ptr->prefixTimeStamp());
        QCOMPARE(layoutStamp->itemAt(1), m_subject->d_ptr->bannerTimeStamp());
    }
}

void Ut_MBannerView::testSettingTimestampDateTimeFormatWhenTimestampForBannerIsSet()
{
    QDateTime testDateTime = QDateTime::currentDateTime();
    m_banner->setBannerTimeStamp(testDateTime);

    QString currentTimeString = MLocale().formatDateTime(testDateTime, MLocale::DateNone, MLocale::TimeShort);

    QCOMPARE(currentTimeString, m_subject->d_func()->bannerTimeStamp()->text());

    QDateTime oldDateTime = testDateTime.addDays(-1);
    m_banner->setBannerTimeStamp(oldDateTime);

    QString oldTimeString = QString("%1 %2 %3").arg(MLocale().formatDateTime(oldDateTime.toLocalTime(), MLocale::DateShort, MLocale::TimeNone))
        .arg(m_subject->style()->timestampSeparator())
        .arg(MLocale().formatDateTime(oldDateTime.toLocalTime(), MLocale::DateNone, MLocale::TimeShort));

    QCOMPARE(oldTimeString, m_subject->d_func()->bannerTimeStamp()->text());

    oldDateTime = testDateTime.addDays(-2);
    m_banner->setBannerTimeStamp(oldDateTime);

    QString olderTimeString = QString("%1 %2 %3").arg(MLocale().formatDateTime(oldDateTime.toLocalTime(), MLocale::DateShort, MLocale::TimeNone))
        .arg(m_subject->style()->timestampSeparator())
        .arg(MLocale().formatDateTime(oldDateTime.toLocalTime(), MLocale::DateNone, MLocale::TimeShort));

    QCOMPARE(olderTimeString, m_subject->d_func()->bannerTimeStamp()->text());
}

void Ut_MBannerView::testTimestampDateTimeFormatIsUpdatedWhenDayHasPassed()
{
    QDateTime testDateTime = QDateTime::currentDateTime();
    m_banner->setBannerTimeStamp(testDateTime);
    QString currentTimeString = MLocale().formatDateTime(testDateTime, MLocale::DateNone, MLocale::TimeShort);

    m_subject->drawForeground(NULL, NULL);
    QCOMPARE(currentTimeString, m_subject->d_func()->bannerTimeStamp()->text());

    QDateTime oldDateTime = testDateTime.addDays(-1);
    m_banner->setBannerTimeStamp(oldDateTime);
    QString oldTimeString = QString("%1 %2 %3").arg(MLocale().formatDateTime(oldDateTime.toLocalTime(), MLocale::DateShort, MLocale::TimeNone))
        .arg(m_subject->style()->timestampSeparator())
        .arg(MLocale().formatDateTime(oldDateTime.toLocalTime(), MLocale::DateNone, MLocale::TimeShort));

    m_subject->drawForeground(NULL, NULL);
    QCOMPARE(oldTimeString, m_subject->d_func()->bannerTimeStamp()->text());
}

QTEST_APPLESS_MAIN(Ut_MBannerView)
