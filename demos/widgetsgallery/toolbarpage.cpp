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

#include "toolbarpage.h"
#include <DuiAction>
#include <DuiButton>
#include <DuiTextEdit>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiWidgetAction>
#include <DuiLayout>
#include <DuiSeparator>

ToolBarPage::ToolBarPage()
{
    gid = TemplatePage::ViewsAndDialogs;
}

ToolBarPage::~ToolBarPage()
{
}

void ToolBarPage::createContent()
{
    TemplatePage::createContent();

    //% "Example 1"
    DuiAction *action = new DuiAction(qtTrId("xx_toolbar_page_example1"), this);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    connect(action, SIGNAL(triggered()), this, SLOT(fourButtons()));
    addAction(action);

    //% "Example 2"
    action = new DuiAction(qtTrId("xx_toolbar_page_example2"), this);
    action->setLocation(DuiAction::ApplicationMenuLocation);
    connect(action, SIGNAL(triggered()), this, SLOT(textEntryWithTwoButtons()));
    addAction(action);

    fourButtons();

    retranslateUi();
}


void ToolBarPage::createLayout()
{
    TemplatePage::createLayout();
    layout->removeItem(container);
    layout->removeItem(separator);
}

void ToolBarPage::retranslateUi()
{
    TemplatePage::retranslateUi();

    //% "ToolBar"
    setTitle(qtTrId("xx_toolbar_page_title"));
    if (!isContentCreated())
        return;

    //% "<b>Toolbar</b> is a container which contains actions that "
    //% "are relevant to the view but not related to any individual "
    //% "item displayed directly within the content area. "
    //% "<ul><li>It can contain up to 4 Icon Buttons "
    //% "(click Menu and then Example 1)</li> "
    //% "<li>A text input field can also be added along with up to "
    //% "2 Icon Buttons (click Menu and then Example 2).</li></ul>"
    infoLabel->setText("<a></a>" + qtTrId("xx_toolbar_page_info"));
}

void ToolBarPage::fourButtons()
{
    clearToolbarActions();
    DuiAction *action = new DuiAction("Icon-pictures", "Pictures", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    DuiAction *actionSMS = new DuiAction("Icon-new-SMS", "SMS", this);
    actionSMS->setLocation(DuiAction::ToolBarLocation);
    insertAction(action, actionSMS);
    action = new DuiAction("Icon-contacts", "Contacts", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    DuiAction *actionVideo = new DuiAction("Icon-video", "Video", this);
    actionVideo->setLocation(DuiAction::ToolBarLocation);
    insertAction(action, actionVideo);
}

void ToolBarPage::textEntryWithTwoButtons()
{
    clearToolbarActions();
    addTextEntry();
    DuiAction *action = new DuiAction("Icon-video", "Video", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    DuiAction *actionSMS = new DuiAction("Icon-new-SMS", "SMS", this);
    actionSMS->setLocation(DuiAction::ToolBarLocation);
    insertAction(action, actionSMS);
}

void ToolBarPage::addTextEntry()
{
    DuiTextEdit *entry = new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());
    entry->setViewType("toolbar");
    DuiWidgetAction *action = new DuiWidgetAction(this);
    action->setLocation(DuiAction::ToolBarLocation);
    action->setWidget(entry);
    addAction(action);
}

void ToolBarPage::clearToolbarActions()
{
    QList<QAction *> acts = actions();
    const int actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        DuiAction *action = qobject_cast<DuiAction *>(acts[i]);
        if (action && action->location().testFlag(DuiAction::ToolBarLocation)) {
            removeAction(action);
        }
    }
}
