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

#include "languagepage.h"

#include <DuiLayout>
#include <DuiLocale>
#include <DuiPopupList>
#include <DuiLabel>
#include <DuiGridLayoutPolicy>
#include <DuiButton>
#include <DuiComboBox>
#include <DuiTheme>
#include <DuiGConfItem>

#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QStringList>

LanguagePage::LanguagePage()
    : comboBox(0),
      model(0)
{
    gid = TemplatePage::UserInput;
}

LanguagePage::~LanguagePage()
{
}

void LanguagePage::createContent()
{
    DuiApplicationPage::createContent();
    DuiWidget *panel = centralWidget();
    DuiLayout *layout = new DuiLayout(panel);
    DuiGridLayoutPolicy *policy = new DuiGridLayoutPolicy(layout);
    comboBox = new DuiComboBox;
    comboBox->setIconID("Icon-browser");
    policy->addItem(comboBox, 1, 1);
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLanguage(int)));
    retranslateUi();
}

void LanguagePage::retranslateUi()
{
    //% "Change Language"
    setTitle(qtTrId("xx_language_title"));
    if (!isContentCreated())
        return;

    QList<QStringList> rows;
    //% "None"
    rows << (QStringList() << qtTrId("xx_language_none") << "")
         //% "Arabic"
         << (QStringList() << qtTrId("xx_language_arabic") << "ar")
         //% "English"
         << (QStringList() << qtTrId("xx_language_english") << "en")
         //% "Finnish"
         << (QStringList() << qtTrId("xx_language_finnish") << "fi")
         //% "German"
         << (QStringList() << qtTrId("xx_language_german") << "de")
         //% "Simplified Chinese"
         << (QStringList() << qtTrId("xx_language_simplified_chinese") << "zh_CN")
         //% "Magyar"
         << (QStringList() << qtTrId("xx_language_hungarian") << "hu");

    QStandardItemModel *newModel = new QStandardItemModel(rows.count(), 2, this);
    int rowsCount = rows.count();
    for (int i = 0; i < rowsCount; ++i)
        for (int j = 0; j < 2; ++j)
            newModel->setItem(i, j, new QStandardItem(rows[i][j]));

//    comboBox->clear();
    comboBox->setItemModel(newModel);
    if (model) delete model;
    model = newModel;
    DuiGConfItem languageItem("/Dui/i18n/Language");
    QString currentLanguage = languageItem.value().toString();
    rowsCount = rows.count();
    for (int i = 0; i < rowsCount; ++i) {
        if (currentLanguage == model->item(i, 1)->text())
            comboBox->setCurrentIndex(i);
    }
    //% "Please select language"
    comboBox->setTitle(qtTrId("xx_language_combobox_title"));
}

void LanguagePage::changeLanguage(int index)
{
    if ((uint) index >= model->rowCount())
        return;
    DuiGConfItem languageItem("/Dui/i18n/Language");
    languageItem.set(model->item(index, 1)->text());
}
