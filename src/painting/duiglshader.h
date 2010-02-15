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

#ifndef DUIGLSHADER_H
#define DUIGLSHADER_H

#include <QObject>

class DuiGLShaderPrivate;
class QGLContext;

/*!
 * \class DuiGLShader
 * \brief Class for managing gl shaders.
 *
 * Class offers an interface for creating shaders. Binary shaders are not
 * currently supported.
 *
 * \deprecated Since 0.16
 */
class DuiGLShader : QObject
{
    Q_OBJECT

public:

    //! Enumeration for shader type.
    enum ShaderType {VertexShader, FragmentShader};

    /*!
     * \brief Constructor.
     * \param type Type of the shader.
     * \param name Name for the shader.
     * \param ctx GL context. If NULL the currently active context is
     *        bound to this shader.
     */
    DuiGLShader(ShaderType type, const QString &name, const QGLContext *ctx = 0);

    /*!
     * Get the id of the shader. The id is used when linking shaders into programs.
     * \return Id of the shader.
     */
    quint32 id() const;

    /*!
     * Get the type of the shader.
     * \return Type of the shader;
     */
    ShaderType type() const;

    /*!
     * Clear all the code that has been added with addSource():
     */
    void clearSource();

    /*!
     * Add some source to shader.
     * \param newSource Ascii source to be added.
     */
    void addSource(const QString &newSource);

    /*!
     * Compile the shader from source.
     * \return True if the shader was compiled successfully, false if compiling fails.
     */
    bool compile();

    /*
     * Get status of the shader.
     * \return True if shader is valid and ready for linking, false if the
     *         shader is invalid (not compiled successfully or the source has
     *         changed after last compilation).
     */
    bool isValid() const;

    /*
     * Get link log.
     * \return Link log.
     */
    QString log() const;

    /*
     * Get name of the shader.
     * \return Name of the shader.
     */
    QString name() const;

private:

    //! Pointer to private implementation class object.
    DuiGLShaderPrivate *const d_ptr;

    Q_DECLARE_PRIVATE(DuiGLShader)
};

#endif
