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
#include <QFileInfo>
#include <QTimer>

#include <MImageWidget>
#include <MLabel>
#include <MVideoWidget>
#include <MButton>
#include <MSlider>
#include <MLinearLayoutPolicy>
#include <MLayout>

ItemDetailPage::ItemDetailPage()
    : video(0),
      slider(0),
      image(0)
{
    setObjectName("itemDetailPage");
}

ItemDetailPage::~ItemDetailPage()
{
    delete video;
    delete slider;
    delete image;
}

QString ItemDetailPage::timedemoTitle()
{
    return "ItemDetailPage";
}


void ItemDetailPage::createContent()
{
    QGraphicsWidget *panel = centralWidget();
    layout = new MLayout(panel);

    if( !videoId.isEmpty() ) {
        policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
        policy->setSpacing(0.0);
        layout->setLandscapePolicy(policy);
        layout->setPortraitPolicy(policy);

        video = new GridVideoWidget(panel);
        connect(video, SIGNAL(videoReady()), this, SLOT(videoReady()));
        video->open(videoId);

        policy->addItem(video);

        QGraphicsLinearLayout* controlLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        controlLayout->setContentsMargins(0,0,0,0);

        button = new MButton(panel);
        button->setObjectName("video-player-button");
        button->setIconID("icon-m-common-pause");
        button->setIconVisible(true);
        button->setTextVisible(false);

        connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
        controlLayout->addItem(button);
        controlLayout->setAlignment(button, Qt::AlignCenter);

        slider = new MSlider(panel);
        slider->setObjectName("video-player-slider");

        connect(slider, SIGNAL(valueChanged(int)), this, SLOT(videoSliderValueChanged(int)));
        connect(slider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()));
        connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));

        controlLayout->addItem(slider);
        policy->addItem(controlLayout);

        setTitle(QFileInfo(videoId).fileName());

        // go fullscreen
        setComponentsDisplayMode(MApplicationPage::AllComponents,
                                       MApplicationPageModel::AutoHide);
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

void ItemDetailPage::videoReady()
{
    video->play();
    video->setFullscreen(true);
    video->setMuted(false);
    video->setVolume(0.8);

    slider->setMinimum(0);
    slider->setMaximum(video->length());
    QTimer::singleShot(100, this, SLOT(updatePosition()));

    setAutoMarginsForComponentsEnabled(false);
    setComponentsDisplayMode(MApplicationPage::NavigationBar/*MApplicationPage::AllComponents*/, MApplicationPageModel::Hide);
}

void ItemDetailPage::sliderPressed()
{
    //if( video )
    //    video->pause();
}

void ItemDetailPage::sliderReleased()
{
    //if( video )
    //    video->play();
}

void ItemDetailPage::videoSliderValueChanged(int newValue)
{
    if( video ) {
        video->seek(newValue);
    }
}

void ItemDetailPage::buttonClicked()
{
    if( video->state() == MVideo::Playing ) {
        video->pause();
        button->setIconID("icon-m-common-play");
        //button->setText("PLAY");
    } else {
        video->play();
        button->setIconID("icon-m-common-pause");
        //button->setText("PAUSE");
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
