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

#include "duiglwidget.h"

#ifdef DUI_USE_OPENGL
#include "duiglshader.h"
#include "duiglshaderprogram.h"
#include <X11/Xlib.h>

class DuiGLWidgetPrivate
{
public:
    DuiGLWidgetPrivate();
    ~DuiGLWidgetPrivate();

    GLfloat **m_matProj;
    GLfloat **m_matWorld;
    GLfloat *m_pixmapVertices;
    GLfloat *m_pixmapTexCoords;
    GLfloat *m_pixmapTexCoordsInv;

    void *m_oldVert;
    void *m_oldCoord;
    void *m_oldColor;
    GLint m_oldProgram;
    GLboolean m_oldDepthTest;
    GLboolean m_oldBlend;

    QVector<GLfloat> m_vertices;
    QVector<GLfloat> m_texCoords;
    QVector<GLfloat> m_color;
    QVector<GLushort> m_indices;

    ShaderCache m_shaderCache;
    ProgramCache m_programCache;
    PixmapHash m_boundPixmaps;

    DuiGLShaderProgram *m_defaultProgram;
    IDuiGLUniformProvider *m_uniformProvider;

    QSize m_viewportSize;

protected:
    DuiGLWidget *q_ptr;

private:
    Q_DECLARE_PUBLIC(DuiGLWidget)
};

DuiGLWidgetPrivate::DuiGLWidgetPrivate() :
        m_oldVert(NULL), m_oldCoord(NULL), m_oldColor(NULL), m_oldProgram(0), m_oldDepthTest(false), m_oldBlend(false), m_uniformProvider(0), q_ptr(NULL)
{
    m_matProj = new GLfloat*[4];
    m_matWorld = new GLfloat*[4];
    for (int i = 0; i < 4; ++i) {
        m_matProj[i] = new GLfloat[4];
        m_matWorld[i] = new GLfloat[4];
    }
    m_pixmapVertices = new GLfloat[8];
    m_pixmapTexCoords = new GLfloat[8];
    m_pixmapTexCoordsInv = new GLfloat[8];

    //init world matrix for the known parts
    //parts that are changeable by the transformation
    //are set when rendering in the setupGL() method
    m_matWorld[0][2] = 0.0;
    m_matWorld[1][2] = 0.0;
    m_matWorld[2][0] = 0.0;
    m_matWorld[2][1] = 0.0;
    m_matWorld[2][2] = 1.0;
    m_matWorld[2][3] = 0.0;
    m_matWorld[3][2] = 0.0;

    //init constant vertex values
    //other parts of the array are initialized when rendering
    m_pixmapVertices[0] = 0.0;
    m_pixmapVertices[1] = 0.0;
    m_pixmapVertices[2] = 0.0;
    m_pixmapVertices[7] = 0.0;

    //init texcoords for common pixmap rendering
    m_pixmapTexCoords[0] = 0.0f; m_pixmapTexCoords[1] = 1.0f;
    m_pixmapTexCoords[2] = 0.0f; m_pixmapTexCoords[3] = 0.0f;
    m_pixmapTexCoords[4] = 1.0f; m_pixmapTexCoords[5] = 0.0f;
    m_pixmapTexCoords[6] = 1.0f; m_pixmapTexCoords[7] = 1.0f;

    /* Compensate for inverted textures

       Rendering done by the window system may be y-inverted compared
       to the standard OpenGL texture representation.  More specifically:
       the X Window system uses a coordinate system where the origin is in
       the upper left; however, the GL uses a coordinate system where the
       origin is in the lower left.
     */
    m_pixmapTexCoordsInv[0] = 0.0f; m_pixmapTexCoordsInv[1] = 0.0f;
    m_pixmapTexCoordsInv[2] = 0.0f; m_pixmapTexCoordsInv[3] = 1.0f;
    m_pixmapTexCoordsInv[4] = 1.0f; m_pixmapTexCoordsInv[5] = 1.0f;
    m_pixmapTexCoordsInv[6] = 1.0f; m_pixmapTexCoordsInv[7] = 0.0f;

    //init index, vertex and texcoord arrays by default with the number
    //of elements needed for rendering scalableimage, particle rendering also
    //uses these and it will resize the array on fly if needed
    m_indices.resize(9 * 6);
    GLushort *indices = m_indices.data();
    for (GLuint i = 0; i < 9; i++) {
        indices[i*6+0] = i * 4 + 0; //x1 y1 TL
        indices[i*6+1] = i * 4 + 1; //x1 y2 BL
        indices[i*6+2] = i * 4 + 2; //x2 y2 BR
        indices[i*6+3] = i * 4 + 0; //x1 y1 TL
        indices[i*6+4] = i * 4 + 2; //x2 y2 BR
        indices[i*6+5] = i * 4 + 3; //x2 y1 TR
    }
    m_vertices.resize(9 * 4 * 2);
    m_texCoords.resize(9 * 4 * 2);

}

DuiGLWidgetPrivate::~DuiGLWidgetPrivate()
{
    for (int i = 0; i < 4; ++i) {
        delete[] m_matProj[i];
        delete[] m_matWorld[i];
    }
    delete[] m_matProj;
    delete[] m_matWorld;
    delete[] m_pixmapVertices;
    delete[] m_pixmapTexCoords;
    delete[] m_pixmapTexCoordsInv;

    ShaderCache::iterator shaderCacheEnd = m_shaderCache.end();
    for (ShaderCache::iterator i = m_shaderCache.begin(); i != shaderCacheEnd; ++i) {
        delete *i;
    }

    ProgramCache::iterator programCacheEnd = m_programCache.end();
    for (ProgramCache::iterator i = m_programCache.begin(); i != programCacheEnd; ++i) {
        delete *i;
    }
}


DuiGLWidget::DuiGLWidget(const QGLFormat &format, QWidget *parent,
                         const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(format, parent, shareWidget, f),
    d_ptr(new DuiGLWidgetPrivate)
{
    qWarning("DuiGLWidget is DEPRECATED.");

    Q_D(DuiGLWidget);

    d->q_ptr = this;
}

DuiGLWidget::~DuiGLWidget()
{
    delete d_ptr;
}

ShaderCache *DuiGLWidget::shaderCache()
{
    Q_D(DuiGLWidget);

    return &d->m_shaderCache;
}

ProgramCache *DuiGLWidget::programCache()
{
    Q_D(DuiGLWidget);

    return &d->m_programCache;
}

PixmapHash *DuiGLWidget::boundPixmaps()
{
    Q_D(DuiGLWidget);

    return &d->m_boundPixmaps;
}

DuiGLShaderProgram *DuiGLWidget::defaultProgram()
{
    Q_D(DuiGLWidget);

    return d->m_defaultProgram;
}

QVector<GLfloat>* DuiGLWidget::vertices()
{
    Q_D(DuiGLWidget);

    return &d->m_vertices;
}

QVector<GLfloat>* DuiGLWidget:: texCoords()
{
    Q_D(DuiGLWidget);

    return &d->m_texCoords;
}

QVector<GLfloat>* DuiGLWidget::color()
{
    Q_D(DuiGLWidget);

    return &d->m_color;
}

QVector<GLushort>* DuiGLWidget::indices()
{
    Q_D(DuiGLWidget);

    return &d->m_indices;
}

GLfloat **DuiGLWidget::matProj()
{
    Q_D(DuiGLWidget);

    return d->m_matProj;
}

GLfloat **DuiGLWidget::matWorld()
{
    Q_D(DuiGLWidget);

    return d->m_matWorld;
}

GLfloat *DuiGLWidget::pixmapVertices()
{
    Q_D(DuiGLWidget);

    return d->m_pixmapVertices;
}

GLfloat *DuiGLWidget::pixmapTexCoords()
{
    Q_D(DuiGLWidget);

    return d->m_pixmapTexCoords;
}

GLfloat *DuiGLWidget::pixmapTexCoordsInv()
{
    Q_D(DuiGLWidget);

    return d->m_pixmapTexCoordsInv;
}

void *DuiGLWidget::oldVert()
{
    Q_D(DuiGLWidget);

    return d->m_oldVert;
}

void *DuiGLWidget::oldCoord()
{
    Q_D(DuiGLWidget);

    return d->m_oldCoord;
}

void *DuiGLWidget::oldColor()
{
    Q_D(DuiGLWidget);

    return d->m_oldColor;
}

GLint *DuiGLWidget::oldProgram()
{
    Q_D(DuiGLWidget);

    return &d->m_oldProgram;
}

GLboolean *DuiGLWidget::oldDepthTest()
{
    Q_D(DuiGLWidget);

    return &d->m_oldDepthTest;
}

GLboolean *DuiGLWidget::oldBlend()
{
    Q_D(DuiGLWidget);

    return &d->m_oldBlend;
}

IDuiGLUniformProvider *DuiGLWidget::uniformProvider()
{
    Q_D(DuiGLWidget);

    return d->m_uniformProvider;
}

QSize *DuiGLWidget::viewportSize()
{
    Q_D(DuiGLWidget);

    return &d->m_viewportSize;
}
#else
DuiGLWidget::DuiGLWidget(const QGLFormat &format, QWidget *parent,
                         const QGLWidget *shareWidget, Qt::WindowFlags f) :
    QGLWidget(format, parent, shareWidget, f)
{
}
#endif
