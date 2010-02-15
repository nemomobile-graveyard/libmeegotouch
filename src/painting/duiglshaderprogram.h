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

#ifndef DUIGLSHADERPROGRAM_H
#define DUIGLSHADERPROGRAM_H

#include <QMap>
#include <QObject>

class QGLContext;
class DuiGLShader;
class DuiGLShaderUniform;
class DuiGLShaderProgramPrivate;

//! Type definition for the uniform list.
typedef QMap<QString, DuiGLShaderUniform> DuiGLUniformList;

/*!
 * \class DuiGLShaderProgram
 * \brief Class for managing gl shader programs.
 *
 * Class offers and interface for creating shader programs from vertex and
 * fragment shaders. It also offers an interface for setting uniforms and
 * activating the shader program.
 *
 * \deprecated Since 0.16
 */
class DuiGLShaderProgram : QObject
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param ctx Pointer to the context where the program is bind to. If NULL,
     *        the program is bind to the currently active context.
     */
    DuiGLShaderProgram(const QGLContext *ctx = 0);

    /*!
     * Get uniform list. The program must be activated using the use() method
     * before setting the uniforms.
     *
     * \return List of uniforms found from the program
     */
    const DuiGLUniformList &uniforms() const;

    /*!
     * Get single named uniform. The program must be activated using the use() method
     * before setting the uniforms.
     *
     * \return Matching uniform, if the uniform does not exist an invalid uniform is returned.
     */
    const DuiGLShaderUniform &uniform(const QString &name) const;

    /*!
     * Add shader to program. Shader and program must be bound to same
     * context otherwise the add fails.
     *
     * \param shader Shader object to be add.
     * \return True if shader was added, false if add fails.
     */
    bool addShader(const DuiGLShader *shader);

    /*
     * Remove shader from program.
     * \param shader Shader to be removed.
     */
    void removeShader(const DuiGLShader *shader);

    /*
     * Remove all shaders from program.
     */
    bool removeAllShaders();

    /*
     * Link the program.
     *\return True if the linking of the program succeeds, false if the linking fails.
     */
    bool link();

    /*
     * Get link log.
     * \return Link log.
     */
    QString log() const;

    /*
     * Get status of the program.
     * \return True if program is valid and ready for activation, false if the
     *         program is invalid (not linked, link has failed.).
     */
    bool isValid() const;

    /*
     * Activate the program. Uniforms cannot be set before the program has been
     * activated. The program must be linked before it can be activated.
     */
    void use() const;

    /*
     * Get the id of the program.
     * \return id of the program.
     */
    quint32 id() const;

private:

    //! Pointer to private implementation class object.
    DuiGLShaderProgramPrivate *const d_ptr;

    Q_DECLARE_PRIVATE(DuiGLShaderProgram)
};

#endif
