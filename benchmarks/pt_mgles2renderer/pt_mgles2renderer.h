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

#ifndef Pt_MGLES2Renderer_H
#define Pt_MGLES2Renderer_H

#include <QObject>

class QGLWidget;
class MGLES2Renderer;
class QPainter;
class QGLContext;

class Pt_MGLES2Renderer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    /**
     * Benchmark the time needed to create an initialize the renderer.
     */
    void firstInstanceCall();

    void activate();

    /**
      * Renderer is already created. No time should be needed here.
      */
    void secondInstanceCall();

    void begin();

    void end();

    // execute begin and end one more time to the time difference between the first and following calls
    void begin2();

    void end2();

    void destroy();

private:
    QGLWidget *glWidget;
    QGLContext *glContext;
    QPainter *painter;
};

#endif
