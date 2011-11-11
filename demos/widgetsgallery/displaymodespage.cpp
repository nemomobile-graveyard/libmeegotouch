/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <MContainer>
#include <MPannableViewport>

#include <QGraphicsLinearLayout>
#include <QStringList>
#include <QTimer>
#include <QPropertyAnimation>
#include "labeledcheckbox.h"

DisplayModesPage::DisplayModesPage()
    : TemplatePage(TemplatePage::ApplicationView),
      comboNavigationBarDisplayMode(0),
      comboEscapeButtonDisplayMode(0),
      comboHomeButtonDisplayMode(0),
      comboStatusBarDisplayMode(0),
      ctnDisplayMode(0),
      ctnWindowState(0),
      fullScreenCheckbox(0),
      navigationBarTransparencyCheckbox(0),
      disableStatusBarEnforcementDuringCallCheckbox(0),
      roundedCornersCheckbox(0)
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
    bool ok;
    setStyleName(inv("CommonApplicationPage"));

    QGraphicsLinearLayout *lytMain = new QGraphicsLinearLayout(Qt::Vertical);
    lytMain->setContentsMargins(0, 0, 0, 0);
    lytMain->setSpacing(0);

    ctnDisplayMode = new MContainer(this);
    ctnDisplayMode->setStyleName(inv("CommonContainer"));

    createWindowStateWidgets();

    QGraphicsLinearLayout *lytDisplayMode = new QGraphicsLinearLayout(Qt::Vertical, ctnDisplayMode->centralWidget()) ;
    lytDisplayMode->setContentsMargins(0, 0, 0, 0);
    lytDisplayMode->setSpacing(0);

    QGraphicsLinearLayout *lytWindowState = new QGraphicsLinearLayout(Qt::Vertical, ctnWindowState->centralWidget());
    lytWindowState->setContentsMargins(0, 0, 0, 0);
    lytWindowState->setSpacing(0);

    // combo box nav bar display mode
    comboNavigationBarDisplayMode = new MComboBox;
    comboNavigationBarDisplayMode->setObjectName("comboNavigationBarDisplayMode");
    comboNavigationBarDisplayMode->setStyleName(inv("CommonComboBox"));
    //%  "Navigation Bar"
    comboNavigationBarDisplayMode->setTitle(qtTrId("xx_displaymodes_navbarcombo"));
    comboNavigationBarDisplayMode->setIconVisible(false);
    connect(comboNavigationBarDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeNavigationBarDisplayMode(int)));

    // combo box escape button display mode
    comboEscapeButtonDisplayMode = new MComboBox;
    comboEscapeButtonDisplayMode->setObjectName("comboEscapeButtonDisplayMode");
    comboEscapeButtonDisplayMode->setStyleName(inv("CommonComboBox"));
    //% "Escape Button"
    comboEscapeButtonDisplayMode->setTitle(qtTrId("xx_displaymodes_escapebtncombo"));
    comboEscapeButtonDisplayMode->setIconVisible(false);
    connect(comboEscapeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeEscapeButtonDisplayMode(int)));

    // combo box home button display mode
    comboHomeButtonDisplayMode = new MComboBox;
    comboHomeButtonDisplayMode->setObjectName("comboHomeButtonDisplayMode");
    comboHomeButtonDisplayMode->setStyleName(inv("CommonComboBox"));
    //% "Home Button"
    comboHomeButtonDisplayMode->setTitle(qtTrId("xx_displaymodes_homebtncombo"));
    comboHomeButtonDisplayMode->setIconVisible(false);
    connect(comboHomeButtonDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeHomeButtonDisplayMode(int)));

    // combo box status bar display mode
    comboStatusBarDisplayMode = new MComboBox;
    comboStatusBarDisplayMode->setObjectName("comboStatusBarDisplayMode");
    comboStatusBarDisplayMode->setStyleName(inv("CommonComboBox"));
    //% "Status Bar"
    comboStatusBarDisplayMode->setTitle(qtTrId("xx_displaymodes_statusbarcombo"));
    comboStatusBarDisplayMode->setIconVisible(false);
    ok = connect(comboStatusBarDisplayMode, SIGNAL(currentIndexChanged(int)),
            SLOT(changeStatusBarDisplayMode(int)));
    if (!ok) qFatal("signal connection failed!");

    lytDisplayMode->addItem(comboNavigationBarDisplayMode);
    lytDisplayMode->addItem(navigationBarTransparencyCheckbox);
    lytDisplayMode->addItem(comboHomeButtonDisplayMode);
    lytDisplayMode->addItem(comboEscapeButtonDisplayMode);
    lytDisplayMode->addItem(comboStatusBarDisplayMode);
    lytDisplayMode->addItem(disableStatusBarEnforcementDuringCallCheckbox);

    lytWindowState->addItem(fullScreenCheckbox);
    lytWindowState->addItem(roundedCornersCheckbox);

    lytMain->addItem(ctnDisplayMode);
    lytMain->addItem(ctnWindowState);

    centralWidget()->setLayout(lytMain);
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);

    addExampleActions();
    retranslateUi();
}

void DisplayModesPage::createWindowStateWidgets()
{
    ctnWindowState = new MContainer(this);
    ctnWindowState->setObjectName("ctnWindowState");
    ctnWindowState->setStyleName(inv("CommonContainer"));

    fullScreenCheckbox = new LabeledCheckbox;
    fullScreenCheckbox->setParent(ctnWindowState);
    fullScreenCheckbox->setObjectName("fullScreenCheckbox");
    connect(fullScreenCheckbox->button, SIGNAL(toggled(bool)), SLOT(changeFullScreenMode(bool)));
    // Init "full screen" checkbox state
    if (MApplication::activeWindow()) {
        fullScreenCheckbox->button->setChecked(MApplication::activeWindow()->isFullScreen());
    }

    roundedCornersCheckbox = new LabeledCheckbox;
    roundedCornersCheckbox->setParent(ctnWindowState);
    roundedCornersCheckbox->setObjectName("roundedCornersCheckbox");
    connect(roundedCornersCheckbox->button, SIGNAL(toggled(bool)), SLOT(changeRoundedCorners(bool)));
    // Init "rounded corners" checkbox state
    if (MApplication::activeWindow()) {
        roundedCornersCheckbox->button->setChecked(MApplication::activeWindow()->isRoundedCornersEnabled());
    }

    navigationBarTransparencyCheckbox = new LabeledCheckbox;
    navigationBarTransparencyCheckbox->setParent(ctnDisplayMode);
    navigationBarTransparencyCheckbox->setObjectName("navigationBarTransparencyCheckbox");
    connect(navigationBarTransparencyCheckbox->button, SIGNAL(toggled(bool)), SLOT(changeNavigationBarTransparency(bool)));

    disableStatusBarEnforcementDuringCallCheckbox = new LabeledCheckbox;
    disableStatusBarEnforcementDuringCallCheckbox->setParent(ctnDisplayMode);
    disableStatusBarEnforcementDuringCallCheckbox->setObjectName("statusBarTransparencyCheckbox");
    connect(disableStatusBarEnforcementDuringCallCheckbox->button,
            SIGNAL(toggled(bool)), SLOT(changeStatusBarEnforcementDuringCall(bool)));
}

void DisplayModesPage::addExampleActions()
{
    MAction *action;

    if (MApplication::instance()->objectName() == "widgetsgallery") {
        action = new MAction("icon-m-toolbar-new-chat", "Chat", this);
    } else {
        action = new MAction("icon-m-toolbar-new-chat-white", "Chat", this);
    }
    action->setObjectName("chatAction");
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);

    if (MApplication::instance()->objectName() == "widgetsgallery") {
        action = new MAction("icon-m-toolbar-send-sms", "SMS", this);
    } else {
        action = new MAction("icon-m-toolbar-send-sms-white", "SMS", this);
    }
    action->setObjectName("smsAction");
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);

    if (MApplication::instance()->objectName() == "widgetsgallery") {
        action = new MAction("icon-m-toolbar-send-email", "Mail", this);
    } else {
        action = new MAction("icon-m-toolbar-send-email-white", "Mail", this);
    }
    action->setObjectName("mailAction");
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
}

void DisplayModesPage::retranslateUi()
{
    //% "Display Modes"
    setTitle(qtTrId("xx_displaymodes_page_title"));
    if (!isContentCreated())
        return;
    //% "Components' display mode"
    ctnDisplayMode->setTitle(qtTrId("xx_displaymodes_display_mode"));

    //% "Window state"
    ctnWindowState->setTitle(qtTrId("xx_displaymodes_window_state"));

    //% "Full Screen"
    fullScreenCheckbox->label->setText(qtTrId("xx_displaymodes_full_screen"));

    //% "Rounded Corners"
    roundedCornersCheckbox->label->setText(qtTrId("xx_displaymodes_rounded_corners"));

    //% "Navigation Bar Transparency"
    navigationBarTransparencyCheckbox->label->setText(qtTrId("xx_displaymodes_navigation_bar_transparency"));

    //% "Disable Status Bar Enforcement During Call"
    disableStatusBarEnforcementDuringCallCheckbox->label->setText(qtTrId("xx_displaymode_status_bar_enforcement_during_call"));

    retranslateDisplayModeComboBox(comboNavigationBarDisplayMode);
    retranslateDisplayModeComboBox(comboHomeButtonDisplayMode);
    retranslateDisplayModeComboBox(comboEscapeButtonDisplayMode);
    retranslateDisplayModeComboBox(comboStatusBarDisplayMode);
}

void DisplayModesPage::retranslateDisplayModeComboBox(MComboBox *combo)
{
    int oldIndex = combo->currentIndex();

    combo->clear();
    //% "Show"
    combo->insertItem(ComboShow, qtTrId("xx_displaymode_show"));
    if (combo != comboEscapeButtonDisplayMode
        && combo != comboStatusBarDisplayMode) {
        //% "Auto Hide"
        combo->insertItem(ComboAutoHide, qtTrId("xx_displaymode_autohide"));
    }
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

void DisplayModesPage::changeStatusBarDisplayMode(int index)
{
    switch (index) {
    case ComboShow:
        setComponentsDisplayMode(StatusBar, MApplicationPageModel::Show);
        break;
    default:
        setComponentsDisplayMode(StatusBar, MApplicationPageModel::Hide);
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

void DisplayModesPage::changeRoundedCorners(bool enable)
{
    MWindow *window = MApplication::activeWindow();

    if (!window)
        return;

    window->setRoundedCornersEnabled(enable);
}

void DisplayModesPage::changeNavigationBarTransparency(bool transparent)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this);
    animation->setDuration(800);
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->setTargetObject(applicationWindow());
    animation->setPropertyName("navigationBarOpacity");
    animation->setStartValue(applicationWindow()->navigationBarOpacity());
    animation->setEndValue(transparent ? 0.5 : 1.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void DisplayModesPage::changeStatusBarEnforcementDuringCall(bool disable)
{
    MWindow *window = MApplication::activeWindow();

    if (!window)
        return;

    window->setProperty("disableStatusBarEnforcementDuringCall", disable);
}