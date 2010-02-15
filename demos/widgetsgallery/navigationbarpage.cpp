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

#include "navigationbarpage.h"

#include <DuiAction>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiButton>
#include <DuiSeparator>
#include <DuiComboBox>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiEscapeButtonPanel>

#include <QGraphicsLinearLayout>
#include <QStringList>
#include <QTimer>

NavigationBarPage::NavigationBarPage()
{
    gid = TemplatePage::ViewsAndDialogs;
}

NavigationBarPage::~NavigationBarPage()
{
}

void NavigationBarPage::createContent()
{
    QGraphicsLinearLayout *lytMain = new QGraphicsLinearLayout(Qt::Vertical);
    lblDisplayMode = new DuiLabel(this);

    QGraphicsLinearLayout *lytButtons = new QGraphicsLinearLayout(Qt::Vertical);

    // combo box nav bar display mode
    comboNavigationBarDisplayMode = new DuiComboBox(this);
    //%  "Navigation Bar"
    comboNavigationBarDisplayMode->setTitle(qtTrId("xx_navigationbar_navbarcombo"));
    comboNavigationBarDisplayMode->setIconVisible(false);
    connect(comboNavigationBarDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeNavigationBarDisplayMode(int)));

    // combo box escape button display mode
    comboEscapeButtonDisplayMode = new DuiComboBox(this);
    //% "Escape Button"
    comboEscapeButtonDisplayMode->setTitle(qtTrId("xx_navigationbar_escapebtncombo"));
    comboEscapeButtonDisplayMode->setIconVisible(false);
    connect(comboEscapeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeEscapeButtonDisplayMode(int)));

    // combo box home button display mode
    comboHomeButtonDisplayMode = new DuiComboBox(this);
    //% "Home Button"
    comboHomeButtonDisplayMode->setTitle(qtTrId("xx_navigationbar_homebtncombo"));
    comboHomeButtonDisplayMode->setIconVisible(false);
    connect(comboHomeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeHomeButtonDisplayMode(int)));

    lytButtons->addItem(comboNavigationBarDisplayMode);
    lytButtons->addItem(comboHomeButtonDisplayMode);
    lytButtons->addItem(comboEscapeButtonDisplayMode);

    DuiSeparator *separator = new DuiSeparator(this);

    lblEscapeModeDesc = new DuiLabel(this);
    comboEscapeMode = new DuiComboBox(this);

    connect(comboEscapeMode, SIGNAL(currentIndexChanged(int)), SLOT(changeEscapeButtonMode(int)));

    lytMain->addItem(lblDisplayMode);
    lytMain->addItem(lytButtons);
    lytMain->addItem(separator);
    lytMain->addItem(lblEscapeModeDesc);
    lytMain->addItem(comboEscapeMode);

    centralWidget()->setLayout(lytMain);
    QTimer::singleShot(0, this, SLOT(setButtonsState()));

    addExampleActions();
    retranslateUi();
}

void NavigationBarPage::addExampleActions()
{
    DuiAction *action;

    action = new DuiAction("Icon-video", "Video", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);

    action = new DuiAction("Icon-new-SMS", "SMS", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);

    action = new DuiAction("Icon-contacts", "Contacts", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
}

void NavigationBarPage::retranslateUi()
{
    //% "Navigation Bar"
    setTitle(qtTrId("xx_navigationbar_page_title"));
    if (!isContentCreated())
        return;
    //% "Components' display mode:"
    lblDisplayMode->setText(qtTrId("xx_navigationbar_display_mode"));
    //% "Escape Button mode:"
    lblEscapeModeDesc->setText(qtTrId("xx_navigationbar_escapebutton_mode_label"));
    int oldIndex = comboEscapeMode->currentIndex();
    comboEscapeMode->clear();
    comboEscapeMode->insertItem(BackButton,
                                "Icon-back",
                                //% "Back button"
                                qtTrId("xx_navigationbar_back_button"));
    comboEscapeMode->insertItem(CloseButton,
                                "Icon-close",
                                //% "Close button"
                                qtTrId("xx_navigationbar_close_button"));
    comboEscapeMode->insertItem(Hidden,
                                //% "Hidden"
                                qtTrId("xx_navigationbar_hidden"));

    if (oldIndex == -1)
        comboEscapeMode->setCurrentIndex(0);
    else
        comboEscapeMode->setCurrentIndex(oldIndex);

    retranslateDisplayModeComboBox(comboNavigationBarDisplayMode);
    retranslateDisplayModeComboBox(comboHomeButtonDisplayMode);
    retranslateDisplayModeComboBox(comboEscapeButtonDisplayMode);
}

void NavigationBarPage::retranslateDisplayModeComboBox(DuiComboBox *combo)
{
    int oldIndex = combo->currentIndex();

    combo->clear();
    //% "Show"
    combo->insertItem(ComboShow, qtTrId("xx_displaymode_show"));
    //% "Auto Hide"
    combo->insertItem(ComboAutoHide, qtTrId("xx_displaymode_autohide"));
    //% "Hide"
    combo->insertItem(ComboHide, qtTrId("xx_displaymode_hide"));

    if (oldIndex == -1)
        combo->setCurrentIndex(0);
    else
        combo->setCurrentIndex(oldIndex);
}

void NavigationBarPage::changeEscapeButtonMode(int index)
{
    switch (index) {
    case BackButton:
        setEscapeButtonMode(DuiEscapeButtonPanelModel::BackMode);
        break;
    case CloseButton:
        setEscapeButtonMode(DuiEscapeButtonPanelModel::CloseMode);
        break;
    }
}

void NavigationBarPage::changeNavigationBarDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(NavigationBar, DuiApplicationPageModel::Show);
        break;
    case ComboAutoHide:
        setComponentsDisplayMode(NavigationBar, DuiApplicationPageModel::AutoHide);
        break;
    default:
        setComponentsDisplayMode(NavigationBar, DuiApplicationPageModel::Hide);
    }
}

void NavigationBarPage::changeEscapeButtonDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(EscapeButton, DuiApplicationPageModel::Show);
        break;
    case ComboAutoHide:
        setComponentsDisplayMode(EscapeButton, DuiApplicationPageModel::AutoHide);
        break;
    default:
        setComponentsDisplayMode(EscapeButton, DuiApplicationPageModel::Hide);
    }
}

void NavigationBarPage::changeHomeButtonDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(HomeButton, DuiApplicationPageModel::Show);
        break;
    case ComboAutoHide:
        setComponentsDisplayMode(HomeButton, DuiApplicationPageModel::AutoHide);
        break;
    default:
        setComponentsDisplayMode(HomeButton, DuiApplicationPageModel::Hide);
    }
}

void NavigationBarPage::setButtonsState()
{
    comboEscapeMode->setCurrentIndex(escapeButtonMode());
}

