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

#include "duiglrenderer.h"
#include "duiglrenderer_p.h"

#ifdef DUI_USE_OPENGL
#include <QGLContext>
#include <QGLFramebufferObject>
#include <QHash>
#include <QX11Info>
#include <QPaintEngine>
#include <EGL/egl.h>
#include <X11/Xlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>

#include "duiglwidget.h"
#include "duiglshader.h"
#include "duiglshaderprogram.h"
#include "duiglshaderuniform.h"

#include "duidebug.h"

#include "duiparticleengine.h"

static const char *DefaultVertSrc = "\n\
    attribute highp vec4 vertex;\n \
    attribute lowp  vec2 texCoord;\n \
    uniform   highp mat4 matProj;\n \
    uniform   highp mat4 matWorld;\n \
    varying   lowp  vec2 fragTexCoord;\n \
    void main(void)\n \
    {\n\
            gl_Position = matProj * matWorld * vertex;\n\
            fragTexCoord = texCoord;\n \
    }\n";

static const char *DefaultFragSrc = "\n\
    varying lowp vec2 fragTexCoord;\n\
    uniform sampler2D texture0;\n\
    uniform lowp float opacity;\n\
    void main(void)\n\
    {\n\
        //gl_FragColor = vec4(0.0,0.0,1.0,1.0);\n\
        gl_FragColor = texture2D(texture0, fragTexCoord) * opacity;\n\
    }\n";

static const char *ParticleVertSrc = "\n\
    attribute highp vec4 vertex;\n \
    attribute lowp  vec2 texCoord;\n \
    attribute lowp  vec4 color;\n \
    uniform   highp mat4 matProj;\n \
    uniform   highp mat4 matWorld;\n \
    varying   lowp  vec2 fragTexCoord;\n \
    varying   lowp  vec4 fragColor;\n \
    void main(void)\n \
    {\n\
            gl_Position = matProj * matWorld * vertex;\n\
            fragTexCoord = texCoord;\n \
            fragColor = color;\n \
    }\n";

static const char *ParticleFragSrc = "\n\
    varying lowp vec2 fragTexCoord;\n\
    varying lowp vec4 fragColor;\n\
    uniform sampler2D texture0;\n\
    void main(void)\n\
    {\n\
        gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;\n\
    }\n";

/*static const char* PatchedVertSrc = "\n\
    attribute highp vec4 vertex;\n \
    uniform   highp mat4 matProj;\n \
    uniform   highp mat4 matWorld;\n \
    void main(void)\n \
    {\n\
            gl_Position = matProj * matWorld * vertex;\n\
    }\n";
static const char* PatchedFragSrc = "\n\
    uniform lowp vec3 color;\n\
    void main(void)\n\
    {\n\
        gl_FragColor.rgb = color;\n\
    }\n";*/

static const EGLint pixmapAttribs[] = {
    EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
    EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGB,
    EGL_NONE
};

static const GLuint DUI_ATTR_VERTEX = 0;
static const GLuint DUI_ATTR_TCOORD = 1;
static const GLuint DUI_ATTR_VCOLOR = 2;

class DuiGLRendererPrivate
{
public:
    DuiGLRendererPrivate();
    virtual ~DuiGLRendererPrivate();

    void setupGL(const QTransform &transform, GLuint texid, const QSizeF &size, bool inverted);
    void setupMatrices(const QTransform &transform);
    void setupTexturing(GLuint texid);
    void setupVertices(const QSizeF &size, bool inverted);
    GLuint setupVertices(const QSizeF &texSize, const QList<QRect>& sourceRects, const QList<QRect>& targetRects);
    GLuint setupVertices(const DuiParticle *particles, int numParticles);
    void restoreGL();
    void setupUniforms(const DuiGLShaderProgram *program, const QSizeF &size, IDuiGLUniformProvider *provider = NULL);

    GLfloat m_matProj[4][4];
    GLfloat m_matWorld[4][4];

    //this are used when rendering scalableimages or particles (variable number of vertices)
    QVector<GLfloat> *m_vertices;
    QVector<GLfloat> *m_texCoords;
    QVector<GLfloat> *m_color;
    QVector<GLushort> *m_indices;

    //this are used when rendering plain pixmap (constant number of vertices)
    GLfloat m_pixmapVertices[8];
    GLfloat m_pixmapTexCoords[8];
    GLfloat m_pixmapTexCoordsInv[8];

    QPointer<DuiGLWidget> m_glWidget;

    ShaderCache *m_shaderCache;

    ProgramCache *m_programCache;

    const DuiGLShaderProgram *m_defaultProgram;

    IDuiGLUniformProvider *m_uniformProvider;

    QSize *m_viewportSize;

    static EGLConfig config;
    EGLDisplay dpy;
    PixmapHash *bound_pixmaps;

    void *m_oldVert;
    void *m_oldCoord;
    void *m_oldColor;
    //GLint m_oldTexture;
    GLint *m_oldProgram;
    GLboolean *m_oldDepthTest;
    GLboolean *m_oldBlend;
};

// Don't touch this! This is independent of QGLWidget
EGLConfig DuiGLRendererPrivate::config = 0;

DuiGLRendererPrivate::DuiGLRendererPrivate()
    : m_vertices(NULL),
    m_texCoords(NULL),
    m_color(NULL),
    m_indices(NULL),
    m_glWidget(NULL),
    m_shaderCache(NULL),
    m_programCache(NULL),
    m_defaultProgram(NULL),
    m_uniformProvider(NULL),
    m_viewportSize(0),
    bound_pixmaps(NULL),
    m_oldVert(NULL),
    m_oldCoord(NULL),
    m_oldColor(NULL),
    m_oldProgram(NULL),
    m_oldDepthTest(NULL),
    m_oldBlend(NULL)
{
    dpy = eglGetDisplay(EGLNativeDisplayType(QX11Info::display()));

    for (int i = 0; i < 8; i++) {
        m_pixmapVertices[i] = 0.0f;
        m_pixmapTexCoords[i] = 0.0f;
        m_pixmapTexCoordsInv[i] = 0.0f;
    }
}

DuiGLRendererPrivate::~DuiGLRendererPrivate()
{
}

void DuiGLRendererPrivate::setupMatrices(const QTransform &transform)
{
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
    //TODO: don't recreate the matrix always when rendering a frame
    //      recreate matrix only when the gl widget size has actually changed
    //setup/correct the projection matrix depending on the viewport size
    GLfloat w = m_viewportSize->width() == -1 ?  m_glWidget->width() :  m_viewportSize->width();
    GLfloat h = m_viewportSize->height() == -1 ? m_glWidget->height() : m_viewportSize->height();
    glViewport(0, 0, w, h);
    m_matProj[0][0] =  2.0 / w;
    m_matProj[1][1] = -2.0 / h;

    //GLfloat matWorld[4][4]  =
    //{
    //    {transform.m11(), transform.m12(), 0.0, transform.m13()},
    //    {transform.m21(), transform.m22(), 0.0, transform.m23()},
    //    {            0.0,             0.0, 1.0,             0.0},
    //    {transform.dx() + 0.5, transform.dy() + 0.5, 0.0, transform.m33()}
    //};
    //generate trasformation matrix to be given to opengl
    //const QTransform& transform = painter->combinedTransform();
    m_matWorld[0][0] = transform.m11();
    m_matWorld[0][1] = transform.m12();
    m_matWorld[0][3] = transform.m13();
    m_matWorld[1][0] = transform.m21();
    m_matWorld[1][1] = transform.m22();
    m_matWorld[1][3] = transform.m23();
    m_matWorld[3][0] = transform.dx();
    m_matWorld[3][1] = transform.dy();
    m_matWorld[3][3] = transform.m33();

    glGetBooleanv(GL_BLEND, m_oldBlend);
    glEnable(GL_BLEND);

    glGetBooleanv(GL_DEPTH_TEST, m_oldDepthTest);
    glDisable(GL_DEPTH_TEST);
}

void DuiGLRendererPrivate::setupGL(const QTransform &transform, GLuint texid, const QSizeF &size, bool inverted)
{
    setupMatrices(transform);
    setupTexturing(texid);
    setupVertices(size, inverted);
}

void DuiGLRendererPrivate::setupTexturing(GLuint texid)
{
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
    //glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_oldTexture);

    //setup texturing
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void DuiGLRendererPrivate::setupVertices(const QSizeF &size, bool inverted)
{
    //setup vertices for the widget
    m_pixmapVertices[3] = size.height();
    m_pixmapVertices[4] = size.width();
    m_pixmapVertices[5] = size.height();
    m_pixmapVertices[6] = size.width();

    glGetVertexAttribPointerv(DUI_ATTR_VERTEX, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldVert);
    glGetVertexAttribPointerv(DUI_ATTR_TCOORD, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldCoord);

    glEnableVertexAttribArray(DUI_ATTR_VERTEX);
    glVertexAttribPointer(DUI_ATTR_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, m_pixmapVertices);
    glEnableVertexAttribArray(DUI_ATTR_TCOORD);
    glVertexAttribPointer(DUI_ATTR_TCOORD, 2, GL_FLOAT, GL_FALSE, 0, inverted ? m_pixmapTexCoordsInv : m_pixmapTexCoords);
}

GLuint DuiGLRendererPrivate::setupVertices(const QSizeF &texSize, const QList<QRect>& sourceRects, const QList<QRect>& targetRects)
{
    int numRects = sourceRects.size();
    if (sourceRects.size() != targetRects.size()) {
        numRects = (sourceRects.size() < targetRects.size()) ?  sourceRects.size() : targetRects.size();
    }

    //setup vertices for the target rects
    GLfloat *vertices = m_vertices->data();
    GLfloat *texcoords = m_texCoords->data();
    GLuint numVert = 0;
    for (int i = 0; i < numRects; ++i) {
        const QRect &rc2 = targetRects.at(i);
        QRectF rc = rc2;
        GLfloat x1 = rc.left();
        GLfloat x2 = rc.right();
        GLfloat y1 = rc.top();
        GLfloat y2 = rc.bottom();

        vertices[numVert++] = x1; vertices[numVert++] = y1;
        vertices[numVert++] = x1; vertices[numVert++] = y2;
        vertices[numVert++] = x2; vertices[numVert++] = y2;
        vertices[numVert++] = x2; vertices[numVert++] = y1;
    }


    //setup texture coords for the target rects
    GLfloat dx = 1.0 / texSize.width();
    GLfloat dy = 1.0 / texSize.height();
    GLuint numCoord = 0;
    for (int i = 0; i < numRects; ++i) {
        QRectF rc = sourceRects.at(i);

        GLfloat u1 = rc.left() * dx;
        GLfloat u2 = rc.right() * dx;
        GLfloat v1 = 1.0 - rc.top() * dy;
        GLfloat v2 = 1.0 - rc.bottom() * dy;

        texcoords[numCoord++] =  u1; texcoords[numCoord++] = v1;
        texcoords[numCoord++] =  u1; texcoords[numCoord++] = v2;
        texcoords[numCoord++] =  u2; texcoords[numCoord++] = v2;
        texcoords[numCoord++] =  u2; texcoords[numCoord++] = v1;
    }

    glGetVertexAttribPointerv(DUI_ATTR_VERTEX, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldVert);
    glGetVertexAttribPointerv(DUI_ATTR_TCOORD, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldCoord);

    glEnableVertexAttribArray(DUI_ATTR_VERTEX);
    glVertexAttribPointer(DUI_ATTR_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(DUI_ATTR_TCOORD);
    glVertexAttribPointer(DUI_ATTR_TCOORD, 2, GL_FLOAT, GL_FALSE, 0, texcoords);

    return numRects * 6;
}

GLuint DuiGLRendererPrivate::setupVertices(const DuiParticle *particles, int numParticles)
{
    //resize the vertex, texcoord and color arrays if needed
    if (m_vertices->size() < numParticles * 4 * 2)
        m_vertices->resize(numParticles * 4 * 2);
    if (m_texCoords->size() < numParticles * 4 * 2)
        m_texCoords->resize(numParticles * 4 * 2);
    if (m_color->size() < numParticles * 4 * 4)
        m_color->resize(numParticles * 4 * 4);

    //fill the vertex, textcoord and color arrays
    GLfloat *vertices = m_vertices->data();
    GLfloat *texcoords = m_texCoords->data();
    GLfloat *colors = m_color->data();
    GLuint numVert = 0;
    GLuint numCoord = 0;
    GLuint numColor = 0;
    GLuint numVisibleParticles = 0;
    for (int i = 0; i < numParticles; ++i) {
        const DuiParticle &particle = particles[i];
        if (particles[i].alive) {
            ++numVisibleParticles;
            GLfloat x1 = particle.px - ((particle.sx / 2.0) * particle.scale);
            GLfloat x2 = particle.px + ((particle.sx / 2.0) * particle.scale);
            GLfloat y1 = particle.py - ((particle.sx / 2.0) * particle.scale);
            GLfloat y2 = particle.py + ((particle.sy / 2.0) * particle.scale);
            vertices[numVert++] = (GLfloat)x1; vertices[numVert++] = (GLfloat)y1;
            vertices[numVert++] = (GLfloat)x1; vertices[numVert++] = (GLfloat)y2;
            vertices[numVert++] = (GLfloat)x2; vertices[numVert++] = (GLfloat)y2;
            vertices[numVert++] = (GLfloat)x2; vertices[numVert++] = (GLfloat)y1;

            texcoords[numCoord++] =  0.0; texcoords[numCoord++] = 1.0;
            texcoords[numCoord++] =  0.0; texcoords[numCoord++] = 0.0;
            texcoords[numCoord++] =  1.0; texcoords[numCoord++] = 0.0;
            texcoords[numCoord++] =  1.0; texcoords[numCoord++] = 1.0;

            colors[numColor++] = qRed(particle.color) / 255.0; colors[numColor++] = qGreen(particle.color) / 255.0; colors[numColor++] = qBlue(particle.color) / 255.0; colors[numColor++] = qAlpha(particle.color) / 255.0;
            colors[numColor++] = qRed(particle.color) / 255.0; colors[numColor++] = qGreen(particle.color) / 255.0; colors[numColor++] = qBlue(particle.color) / 255.0; colors[numColor++] = qAlpha(particle.color) / 255.0;
            colors[numColor++] = qRed(particle.color) / 255.0; colors[numColor++] = qGreen(particle.color) / 255.0; colors[numColor++] = qBlue(particle.color) / 255.0; colors[numColor++] = qAlpha(particle.color) / 255.0;
            colors[numColor++] = qRed(particle.color) / 255.0; colors[numColor++] = qGreen(particle.color) / 255.0; colors[numColor++] = qBlue(particle.color) / 255.0; colors[numColor++] = qAlpha(particle.color) / 255.0;
        }
    }

    //reinitialize the index array if the particle count has increased over the current maximum limit
    if (m_indices->size() < (int)numVisibleParticles * 6) {
        m_indices->resize(numVisibleParticles * 6);
        GLushort *indices = m_indices->data();
        for (GLuint i = 0; i < numVisibleParticles; i++) {
            indices[i*6+0] = i * 4 + 0; //x1 y1 TL
            indices[i*6+1] = i * 4 + 1; //x1 y2 BL
            indices[i*6+2] = i * 4 + 2; //x2 y2 BR
            indices[i*6+3] = i * 4 + 0; //x1 y1 TL
            indices[i*6+4] = i * 4 + 2; //x2 y2 BR
            indices[i*6+5] = i * 4 + 3; //x2 y1 TR
        }
    }

    glGetVertexAttribPointerv(DUI_ATTR_VERTEX, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldVert);
    glGetVertexAttribPointerv(DUI_ATTR_TCOORD, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldCoord);
    glGetVertexAttribPointerv(DUI_ATTR_VCOLOR, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_oldColor);

    glEnableVertexAttribArray(DUI_ATTR_VERTEX);
    glVertexAttribPointer(DUI_ATTR_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(DUI_ATTR_TCOORD);
    glVertexAttribPointer(DUI_ATTR_TCOORD, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
    glEnableVertexAttribArray(DUI_ATTR_VCOLOR);
    glVertexAttribPointer(DUI_ATTR_VCOLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

    return numVisibleParticles * 6;
}

void DuiGLRendererPrivate::restoreGL()
{
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
    if (m_glWidget) {
        m_glWidget->paintEngine()->syncState();
    }

    //glDisable(GL_BLEND);

    //disable vertex and texture coord arrays
    //glDisableVertexAttribArray(DUI_ATTR_VERTEX);
    //glDisableVertexAttribArray(DUI_ATTR_TCOORD);
    glDisableVertexAttribArray(DUI_ATTR_VCOLOR);

    glVertexAttribPointer(DUI_ATTR_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, m_oldVert);
    glVertexAttribPointer(DUI_ATTR_TCOORD, 2, GL_FLOAT, GL_FALSE, 0, m_oldCoord);
    //glVertexAttribPointer(DUI_ATTR_VCOLOR, 4, GL_FLOAT, GL_FALSE, 0, m_oldColor);

    //activate the first texture unit so that qt still renders correctly
    //for some reason they never activate the unit after initialization.
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, m_oldTexture);

    glUseProgram(*m_oldProgram);

    if (!(*m_oldBlend))
        glDisable(GL_BLEND);

    if ((*m_oldDepthTest))
        glEnable(GL_DEPTH_TEST);

    //reset the manually set viewport size after rendering
    *m_viewportSize = QSize(-1, -1);

    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}


void DuiGLRendererPrivate::setupUniforms(const DuiGLShaderProgram *program, const QSizeF &size, IDuiGLUniformProvider *provider)
{
    //use provider given as parameter, if is is NULL then use global provider
    //previously set with DuiGLRenderer::setUniformProvider method.
    IDuiGLUniformProvider *usedProvider = provider ? provider : m_uniformProvider;
    if (usedProvider) {
        const DuiGLUniformList &uniforms = program->uniforms();
        DuiGLUniformList::const_iterator uniformsEnd = uniforms.constEnd();

        for (DuiGLUniformList::const_iterator i = uniforms.begin(); i != uniformsEnd; ++i) {
            //set the default values and give rest to the provider
            QString uniformId = i.key();
            if (uniformId == "texture0")
                i.value() = (GLuint)0;
            else if (uniformId == "matWorld")
                i.value() = m_matWorld;
            else if (uniformId == "matProj")
                i.value() = m_matProj;
            else if (uniformId == "invSize")
                i.value() = QSizeF(1.0f / size.width(), 1.0f / size.height());
            else {
                usedProvider->setUniformValue(uniformId, *i);
            }
        }
    } else {
        //just set the default values if found
        const DuiGLUniformList &uniforms = program->uniforms();
        const DuiGLUniformList::const_iterator end = uniforms.constEnd();
        DuiGLUniformList::const_iterator i = uniforms.constFind("texture0");
        if (i != end)
            program->uniform("texture0") = (GLuint)0;
        i = uniforms.constFind("matWorld");
        if (i != end)
            program->uniform("matWorld") = m_matWorld;
        i = uniforms.constFind("matProj");
        if (i != end)
            program->uniform("matProj") = m_matProj;
        i = uniforms.constFind("invSize");
        if (i != end)
            program->uniform("invSize") = QSizeF(1.0f / size.width(), 1.0f / size.height());
    }
}


DuiGLRenderer::DuiGLRenderer()
    : d_ptr(new DuiGLRendererPrivate())
{
    qWarning("DuiGLRenderer is DEPRECATED. Use DuiGLES2Renderer instead.");
}

DuiGLRenderer::~DuiGLRenderer()
{
    delete d_ptr;
}
#else

DuiGLRenderer::DuiGLRenderer()
    : d_ptr(NULL)
{
    qWarning("DuiGLRenderer is DEPRECATED. Use DuiGLES2Renderer instead.");
}

DuiGLRenderer::~DuiGLRenderer()
{
}

#endif

DuiGLRenderer *DuiGLRenderer::instance()
{
    //qWarning("DuiGLRenderer is DEPRECATED. Use DuiGLES2Renderer instead.");

    static DuiGLRenderer duiGLRenderer;
    return &duiGLRenderer;
}

#ifdef DUI_USE_OPENGL
void DuiGLRenderer::initGL(DuiGLWidget *glWidget)
{
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
//    d_ptr->dpy = eglGetDisplay(EGLNativeDisplayType(QX11Info::display()));
//    d_ptr->m_glWidget = glWidget;

    //init the orthogonal projection matrix
    //glOrtho(0, w, h, 0, -1, 1):
    //2.0/w,  0.0,    0.0, -1.0
    //0.0,   -2.0/h,  0.0,  1.0
    //0.0,    0.0,   -1.0,  0.0
    //0.0,    0.0,    0.0,  1.0
    GLfloat w = glWidget->width();
    GLfloat h = glWidget->height();
    d_ptr->m_matProj[0][0] =  2.0 / w; d_ptr->m_matProj[1][0] =  0.0;   d_ptr->m_matProj[2][0] =  0.0; d_ptr->m_matProj[3][0] = -1.0;
    d_ptr->m_matProj[0][1] =  0.0;   d_ptr->m_matProj[1][1] = -2.0 / h; d_ptr->m_matProj[2][1] =  0.0; d_ptr->m_matProj[3][1] =  1.0;
    d_ptr->m_matProj[0][2] =  0.0;   d_ptr->m_matProj[1][2] =  0.0;   d_ptr->m_matProj[2][2] = -1.0; d_ptr->m_matProj[3][2] =  0.0;
    d_ptr->m_matProj[0][3] =  0.0;   d_ptr->m_matProj[1][3] =  0.0;   d_ptr->m_matProj[2][3] =  0.0; d_ptr->m_matProj[3][3] =  1.0;

    //TODO: Create some other default shaders here as well?

    //from file
    //createShader("DefaultVert", DuiGLShader::VertexShader);
    //createShader("DefaultFrag", DuiGLShader::FragmentShader);

    //from src
    createShader("DefaultVert", DefaultVertSrc, DuiShaderVertex);
    createShader("DefaultFrag", DefaultFragSrc, DuiShaderFragment);


    createShader("ParticleVert", ParticleVertSrc, DuiShaderVertex);
    createShader("ParticleFrag", ParticleFragSrc, DuiShaderFragment);

    d_ptr->m_defaultProgram = createProgram("Default", "DefaultVert", "DefaultFrag");
    createProgram("Particle", "ParticleVert", "ParticleFrag");

    //TODO: Future optimizations, uncomment these if/when the center of the
    //      scalableimage is rendered with solid color.
    /*createShader("PatchedVert", PatchedVertSrc, DuiShaderVertex);
    createShader("PatchedFrag", PatchedFragSrc, DuiShaderFragment);
    createProgram("Patched", "PatchedVert", "PatchedFrag");*/
}

DuiGLWidget *DuiGLRenderer::GLWidget() const
{
    return d_ptr->m_glWidget;
}

void DuiGLRenderer::setGLWidget(DuiGLWidget *glWidget)
{
    if (d_ptr->m_glWidget != glWidget) {
        if (glWidget) {
            GLfloat **mp = glWidget->matProj();
            GLfloat **mw = glWidget->matWorld();
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) {
                    d_ptr->m_matProj[i][j] = mp[i][j];
                    d_ptr->m_matWorld[i][j] = mw[i][j];
                }
            GLfloat *pv = glWidget->pixmapVertices();
            GLfloat *ptc = glWidget->pixmapTexCoords();
            GLfloat *ptci = glWidget->pixmapTexCoordsInv();
            for (int i = 0; i < 8; ++i) {
                d_ptr->m_pixmapVertices[i] = pv[i];
                d_ptr->m_pixmapTexCoords[i] = ptc[i];
                d_ptr->m_pixmapTexCoordsInv[i] = ptci[i];
            }
            d_ptr->m_vertices = glWidget->vertices();
            d_ptr->m_texCoords = glWidget->texCoords();
            d_ptr->m_color = glWidget->color();
            d_ptr->m_indices = glWidget->indices();
            d_ptr->m_shaderCache = glWidget->shaderCache();
            d_ptr->m_programCache = glWidget->programCache();
            d_ptr->m_defaultProgram = glWidget->defaultProgram();
            d_ptr->m_uniformProvider = glWidget->uniformProvider();
            d_ptr->m_viewportSize = glWidget->viewportSize();
            d_ptr->bound_pixmaps = glWidget->boundPixmaps();
            d_ptr->m_oldVert = glWidget->oldVert();
            d_ptr->m_oldCoord = glWidget->oldCoord();
            d_ptr->m_oldColor = glWidget->oldColor();
            d_ptr->m_oldProgram = glWidget->oldProgram();
            d_ptr->m_oldDepthTest = glWidget->oldDepthTest();
            d_ptr->m_oldBlend = glWidget->oldBlend();
            glWidget->makeCurrent();
        } else {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) {
                    d_ptr->m_matProj[i][j] = 0;
                    d_ptr->m_matWorld[i][j] = 0;
                }
            d_ptr->m_vertices = 0;
            d_ptr->m_texCoords = 0;
            d_ptr->m_color = 0;
            d_ptr->m_indices = 0;
            d_ptr->m_shaderCache = 0;
            d_ptr->m_programCache = 0;
            d_ptr->m_defaultProgram = 0;
            d_ptr->m_uniformProvider = 0;
            d_ptr->m_viewportSize = 0;
            d_ptr->bound_pixmaps = 0;
            d_ptr->m_oldVert = 0;
            d_ptr->m_oldCoord = 0;
            d_ptr->m_oldColor = 0;
            d_ptr->m_oldProgram = 0;
            d_ptr->m_oldDepthTest = 0;
            d_ptr->m_oldBlend = 0;
        }
        d_ptr->m_glWidget = glWidget;
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
}

const DuiGLShader *DuiGLRenderer::createShader(const QString &filename, DuiShaderType type)
{
    //open the shader file for reading
    QFile shaderFile(filename);
    if (!shaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        duiWarning("DuiGLRenderer") << "createShader(" << filename << ") could not open file.";
        return NULL;
    }

    //read the source from the file
    QTextStream stream(&shaderFile);
    QString source = stream.readAll();
    if (source.length() == 0) {
        duiWarning("DuiGLRenderer") << "createShader(" << filename << ") file does not contain data.";
        return NULL;
    }

    //try to create a shader from the source
    QFileInfo fileInfo(shaderFile);
    QString key = fileInfo.fileName();

    const DuiGLShader *shader = createShader(key, source, type);;
    if (!shader) {
        //TODO Try to load shader binary here
    }

    return shader;
}

const DuiGLShader *DuiGLRenderer::createShader(const QString &key, const QString &source, DuiShaderType type)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "createShader()" << "DuiGLRenderer not initialized yet.";
        return NULL;
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();

    QGLContext *ctx = (QGLContext *)d_ptr->m_glWidget->context();
    DuiGLShader *shader = NULL;
    if (type == DuiShaderUndefined) {
        //TODO: this kind of "autodetection" works only when linking programs with invalid shaders.
        //      Is this kind of automatic shader detection needed? It could be be achieved by detecting
        //      some special keywords from shader sources.
        /*duiDebug("DuiGLRenderer") << "DuiGLRenderer::createShader() Auto detecting shader type.";
        shader = new DuiGLShader(DuiGLShader::VertexShader, ctx);
        shader->addSource(source.toLatin1());
        if( !shader->compile() )
        {
            duiWarning("DuiGLRenderer) << "createShader(" << key << "):" << " shader failed to compile:" << shader->log();
            delete shader;
            shader = new DuiGLShader(DuiGLShader::FragmentShader, ctx);
            shader->addSource(source.toLatin1());
            if( !shader->compile() )
            {
                duiWarning("DuiGLRenderer) << "createShader(" << key << "):" << " shader failed to compile:" << shader->log();
                delete shader;
                return NULL;
            }
        }*/
        return NULL;
    } else {
        shader = new DuiGLShader((DuiGLShader::ShaderType)type, key, ctx);
        if (shader->id() > 0) {
            shader->addSource(source/*.toLatin1()*/);
            if (!shader->compile()) {
                duiWarning("DuiGLRenderer") << "createShader(" << key << "):" << " shader failed to compile:" << shader->log();
                delete shader;
                return NULL;
            }
        } else {
            duiWarning("DuiGLRenderer") << "Shader creation failed.";
            delete shader;
            return NULL;
        }
    }
    d_ptr->m_shaderCache->insert(key, shader);
    return shader;
}

const DuiGLShader *DuiGLRenderer::getShader(const QString &key)
{
    if (d_ptr->m_shaderCache->contains(key)) {
        return (*d_ptr->m_shaderCache)[key];
    }
    return NULL;
}

const DuiGLShaderProgram *DuiGLRenderer::createProgram(const QString &key, const QList<QString>& shaderKeyList)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "createProgram()" << "DuiGLRenderer not initialized yet.";
        return NULL;
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();

    QGLContext *ctx = (QGLContext *)d_ptr->m_glWidget->context();

    //create the shader program and add the wanted shaders into it
    bool vert = false;
    bool frag = false;
    DuiGLShaderProgram *program = new DuiGLShaderProgram(ctx);

    QList<QString>::const_iterator shaderKeyListEnd = shaderKeyList.constEnd();

    for (QList<QString>::const_iterator i = shaderKeyList.begin(); i != shaderKeyListEnd; ++i) {
        //fetch precompiled shader from cache and add it to the program
        const DuiGLShader *shader = getShader(*i);
        if (shader) {
            program->addShader(shader);
            if (shader->type() ==  DuiGLShader::VertexShader)
                vert = true;
            else if (shader->type() ==  DuiGLShader::FragmentShader)
                frag = true;
        } else {
            duiWarning("DuiGLRenderer") << "createProgram() Ignoring shader " << *i;
        }
    }

    //add default frag or vert shaders if missing from shader list
    if (!vert) {
        duiWarning("DuiGLRenderer") << "createProgram() Vertex shader missing, adding default one.";
        if (!getShader("DefaultVert")) {
            duiWarning("DuiGLRenderer") << "createProgram() Default shader does not exist.";
            delete program;
            return NULL;
        }
        program->addShader(getShader("DefaultVert"));
    }
    if (!frag) {
        duiWarning("DuiGLRenderer") << "createProgram() Fragment shader missing, adding default one.";
        if (!getShader("DefaultFrag")) {
            duiWarning("DuiGLRenderer") << "createProgram() Default shader does not exist.";
            delete program;
            return NULL;
        }
        program->addShader(getShader("DefaultFrag"));
    }

    glBindAttribLocation(program->id(), DUI_ATTR_VERTEX, "vertex");
    glBindAttribLocation(program->id(), DUI_ATTR_TCOORD, "texCoord");
    glBindAttribLocation(program->id(), DUI_ATTR_VCOLOR, "color");

    //link program
    if (!program->link()) {
        duiWarning("DuiGLRenderer") << "createProgram(" << key << ", " << shaderKeyList << "):" << " failed to link:" << program->log();
        delete program;
        return NULL;
    }

    d_ptr->m_programCache->insert(key, program);
    return program;
}

const DuiGLShaderProgram *DuiGLRenderer::createProgram(const QString &key, const QString &vertShader, const QString &fragShader)
{
    QList<QString> shaders;
    shaders.push_back(vertShader);
    shaders.push_back(fragShader);
    return createProgram(key, shaders);
}

//DuiGLShaderProgram* DuiGLRenderer::createProgramFromFiles(const QString& key, const QList<QString>& shaderFileList)
//{
//}

const DuiGLShaderProgram *DuiGLRenderer::createProgramFromFiles(const QString &key, const QString &fragShaderFile, const QString &vertShaderFile)
{
    const DuiGLShader *frag = createShader(fragShaderFile, DuiShaderFragment);
    if (!frag) {
        frag = getShader("DefaultFrag");
    }

    const DuiGLShader *vert = createShader(vertShaderFile, DuiShaderVertex);
    if (!vert) {
        vert = getShader("DefaultVert");
    }

    return createProgram(key, frag->name(), vert->name());
}

const DuiGLShaderProgram *DuiGLRenderer::getProgram(const QString &key)
{
    if (d_ptr->m_programCache->contains(key)) {
        return (*d_ptr->m_programCache)[key];
    }

    return NULL;
}


quint32 DuiGLRenderer::bindX11Pixmap(Qt::HANDLE pixmap)
{
    EGLSurface eglpixmap = 0;

    // If surface does not yet exist, initialize and find appropriates EGL surface
    if (!d_ptr->config) {
        EGLint ecfgs = 0;
        EGLConfig configs[20];

        static const EGLint pixmap_config[] = {
            EGL_SURFACE_TYPE,       EGL_PIXMAP_BIT,
            EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
            EGL_DEPTH_SIZE,         0,
            EGL_BIND_TO_TEXTURE_RGB, EGL_TRUE,
            EGL_NONE
        };

        if (!eglChooseConfig(d_ptr->dpy, pixmap_config, configs,
                             20, &ecfgs) || !ecfgs) {
            duiWarning("DuiGLRenderer") << "No appropriate EGL configuration for texture from pixmap!";
            return 0;
        }
        for (EGLint i = 0; i < ecfgs; ++i) {
            eglpixmap = eglCreatePixmapSurface(d_ptr->dpy, configs[i],
                                               (EGLNativePixmapType) pixmap,
                                               pixmapAttribs);
            if (eglpixmap == EGL_NO_SURFACE)
                continue;
            else {
                d_ptr->config = configs[i];
                break;
            }
        }
    }

    // Returns the cached texture if we have one already
    TexturePixmap tp = d_ptr->bound_pixmaps->value(pixmap);
    if (tp.texture > 0)
        return tp.texture;

    // .. or create a new one
    if (!eglpixmap)
        eglpixmap = eglCreatePixmapSurface(d_ptr->dpy, d_ptr->config,
                                           (EGLNativePixmapType) pixmap,
                                           pixmapAttribs);
    if (eglpixmap == EGL_NO_SURFACE) {
        // Guys! do not put duiWarning here, it can't understand HEX!
        qWarning("Cannot create EGL surface: 0x%x", eglGetError());
        return 0;
    }
    tp.eglpixmap = eglpixmap;

    glGenTextures(1, &tp.texture);
    (*d_ptr->bound_pixmaps)[pixmap] = tp;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tp.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return tp.texture;
}

void DuiGLRenderer::unbindX11Pixmap(Qt::HANDLE pixmap)
{
    TexturePixmap tp = d_ptr->bound_pixmaps->value(pixmap);
    if (!tp.texture)
        return;

    // free the pixmap and texture cache
    eglDestroySurface(d_ptr->dpy, tp.eglpixmap);
    tp.eglpixmap =  EGL_NO_SURFACE;
    //XSync(QX11Info::display(), FALSE);
    glDeleteTextures(1, &tp.texture);
    d_ptr->bound_pixmaps->remove(pixmap);
}

void DuiGLRenderer::updateX11Pixmap(Qt::HANDLE pixmap)
{
    // catch updates to the pixmap here when they happen
    TexturePixmap tp = d_ptr->bound_pixmaps->value(pixmap);
    if (!tp.texture)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tp.texture);

    if (eglReleaseTexImage(d_ptr->dpy, tp.eglpixmap,
                           EGL_BACK_BUFFER) == EGL_FALSE)
        duiWarning("DuiGLRenderer: Update pixmap, cant release bound texture");
    if (eglBindTexImage(d_ptr->dpy, tp.eglpixmap,
                        EGL_BACK_BUFFER) == EGL_FALSE)
        // Guys! do not put duiWarning here, it can't understand HEX!
        qWarning("DuiGLRenderer: Update pixmap, can't bind EGL texture to pixmap: 0x%x", eglGetError());
}

void DuiGLRenderer::drawTexture(const QTransform &transform, quint32 texId, const QSizeF &size, qreal opacity,
                                bool inverted)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "DuiGLRenderer::drawTexture()" << "DuiGLRenderer not initialized yet.";
        return;
    }
    glGetIntegerv(GL_CURRENT_PROGRAM, d_ptr->m_oldProgram);

    d_ptr->m_defaultProgram->use();

    d_ptr->setupGL(transform, texId, size, inverted);

    d_ptr->m_defaultProgram->uniform("texture0") = (GLuint)0;
    d_ptr->m_defaultProgram->uniform("matWorld") = d_ptr->m_matWorld;
    d_ptr->m_defaultProgram->uniform("matProj") = d_ptr->m_matProj;

    GLfloat o = opacity;
    d_ptr->m_defaultProgram->uniform("opacity") = o;

    //render
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    d_ptr->restoreGL();
}

void DuiGLRenderer::drawTexture(const DuiGLShaderProgram *program, const QTransform &transform, quint32 texId,
                                const QSizeF &size, IDuiGLUniformProvider *provider,
                                bool inverted)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "drawTexture()" << "DuiGLRenderer not initialized yet.";
        return;
    }

    //program->use();
    d_ptr->setupGL(transform, texId, size, inverted);

    d_ptr->setupUniforms(program, size, provider);

    //render
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    d_ptr->restoreGL();
}

void DuiGLRenderer::drawTexture(const QString &programId, const QTransform &transform, quint32 texId,
                                const QSizeF &size, IDuiGLUniformProvider *provider,
                                bool inverted)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "drawTexture()" << "DuiGLRenderer not initialized yet.";
        return;
    }

//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
    d_ptr->m_glWidget->makeCurrent();

    const DuiGLShaderProgram *program = getProgram(programId);
    if (!program) {
        duiWarning("DuiGLRenderer") << "drawPixmap(): Invalid program" << programId;
        return;
    }

    glGetIntegerv(GL_CURRENT_PROGRAM, d_ptr->m_oldProgram);

    program->use();

    d_ptr->setupGL(transform, texId, size, inverted);
    d_ptr->setupUniforms(program, size, provider);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    d_ptr->restoreGL();
}

void DuiGLRenderer::drawPixmap(const QTransform &transform, const QPixmap &pixmap, qreal opacity)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "drawPixmap()" << "DuiGLRenderer not initialized yet.";
        return;
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();

    GLuint tex = d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::PremultipliedAlphaBindOption);
    drawTexture(transform, tex, pixmap.size(), opacity);
}

void DuiGLRenderer::drawPatchedPixmap(const QTransform &transform, const QPixmap &pixmap, qreal opacity,
                                      const QList<QRect>& sourceRects, const QList<QRect>& targetRects)
{
    if (!d_ptr->m_glWidget) {

        duiWarning("DuiGLRenderer") << "drawPatchedPixmap()" << "DuiGLRenderer not initialized yet.";
        return;
    }

    d_ptr->m_glWidget->makeCurrent();
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();
    GLuint tex = d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::PremultipliedAlphaBindOption);

    glGetIntegerv(GL_CURRENT_PROGRAM, d_ptr->m_oldProgram);
    d_ptr->m_defaultProgram->use();

    d_ptr->setupMatrices(transform);
    d_ptr->setupTexturing(tex);
    GLuint num = d_ptr->setupVertices(pixmap.size(), sourceRects, targetRects);

    d_ptr->m_defaultProgram->uniform("texture0") = (GLuint)0;
    d_ptr->m_defaultProgram->uniform("matWorld") = d_ptr->m_matWorld;
    d_ptr->m_defaultProgram->uniform("matProj") = d_ptr->m_matProj;

    GLfloat o = opacity;
    d_ptr->m_defaultProgram->uniform("opacity") = o;

    glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_SHORT, d_ptr->m_indices->data());

    //TODO: Future optimizations, uncomment these when the graphics are updated
    //      to support solid center patch.
    //draw corners and edges with blending and draw center without blending
    /*glDrawElements(GL_TRIANGLES, num-6, GL_UNSIGNED_SHORT, d_ptr->m_indices.data());
    glDisable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ZERO);

    //draw center without texture lookups with single color
    //DuiGLVec3 color2 = {0.5,0.5,0.5};
    //const DuiGLShaderProgram* prg2 = getProgram("Patched");
    //glDisableVertexAttribArray(1);
    //prg2->use();
    //prg2->uniforms()["matWorld"] = d_ptr->m_matWorld;
    //prg2->uniforms()["matProj"] = d_ptr->m_matProj;
    //prg2->uniforms()["color"] = color2;

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &d_ptr->m_indices.data()[num-6]);*/

    d_ptr->restoreGL();
}

void DuiGLRenderer::drawPixmap(const DuiGLShaderProgram *program, const QTransform &transform,
                               const QPixmap &pixmap, IDuiGLUniformProvider *provider)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "drawPixmap()" << "DuiGLRenderer not initialized yet.";
        return;
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();

    GLuint tex = d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::PremultipliedAlphaBindOption);
    drawTexture(program, transform, tex, pixmap.size(), provider);
}

void DuiGLRenderer::drawPixmap(const QString &programId, const QTransform &transform,
                               const QPixmap &pixmap, IDuiGLUniformProvider *provider)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "drawPixmap()" << "DuiGLRenderer not initialized yet.";
        return;
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();

    GLuint tex = d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::PremultipliedAlphaBindOption);
    drawTexture(programId, transform, tex, pixmap.size(), provider);
}

void DuiGLRenderer::drawParticles(const DuiParticle *particles, int numParticles, const QPixmap &pixmap, const QTransform &transform)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "renderParticles()" << "DuiGLRenderer not initialized yet.";
        return;
    }

    d_ptr->m_glWidget->makeCurrent();
    GLuint tex = d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::PremultipliedAlphaBindOption);

    glGetIntegerv(GL_CURRENT_PROGRAM, d_ptr->m_oldProgram);
    const DuiGLShaderProgram *prg = getProgram("Particle");
    prg->use();

    d_ptr->setupMatrices(transform);
    d_ptr->setupTexturing(tex);
    GLuint num = d_ptr->setupVertices(particles, numParticles);

    prg->uniform("texture0") = (GLuint)0;
    prg->uniform("matWorld") = d_ptr->m_matWorld;
    prg->uniform("matProj") = d_ptr->m_matProj;

    GLint srca;
    GLint dsta;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &srca);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &dsta);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_SHORT, d_ptr->m_indices->data());

    glBlendFunc(srca, dsta);

    d_ptr->restoreGL();
}

void DuiGLRenderer::setUniformProvider(IDuiGLUniformProvider *provider)
{
    d_ptr->m_uniformProvider = provider;
}

QGLWidget *DuiGLRenderer::viewport()
{
    return d_ptr->m_glWidget;
}

void DuiGLRenderer::setViewportSize(const QSize &size)
{
    *d_ptr->m_viewportSize = size;
}

QPixmap DuiGLRenderer::composite(const DuiCompositionLayerList &layerList)
{
    if (!d_ptr->m_glWidget) {
        duiWarning("DuiGLRenderer") << "composite()" << "DuiGLRenderer not initialized yet.";
        return QPixmap();
    }
//    qDebug() << __FUNCTION__ << (quint32) QGLContext::currentContext();

    //resolve the destination pixmap size
    QSize dstSize(0, 0);
    const int layerListSize = layerList.size();
    for (int i = 0; i < layerListSize; i++) {
        const QSize pmSize = layerList[i].pixmap().size();
        if (pmSize.width() > dstSize.width())
            dstSize.setWidth(pmSize.width());
        if (pmSize.height() > dstSize.height())
            dstSize.setHeight(pmSize.height());
    }

    d_ptr->m_glWidget->makeCurrent();

    //QGLContext* ctx = (QGLContext*)d_ptr->m_glWidget->context();
    //ctx->makeCurrent();
    QGLFramebufferObject fbo(dstSize, QGLFramebufferObject::CombinedDepthStencil);
    fbo.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < layerListSize; ++i) {
        const DuiCompositionLayer::DuiCompositionMode mode = layerList[i].mode();
        if (mode == DuiCompositionLayer::Normal) {
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        } else if (mode == DuiCompositionLayer::Multiply) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        setViewportSize(dstSize);
        drawPixmap(QTransform(),  layerList[i].pixmap(), layerList[i].opacity());
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fbo.release();
    return QPixmap::fromImage(fbo.toImage());
}

#else

void DuiGLRenderer::initGL(DuiGLWidget *)
{
}

const DuiGLShader *DuiGLRenderer::createShader(const QString &, DuiShaderType)
{
    return NULL;
}

const DuiGLShader *DuiGLRenderer::createShader(const QString &, const QString &, DuiShaderType)
{
    return NULL;
}

const DuiGLShader *DuiGLRenderer::getShader(const QString &)
{
    return NULL;
}

const DuiGLShaderProgram *DuiGLRenderer::createProgram(const QString &, const QList<QString>&)
{
    return NULL;
}

const DuiGLShaderProgram *DuiGLRenderer::createProgram(const QString &, const QString &, const QString &)
{
    return NULL;
}

const DuiGLShaderProgram *DuiGLRenderer::createProgramFromFiles(const QString &, const QString &, const QString &)
{
    return NULL;
}

const DuiGLShaderProgram *DuiGLRenderer::getProgram(const QString &)
{
    return NULL;
}

quint32 DuiGLRenderer::bindX11Pixmap(Qt::HANDLE)
{
    return 0;
}

void DuiGLRenderer::unbindX11Pixmap(Qt::HANDLE)
{
}

void DuiGLRenderer::updateX11Pixmap(Qt::HANDLE)
{
}

void DuiGLRenderer::drawTexture(const QTransform &, quint32, const QSizeF &, qreal, bool)
{
}

void DuiGLRenderer::drawTexture(const DuiGLShaderProgram *, const QTransform &, quint32,
                                const QSizeF &, IDuiGLUniformProvider *,
                                bool)
{
}

void DuiGLRenderer::drawTexture(const QString &, const QTransform &, quint32,
                                const QSizeF &, IDuiGLUniformProvider *,
                                bool)
{
}

void DuiGLRenderer::drawPixmap(const QTransform &, const QPixmap &, qreal)
{
}

void DuiGLRenderer::drawPatchedPixmap(const QTransform &, const QPixmap &, qreal,
                                      const QList<QRect>&, const QList<QRect>&)
{
}

void DuiGLRenderer::drawPixmap(const DuiGLShaderProgram *, const QTransform &,
                               const QPixmap &, IDuiGLUniformProvider *)
{
}

void DuiGLRenderer::drawPixmap(const QString &, const QTransform &,
                               const QPixmap &, IDuiGLUniformProvider *)
{
}

void DuiGLRenderer::setUniformProvider(IDuiGLUniformProvider *)
{
}

QGLWidget *DuiGLRenderer::viewport()
{
    return NULL;
}

void DuiGLRenderer::setViewportSize(const QSize &)
{
}

QPixmap DuiGLRenderer::composite(const DuiCompositionLayerList &)
{
    return QPixmap();
}



#endif
