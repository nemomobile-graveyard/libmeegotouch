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
#include "gridvideowidget.h"
#include "gridmodel.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QFileInfo>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPinchGesture>

#include <MImageWidget>
#include <MLabel>
#ifdef HAVE_GSTREAMER
#include <MVideoWidget>
#endif
#include <MButton>
#include <MSlider>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MComponentData>

const int ANIMATION_TIME = 1000;
const int INACTIVITY_TIMEOUT = 5000;

#ifdef HAVE_GSTREAMER

MyVideoWidget::MyVideoWidget(QGraphicsItem *parent)
    : MVideoWidget(parent)
{
}

void MyVideoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MVideoWidget::mousePressEvent(event);
    event->accept();
}

void MyVideoWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MVideoWidget::mouseReleaseEvent(event);
    emit clicked();
}

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
    if (zoom < 0.02)
        zoom = 0.02;
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
    qreal dx = (widgetSize.width() - t.width()) / 2.0;
    qreal dy = (widgetSize.height() - t.height()) / 2.0;

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
    qreal dx = (originalSize.width() - sourceSize.width()) / 2.0;
    qreal dy = (originalSize.height() - sourceSize.height()) / 2.0;

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

MyVideoOverlayToolbar::MyVideoOverlayToolbar(QGraphicsItem *parent)
    : MWidgetController(parent),
      landscapePolicy(0),
      portraitPolicy(0)
{
    MLayout *layout = new MLayout(this);

    landscapePolicy = new MGridLayoutPolicy(layout);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);

    portraitPolicy = new MGridLayoutPolicy(layout);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
}

MyVideoOverlayToolbar::~MyVideoOverlayToolbar()
{
}

void MyVideoOverlayToolbar::addItem(QGraphicsLayoutItem* button)
{
    int count = landscapePolicy->count();

    int row = count / 3;
    int column = count % 3;

    landscapePolicy->addItem(button, row, column);
    portraitPolicy->addItem(button, row, column);
}

#endif

ItemDetailPage::ItemDetailPage() :
      TimedemoPage(),
      layout(0),
      policy(0),

      slider(0),
      button(0),
      image(0),

#ifdef HAVE_GSTREAMER
      video(0),
      lContainer(0),
      rContainer(0),
      tContainer(0),
      bContainer(0),
#endif

      hideAnimation(0),
      showAnimation(0),

      imageId(),
      videoId(),

      inactivityTimer(),

      lastScaleFactor(1.0),
      pinching(false)
{
    setObjectName("itemDetailPage");
}

ItemDetailPage::~ItemDetailPage()
{
#ifdef HAVE_GSTREAMER
    delete video;
#endif
    delete slider;

    delete hideAnimation;
    delete showAnimation;
}

QString ItemDetailPage::timedemoTitle()
{
    return "ItemDetailPage";
}

void ItemDetailPage::createContent()
{
    inactivityTimer.setInterval(INACTIVITY_TIMEOUT);
    connect(&inactivityTimer, SIGNAL(timeout()),
               this, SLOT(hideOverlay()),
               Qt::UniqueConnection);

    QGraphicsWidget *panel = centralWidget();
    layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);

#ifdef HAVE_GSTREAMER
    if (!videoId.isEmpty()) {
        QFileInfo info(videoId);
        setTitle(info.fileName());

        video = new MyVideoWidget(panel);
        connect(video, SIGNAL(videoReady()), this, SLOT(videoReady()));

//set video to fullscreen mode immediately only on device where the
//Xv rendering with color-key is supported for sure, to avoid flickering
//on desktop machines that does not support color-keying.
#ifdef __arm__
        video->setFullscreen(true);
#endif
        video->open(videoId);
        video->setAspectRatioMode(MVideoWidgetModel::AspectRatioScaled);
        connect(video, SIGNAL(clicked()), this, SLOT(buttonClicked()));

        button = new MButton(panel);
        button->setViewType(MButton::iconType);
        button->setObjectName("video-player-button");
        button->setIconID("icon-m-toolbar-mediacontrol-pause");
        connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

        MButton* bPrev = new MButton(panel);
        bPrev->setViewType(MButton::iconType);
        bPrev->setObjectName("video-player-button");
        bPrev->setIconID("icon-m-toolbar-mediacontrol-previous");
        connect(bPrev, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

        MButton* bNext = new MButton(panel);
        bNext->setViewType(MButton::iconType);
        bNext->setObjectName("video-player-button");
        bNext->setIconID("icon-m-toolbar-mediacontrol-next");
        connect(bNext, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));

        MButton* bBack = new MButton(panel);
        bBack->setViewType(MButton::iconType);
        bBack->setObjectName("MNavigationBarBackButton");
        connect(bBack, SIGNAL(clicked(bool)), this, SLOT(dismiss()));

        slider = new MSlider(panel);
        slider->setObjectName("video-player-slider");
        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(videoSliderValueChanged(int)));
        connect(slider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
        connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
        slider->setMinLabelVisible(true);
        slider->setMaxLabelVisible(true);

        cContainer = new MyVideoOverlayToolbar(panel);
        cContainer->addItem(bPrev);
        cContainer->addItem(button);
        cContainer->addItem(bNext);
        cContainer->setObjectName("center-overlay-container");
        cContainer->setViewType("background");
        cContainer->setGeometry(QRect(0,0,100,100));

        bContainer = new MyVideoOverlayToolbar(panel);
        bContainer->addItem(bBack);
        bContainer->addItem(slider);
        bContainer->setObjectName("bottom-overlay-container");
        bContainer->setViewType("background");
        bContainer->setGeometry(QRect(0,0,100,100));

        MLabel* label = new MLabel(panel);
        label->setObjectName("video-title-label");
        label->setText(info.fileName());
        label->setAlignment(Qt::AlignCenter);

        tContainer = new MyVideoOverlayToolbar(panel);
        tContainer->addItem(label);
        tContainer->setObjectName("top-overlay-container");
        tContainer->setViewType("background");
        tContainer->setGeometry(QRect(0,0,label->preferredWidth(),0));

        cContainer->setOpacity(0.0);
        tContainer->setOpacity(0.0);
        bContainer->setOpacity(0.0);

        hideAnimation = new QParallelAnimationGroup();
        showAnimation = new QParallelAnimationGroup();

        QPropertyAnimation* animation = new QPropertyAnimation(cContainer, "opacity");
        animation->setDuration(ANIMATION_TIME);
        animation->setEndValue(0.0);
        hideAnimation->addAnimation(animation);

        animation = new QPropertyAnimation(tContainer, "opacity");
        animation->setDuration(ANIMATION_TIME);
        animation->setEndValue(0.0);
        hideAnimation->addAnimation(animation);

        animation = new QPropertyAnimation(bContainer, "opacity");
        animation->setDuration(ANIMATION_TIME);
        animation->setEndValue(0.0);
        hideAnimation->addAnimation(animation);

        animation = new QPropertyAnimation(cContainer, "opacity");
        animation->setDuration(ANIMATION_TIME);
        animation->setEndValue(1.0);
        showAnimation->addAnimation(animation);

        animation = new QPropertyAnimation(tContainer, "opacity");
        animation->setDuration(ANIMATION_TIME);
        animation->setEndValue(1.0);
        showAnimation->addAnimation(animation);

        animation = new QPropertyAnimation(bContainer, "opacity");
        animation->setDuration(ANIMATION_TIME);
        animation->setEndValue(1.0);
        showAnimation->addAnimation(animation);

        relayout();
        ungrabKeyboard();
        ungrabGesture(Qt::PinchGesture);
    } else if( !imageId.isEmpty() ) {
#else
    if (!imageId.isEmpty()) {
#endif
        setPannable(false);

        policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policy->setSpacing(0.0);
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
        grabGesture(Qt::PinchGesture);
    }
    retranslateUi();
}

void ItemDetailPage::retranslateUi()
{
}

void ItemDetailPage::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MApplicationPage::resizeEvent(event);
    relayout();
}

void ItemDetailPage::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
    if (!image)
        return;

    if (gesture->state() == Qt::GestureStarted) {
        lastScaleFactor = 1.0;
        pinching = true;
    }

    if (gesture->state() == Qt::GestureFinished || gesture->state() == Qt::GestureCanceled)
        pinching = false;

    qreal scale = image->zoomFactor() * (gesture->scaleFactor() - lastScaleFactor + 1);
    // FIXME: Once the updated pinch gesture arrives replace the above calcuation with the one below.
    // qreal scale = image->zoomFactor() * gesture->scaleFactor();
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
    image->setOffset(image->offset() + delta);
    image->update();
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

void ItemDetailPage::relayout()
{
#ifdef HAVE_GSTREAMER
    if( video ) {
        const QSizeF& s = size();
        QPoint cPos = QPoint(((s.width() / 2) - (cContainer->size().width()/2)),
                             ((s.height() / 2) - (cContainer->size().height()/2)));
        QPoint bPos = QPoint(((s.width() / 2) - (bContainer->size().width()/2)),
                             (s.height() - bContainer->size().height()));
        QPoint tPos = QPoint(((s.width() / 2) - (tContainer->size().width()/2)),
                              0);

        cContainer->setPos(cPos);
        bContainer->setPos(bPos);
        tContainer->setPos(tPos);

        video->setGeometry(QRectF(0,0,s.width(), s.height()));
    }
#endif
}

void ItemDetailPage::showOverlay()
{
    hideAnimation->stop();
    showAnimation->start();
    inactivityTimer.start();

    setComponentsDisplayMode(MApplicationPage::HomeButton | MApplicationPage::EscapeButton, MApplicationPageModel::Show);
}

void ItemDetailPage::hideOverlay()
{
    if( slider->state() == MSliderModel::Pressed ) {
        inactivityTimer.start();
    }
    else {
        showAnimation->stop();
        hideAnimation->start();
        inactivityTimer.stop();
        setComponentsDisplayMode(MApplicationPage::HomeButton | MApplicationPage::EscapeButton, MApplicationPageModel::Hide);
    }
}

void ItemDetailPage::videoReady()
{
#ifdef HAVE_GSTREAMER
    video->play();
    video->setMuted(false);
    video->setVolume(0.8);

    slider->setMinimum(0);
    slider->setMaximum(video->length());
    QTimer::singleShot(100, this, SLOT(updatePosition()));

    int minutes = (video->length() / 1000) / 60;
    int seconds = (video->length() / 1000) % 60;
    slider->setMinLabel("0:00");
    slider->setMaxLabel(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));

    setPannable(false);
    setAutoMarginsForComponentsEnabled(false);
    setComponentsDisplayMode(MApplicationPage::NavigationBar, MApplicationPageModel::Hide);

    showOverlay();

    setAutoMarginsForComponentsEnabled(false);
    setComponentsDisplayMode(MApplicationPage::NavigationBar/*MApplicationPage::AllComponents*/, MApplicationPageModel::Hide);
#endif
}

void ItemDetailPage::sliderPressed()
{
    inactivityTimer.start();
}

void ItemDetailPage::sliderReleased()
{
    inactivityTimer.start();
}

void ItemDetailPage::videoSliderValueChanged(int newValue)
{
#ifdef HAVE_GSTREAMER
    if( video ) {
        video->seek(newValue);
    }
#else
    Q_UNUSED(newValue)
#endif
}

void ItemDetailPage::buttonClicked()
{
#ifdef HAVE_GSTREAMER
    if( cContainer->opacity() == 1 )
        inactivityTimer.start();
    else
        showOverlay();

    MyVideoWidget* v = qobject_cast<MyVideoWidget*>(sender());
    if( !v ) {
        MButton* b = qobject_cast<MButton*>(sender());
        if( b == button ) {
            if( video->state() == MVideo::Playing ) {
                video->pause();
                button->setIconID("icon-m-toolbar-mediacontrol-play");
            } else {
                video->play();
                button->setIconID("icon-m-toolbar-mediacontrol-pause");
            }
        } else
            video->setFullscreen(!video->isFullscreen());
    }
#endif
}


void ItemDetailPage::updatePosition()
{
#ifdef HAVE_GSTREAMER
    if( video ) {
        slider->setValue(video->position());
        QTimer::singleShot(100, this, SLOT(updatePosition()));
    }
#endif
}
