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

#include "checkboxpage.h"
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiApplicationPage>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiButton>
#include <DuiButtonGroup>
#include <DuiDebug>
#include <DuiApplication>
#include <QGraphicsLinearLayout>

CheckboxPage::CheckboxPage() :
    TemplatePage(),
    checkbox(0),
    checkboxLabel(0)
{
    gid = TemplatePage::Buttons;
}

CheckboxPage::~CheckboxPage()
{
}

QString CheckboxPage::timedemoTitle()
{
    return "Checkbox";
}

void CheckboxPage::createContent()
{
    TemplatePage::createContent();

    // Icon buttons
    checkbox = new DuiButton();
    checkbox->setViewType(DuiButton::checkboxType);
    checkbox->setCheckable(true);
    checkboxLabel = new DuiLabel();
    checkboxLabel->setWordWrap(true);
    checkboxLabel->setTextElide(true);
    
    // Layout for checkbox and it's infolabel
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->addItem(checkbox);
    l->addItem(checkboxLabel);
    l->setAlignment(checkbox, Qt::AlignCenter);
    l->setAlignment(checkboxLabel, Qt::AlignCenter);
    containerPolicy->addItem(l);
    containerPolicy->addStretch();
    retranslateUi();
}

void CheckboxPage::retranslateUi()
{
    //% "Checkbox"
    setTitle(qtTrId("xx_checkbox_page_title"));
    if (!isContentCreated())
        return;

    //% "The Checkbox is a variant of Button that allows users "
    //% "to set the state of variable or setting which has two "
    //% "values, On and Off.\n\nCheckbox essentially does the "
    //% "same thing as the Switch button, so please consider "
    //% "using a Switch instead."
    infoLabel->setText("<a></a>" + qtTrId("xx_checkbox_page_info_label"));

    //% "I have read and understood the\ninstructions."
    checkboxLabel->setText("<a></a>" + qtTrId("xx_checkbox_page_checkbox_label"));
}


