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

#include "iconbuttonpage.h"
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
#include <QGraphicsLinearLayout>

IconButtonPage::IconButtonPage() :
        TemplatePage(TemplatePage::SimpleWidgets),
    iconButton1(0),
    iconButton2(0)
{
}

IconButtonPage::~IconButtonPage()
{
}

QString IconButtonPage::timedemoTitle()
{
    return "IconButton";
}

void IconButtonPage::createContent()
{
    TemplatePage::createContent();

    QGraphicsWidget *w = new QGraphicsWidget();
    w->setMinimumSize(container->minimumWidth(), 5);
    w->setMaximumSize(container->maximumWidth(), 5);

    containerPolicy->setSpacing(40);
    containerPolicy->addItem(w);
    //containerPolicy->addStretch();

    // Icon buttons
    iconButton1 = new MButton();
    iconButton1->setViewType(MButton::iconType);
    iconButton1->setIconID("icon-m-toolbar-new-chat");

    containerPolicy->addItem(iconButton1, Qt::AlignCenter);

    iconButton2 = new MButton();
    iconButton2->setViewType(MButton::iconType);
    iconButton2->setObjectName("iconButton");
    iconButton2->setIconID("icon-m-toolbar-send-email");
    iconButton2->setMaximumWidth(175);

    containerPolicy->addItem(iconButton2, Qt::AlignCenter);
    containerPolicy->addStretch();
    retranslateUi();
}

void IconButtonPage::retranslateUi()
{
    //% "Icon Button"
    setTitle(qtTrId("xx_icon_button_page_title"));
    if (!isContentCreated())
        return;

    //% "Icon Buttons have an icon and can have a text label. "
    //% "Icon button differs from push button by its visual presentation: "
    //% "it does not have button-style edges like a Push Button does.\n\n "
    //% "Icon Buttons are used in e.g. Toolbars."
    infoLabel->setText("<a></a>" + qtTrId("xx_icon_button_page_info_label").replace('\n', "<br>"));
    //% "Lorem ipsum"
    iconButton2->setText(qtTrId("xx_icon_button_page_icon_button2"));
}


