/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sliderpage.h"

#include <MApplication>
#include <MContainer>
#include <MLabel>
#include <MButton>
#include <MSlider>
#include <MSeekBar>
#include <MLayout>
#include <MLocale>
#include <MApplicationPage>
#include <MLinearLayoutPolicy>
#include <MStylableWidget>
#include <QGraphicsGridLayout>
#include <MImageWidget>
#include <QTimer>

SliderPage::SliderPage() :
    TemplatePage(TemplatePage::UserInput),
    ageLabel(0),
    ageSlider(0),
    ageContainer(0),
    playerSeekBar(0),
    playerButton(0),
    playerContainer(0),
    playerMinLabel(0),
    playerMaxLabel(0),
    playTime(1230),
    playerSeekBarIsPressed(false),
    loadedContentMinimum(0),
    loadedContentMaximum(0),
    brightnessSlider(0),
    brightnessContainer(0)
{
}

SliderPage::~SliderPage()
{
}

QString SliderPage::timedemoTitle()
{
    return "Slider";
}

void SliderPage::createContent()
{
    TemplatePage::createContent();

    MLayout *ageLayout = new MLayout;

    MLinearLayoutPolicy *ageLayoutPolicy = new MLinearLayoutPolicy(ageLayout, Qt::Vertical);
    ageLayoutPolicy->setSpacing(0);
    ageLayoutPolicy->setContentsMargins(0, 0, 0, 0);

    ageLabel = new MLabel;
    ageLabel->setObjectName("ageLabel");
    ageLabel->setStyleName(inv("CommonFieldLabel"));
    ageLabel->setTextElide(true);

    ageLayoutPolicy->addItem(ageLabel);
    ageLayoutPolicy->setStretchFactor(ageLabel, 0);

    ageSlider = new MSlider;
    ageSlider->setObjectName("ageSlider");
    ageSlider->setStyleName(inv("CommonSlider"));
    ageLayoutPolicy->addItem(ageSlider);
    ageLayoutPolicy->setStretchFactor(ageSlider, 1);

    ageContainer = new MContainer;
    ageContainer->setObjectName("ageContainer");
    ageContainer->setStyleName(inv("CommonContainer"));
    ageContainer->centralWidget()->setLayout(ageLayout);
    containerPolicy->addItem(ageContainer);

    QObject::connect(ageSlider, SIGNAL(valueChanged(int)), this, SLOT(modifyAgeSliderHandle(int)));

    QGraphicsGridLayout *playerLayout = new QGraphicsGridLayout;
    playerLayout->setSpacing(0);
    playerLayout->setContentsMargins(0, 0, 0, 0);

    playerButton = new MButton;
    playerButton->setObjectName("playerButton");
    playerButton->setStyleName("CommonMainIcon");
    playerButton->setViewType(MButton::iconType);
    if (MApplication::instance()->objectName() == "widgetsgallery") {
        playerButton->setIconID("icon-m-toolbar-mediacontrol-play");
    } else {
        playerButton->setIconID("icon-m-toolbar-mediacontrol-play-white");
    }
    playerButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    playerLayout->addItem(playerButton, 0, 0);

    MStylableWidget *playerWidget = new MStylableWidget;
    playerLayout->addItem(playerWidget, 0, 1);

    playerSeekBar = new MSeekBar(playerWidget);
    playerSeekBar->setObjectName("playerSeekBar");
    playerSeekBar->setStyleName(inv("CommonSeeker"));
    playerSeekBar->setMaximumSize(380, -1);
    playerSeekBar->setMinimumSize(380, -1);
    playerSeekBar->setPreferredSize(380, -1);

    playerMinLabel = new MLabel(playerWidget);
    playerMinLabel->setAlignment(Qt::AlignLeft);
    playerMinLabel->setStyleName(inv("CommonSeekerLabel"));
    playerMinLabel->setMinimumSize(150, -1);
    playerMinLabel->setMaximumSize(150, -1);
    playerMinLabel->setPreferredSize(150, -1);

    playerMaxLabel = new MLabel(playerWidget);
    playerMaxLabel->setAlignment(Qt::AlignRight);
    playerMaxLabel->setStyleName(inv("CommonSeekerLabel"));
    playerMaxLabel->setMinimumSize(150, -1);
    playerMaxLabel->setMaximumSize(150, -1);
    playerMaxLabel->setPreferredSize(150, -1);

    /* See NB#212946 for why this is so ugly. */
    playerSeekBar->setPos(0, -17);
    playerMinLabel->setPos(0, 38);
    playerMaxLabel->setPos(230, 38);

    playerContainer = new MContainer;
    playerContainer->setObjectName("playerContainer");
    playerContainer->setStyleName(inv("CommonContainer"));
    playerContainer->centralWidget()->setLayout(playerLayout);
    containerPolicy->addItem(playerContainer);

    QObject::connect(playerSeekBar, SIGNAL(valueChanged(int)), this, SLOT(modifyPlayerSeekBarHandle(int)));
    QObject::connect(playerSeekBar, SIGNAL(sliderPressed()), this, SLOT(playerSeekBarPressed()));
    QObject::connect(playerSeekBar, SIGNAL(sliderReleased()), this, SLOT(playerSeekBarReleased()));
    QObject::connect(playerSeekBar, SIGNAL(outOfLoadedContentRange()), this, SLOT(playerOutOfLoadedContentRange()));

    QObject::connect(playerButton, SIGNAL(clicked()), this, SLOT(playerButtonClicked()));

    MLayout *brightnessLayout = new MLayout;
    MLinearLayoutPolicy *brightnessLayoutPolicy = new MLinearLayoutPolicy(brightnessLayout, Qt::Horizontal);
    brightnessLayoutPolicy->setSpacing(0);
    brightnessLayoutPolicy->setContentsMargins(0, 0, 0, 0);

    MImageWidget *t1 = new MImageWidget;
    t1->setStyleName("CommonButtonIcon");
    brightnessLayoutPolicy->addItem(t1, Qt::AlignCenter);

    brightnessSlider = new MSlider;
    brightnessSlider->setObjectName("brightnessSlider");
    brightnessSlider->setStyleName(inv("CommonSlider"));
    brightnessLayoutPolicy->addItem(brightnessSlider, Qt::AlignCenter);

    MImageWidget *t2 = new MImageWidget;
    t2->setStyleName("CommonRightIcon");
    brightnessLayoutPolicy->addItem(t2, Qt::AlignCenter);

    if (MApplication::instance()->objectName() == "widgetsgallery") {
        t1->setImage("icon-s-common-remove");
        t2->setImage("icon-s-common-add");
    } else {
        t1->setImage("icon-s-common-remove-inverse");
        t2->setImage("icon-s-common-add-inverse");
    }

    brightnessContainer = new MContainer;
    brightnessContainer->setObjectName("brightnessContainer");
    brightnessContainer->setStyleName(inv("CommonContainer"));
    brightnessContainer->centralWidget()->setLayout(brightnessLayout);
    containerPolicy->addItem(brightnessContainer);

    QObject::connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(modifyBrightnessSliderHandle(int)));

    retranslateUi();
}

void SliderPage::createLayout()
{
    layout = new MLayout(centralWidget());

    landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    landscapePolicy->setContentsMargins(0, 0, 0, 0);
    landscapePolicy->setSpacing(0);

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->setContentsMargins(0, 0, 0, 0);
    portraitPolicy->setSpacing(0);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    container = new MWidget;
    container->setObjectName("container");

    infoLabel = new MLabel;
    infoLabel->setObjectName("infoLabel");
    infoLabel->setStyleName(inv("CommonBodyText"));
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);

    landscapePolicy->addItem(infoLabel);
    landscapePolicy->addItem(container);

    portraitPolicy->addItem(infoLabel);
    portraitPolicy->addItem(container);
}

void SliderPage::retranslateUi()
{
    //% "Sliders"
    setTitle(qtTrId("xx_slider_page_title"));
    if (!isContentCreated())
        return;

    //% "Slider is used for continuous set of values, "
    //% "among which user can choose one value.\n\n"
    //% "Seekbar is a special type of Slider used for "
    //% "displaying playback status for multimedia content."
    infoLabel->setText("<a></a>" + qtTrId("xx_slider_page_info_label").replace('\n', "<br>"));

    //% "Personal data"
    ageContainer->setTitle(qtTrId("xx_slider_personal_data_label"));

    // The engineering English below has lengthvariants:
    //% "Please select your age:\x9c!! Please select your age:\x9c!! Please select your age:\x9c!! Age:"
    ageLabel->setText(qtTrId("xx_slider_age_label"));

    ageSlider->setRange(0, 100);
    ageSlider->setMinLabelVisible(false);
    ageSlider->setMaxLabelVisible(false);
    ageSlider->setHandleLabelVisible(true);

    ageSlider->setMinLabel(QString::number(ageSlider->minimum()));
    ageSlider->setMaxLabel(QString::number(ageSlider->maximum()));

    modifyAgeSliderHandle(ageSlider->value());

    //% "Player:"
    playerContainer->setTitle(qtTrId("xx_slider_player_label"));

    playerSeekBar->setMinLabelVisible(false);
    playerSeekBar->setMaxLabelVisible(false);
    playerSeekBar->setHandleLabelVisible(true);

    playerSeekBar->setMinimum(0);
    playerSeekBar->setMaximum(playTime);

    int minutes = (playTime / 10) / 60;
    int seconds = (playTime / 10) % 60;

    playerMinLabel->setText("0:00");

    //minutes on one digit seconds on two digits
    playerMaxLabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));

    modifyPlayerSeekBarHandle(playerSeekBar->minimum());

    //% "Brightness:"
    brightnessContainer->setTitle(qtTrId("xx_slider_brightness_label"));

    brightnessSlider->setRange(0, 100);
    brightnessSlider->setSteps(4);
    brightnessSlider->setMinLabelVisible(false);
    brightnessSlider->setMaxLabelVisible(false);
    brightnessSlider->setHandleLabelVisible(true);

    brightnessSlider->setMinLabelIconID("icon-m-common-strength1");
    brightnessSlider->setMaxLabelIconID("icon-m-common-strength5");

    modifyBrightnessSliderHandle(brightnessSlider->value());

    update();
}

void SliderPage::modifyAgeSliderHandle(int newValue)
{
    ageSlider->setHandleLabel( QString::number(newValue) );
}

void SliderPage::playerButtonClicked()
{
    QTimer::singleShot(0, this, SLOT(playTimesliceElapsed()));

    loadedContentMinimum = 0;
    loadedContentMaximum = 0;
    playerSeekBar->setLoadedContentMinimum(loadedContentMinimum);
    playerSeekBar->setLoadedContentMaximum(loadedContentMaximum);

    QTimer::singleShot(0, this, SLOT(loadContentTimesliceElapsed()));
}

void SliderPage::playTimesliceElapsed()
{
    if (playerSeekBar->value() < playerSeekBar->maximum())
        if (!playerSeekBarIsPressed)
            playerSeekBar->setValue(playerSeekBar->value() + 1);

    if (playerSeekBar->value() < playerSeekBar->maximum())
        QTimer::singleShot(100, this, SLOT(playTimesliceElapsed()));
}

void SliderPage::loadContentTimesliceElapsed()
{
    if (loadedContentMaximum < playTime)
        loadedContentMaximum++;

    playerSeekBar->setLoadedContentMaximum(loadedContentMaximum);

    if (loadedContentMaximum < playTime)
        QTimer::singleShot(30, this, SLOT(loadContentTimesliceElapsed()));
}

void SliderPage::modifyPlayerSeekBarHandle(int newValue)
{
    int minutes = (newValue / 10) / 60;
    int seconds = (newValue / 10) % 60;

    playerSeekBar->setHandleLabel(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
}

void SliderPage::playerSeekBarPressed()
{
    playerSeekBarIsPressed = true;
}

void SliderPage::playerSeekBarReleased()
{
    playerSeekBarIsPressed = false;
}

void SliderPage::playerOutOfLoadedContentRange()
{
    loadedContentMinimum = playerSeekBar->value();
    loadedContentMaximum = playerSeekBar->value();

    playerSeekBar->setLoadedContentMinimum(loadedContentMinimum);
    playerSeekBar->setLoadedContentMaximum(loadedContentMaximum);
}

void SliderPage::modifyBrightnessSliderHandle(int newValue)
{
    brightnessSlider->setHandleLabel(QString::number(newValue) + '%');
}
