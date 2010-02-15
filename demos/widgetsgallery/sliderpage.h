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

#ifndef SLIDERPAGE_H
#define SLIDERPAGE_H

#include "templatepage.h"

class DuiLabel;
class DuiButton;
class DuiSlider;
class DuiSeekBar;
class DuiContainer;

class SliderPage : public TemplatePage
{
    Q_OBJECT
public:
    SliderPage();
    virtual ~SliderPage();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

private:
    DuiLabel *ageLabel;
    DuiSlider *ageSlider;
    DuiContainer *ageContainer;

    DuiSeekBar *playerSeekBar;
    DuiButton *playerButton;
    DuiContainer *playerContainer;

    int playTime;
    bool playerSeekBarIsPressed;

    int loadedContentMinimum;
    int loadedContentMaximum;

    DuiSlider *brightnessSlider;
    DuiContainer *brightnessContainer;

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
    void showBrightnessSliderHandle();
    void hideBrightnessSliderHandle();
};

#endif // SLIDERPAGE_H
