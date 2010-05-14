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

#include <MImageWidget>
#include <MLabel>
#include <MVideoWidget>
#include <MButton>
#include <MSlider>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MDebug>

const int ANIMATION_TIME = 1000;
const int INACTIVITY_TIMEOUT = 5000;

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

ItemDetailPage::ItemDetailPage()
    : video(0),
      slider(0),
      image(0),
      hideAnimation(0),
      showAnimation(0)
{
    setObjectName("itemDetailPage");
}

ItemDetailPage::~ItemDetailPage()
{
    delete video;
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

    if( !videoId.isEmpty() ) {
        QFileInfo info(videoId);        
        setTitle(info.fileName());

        setObjectName("video-detail-page");
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
        policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
        policy->setSpacing(0.0);
        layout->setLandscapePolicy(policy);
        layout->setPortraitPolicy(policy);

        MButton* button0 = new MButton;
        button0->setText("-");
        connect(button0, SIGNAL(clicked()),this,SLOT(rateNoneButtonClicked()));

        MButton* button1 = new MButton;
        button1->setText("*");
        connect(button1, SIGNAL(clicked()),this,SLOT(rateOneButtonClicked()));

        MButton* button2 = new MButton;
        button2->setText("**");
        connect(button2, SIGNAL(clicked()),this,SLOT(rateTwoButtonClicked()));

        MButton* button3 = new MButton;
        button3->setText("***");
        connect(button3, SIGNAL(clicked()),this,SLOT(rateThreeButtonClicked()));

        MButton* button4 = new MButton;
        button4->setText("****");
        connect(button4, SIGNAL(clicked()),this,SLOT(rateFourButtonClicked()));

        MButton* button5 = new MButton;
        button5->setText("*****");
        connect(button5, SIGNAL(clicked()),this,SLOT(rateFiveButtonClicked()));

        image = new MImageWidget(panel);
        image->setImage(QImage(imageId));
        policy->addItem(image);

        MLayout* bl = new MLayout;
        bl->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
        MLinearLayoutPolicy* blp = new MLinearLayoutPolicy(bl,Qt::Vertical);
        blp->addItem(button0);
        blp->addItem(button1);
        blp->addItem(button2);
        blp->addItem(button3);
        blp->addItem(button4);
        blp->addItem(button5);
        bl->setLandscapePolicy(blp);

        policy->addItem(bl);

        setTitle(QFileInfo(imageId).fileName());

        // go fullscreen
        setComponentsDisplayMode(MApplicationPage::AllComponents,
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

void ItemDetailPage::relayout()
{
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
    if( video ) {
        video->seek(newValue);
    }
}

void ItemDetailPage::buttonClicked()
{
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
}


void ItemDetailPage::updatePosition()
{
    if( video ) {
        slider->setValue(video->position());
        QTimer::singleShot(100, this, SLOT(updatePosition()));
    }
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
