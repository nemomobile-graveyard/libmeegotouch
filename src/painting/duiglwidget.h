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

#ifndef DUIGLWIDGET_H
#define DUIGLWIDGET_H

#include <QGLWidget>
#include "duiglrenderer.h"

class QGLFormat;

#ifdef DUI_USE_OPENGL
#   include <QHash>
#   include <EGL/egl.h>

class DuiGLWidgetPrivate;
class DuiGLShader;
class DuiGLShaderProgram;
class IDuiGLUniformProvider;

struct TexturePixmap {
    TexturePixmap(): eglpixmap(0), texture(0) {}
    EGLSurface eglpixmap;
    quint32   texture;
};

typedef QHash<QString, DuiGLShader *> ShaderCache;
typedef QHash<QString, DuiGLShaderProgram *> ProgramCache;
typedef QHash<Qt::HANDLE, TexturePixmap> PixmapHash;
#endif // DUI_USE_OPENGL

/*
 * This class represents the painting surface of DuiWindow used by
 * DuiGLRenderer. Apart from QGLWidget, it caches shaders, matrices,
 * pixmaps, vertices and other data generated in DuiGLRenderer::initGL()
 * method. When switching between painting, the cached data is loaded
 * to DuiGLRenderer so it doesn't have to recreate it.
 *
 * \sa DuiGLRenderer
 *
 * \deprecated since 0.16
 */
class DUI_EXPORT DuiGLWidget : public QGLWidget
{
public:
    DuiGLWidget(const QGLFormat &format, QWidget *parent = 0,
                const QGLWidget *shareWidget = 0, Qt::WindowFlags f = 0);
#ifdef DUI_USE_OPENGL
    ~DuiGLWidget();

    ShaderCache *shaderCache();
    ProgramCache *programCache();
    PixmapHash *boundPixmaps();

    DuiGLShaderProgram *defaultProgram();

    QVector<GLfloat>* vertices();
    QVector<GLfloat>* texCoords();
    QVector<GLfloat>* color();
    QVector<GLushort>* indices();

    GLfloat **matProj();
    GLfloat **matWorld();
    GLfloat *pixmapVertices();
    GLfloat *pixmapTexCoords();
    GLfloat *pixmapTexCoordsInv();

    void *oldVert();
    void *oldCoord();
    void *oldColor();
    GLint *oldProgram();
    GLboolean *oldDepthTest();
    GLboolean *oldBlend();

    IDuiGLUniformProvider *uniformProvider();

    QSize *viewportSize();
protected:
    DuiGLWidgetPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiGLWidget)
    Q_DECLARE_PRIVATE(DuiGLWidget)
#endif // DUI_USE_OPENGL
};

#endif // DUIGLWIDGET_H
