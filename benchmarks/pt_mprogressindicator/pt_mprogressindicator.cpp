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
#include <MTheme>
#include <MProgressIndicator>
#include <MProgressIndicatorBarView>
#include "mspinnerview.h"

#include "pt_mprogressindicator.h"


MApplication *app;

void Pt_MProgressIndicator::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./widgetsgallery";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
    //MTheme::instance()->changeTheme("common");
    //MTheme::addPixmapDirectory("images/", true);

    currentViewName = FirstView;
}

void Pt_MProgressIndicator::cleanupTestCase()
{
    delete app;
}

void Pt_MProgressIndicator::createView()
{
    switch (this->currentViewName) {
    case Default:
        this->currentView = new MSpinnerView(m_subject);
        break;
    case Bar:
        this->currentView = new MProgressIndicatorBarView(m_subject);
        break;
    default:
        break;
    }
}

void Pt_MProgressIndicator::fetchNewRow()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(qint32, viewIndex);

    this->currentViewName  = (ViewName)viewIndex;
    this->width            = width;
    this->height           = height;
}

void Pt_MProgressIndicator::createController()
{
    this->m_subject = new MProgressIndicator();

    this->m_subject->setMinimumSize(this->width, this->height);
    this->m_subject->setMaximumSize(this->width, this->height);
    this->m_subject->setRange(0, 100);
    this->m_subject->setValue(50);
}

void Pt_MProgressIndicator::createPaintDevice()
{
    this->pixmap = new QPixmap(864, 864);
    this->pixmap->fill(QColor(0, 0, 0, 0));
    this->painter = new QPainter(pixmap);
}

void Pt_MProgressIndicator::init()
{
    fetchNewRow();

    createController();

    createView();

    if (this->currentView) {
        this->m_subject->setView(this->currentView);   // transfers ownership to controller
    }

    //wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

    // create pixmap paintdevice
    createPaintDevice();
}

void Pt_MProgressIndicator::cleanup()
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

void Pt_MProgressIndicator::paintPerformance()
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

void Pt_MProgressIndicator::paintPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<qint32>("viewIndex");

    for (qint32 viewIndex = FirstView; viewIndex < NoViews; viewIndex++) {
        QTest::newRow("864x64") << 864 / 2 << 64 << viewIndex;
        QTest::newRow("864x128") << 864 / 2 << 128 << viewIndex;
    }
}

QTEST_APPLESS_MAIN(Pt_MProgressIndicator)
