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
    pushButton7(NULL),
    pushButton8(NULL),
    pushButton9(NULL),
    hLabel1(NULL),
    hLabel2(NULL),
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

  // Init horizontal button group, default style
    MLayout* hLayout1 = new MLayout;
    MLinearLayoutPolicy* hPolicy1 = new MLinearLayoutPolicy(hLayout1, Qt::Horizontal);
    hPolicy1->setContentsMargins(0, 0, 0, 0);
    hPolicy1->setSpacing(0);
    // The policy notifies the widgets of their relative position inside the layout,
    // this causes the buttons to be rendered with different backgrounds for each position
    hPolicy1->setNotifyWidgetsOfLayoutPositionEnabled(true);

    //make buttons evenly sized
    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    
    pushButton1 = new MButton();
    // The group type makes the button use the MButton[group] CSS block from the theme,
    // this allows for gapless groups by modifying the margins.
    pushButton1->setViewType(MButton::groupType);
    pushButton1->setSizePolicy(sizePolicy);
    pushButton2 = new MButton();
    pushButton2->setViewType(MButton::groupType);
    pushButton2->setSizePolicy(sizePolicy);
    pushButton3 = new MButton();
    pushButton3->setViewType(MButton::groupType);
    pushButton3->setSizePolicy(sizePolicy);
    hPolicy1->addItem(pushButton1);
    hPolicy1->addItem(pushButton2);
    hPolicy1->addItem(pushButton3);

    // Init horizontal button group, exclusive
    MLayout* hLayout2 = new MLayout;
    MLinearLayoutPolicy* hPolicy2 = new MLinearLayoutPolicy(hLayout2, Qt::Horizontal);
    hPolicy2->setContentsMargins(0, 0, 0, 0);
    hPolicy2->setSpacing(0);
    hPolicy2->setNotifyWidgetsOfLayoutPositionEnabled(true);
    pushButton4 = new MButton();
    pushButton4->setCheckable(true);
    pushButton4->setViewType(MButton::groupType);
    pushButton4->setSizePolicy(sizePolicy);
    pushButton5 = new MButton();
    pushButton5->setCheckable(true);
    pushButton5->setChecked(true); // Let's make this our default
    pushButton5->setViewType(MButton::groupType);
    pushButton5->setSizePolicy(sizePolicy);
    pushButton6 = new MButton();
    pushButton6->setCheckable(true);
    pushButton6->setViewType(MButton::groupType);
    pushButton6->setSizePolicy(sizePolicy);
    hPolicy2->addItem(pushButton4);
    hPolicy2->addItem(pushButton5);
    hPolicy2->addItem(pushButton6);
    // The group enforces the logical exclusivity
    MButtonGroup* group = new MButtonGroup();
    group->addButton(pushButton4);
    group->addButton(pushButton5);
    group->addButton(pushButton6);

    // Init vertical button group, checkable
    MLayout* vLayout = new MLayout;
    MLinearLayoutPolicy* vPolicy = new MLinearLayoutPolicy(vLayout, Qt::Vertical);
    vPolicy->setNotifyWidgetsOfLayoutPositionEnabled(true);
    vPolicy->setSpacing(0);
    pushButton7 = new MButton();
    pushButton7->setCheckable(true);
    pushButton7->setViewType(MButton::groupType);
    pushButton8 = new MButton();
    pushButton8->setCheckable(true);
    pushButton8->setViewType(MButton::groupType);
    pushButton9 = new MButton();
    pushButton9->setCheckable(true);
    pushButton9->setViewType(MButton::groupType);
    vPolicy->addItem(pushButton7);
    vPolicy->addItem(pushButton8);
    vPolicy->addItem(pushButton9);

    //add label and horizontal button group into page
    hLabel1 = new MLabel();
    containerPolicy->addItem(hLabel1);
    containerPolicy->addItem(hLayout1);

    //add small spacer between button groups
    QGraphicsWidget* spacer1 = new QGraphicsWidget;
    spacer1->setMinimumSize(0, 20);
    spacer1->setPreferredSize(0, 20);
    spacer1->setMaximumSize(0, 20);
    containerPolicy->addItem(spacer1);

    //add label and horizontal button group into page
    hLabel2 = new MLabel();
    containerPolicy->addItem(hLabel2);
    containerPolicy->addItem(hLayout2);

    //add small spacer between button groups
    QGraphicsWidget* spacer2 = new QGraphicsWidget;
    spacer2->setMinimumSize(0, 20);
    spacer2->setPreferredSize(0, 20);
    spacer2->setMaximumSize(0, 20);
    containerPolicy->addItem(spacer2);

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
        
    /*% "Buttons can be grouped together based on visual or logic requirements.\n\n"
        "The basic group is formed by placing buttons together "
        "into a layout. Depending on the theme, the buttons are typically "
        "drawn as connected with each other.\n\n"
        "Logical exclusivity can be added to a group, which ensures only one "
        "of the buttons can be selected at a time. If a group is not exclusive, "
        "multiple options from the group can be selected."
     */
    infoLabel->setText("<a></a>" + qtTrId("xx_button_group_page_info_label").replace('\n', "<br>"));

    //% "Horizontal group"
    hLabel1->setText(qtTrId("xx_button_group_page_hlabel1"));
    //% "Yes"
    pushButton1->setText(qtTrId("xx_button_group_page_button1"));
    //% "No"
    pushButton2->setText(qtTrId("xx_button_group_page_button2"));
    //% "Maybe"
    pushButton3->setText(qtTrId("xx_button_group_page_button3"));

    //% "Horizontal group, exclusive"
    hLabel2->setText(qtTrId("xx_button_group_page_vlabel2"));
    //% "Cost"
    pushButton4->setText(qtTrId("xx_button_group_page_button4"));
    //% "Quality"
    pushButton5->setText(qtTrId("xx_button_group_page_button5"));
    //% "Schedule"
    pushButton6->setText(qtTrId("xx_button_group_page_button6"));

    //% "Vertical group, checkable"
    vLabel->setText(qtTrId("xx_button_group_page_vlabel"));
    //% "Milk"
    pushButton7->setText(qtTrId("xx_button_group_page_button7"));
    //% "Sugar"
    pushButton8->setText(qtTrId("xx_button_group_page_button8"));
    //% "Biscotti"
    pushButton9->setText(qtTrId("xx_button_group_page_button9"));
}
