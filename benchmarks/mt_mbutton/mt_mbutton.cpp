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

#include "memorybenchmark.h"
#include "memorybenchmark.inl"
#include "mt_mbutton.h"

#include "mclassfactory.h"
#include "mwidgetview.h"

#include <QPainter>
#include <QPixmap>
#include <MButton>
#include <MApplication>
#include <MTheme>

MApplication *app;
MButton *warmupButton;
const int W = 250;
const int H = 100;

void Mt_MButton::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./Mt_MButton";
    app = new MApplication(argc, &app_name);

    QPixmap pixmap(W, H);
    QPainter painter(&pixmap);

    warmupButton = new MButton();
    warmupButton->setText("text");
    warmupButton->setIconID("icon-m-framework-home");
    warmupButton->setToggledIconID("icon-m-framework-close");

    warmupButton->setGeometry(QRectF(0, 0, W, H));

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    warmupButton->paint(&painter, NULL);

    delete warmupButton;
    warmupButton = NULL;
}

void Mt_MButton::cleanupTestCase()
{
    delete warmupButton;
    delete app;
}

void Mt_MButton::initSchema()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("view");
    QTest::addColumn<QString>("icon1");
    QTest::addColumn<QString>("icon2");
    QTest::addColumn<QString>("text");
}

void Mt_MButton::memoryMButtonView()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    memoryBenchmark(width, height, view, icon1, icon2, text);
}

void Mt_MButton::memoryMButtonView_data()
{
    initSchema();

    QTest::newRow("Empty") << W << H << "MButtonView" << "" << "" << "";
    QTest::newRow("Text") << W << H << "MButtonView" << "" << "" << "ButtonText";
    QTest::newRow("Icon") << W << H << "MButtonView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("Text&Icon") << W << H << "MButtonView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("Toggle") << W << H << "MButtonView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("Toggle&Text") << W << H << "MButtonView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";
}

void Mt_MButton::memoryMButtonDefaultView()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    memoryBenchmark(width, height, view, icon1, icon2, text);
}

void Mt_MButton::memoryMButtonDefaultView_data()
{
    initSchema();

    QTest::newRow("Empty") << W << H << "MButtonDefaultView" << "" << "" << "";
    QTest::newRow("Text") << W << H << "MButtonDefaultView" << "" << "" << "ButtonText";
    QTest::newRow("Icon") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("Text&Icon") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("Toggle") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("Toggle&Text") << W << H << "MButtonDefaultView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";
}

void Mt_MButton::memoryMButtonIconView()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    memoryBenchmark(width, height, view, icon1, icon2, text);
}

void Mt_MButton::memoryMButtonIconView_data()
{
    initSchema();

    QTest::newRow("Empty") << W << H << "MButtonIconView" << "" << "" << "";
    QTest::newRow("Text") << W << H << "MButtonIconView" << "" << "" << "ButtonText";
    QTest::newRow("Icon") << W << H << "MButtonIconView" << "icon-m-framework-home" << "" << "";
    QTest::newRow("Text&Icon") << W << H << "MButtonIconView" << "icon-m-framework-home" << "" << "ButtonText";
    QTest::newRow("Toggle") << W << H << "MButtonIconView" << "icon-m-framework-home" << "icon-m-framework-close" << "";
    QTest::newRow("Toggle&Text") << W << H << "MButtonIconView" << "icon-m-framework-home" << "icon-m-framework-close" << "ButtonText";
}

void Mt_MButton::memoryMButtonViewTimeline()
{
    memoryBenchmark(W, H, "MButtonView", "icon-m-framework-home", "icon-m-framework-close", "ButtonText");
}

void Mt_MButton::memoryMButtonViewDefaultViewTimeline()
{
    memoryBenchmark(W, H, "MButtonDefaultView", "icon-m-framework-home", "icon-m-framework-close", "ButtonText");
}

void Mt_MButton::memoryMButtonViewIconViewTimeline()
{
    memoryBenchmark(W, H, "MButtonIconView", "icon-m-framework-home", "icon-m-framework-close", "ButtonText");
}

void Mt_MButton::memoryBenchmark(qint32 width, qint32 height, const QString &view, const QString &icon1, const QString &icon2, const QString &text)
{
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, width, height), QColor(0, 0, 0));

    beginMemoryBenchmark();

    MButton *button = new MButton();

    MWidgetView *buttonView = MClassFactory::instance()->createView(view.toStdString().c_str(), button);
    button->setView(buttonView);

    button->setGeometry(QRectF(0, 0, width, height));
    if (!text.isEmpty()) {
        button->setText(text);
        button->setTextVisible(true);
    } else
        button->setTextVisible(false);

    if (!icon1.isEmpty()) {
        button->setIconID(icon1);
        button->setIconVisible(true);
    } else
        button->setIconVisible(false);

    if (!icon2.isEmpty()) {
        button->setToggledIconID(icon2);
        button->setIconVisible(true);
    }

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    button->paint(&painter, NULL);

    outputAllocatedMemorySize();

    delete button;

    endMemoryBenchmark();

    // save a shot (for debugging)
//#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << view
            << "_"
            << "\"" << icon1 << "\""
            << "_"
            << "\"" << icon2 << "\""
            << "_"
            << "\"" << text << "\""
            << "_"
            << width
            << "x"
            << height
            << ".png";
    pixmap.save(kuva, "png", -1);
#endif
}

QTEST_APPLESS_MAIN(Mt_MButton)

