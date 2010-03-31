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

#include "applicationmenupage.h"

#include <QStringListModel>

#include <DuiApplication>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiApplicationPage>
#include <DuiLinearLayoutPolicy>
#include <DuiAction>
#include <DuiApplicationMenu>
#include <DuiWidgetAction>

#include <DuiNavigationBar>
#include <DuiApplicationWindow>
#include <DuiSeparator>
#include <DuiDebug>
#include <DuiButton>
#include <DuiGridLayoutPolicy>
#include <DuiComboBox>

ApplicationMenuPage::ApplicationMenuPage()
{
    gid = TemplatePage::ViewsAndDialogs;
    m_textIndex = 2;
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
    setTitle(qtTrId("xx_viewmenu_page_title"));
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
}

void ApplicationMenuPage::createContent()
{
    TemplatePage::createContent();

    QGraphicsWidget *panel = centralWidget();

    DuiAction *action = new DuiAction("Italic", panel);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(makeTextItalic()));

    action = new DuiAction("Normal", panel);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(makeTextNormal()));

    DuiWidgetAction *widgetAction = new DuiWidgetAction(panel);
    widgetAction->setLocation(DuiAction::ApplicationMenuLocation);

    QStringList list;
    for (int i = 0; i < 5; ++i) {
        list << QString::number(100 + i);
    }
    comboBox = new DuiComboBox;
    comboBox->addItems(list);

    comboBox->setIconVisible(false);
    comboBox->setTitle("ComboBox");
    comboBox->setCurrentIndex(0);
    widgetAction->setWidget(comboBox);

    action = new DuiAction(NULL, "Window Icon", panel);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(toggleWindowIconVisibility()));
    insertAction(action, widgetAction);

    action = new DuiAction("icon-m-list", "", panel);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    action->setStyleAction(true);
    action->setCheckable(true);
    action->setChecked(true);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showButtonsAsList()));

    action = new DuiAction("icon-m-grid", "", panel);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    action->setStyleAction(true);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showButtonsAsGrid()));

    // for more readability
    listPolicy = containerPolicy;
    gridPolicy = new DuiGridLayoutPolicy(containerLayout);

    DuiButton *button = new DuiButton("Item 1", container);
    containerPolicy->addItem(button);
    gridPolicy->addItem(button, 0, 0);
    button = new DuiButton("Item 2", container);
    containerPolicy->addItem(button);
    gridPolicy->addItem(button, 0, 1);
    button = new DuiButton("Item 3", container);
    containerPolicy->addItem(button);
    gridPolicy->addItem(button, 1, 0);
    button = new DuiButton("Item 4", container);
    containerPolicy->addItem(button);
    gridPolicy->addItem(button, 1, 1);

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
    DuiApplicationWindow *window = DuiApplication::activeApplicationWindow();
    if (!window) {
        return;
    }
    QString oldIconID = window->windowIconID();
    if (oldIconID.isEmpty()) {
        window->setWindowIconID("Icon-browser");
    } else {
        window->setWindowIconID("");
    }
}

void ApplicationMenuPage::showButtonsAsList()
{
    containerLayout->setPolicy(listPolicy);
}

void ApplicationMenuPage::showButtonsAsGrid()
{
    containerLayout->setPolicy(gridPolicy);
}
