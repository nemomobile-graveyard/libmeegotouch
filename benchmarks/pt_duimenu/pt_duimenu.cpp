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
#include <DuiApplicationMenu>
#include <DuiApplicationMenuView>
#include <DuiMenuObjectView>
#include <DuiWindow>
#include <DuiTheme>
#include <DuiAction>
#include "duiclassfactory.h"

#include "pt_duimenu.h"

DuiApplication *app;
DuiWindow *appWin;

void Pt_DuiMenu::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./pt_duimenu";
    app = new DuiApplication(argc, &app_name);
    appWin = new DuiWindow;
}

void Pt_DuiMenu::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Pt_DuiMenu::init()
{
    //wait for the resource loading to finish
    while (DuiTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

}

void Pt_DuiMenu::cleanup()
{
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT

    QString kuva = this->type + ".png";
    this->pixmap->save(kuva, "png", -1);

#endif

    delete this->painter;
    this->painter = 0;
    delete this->pixmap;
    this->pixmap = 0;

    delete m_subject;
    m_subject = 0;
}

void Pt_DuiMenu::paintPerformance_data()
{
    QTest::addColumn<QString>("view");

    QTest::newRow("DuiApplicationMenuView") << "DuiApplicationMenuView";
}

void Pt_DuiMenu::paintPerformance()
{
    QFETCH(QString, view);

    m_subject = new DuiApplicationMenu();
    DuiWidgetView *menuView = DuiClassFactory::instance()->createView(view.toStdString().c_str(), m_subject);
    m_subject->setView(menuView);

    for (int i = 0; i < 10; ++i)
        m_subject->addAction(new DuiAction("Action " + QString::number(i), this));

    int width = appWin->visibleSceneSize().width();
    int height = appWin->visibleSceneSize().height();

    // create pixmap paintdevice
    this->pixmap = new QPixmap(width, height);
    this->pixmap->fill(QColor(0, 0, 0, 0));
    this->painter = new QPainter(this->pixmap);

    while (DuiTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    // actual benchmark
    QBENCHMARK {
        m_subject->paint(painter, NULL);
    }
}


QTEST_APPLESS_MAIN(Pt_DuiMenu)
