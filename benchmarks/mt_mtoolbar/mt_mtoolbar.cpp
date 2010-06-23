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
#include "mt_mtoolbar.h"

#include <QPainter>
#include <QPixmap>
#include <MToolBar>
#include <MApplication>
#include <MTheme>
#include <MAction>

MApplication *app;

void Mt_MToolBar::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./mt_mmenu";
    app = new MApplication(argc, &app_name);

    // We need to warmup QTs painters and probably
    // some other subsystems
    QPixmap pixmap(300, 60);
    QPainter painter(&pixmap);
    MToolBar *toolbar = new MToolBar();
    toolbar->setGeometry(QRectF(0, 0, 300, 60));
    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    for (int i = 0; i < 1; ++i)
        toolbar->addAction(new MAction("icon-l-music", "Action " + QString::number(i), this));
    toolbar->paint(&painter, NULL);
    delete toolbar;
}

void Mt_MToolBar::cleanupTestCase()
{
    delete app;
}

void Mt_MToolBar::memoryBenchmark_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("icon");

    QTest::newRow("300x60") << 300 << 60 << "icon-l-music";
}

void Mt_MToolBar::memoryBenchmark()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, icon);

    // create pixmap paintdevice
    QPixmap pixmap(width, height);
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    MToolBar *toolbar = new MToolBar();
    toolbar->setGeometry(QRectF(0, 0, width, height));
    for (int i = 0; i < 5; ++i)
        toolbar->addAction(new MAction(icon, "Action " + QString::number(i), this));

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    toolbar->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    delete toolbar;

    endMemoryBenchmark();
}

QTEST_APPLESS_MAIN(Mt_MToolBar)

