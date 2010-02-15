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

#include "memorybenchmark.h"
#include "memorybenchmark.inl"
#include "mt_duibutton.h"

#include "duiclassfactory.h"
#include "duiwidgetview.h"

#include <QPainter>
#include <QPixmap>
#include <DuiButton>
#include <DuiApplication>
#include <DuiTheme>

DuiApplication *app;
DuiButton *warmupButton;
const int W = 250;
const int H = 100;

void Mt_DuiButton::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./Mt_DuiButton";
    app = new DuiApplication(argc, &app_name);

    QPixmap pixmap(W, H);
    QPainter painter(&pixmap);

    warmupButton = new DuiButton();
    warmupButton->setText("text");
    warmupButton->setIconID("Icon-home");
    warmupButton->setToggledIconID("Icon-close");

    warmupButton->setGeometry(QRectF(0, 0, W, H));

    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    warmupButton->paint(&painter, NULL);

    delete warmupButton;
    warmupButton = NULL;
}

void Mt_DuiButton::cleanupTestCase()
{
    delete warmupButton;
    delete app;
}

void Mt_DuiButton::initSchema()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("view");
    QTest::addColumn<QString>("icon1");
    QTest::addColumn<QString>("icon2");
    QTest::addColumn<QString>("text");
}

void Mt_DuiButton::memoryDuiButtonView()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    memoryBenchmark(width, height, view, icon1, icon2, text);
}

void Mt_DuiButton::memoryDuiButtonView_data()
{
    initSchema();

    QTest::newRow("Empty") << W << H << "DuiButtonView" << "" << "" << "";
    QTest::newRow("Text") << W << H << "DuiButtonView" << "" << "" << "ButtonText";
    QTest::newRow("Icon") << W << H << "DuiButtonView" << "Icon-home" << "" << "";
    QTest::newRow("Text&Icon") << W << H << "DuiButtonView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("Toggle") << W << H << "DuiButtonView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("Toggle&Text") << W << H << "DuiButtonView" << "Icon-home" << "Icon-close" << "ButtonText";
}

void Mt_DuiButton::memoryDuiButtonDefaultView()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    memoryBenchmark(width, height, view, icon1, icon2, text);
}

void Mt_DuiButton::memoryDuiButtonDefaultView_data()
{
    initSchema();

    QTest::newRow("Empty") << W << H << "DuiButtonDefaultView" << "" << "" << "";
    QTest::newRow("Text") << W << H << "DuiButtonDefaultView" << "" << "" << "ButtonText";
    QTest::newRow("Icon") << W << H << "DuiButtonDefaultView" << "Icon-home" << "" << "";
    QTest::newRow("Text&Icon") << W << H << "DuiButtonDefaultView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("Toggle") << W << H << "DuiButtonDefaultView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("Toggle&Text") << W << H << "DuiButtonDefaultView" << "Icon-home" << "Icon-close" << "ButtonText";
}

void Mt_DuiButton::memoryDuiButtonIconView()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, view);
    QFETCH(QString, icon1);
    QFETCH(QString, icon2);
    QFETCH(QString, text);

    memoryBenchmark(width, height, view, icon1, icon2, text);
}

void Mt_DuiButton::memoryDuiButtonIconView_data()
{
    initSchema();

    QTest::newRow("Empty") << W << H << "DuiButtonIconView" << "" << "" << "";
    QTest::newRow("Text") << W << H << "DuiButtonIconView" << "" << "" << "ButtonText";
    QTest::newRow("Icon") << W << H << "DuiButtonIconView" << "Icon-home" << "" << "";
    QTest::newRow("Text&Icon") << W << H << "DuiButtonIconView" << "Icon-home" << "" << "ButtonText";
    QTest::newRow("Toggle") << W << H << "DuiButtonIconView" << "Icon-home" << "Icon-close" << "";
    QTest::newRow("Toggle&Text") << W << H << "DuiButtonIconView" << "Icon-home" << "Icon-close" << "ButtonText";
}

void Mt_DuiButton::memoryDuiButtonViewTimeline()
{
    memoryBenchmark(W, H, "DuiButtonView", "Icon-home", "Icon-close", "ButtonText");
}

void Mt_DuiButton::memoryDuiButtonViewDefaultViewTimeline()
{
    memoryBenchmark(W, H, "DuiButtonDefaultView", "Icon-home", "Icon-close", "ButtonText");
}

void Mt_DuiButton::memoryDuiButtonViewIconViewTimeline()
{
    memoryBenchmark(W, H, "DuiButtonIconView", "Icon-home", "Icon-close", "ButtonText");
}

void Mt_DuiButton::memoryBenchmark(qint32 width, qint32 height, const QString &view, const QString &icon1, const QString &icon2, const QString &text)
{
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, width, height), QColor(0, 0, 0));

    beginMemoryBenchmark();

    DuiButton *button = new DuiButton();

    DuiWidgetView *buttonView = DuiClassFactory::instance()->createView(view.toStdString().c_str(), button);
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

    while (DuiTheme::hasPendingRequests()) {
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

QTEST_APPLESS_MAIN(Mt_DuiButton)

