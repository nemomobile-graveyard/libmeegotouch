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

#include "comboboxpage.h"

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MLocale>
#include <MLabel>
#include <MComboBox>
#include <MPopupList>
#include <MButton>

#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QTimer>

ComboBoxPage::ComboBoxPage()
    : TemplatePage(TemplatePage::ListsGridsAndPopups),
    model1(0),
      proxyModel(0)
{
}

ComboBoxPage::~ComboBoxPage()
{
}

QString ComboBoxPage::timedemoTitle()
{
    return "ComboBox";
}

void ComboBoxPage::createContent()
{
    MApplicationPage::createContent();

    createLayout();
    containerLayout = new MLayout(container);

    containerPolicy = new MLinearLayoutPolicy(containerLayout, Qt::Vertical);
    containerLayout->setPolicy(containerPolicy);

    QStringList list;
    for (int i = 0; i < 100; ++i) {
        list << QString::number(10000 + i);
    }

    comboBox1 = new MComboBox;
    comboBox1->setIconID("Icon-pictures");
    comboBox1->addItems(list);
    comboBox1->setCurrentIndex(50);

    comboBox2 = new MComboBox;
    comboBox2->setIconID("Icon-pictures");

    model1 = new QStringListModel(this);
    model1->setStringList(list);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model1);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->sort(0, Qt::DescendingOrder);

    comboBox2->setItemModel(proxyModel);

    comboBox3 = new MComboBox;
    comboBox3->setIconID("Icon-music");
    comboBox3->setTitle("Time-consuming setting");
    comboBox3->addItem("Setting 1");
    comboBox3->addItem("Setting 2");
    comboBox3->addItem("Setting 3");
    comboBox3->addItem("Setting 4");
    comboBox3->addItem("Setting 5");

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(3000);
    QObject::connect(comboBox3, SIGNAL(currentIndexChanged(int)), comboBox3, SLOT(showProgressIndicator()));
    QObject::connect(comboBox3, SIGNAL(currentIndexChanged(int)), timer, SLOT(start()));
    QObject::connect(timer, SIGNAL(timeout()), comboBox3, SLOT(hideProgressIndicator()));

    containerPolicy->addItem(comboBox1);
    containerPolicy->addItem(comboBox2);
    containerPolicy->addItem(comboBox3);

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
    //% "Time-consuming setting"
    comboBox3->setTitle(qtTrId("xx_popup_time_consuming"));

    //% "The MComboBox widget is a combined button and popup list. "
    //% "It is very similar to QComboBox, but does not allow editing the text."
    infoLabel->setText("<a></a>" + qtTrId("xx_combobox_page_info"));
}

