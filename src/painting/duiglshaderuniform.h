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

#ifndef DUIGLSHADERUNIFORM_H
#define DUIGLSHADERUNIFORM_H

#include <QObject>
#include "duiexport.h"

class QSizeF;
class QPointF;
class QTransform;
class QColor;
class DuiGLShaderUniformPrivate;

//! Structure for vec2.
typedef struct {
    float x;
    float y;
} DuiGLVec2;

//! Structure for vec3.
typedef struct {
    float x;
    float y;
    float z;
} DuiGLVec3;

//! Structure for vec4.
typedef struct {
    float x;
    float y;
    float z;
    float w;
} DuiGLVec4;

//! Enumeration for the supported uniform types.
enum DuiGLUniformType {
    DuiGLUniformInvalid,    //! Invalid type
    DuiGLUniformFloat,
    DuiGLUniformFloatVec2,
    DuiGLUniformFloatVec3,
    DuiGLUniformFloatVec4,
    DuiGLUniformInt,
    DuiGLUniformIntVec2,
    DuiGLUniformIntVec3,
    DuiGLUniformIntVec4,
    DuiGLUniformMat2,
    DuiGLUniformMat3,
    DuiGLUniformMat4,
    DuiGLUniformSampler2D
};


/*!
 * \class DuiGLShaderUniform
 * \brief Class for defining shader uniforms.
 *
 * Class offers and interface for defining and setting shader uniforms.
 */
class DUI_EXPORT DuiGLShaderUniform
{

public:

    /*!
     * \brief Constructor.
     * \param type Type for the uniform.
     * \param id GL position id of the uniform.
     */
    DuiGLShaderUniform(DuiGLUniformType type, quint32 id);

    /*!
     * \brief Copy constructor. Used by QMap.
     */
    DuiGLShaderUniform(const DuiGLShaderUniform &);


    /*!
     * \brief Default constructor.
     */
    DuiGLShaderUniform();

    /*!
     * \brief Destructor.
     */
    virtual ~DuiGLShaderUniform();

    /*!
     * Get the type of the uniform.
     * \return Type of the uniform.
     */
    DuiGLUniformType type() const;

    /*!
     * Get the id of the uniform.
     * \return Id of the uniform.
     */
    quint32 id() const;

    /*!
     * Overloaded assignment operator for float.
     */
    const DuiGLShaderUniform &operator=(const float &) const;
    /*!
     * Overloaded assignment operator for DuiGLVec2.
     */
    const DuiGLShaderUniform &operator=(const DuiGLVec2 &) const;
    /*!
     * Overloaded assignment operator for QSizeF.
     */
    const DuiGLShaderUniform &operator=(const QSizeF &) const;
    /*!
     * Overloaded assignment operator for QPointF.
     */
    const DuiGLShaderUniform &operator=(const QPointF &) const;
    /*!
     * Overloaded assignment operator for DuiGLVec3.
     */
    const DuiGLShaderUniform &operator=(const DuiGLVec3 &) const;
    /*!
     * Overloaded assignment operator for DuiGLVec4.
     */
    const DuiGLShaderUniform &operator=(const DuiGLVec4 &) const;
    /*!
     * Overloaded assignment operator for QColor.
     */
    const DuiGLShaderUniform &operator=(const QColor &) const;
    /*!
     * Overloaded assignment operator for float[2][2].
     */
    const DuiGLShaderUniform &operator=(const float[2][2]) const;
    /*!
     * Overloaded assignment operator for float[3][3].
     */
    const DuiGLShaderUniform &operator=(const float[3][3]) const;
    /*!
     * Overloaded assignment operator for QTransform.
     */
    const DuiGLShaderUniform &operator=(const QTransform &) const;
    /*!
     * Overloaded assignment operator for float[4][4].
     */
    const DuiGLShaderUniform &operator=(const float[4][4]) const;
    /*!
     * Overloaded assignment operator for quint32.
     */
    const DuiGLShaderUniform &operator=(const quint32 &) const;
    /*!
     * Overloaded copy assigment operator. This used by QMap.
     */
    const DuiGLShaderUniform &operator=(const DuiGLShaderUniform &);

private:

    //! Pointer to private implementation class object.
    DuiGLShaderUniformPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(DuiGLShaderUniform)
    //Q_DISABLE_COPY(DuiGLShaderUniform)
};

#endif
