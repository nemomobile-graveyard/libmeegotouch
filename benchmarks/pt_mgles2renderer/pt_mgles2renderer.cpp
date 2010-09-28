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

#include "pt_mgles2renderer.h"

#include <mbenchmark.h>
#include <MTheme>
#include <MGLES2Renderer>

#include <QtTest/QtTest>
#include <QGLWidget>

void Pt_MGLES2Renderer::initTestCase()
{
    glWidget = new QGLWidget();
    glContext = const_cast<QGLContext*>(glWidget->context());
    painter = new QPainter();
}

void Pt_MGLES2Renderer::cleanupTestCase()
{
    delete glWidget;
    delete painter;
}

void Pt_MGLES2Renderer::firstInstanceCall()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::instance(glContext);
    )
}

void Pt_MGLES2Renderer::activate()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::activate(glContext);
    )
}

void Pt_MGLES2Renderer::secondInstanceCall()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::instance();
    )
}

void Pt_MGLES2Renderer::begin()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::instance()->begin(painter);
    )
}

void Pt_MGLES2Renderer::end()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::instance()->end();
    )
}

void Pt_MGLES2Renderer::begin2()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::instance()->begin(painter);
    )
}

void Pt_MGLES2Renderer::end2()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::instance()->end();
    )
}

void Pt_MGLES2Renderer::destroy()
{
    MBENCHMARK_ONCE (
        MGLES2Renderer::destroy(glContext);
    )
}

QTEST_MAIN(Pt_MGLES2Renderer)
