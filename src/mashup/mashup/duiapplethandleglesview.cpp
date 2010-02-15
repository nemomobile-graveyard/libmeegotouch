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

#include <QPainter>
#include "duiviewcreator.h"
#include <DuiApplication>
#include "duiapplethandle.h"
#include "duiapplethandleglesview.h"
#include "duiapplethandleglesview_p.h"
#include <QGLFramebufferObject>

DuiAppletHandleGLESViewPrivate::DuiAppletHandleGLESViewPrivate(DuiAppletHandle *appletHandle) :
    DuiAppletHandleViewPrivate(appletHandle),
    textureID(-1),
    brokenAppletPixmap(QPixmap())
{
    if (DuiGLRenderer::instance()->getProgram("DuiAppletHandleViewShader") == NULL) {
        // Create a fragment shader for blurring an image
        static const char *fragmentShader = "\
            varying mediump vec2 fragTexCoord;\
            uniform sampler2D texture0;\
            uniform mediump float width;\
            uniform mediump float height;\
            void main(void)\
            {\
                const int n = 3;\
                const mediump float l = float(n * 2 + 1);\
                const mediump float mult = 1.0 / (l * l);\
                gl_FragColor = vec4(0);\
                mediump vec2 coord = fragTexCoord + vec2(-float(n) * width, -float(n) * height);\
                for(int y = -n; y <= n; ++y) {\
                    for(int x = -n; x <= n; ++x) {\
                        gl_FragColor += texture2D(texture0, coord);\
                        coord.x += width;\
                    }\
                    coord += vec2(width * -l, height);\
                }\
                gl_FragColor *= mult;\
                lowp float c = (gl_FragColor.r * 0.3 + gl_FragColor.g * 0.59 + gl_FragColor.b * 0.11);\
                gl_FragColor.rgb = vec3(c, c, c);\
            }";

        DuiGLRenderer::instance()->createShader("DuiAppletHandleViewFragmentShader", fragmentShader, DuiGLRenderer::DuiShaderFragment);
        DuiGLRenderer::instance()->createProgram("DuiAppletHandleViewShader", "", "DuiAppletHandleViewFragmentShader");
    }
}

DuiAppletHandleGLESViewPrivate::~DuiAppletHandleGLESViewPrivate()
{
    destroyPixmaps();
}

void DuiAppletHandleGLESViewPrivate::destroyPixmaps()
{
    // Delete texture if there is one
    if (textureID >= 0) {
        DuiGLRenderer::instance()->viewport()->deleteTexture(textureID);
        textureID = -1;
    }

    brokenAppletPixmap = QPixmap();

    DuiAppletHandleViewPrivate::destroyPixmaps();
}

void DuiAppletHandleGLESViewPrivate::drawAppletBrokenState()
{
    Q_Q(const DuiAppletHandleGLESView);

    if (!DuiApplication::softwareRendering() && brokenAppletPixmap.isNull() && !localPixmap.isNull()) {
        textureID = DuiGLRenderer::instance()->viewport()->bindTexture(localPixmap);

        DuiAppletHandleViewUniformProvider uniforms(localPixmap.width(), localPixmap.height());

        QSize newSize = localPixmap.size() * q->model()->appletScale();
        QGLFramebufferObject fbo(newSize);
        fbo.bind();
        DuiGLRenderer::instance()->setViewportSize(newSize);
        DuiGLRenderer::instance()->drawTexture("DuiAppletHandleViewShader",
                                               QTransform(),
                                               textureID,
                                               newSize,
                                               &uniforms);
        brokenAppletPixmap = QPixmap::fromImage(fbo.toImage());
        fbo.release();
    }

    DuiAppletHandleViewPrivate::drawAppletBrokenState();
}

void DuiAppletHandleGLESViewPrivate::drawAppletPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const
{
    // Draw the applet pixmap scaled so that it fits the screen
    if (brokenState && !brokenAppletPixmap.isNull()) {
        painter->drawPixmap(0, 0, brokenAppletPixmap);
    } else {
        painter->drawPixmap(targetGeometry, localPixmap, sourceGeometry);
    }
}

DuiAppletHandleGLESView::DuiAppletHandleGLESView(DuiAppletHandle *appletHandle) :
    DuiAppletHandleView(* new DuiAppletHandleGLESViewPrivate(appletHandle), appletHandle)
{
}

DuiAppletHandleGLESView::~DuiAppletHandleGLESView()
{
}

DuiAppletHandleViewUniformProvider::DuiAppletHandleViewUniformProvider(float width, float height) :
    width(qMax(width, 1.0f)), height(qMax(height, 1.0f))
{
}

DuiAppletHandleViewUniformProvider::~DuiAppletHandleViewUniformProvider()
{
}

bool DuiAppletHandleViewUniformProvider::setUniformValue(const QString &name, const DuiGLShaderUniform &uniform)
{
    if (name == "width") {
        uniform = 1.0f / width;
        return true;
    } else if (name == "height") {
        uniform = 1.0f / height;
        return true;
    }

    return false;
}

DUI_REGISTER_VIEW_NEW(DuiAppletHandleGLESView, DuiAppletHandle)
