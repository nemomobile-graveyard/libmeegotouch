/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duiapplication.h"
#include "duiinfobanner.h"
#include "duiinfobannereventview.h"
#include "ut_duiinfobanner.h"

Q_DECLARE_METATYPE(DuiInfoBanner::BannerType);
Q_DECLARE_METATYPE(DuiTheme::ViewType);

DuiApplication *app;

void Ut_DuiInfoBanner::init()
{
}

void Ut_DuiInfoBanner::cleanup()
{
}

void Ut_DuiInfoBanner::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_duiinfobanner" };
    app = new DuiApplication(argc, argv);
}


void Ut_DuiInfoBanner::cleanupTestCase()
{
    delete app;
    app = NULL;
}

void Ut_DuiInfoBanner::testCreation_data()
{
    QTest::addColumn<DuiInfoBanner::BannerType>("type");
    QTest::addColumn<DuiTheme::ViewType>("variant");
    QTest::addColumn<QString>("icon");
    QTest::addColumn<QString>("body");
    QTest::addColumn<QString>("buttonIconId");

    QTest::newRow("info") << DuiInfoBanner::Information
                          << DuiTheme::ViewType(DuiInfoBanner::informationType)
                          << QString("Icon-close")
                          << QString("<font color=\"white\">Battery is running low</font>")
                          << QString("");

    QTest::newRow("event") << DuiInfoBanner::Event
                           << DuiTheme::ViewType(DuiInfoBanner::eventType)
                           << QString("ebf7db7b6ab73c6cba8fc87da2d1fdcf")
                           << QString("<font color=\"white\" size=\"3\">Ida Taipale</font><br/><font color=\"white\" size=\"2\">Have you seen my dog?</font>")
                           << QString("Icon-new-SMS");

    QTest::newRow("invalid") << (DuiInfoBanner::BannerType)10
                             << DuiTheme::ViewType(DuiInfoBanner::eventType)
                             << QString("XXXXXXX invalid image XXXXXXX")
                             << QString("XXXXXXX invalid contents XXXXXXXX")
                             << QString("XXXXXXX invalid icon XXXXXXXXXXX");
}

void Ut_DuiInfoBanner::testCreation()
{
    QFETCH(DuiInfoBanner::BannerType, type);
    QFETCH(DuiTheme::ViewType, variant);
    QFETCH(QString, icon);
    QFETCH(QString, body);
    QFETCH(QString, buttonIconId);

    DuiInfoBanner *banner = new DuiInfoBanner(type);
    banner->setImageID(icon);
    banner->setBodyText(body);
    banner->setIconID(buttonIconId);

    // verify that model has been set
    QVERIFY(banner->model() != 0);
    QVERIFY(banner->viewType() == variant);

    delete banner;
}

void Ut_DuiInfoBanner::testUpdate()
{
    DuiInfoBanner *banner = new DuiInfoBanner(DuiInfoBanner::Information);

    banner->setImageID("image1");
    banner->setIconID("icon1");
    banner->setBodyText("body1");
    banner->setButtonText("button");

    QCOMPARE(banner->bannerType(), DuiInfoBanner::Information);
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

void Ut_DuiInfoBanner::testClicking()
{
    DuiInfoBanner *banner = new DuiInfoBanner(DuiInfoBanner::Information);
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

QTEST_APPLESS_MAIN(Ut_DuiInfoBanner)
