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

#ifndef MIMAGEWIDGET_STUB
#define MIMAGEWIDGET_STUB

#include "mimagewidget.h"
#include <stubbase.h>
#include <mwidgetcontroller_stub.h>


// 1. DECLARE STUB
class MImageWidgetStub : public StubBase
{
public:
    virtual void MImageWidgetStubConstructor(QGraphicsWidget *parent);
    virtual void MImageWidgetStubDestructor();
    virtual void setImageName(const QString &imageName);
    virtual void setTargetSize(const QSizeF &size);
    virtual void setZoomFactor(float factor);
    virtual void setZoomFactor(float fx, float fy);
    virtual void setAspectRatioMode(Qt::AspectRatioMode mode);
    virtual void setCropSize(const QSizeF &size, const QPointF &topLeft = QPointF(-1.0, -1.0));
    virtual void setBorders(float left, float top, float right, float bottom);
    virtual void setBackgroundColor(const QColor &color);
    virtual void setBackgroundOpacity(const float opacity);
    virtual void setImage(const QImage &image);
    virtual void setPixmap(const QPixmap &pixmap);

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    MImageWidgetPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MImageWidgetStub::MImageWidgetStubConstructor(QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

}
void MImageWidgetStub::MImageWidgetStubDestructor()
{
}

void MImageWidgetStub::setImageName(const QString &imageName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(imageName));
    stubMethodEntered("setImageName", params);
}

void MImageWidgetStub::setTargetSize(const QSizeF &size)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF>(size));
    stubMethodEntered("setTargetSize", params);
}

void MImageWidgetStub::setZoomFactor(float factor)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(factor));
    stubMethodEntered("setZoomFactor", params);
}

void MImageWidgetStub::setZoomFactor(float fx, float fy)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(fx));
    params.append(new Parameter<float>(fy));
    stubMethodEntered("setZoomFactor", params);
}

void MImageWidgetStub::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::AspectRatioMode>(mode));
    stubMethodEntered("setAspectRatioMode", params);
}

void MImageWidgetStub::setCropSize(const QSizeF &size, const QPointF &topLeft)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF>(size));
    params.append(new Parameter<QPointF>(topLeft));
    stubMethodEntered("setCropSize", params);
}

void MImageWidgetStub::setBorders(float left, float top, float right, float bottom)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(left));
    params.append(new Parameter<float>(top));
    params.append(new Parameter<float>(right));
    params.append(new Parameter<float>(bottom));
    stubMethodEntered("setBorders", params);
}

void MImageWidgetStub::setBackgroundColor(const QColor &color)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QColor>(color));
    stubMethodEntered("setBackgroundColor", params);
}

void MImageWidgetStub::setBackgroundOpacity(const float opacity)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(opacity));
    stubMethodEntered("setBackgroundOpacity", params);
}

void MImageWidgetStub::setImage(const QImage &image)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QImage>(image));
    stubMethodEntered("setImage", params);
}

void MImageWidgetStub::setPixmap(const QPixmap &pixmap)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPixmap>(pixmap));
    stubMethodEntered("setPixmap", params);
}

QSizeF MImageWidgetStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

void MImageWidgetStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mousePressEvent", params);
}

void MImageWidgetStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mouseReleaseEvent", params);
}


// 3. CREATE A STUB INSTANCE
MImageWidgetStub gDefaultMImageWidgetStub;
MImageWidgetStub *gMImageWidgetStub = &gDefaultMImageWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MImageWidget::MImageWidget(MWidget *parent)
{
    gMImageWidgetStub->MImageWidgetStubConstructor(parent);
}

MImageWidget::MImageWidget(const QString &imageName, MWidget *parent)
{
    Q_UNUSED(imageName);
    gMImageWidgetStub->MImageWidgetStubConstructor(parent);
}

MImageWidget::MImageWidget(const QImage *image, MWidget *parent)
{
    Q_UNUSED(image);
    gMImageWidgetStub->MImageWidgetStubConstructor(parent);
}

MImageWidget::MImageWidget(const QPixmap *pixmap, MWidget *parent)
{
    Q_UNUSED(pixmap);
    gMImageWidgetStub->MImageWidgetStubConstructor(parent);
}

MImageWidget::~MImageWidget()
{
    gMImageWidgetStub->MImageWidgetStubDestructor();
}

void MImageWidget::setImageName(const QString &imageName)
{
    gMImageWidgetStub->setImageName(imageName);
}

void MImageWidget::setTargetSize(const QSizeF &size)
{
    gMImageWidgetStub->setTargetSize(size);
}

void MImageWidget::setZoomFactor(float factor)
{
    gMImageWidgetStub->setZoomFactor(factor);
}

void MImageWidget::setZoomFactor(float fx, float fy)
{
    gMImageWidgetStub->setZoomFactor(fx, fy);
}

void MImageWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    gMImageWidgetStub->setAspectRatioMode(mode);
}

void MImageWidget::setCropSize(const QSizeF &size, const QPointF &topLeft)
{
    gMImageWidgetStub->setCropSize(size, topLeft);
}

void MImageWidget::setBorders(float left, float top, float right, float bottom)
{
    gMImageWidgetStub->setBorders(left, top, right, bottom);
}

void MImageWidget::setBackgroundColor(const QColor &color)
{
    gMImageWidgetStub->setBackgroundColor(color);
}

void MImageWidget::setBackgroundOpacity(const float opacity)
{
    gMImageWidgetStub->setBackgroundOpacity(opacity);
}

void MImageWidget::setImage(const QImage &image)
{
    gMImageWidgetStub->setImage(image);
}

void MImageWidget::setPixmap(const QPixmap &pixmap)
{
    gMImageWidgetStub->setPixmap(pixmap);
}

QSizeF MImageWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMImageWidgetStub->sizeHint(which, constraint);
}

void MImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gMImageWidgetStub->mousePressEvent(event);
}

void MImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gMImageWidgetStub->mouseReleaseEvent(event);
}

#endif
