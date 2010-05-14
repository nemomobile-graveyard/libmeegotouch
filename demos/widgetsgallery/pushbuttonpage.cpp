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

#include "pushbuttonpage.h"
#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MApplicationPage>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MButtonGroup>
#include <MDebug>
#include <MApplication>

PushButtonPage::PushButtonPage() :
    TemplatePage(),
    pushButton1(0),
    pushButton2(0),
    pushButton3(0)
{
    gid = TemplatePage::Buttons;
}

PushButtonPage::~PushButtonPage()
{
}

QString PushButtonPage::timedemoTitle()
{
    return "PushButton";
}

void PushButtonPage::createContent()
{
    TemplatePage::createContent();

    // Push Buttons
    pushButton1 = new MButton();
    containerPolicy->addItem(pushButton1);

    pushButton2 = new MButton();
    pushButton2->setIconID("icon-m-messaging-smiley-surprised");
    containerPolicy->addItem(pushButton2);

    pushButton3 = new MButton();
    pushButton3->setIcon(QIcon::fromTheme("phone"));
    containerPolicy->addItem(pushButton3);

    retranslateUi();
}

void PushButtonPage::retranslateUi()
{
    //% "Push Button"
    setTitle(qtTrId("xx_push_button_page_title"));
    if (!isContentCreated())
        return;
    //% "Push Buttons can have an icon and a label. "
    //% "Push Button can be used in any container but usually "
    //% "it is used with Dialogs, Notifications and Main windows "
    //% "to invoke actions.\n\n"
    //% "A Push Button generally looks like a traditional button, "
    //% "with button-style edges."
    infoLabel->setText("<a></a>" + qtTrId("xx_push_button_page_info_label"));
    //% "Lorem"
    pushButton1->setText(qtTrId("xx_push_button_page_button1"));
    //% "Ipsum"
    pushButton2->setText(qtTrId("xx_push_button_page_button2"));
    //% "Dolor\nsit amet"
    pushButton3->setText(qtTrId("xx_push_button_page_button3"));
}


