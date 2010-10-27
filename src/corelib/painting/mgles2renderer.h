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

#ifndef MGLES2RENDERER_H
#define MGLES2RENDERER_H

#include <QList>
#include <QSize>
#include <QString>
#include <QtOpenGL>
#include "mexport.h"

class QGLContext;
class MGLES2RendererPrivate;

class QGLShader;
class QGLShaderProgram;
class QPainter;
class QPixmap;
class QRect;

/*!
    \class MGLES2Renderer
    \brief A singleton class for rendering textured quads with custom shaders.

    MGLES2Renderer class implements and provides an easy interface for
    compiling and setuping glsl shaders. The class also provides functionality
    for rendering textured quads directly with GLES2. Class caches the compiled
    shaders so that they can used inside a process without always recompiling
    them. Following uniforms and attributes are considered default and they are
    initialized by the MGLES2Renderer class automatically:

    \code
    //vertex shader variables
    attribute highp vec4 vertex;   //object space vertex coordinate
    attribute lowp  vec2 texCoord; //texture coordinates of the vertex
    uniform   highp mat4 matProj;  //projection matrix
    uniform   highp mat4 matWorld; //world transformation matrix
    uniform   highp vec2 invSize;  //inversed size of rendered quad

    //fragment shader variables
    uniform sampler2D textureX; //textures for each texture unit (eg. texture0, texture1 ...)
    \endcode
*/
class M_CORE_EXPORT MGLES2Renderer
{
public:

    /*!
        \brief Destructor.
     */
    virtual ~MGLES2Renderer();

    /*!
        \brief Returns instance of MGLES2Renderer object for the specified QGLContext.

        If the renderer for the specified glContext does not yet exist it
        will be  created and initialized.

        Renderer object is created automatically for each MWindow if SW
        rendering is enabled.

        \return MGLES2Renderer object
    */
    static MGLES2Renderer *instance(QGLContext *glContext);
    static MGLES2Renderer *instance(QGLWidget *glWidget);

    /*!
        \brief Returns instance of MGLES2Renderer which was previously set active.

        Returns NULL if no renderer has been set active.

        In normal cases widgets should call this method when they need instance
        of the renderer. MWindow is responsible for activating a renderer.
        If the method returns NULL, it usually means that SW rendering has been
        forcefully applied (or the current development environment does not
        support GLES2).

        \return Active MGLES2Renderer object
    */
    static MGLES2Renderer *instance();

    /*!
        \brief Activates a renderer for the specified QGLContext.

        If the renderer for the specified glContext does not yet exist it
        will be created and initialized.

        MWindow is responsible for setting the active renderer automatically
        when the framework is drawing into it. Widgets should not call this
        method manually.
    */
    static void activate(QGLContext *glContext);
    static void activate(QGLWidget *glWidget);

    /*!
        \brief Destroys a renderer of the specified QGLContext.

        The \a glContext is not destroyed by this method.
    */
    static void destroy(QGLContext *glContext);
    static void destroy(QGLWidget *glWidget);

    /*!
        \brief Destroys all the renderer instances.
    */
    static void destroyAll();

    /*!
        \brief Fetch a shader program by its source file names.

        If the group of shaders does not include a vertex shader or fragment
        shader a default one is added.

        \param frag Filename of the fragment shader source
        \param vert Filename of the vertex shader source
        \return The shader program, or NULL on failure
    */
    QGLShaderProgram *getShaderProgram(const QString &frag = QString(), const QString &vert = QString());

    /*!
        \brief Compile and link a shader program.

        Compiles the specified shader source files and links the objects
        to a shader program.  No caching takes place at this point and
        a new shader program is always created.  Application code should
        generally call getShaderProgram() instead of this method.

        \param frag Filename of the fragment shader source
        \param vert Filename of the vertex shader source
        \return The shader program, or NULL on failure
    */
    QGLShaderProgram *compileShaderProgram(const QString &frag, const QString &vert);

    /*!
        \brief Bind an X pixmap as a texture.

        Uses the Nokia texture from pixmap extension to ensure that same
        pixmap in memory is used as a texture. Returns a texture id that
        can be drawn with drawTexture() or in later glBindTexture() calls.
        The texture that is generated is cached, so multiple calls to
        bindPixmap() with the same X pixmap will return the same texture
        id. Ideally, this function should be called once as soon as the
        pixmap id is obtained. Before drawing the contents of the texture,
        ensure updateX11Pixmap() is called to update its contents.

        \param pixmap specifies the X pixmap to be bound as a texture
        \deprecated There is no replacing functionality
    */
    quint32 bindX11Pixmap(Qt::HANDLE pixmap);

    /*!
        \brief Unbind the texture from an X pixmap.

        Deletes the texture cache as well. Call this to free the resources
        of the pixmap bound as a texture

        \param pixmap specifies the X pixmap to be bound as a texture
        \deprecated There is no replacing functionality
    */
    void unbindX11Pixmap(Qt::HANDLE pixmap);

    /*!
        \brief Updates the contents of bound X pixmap.
        Call this every time the contents of the pixmap changes i.e. from X
        Damage events.

        \param pixmap specifies the X pixmap to be bound as a texture
        \deprecated There is no replacing functionality
    */
    void updateX11Pixmap(Qt::HANDLE pixmap);

    /*!
        \brief Begin painting with MGLES2Renderer using default program.

        If \a painter is not NULL, renderer will extract opacity and
        transformation and use those when rendering pixmaps into screen.
        The renderer will also cache the current state of the painter, the
        state will be restored when calling the ::end() method.

        After calling this method you can still change the opacity and
        transformation from the painter and they will be taken into account
        when drawing pixmaps.
    */
    void begin(QPainter *painter);

    /*!
        \brief Begin painting with MGLES2Renderer using the specified program.

        If NULL is given as \a program, program needs to manually activated by
        calling activateProgram().
    */
    void begin(QPainter *painter, QGLShaderProgram *program);

    /*!
        \brief Restores to a state that was before calling ::begin().
    */
    void end();

    /*!
        \brief Activate a program and program specific data.
    */
    void activateProgram(QGLShaderProgram *program);

    /*!
        \brief Bind pixmap as texture.

        Bind \a pixmap to wanted texture unit and shader uniform. By default
        the pixmaps are bound to shaders using "textureN" as the uniform name,
        where N equals to \a unit.

        Size of pixmap bound to the unit 0, will be used when drawing textures
        with drawing methods.
    */
    void bindTexture(const QPixmap &pixmap, quint32 unit = 0, const QString &uniformName = QString());

    /*!
        \brief Bind texture.

        Bind \a texId to wanted texture unit and shader uniform. By default
        the pixmaps are bound to shaders using "textureN" as the uniform name,
        where N equals to \a unit.

        Optional texture size can be given as parameter, inversion of it will
        be given to shaders if they want to use it (uniform name == "invSize").
        Only size given when binding unit 0 is used. The texture size is also
        needed by some of the drawing methods.
    */
    void bindTexture(quint32 texId, const QSize &texSize = QSize(-1, -1), quint32 unit = 0, const QString &uniformName = QString());


    //TODO: Refactor this to be more automatic, only x pixmaps need inverting.
    //use inverted textures coords
    void setInvertTexture(bool invert);

    /*!
        \brief Draw texture.

        Draws the bound texture(s) into the rectangle at position (\a x, \a y)
        with the given \a width and \a height.
    */
    void draw(int x, int y, int width, int height);

    /*!
        \brief Draw texture.

        Draws the bound texture(s) at position (\a x, \a y). The size that
        was given as parameter to bindTexture() will be used as width and
        height.
    */
    void draw(int x, int y);

    /*!
        \brief Draw texture.

        Draws the bound texture(s) into the given \a rectangle.
    */
    void draw(const QRect &rectangle);

    /*!
        \brief Draw texture.

        Draws multiple patches of texture(s) \a sources into the given
        \a targets.

        The size that was given as parameter to bindTexture() will be used
        for calculating the texture coordinates from the source rectangles.
    */
    void draw(const QList<QRect>& targets, const QList<QRect>& sources);

    /*!
        \brief Draw texture.

        Draws the rectangular portion \a source of the bound texture(s)
        into the given \a target rectangle.

        The size that was given as parameter to bindTexture() will be used
        for calculating the texture coordinates from the source rectangle.
    */
    void draw(const QRect &target, const QRect &source);

    /*!
        \brief Set the viewport size.

        By default the viewport size is defined by the viewport widget (screen
        size), but for example when rendering into a texture using FBO the
        viewport size must be set manually to match the FBO size.

        The viewport size will be reset to screen size when calling the end()
        method. Viewport size can be reset manually by giving QSize(-1,-1) as
        parameter.

        \param QSize Size of the viewport.
    */
    void setViewportSize(const QSize &size);

private:
    /*!
        \brief Constructor.
    */
    MGLES2Renderer();

    /*!
        \brief Initialization method for the class.

        Sets the used QGLContext.
    */
    void init(QGLContext *glContext);

    //! Pointer to private implementation class object.
    MGLES2RendererPrivate *const d_ptr;

    Q_DISABLE_COPY(MGLES2Renderer)
    Q_DECLARE_PRIVATE(MGLES2Renderer)

#ifdef UNIT_TEST
    friend class Pt_MGLES2Renderer;
#endif
};

#endif

