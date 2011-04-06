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
#include <MApplicationWindow>
#include <MScalableImage>
#include <MTheme>
#include <MWindow>
#include <MComponentCache>
#include <QPainter>

#include "pt_mscalableimage.h"

MApplication *app;
MApplicationWindow *window;

void Pt_MScalableImage::initTestCase()
{
}

void Pt_MScalableImage::cleanupTestCase()
{
}

void Pt_MScalableImage::init()
{
    //wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }
}

void Pt_MScalableImage::cleanup()
{
    if (app->hasPendingEvents())
        app->processEvents();
}

void Pt_MScalableImage::paintScalablePerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintScalablePerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    MScalableImage *image = const_cast<MScalableImage *>(MTheme::scalableImage("meegotouch-button-background", 11, 11, 11, 11));

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    // actual benchmark
    if (painter.isActive()) {
        QBENCHMARK {
            image->draw(0, 0, width, height, &painter);
        }
        painter.end();
    }
    delete pixmap;
    MTheme::releaseScalableImage(image);
}

void Pt_MScalableImage::paintPOTScalablePerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintPOTScalablePerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *imagePixmap = MTheme::pixmap("meegotouch-button-background", QSize(32, 32));
    MScalableImage *image = new MScalableImage(imagePixmap, 11, 11, 11, 11);

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        pixmap->fill(QColor(255, 255, 255, 255));
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
        painter.fillRect(0, 0, width, height, QColor(255, 255, 255, 255));
    }

    // actual benchmark
    if (painter.isActive()) {
        QBENCHMARK {
            image->draw(0, 0, width, height, &painter);
        }
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(imagePixmap);
    delete image;
}

void Pt_MScalableImage::paintHugeScalablePerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintHugeScalablePerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *imagePixmap = MTheme::pixmap("meegotouch-button-background", QSize(500, 500));
    MScalableImage *image = new MScalableImage(imagePixmap, 11, 11, 11, 11);

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    // actual benchmark
    if (painter.isActive()) {
        QBENCHMARK {
            image->draw(0, 0, width, height, &painter);
        }
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(imagePixmap);
    delete image;
}

void Pt_MScalableImage::paintPixmapPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintPixmapPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *image = MTheme::pixmap("meegotouch-button-background");

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    //warmup
    //painter.drawPixmap(0, 0, *image);

    // actual benchmark
    if (painter.isActive()) {
        QBENCHMARK {
            painter.drawPixmap(0, 0, width, height, *image);
        }
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(image);
}

void Pt_MScalableImage::paintHugePixmapPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintHugePixmapPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *image = MTheme::pixmap("meegotouch-button-background", QSize(500, 500));

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    //warmup
    //painter.drawPixmap(0, 0, *image);

    // actual benchmark
    if (painter.isActive()) {
        QBENCHMARK {
            painter.drawPixmap(0, 0, width, height, *image);
        }
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(image);
}

void Pt_MScalableImage::paintSmoothPixmapPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintSmoothPixmapPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *image = MTheme::pixmap("meegotouch-button-background");

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    //warmup
    //painter.drawPixmap(0, 0, *image);

    // actual benchmark
    if (painter.isActive()) {
        bool enabled = painter.renderHints() & QPainter::SmoothPixmapTransform;
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        QBENCHMARK {
            painter.drawPixmap(0, 0, width, height, *image);
        }
        painter.setRenderHint(QPainter::SmoothPixmapTransform, enabled);
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(image);
}

void Pt_MScalableImage::paintSmoothHugePixmapPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintSmoothHugePixmapPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *image = MTheme::pixmap("meegotouch-button-background", QSize(500, 500));

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    //warmup
    //painter.drawPixmap(0, 0, *image);

    // actual benchmark
    if (painter.isActive()) {
        bool enabled = painter.renderHints() & QPainter::SmoothPixmapTransform;
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        QBENCHMARK {
            painter.drawPixmap(0, 0, width, height, *image);
        }
        painter.setRenderHint(QPainter::SmoothPixmapTransform, enabled);
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(image);
}

void Pt_MScalableImage::paintBorderPixmapPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");

    QTest::newRow("51x51 SW") << 51 << 51 << false;
    QTest::newRow("32x32 SW") << 32 << 32 << false;
    QTest::newRow("64x64 SW") << 64 << 64 << false;
    QTest::newRow("500x500 SW") << 500 << 500 << false;

    QTest::newRow("51x51 HW") << 51 << 51 << true;
    QTest::newRow("32x32 HW") << 32 << 32 << true;
    QTest::newRow("64x64 HW") << 64 << 64 << true;
    QTest::newRow("500x500 HW") << 500 << 500 << true;
}

void Pt_MScalableImage::paintBorderPixmapPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);

    const QPixmap *image = MTheme::pixmap("meegotouch-button-background");

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        painter.begin(pixmap);
    } else {
        painter.begin(window->viewport());
    }

    //warmup
    //painter.drawPixmap(0, 0, *image);

    // actual benchmark
    if (painter.isActive()) {
        QRect rect(0, 0, width, height);
        QMargins margins(11, 11, 11, 11);
        QBENCHMARK {
            qDrawBorderPixmap(&painter, rect, margins, *image);
        }
        painter.end();
    }
    delete pixmap;
    MTheme::releasePixmap(image);
}

int main(int argc, char *argv[])
{
    app = new MApplication(argc, argv);

    window = new MApplicationWindow();
    window->activateWindow();
    window->show();

    Pt_MScalableImage tc;
    int result = QTest::qExec(&tc, argc, argv);

    delete window;
    delete app;

    return result;
}
