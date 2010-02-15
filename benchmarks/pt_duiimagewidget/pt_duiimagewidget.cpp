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

#include "pt_duiimagewidget.h"

#include <QObject>
#include <DuiApplication>
#include <DuiImageWidget>
#include <DuiImageWidgetView>
#include <DuiTheme>


DuiApplication *app;

void Pt_DuiImageWidget::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./pt_duiimage";
    app = new DuiApplication(argc, &app_name);
    DuiTheme::addPixmapDirectory(QCoreApplication::applicationDirPath());

    m_subject = new DuiImageWidget();
}

void Pt_DuiImageWidget::cleanupTestCase()
{
    delete m_subject;
    delete app;
}

void Pt_DuiImageWidget::paintPerformance_data()
{
    //wait for the resource loading to finish
    while (DuiTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }

    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<QString>("imagename");

    QTest::newRow("300x300 PNG") << 300 << 300 << "testpng";
    QTest::newRow("300x300 SVG") << 300 << 300 << "Layer_1";

    // fullscreen
    QTest::newRow("864x480 PNG") << 864 << 480 << "testpng";
    QTest::newRow("864x480 SVG") << 864 << 480 << "Layer_1";
}

void Pt_DuiImageWidget::paintPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(QString, imagename);

    m_subject->setImage(imagename);
    m_subject->setGeometry(QRectF(0, 0, width, height));

    // create pixmap paintdevice
    QPixmap *pixmap = new QPixmap(width, height);
    pixmap->fill(QColor(0, 0, 0, 0));
    QPainter *painter = new QPainter(pixmap);

    // actual benchmark
    QBENCHMARK {
        m_subject->paint(painter, NULL);
    }

    // save a shot (for debugging)
#ifdef SCREENSHOT

    QStringList list;
    list.append(imagename);
    list.append(QString::number(this->m_subject->size().width()));
    list.append(QString::number(this->m_subject->size().height()));

    QString kuva = list.join("_") + ".png";
    pixmap->save(kuva, "png", -1);

#endif

    delete painter;
    delete pixmap;
}


QTEST_APPLESS_MAIN(Pt_DuiImageWidget)
