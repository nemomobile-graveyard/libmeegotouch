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

#include <QApplication>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGLWidget>

#include <QProcess>
#include <QTimer>
#include <qtest.h>

#include "pt_minimalqtapplication.h"

#define MY_QBENCHMARK_ONCE(CODE) \
    if (!noBenchmark) { \
        QBENCHMARK_ONCE { \
            static bool firstRun = true; \
            if (!firstRun) { \
                firstRun = false; \
            } else { \
                CODE \
            } \
        } \
    } else { \
        CODE \
    }

Pt_minimalqtapplication::Pt_minimalqtapplication()
    : noBenchmark(false),
      window(0),
      graphicsScene(0),
      graphicsView(0),
      glWidget(0)
{
}

int Pt_minimalqtapplication::executeAll()
{
    noBenchmark = true;

    createAndShowMainWindowAndGraphicsView();
    createGraphicsScene();
    createGLWidget();
    setupGraphicsView();
    drawSomething();

    QTimer::singleShot(0, qApp, SLOT(quit()));
    return qApp->exec();
}

void Pt_minimalqtapplication::createAndShowMainWindowAndGraphicsView()
{
    MY_QBENCHMARK_ONCE(
        window = new QMainWindow();
        window->show();

        graphicsView = new QGraphicsView(window);
        window->setCentralWidget(graphicsView);

    )
}

void Pt_minimalqtapplication::createGraphicsScene()
{
    MY_QBENCHMARK_ONCE(
        graphicsScene = new QGraphicsScene(graphicsView);
        graphicsView->setScene(graphicsScene);
    )
}

void Pt_minimalqtapplication::createGLWidget()
{
    MY_QBENCHMARK_ONCE(
        QGLFormat fmt;
        // disable multisampling, is enabled by default in Qt
        fmt.setSampleBuffers(false);
        fmt.setSamples(0);

        glWidget = new QGLWidget(fmt);
    )
}

void Pt_minimalqtapplication::setupGraphicsView()
{
    MY_QBENCHMARK_ONCE(
        QSize screenSize(864, 480);

        graphicsView->setOptimizationFlag(QGraphicsView::DontSavePainterState);
        graphicsView->resize(screenSize);
        graphicsView->setFrameStyle(0);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setSceneRect(QRectF(QPointF(), screenSize));
        graphicsView->centerOn(graphicsView->sceneRect().center());
        graphicsView->setBackgroundBrush(Qt::black);
    )
}

void Pt_minimalqtapplication::drawSomething()
{
    MY_QBENCHMARK_ONCE(
        QPen pen(Qt::green);
        graphicsScene->addRect(QRect(50, 50, 100, 100), pen);
        graphicsScene->addRect(QRect(150, 150, 100, 100), pen);
    )
}

void Pt_minimalqtapplication::overallRuntime()
{
    QProcess proc;
    const QString program = QCoreApplication::applicationFilePath();
    const QStringList arguments = QStringList() << QLatin1String("--exit-immediately");

    QBENCHMARK_ONCE {
        // execute ourselve and check for success
        proc.start(program, arguments);
        QVERIFY(proc.waitForStarted());
        QVERIFY(proc.waitForFinished());
        QCOMPARE(proc.exitStatus(), QProcess::NormalExit);
        QCOMPARE(proc.exitCode(), 0);
    }
}

int main(int argc, char **argv)
{

    QApplication a(argc, argv);
    Pt_minimalqtapplication test;

    if (argc >= 2 && strcmp("--exit-immediately", argv[1]) == 0) {
        return test.executeAll();
    }

    return QTest::qExec(&test, argc, argv);
}
