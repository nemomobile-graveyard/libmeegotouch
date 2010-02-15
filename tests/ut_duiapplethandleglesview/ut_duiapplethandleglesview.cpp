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

#include <QTest>
#include <QGLWidget>
#include <qgl.h>
#include "ut_duiapplethandleglesview.h"
#include <duiapplethandleview_p.h>
#include <duiappletcommunicator.h>
#include <duiapplication.h>
#include <duiapplethandle_stub.h>

DuiApplication *app;

// QPainter stubs
bool pixmapDrawn;

void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &pixmap, const QRectF &sourceRect)
{
    Q_UNUSED(targetRect)
    Q_UNUSED(pixmap)
    Q_UNUSED(sourceRect)

    pixmapDrawn = true;
}

// QGLWidget stubs
bool textureDeleted = false;

void QGLWidget::deleteTexture(GLuint)
{
    textureDeleted = true;
}

GLuint QGLWidget::bindTexture(const QPixmap &, GLenum, GLint)
{
    return 1;
}


// QGLFramebufferObject stubs
class QGLFramebufferObjectPrivate
{
};

QGLFramebufferObject::QGLFramebufferObject(const QSize &, GLenum)
{
}

QGLFramebufferObject::~QGLFramebufferObject()
{
}

bool QGLFramebufferObject::bind()
{
    return false;
}

QSize QGLFramebufferObject::size() const
{
    return QSize();
}

QImage QGLFramebufferObject::toImage() const
{
    return QImage();
}

bool QGLFramebufferObject::release()
{
    return false;
}

// DuiAppletHandleViewPrivate stubs
bool appletHandleViewPixmapsDestroyed = false;

void DuiAppletHandleViewPrivate::destroyPixmaps()
{
    appletHandleViewPixmapsDestroyed = true;
}

// DuiGLRenderer stubs
QGLWidget *DuiGLRenderer::viewport()
{
    static QGLWidget glWidget;

    return &glWidget;
}

bool glTextureDrawn;
QSizeF glTextureSize;

void DuiGLRenderer::drawTexture(const QString &, const QTransform &, quint32,
                                const QSizeF &size, IDuiGLUniformProvider *,
                                bool)
{
    glTextureDrawn = true;
    glTextureSize = size;
}

// DuiGLShaderUniform stubs
DuiGLShaderUniform::DuiGLShaderUniform() : d_ptr(0) {}
DuiGLShaderUniform::~DuiGLShaderUniform() {}

float uniformValue = 0;

const DuiGLShaderUniform &DuiGLShaderUniform::operator=(const float &v) const
{
    uniformValue = v;

    return *this;
}

// DuiWidgetController stubs
void DuiWidgetController::setGeometry(const QRectF &)
{
}

// DuiWidgetView stubs
DuiWidgetView::~DuiWidgetView()
{
}

void DuiWidgetView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    drawContents(painter, option);
}

void DuiWidgetView::update(const QRectF &)
{
    QPainter p;
    drawContents(&p, NULL);
}

// A test applet view implementation
DuiTestAppletHandleGLESView::DuiTestAppletHandleGLESView(DuiAppletHandle *handle) :
    DuiAppletHandleGLESView(handle)
{
}

// Unit test cases
void Ut_DuiAppletHandleGLESView::initTestCase()
{
    int argc = 1;
    char *app_name = (char *) "./ut_duiapplethandleglesview";
    app = new DuiApplication(argc, &app_name);
}


void Ut_DuiAppletHandleGLESView::cleanupTestCase()
{
    delete app;
}

void Ut_DuiAppletHandleGLESView::init()
{
    handle     = new DuiAppletHandle();
    handleView = new DuiTestAppletHandleGLESView(handle);

    glTextureDrawn = false;
    pixmapDrawn = false;
    uniformValue = 0;
    glTextureSize = QSizeF();
    appletHandleViewPixmapsDestroyed = false;
    textureDeleted = false;
}

void Ut_DuiAppletHandleGLESView::cleanup()
{
    delete handleView;
    delete handle;
}

void Ut_DuiAppletHandleGLESView::testBrokenAppletDrawing()
{
    DuiAppletHandleModel model;
    handleView->setModel(&model);

    // set the geometry, so a pixmap is created for the view
    handleView->setGeometry(QRectF(0, 0, 10, 10));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    model.setState(DuiAppletHandleModel::BROKEN);
    QVERIFY(glTextureDrawn);
    QVERIFY(pixmapDrawn);
    QCOMPARE(glTextureSize, QSizeF(10, 10));
}

void Ut_DuiAppletHandleGLESView::testPixmapAndTextureCleanup()
{
    DuiAppletHandleModel model;
    handleView->setModel(&model);

    // set the geometry, so a pixmap and broken view texture is created for the view
    handleView->setGeometry(QRectF(0, 0, 10, 10));
    QMetaObject::invokeMethod(handleView, "pixmapTakenIntoUse", Qt::DirectConnection, Q_ARG(Qt::HANDLE, 0));
    model.setState(DuiAppletHandleModel::BROKEN);
    // test that the old textures and pixmaps of the base class are destroyed when state is changed to running
    model.setState(DuiAppletHandleModel::RUNNING);
    // The destruction of the base class pixmaps can only be verified if DUI_EXPORT is used
//    QVERIFY(appletHandleViewPixmapsDestroyed);
    QVERIFY(textureDeleted);
}

void Ut_DuiAppletHandleGLESView::testUniforms()
{
    DuiGLShaderUniform u;
    DuiAppletHandleViewUniformProvider upm1(-1.0f, -1.0f);
    uniformValue = 0;
    upm1.setUniformValue("width", u);
    QCOMPARE(uniformValue, 1.0f);
    uniformValue = 0;
    upm1.setUniformValue("height", u);
    QCOMPARE(uniformValue, 1.0f);
    DuiAppletHandleViewUniformProvider up0(0.0f, 0.0f);
    uniformValue = 0;
    up0.setUniformValue("width", u);
    QCOMPARE(uniformValue, 1.0f);
    uniformValue = 0;
    up0.setUniformValue("height", u);
    QCOMPARE(uniformValue, 1.0f);
    DuiAppletHandleViewUniformProvider up1(1.0f, 1.0f);
    uniformValue = 0;
    up1.setUniformValue("width", u);
    QCOMPARE(uniformValue, 1.0f);
    uniformValue = 0;
    up1.setUniformValue("height", u);
    QCOMPARE(uniformValue, 1.0f);
    DuiAppletHandleViewUniformProvider up2(10.0f, 10.0f);
    uniformValue = 0;
    up2.setUniformValue("width", u);
    QCOMPARE(uniformValue, 0.1f);
    uniformValue = 0;
    up2.setUniformValue("height", u);
    QCOMPARE(uniformValue, 0.1f);
}


QTEST_APPLESS_MAIN(Ut_DuiAppletHandleGLESView)

