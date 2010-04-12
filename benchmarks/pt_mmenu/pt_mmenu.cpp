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
#include <MApplicationMenu>
#include <MApplicationMenuView>
#include <MMenuObjectView>
#include <MWindow>
#include <MTheme>
#include <MAction>
#include "mclassfactory.h"

#include "pt_mmenu.h"

MApplication *app;
MWindow *appWin;

void Pt_MMenu::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./pt_mmenu";
    app = new MApplication(argc, &app_name);
    appWin = new MWindow;
}

void Pt_MMenu::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Pt_MMenu::init()
{
    //wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

}

void Pt_MMenu::cleanup()
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

void Pt_MMenu::paintPerformance_data()
{
    QTest::addColumn<QString>("view");

    QTest::newRow("MApplicationMenuView") << "MApplicationMenuView";
}

void Pt_MMenu::paintPerformance()
{
    QFETCH(QString, view);

    m_subject = new MApplicationMenu();
    MWidgetView *menuView = MClassFactory::instance()->createView(view.toStdString().c_str(), m_subject);
    m_subject->setView(menuView);

    for (int i = 0; i < 10; ++i)
        m_subject->addAction(new MAction("Action " + QString::number(i), this));

    int width = appWin->visibleSceneSize().width();
    int height = appWin->visibleSceneSize().height();

    // create pixmap paintdevice
    this->pixmap = new QPixmap(width, height);
    this->pixmap->fill(QColor(0, 0, 0, 0));
    this->painter = new QPainter(this->pixmap);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    // actual benchmark
    QBENCHMARK {
        m_subject->paint(painter, NULL);
    }
}


QTEST_APPLESS_MAIN(Pt_MMenu)
