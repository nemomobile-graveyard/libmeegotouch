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
#include "minfobanner.h"
#include "minfobannereventview.h"
#include "ut_minfobanner.h"

Q_DECLARE_METATYPE(MInfoBanner::BannerType);
Q_DECLARE_METATYPE(MTheme::ViewType);

MApplication *app;

void Ut_MInfoBanner::init()
{
}

void Ut_MInfoBanner::cleanup()
{
}

void Ut_MInfoBanner::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_minfobanner" };
    app = new MApplication(argc, argv);
}


void Ut_MInfoBanner::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_MInfoBanner::testCreation_data()
{
    QTest::addColumn<MInfoBanner::BannerType>("type");
    QTest::addColumn<MTheme::ViewType>("variant");
    QTest::addColumn<QString>("icon");
    QTest::addColumn<QString>("body");
    QTest::addColumn<QString>("buttonIconId");

    QTest::newRow("info") << MInfoBanner::Information
                          << MTheme::ViewType(MInfoBanner::informationType)
                          << QString("icon-m-framework-close")
                          << QString("<font color=\"white\">Battery is running low</font>")
                          << QString("");

    QTest::newRow("event") << MInfoBanner::Event
                           << MTheme::ViewType(MInfoBanner::eventType)
                           << QString("ebf7db7b6ab73c6cba8fc87da2d1fdcf")
                           << QString("<font color=\"white\" size=\"3\">Ida Taipale</font><br/><font color=\"white\" size=\"2\">Have you seen my dog?</font>")
                           << QString("icon-m-toolbar-send-sms");

    QTest::newRow("invalid") << (MInfoBanner::BannerType)10
                             << MTheme::ViewType(MInfoBanner::eventType)
                             << QString("XXXXXXX invalid image XXXXXXX")
                             << QString("XXXXXXX invalid contents XXXXXXXX")
                             << QString("XXXXXXX invalid icon XXXXXXXXXXX");
}

void Ut_MInfoBanner::testCreation()
{
    QFETCH(MInfoBanner::BannerType, type);
    QFETCH(MTheme::ViewType, variant);
    QFETCH(QString, icon);
    QFETCH(QString, body);
    QFETCH(QString, buttonIconId);

    MInfoBanner *banner = new MInfoBanner(type);
    banner->setImageID(icon);
    banner->setBodyText(body);
    banner->setIconID(buttonIconId);

    // verify that model has been set
    QVERIFY(banner->model() != 0);
    QVERIFY(banner->viewType() == variant);

    delete banner;
}

void Ut_MInfoBanner::testUpdate()
{
    MInfoBanner *banner = new MInfoBanner(MInfoBanner::Information);

    banner->setImageID("image1");
    banner->setIconID("icon1");
    banner->setBodyText("body1");
    banner->setButtonText("button");

    QCOMPARE(banner->bannerType(), MInfoBanner::Information);
    QCOMPARE(banner->imageID(), QString("image1"));
    QCOMPARE(banner->bodyText(),  QString("body1"));
    QCOMPARE(banner->iconID(),  QString("icon1"));
    QCOMPARE(banner->buttonText(), QString("button"));
    QCOMPARE(banner->pixmap().isNull(), true);

    banner->setPixmap(QPixmap(50, 50));
    QCOMPARE(banner->pixmap().size(), QSize(50, 50));
    QCOMPARE(banner->pixmap().isNull(), false);
    QCOMPARE(banner->imageID(), QString());

    delete banner;
}

void Ut_MInfoBanner::testClicking()
{
    MInfoBanner *banner = new MInfoBanner(MInfoBanner::Information);
    banner->setIconID("ButtonIconId");

    // click the banner
    QSignalSpy clickedSpy(banner, SIGNAL(clicked()));
    banner->click();
    QCOMPARE(clickedSpy.count(), 1);

    // click the button in the banner
    QSignalSpy buttonClickedSpy(banner, SIGNAL(buttonClicked()));
    banner->setButtonText("test");
    banner->buttonClicked();
    QCOMPARE(buttonClickedSpy.count(), 1);

    delete banner;
}

QTEST_APPLESS_MAIN(Ut_MInfoBanner)
