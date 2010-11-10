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

#include "customnavigationbarpage.h"
#include "mynavbarcontent.h"

#include <QGraphicsLinearLayout>

CustomNavigationBarPage::CustomNavigationBarPage()
    : TemplatePage(TemplatePage::ApplicationView)
{
}

CustomNavigationBarPage::~CustomNavigationBarPage()
{
}

QString CustomNavigationBarPage::timedemoTitle()
{
    return "CustomNavigationBar";
}

void CustomNavigationBarPage::createContent()
{
    MApplicationPage::createContent();

    infoLabel = new MLabel;
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop);

    setCentralWidget(infoLabel);

    MyNavBarContent *myNavBarContent = new MyNavBarContent;
    connect(myNavBarContent, SIGNAL(okClicked()), SLOT(dismiss()));
    setCustomNavigationBarContent(myNavBarContent);

    retranslateUi();
}

void CustomNavigationBarPage::retranslateUi()
{
    //% "Pages can define their own custom content for the navigation bar."
    //% " This is useful for cases that demand more flexibility than what"
    //% " MAction API can provide.\n\n"
    //% "In this example, pressing \"OK\" will cause the page to be dismissed,"
    //% "which is exactly what the standard back button would do.\n\n"
    //% "When a custom navigation bar content is used, the application developer"
    //% " is responsible for providing the navigation controls (back, close,"
    //% " menu) himself."
    infoLabel->setText("<a></a>" + qtTrId("xx_customnavbar_page_info_label").replace('\n', "<br>"));
}