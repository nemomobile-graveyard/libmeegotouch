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

#ifndef MVIDEOWIDGETSHADERS_H
#define MVIDEOWIDGETSHADERS_H

static const char *MVideoVertShaderSource = "\
    attribute highp vec4 inputVertex; \
    attribute lowp  vec2 textureCoord; \
    uniform   highp mat4 pmvMatrix; \
    varying   lowp  vec2 fragTextureCoord; \
    void main(void) \
    {\
            gl_Position = pmvMatrix * inputVertex;\
            fragTextureCoord = textureCoord; \
    }";

static const char *MVideoVertShaderSource2 = "\
    attribute highp vec4 inputVertex; \
    attribute lowp  vec2 textureCoord; \
    uniform   highp mat4 matProj; \
    uniform   highp mat4 matWorld; \
    varying   lowp  vec2 fragTextureCoord; \
    void main(void) \
    {\
            gl_Position = matProj * matWorld * inputVertex;\
            fragTextureCoord = textureCoord; \
    }";

static const char *MRgbVideoFragShaderSource = "\
    varying lowp vec2 fragTextureCoord;\
    uniform sampler2D texture;\
    uniform lowp float opacity; \
    void main(void) \
    {\
        gl_FragColor = texture2D(texture, fragTextureCoord) * opacity; \
    }";

static const char *MYuvVideoFragShaderSource = "\
    varying lowp vec2 fragTextureCoord;\
    uniform sampler2D textureY;\
    uniform sampler2D textureU;\
    uniform sampler2D textureV;\
    uniform lowp float     opacity; \
    void main(void) \
    {\
        highp float y = texture2D(textureY, fragTextureCoord).x; \
        highp float u = texture2D(textureU, fragTextureCoord).x; \
        highp float v = texture2D(textureV, fragTextureCoord).x; \
        u = u - 0.5; \
        v = v - 0.5; \
        y = 1.164 * (y - 0.0625);\
        gl_FragColor = vec4(y + 1.403 * v, y - (0.344 * u) - (0.714 * v), y + (1.770 * u), 1.0) * opacity; \
    }";

static const char *MYuvVideoFragShaderSource2 = "\
    varying lowp vec2 fragTextureCoord;\
    uniform sampler2D textureYUV;\
    uniform lowp float     opacity; \
    void main(void) \
    {\
        highp vec3  yuv = texture2D(textureYUV, fragTextureCoord).xyz; \
        highp float y = yuv.x; \
        highp float u = yuv.y; \
        highp float v = yuv.z; \
        u = u - 0.5; \
        v = v - 0.5; \
        y = 1.164 * (y - 0.0625);\
        gl_FragColor = vec4(y + 1.403 * v, y - (0.344 * u) - (0.714 * v), y + (1.770 * u), 1.0) * opacity; \
    }";

/*
        y = y - 0.0625;\
        gl_FragColor = vec4(y + 1.403 * v, y - (0.344 * u) - (0.714 * v), y + (1.770 * u), 1.0); \


        y = 1.164 * (y - 0.0625);\
        gl_FragColor = vec4(y + 1.596 * v, y - (0.391 * u) - (0.813 * v), y + (2.018 * u), 1.0); \
*/

#endif
