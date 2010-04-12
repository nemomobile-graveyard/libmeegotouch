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

#ifndef SLIDERPAGE_H
#define SLIDERPAGE_H

#include "templatepage.h"

class MLabel;
class MButton;
class MSlider;
class MSeekBar;
class MContainer;

class SliderPage : public TemplatePage
{
    Q_OBJECT
public:
    SliderPage();
    virtual ~SliderPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

private:
    MLabel *ageLabel;
    MSlider *ageSlider;
    MContainer *ageContainer;

    MSeekBar *playerSeekBar;
    MButton *playerButton;
    MContainer *playerContainer;

    int playTime;
    bool playerSeekBarIsPressed;

    int loadedContentMinimum;
    int loadedContentMaximum;

    MSlider *brightnessSlider;
    MContainer *brightnessContainer;

private slots:
    void modifyAgeSliderHandle(int newValue);

    void playerButtonClicked();
    void playTimesliceElapsed();
    void loadContentTimesliceElapsed();
    void modifyPlayerSeekBarHandle(int newValue);
    void playerSeekBarPressed();
    void playerSeekBarReleased();
    void playerOutOfLoadedContentRange();

    void modifyBrightnessSliderHandle(int newValue);
};

#endif // SLIDERPAGE_H
