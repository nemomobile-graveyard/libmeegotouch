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
#include <MTheme>
#include <MSlider>
#include <MSliderView>
#include <MApplication>

#include "pt_mslider.h"


MApplication *app;

void Pt_MSlider::initTestCase()
{
    int argc = 1;
    const char *argv[argc];
    char appName[] = "./pt_mslider";
    argv[0] = appName;
    app = new MApplication(argc, (char **)argv);
}

void Pt_MSlider::cleanupTestCase()
{
    delete app;
}

void Pt_MSlider::createView()
{
    currentView = new MSliderView(m_subject);
}

void Pt_MSlider::fetchNewRow()
{
    QFETCH(qint32, orientation);

    this->orientation      = orientation;
}

void Pt_MSlider::createController()
{
    this->m_subject = new MSlider();

    this->m_subject->setMinLabelIconID("icon-l-contacts");
    this->m_subject->setMaxLabelIconID("icon-l-browser");
    this->m_subject->setHandleLabelIconID("icon-l-music");
    this->m_subject->setMaxLabel("Max");
    this->m_subject->setHandleLabel("Thumb");
    this->m_subject->setMinLabelVisible(true);
    this->m_subject->setMaxLabelVisible(true);
    this->m_subject->setHandleLabelVisible(true);
    this->m_subject->setRange(0, 100);
    this->m_subject->setValue(50);
    this->m_subject->setOrientation((this->orientation == 0) ? Qt::Horizontal : Qt::Vertical);
}

void Pt_MSlider::createPaintDevice()
{
    this->pixmap = new QPixmap(864, 864);
    this->pixmap->fill(QColor(0, 0, 0, 0));
    this->painter = new QPainter(pixmap);
}

void Pt_MSlider::init()
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

void Pt_MSlider::cleanup()
{
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    QTextStream(&kuva)
            << "view_"
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

void Pt_MSlider::paintPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);

    painter->translate(width, height);
    // actual benchmark
    QBENCHMARK {
        this->currentView->paint(painter, NULL);
    }
}

void Pt_MSlider::paintPerformance_data()
{

    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<qint32>("orientation");

    // horizontal max
    QTest::newRow("864x32") << 864  << 32 << 0;

    // vertical max
    QTest::newRow("32x480") << 32 << 480 << 1;
}

QTEST_APPLESS_MAIN(Pt_MSlider)
