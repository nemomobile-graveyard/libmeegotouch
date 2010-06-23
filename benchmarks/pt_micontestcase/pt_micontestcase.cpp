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
#include <QGraphicsSceneMouseEvent>
#include <MTheme>
#include <MButton>
#include <MButtonView>
#include <MButtonIconView>
#include <mbuttonobjectmenuitemview.h>
#include <MButtonSpinView>
#include <mbuttonviewmenuitemview.h>
#include <mbuttonviewmenuview.h>
#include <MStyleAttribute>
#include <MApplication>

#include "pt_micontestcase.h"

MApplication *app;

void Pt_MButton::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./eternia";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
    MTheme::instance()->changeTheme("common");
    MTheme::addPixmapDirectory("images/", true);

    currentViewIndex = 0;
}

void Pt_MButton::cleanupTestCase()
{
    delete app;
}

void Pt_MButton::init()
{
    // get size dimensions for test
    QFETCH(qint32, viewIndex);
    QFETCH(qint32, width);
    QFETCH(qint32, height);

    this->currentViewIndex = viewIndex;
    this->width = width;
    this->height = height;

    // create widget, set size
    m_subject = new MButton();

    m_subject->setObjectName("MyButton2");
    m_subject->setText("SILENCE!");
    m_subject->setMinimumSize(width, height);
    m_subject->setMaximumSize(width, height);

    // create wanted view
    views[ this->currentViewIndex ] = new MButtonView(m_subject);

    m_subject->setView(views[ this->currentViewIndex ]);   // transfers ownership to controller
    m_subject->setIconID("icon-m-telephony-ongoing-muted");

    views[ this->currentViewIndex ]->updateStyle();
    views[ this->currentViewIndex ]->styleUpdated();

    // if this is paced above 'updateStyle()', then get a core dump; if above setView(), then no icon
    //m_subject->setIconID("icon-m-telephony-ongoing-muted");
    qDebug() << "ICONID" << m_subject->iconID();

    sleep(1);
    QCoreApplication::processEvents();

    // create pixmap paintdevice
    pixmap = new QPixmap(width, height);
    pixmap->fill(Qt::black);
    painter = new QPainter(pixmap);
}

void Pt_MButton::cleanup()
{
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << "view_"
            << this->currentViewIndex
            << "_"
            << m_subject->size().width()
            << "x"
            << m_subject->size().height()
            << ".png";
    if (!written.contains(kuva)) {
        pixmap->save(kuva, "png", -1);
        written.append(kuva);
    }
#endif

    // delete m_subject;
    // m_subject = 0;
    // if you call 'setView()' on this, then the controller owns the object
    //delete view;
    //view = 0;
    delete painter;
    painter = 0;
    delete pixmap;
    pixmap = 0;

}

void Pt_MButton::paintPerformance()
{
    // actual benchmark
    //QBENCHMARK
    {
        views[ currentViewIndex ]->paint(painter, NULL);
    }
}

void Pt_MButton::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<qint32>("viewIndex");

    QTest::newRow("864x480") << 864 << 480 << 0;
}

QTEST_APPLESS_MAIN(Pt_MButton)
