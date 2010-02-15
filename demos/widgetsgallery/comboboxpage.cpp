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

#include "comboboxpage.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiComboBox>
#include <DuiPopupList>
#include <DuiButton>

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QStringList>

ComboBoxPage::ComboBoxPage()
    : model1(0),
      proxyModel(0)
{
    gid = TemplatePage::ListsGridsAndMenus;

}

ComboBoxPage::~ComboBoxPage()
{
}

void ComboBoxPage::createContent()
{
    DuiApplicationPage::createContent();

    createLayout();
    containerLayout = new DuiLayout(container);

    containerPolicy = new DuiLinearLayoutPolicy(containerLayout, Qt::Vertical);
    containerLayout->setPolicy(containerPolicy);

    QStringList list;
    for (int i = 0; i < 10000; ++i) {
        list << QString::number(10000 + i);
    }

    comboBox1 = new DuiComboBox;
    comboBox1->setIconID("Icon-pictures");
    comboBox1->addItems(list);

    comboBox2 = new DuiComboBox;
    comboBox2->setIconID("Icon-pictures");

    model1 = new QStringListModel(this);
    model1->setStringList(list);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model1);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->sort(0, Qt::DescendingOrder);

    comboBox2->setItemModel(proxyModel);

    containerPolicy->addItem(comboBox1);
    containerPolicy->addItem(comboBox2);

    retranslateUi();
}

void ComboBoxPage::retranslateUi()
{
    //% "ComboBox"
    setTitle(qtTrId("xx_combobox_title"));
    if (!isContentCreated())
        return;
    //% "Title"
    comboBox1->setTitle(qtTrId("xx_popup_generic_title"));
    //% "Sort - DescendingOrder"
    comboBox2->setTitle(qtTrId("xx_popup_sort_descending_order"));

    //% "The DuiComboBox widget is a combined button and popup list. "
    //% "It is very similar to QComboBox, but does not allow editing the text."
    infoLabel->setText("<a></a>" + qtTrId("xx_combobox_page_info"));
}

