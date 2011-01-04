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

#include "itemdetailpage.h"
#include "gridmodel.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPinchGesture>

#ifdef QT_OPENGL_LIB
#include <QtOpenGL>
#endif

#include <MImageWidget>
#include <MLabel>
#include <MButton>
#include <MSlider>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MComponentData>

MyImageWidget::MyImageWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
    scaleK(0.0)
{

}

void MyImageWidget::setImage(const QImage &image)
{
    this->image = image;
    sourceRect = image.rect();

    updateImageGeometry();
}

void MyImageWidget::updateImageGeometry()
{
    QSizeF imageSize = image.size();

    calculateDrawRect(imageSize);
    calculateSourceRect(imageSize);
}

void MyImageWidget::setZoomFactor(qreal zoom)
{
    if (zoom > 50)
        zoom = 50;
    if (zoom < 0.02f)
        zoom = 0.02f;
    scaleK = zoom;
    updateImageGeometry();
}

qreal MyImageWidget::zoomFactor()
{
    if (scaleK == 0) {
        scaleK = qMin(size().width() / image.width(), size().height() / image.height());
    }
    return scaleK;
}

void MyImageWidget::setOffset(const QPointF &offset)
{
    paintOffset = offset;
    updateImageGeometry();
}

QPointF MyImageWidget::offset()
{
    return paintOffset;
}

void MyImageWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);
    updateImageGeometry();
}

void MyImageWidget::calculateDrawRect(const QSizeF &imageSize)
{
    // no image, return
    if (imageSize.isEmpty())
        return;

    // get target size, bounded by widget size
    QSizeF widgetSize = size();
    QSizeF targetSize = widgetSize;
    QSizeF t;

    // get the image display size
    qreal fx = zoomFactor(), fy = zoomFactor();


    t.setWidth(imageSize.width()*fx);
    t.setHeight(imageSize.height()*fy);

    // limited by target size
    t = targetSize.boundedTo(t);

    // calculate the rectangle of draw
    qreal dx = (widgetSize.width() - t.width()) / 2.f;
    qreal dy = (widgetSize.height() - t.height()) / 2.f;

    // calculate draw rect   
    targetRect.setRect(dx, dy, t.width(), t.height());

    if (dx > 0)
        paintOffset.setX(0);
    if (dy > 0)
        paintOffset.setY(0);
}

QSizeF MyImageWidget::calculateSourceSize(const QSizeF &imageSize)
{
    QSizeF sourceSize = imageSize;
    QSizeF targetSize = size();

    // protection codes
    if (sourceSize.width() < 1.0)
        sourceSize.setWidth(1.0);
    if (sourceSize.height() < 1.0)
        sourceSize.setHeight(1.0);

    QSizeF t;

    // get the image display size
    qreal fx = zoomFactor(), fy = zoomFactor();

    t.setWidth(imageSize.width()*fx);
    t.setHeight(imageSize.height()*fy);

    // update sourceSize for crop section by compare with targetSize, simulate zoom effect
    qreal value;
    if (t.width() > targetSize.width()) {
        value = sourceSize.width();
        sourceSize.setWidth(qRound(value * targetSize.width() / t.width()));
    }
    if (t.height() > targetSize.height()) {
        value = sourceSize.height();
        sourceSize.setHeight(qRound(value * targetSize.height() / t.height()));
    }

    return sourceSize;
}

void MyImageWidget::calculateSourceRect(const QSizeF &imageSize)
{
    QSizeF originalSize = image.size();
    QSizeF sourceSize = calculateSourceSize(imageSize);

    // calculate default crop section
    qreal dx = (originalSize.width() - sourceSize.width()) / 2.f;
    qreal dy = (originalSize.height() - sourceSize.height()) / 2.f;

    qreal xOffset = paintOffset.x() / zoomFactor();
    qreal yOffset = paintOffset.y() / zoomFactor();

    if (dx + sourceSize.width() + xOffset > originalSize.width()) {
        xOffset = originalSize.width() - dx - sourceSize.width();
    }

    if (dx + xOffset > 0)
        dx += xOffset;
    else {
        xOffset = -dx;
        dx = 0;
    }

    if (dy + sourceSize.height() + yOffset > originalSize.height()) {
        yOffset = originalSize.height() - dy - sourceSize.height();
    }

    if (dy + yOffset > 0)
        dy += yOffset;
    else {
        yOffset = -dy;
        dy = 0;
    }

    paintOffset.setX(xOffset * zoomFactor());
    paintOffset.setY(yOffset * zoomFactor());

    sourceRect = QRect(dx, dy, sourceSize.width(), sourceSize.height());
}

void MyImageWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->drawImage(targetRect, image, sourceRect);
}

ItemDetailPage::ItemDetailPage() :
      TimedemoPage(),
      layout(0),
      policy(0),
      image(0),
      imageId(),
      pinching(false),
      lastScaleFactor(1.f)
{
    setObjectName("itemDetailPage");
}

ItemDetailPage::~ItemDetailPage()
{
}

QString ItemDetailPage::timedemoTitle()
{
    return "ItemDetailPage";
}

void ItemDetailPage::createContent()
{
    QGraphicsWidget *panel = centralWidget();
    layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);

    if (!imageId.isEmpty()) {
        setPannable(false);

        policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policy->setSpacing(0.f);
        layout->setPolicy(policy);

        QImage realImage(imageId);

#ifdef QT_OPENGL_LIB
        int maxTextureSize = -1;
        if (!MComponentData::softwareRendering()) {
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        }

        if (!MComponentData::softwareRendering() && (realImage.size().width() > maxTextureSize || realImage.size().height() > maxTextureSize))
            realImage = realImage.scaled(QSize(maxTextureSize, maxTextureSize), Qt::KeepAspectRatio, Qt::SmoothTransformation);
#endif
        image = new MyImageWidget(centralWidget());
        image->setImage(realImage);

        image->setZoomFactor(qMin(size().width() / realImage.width(), size().height() / realImage.height()));

        policy->addItem(image);

        setTitle(QFileInfo(imageId).fileName());

        // go fullscreen
        setComponentsDisplayMode(MApplicationPage::NavigationBar,
                                       MApplicationPageModel::AutoHide);
        grabKeyboard();        
    }
    retranslateUi();
}

void ItemDetailPage::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
    if (!image)
        return;

    if (gesture->state() == Qt::GestureStarted) {
        lastScaleFactor = 1.f;
        pinching = true;
    }

    if (gesture->state() == Qt::GestureFinished || gesture->state() == Qt::GestureCanceled)
        pinching = false;

    qreal scale = image->zoomFactor() * gesture->scaleFactor();
    image->setZoomFactor(scale);

    lastScaleFactor = gesture->scaleFactor();
    event->accept(gesture);
}

void ItemDetailPage::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    lastMousePosition = event->pos();
}

void ItemDetailPage::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!image && pinching)
        return;

    QPointF delta = -(event->pos() - lastMousePosition);
    lastMousePosition = event->pos();

    if (image) {
        image->setOffset(image->offset() + delta);
        image->update();
    }
}

void ItemDetailPage::keyPressEvent(QKeyEvent *event)
{
    if (!image)
        return;

    if (event->key() == Qt::Key_Plus)
        image->setZoomFactor(image->zoomFactor() * 1.5);
    else if (event->key() == Qt::Key_Minus)
        image->setZoomFactor(image->zoomFactor() / 1.5);

    image->update();
}
