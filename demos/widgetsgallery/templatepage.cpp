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

#include "templatepage.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MApplication>
#include <MWindow>
#include <MButton>
#include <MSeparator>
#include <MLabel>
#include <MLocale>

TemplatePage::TemplatePage() :
    gid(0),
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
    MApplicationPage::createContent();
    createLayout();
    containerLayout = new MLayout(container);
    containerPolicy = new MLinearLayoutPolicy(containerLayout, Qt::Vertical);
    containerLayout->setPolicy(containerPolicy);
}

void TemplatePage::createLayout()
{
    QGraphicsWidget *panel = centralWidget();
    layout = new MLayout(panel);

    landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->setContentsMargins(0, 30, 0, 0);

    portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->setContentsMargins(0, 30, 0, 0);

    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);

    container = new MWidget();
    container->setMinimumWidth(0);
    container->setPreferredWidth(0);
    container->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    separator = new MSeparator();
    separator->setOrientation(Qt::Vertical);

    infoLabel = new MLabel();
    infoLabel->setMinimumWidth(0);
    infoLabel->setPreferredWidth(0);
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);
    infoLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

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

