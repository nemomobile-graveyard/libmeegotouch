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

#ifdef QT_OPENGL_LIB
#ifdef QT_OPENGL_ES_2
#include "duiglshaderprogram.h"
#include "duiglshader.h"
#include "duiglshaderuniform.h"

#include <QtOpenGL>


class DuiGLShaderProgramPrivate
{
public:
    DuiGLShaderProgramPrivate() : valid(false), programId(0), ctx(0) {}
    void populateVariableLists();
    DuiGLUniformType glUniformTypeToDuiUniformType(GLenum type);

    QVector<const DuiGLShader *>     shaders;
    DuiGLUniformList          uniforms;
    bool                    valid;
    GLuint                  programId;
    QGLContext             *ctx;
};



void DuiGLShaderProgramPrivate::populateVariableLists()
{
    uniforms.clear();

    int count;
    int sizeOfNameBuff;
    char *name;
    GLint nameLength;
    GLenum type;
    GLint  size;
    GLint  location;

    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &sizeOfNameBuff);

    name = new char[sizeOfNameBuff];

    for (int i = 0; i < count; ++i) {
        nameLength = -1;
        glGetActiveUniform(programId, i, sizeOfNameBuff, &nameLength, &size, &type, name);
        if (nameLength == -1)
            continue;

        location = glGetUniformLocation(programId, name);
        uniforms.insert(QString::fromAscii(name), DuiGLShaderUniform(glUniformTypeToDuiUniformType(type), location));
    }

    delete[] name;
}

DuiGLUniformType DuiGLShaderProgramPrivate::glUniformTypeToDuiUniformType(GLenum type)
{
    DuiGLUniformType duitype = DuiGLUniformInvalid;
    switch (type) {
    case GL_FLOAT: {
        duitype = DuiGLUniformFloat;
    } break;
    case GL_FLOAT_VEC2: {
        duitype = DuiGLUniformFloatVec2;
    } break;
    case GL_FLOAT_VEC3: {
        duitype = DuiGLUniformFloatVec3;
    } break;
    case GL_FLOAT_VEC4: {
        duitype = DuiGLUniformFloatVec4;
    } break;
    case GL_INT: {
        duitype = DuiGLUniformInt;
    } break;
    case GL_INT_VEC2: {
        duitype = DuiGLUniformIntVec2;
    } break;
    case GL_INT_VEC3: {
        duitype = DuiGLUniformIntVec3;
    } break;
    case GL_INT_VEC4: {
        duitype = DuiGLUniformIntVec4;
    } break;
    case GL_FLOAT_MAT2: {
        duitype = DuiGLUniformMat2;
    } break;
    case GL_FLOAT_MAT3: {
        duitype = DuiGLUniformMat3;
    } break;
    case GL_FLOAT_MAT4: {
        duitype = DuiGLUniformMat4;
    } break;
    case GL_SAMPLER_2D: {
        duitype = DuiGLUniformSampler2D;
    } break;

    default: {
        qWarning() << "DuiGLShaderProgramPrivate::glUniformTypeToDuiUniformType(): Unsupported uniform " << type;
    }
    }

    return duitype;
}

DuiGLShaderProgram::DuiGLShaderProgram(const QGLContext *ctx)
    : d_ptr(new DuiGLShaderProgramPrivate)
{
    //qWarning("DuiGLShaderProgram is DEPRECATED. Use QGLShaderProgram instead.");


    Q_D(DuiGLShaderProgram);
    if (!ctx)
        ctx = QGLContext::currentContext();

    if (!ctx) {
        qWarning("DuiGLShaderProgram being created without a context");
        return;
    }

    d->ctx = const_cast<QGLContext *>(ctx);
    d->ctx->makeCurrent();

    d->programId = glCreateProgram();

    d->valid = false;
}


const DuiGLUniformList &DuiGLShaderProgram::uniforms() const
{
    Q_D(const DuiGLShaderProgram);
    return const_cast<const DuiGLUniformList &>(d->uniforms);
}

const DuiGLShaderUniform &DuiGLShaderProgram::uniform(const QString &name) const
{
    Q_D(const DuiGLShaderProgram);
    const DuiGLUniformList::const_iterator end = d->uniforms.constEnd();
    const DuiGLUniformList::const_iterator i = d->uniforms.constFind(name);
    if (i != end)
        return *i;
    else {
        qWarning("Unknown uniform \"%s\" Either the uniform doesn't exist or it was removed at shader link.", name.toStdString().c_str());
        const static DuiGLShaderUniform u;
        return u;
    }
}


bool DuiGLShaderProgram::addShader(const DuiGLShader *shader)
{
    Q_D(DuiGLShaderProgram);
    if (!shader || !d->ctx)
        return false;

    //if (newShader->context() != d->ctx) {
    //    qWarning("Shader object's context does not match program's context");
    //    return false;
    //}

    if (!shader->isValid())
        return false;

    QGLContext *ctx = d->ctx;
    if (!ctx)
        return false;
    ctx->makeCurrent();

    glAttachShader(d->programId, shader->id());

    d->shaders.append(shader);
    return true;
}


void DuiGLShaderProgram::removeShader(const DuiGLShader *shader)
{
    Q_D(DuiGLShaderProgram);

    int idx = d->shaders.indexOf(shader);

    if (idx == -1)
        return;

    d->shaders.remove(idx);

    QGLContext *ctx = d->ctx;
    if (!ctx)
        return;
    ctx->makeCurrent();

    glDetachShader(d->programId, shader->id());
}


bool DuiGLShaderProgram::removeAllShaders()
{
    Q_D(DuiGLShaderProgram);

    QGLContext *ctx = d->ctx;
    if (!ctx)
        return false;
    ctx->makeCurrent();

    foreach(const DuiGLShader * shader, d->shaders) {
        glDetachShader(d->programId, shader->id());
    }
    d->shaders.clear();
    return true;
}

bool DuiGLShaderProgram::link()
{
    Q_D(DuiGLShaderProgram);

    QGLContext *ctx = d->ctx;
    if (!ctx)
        return false;
    ctx->makeCurrent();

    glLinkProgram(d->programId);


    GLint linked;
    glGetProgramiv(d->programId, GL_LINK_STATUS, &linked);

    if (!linked)
        return false;

    d->populateVariableLists();

    d->valid = true;
    return true;
}

void DuiGLShaderProgram::use() const
{
    qWarning("DuiGLShaderProgram is DEPRECATED. Use QGLShaderProgram instead.");

    Q_D(const DuiGLShaderProgram);
    if (!d->valid)
        return;

    glUseProgram(d->programId);
}


QString DuiGLShaderProgram::log() const
{
    Q_D(const DuiGLShaderProgram);

    QGLContext *ctx = d->ctx;
    if (!ctx)
        return QString();
    //ctx->makeCurrent();

    GLint  logSize = -666;
    glGetProgramiv(d->programId, GL_INFO_LOG_LENGTH, &logSize);

    char *logData = new char[logSize];
    GLint  logLength;

    glGetProgramInfoLog(d->programId, logSize, &logLength, logData);

    QString result = QString::fromAscii(logData);
    delete [] logData;

    return result;
}

quint32 DuiGLShaderProgram::id() const
{
    Q_D(const DuiGLShaderProgram);
    return d->programId;
}

#endif
#endif
