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

#include "displaymodespage.h"

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

DisplayModesPage::DisplayModesPage() : TemplatePage(TemplatePage::ApplicationView)
{
}

DisplayModesPage::~DisplayModesPage()
{
}

QString DisplayModesPage::timedemoTitle()
{
    return "DisplayModes";
}

void DisplayModesPage::createContent()
{
    QGraphicsLinearLayout *lytMain = new QGraphicsLinearLayout(Qt::Vertical);
    lblDisplayMode = new MLabel(this);

    QGraphicsLinearLayout *lytButtons = new QGraphicsLinearLayout(Qt::Vertical);

    // combo box nav bar display mode
    comboNavigationBarDisplayMode = new MComboBox(this);
    //%  "Navigation Bar"
    comboNavigationBarDisplayMode->setTitle(qtTrId("xx_displaymodes_navbarcombo"));
    comboNavigationBarDisplayMode->setIconVisible(false);
    connect(comboNavigationBarDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeNavigationBarDisplayMode(int)));

    // combo box escape button display mode
    comboEscapeButtonDisplayMode = new MComboBox(this);
    //% "Escape Button"
    comboEscapeButtonDisplayMode->setTitle(qtTrId("xx_displaymodes_escapebtncombo"));
    comboEscapeButtonDisplayMode->setIconVisible(false);
    connect(comboEscapeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeEscapeButtonDisplayMode(int)));

    // combo box home button display mode
    comboHomeButtonDisplayMode = new MComboBox(this);
    //% "Home Button"
    comboHomeButtonDisplayMode->setTitle(qtTrId("xx_displaymodes_homebtncombo"));
    comboHomeButtonDisplayMode->setIconVisible(false);
    connect(comboHomeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeHomeButtonDisplayMode(int)));

    lytButtons->addItem(comboNavigationBarDisplayMode);
    lytButtons->addItem(comboHomeButtonDisplayMode);
    lytButtons->addItem(comboEscapeButtonDisplayMode);

    createWindowStateWidgets();

    lytMain->addItem(lblDisplayMode);
    lytMain->addItem(lytButtons);
    lytMain->addItem(new MSeparator);
    lytMain->addItem(lblWindowState);
    lytMain->addItem(fullScreenCheckboxLayout);

    centralWidget()->setLayout(lytMain);

    addExampleActions();
    retranslateUi();
}

void DisplayModesPage::createWindowStateWidgets()
{
    lblWindowState = new MLabel;

    checkboxFullScreen = new MButton;
    checkboxFullScreen->setViewType(MButton::checkboxType);
    checkboxFullScreen->setCheckable(true);
    connect(checkboxFullScreen, SIGNAL(toggled(bool)), SLOT(changeFullScreenMode(bool)));

    // Init "full screen" checkbox state
    if (MApplication::activeWindow()) {
        checkboxFullScreen->setChecked(MApplication::activeWindow()->isFullScreen());
    }

    lblFullScreen = new MLabel;
    lblFullScreen->setWordWrap(true);
    lblFullScreen->setTextElide(true);

    fullScreenCheckboxLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    fullScreenCheckboxLayout->addItem(checkboxFullScreen);
    fullScreenCheckboxLayout->addItem(lblFullScreen);
    fullScreenCheckboxLayout->setAlignment(checkboxFullScreen, Qt::AlignCenter);
    fullScreenCheckboxLayout->setAlignment(lblFullScreen, Qt::AlignCenter);
}

void DisplayModesPage::addExampleActions()
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

void DisplayModesPage::retranslateUi()
{
    //% "Display Modes and Full Screen"
    setTitle(qtTrId("xx_displaymodes_page_title"));
    if (!isContentCreated())
        return;
    //% "Components' display mode:"
    lblDisplayMode->setText(qtTrId("xx_displaymodes_display_mode"));

    //% "Window state:"
    lblWindowState->setText(qtTrId("xx_displaymodes_window_state"));

    //% "Full Screen"
    lblFullScreen->setText(qtTrId("xx_displaymodes_full_screen"));

    retranslateDisplayModeComboBox(comboNavigationBarDisplayMode);
    retranslateDisplayModeComboBox(comboHomeButtonDisplayMode);
    retranslateDisplayModeComboBox(comboEscapeButtonDisplayMode);
}

void DisplayModesPage::retranslateDisplayModeComboBox(MComboBox *combo)
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

void DisplayModesPage::changeNavigationBarDisplayMode(int index)
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

void DisplayModesPage::changeEscapeButtonDisplayMode(int index)
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

void DisplayModesPage::changeHomeButtonDisplayMode(int index)
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

void DisplayModesPage::changeFullScreenMode(bool fullScreen)
{
    MWindow *window = MApplication::activeWindow();

    if (!window)
        return;

    if (fullScreen) {
        window->showFullScreen();
    } else {
        window->showNormal();
    }
}
