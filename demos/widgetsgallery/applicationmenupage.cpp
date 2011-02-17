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

#include "applicationmenupage.h"

#include <QStringListModel>

#include <MApplication>
#include <MLabel>
#include <MLayout>
#include <MLocale>
#include <MApplicationPage>
#include <MLinearLayoutPolicy>
#include <MAction>
#include <MApplicationMenu>
#include <MWidgetAction>

#include <MNavigationBar>
#include <MApplicationWindow>
#include <MSeparator>
#include <MDebug>
#include <MButton>
#include <MGridLayoutPolicy>
#include <MComboBox>

ApplicationMenuPage::ApplicationMenuPage()
    : TemplatePage(TemplatePage::ApplicationView)
    , m_textIndex(2)
    , policy(0)
    , comboBox(0)
    , actionItalic(0)
    , actionNormal(0)
    , actionIcon(0)
{
}

ApplicationMenuPage::~ApplicationMenuPage()
{
}

QString ApplicationMenuPage::timedemoTitle()
{
    return "ApplicationMenu";
}

void ApplicationMenuPage::retranslateUi()
{
    //% "Application Menu"
    setTitle(qtTrId("xx_applicationmenu_page_title"));
    if (!isContentCreated())
        return;

    //% "Application Menu is a menu element available to any view. "
    //% "<ul><li>It can be opened by tapping on the menu title area."
    //% "</li><li>It contains commands and view settings for the view "
    //% "in question and an optional area for style commands.</li>"
    //% "<li>Each view of the application can have different menu "
    //% "contents.</li></ul><b>Guidelines</b> for mandatory menu items: "
    //% "<ul><li>Settings should be on the main View of the application, "
    //% "unless the settings are specific to a sub-View</li>"
    //% "<li>Help should be available in each View </li>"
    //% "<li>Exit task should be present in all Views except for the "
    //% "main View of the application.</li></ul>"
    infoLabel->setText("<a></a>" + qtTrId("xx_applicationmenu_page_info"));

    showText(m_textIndex);

    //% "Italic"
    actionItalic->setText(qtTrId("xx_appmenu_italic"));
    //% "Normal"
    actionNormal->setText(qtTrId("xx_appmenu_normal"));
    //% "Window Icon"
    actionIcon->setText(qtTrId("xx_appmenu_window_icon"));
}

void ApplicationMenuPage::createContent()
{
    MApplicationPage::createContent();

    QGraphicsWidget *panel = centralWidget();

    infoLabel = new MLabel(panel);
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);

    actionItalic = new MAction(panel);
    actionItalic->setLocation(MAction::ApplicationMenuLocation);
    addAction(actionItalic);
    connect(actionItalic, SIGNAL(triggered()), this, SLOT(makeTextItalic()));

    actionNormal = new MAction(panel);
    actionNormal->setLocation(MAction::ApplicationMenuLocation);
    addAction(actionNormal);
    connect(actionNormal, SIGNAL(triggered()), this, SLOT(makeTextNormal()));

    MWidgetAction *widgetAction = new MWidgetAction(panel);
    widgetAction->setLocation(MAction::ApplicationMenuLocation);

    QStringList list;
    for (int i = 0; i < 5; ++i) {
        list << QString::number(100 + i);
    }
    comboBox = new MComboBox;
    comboBox->addItems(list);

    comboBox->setIconVisible(false);
    comboBox->setTitle("ComboBox");
    comboBox->setCurrentIndex(0);
    widgetAction->setWidget(comboBox);

    actionIcon = new MAction(panel);
    actionIcon->setLocation(MAction::ApplicationMenuLocation);
    addAction(actionIcon);
    connect(actionIcon, SIGNAL(triggered()), this, SLOT(toggleWindowIconVisibility()));
    insertAction(actionIcon, widgetAction);

    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);

    policy->addItem(infoLabel);

    retranslateUi();
}

void ApplicationMenuPage::showText(int index)
{
    m_textIndex = index;
    QFont f = infoLabel->font();

    switch (index) {
    case 1: {
        f.setItalic(true);
        infoLabel->setFont(f);
        break;
    }
    case 2: {
        f.setBold(false); f.setItalic(false);
        infoLabel->setFont(f);
        infoLabel->setAlignment(Qt::AlignLeft);
        break;
    }
    }
}

void ApplicationMenuPage::makeTextItalic()
{
    showText(1);
}

void ApplicationMenuPage::makeTextNormal()
{
    showText(2);
}

void ApplicationMenuPage::toggleWindowIconVisibility()
{
    MApplicationWindow *window = MApplication::activeApplicationWindow();
    if (!window) {
        return;
    }
    QString oldIconID = window->windowIconID();
    if (oldIconID.isEmpty()) {
        window->setWindowIconID("icon-m-toolbar-gallery");
    } else {
        window->setWindowIconID("");
    }
}
