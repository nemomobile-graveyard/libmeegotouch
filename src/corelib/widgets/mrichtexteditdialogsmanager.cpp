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

#include <QFontDatabase>
#include <QStringListModel>
#include <QDebug>
#include <MComboBox>
#include <MDialog>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include "mrichtexteditdialogsmanager_p.h"

MRichTextEditDialogsManager *MRichTextEditDialogsManager::dialogsManager = 0;


MRichTextEditDialogsManager::MRichTextEditDialogsManager()
    : fontFamilyCombo(0)
{
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
    if (dialogs.textStyles.first) {
        return;
    }

    dialogs.textStyles.first = new MDialog();
    //% "Text styles"
    dialogs.textStyles.first->setTitle(qtTrId("qtn_comm_text_styles"));
    dialogs.textStyles.second = false;

    QGraphicsWidget *centralWidget = dialogs.textStyles.first->centralWidget();
    MLayout *layout = new MLayout(centralWidget);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    QFontDatabase fontDatabase;
    QStringList fontFamilyValues = fontDatabase.families();
    fontFamilyCombo = new MComboBox(centralWidget);
    //% "Font"
    fontFamilyCombo->setTitle(qtTrId("qtn_comm_font"));
    QStringListModel *fontModel = new QStringListModel(centralWidget);
    fontModel->setStringList(fontFamilyValues);
    fontFamilyCombo->setItemModel(fontModel);
    policy->addItem(fontFamilyCombo);

    connect(fontFamilyCombo, SIGNAL(activated(QString)),
            this, SIGNAL(fontFamilySelected(QString)));
}


void MRichTextEditDialogsManager::setTextStyleValues(const QString &fontfamily)
{
    Q_ASSERT(fontFamilyCombo);

    int familyIndex = -1;
    QStringListModel *fontModel = dynamic_cast<QStringListModel *>(fontFamilyCombo->itemModel());
    if (fontModel) {
        QStringList fontFamilies = fontModel->stringList();
        familyIndex = fontFamilies.indexOf(fontfamily);
    }

    fontFamilyCombo->setCurrentIndex(familyIndex);
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

void MRichTextEditDialogsManager::showTextStylingDialog(const QString &fontfamily)
{
    initTextStylingDialog();
    setTextStyleValues(fontfamily);
    execDialog(&dialogs.textStyles);
}

