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

#include <QObject>
#include <MApplication>
#include <MToolBar>
#include <MTheme>
#include <MAction>

#include "pt_mtoolbar.h"

MApplication *app;

void Pt_MToolBar::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./pt_mtoolbar";
    app = new MApplication(argc, &app_name);
}

void Pt_MToolBar::cleanupTestCase()
{
    delete app;
}

void Pt_MToolBar::init()
{
    //wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }
}

void Pt_MToolBar::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");

    QTest::newRow("300x60") << 300 << 60;
}

void Pt_MToolBar::paintPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);

    MToolBar *toolbar = new MToolBar();

    for (int i = 0; i < 4; ++i)
        toolbar->addAction(new MAction("Action " + QString::number(i), this));

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


QTEST_APPLESS_MAIN(Pt_MToolBar)
