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

#include <qconfig.h>
#include "duiglshaderuniform.h"

#ifdef QT_OPENGL_LIB
#ifdef QT_OPENGL_ES_2
#define DUI_USE_OPENGL
#include <QtOpenGL>
#endif
#endif

#ifdef DUI_USE_OPENGL
class DuiGLShaderUniformPrivate
{
public:
    DuiGLShaderUniformPrivate() : m_id(0), m_type(DuiGLUniformInvalid) {}

    GLuint  m_id;
    DuiGLUniformType m_type;
};

DuiGLShaderUniform::DuiGLShaderUniform(DuiGLUniformType type, quint32 id)
    : d_ptr(new DuiGLShaderUniformPrivate)
{
    //qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");

    d_ptr->m_id = id;
    d_ptr->m_type = type;
}

DuiGLShaderUniform::DuiGLShaderUniform(const DuiGLShaderUniform &uniform)
    : d_ptr(new DuiGLShaderUniformPrivate)
{
    //qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");

    d_ptr->m_id = uniform.id();
    d_ptr->m_type = uniform.type();
}

DuiGLShaderUniform::DuiGLShaderUniform()
    : d_ptr(new DuiGLShaderUniformPrivate)
{
    //qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    qWarning("Unknown uniform! Either the uniform doesn't exist or it was removed at shader link.");
}

DuiGLShaderUniform::~DuiGLShaderUniform()
{
    if (d_ptr)
        delete d_ptr;
}

DuiGLUniformType DuiGLShaderUniform::type() const
{
    return d_ptr->m_type;
}

quint32 DuiGLShaderUniform::id() const
{
    return d_ptr->m_id;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const GLfloat &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloat)
        return *this;

    glUniform1f(d_ptr->m_id, rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLVec2 &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloatVec2)
        return *this;

    glUniform2fv(d_ptr->m_id, 1, (const GLfloat *)&rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QSizeF &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloatVec2)
        return *this;

    glUniform2f(d_ptr->m_id, rhs.width(), rhs.height());

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QPointF &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloatVec2)
        return *this;

    glUniform2f(d_ptr->m_id, rhs.x(), rhs.y());

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLVec3 &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloatVec3)
        return *this;

    glUniform3fv(d_ptr->m_id, 1, (const GLfloat *)&rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLVec4 &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloatVec4)
        return *this;

    glUniform4fv(d_ptr->m_id, 1, (const GLfloat *)&rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QColor &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformFloatVec4)
        return *this;

    glUniform4f(d_ptr->m_id, rhs.redF(), rhs.greenF(), rhs.blueF(), rhs.alphaF());

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float rhs[2][2]) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformMat2)
        return *this;

    glUniformMatrix2fv(d_ptr->m_id, 1, GL_FALSE, (GLfloat *)rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float rhs[3][3]) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformMat3)
        return *this;

    glUniformMatrix3fv(d_ptr->m_id, 1, GL_FALSE, (GLfloat *)rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QTransform &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformMat3)
        return *this;

    // Transposes ready for GL
    GLfloat mat3[3][3] = {
        {rhs.m11(), rhs.m12(), rhs.m13()},
        {rhs.m21(), rhs.m22(), rhs.m23()},
        {rhs.m31(), rhs.m32(), rhs.m33()}
    };

    glUniformMatrix3fv(d_ptr->m_id, 1, GL_FALSE, (GLfloat *)mat3);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float rhs[4][4]) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformMat4)
        return *this;

    glUniformMatrix4fv(d_ptr->m_id, 1, GL_FALSE, (GLfloat *)rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const quint32 &rhs) const
{
    qWarning("DuiGLShaderUniform is DEPRECATED. Use QGLShaderProgram::setUniformValue() methods instead.");
    if (d_ptr->m_type != DuiGLUniformSampler2D)
        return *this;

    glUniform1i(d_ptr->m_id, rhs);

    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLShaderUniform &uniform)
{
    d_ptr->m_id = uniform.id();
    d_ptr->m_type = uniform.type();
    return *this;
}

#else

DuiGLShaderUniform::DuiGLShaderUniform(DuiGLUniformType, quint32)
    : d_ptr(NULL)
{
}

DuiGLShaderUniform::DuiGLShaderUniform()
    : d_ptr(NULL)
{
}

DuiGLShaderUniform::~DuiGLShaderUniform()
{
}

DuiGLUniformType DuiGLShaderUniform::type() const
{
    return DuiGLUniformInvalid;
}

quint32 DuiGLShaderUniform::id() const
{
    return 0;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLVec2 &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QSizeF &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QPointF &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLVec3 &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLVec4 &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QColor &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float[2][2]) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float[3][3]) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const QTransform &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float[4][4]) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const quint32 &) const
{
    return *this;
}

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const DuiGLShaderUniform &)
{
    return *this;
}


#endif
