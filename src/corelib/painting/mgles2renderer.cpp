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

#include "mgles2renderer.h"

#ifdef M_USE_OPENGL
#include <QGLContext>
#include <QGLFramebufferObject>
#include <QHash>
#include <qglshaderprogram.h>
#include <QX11Info>
#include <QPaintEngine>
#include <QPair>
#include <EGL/egl.h>
#include <X11/Xlib.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>

#include "mdebug.h"

namespace
{

    typedef QPair<QString, QString> MGLProgramKey;
    typedef QHash<MGLProgramKey, QGLShaderProgram *> MGLProgramCache;

    static const EGLint pixmapAttribs[] = {
        EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
        EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGB,
        EGL_NONE
    };

//M specific attribute array indices
    static const GLuint M_ATTR_VERTEX = 0;
    static const GLuint M_ATTR_TCOORD = 1;
    static const GLuint M_ATTR_VCOLOR = 2;

    static QString defaultVertexShader()
    {
        return QString::fromLatin1(M_SHADER_SOURCE_DIR "/default.vert");
    }

    static QString defaultFragmentShader()
    {
        return QString::fromLatin1(M_SHADER_SOURCE_DIR "/default.frag");
    }

    static QString canonicalFilename(const QString &filename)
    {
        if (filename.isEmpty())
            return QString();
        else
            return QFileInfo(filename).canonicalFilePath();
    }

} // anonymous namespace

class MGLES2RendererPrivate
{
public:
    MGLES2RendererPrivate();
    virtual ~MGLES2RendererPrivate();

    GLuint setupVertices(const QSizeF &texSize, const QList<QRect>& sourceRects, const QList<QRect>& targetRects);
    void resizeAttrArrays(int numQuads);
    QGLShaderProgram *requestBinaryProgram(const QString &frag, const QString &vert);

    GLfloat m_matProj[4][4];
    GLfloat m_matWorld[4][4];

    QVector<GLfloat> m_vertices;
    QVector<GLfloat> m_texCoords;
    QVector<GLfloat> m_color;
    QVector<GLushort> m_indices;

    QGLWidget *m_glWidget;

    MGLProgramCache m_programCache;
    QGLShaderProgram *m_defaultProgram;

    QSize m_viewportSize;

    struct TexturePixmap {
        TexturePixmap(): eglpixmap(0), texture(0) {}
        EGLSurface eglpixmap;
        quint32   texture;
    };

    typedef QHash<Qt::HANDLE, TexturePixmap> PixmapHash;

    EGLConfig config;
    EGLDisplay dpy;
    PixmapHash bound_pixmaps;

    static QMap<QGLWidget *, MGLES2Renderer *> glRenderers;
    static MGLES2Renderer *activeRenderer;

    QPainter *m_activePainter;
    QGLShaderProgram *m_activeProgram;
    QSize m_boundTexSize;
    bool m_invertTexture;
};

QMap<QGLWidget *, MGLES2Renderer *> MGLES2RendererPrivate::glRenderers;
MGLES2Renderer *MGLES2RendererPrivate::activeRenderer = NULL;

MGLES2RendererPrivate::MGLES2RendererPrivate()
    : m_glWidget(NULL), m_viewportSize(-1, -1)
{
    //init world matrix for the known parts
    //parts that are changeable by the transformation
    //are set when rendering in the textures
    m_matWorld[0][2] = 0.0;
    m_matWorld[1][2] = 0.0;
    m_matWorld[2][0] = 0.0;
    m_matWorld[2][1] = 0.0;
    m_matWorld[2][2] = 1.0;
    m_matWorld[2][3] = 0.0;
    m_matWorld[3][2] = 0.0;

    //init vertex and texcoord arrays, 4 first vertices are used to draw standard
    //pixmaps and all the remaining vertices can be used to draw dynamic geometries
    //like scalable images and other patched stuff, the vertices are packed into same
    //array so that we don't need to switch the used array on fly.
    m_vertices.resize(8 + (9 * 4 * 2));
    m_texCoords.resize(8 + (9 * 4 * 2));
    GLfloat *tc = m_texCoords.data();
    tc[0] = 0.0; tc[1] = 1.0;
    tc[2] = 0.0; tc[3] = 0.0;
    tc[4] = 1.0; tc[5] = 0.0;
    tc[6] = 1.0; tc[7] = 1.0;

    //init index array that is used to draw dynamic geometries
    //the indices are offset by 4 because the 4 vertices that
    //are used to draw normal pixmaps are in the beginning of
    //the array
    m_indices.resize(9 * 6);
    GLushort *indices = m_indices.data();
    for (GLuint i = 0; i < 9; i++) {
        indices[i*6+0] = 4 + (i * 4 + 0); //x1 y1 TL
        indices[i*6+1] = 4 + (i * 4 + 1); //x1 y2 BL
        indices[i*6+2] = 4 + (i * 4 + 2); //x2 y2 BR
        indices[i*6+3] = 4 + (i * 4 + 0); //x1 y1 TL
        indices[i*6+4] = 4 + (i * 4 + 2); //x2 y2 BR
        indices[i*6+5] = 4 + (i * 4 + 3); //x2 y1 TR
    }

    dpy = eglGetDisplay(EGLNativeDisplayType(QX11Info::display()));

    m_activePainter = NULL;
    m_invertTexture = false;
}

MGLES2RendererPrivate::~MGLES2RendererPrivate()
{
    qDeleteAll(m_programCache);

    const PixmapHash::const_iterator bound_pixmaps_end = bound_pixmaps.constEnd();
    for (PixmapHash::const_iterator i = bound_pixmaps.constBegin(); i != bound_pixmaps_end; ++i) {
        eglDestroySurface(dpy, i->eglpixmap);
        glDeleteTextures(1, &i->texture);
    }
}

GLuint MGLES2RendererPrivate::setupVertices(const QSizeF &texSize, const QList<QRect>& sourceRects, const QList<QRect>& targetRects)
{
    const int numRects = qMin(sourceRects.size(), targetRects.size());
    resizeAttrArrays(numRects);

    const GLfloat dx = 1.0f / texSize.width();
    const GLfloat dy = 1.0f / texSize.height();

    const QVector<GLfloat>::iterator vertices  = m_vertices.begin()  + 8;
    const QVector<GLfloat>::iterator texcoords = m_texCoords.begin() + 8;

    for (int i = 0; i < numRects; ++i) {
        // setup vertices for the target rectangle
        const QRect &target = targetRects[i];
        const GLfloat x1 = target.left();
        const GLfloat x2 = target.right();
        const GLfloat y1 = target.top();
        const GLfloat y2 = target.bottom();

        vertices[8*i]     = x1; vertices[8*i + 1] = y1;
        vertices[8*i + 2] = x1; vertices[8*i + 3] = y2;
        vertices[8*i + 4] = x2; vertices[8*i + 5] = y2;
        vertices[8*i + 6] = x2; vertices[8*i + 7] = y1;

        // setup texture coords for the source rectangle
        const QRect &source = sourceRects[i];
        const GLfloat u1 = dx * float(source.left());
        const GLfloat u2 = dx * float(source.right());
        GLfloat v1 = dy * float(source.top());
        GLfloat v2 = dy * float(source.bottom());
        if (m_invertTexture) {
            v1 = 1.0f - v1;
            v2 = 1.0f - v2;
        }
        texcoords[8*i]     = u1; texcoords[8*i + 1] = v1;
        texcoords[8*i + 2] = u1; texcoords[8*i + 3] = v2;
        texcoords[8*i + 4] = u2; texcoords[8*i + 5] = v2;
        texcoords[8*i + 6] = u2; texcoords[8*i + 7] = v1;
    }
    return numRects * 6;
}

void MGLES2RendererPrivate::resizeAttrArrays(int numQuads)
{
    if (numQuads * 6 > m_indices.size()) {
        //resize the vertex and texcoord arrays to needed size
        m_vertices.resize(8 + (numQuads * 4 * 2));
        m_texCoords.resize(8 + (numQuads * 4 * 2));

        //resize the index array and fill the new indices
        GLuint oldcount = m_indices.size() / 6;
        GLuint newcount = numQuads;
        m_indices.resize(numQuads * 6);
        GLushort *indices = m_indices.data();
        for (GLuint i = oldcount; i < newcount; i++) {
            indices[i*6+0] = 4 + (i * 4 + 0); //x1 y1 TL
            indices[i*6+1] = 4 + (i * 4 + 1); //x1 y2 BL
            indices[i*6+2] = 4 + (i * 4 + 2); //x2 y2 BR
            indices[i*6+3] = 4 + (i * 4 + 0); //x1 y1 TL
            indices[i*6+4] = 4 + (i * 4 + 2); //x2 y2 BR
            indices[i*6+5] = 4 + (i * 4 + 3); //x2 y1 TR
        }

        //reinit the attribute array pointers because resizing might of
        //changed pointer returned by the data()
        glEnableVertexAttribArray(M_ATTR_VERTEX);
        glVertexAttribPointer(M_ATTR_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, m_vertices.data());
        glEnableVertexAttribArray(M_ATTR_TCOORD);
        glVertexAttribPointer(M_ATTR_TCOORD, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords.data());
    }
}

QGLShaderProgram *MGLES2RendererPrivate::requestBinaryProgram(const QString &frag, const QString &vert)
{
    // not implemented yet
    (void) frag;
    (void) vert;
    return 0;
}

MGLES2Renderer::MGLES2Renderer()
    : d_ptr(new MGLES2RendererPrivate())
{
}

MGLES2Renderer::~MGLES2Renderer()
{
    delete d_ptr;
}

MGLES2Renderer *MGLES2Renderer::instance()
{
    if (!MGLES2RendererPrivate::activeRenderer)
        mWarning("MGLES2Renderer::instance()") << "No active renderer.";

    return MGLES2RendererPrivate::activeRenderer;
}

MGLES2Renderer *MGLES2Renderer::instance(QGLWidget *glWidget)
{
    MGLES2Renderer *renderer = MGLES2RendererPrivate::glRenderers.value(glWidget);

    if (!renderer) {
        renderer = new MGLES2Renderer();
        renderer->init(glWidget);
        MGLES2RendererPrivate::glRenderers.insert(glWidget, renderer);
    }
    return renderer;
}

void MGLES2Renderer::activate(QGLWidget *glWidget)
{
    if (glWidget)
        MGLES2RendererPrivate::activeRenderer = MGLES2Renderer::instance(glWidget);
    else
        MGLES2RendererPrivate::activeRenderer = NULL;
}

void MGLES2Renderer::destroy(QGLWidget *glWidget)
{
    if (MGLES2Renderer *const renderer = MGLES2RendererPrivate::glRenderers.take(glWidget)) {

        if (renderer == MGLES2RendererPrivate::activeRenderer) {
            mWarning("MGLES2Renderer::destroy()") << "Destroying active renderer.";
            MGLES2Renderer::activate(NULL);
        }
        delete renderer;
    }
}

void MGLES2Renderer::destroyAll()
{
    if (!MGLES2RendererPrivate::glRenderers.empty()) {
        mWarning("MGLES2Renderer::destroyAll()") << "Renderers still existing.";
        MGLES2Renderer::activate(NULL);
        qDeleteAll(MGLES2RendererPrivate::glRenderers);
        MGLES2RendererPrivate::glRenderers.clear();
    }
}

void MGLES2Renderer::init(QGLWidget *glWidget)
{
    d_ptr->dpy = eglGetDisplay(EGLNativeDisplayType(QX11Info::display()));
    d_ptr->m_glWidget = glWidget;
    d_ptr->m_glWidget->makeCurrent();

    //init the orthogonal projection matrix
    //glOrtho(0, w, h, 0, -1, 1):
    //2.0/w,  0.0,    0.0, -1.0
    //0.0,   -2.0/h,  0.0,  1.0
    //0.0,    0.0,   -1.0,  0.0
    //0.0,    0.0,    0.0,  1.0
    GLfloat w = glWidget->width();
    GLfloat h = glWidget->height();
    d_ptr->m_matProj[0][0] =  2.0f / w; d_ptr->m_matProj[1][0] =  0.0;   d_ptr->m_matProj[2][0] =  0.0; d_ptr->m_matProj[3][0] = -1.0;
    d_ptr->m_matProj[0][1] =  0.0;   d_ptr->m_matProj[1][1] = -2.0f / h; d_ptr->m_matProj[2][1] =  0.0; d_ptr->m_matProj[3][1] =  1.0;
    d_ptr->m_matProj[0][2] =  0.0;   d_ptr->m_matProj[1][2] =  0.0;   d_ptr->m_matProj[2][2] = -1.0; d_ptr->m_matProj[3][2] =  0.0;
    d_ptr->m_matProj[0][3] =  0.0;   d_ptr->m_matProj[1][3] =  0.0;   d_ptr->m_matProj[2][3] =  0.0; d_ptr->m_matProj[3][3] =  1.0;

    d_ptr->m_defaultProgram = getShaderProgram(QString(), QString());
}

QGLShaderProgram *MGLES2Renderer::getShaderProgram(const QString &frag, const QString &vert)
{
    // First try a direct lookup with the passed-in filenames.  If they are
    // already canonical and the program exists in the cache, the file system
    // access can be skipped altogether.
    QGLShaderProgram *program = d_ptr->m_programCache.value(MGLProgramKey(frag, vert));

    if (!program) {
        const MGLProgramKey key(canonicalFilename(frag), canonicalFilename(vert));
        program = d_ptr->m_programCache.value(key);

        if (!program) {
            program = d_ptr->requestBinaryProgram(key.first, key.second);
            if (!program)
                program = compileShaderProgram(key.first, key.second);
            if (program)
                d_ptr->m_programCache.insert(key, program);
        }
    }
    return program;
}

QGLShaderProgram *MGLES2Renderer::compileShaderProgram(const QString &frag, const QString &vert)
{
    const QString fragFilename = (frag.isEmpty()) ? defaultFragmentShader() : frag;
    QGLShader fragShader(QGLShader::Fragment, d_ptr->m_glWidget->context());
    if (!fragShader.compileSourceFile(fragFilename)) {
        mWarning("MGLES2Renderer") << "failed to compile fragment shader" << fragFilename
                                       << '\n' << fragShader.log();
        return 0;
    }
    const QString vertFilename = (vert.isEmpty()) ? defaultVertexShader() : vert;
    QGLShader vertShader(QGLShader::Vertex, d_ptr->m_glWidget->context());
    if (!vertShader.compileSourceFile(vertFilename)) {
        mWarning("MGLES2Renderer") << "failed to compile vertex shader" << vertFilename
                                       << '\n' << vertShader.log();
        return 0;
    }
    QGLShaderProgram *const program = new QGLShaderProgram(d_ptr->m_glWidget->context());

    if (program->addShader(&fragShader) && program->addShader(&vertShader)) {
        // bind needed attribute arrays to specific indices -- TODO: What's this for exactly?
        program->bindAttributeLocation("vertex", M_ATTR_VERTEX);
        program->bindAttributeLocation("texCoord", M_ATTR_TCOORD);
        program->bindAttributeLocation("color", M_ATTR_VCOLOR);

        // TODO: default context implicit here, but not above?
        if (program->link() && program->bind()) {
            // Setup default texturing uniforms here so we dont need setup them in runtime
            const QByteArray prefix("texture");
            GLint maxTexUnits = 0;
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexUnits);

            for (int i = 0; i < maxTexUnits; ++i) {
                const int loc = program->uniformLocation(prefix + QByteArray::number(i));
                if (loc >= 0)
                    program->setUniformValue(loc, GLuint(i));
            }
        }
        return program;
    }
    mWarning("MGLES2Renderer") << "failed to link shader program"
                                   << (fragFilename + ':' + vertFilename);
    delete program;
    return 0;
}

quint32 MGLES2Renderer::bindX11Pixmap(Qt::HANDLE pixmap)
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
            mWarning("MGLES2Renderer") << "No appropriate EGL configuration for texture from pixmap!";
            return 0;
        }
        for (EGLint i = 0; i < ecfgs; i++) {
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
    MGLES2RendererPrivate::TexturePixmap tp = d_ptr->bound_pixmaps.value(pixmap);
    if (tp.texture > 0)
        return tp.texture;

    // .. or create a new one
    if (!eglpixmap)
        eglpixmap = eglCreatePixmapSurface(d_ptr->dpy, d_ptr->config,
                                           (EGLNativePixmapType) pixmap,
                                           pixmapAttribs);
    if (eglpixmap == EGL_NO_SURFACE) {
        mWarning("MGLES2Renderer") << "Cannot create EGL surface:" << eglGetError();
        return 0;
    }
    tp.eglpixmap = eglpixmap;

    glGenTextures(1, &tp.texture);
    d_ptr->bound_pixmaps[pixmap] = tp;
    glBindTexture(GL_TEXTURE_2D, tp.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glEnable(GL_TEXTURE_2D);

    return tp.texture;
}

void MGLES2Renderer::unbindX11Pixmap(Qt::HANDLE pixmap)
{
    MGLES2RendererPrivate::TexturePixmap tp = d_ptr->bound_pixmaps.value(pixmap);
    if (!tp.texture)
        return;

    // free the pixmap and texture cache
    eglDestroySurface(d_ptr->dpy, tp.eglpixmap);
    tp.eglpixmap =  EGL_NO_SURFACE;
    //XSync(QX11Info::display(), FALSE);
    glDeleteTextures(1, &tp.texture);
    d_ptr->bound_pixmaps.remove(pixmap);
}

void MGLES2Renderer::updateX11Pixmap(Qt::HANDLE pixmap)
{
    // catch updates to the pixmap here when they happen
    MGLES2RendererPrivate::TexturePixmap tp = d_ptr->bound_pixmaps.value(pixmap);
    if (!tp.texture)
        return;

    glBindTexture(GL_TEXTURE_2D, tp.texture);

    if (eglReleaseTexImage(d_ptr->dpy, tp.eglpixmap,
                           EGL_BACK_BUFFER) == EGL_FALSE)
        mWarning("MGLES2Renderer: Update pixmap, cant release bound texture");
    if (eglBindTexImage(d_ptr->dpy, tp.eglpixmap,
                        EGL_BACK_BUFFER) == EGL_FALSE) {
        mWarning("MGLES2Renderer: Update pixmap, can't bind EGL texture to pixmap:") << eglGetError();
    }
    glEnable(GL_TEXTURE_2D);
}

void MGLES2Renderer::begin(QPainter *painter)
{
    begin(painter, d_ptr->m_defaultProgram);
}

void MGLES2Renderer::begin(QPainter *painter, QGLShaderProgram *program)
{
    if (d_ptr->m_activePainter)
        end();

    if (painter) {
        d_ptr->m_activePainter = painter;
        d_ptr->m_activePainter->beginNativePainting();
    }

    //TODO: do this only when setting the viewport size manually?
    //      otherwise set this and the uniform only once when creating the program
    //setup orthogonal projection matrix
    GLfloat w = d_ptr->m_viewportSize.width() == -1 ?  d_ptr->m_glWidget->width() :  d_ptr->m_viewportSize.width();
    GLfloat h = d_ptr->m_viewportSize.height() == -1 ? d_ptr->m_glWidget->height() : d_ptr->m_viewportSize.height();
    glViewport(0, 0, w, h);
    d_ptr->m_matProj[0][0] =  2.0 / w;
    d_ptr->m_matProj[1][1] = -2.0 / h;

    //setup shader program
    d_ptr->m_activeProgram = program;
    if (d_ptr->m_activeProgram) {
        d_ptr->m_activeProgram->bind();
        d_ptr->m_activeProgram->setUniformValue("matProj", d_ptr->m_matProj);
    }

    glEnableVertexAttribArray(M_ATTR_VERTEX);
    glVertexAttribPointer(M_ATTR_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, d_ptr->m_vertices.data());
    glEnableVertexAttribArray(M_ATTR_TCOORD);
    glVertexAttribPointer(M_ATTR_TCOORD, 2, GL_FLOAT, GL_FALSE, 0, d_ptr->m_texCoords.data());

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}

void MGLES2Renderer::end()
{
    if (d_ptr->m_activePainter) {
        d_ptr->m_activePainter->endNativePainting();
        d_ptr->m_activePainter = NULL;
    }
    if (d_ptr->m_activeProgram) {
        d_ptr->m_activeProgram->release();
        d_ptr->m_activeProgram = NULL;
    }

    d_ptr->m_boundTexSize = QSize(-1, -1);
    d_ptr->m_viewportSize = QSize(-1, -1);

    //TODO: Refactor this to be more automatic, only x pixmaps need inverting.
    d_ptr->m_invertTexture = false;
    GLfloat *tc = d_ptr->m_texCoords.data();
    tc[0] = 0.0; tc[1] = 1.0;
    tc[2] = 0.0; tc[3] = 0.0;
    tc[4] = 1.0; tc[5] = 0.0;
    tc[6] = 1.0; tc[7] = 1.0;
}

void MGLES2Renderer::activateProgram(QGLShaderProgram *program)
{
    if (program) {
        d_ptr->m_activeProgram = program;
        d_ptr->m_activeProgram->bind();
        d_ptr->m_activeProgram->setUniformValue("matProj", d_ptr->m_matProj);
    } else {
        mWarning("MGLES2Renderer") << "activateProgram() Invalid program.";
    }
}

void MGLES2Renderer::bindTexture(const QPixmap &pixmap, quint32 unit, const QString &uniformName)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    bindTexture(d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::PremultipliedAlphaBindOption), pixmap.size(), unit, uniformName);
    //bindTexture(d_ptr->m_glWidget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::NoBindOption), pixmap.size(), unit, uniformName);
}

void MGLES2Renderer::bindTexture(quint32 texId, const QSize &texSize, quint32 unit, const QString &uniformName)
{
    //setup uniform only if some custom texture name has
    //been defined, the default texture uniforms (texture0,
    //texture1 etc.) was setup when creating the program
    if (d_ptr->m_activeProgram && !uniformName.isEmpty()) {
        d_ptr->m_activeProgram->setUniformValue(uniformName.toStdString().c_str(), (GLuint)unit);
    }

    //the unit 0 defines the texture size
    //that is given to shaders who need it
    if (unit == 0)
        d_ptr->m_boundTexSize = texSize;

    //bind texture to wanted unit
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texId);

    //setup texture filtering based on the active painter
    if (d_ptr->m_activePainter) {
        if (d_ptr->m_activePainter->renderHints() & QPainter::SmoothPixmapTransform) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glEnable(GL_TEXTURE_2D);
}

void MGLES2Renderer::setInvertTexture(bool invert)
{
    /* Compensate for inverted textures

        Rendering done by the window system may be y-inverted compared
        to the standard OpenGL texture representation.  More specifically:
        the X Window system uses a coordinate system where the origin is in
        the upper left; however, the GL uses a coordinate system where the
        origin is in the lower left.
    */

    //TODO: Refactor this to be more automatic, only x pixmaps need inverting.
    d_ptr->m_invertTexture = invert;
    GLfloat *tc = d_ptr->m_texCoords.data();
    if (invert) {
        tc[0] = 0.0; tc[1] = 0.0;
        tc[2] = 0.0; tc[3] = 1.0;
        tc[4] = 1.0; tc[5] = 1.0;
        tc[6] = 1.0; tc[7] = 0.0;
    } else {
        tc[0] = 0.0; tc[1] = 1.0;
        tc[2] = 0.0; tc[3] = 0.0;
        tc[4] = 1.0; tc[5] = 0.0;
        tc[6] = 1.0; tc[7] = 1.0;
    }
}

void MGLES2Renderer::draw(int x, int y, int width, int height)
{
    draw(QRect(x, y, width, height));
}

void MGLES2Renderer::draw(int x, int y)
{
    draw(x, y, d_ptr->m_boundTexSize.width(), d_ptr->m_boundTexSize.height());
}

void MGLES2Renderer::draw(const QRect &rectangle)
{
    GLfloat *vertices = d_ptr->m_vertices.data();
    vertices[0] = rectangle.left();  vertices[1] = rectangle.top();
    vertices[2] = rectangle.left();  vertices[3] = rectangle.bottom();
    vertices[4] = rectangle.right(); vertices[5] = rectangle.bottom();
    vertices[6] = rectangle.right(); vertices[7] = rectangle.top();

    QTransform transform;
    GLfloat o = 1.0;
    if (d_ptr->m_activePainter) {
        transform = d_ptr->m_activePainter->combinedTransform();
        o = d_ptr->m_activePainter->opacity();
    }
    d_ptr->m_matWorld[0][0] = transform.m11();
    d_ptr->m_matWorld[0][1] = transform.m12();
    d_ptr->m_matWorld[0][3] = transform.m13();
    d_ptr->m_matWorld[1][0] = transform.m21();
    d_ptr->m_matWorld[1][1] = transform.m22();
    d_ptr->m_matWorld[1][3] = transform.m23();
    d_ptr->m_matWorld[3][0] = transform.dx();
    d_ptr->m_matWorld[3][1] = transform.dy();
    d_ptr->m_matWorld[3][3] = transform.m33();
    d_ptr->m_activeProgram->setUniformValue("matWorld", d_ptr->m_matWorld);
    d_ptr->m_activeProgram->setUniformValue("opacity", o);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void MGLES2Renderer::draw(const QList<QRect>& targets, const QList<QRect>& sources)
{
    GLuint num = d_ptr->setupVertices(d_ptr->m_boundTexSize, sources, targets);


    QTransform transform;
    GLfloat o = 1.0;
    if (d_ptr->m_activePainter) {
        transform = d_ptr->m_activePainter->combinedTransform();
        o = d_ptr->m_activePainter->opacity();
    }

    d_ptr->m_matWorld[0][0] = transform.m11();
    d_ptr->m_matWorld[0][1] = transform.m12();
    d_ptr->m_matWorld[0][3] = transform.m13();
    d_ptr->m_matWorld[1][0] = transform.m21();
    d_ptr->m_matWorld[1][1] = transform.m22();
    d_ptr->m_matWorld[1][3] = transform.m23();
    d_ptr->m_matWorld[3][0] = transform.dx();
    d_ptr->m_matWorld[3][1] = transform.dy();
    d_ptr->m_matWorld[3][3] = transform.m33();
    d_ptr->m_activeProgram->setUniformValue("matWorld", d_ptr->m_matWorld);
    d_ptr->m_activeProgram->setUniformValue("opacity", o);

    glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_SHORT, d_ptr->m_indices.data());
}

void MGLES2Renderer::draw(const QRect &target, const QRect &source)
{
    QList<QRect> targets;
    QList<QRect> sources;
    targets.append(target);
    sources.append(source);
    draw(targets, sources);
}

void MGLES2Renderer::setViewportSize(const QSize &size)
{
    d_ptr->m_viewportSize = size;
}

#else
MGLES2Renderer *MGLES2Renderer::instance()
{
    return NULL;
}
MGLES2Renderer *MGLES2Renderer::instance(QGLWidget *)
{
    return NULL;
}
void MGLES2Renderer::activate(QGLWidget *) {}
void MGLES2Renderer::destroy(QGLWidget *) {}
void MGLES2Renderer::destroyAll() {}
QGLShaderProgram *MGLES2Renderer::getShaderProgram(const QString &, const QString &)
{
    return NULL;
}
QGLShaderProgram *MGLES2Renderer::compileShaderProgram(const QString &, const QString &)
{
    return NULL;
}
quint32 MGLES2Renderer::bindX11Pixmap(Qt::HANDLE)
{
    return 0;
}
void MGLES2Renderer::unbindX11Pixmap(Qt::HANDLE) {}
void MGLES2Renderer::updateX11Pixmap(Qt::HANDLE) {}
void MGLES2Renderer::begin(QPainter *) {}
void MGLES2Renderer::begin(QPainter *, QGLShaderProgram *) {}
void MGLES2Renderer::end() {}
void MGLES2Renderer::activateProgram(QGLShaderProgram *) {}
void MGLES2Renderer::bindTexture(const QPixmap &, quint32, const QString &) {}
void MGLES2Renderer::bindTexture(quint32, const QSize &, quint32, const QString &) {}
void MGLES2Renderer::setInvertTexture(bool) {}
void MGLES2Renderer::draw(int, int, int, int) {}
void MGLES2Renderer::draw(int, int) {}
void MGLES2Renderer::draw(const QRect &) {}
void MGLES2Renderer::draw(const QList<QRect>&, const QList<QRect>&) {}
void MGLES2Renderer::draw(const QRect &, const QRect &) {}
void MGLES2Renderer::setViewportSize(const QSize &) {}
#endif
