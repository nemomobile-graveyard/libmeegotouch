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

#include "buttongrouppage.h"
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

ButtonGroupPage::ButtonGroupPage() :
    TemplatePage(TemplatePage::Buttons),
    pushButton1(NULL),
    pushButton2(NULL),
    pushButton3(NULL),
    pushButton4(NULL),
    pushButton5(NULL),
    pushButton6(NULL),
    hLabel(NULL),
    vLabel(NULL)
{
}

ButtonGroupPage::~ButtonGroupPage()
{
}

QString ButtonGroupPage::timedemoTitle()
{
    return "ButtonGroup";
}

void ButtonGroupPage::createContent()
{
    TemplatePage::createContent();

    //init horizontal button group
    MLayout* hLayout = new MLayout;
    MLinearLayoutPolicy* hPolicy = new MLinearLayoutPolicy(hLayout, Qt::Horizontal);
    hPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    hPolicy->setObjectName("buttongroup");
    pushButton1 = new MButton();
    pushButton1->setCheckable(true);
    pushButton1->setViewType(MButton::groupType);
    pushButton2 = new MButton();
    pushButton2->setCheckable(true);
    pushButton2->setViewType(MButton::groupType);
    pushButton3 = new MButton();
    pushButton3->setCheckable(true);
    pushButton3->setViewType(MButton::groupType);
    hPolicy->addItem(pushButton1);
    hPolicy->addItem(pushButton2);
    hPolicy->addItem(pushButton3);

    //init vertical button group
    MLayout* vLayout = new MLayout;
    MLinearLayoutPolicy* vPolicy = new MLinearLayoutPolicy(vLayout, Qt::Vertical);
    vPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    vPolicy->setObjectName("buttongroup");
    pushButton4 = new MButton();
    pushButton4->setCheckable(true);
    pushButton4->setViewType(MButton::groupType);
    pushButton5 = new MButton();
    pushButton5->setCheckable(true);
    pushButton5->setViewType(MButton::groupType);
    pushButton6 = new MButton();
    pushButton6->setCheckable(true);
    pushButton6->setViewType(MButton::groupType);
    vPolicy->addItem(pushButton4);
    vPolicy->addItem(pushButton5);
    vPolicy->addItem(pushButton6);

    //add label and horizontal button group into page
    hLabel = new MLabel();
    containerPolicy->addItem(hLabel);
    containerPolicy->addItem(hLayout);

    //add small spacer between horizontal and vertical button groups
    QGraphicsWidget* spacer = new QGraphicsWidget;
    spacer->setMinimumSize(0, 20);
    spacer->setPreferredSize(0, 20);
    spacer->setMaximumSize(0, 20);
    containerPolicy->addItem(spacer);

    //add label and vertical button group into page
    vLabel = new MLabel();
    containerPolicy->addItem(vLabel);
    containerPolicy->addItem(vLayout);

    retranslateUi();
}

void ButtonGroupPage::retranslateUi()
{
    //% "Button Group"
    setTitle(qtTrId("xx_button_group_page_title"));
    if (!isContentCreated())
        return;
        
    //% "Button group information label."
    infoLabel->setText("<a></a>" + qtTrId("xx_button_group_page_info_label"));

    //% "Horizontal button group:"
    hLabel->setText(qtTrId("xx_button_group_page_hlabel"));
    //% "Lorem"
    pushButton1->setText(qtTrId("xx_button_group_page_button1"));
    //% "Ipsum"
    pushButton2->setText(qtTrId("xx_button_group_page_button2"));
    //% "Dolor"
    pushButton3->setText(qtTrId("xx_button_group_page_button3"));

    //% "Vertical button group:"
    vLabel->setText(qtTrId("xx_button_group_page_vlabel"));
    //% "Consectetur"
    pushButton4->setText(qtTrId("xx_button_group_page_button4"));
    //% "Adipisicing"
    pushButton5->setText(qtTrId("xx_button_group_page_button5"));
    //% "Elit"
    pushButton6->setText(qtTrId("xx_button_group_page_button6"));
}


