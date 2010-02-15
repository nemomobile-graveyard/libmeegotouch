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

#ifndef DUIIMAGEWIDGET_STUB
#define DUIIMAGEWIDGET_STUB

#include "duiimagewidget.h"
#include <stubbase.h>
#include <duiwidgetcontroller_stub.h>


// 1. DECLARE STUB
class DuiImageWidgetStub : public StubBase
{
public:
    virtual void DuiImageWidgetStubConstructor(QGraphicsWidget *parent);
    virtual void DuiImageWidgetStubDestructor();
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

    DuiImageWidgetPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiImageWidgetStub::DuiImageWidgetStubConstructor(QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

}
void DuiImageWidgetStub::DuiImageWidgetStubDestructor()
{
}

void DuiImageWidgetStub::setImageName(const QString &imageName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(imageName));
    stubMethodEntered("setImageName", params);
}

void DuiImageWidgetStub::setTargetSize(const QSizeF &size)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF>(size));
    stubMethodEntered("setTargetSize", params);
}

void DuiImageWidgetStub::setZoomFactor(float factor)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(factor));
    stubMethodEntered("setZoomFactor", params);
}

void DuiImageWidgetStub::setZoomFactor(float fx, float fy)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(fx));
    params.append(new Parameter<float>(fy));
    stubMethodEntered("setZoomFactor", params);
}

void DuiImageWidgetStub::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::AspectRatioMode>(mode));
    stubMethodEntered("setAspectRatioMode", params);
}

void DuiImageWidgetStub::setCropSize(const QSizeF &size, const QPointF &topLeft)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QSizeF>(size));
    params.append(new Parameter<QPointF>(topLeft));
    stubMethodEntered("setCropSize", params);
}

void DuiImageWidgetStub::setBorders(float left, float top, float right, float bottom)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(left));
    params.append(new Parameter<float>(top));
    params.append(new Parameter<float>(right));
    params.append(new Parameter<float>(bottom));
    stubMethodEntered("setBorders", params);
}

void DuiImageWidgetStub::setBackgroundColor(const QColor &color)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QColor>(color));
    stubMethodEntered("setBackgroundColor", params);
}

void DuiImageWidgetStub::setBackgroundOpacity(const float opacity)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<float>(opacity));
    stubMethodEntered("setBackgroundOpacity", params);
}

void DuiImageWidgetStub::setImage(const QImage &image)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QImage>(image));
    stubMethodEntered("setImage", params);
}

void DuiImageWidgetStub::setPixmap(const QPixmap &pixmap)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPixmap>(pixmap));
    stubMethodEntered("setPixmap", params);
}

QSizeF DuiImageWidgetStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

void DuiImageWidgetStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiImageWidgetStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mouseReleaseEvent", params);
}


// 3. CREATE A STUB INSTANCE
DuiImageWidgetStub gDefaultDuiImageWidgetStub;
DuiImageWidgetStub *gDuiImageWidgetStub = &gDefaultDuiImageWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiImageWidget::DuiImageWidget(DuiWidget *parent)
{
    gDuiImageWidgetStub->DuiImageWidgetStubConstructor(parent);
}

DuiImageWidget::DuiImageWidget(const QString &imageName, DuiWidget *parent)
{
    Q_UNUSED(imageName);
    gDuiImageWidgetStub->DuiImageWidgetStubConstructor(parent);
}

DuiImageWidget::DuiImageWidget(const QImage *image, DuiWidget *parent)
{
    Q_UNUSED(image);
    gDuiImageWidgetStub->DuiImageWidgetStubConstructor(parent);
}

DuiImageWidget::DuiImageWidget(const QPixmap *pixmap, DuiWidget *parent)
{
    Q_UNUSED(pixmap);
    gDuiImageWidgetStub->DuiImageWidgetStubConstructor(parent);
}

DuiImageWidget::~DuiImageWidget()
{
    gDuiImageWidgetStub->DuiImageWidgetStubDestructor();
}

void DuiImageWidget::setImageName(const QString &imageName)
{
    gDuiImageWidgetStub->setImageName(imageName);
}

void DuiImageWidget::setTargetSize(const QSizeF &size)
{
    gDuiImageWidgetStub->setTargetSize(size);
}

void DuiImageWidget::setZoomFactor(float factor)
{
    gDuiImageWidgetStub->setZoomFactor(factor);
}

void DuiImageWidget::setZoomFactor(float fx, float fy)
{
    gDuiImageWidgetStub->setZoomFactor(fx, fy);
}

void DuiImageWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    gDuiImageWidgetStub->setAspectRatioMode(mode);
}

void DuiImageWidget::setCropSize(const QSizeF &size, const QPointF &topLeft)
{
    gDuiImageWidgetStub->setCropSize(size, topLeft);
}

void DuiImageWidget::setBorders(float left, float top, float right, float bottom)
{
    gDuiImageWidgetStub->setBorders(left, top, right, bottom);
}

void DuiImageWidget::setBackgroundColor(const QColor &color)
{
    gDuiImageWidgetStub->setBackgroundColor(color);
}

void DuiImageWidget::setBackgroundOpacity(const float opacity)
{
    gDuiImageWidgetStub->setBackgroundOpacity(opacity);
}

void DuiImageWidget::setImage(const QImage &image)
{
    gDuiImageWidgetStub->setImage(image);
}

void DuiImageWidget::setPixmap(const QPixmap &pixmap)
{
    gDuiImageWidgetStub->setPixmap(pixmap);
}

QSizeF DuiImageWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiImageWidgetStub->sizeHint(which, constraint);
}

void DuiImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiImageWidgetStub->mousePressEvent(event);
}

void DuiImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiImageWidgetStub->mouseReleaseEvent(event);
}

#endif
