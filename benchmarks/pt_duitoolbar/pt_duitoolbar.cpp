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

#include <QObject>
#include <DuiApplication>
#include <DuiToolBar>
#include <DuiTheme>
#include <DuiAction>

#include "pt_duitoolbar.h"

DuiApplication *app;

void Pt_DuiToolBar::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./pt_duitoolbar";
    app = new DuiApplication(argc, &app_name);
}

void Pt_DuiToolBar::cleanupTestCase()
{
    delete app;
}

void Pt_DuiToolBar::init()
{
    //wait for the resource loading to finish
    while (DuiTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }
}

void Pt_DuiToolBar::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");

    QTest::newRow("300x60") << 300 << 60;
}

void Pt_DuiToolBar::paintPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);

    DuiToolBar *toolbar = new DuiToolBar();

    for (int i = 0; i < 4; ++i)
        toolbar->addAction(new DuiAction("Action " + QString::number(i), this));

    // create pixmap paintdevice
    QPixmap *pixmap = new QPixmap(width, height);
    pixmap->fill(QColor(0, 0, 0, 0));
    QPainter *painter = new QPainter(pixmap);

    // actual benchmark

    QBENCHMARK {
        toolbar->paint(painter, NULL);
    }

    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT

    QString kuva = "toolbar.png";
    pixmap->save(kuva, "png", -1);

#endif

    delete painter;
    delete pixmap;
    delete toolbar;
}


QTEST_APPLESS_MAIN(Pt_DuiToolBar)
