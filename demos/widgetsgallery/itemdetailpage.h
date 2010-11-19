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

#ifndef ITEMDETAILPAGE_H
#define ITEMDETAILPAGE_H

#include "timedemopage.h"
#include "gridmodel.h"

#include <MImageWidget>
#include <MWidgetController>
#include <MWidgetModel>
#include <QTimer>

class MLayout;
class MLinearLayoutPolicy;
class MGridLayoutPolicy;

class MyImageWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    MyImageWidget(QGraphicsItem *parent=0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setImage(const QImage &image);

    void setZoomFactor(qreal zoom);
    qreal zoomFactor();

    inline void setOffset(qreal x, qreal y) {
        setOffset(QPointF(x, y));
    }

    void setOffset(const QPointF &offset);
    QPointF offset();

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

    void calculateDrawRect(const QSizeF &imageSize);
    QSizeF calculateSourceSize(const QSizeF &imageSize);
    void calculateSourceRect(const QSizeF &imageSize);
    void updateImageGeometry();

private:
    QImage image;
    QRect sourceRect;
    QRect targetRect;
    qreal scaleK;
    QPointF paintOffset;
};

//page for showing video or image in it's native size
class ItemDetailPage  : public TimedemoPage
{
    Q_OBJECT

public:
    ItemDetailPage();
    virtual ~ItemDetailPage();

    virtual QString timedemoTitle();
    void setImageId(const QString& id) {imageId = id;}

    virtual void createContent();

protected:
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:

    void  relayout();

    MLayout* layout;
    MLinearLayoutPolicy* policy;
    MyImageWidget *image;

    QString imageId;
    bool pinching;
    qreal lastScaleFactor;
    QPointF lastMousePosition;
};

#endif
