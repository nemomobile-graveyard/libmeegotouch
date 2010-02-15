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

#include "templatepage.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiApplication>
#include <DuiWindow>
#include <DuiButton>
#include <DuiSeparator>
#include <DuiLabel>
#include <DuiLocale>

TemplatePage::TemplatePage() :
    DuiApplicationPage(), gid(0),
    layout(0), landscapePolicy(0), portraitPolicy(0),
    container(0), separator(0), infoLabel(0),
    containerLayout(0), containerPolicy(0)
{
}

TemplatePage::~TemplatePage()
{
}

QStringList TemplatePage::groupNames()
{
    QStringList list;
    //% "Views and Dialogs"
    list << qtTrId("xx_template_page_views_dialogs");
    //% "Contains and Applets"
    list << qtTrId("xx_template_page_contains_applets");
    //% "Layouts and Visuals"
    list << qtTrId("xx_template_page_layouts_visuals");
    //% "Buttons"
    list << qtTrId("xx_template_page_buttons");
    //% "Indicators and Notifications"
    list << qtTrId("xx_template_page_indicators_notifications");
    //% "Lists, Grids and Menus"
    list << qtTrId("xx_template_page_lists_grids_menus");
    //% "User input"
    list << qtTrId("xx_template_page_user_input");
    //% "Input Feedback"
    list << qtTrId("xx_template_page_input_feedback");
    return list;
}

int TemplatePage::groupID()
{
    return gid;
}

void TemplatePage::createContent()
{
    DuiApplicationPage::createContent();
    createLayout();
    containerLayout = new DuiLayout(container);
    containerPolicy = new DuiLinearLayoutPolicy(containerLayout, Qt::Vertical);
    containerLayout->setPolicy(containerPolicy);
}

void TemplatePage::createLayout()
{
    DuiWidget *panel = centralWidget();
    layout = new DuiLayout(panel);

    landscapePolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->setContentsMargins(0, 30, 0, 0);

    portraitPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->setContentsMargins(0, 30, 0, 0);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    // main container
    QSize s = DuiApplication::activeWindow()->visibleSceneSize();

    container = new DuiWidget();
    container->setMaximumWidth(s.height());
    container->setMinimumWidth(s.height());

    separator = new DuiSeparator();
    separator->setOrientation(Qt::Vertical);

    infoLabel = new DuiLabel();
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);

    landscapePolicy->addItem(container);
    landscapePolicy->addItem(separator);
    landscapePolicy->addItem(infoLabel);

    portraitPolicy->addItem(infoLabel);
    portraitPolicy->addItem(container);
}

void TemplatePage::retranslateUi()
{
    //% "Template"
    setTitle(qtTrId("xx_template_page_title"));

    if (!isContentCreated())
        return;

    //% "Sample template"
    infoLabel->setText(qtTrId("xx_sample template"));
}

