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

#include "progressbarpage.h"
#include <QPropertyAnimation>
#include <MButton>
#include <MLabel>
#include <MTheme>
#include <MLocale>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MProgressIndicator>
#include <MSceneManager>

ProgressBarPage::ProgressBarPage() :
    TemplatePage(TemplatePage::SimpleWidgets),
    bar1(0),
    bar2(0),
    label1(0),
    label2(0)
{
}

ProgressBarPage::~ProgressBarPage()
{
}

QString ProgressBarPage::timedemoTitle()
{
    return "ProgressBar";
}

void ProgressBarPage::createContent()
{
    TemplatePage::createContent();

    label1 = new MLabel();
    containerPolicy->addItem(label1);

    bar1 = new MProgressIndicator(centralWidget(), MProgressIndicator::barType);
    // Since range's type is int, we want a fairly large number here, to get a smooth animation
    bar1->setRange(0, 99999);
    bar1->setValue(0);
    containerPolicy->addItem(bar1);

    label2 = new MLabel();
    containerPolicy->addItem(label2);

    bar2 = new MProgressIndicator(centralWidget(), MProgressIndicator::barType);
    bar2->setRange(0, 9);
    bar2->setUnknownDuration(true);
    containerPolicy->addItem(bar2);

    QPropertyAnimation* animation = new QPropertyAnimation(bar1, "value", this);
    // loop forever
    animation->setLoopCount(-1);
    // start and end value should match bar1's range
    animation->setStartValue(0);
    animation->setEndValue(99999);
    // 10 ms to fill the bar
    animation->setDuration(10000);
    animation->start();

    retranslateUi();
}

void ProgressBarPage::retranslateUi()
{
    //% "Progress Bar"
    setTitle(qtTrId("xx_progressbar_page_title"));
    if (!isContentCreated())
        return;
    //% "A Progress Bar can indicate an ongoing process "
    //% "with either known or unknown durations."
    infoLabel->setText("<a></a>" + qtTrId("xx_progressbar_page_info_label"));
    //% "Downloading nicepic.jpg"
    label1->setText(qtTrId("xx_progressindicator_known_duration_bar"));
    //% "Installing CoolApp"
    label2->setText(qtTrId("xx_progressindicator_unknown_duration_bar"));
}



