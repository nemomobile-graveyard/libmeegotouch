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

#include "duiglshader.h"
#include <QtOpenGL>


class DuiGLShaderPrivate
{
public:
    DuiGLShaderPrivate() : shaderId(0), valid(false), ctx(0) {}

    GLuint                  shaderId;
    QString                 source;
    bool                    valid;
    DuiGLShader::ShaderType   type;
    QGLContext             *ctx;
    QString                 name;
};


DuiGLShader::DuiGLShader(DuiGLShader::ShaderType type, const QString &name, const QGLContext *ctx)
    : d_ptr(new DuiGLShaderPrivate)
{
    qWarning("DuiGLShader is DEPRECATED. Use QGLShader instead.");

    Q_D(DuiGLShader);

    if (!ctx)
        ctx = QGLContext::currentContext();

    if (!ctx) {
        qWarning("DuiGLShader being created without a context");
        return;
    }

    d->ctx = const_cast<QGLContext *>(ctx);
    d->ctx->makeCurrent();

    if (type == DuiGLShader::FragmentShader)
        d->shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    else
        d->shaderId = glCreateShader(GL_VERTEX_SHADER);

    if (d->shaderId == 0) {
        qWarning("Error creating shader object");
        return;
    }

    d->type = type;

    d->name = name;
}

quint32 DuiGLShader::id() const
{
    Q_D(const DuiGLShader);
    return d->shaderId;
}

DuiGLShader::ShaderType DuiGLShader::type() const
{
    Q_D(const DuiGLShader);
    return d->type;
}

QString DuiGLShader::name() const
{
    Q_D(const DuiGLShader);

    return d->name;
}


void DuiGLShader::clearSource()
{
    Q_D(DuiGLShader);
    d->source.clear();
    d->valid = false;
}

void DuiGLShader::addSource(const QString &newSource)
{
    Q_D(DuiGLShader);
    d->source += newSource;
    d->valid = false;
}


bool DuiGLShader::compile()
{
    Q_D(DuiGLShader);

    d->valid = false;

    if (d->source.size() == 0)
        return false;

    const QByteArray src_ba = d->source.toAscii();
    const char *src = src_ba.constData();

    glShaderSource(d->shaderId, 1, &src, 0);

    glCompileShader(d->shaderId);

    GLint shaderCompiled;
    glGetShaderiv(d->shaderId, GL_COMPILE_STATUS, &shaderCompiled);
    if (!shaderCompiled)
        return false;

    d->valid = true;
    return true;
}

bool DuiGLShader::isValid() const
{
    Q_D(const DuiGLShader);
    return d->valid;
}

QString DuiGLShader::log() const
{
    Q_D(const DuiGLShader);

    char  *logData;
    GLint  logSize;
    GLint  logLength;

    glGetShaderiv(d->shaderId, GL_INFO_LOG_LENGTH, &logSize);

    if (!logSize)
        return QString();

    logData = new char[logSize];
    glGetShaderInfoLog(d->shaderId, logSize, &logLength, logData);
    QString result = QString::fromAscii(logData);
    delete [] logData;

    return result;
}

#endif
#endif
