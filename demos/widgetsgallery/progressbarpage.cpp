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

#include <MButton>
#include <MLabel>
#include <MTheme>
#include <MLocale>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MProgressIndicator>
#include <QPropertyAnimation>
#include <MSceneManager>

ProgressBarPage::ProgressBarPage() :
    TemplatePage(TemplatePage::SimpleWidgets),
    button1(0),
    bar1ValueAnimation(0),
    bar1(0),
    bar2(0),
    label1(0),
    label2(0),
    position(0)
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

    bar1 = new MProgressIndicator(0, MProgressIndicator::barType);
    // Since range's type is int, we want a fairly large number here, to get a smooth animation
    bar1->setRange(0, 1000);
    bar1->setValue(0);
    containerPolicy->addItem(bar1);

    bar1ValueAnimation = new QPropertyAnimation(bar1, "value", this);
    bar1ValueAnimation->setEasingCurve(QEasingCurve::OutInQuad);

    button1 = new MButton();
    button1->setIconID("icon-m-common-play");
    containerPolicy->addItem(button1, Qt::AlignHCenter);
    containerPolicy->setVerticalSpacing(10);

    label2 = new MLabel();
    containerPolicy->addItem(label2);

    bar2 = new MProgressIndicator(0, MProgressIndicator::barType);
    bar2->setRange(0, 9);
    bar2->setUnknownDuration(true);
    containerPolicy->addItem(bar2);

    connect(button1, SIGNAL(clicked()), this, SLOT(animateProgressBar1()));

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

void ProgressBarPage::animateProgressBar1()
{
    if (bar1ValueAnimation->state() != QAbstractAnimation::Running) {
        // start and end value should match bar1's range
        bar1ValueAnimation->setStartValue(bar1->minimum());
        bar1ValueAnimation->setEndValue(bar1->maximum());
        // 10 seconds to fill the bar
        bar1ValueAnimation->setDuration(10000);
        bar1ValueAnimation->start();
    }
}
