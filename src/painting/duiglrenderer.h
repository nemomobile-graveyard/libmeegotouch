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

#ifndef DUIGLRENDERER_H
#define DUIGLRENDERER_H

#include <QList>
#include <QString>
#include "duiexport.h"

class QGLWidget;
class DuiGLWidget;
class DuiGLRendererPrivate;
class DuiGLShaderUniform;
class DuiGLShader;
class DuiGLShaderProgram;
class DuiParticle;
class QTransform;
class QSizeF;

//some definitions to enable building without gles2 libraries
#ifdef QT_OPENGL_LIB
#include <QtOpenGL>
#ifdef QT_OPENGL_ES_2
#define DUI_USE_OPENGL
#endif
#endif

class DuiCompositionLayer;
typedef QList<DuiCompositionLayer> DuiCompositionLayerList;

/*!
 * \class IDuiGLUniformProvider
 * \brief Callback interface for setting uniform values for custom shaders.
 */
class DUI_EXPORT IDuiGLUniformProvider
{
public:
    virtual ~IDuiGLUniformProvider() {}

    /*!
     * Initialize uniform \a name to wanted value.
     *
     * \param name Name of the uniform.
     * \param uniform Uniform to be set.
     * \return True if uniform was set by the method, false if uniform was not set
     *         by the method.
     */
    virtual bool setUniformValue(const QString &name, const DuiGLShaderUniform &uniform) = 0;
};

/*!
 * \class DuiGLRenderer
 * \brief A singleton class for rendering textured quads with custom shaders.
 *
 * DuiGLRenderer class implements and provides an easy interface for
 * compiling and setuping glsl shaders. The class also provides functionality
 * for rendering textured quads directly with GLES2. Class caches the compiled
 * shaders so that they can used inside a process without always recompiling
 * them. Following uniforms and attributes are considered default and they are
 * initialized by the DuiGLRenderer class automatically:
 *
 * \code
 * //vertex shader variables
 * attribute highp vec4 vertex;   //object space vertex coordinate
 * attribute lowp  vec2 texCoord; //texture coordinates of the vertex
 * uniform   highp mat4 matProj;  //projection matrix
 * uniform   highp mat4 matWorld; //world transformation matrix
 * uniform   highp vec2 invSize;  //inversed size of rendered quad
 *
 * //fragment shader variables
 * uniform sampler2D textureX;    //textures for each texture unit (eg. texture0, texture1 ...)
 * \endcode
 *
 * The DuiGLUniformProvider class can be used to automate setting of the custom
 * uniforms found from the shaders. A user of the class can either set a global
 * uniform provider callback by using the DuiGLRenderer::setUniformProvider()
 * method or the user can give a separate provider interface for each draw call. User
 * can also set the uniforms manually using the DuiGLShaderProgram class. The callback
 * method will be called for each non-default uniform found from the used shader program.
 *
 * \deprecated Since 0.16
 */
class DUI_EXPORT DuiGLRenderer
{
public:

    //! Enumeration for supported shader types.
    enum DuiShaderType {
        DuiShaderVertex,
        DuiShaderFragment,
        //DuiShaderVertexBin,
        //DuiShaderFragmentBin,
        DuiShaderUndefined
    };

    /*!
     * \brief Destructor.
     */
    virtual ~DuiGLRenderer();

    /*!
     * Returns singleton DuiGLRenderer object
     *
     * \return DuiGLRenderer object
     */
    static DuiGLRenderer *instance();

    /*!
     * Initialization method for the class. Method initializes few default
     * shaders and other GL specific data needed for rendering. This should
     * be called after the QGLWidget viewport has been created for the
     * QGraphicsView. The class cannot be used without a valid QGLWidget
     * object.
     *
     * \param glWidget Instance of QGLWidget that is needed to for executing GL commands.
     */
    void initGL(DuiGLWidget *glWidget);

    /*!
     * Returns a pointer to the currently used DuiGLWidget.
     */
    DuiGLWidget *GLWidget() const;

    /*!
     * Sets the \a glWidget as the currently used DuiGLWidget.
     */
    void setGLWidget(DuiGLWidget *glWidget);

    /*!
     * Create a shader from a file and push it into cache.
     *
     * \param filename Filename of the shader. This will be used as key as well.
     *        File can contain either precompiled shader binary or the ascii
     *        source of the shader.
     * \param type Type of the shader, if the type is set to UndefinedShader the
     *        method will try to auto detect the type of the shader (frag or vert).
     * \return Instance of the created shader, NULL if the creation fails.
     */
    const DuiGLShader *createShader(const QString &filename, DuiShaderType type);

    /*!
     * Compile a shader from a source and push it into cache.
     *
     * \param key Literal key for the shader.
     * \param source Source code of the shader.
     * \param type Type of the shader, if the type is set to UndefinedShader the
     *        method will try to auto detect the type of the shader (frag or vert).
     * \return Instance of the created shader, NULL if the creation fails.
     */
    const DuiGLShader *createShader(const QString &key, const QString &source, DuiShaderType type);

    /*!
     * Get shader instance by key.
     *
     * \param key Literal key of the wanted shader.
     * \return Instance of the shader, NULL if the shader does not exist.
     */
    const DuiGLShader *getShader(const QString &key);

    /*!
     * Create a shader program from a group of already compiled shaders. If
     * the group of shaders does not include a vertex shader or fragment
     * shader a default one is added.
     *
     * \param key Literal key for the created program.
     * \param shaderKeyList List of shaders that will be linked into the
     *        program. If list contains a key that cannot be found from the
     *        shader cache it will be ignored.
     * \return Instance of the program, NULL if the program creation fails.
     */
    const DuiGLShaderProgram *createProgram(const QString &key, const QList<QString>& shaderKeyList);

    /*!
     * Create a shader program from exactly one vertex shader and one
     * fragment shader.
     *
     * \param key Literal key for the created program.
     * \param vertShader Key of the vertex shader. By default the key is set to ""
     *        so that default vertex shader is used.
     * \param fragShader Key of the fragment shader.
     * \return Instance of the program, NULL if the program creation fails.
     */
    const DuiGLShaderProgram *createProgram(const QString &key, const QString &vertShader, const QString &fragShader = QString());

    /*!
     * Create a shader program from a group of files. If the group of shaders
     * does not include a vertex shader or fragment shader a default one is
     * added. The shaders in the list are compiled and cached same way as if
     * they are created with separate call to createShader() method.
     *
     * \param key Literal key for the created program.
     * \param shaderFileList List of shaders files that will be compiled and
     *        linked into the program. If a list contains invalid shader
     *        (file doest not exist or shader compilation fails) it will be ignored.
     * \return Instance of the program, NULL if the program creation fails.
     */
    //DuiGLShaderProgram* createProgramFromFiles(const QString& key, const QList<QString>& shaderFileList);

    /*!
     * Create a shader program from exactly one vertex shader and one
     * fragment shader file.
     *
     * \param key Literal key for the created program.
     * \param fragShaderFile Filename of the fragment shader.
     * \param vertShaderFile Filename of the vertex shader. By default the filename is set to ""
     *        so that default vertex shader is used.
     * \return Instance of the program, NULL if the program creation fails.
     */
    const DuiGLShaderProgram *createProgramFromFiles(const QString &key, const QString &fragShaderFile, const QString &vertShaderFile = QString());

    /*!
     * Get program instance by key.
     *
     * \param key Literal key of the wanted program.
     * \return Instance of the program, NULL if the program does not exist.
     */
    const DuiGLShaderProgram *getProgram(const QString &key);

    /*!
     * Bind an X pixmap as a texture. Uses the Nokia texture from pixmap
     * extension to ensure that same pixmap in memory is used as a texture.
     * Returns a texture id that can be drawn with drawTexture() or in later
     * glBindTexture() calls. The texture that is generated is cached,
     * so multiple calls to bindPixmap() with the same X pixmap will return
     * the same texture id. Ideally, this function should be called once
     * as soon as the pixmap id is obtained. Before drawing the contents
     * of the texture, ensure updateX11Pixmap() is called to update its
     * contents.
     *
     * \param pixmap specifies the X pixmap to be bound as a texture
     */
    quint32 bindX11Pixmap(Qt::HANDLE pixmap);

    /*!
     * Unbind the texture from an X pixmap. Deletes the texture cache as
     * well. Call this to free the resources of the pixmap bound as a texture
     *
     * \param pixmap specifies the X pixmap to be bound as a texture
     */
    void unbindX11Pixmap(Qt::HANDLE pixmap);

    /*!
     * Updates the contents of bound X pixmap. Call this every time the
     * contents of the pixmap changes i.e. from X Damage events
     *
     * \param pixmap specifies the X pixmap to be bound as a texture
     */
    void updateX11Pixmap(Qt::HANDLE pixmap);

    /*!
     * Draw textured quad using default shader.
     *
     * \param transform Transformation for the rendered quad.
     * \param texId Id of the rendered texture.
     * \param size Size of the rendered quad.
     * \param opacity Transparency factor for the rendered quad.
     * \param inverted Set to true if texture to be drawn is an inverted texture. False by default
     */
    void drawTexture(const QTransform &transform, quint32 texId, const QSizeF &size, qreal opacity,
                     bool inverted = false);

    /*!
     * Draw textured quad using a custom shader.
     *
     * \param program The used custom shader program. The program must have
     *        been set active prior to this method call. Also all the uniforms
     *        used by the program needs to be set before calling this method.
     * \param transform Transformation for the rendered quad.
     * \param texId Id of the rendered texture.
     * \param size Size of the rendered quad.
     * \param provider Callback interface for setting custom uniform values
     *        used by the shader program.
     * \param inverted Set to true if texture to be drawn is an inverted texture. False by default
     */
    void drawTexture(const DuiGLShaderProgram *program, const QTransform &transform,
                     quint32 texId, const QSizeF &size, IDuiGLUniformProvider *provider,
                     bool inverted = false);

    /*!
     * Draw textured quad using a custom shader.
     *
     * \param programId Id of the used custom shader program. Method will
     *        fetch the precompiled program object from the cache and makes
     *        it active.
     * \param transform Transformation for the rendered quad.
     * \param texId Id of the rendered texture.
     * \param size Size of the rendered quad.
     * \param provider Callback interface for setting custom uniform values
     *        used by the shader program.
     * \param inverted Set to true if texture to be drawn is an inverted texture. False by default
     */
    void drawTexture(const QString &programId, const QTransform &transform,
                     quint32 texId, const QSizeF &size, IDuiGLUniformProvider *provider = NULL,
                     bool inverted = false);

    /*!
     * Draw textured quad using default shader.
     *
     * \param transform Transformation for the rendered quad.
     * \param pixmap Pixmap for the rendered quad.
     * \param opacity Transparency factor for the rendered quad.
     */
    void drawPixmap(const QTransform &transform, const QPixmap &pixmap, qreal opacity);


    /*!
     * \brief Draw several patches from a single pixmap to screen.
     *
     * \param transform Transformation for the patches.
     * \param pixmap Source pixmap.
     * \param opacity Transparency factor for the rendered patches.
     * \param sourceRects List of source rectangles (pixmap).
     * \param targetRects List of target rectangles (screen).
     */
    void drawPatchedPixmap(const QTransform &transform, const QPixmap &pixmap, qreal opacity,
                           const QList<QRect>& sourceRects, const QList<QRect>& targetRects);

    /*!
     * Draw textured quad using a custom shader.
     *
     * \param program The used custom shader program. The program must have
     *        been set active prior to this method call. Also all the uniforms
     *        used by the program needs to be set before calling this method.
     * \param transform Transformation for the rendered quad.
     * \param pixmap Pixmap for the rendered quad.
     * \param provider Callback interface for setting custom uniform values
     *        used by the shader program.
     */
    void drawPixmap(const DuiGLShaderProgram *program, const QTransform &transform,
                    const QPixmap &pixmap, IDuiGLUniformProvider *provider = NULL);

    /*!
     * Draw textured quad using a custom shader.
     *
     * \param programId Id of the used custom shader program. Method will
     *        fetch the precompiled program object from the cache and makes
     *        it active.
     * \param transform Transformation for the rendered quad.
     * \param pixmap Pixmap for the rendered quad.
     * \param provider Callback interface for setting custom uniform values
     *        used by the shader program.
     */
    void drawPixmap(const QString &programId, const QTransform &transform,
                    const QPixmap &pixmap, IDuiGLUniformProvider *provider = NULL);

    /*!
     * Draw particles.
     *
     * \param particles Array of particles.
     * \param numParticles Particle count.
     * \param pixmap Pixmap for the particles.
     * \param transform Transformation for the particles.
     */
    void drawParticles(const DuiParticle *particles, int numParticles, const QPixmap &pixmap, const QTransform &transform);

    /*!
     * Set the global uniform provider interface. The global uniform provider
     * will be always used unless another provider interface is given into
     * the drawing methods.
     *
     * \param provider An interface for providing values for the non-default
     *        uniforms.
     */
    void setUniformProvider(IDuiGLUniformProvider *provider);

    /*!
     * Returns the viewport widget used for drawing. The widget is the same one
     * that was previously set when initializing the renderer using the initGL()
     * method.
     *
     * \return Viewport widget.
     */
    QGLWidget *viewport();

    /*!
     * Set the viewport size. By default the viewport size is defined by the
     * viewport widget (screen size), but for example when rendering into a
     * texture using FBO the viewport size must be set manually to match the
     * FBO size. After calling one of the rendering methods the viewport size
     * is automatically reset to match the screen size.
     *
     * \param QSize Size of the viewport.
     */
    void setViewportSize(const QSize &size);

    /*!
     * Composite multiple pixmap layers into one pixmap.
     *
     * \param layerList List of the composited layers.
     * \return Composited pixmap.
     */
    QPixmap composite(const DuiCompositionLayerList &layerList);

private:

    Q_DISABLE_COPY(DuiGLRenderer)

    /*!
     * \brief Constructor. Initializes default shaders and data needed for rendering.
     */
    DuiGLRenderer();

    //! Pointer to private implementation class object.
    DuiGLRendererPrivate *const d_ptr;

    Q_DECLARE_PRIVATE(DuiGLRenderer)
};

#endif

