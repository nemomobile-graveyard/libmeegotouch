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
#include <DuiTheme>
#include <DuiProgressIndicator>
#include <DuiProgressIndicatorBarView>
#include "duispinnerview.h"

#include "pt_duiprogressindicator.h"


DuiApplication *app;

void Pt_DuiProgressIndicator::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./widgetsgallery";
    argv[0] = appName;
    app = new DuiApplication(argc, (char **)argv);
    //DuiTheme::instance()->changeTheme("common");
    //DuiTheme::addPixmapDirectory("images/", true);

    currentViewName = FirstView;
}

void Pt_DuiProgressIndicator::cleanupTestCase()
{
    delete app;
}

void Pt_DuiProgressIndicator::createView()
{
    switch (this->currentViewName) {
    case Default:
        this->currentView = new DuiSpinnerView(m_subject);
        break;
    case Bar:
        this->currentView = new DuiProgressIndicatorBarView(m_subject);
        break;
    default:
        break;
    }
}

void Pt_DuiProgressIndicator::fetchNewRow()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(qint32, viewIndex);

    this->currentViewName  = (ViewName)viewIndex;
    this->width            = width;
    this->height           = height;
}

void Pt_DuiProgressIndicator::createController()
{
    this->m_subject = new DuiProgressIndicator();

    this->m_subject->setMinimumSize(this->width, this->height);
    this->m_subject->setMaximumSize(this->width, this->height);
    this->m_subject->setRange(0, 100);
    this->m_subject->setValue(50);
}

void Pt_DuiProgressIndicator::createPaintDevice()
{
    this->pixmap = new QPixmap(864, 864);
    this->pixmap->fill(QColor(0, 0, 0, 0));
    this->painter = new QPainter(pixmap);
}

void Pt_DuiProgressIndicator::init()
{
    fetchNewRow();

    createController();

    createView();

    if (this->currentView) {
        this->m_subject->setView(this->currentView);   // transfers ownership to controller
    }

    //wait for the resource loading to finish
    while (DuiTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

    // create pixmap paintdevice
    createPaintDevice();
}

void Pt_DuiProgressIndicator::cleanup()
{
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << "view_"
            << this->currentViewName
            << "_"
            << this->m_subject->size().width()
            << "x"
            << this->m_subject->size().height()
            << ".png";
    if (!written.contains(kuva)) {
        this->pixmap->save(kuva, "png", -1);
        this->written.append(kuva);
    }
#endif

    delete this->m_subject;
    this->m_subject = 0;

    delete this->painter;
    this->painter = 0;
    delete this->pixmap;
    this->pixmap = 0;

}

void Pt_DuiProgressIndicator::paintPerformance()
{
    painter->translate(864 / 4, 864 / 4);
    // actual benchmark
    QBENCHMARK {
        switch (this->currentViewName) {
        case Default :
        case Bar :
            this->currentView->paint(painter, NULL, NULL);
            break;
        default:
            break;
        }
    }
}

void Pt_DuiProgressIndicator::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<qint32>("viewIndex");

    for (qint32 viewIndex = FirstView; viewIndex < NoViews; viewIndex++) {
        QTest::newRow("864x64") << 864 / 2 << 64 << viewIndex;
        QTest::newRow("864x128") << 864 / 2 << 128 << viewIndex;
    }
}

QTEST_APPLESS_MAIN(Pt_DuiProgressIndicator)
