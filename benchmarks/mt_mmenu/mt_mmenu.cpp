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
#include "mt_mmenu.h"
#include "mclassfactory.h"

#include <QPainter>
#include <QPixmap>
#include <MApplicationMenu>
#include <MApplication>
#include <MTheme>
#include <MSceneManager>

MApplication *app;

void Mt_MMenu::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./mt_mmenu";
    app = new MApplication(argc, &app_name);

    int width = 864;
    int height = 480;
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    MApplicationMenu *warmupMenu = new MApplicationMenu();
    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    warmupMenu->paint(&painter, NULL);
    delete warmupMenu;
}

void Mt_MMenu::cleanupTestCase()
{
    delete app;
}

void Mt_MMenu::memoryMMenuView()
{
    memoryBenchmark("MApplicationMenuView");
}

void Mt_MMenu::memoryBenchmark(const QString &view)
{
    int width = 864;
    int height = 480;

    // create pixmap paintdevice
    QPixmap pixmap(width, height);
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    MApplicationMenu *menu = new MApplicationMenu();
    MWidgetView *menuView = MClassFactory::instance()->createView(view.toStdString().c_str(), menu);
    menu->setView(menuView);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    menu->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    delete menu;
    endMemoryBenchmark();
}

QTEST_APPLESS_MAIN(Mt_MMenu)

