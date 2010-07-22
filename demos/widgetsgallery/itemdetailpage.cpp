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
#include <MDebug>

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

MyVideoOverlayToolbar::MyVideoOverlayToolbar(QGraphicsItem *parent)
    : MWidgetController(parent)
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
      slider(0),
      image(0),
#ifdef HAVE_GSTREAMER
      video(0),
#endif
      hideAnimation(0),
      showAnimation(0),
      scaleFactor(10.0),
      lastScaleFactor(1.0)
{
    setObjectName("itemDetailPage");
    setAcceptTouchEvents(true);
    grabGesture(Qt::PinchGesture);
}

ItemDetailPage::~ItemDetailPage()
{
#ifdef HAVE_GSTREAMER
    delete video;
#endif
    delete slider;
    delete image;

    delete hideAnimation;
    delete showAnimation;
}

QString ItemDetailPage::timedemoTitle()
{
    return "ItemDetailPage";
}

void ItemDetailPage::createContent()
{
    mWarning("ItemDetailPage::createContent()");

    inactivityTimer.setInterval(INACTIVITY_TIMEOUT);
    connect(&inactivityTimer, SIGNAL(timeout()),
               this, SLOT(hideOverlay()),
               Qt::UniqueConnection);

    QGraphicsWidget *panel = centralWidget();
    layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);

#ifdef HAVE_GSTREAMER
    if( !videoId.isEmpty() ) {
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
    } else if( !imageId.isEmpty() ) {
#else
    if( !imageId.isEmpty() ) {
#endif
        policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policy->setSpacing(0.0);
        layout->setPolicy(policy);

        image = new MImageWidget(panel);
        image->setImage(QImage(imageId));
        policy->addItem(image);

        setTitle(QFileInfo(imageId).fileName());

        // go fullscreen
        setComponentsDisplayMode(MApplicationPage::NavigationBar,
                                       MApplicationPageModel::AutoHide);
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
    if (gesture->state() == Qt::GestureStarted) {
        lastScaleFactor = 1.0;
    }

    image->setZoomFactor(scaleFactor / 10.0);
    image->update();
    scaleFactor = scaleFactor * (gesture->scaleFactor() - lastScaleFactor + 1);

    lastScaleFactor = gesture->scaleFactor();

    if (scaleFactor < 1)
        scaleFactor = 1;
    else if (scaleFactor > 50)
        scaleFactor = 50;

    event->accept(gesture);
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

void ItemDetailPage::rateNoneButtonClicked()
{
    emit rate(MediaType::NoStar,imageId);
    emit backButtonClicked();
    dismiss();
}

void ItemDetailPage::rateOneButtonClicked()
{
    emit rate(MediaType::OneStar,imageId);
    emit backButtonClicked();
    dismiss();
}

void ItemDetailPage::rateTwoButtonClicked()
{
    emit rate(MediaType::TwoStars,imageId);
    emit backButtonClicked();
    dismiss();
}

void ItemDetailPage::rateThreeButtonClicked()
{
    emit rate(MediaType::ThreeStars,imageId);
    emit backButtonClicked();
    dismiss();
}

void ItemDetailPage::rateFourButtonClicked()
{
    emit rate(MediaType::FourStars,imageId);
    emit backButtonClicked();
    dismiss();
}

void ItemDetailPage::rateFiveButtonClicked()
{
    emit rate(MediaType::FiveStars,imageId);
    emit backButtonClicked();
    dismiss();
}
