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
#include "mt_duimenu.h"
#include "duiclassfactory.h"

#include <QPainter>
#include <QPixmap>
#include <DuiApplicationMenu>
#include <DuiApplication>
#include <DuiTheme>
#include <DuiSceneManager>

DuiApplication *app;

void Mt_DuiMenu::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./mt_duimenu";
    app = new DuiApplication(argc, &app_name);

    int width = 864;
    int height = 480;
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    DuiApplicationMenu *warmupMenu = new DuiApplicationMenu();
    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }
    warmupMenu->paint(&painter, NULL);
    delete warmupMenu;
}

void Mt_DuiMenu::cleanupTestCase()
{
    delete app;
}

void Mt_DuiMenu::memoryDuiMenuView()
{
    memoryBenchmark("DuiApplicationMenuView");
}

void Mt_DuiMenu::memoryBenchmark(const QString &view)
{
    int width = 864;
    int height = 480;

    // create pixmap paintdevice
    QPixmap pixmap(width, height);
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);

    beginMemoryBenchmark();

    DuiApplicationMenu *menu = new DuiApplicationMenu();
    DuiWidgetView *menuView = DuiClassFactory::instance()->createView(view.toStdString().c_str(), menu);
    menu->setView(menuView);

    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    menu->paint(&painter, NULL);

    // Print used memory, it will be recorded in a log
    outputAllocatedMemorySize();

    delete menu;
    endMemoryBenchmark();
}

QTEST_APPLESS_MAIN(Mt_DuiMenu)

