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
#include <MScalableImage>
#include <MTheme>
#include <QPainter>
#include "mglrenderer.h"

#include "pt_mscalableimage.h"

#ifdef M_USE_OPENGL
#include <QGLWidget>
#endif

MApplication *app;
#ifdef M_USE_OPENGL
QGLWidget *glw  = NULL;
#endif
void Pt_MScalableImage::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./pt_mscalableimage";
    app = new MApplication(argc, &app_name);
#ifdef M_USE_OPENGL
    glw = new QGLWidget();
    MGLRenderer::instance()->initGL(glw);
#endif
}

void Pt_MScalableImage::cleanupTestCase()
{
#ifdef M_USE_OPENGL
    delete glw;
#endif
    delete app;
}

void Pt_MScalableImage::init()
{
    //wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }
}

void Pt_MScalableImage::paintScalablePerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");
    QTest::addColumn<bool>("useGLRenderer");

    QTest::newRow("300x300 SW") << 300 << 300 << false << false;
    QTest::newRow("300x300 HW !MGLRenderer") << 300 << 300 << true << false;
    QTest::newRow("300x300 HW MGLRenderer") << 300 << 300 << true << true;
}

void Pt_MScalableImage::paintScalablePerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);
    QFETCH(bool, useGLRenderer);

    MScalableImage *image = const_cast<MScalableImage *>(MTheme::scalableImage("stretchbutton", 11, 11, 11, 11));
    image->enableOptimizedRendering(useGLRenderer);

    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        pixmap->fill(QColor(255, 255, 255, 255));
        painter.begin(pixmap);
    } else {
#ifdef M_USE_OPENGL
        glw->resize(width, height);
        MGLRenderer::instance()->initGL(glw);
        painter.begin(glw);
        painter.fillRect(0, 0, width, height, QColor(255, 255, 255, 255));
#else
        qWarning("Cannot run HW test.");
#endif
    }

    // actual benchmark
    if (painter.isActive()) {
        QBENCHMARK {
            image->draw(0, 0, width, height, &painter);
        }
        painter.end();
    }
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    kuva.sprintf("scalable_%d_%d_%d_%d.png", width, height, HW, useGLRenderer);
    if (pixmap)
        pixmap->save(kuva, "png", -1);
    else {
#ifdef M_USE_OPENGL
        glw->grabFrameBuffer().save(kuva, "png", -1);
#endif
    }
#endif
    delete pixmap;
    MTheme::releaseScalableImage(image);
}

void Pt_MScalableImage::paintPixmapPerformance_data()
{
    QTest::addColumn<qint32>("width");
    QTest::addColumn<qint32>("height");
    QTest::addColumn<bool>("HW");
    QTest::addColumn<bool>("useGLRenderer");

    QTest::newRow("300x300 SW") << 300 << 300 << false << false;
    QTest::newRow("300x300 HW !MGLRenderer") << 300 << 300 << true << false;
    QTest::newRow("300x300 HW MGLRenderer") << 300 << 300 << true << true;
}


void Pt_MScalableImage::paintPixmapPerformance()
{
    QFETCH(qint32, width);
    QFETCH(qint32, height);
    QFETCH(bool, HW);
    QFETCH(bool, useGLRenderer);

    const QPixmap *image = MTheme::pixmap("stretchbutton", QSize(width, height));
    //glw->bindTexture(*image);
    //((MyScalable*)image)->setUseMGLRenderer(useGLRenderer);
    //wait for the resource loading to finish
    while (MTheme::instance()->hasPendingRequests()) {
        usleep(100);
        QCoreApplication::processEvents();
    }
    QPainter painter;
    QPixmap *pixmap = NULL;
    if (!HW) {
        pixmap = new QPixmap(width, height);
        pixmap->fill(QColor(255, 255, 255, 255));
        painter.begin(pixmap);
    } else {
#ifdef M_USE_OPENGL
        glw->resize(width, height);
        painter.begin(glw);
        painter.fillRect(0, 0, width, height, QColor(255, 255, 255, 255));
#else
        qWarning("Cannot run HW test.");
#endif
    }

    //warmup
    //painter.drawPixmap(0, 0, *image);

    // actual benchmark
    if (painter.isActive()) {
        if (useGLRenderer) {
#ifdef M_USE_OPENGL
            QBENCHMARK {
                MGLRenderer::instance()->drawPixmap(painter.combinedTransform(), *image, 1.0);
            }
#endif
        } else {
            QBENCHMARK {
                painter.drawPixmap(0, 0, *image);
            }
        }
        painter.end();
    }
    // save a shot (for debugging)
#define SCREENSHOT
#ifdef SCREENSHOT
    QString kuva;
    kuva.sprintf("pixmap_%d_%d_%d_%d.png", width, height, HW, useGLRenderer);
    if (pixmap)
        pixmap->save(kuva, "png", -1);
    else {
#ifdef M_USE_OPENGL
        glw->grabFrameBuffer().save(kuva, "png", -1);
#endif
    }
#endif
    delete pixmap;
    MTheme::releasePixmap(image);
}
QTEST_APPLESS_MAIN(Pt_MScalableImage)
