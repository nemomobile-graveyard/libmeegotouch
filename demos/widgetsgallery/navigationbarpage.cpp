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

#include "navigationbarpage.h"

#include <MAction>
#include <MLocale>
#include <MLabel>
#include <MButton>
#include <MSeparator>
#include <MComboBox>
#include <MApplication>
#include <MApplicationWindow>
#include <MEscapeButtonPanel>

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

QString NavigationBarPage::timedemoTitle()
{
    return "NavigationBar";
}

void NavigationBarPage::createContent()
{
    QGraphicsLinearLayout *lytMain = new QGraphicsLinearLayout(Qt::Vertical);
    lblDisplayMode = new MLabel(this);

    QGraphicsLinearLayout *lytButtons = new QGraphicsLinearLayout(Qt::Vertical);

    // combo box nav bar display mode
    comboNavigationBarDisplayMode = new MComboBox(this);
    //%  "Navigation Bar"
    comboNavigationBarDisplayMode->setTitle(qtTrId("xx_navigationbar_navbarcombo"));
    comboNavigationBarDisplayMode->setIconVisible(false);
    connect(comboNavigationBarDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeNavigationBarDisplayMode(int)));

    // combo box escape button display mode
    comboEscapeButtonDisplayMode = new MComboBox(this);
    //% "Escape Button"
    comboEscapeButtonDisplayMode->setTitle(qtTrId("xx_navigationbar_escapebtncombo"));
    comboEscapeButtonDisplayMode->setIconVisible(false);
    connect(comboEscapeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeEscapeButtonDisplayMode(int)));

    // combo box home button display mode
    comboHomeButtonDisplayMode = new MComboBox(this);
    //% "Home Button"
    comboHomeButtonDisplayMode->setTitle(qtTrId("xx_navigationbar_homebtncombo"));
    comboHomeButtonDisplayMode->setIconVisible(false);
    connect(comboHomeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeHomeButtonDisplayMode(int)));

    lytButtons->addItem(comboNavigationBarDisplayMode);
    lytButtons->addItem(comboHomeButtonDisplayMode);
    lytButtons->addItem(comboEscapeButtonDisplayMode);

    MSeparator *separator = new MSeparator(this);

    lblEscapeModeDesc = new MLabel(this);
    comboEscapeMode = new MComboBox(this);

    connect(comboEscapeMode, SIGNAL(currentIndexChanged(int)), SLOT(changeEscapeMode(int)));

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
    MAction *action;

    action = new MAction("Icon-video", "Video", this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);

    action = new MAction("Icon-new-SMS", "SMS", this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);

    action = new MAction("Icon-contacts", "Contacts", this);
    action->setLocation(MAction::ToolBarLocation);
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
    lblEscapeModeDesc->setText(qtTrId("xx_navigationbar_escape_mode_label"));
    int oldIndex = comboEscapeMode->currentIndex();
    comboEscapeMode->clear();
    comboEscapeMode->insertItem(Auto,
                                //% "Auto"
                                qtTrId("xx_navigationbar_auto"));
    comboEscapeMode->insertItem(ManualBack,
                                "Icon-back",
                                //% "Manual Back"
                                qtTrId("xx_navigationbar_manual_back"));
    comboEscapeMode->insertItem(CloseWindow,
                                "Icon-close",
                                //% "Close Window"
                                qtTrId("xx_navigationbar_close_window"));

    if (oldIndex == -1)
        comboEscapeMode->setCurrentIndex(0);
    else
        comboEscapeMode->setCurrentIndex(oldIndex);

    retranslateDisplayModeComboBox(comboNavigationBarDisplayMode);
    retranslateDisplayModeComboBox(comboHomeButtonDisplayMode);
    retranslateDisplayModeComboBox(comboEscapeButtonDisplayMode);
}

void NavigationBarPage::retranslateDisplayModeComboBox(MComboBox *combo)
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

void NavigationBarPage::changeEscapeMode(int index)
{
    switch (index) {
        case Auto:
            setEscapeMode(MApplicationPageModel::EscapeAuto);
            break;
        case ManualBack:
            setEscapeMode(MApplicationPageModel::EscapeManualBack);
            break;
        case CloseWindow:
            setEscapeMode(MApplicationPageModel::EscapeCloseWindow);
            break;
        default:
            qFatal("Invalid index");
    }
}

void NavigationBarPage::changeNavigationBarDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(NavigationBar, MApplicationPageModel::Show);
        break;
    case ComboAutoHide:
        setComponentsDisplayMode(NavigationBar, MApplicationPageModel::AutoHide);
        break;
    default:
        setComponentsDisplayMode(NavigationBar, MApplicationPageModel::Hide);
    }
}

void NavigationBarPage::changeEscapeButtonDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(EscapeButton, MApplicationPageModel::Show);
        break;
    case ComboAutoHide:
        setComponentsDisplayMode(EscapeButton, MApplicationPageModel::AutoHide);
        break;
    default:
        setComponentsDisplayMode(EscapeButton, MApplicationPageModel::Hide);
    }
}

void NavigationBarPage::changeHomeButtonDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(HomeButton, MApplicationPageModel::Show);
        break;
    case ComboAutoHide:
        setComponentsDisplayMode(HomeButton, MApplicationPageModel::AutoHide);
        break;
    default:
        setComponentsDisplayMode(HomeButton, MApplicationPageModel::Hide);
    }
}

void NavigationBarPage::setButtonsState()
{
    switch (escapeMode()) {
        case MApplicationPageModel::EscapeAuto:
            comboEscapeMode->setCurrentIndex(Auto);
            break;
        case MApplicationPageModel::EscapeManualBack:
            comboEscapeMode->setCurrentIndex(ManualBack);
            break;
        case MApplicationPageModel::EscapeCloseWindow:
            comboEscapeMode->setCurrentIndex(CloseWindow);
            break;
        default:
            qFatal("Invalid page escape mode.");
    }
}

