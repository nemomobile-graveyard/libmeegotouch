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

#ifndef PT_MINIMALQTAPPLICATION_H
#define PT_MINIMALQTAPPLICATION_H

#include <QObject>

class QMainWindow;
class QGraphicsScene;
class QGraphicsView;
class QGLWidget;

/**
  * Benchmark runtime of a simple Qt application. It mimics a minimal M application and therefor
  * creates QGraphicsView/QGraphicsscene and initializes them.
  * All elements are benchmarked individually and the whole application is benchmarked at once.
 */
class Pt_minimalqtapplication : public QObject
{
    Q_OBJECT
public:
    Pt_minimalqtapplication();

    /**
      * Execute a whole simple Qt application by calling the slots from below.
      */
    int executeAll();

private slots:
    void overallRuntime();
    void createAndShowMainWindowAndGraphicsView();
    void createGraphicsScene();
    void createGLWidget();
    void setupGraphicsView();
    void drawSomething();

private:
    bool noBenchmark;

    QMainWindow *window;
    QGraphicsScene *graphicsScene;
    QGraphicsView *graphicsView;
    QGLWidget *glWidget;
};

#endif // PT_MINIMALQTAPPLICATION_H
