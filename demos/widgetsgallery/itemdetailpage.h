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

#include "timedemopage.h"
#include "gridmodel.h"

class MLayout;
class MSlider;
class MButton;
class MImageWidget;
class MVideoWidget;
class MLinearLayoutPolicy;

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
    void videoReady();

    void sliderPressed();
    void sliderReleased();
    void videoSliderValueChanged(int newValue);

    void buttonClicked();

    void rateNoneButtonClicked();
    void rateOneButtonClicked();
    void rateTwoButtonClicked();
    void rateThreeButtonClicked();
    void rateFourButtonClicked();
    void rateFiveButtonClicked();

    void updatePosition();

signals:
    void rate(MediaType::Rating rating, const QString& id);

protected:
    virtual void retranslateUi();

private:
    MLayout* layout;
    MLinearLayoutPolicy* policy;

    MVideoWidget* video;
    MSlider* slider;
    MButton* button;
    MImageWidget* image;

    QString imageId;
    QString videoId;
};
