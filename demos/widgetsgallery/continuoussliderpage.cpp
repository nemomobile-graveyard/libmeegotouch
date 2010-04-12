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

#include "continuoussliderpage.h"
#include <QTimer>
#include <QDebug>
#include <MButton>
#include <MLocale>
#include <MLabel>
#include <MTheme>
#include <MImage>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MSlider>
#include <MSliderView>
#include <MSeekBar>
#include <MApplication>
#include <MApplicationWindow>

ContinuousSliderPage::ContinuousSliderPage()
    : TemplatePage(),
      label1(0),
      label2(0),
      label3(0),
      slider3(0),
      label4(0),
      seekbar4(0)
{
    gid = TemplatePage::UserInput;
}

ContinuousSliderPage::~ContinuousSliderPage()
{
}

void ContinuousSliderPage::createContent()
{
    MApplicationPage::createContent();

    MLayout *layout = new MLayout();
    centralWidget()->setLayout(layout);

    l_policy = new MGridLayoutPolicy(layout);
    l_policy->setContentsMargins(32, 32, 32, 32);
    l_policy->setRowSpacing(3, 80);

    p_policy = new MGridLayoutPolicy(layout);
    p_policy->setContentsMargins(32, 32, 32, 32);
    p_policy->setRowSpacing(5, 80);

    label1 = new MLabel(this);
    label1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    MSlider *slider1 = new MSlider(this);
    slider1->setObjectName("wgSlider1");
    slider1->setThumbLabelIconID("Icon-music");
    slider1->setMinLabelIconID("Icon-home");
    slider1->setMaxLabelIconID("Icon-browser");
    slider1->setMinLabelVisible(true);
    slider1->setMaxLabelVisible(true);
    slider1->setThumbLabelVisible(true);
    slider1->setOrientation(Qt::Horizontal);
    slider1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    label2  = new MLabel(this);
    label2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    MSlider *slider2 = new MSlider(this);
    slider2->setObjectName("wgSlider2");
    slider2->setOrientation(Qt::Horizontal);
    slider2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    label3  = new MLabel(this);
    label3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    slider3 = new MSlider(this);
    slider3->setObjectName("wgSlider3");
    slider3->setOrientation(Qt::Horizontal);
    slider3->setMinLabelVisible(true);
    slider3->setMaxLabelVisible(true);
    slider3->setMinLabel("min");
    slider3->setMaxLabel("max");
    slider3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    label4  = new MLabel(this);
    label4->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    seekbar4 = new MSeekBar(this);
    seekbar4->setObjectName("wgSeekBar4");
    seekbar4->setOrientation(Qt::Horizontal);
    //% "Min"
    seekbar4->setMinLabel(qtTrId("xx_continuous_slider_min"));
    //% "Max"
    seekbar4->setMaxLabel(qtTrId("xx_continuous_slider_max"));
    //% "Thumb"
    seekbar4->setThumbLabel(qtTrId("xx_continuous_slider_thumb"));
    seekbar4->setMinLabelVisible(true);
    seekbar4->setMaxLabelVisible(true);
    seekbar4->setThumbLabelVisible(true);
    seekbar4->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    seekbar4->setContentType(MSeekBarModel::ContentOnline);
    seekbar4->setLoadedContentMinimum(20);
    seekbar4->setLoadedContentMaximum(50);

    l_policy->addItem(label1,  0, 0);
    l_policy->addItem(slider1, 1, 0);
    l_policy->addItem(label2,  0, 1);
    l_policy->addItem(slider2, 1, 1);
    l_policy->addItem(label3,  2, 0);
    l_policy->addItem(slider3, 3, 0);
    l_policy->addItem(label4,  2, 1);
    l_policy->addItem(seekbar4, 3, 1);

    p_policy->addItem(label1,  0, 0, 1, 2);
    p_policy->addItem(slider1, 1, 0, 1, 2);
    p_policy->addItem(label3,  2, 0, 1, 2);
    p_policy->addItem(slider3, 3, 0, 1, 2);
    p_policy->addItem(label2,  4, 0, 1, 2);
    p_policy->addItem(slider2, 5, 0, 1, 2);
    p_policy->addItem(label4,  6, 0, 1, 2);
    p_policy->addItem(seekbar4, 7, 0, 1, 2);

    connect(MApplication::activeApplicationWindow(),
            SIGNAL(orientationChanged(M::Orientation)),
            this,
            SLOT(pageRotated(M::Orientation)));

    retranslateUi();
}

void ContinuousSliderPage::retranslateUi()
{
    //% "Continuous Slider"
    setTitle(qtTrId("xx_continuous_slider_title"));
    if (!isContentCreated())
        return;
    //% "with icons"
    label1->setText(qtTrId("xx_continuous_slider_with_icons"));
    //% "plain slider"
    label2->setText(qtTrId("xx_continuous_slider_plain_slider"));
    //% "with labels"
    label3->setText(qtTrId("xx_continuous_slider_with_labels"));
    //% "Min"
    slider3->setMinLabel(qtTrId("xx_continuous_slider_min"));
    //% "Max"
    slider3->setMaxLabel(qtTrId("xx_continuous_slider_max"));
    //% "Thumb"
    slider3->setThumbLabel(qtTrId("xx_continuous_slider_thumb"));
    //% "seekbar"
    label4->setText(qtTrId("xx_continuous_slider_seekbar"));
    //% "Min"
    seekbar4->setMinLabel(qtTrId("xx_continuous_slider_min"));
    //% "Max"
    seekbar4->setMaxLabel(qtTrId("xx_continuous_slider_max"));
    //% "Thumb"
    seekbar4->setThumbLabel(qtTrId("xx_continuous_slider_thumb"));
    update();
}

void ContinuousSliderPage::pageRotated(const M::Orientation &orientation)
{
    QSize s = MApplication::activeApplicationWindow()->visibleSceneSize();

    centralWidget()->setMinimumSize(s);
    centralWidget()->setMaximumSize(s);
    centralWidget()->setPreferredSize(s);

    if (orientation == M::Portrait) {
        p_policy->activate();
    } else {
        l_policy->activate();
    }
}

void ContinuousSliderPage::rotatel()
{
    rotate(-1);
}

void ContinuousSliderPage::rotater()
{
    rotate(1);
}

void ContinuousSliderPage::rotate(int d)
{
    static const M::OrientationAngle angles[] = {
        M::Angle0,
        M::Angle90,
        M::Angle180,
        M::Angle270
    };

    static int a = 0;

    a = (a + d) & 3;

    MApplication::activeApplicationWindow()->setOrientationAngle(angles[a]);
}
