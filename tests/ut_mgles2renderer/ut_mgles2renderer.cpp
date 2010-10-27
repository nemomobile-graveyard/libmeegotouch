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

#include "ut_mgles2renderer.h"

#include <MApplication>
#include <MGLES2Renderer>

#include <QtTest>

MApplication *app;
void Ut_MGLE2Renderer::initTestCase()
{
    static int argc = 1;
    static char *argv[1] = { (char *) "./ut_mgles2renderer" };
    app = new MApplication(argc, argv);
}

void Ut_MGLE2Renderer::cleanupTestCase()
{
    delete app;
}

void Ut_MGLE2Renderer::testInitActivateDestroy()
{
#ifdef M_USE_OPENGL
    QGLWidget glw1;
    QGLWidget glw2;
    QGLWidget glw3;
    
    MGLES2Renderer* r1 = NULL;
    MGLES2Renderer* r2 = NULL;
    MGLES2Renderer* r3 = NULL;

    //no renderers init yet
    QVERIFY(!MGLES2Renderer::instance());
    
    //init
    QVERIFY(r1 = MGLES2Renderer::instance(&glw1));
    QVERIFY(r2 = MGLES2Renderer::instance(const_cast<QGLContext*>(glw2.context())));
    QVERIFY(r1 != r2);
    
    //activate
    MGLES2Renderer::activate(&glw1);
    QVERIFY(MGLES2Renderer::instance() == r1);
    MGLES2Renderer::activate(const_cast<QGLContext*>(glw2.context()));
    QVERIFY(MGLES2Renderer::instance() == r2);
    MGLES2Renderer::activate(&glw3);
    QVERIFY(r3 = MGLES2Renderer::instance());
    QVERIFY(r3 != r2);
    
    //destroy
    MGLES2Renderer::destroy(&glw3);
    QVERIFY(!MGLES2Renderer::instance());
    MGLES2Renderer::destroy(const_cast<QGLContext*>(glw2.context()));
    MGLES2Renderer::activate(&glw1);
    MGLES2Renderer::destroyAll();
    QVERIFY(!MGLES2Renderer::instance());
#endif
}

void Ut_MGLE2Renderer::testShaderCompilation()
{
#ifdef M_USE_OPENGL
    QGLWidget glw;
    MGLES2Renderer::activate(&glw);
    MGLES2Renderer* r = MGLES2Renderer::instance();

    //default shader
    QGLShaderProgram* defaultProg = r->getShaderProgram();
    QVERIFY(defaultProg);

    //custom frag
    QGLShaderProgram* prog1 = r->getShaderProgram(qApp->applicationDirPath() + "/shader1.frag");
    QVERIFY(prog1);
    QVERIFY(prog1 != defaultProg);

    //custom vert
    QGLShaderProgram* prog2 = r->getShaderProgram("", qApp->applicationDirPath() + "/shader1.vert");
    QVERIFY(prog2);
    QVERIFY(prog2 != defaultProg);
    QVERIFY(prog2 != prog1);
    
    //custom frag+vert
    QGLShaderProgram* prog3 = r->getShaderProgram(qApp->applicationDirPath() + "/shader1.frag", qApp->applicationDirPath() + "/shader1.vert");
    QVERIFY(prog3);
    QVERIFY(prog3 != defaultProg);
    QVERIFY(prog3 != prog1);
    QVERIFY(prog3 != prog2);
    
    //invalid frag
    QGLShaderProgram* prog4 = r->getShaderProgram(qApp->applicationDirPath() + "/shader2.frag");
    QVERIFY(!prog4);
    
    //invalid vert
    QGLShaderProgram* prog5 = r->getShaderProgram("", qApp->applicationDirPath() + "/shader2.vert");
    QVERIFY(!prog5);

    MGLES2Renderer::destroy(&glw);
#endif
}

void Ut_MGLE2Renderer::testDrawing()
{
#ifdef M_USE_OPENGL
    QGLWidget glw;
    glw.resize(67,67);
    glw.makeCurrent();
    MGLES2Renderer::activate(&glw);
    MGLES2Renderer* r = MGLES2Renderer::instance();

    QPixmap pm(32,32);
    pm.fill(Qt::red);

    //draw with default shader
    r->setViewportSize(glw.size());
    r->begin(NULL);
    r->bindTexture(pm);
    r->draw(QRect(1,1,32,32));
    r->draw(34,1);
    r->draw(1,34,32,32);
    r->setInvertTexture(true);
    r->draw(QRect(34,34,32,32), QRect(4,4,24,24));
    r->end();
        
    //draw with custom shader
    r->begin(NULL, r->getShaderProgram(qApp->applicationDirPath() + "/shader1.frag", qApp->applicationDirPath() + "/shader1.vert"));
    r->bindTexture(pm);
    r->draw(17,17,33,33);
    r->end();
    
    //compare resulted image with reference image    
    QImage image1 = glw.grabFrameBuffer();
    //image1.save("glw1.png", "PNG");
    QImage image2(qApp->applicationDirPath() + "/glw1.png", "PNG");
    QVERIFY(image1 == image2);

    //draw patches
    glw.resize(64,64);
    QList<QRect> targets;
    targets.append(QRect(0,0,32,32));
    targets.append(QRect(32,0,32,32));
    targets.append(QRect(0,32,32,32));
    targets.append(QRect(32,32,32,32));
    QList<QRect> sources;
    sources.append(QRect(17,17,16,16));
    sources.append(QRect(32,17,16,16));
    sources.append(QRect(17,32,16,16));
    sources.append(QRect(32,32,16,16));
    pm.convertFromImage(image2);
    r->begin(NULL);
    r->bindTexture(pm);
    r->draw(targets, sources);
    r->end();

    //compare resulted image with reference image    
    image1 = glw.grabFrameBuffer();
    //image1.save("glw2.png", "PNG");
    image2.load(qApp->applicationDirPath() + "/glw2.png", "PNG");
    QVERIFY(image1 == image2);
#endif
}
QTEST_APPLESS_MAIN(Ut_MGLE2Renderer)
