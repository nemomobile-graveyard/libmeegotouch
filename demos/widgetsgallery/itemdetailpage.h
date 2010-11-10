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

#include <MContainer>
#include <MImageWidget>
#ifdef HAVE_GSTREAMER
#include <MVideoWidget>
#endif
#include <MWidgetController>
#include <MWidgetModel>
#include <QTimer>

class MLayout;
class MSlider;
class MButton;
class MLinearLayoutPolicy;
class MGridLayoutPolicy;
class QParallelAnimationGroup;

#ifdef HAVE_GSTREAMER

//video widget which emits clicked signal
class MyVideoWidget : public MVideoWidget
{
    Q_OBJECT

    public:
        MyVideoWidget(QGraphicsItem *parent = 0);
Q_SIGNALS:
        void clicked();

    protected:

         virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
         virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
};

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

//container widget for the video playback controls
class MyVideoOverlayToolbar : public MWidgetController
{
public:
    M_CONTROLLER(MWidget)

    MyVideoOverlayToolbar(QGraphicsItem *parent = 0);
    virtual ~MyVideoOverlayToolbar();

    void addItem(QGraphicsLayoutItem *button);

    private:
        MGridLayoutPolicy  *landscapePolicy;
        MGridLayoutPolicy  *portraitPolicy;
};

#endif

//page for showing video or image in it's native size
class ItemDetailPage  : public TimedemoPage
{
    Q_OBJECT

public:
    ItemDetailPage();
    virtual ~ItemDetailPage();

    virtual QString timedemoTitle();

    void setVideoId(const QString& id) {videoId = id; imageId = "";}
    void setImageId(const QString& id) {imageId = id; videoId = "";}

    virtual void createContent();

public slots:

    void showOverlay();
    void hideOverlay();

    void videoReady();

    void sliderPressed();
    void sliderReleased();
    void videoSliderValueChanged(int newValue);

    void buttonClicked();
    void updatePosition();

signals:
protected:
    virtual void retranslateUi();
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:

    void  relayout();

    MLayout* layout;
    MLinearLayoutPolicy* policy;

    MSlider* slider;
    MButton* button;
    MyImageWidget *image;

#ifdef HAVE_GSTREAMER
    MyVideoWidget* video;
    MyVideoOverlayToolbar* cContainer;
    MyVideoOverlayToolbar* lContainer;
    MyVideoOverlayToolbar* rContainer;
    MyVideoOverlayToolbar* tContainer;
    MyVideoOverlayToolbar* bContainer;
#endif

    QParallelAnimationGroup* hideAnimation;
    QParallelAnimationGroup* showAnimation;

    QString imageId;
    QString videoId;

    QTimer inactivityTimer;

    qreal lastScaleFactor;

    bool pinching;

    QPointF lastMousePosition;
};

#endif
