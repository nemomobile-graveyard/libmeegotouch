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

#include "feedbackpage.h"

#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiApplicationPage>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiButton>
#include <DuiButtonIconView>
#include <DuiButtonGroup>
#include <DuiDebug>
#include <DuiApplication>

FeedbackPage::FeedbackPage() :
    TemplatePage(),
    feedbackButton1(0),
    feedbackButton2(0),
    feedbackButton3(0),
    feedbackButton4(0),
    feedbackButton5(0),
    feedbackButton6(0)
{
    gid = TemplatePage::InputFeedback;
}

FeedbackPage::~FeedbackPage()
{
}

QString FeedbackPage::timedemoTitle()
{
    return "InputFeedback";
}

void FeedbackPage::createContent()
{
    TemplatePage::createContent();

    landscapePolicy->setContentsMargins(30, 0, 0, 0);

    feedbackButton1 = new DuiButton();
    feedbackButton1->setObjectName("feedbackButton1");
    containerPolicy->addItem(feedbackButton1);

    feedbackButton2 = new DuiButton();
    feedbackButton2->setObjectName("feedbackButton2");
    containerPolicy->addItem(feedbackButton2);

    feedbackButton3 = new DuiButton();
    feedbackButton3->setObjectName("feedbackButton3");
    containerPolicy->addItem(feedbackButton3);

    feedbackButton4 = new DuiButton();
    feedbackButton4->setObjectName("feedbackButton4");
    containerPolicy->addItem(feedbackButton4);

    feedbackButton5 = new DuiButton();
    feedbackButton5->setObjectName("feedbackButton5");
    containerPolicy->addItem(feedbackButton5);

    feedbackButton6 = new DuiButton();
    feedbackButton6->setObjectName("feedbackButton6");
    containerPolicy->addItem(feedbackButton6);

    retranslateUi();
}

void FeedbackPage::retranslateUi()
{
    //% "Input Feedback"
    setTitle(qtTrId("xx_input_feedback_page_title"));
    if (!isContentCreated())
        return;
    //% "This page contains buttons with custom "
    //% "input feedbacks. Any DuiWidget can have input "
    //% "feedback for press and release events. Input "
    //% "feedback can be a sound effect and/or a haptic "
    //% "effect (piezo, vibra) depending on the product.\n\n"
    //% "Input feedbacks can be easily configured for "
    //% "different purposes using the theming system."
    infoLabel->setText("<a></a>" + qtTrId("xx_input_feedback_page_info_label"));
    //% "Default"
    feedbackButton1->setText(qtTrId("xx_input_feedback_page_button1"));
    //% "Different"
    feedbackButton2->setText(qtTrId("xx_input_feedback_page_button2"));
    //% "Sluggish"
    feedbackButton3->setText(qtTrId("xx_input_feedback_page_button3"));
    //% "Babytoy"
    feedbackButton4->setText(qtTrId("xx_input_feedback_page_button4"));
    //% "Space"
    feedbackButton5->setText(qtTrId("xx_input_feedback_page_button5"));
    //% "Silent"
    feedbackButton6->setText(qtTrId("xx_input_feedback_page_button6"));
}


