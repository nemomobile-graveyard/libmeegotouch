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

#define CONTAINER_MIN_WIDTH 400

TemplatePage::TemplatePage(TemplatePage::Category category) :
    TimedemoPage(),
    layout(0), landscapePolicy(0), portraitPolicy(0),
    container(0), separator(0), infoLabel(0),
    containerLayout(0), containerPolicy(0), pageCategory(category)
{
}

TemplatePage::~TemplatePage()
{
}

TemplatePage::Category TemplatePage::category()
{
    return pageCategory;
}
/*
QStringList TemplatePage::groupNames()
{

}*/

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
    container->setMinimumWidth(CONTAINER_MIN_WIDTH);
    container->setPreferredWidth(CONTAINER_MIN_WIDTH);
    container->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    separator = new MSeparator();
    separator->setOrientation(Qt::Vertical);

    infoLabel = new MLabel();
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

