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

#include "mrichtexteditdialogsmanager_p.h"
#include "mcolorcombobox.h"

#include <QFontDatabase>
#include <QStringListModel>
#include <QGraphicsSceneMouseEvent>
#include <MApplication>
#include <MApplicationWindow>
#include <MBasicListItem>
#include <MComboBox>
#include <MDialog>
#include <MImageWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MSceneManager>
#include <MTheme>

MRichTextEditDialogsManager *MRichTextEditDialogsManager::dialogsManager = 0;

MRichTextEditDialogsManager::MRichTextEditDialogsManager()
    : fontFamilyCombo(0),
      fontSizeCombo(0),
      fontColorCombo(0),
      selectedFontSize(-1)
{
    //Maximum 64pt see NB#274753
    for (int i = 8; i <= 64; i++ ) {
        sizeValues.append(i);
    }
}


MRichTextEditDialogsManager::~MRichTextEditDialogsManager()
{
    delete dialogs.textStyles.first;
}


MRichTextEditDialogsManager *MRichTextEditDialogsManager::instance()
{
    if (!dialogsManager) {
        dialogsManager = new MRichTextEditDialogsManager();
    }

    return dialogsManager;
}


void MRichTextEditDialogsManager::initTextStylingDialog()
{
    selectedFontSize = -1;
    selectedFontFamily = QString();
    selectedFontColor = QColor();

    if (dialogs.textStyles.first) {
        return;
    }
    //% "Text styles"
    dialogs.textStyles.first = new MDialog(qtTrId("qtn_comm_text_styles"),M::DoneButton);
    dialogs.textStyles.second = false;

    QGraphicsWidget *centralWidget = dialogs.textStyles.first->centralWidget();
    MLayout *layout = new MLayout(centralWidget);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    // Initialize Font Family combo box
    QFontDatabase fontDatabase;
    QStringList fontFamilyValues = fontDatabase.families();

    // NOTE: black listing some ill-behaving fonts temporarily
    fontFamilyValues.removeAll("Webdings");
    fontFamilyValues.removeAll("Wingdings");

    fontFamilyCombo = new MComboBox(centralWidget);
    //% "Font"
    fontFamilyCombo->setTitle(qtTrId("qtn_comm_font"));
    QStringListModel *fontModel = new QStringListModel(centralWidget);
    fontModel->setStringList(fontFamilyValues);
    fontFamilyCombo->setItemModel(fontModel);
    policy->addItem(fontFamilyCombo);

    connect(fontFamilyCombo, SIGNAL(activated(QString)),
            this, SLOT(rememberFontFamily(QString)));

    // Initialize Font Size combo box
    fontSizeCombo = new MComboBox(centralWidget);
    //% "Font size"
    fontSizeCombo->setTitle(qtTrId("qtn_comm_font_size"));
    QStringListModel *sizeModel = new QStringListModel(centralWidget);
    fontSizeCombo->setItemModel(sizeModel);
    policy->addItem(fontSizeCombo);
    connect(fontSizeCombo, SIGNAL(activated(QString)),
            this, SLOT(rememberFontSize()));

    // Initialize Font Color item
    fontColorCombo = new MColorComboBox(centralWidget);
    //% "Font color"
    fontColorCombo->setTitle(qtTrId("qtn_comm_font_color_combobox"));
    policy->addItem(fontColorCombo);
    connect(fontColorCombo, SIGNAL(colorPicked(QColor)),
            this, SLOT(rememberFontColor(QColor)));

    // Selections are applied at pressing button "Done"
    connect(dialogs.textStyles.first, SIGNAL(accepted()),
            this, SLOT(applySelection()));
}


void MRichTextEditDialogsManager::rememberFontSize()
{
    Q_ASSERT(fontSizeCombo);

    int sizeIndex = fontSizeCombo->currentIndex();

    int size = 0;
    if (sizeIndex >= 0 &&
        sizeIndex < sizeValues.size()) {
        size = sizeValues[sizeIndex];
    } else {
        Q_ASSERT(false);
    }

    selectedFontSize = size;
}

void MRichTextEditDialogsManager::rememberFontColor(const QColor &color)
{
    selectedFontColor = color;
}

void MRichTextEditDialogsManager::rememberFontFamily(const QString &family)
{
    selectedFontFamily = family;
}

void MRichTextEditDialogsManager::applySelection()
{
    if (!selectedFontFamily.isNull()) {
        emit fontFamilySelected(selectedFontFamily);
    }
    if (selectedFontSize != -1) {
        emit fontSizeSelected(selectedFontSize);
    }
    if (selectedFontColor.isValid()) {
        emit fontColorSelected(selectedFontColor);
    }
}

void MRichTextEditDialogsManager::setTextStyleValues(const QString &fontfamily, int fontPointSize,
                                                     const QColor &fontColor)
{

    Q_ASSERT(fontFamilyCombo);

    int familyIndex = -1;
    QStringListModel *fontFamilyModel = dynamic_cast<QStringListModel *>(fontFamilyCombo->itemModel());
    if (fontFamilyModel) {
        QStringList fontFamilies = fontFamilyModel->stringList();
        familyIndex = fontFamilies.indexOf(fontfamily);
    }

    fontFamilyCombo->setCurrentIndex(familyIndex);

    Q_ASSERT(fontSizeCombo);

    QStringList fontSizeValues = generateFontSizeComboValues();
    QStringListModel *fontSizeModel = dynamic_cast<QStringListModel *>(fontSizeCombo->itemModel());
    if (fontSizeModel) {
        fontSizeModel->setStringList(fontSizeValues);
    }

    //% "%L1 points"
    QString fontSize(qtTrId("qtn_comm_font_size_value").arg(fontPointSize));
    int sizeIndex = fontSizeValues.indexOf(fontSize);
    fontSizeCombo->setCurrentIndex(sizeIndex);

    Q_ASSERT(fontColorCombo);

    fontColorCombo->setPickedColor(fontColor);
}


void MRichTextEditDialogsManager::execDialog(ActiveDialog *activeDialog)
{
    if (activeDialog &&
        !activeDialog->second &&
        activeDialog->first) {
        activeDialog->second = true;
        activeDialog->first->exec();
        activeDialog->second = false;
    }
}

void MRichTextEditDialogsManager::showTextStylingDialog(const QString &fontfamily, int fontPointSize,
                                                        const QColor &fontColor)
{
    initTextStylingDialog();
    setTextStyleValues(fontfamily, fontPointSize, fontColor);
    execDialog(&dialogs.textStyles);
}

QStringList MRichTextEditDialogsManager::generateFontSizeComboValues() const
{
    QStringList fontSizeValues;
    foreach (int value,sizeValues) {
        //% "%L1 points"
        fontSizeValues  << qtTrId("qtn_comm_font_size_value").arg(value);
    }
    return fontSizeValues;
}
