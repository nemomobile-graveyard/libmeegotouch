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

#include "duilistpage.h"
#include "utils.h"
#include "duiabstractcellcreator.h"
#include "phonebookmodel.h"
#include <QDir>
#include <QTime>

#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QTextCodec>

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#include <DuiLocale>
#include <DuiList>
#include <DuiLabel>
#include <DuiImageWidget>
#include <DuiButton>
#include <DuiTheme>
#include <DuiAction>

#include <DuiComboBox>
#include <DuiDebug>

#include <DuiContentItem>
#include <DuiWidgetAction>

DuiListPage::DuiListPage() :
    model(NULL), list(NULL), currentSortingIndex(0)
{
    setPannableAreaInteractive(true);
    gid = TemplatePage::ListsGridsAndMenus;
}

DuiListPage::~DuiListPage()
{
    delete model;
}

class DuiContentItemCreator : public DuiAbstractCellCreator<DuiContentItem>
{
public:
    DuiContentItemCreator() : amountOfColumns(1) {

    }

    void updateCell(const QModelIndex &index, DuiWidget *cell) const {
        DuiContentItem *contentItem = qobject_cast<DuiContentItem *>(cell);
        if (contentItem == NULL) // TODO This is shouldn't happen, list must know what it doing, but with multiple columns it happens sometimes
            return;

        QVariant data = index.data(Qt::DisplayRole);
        PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(data.value<void *>());
        contentItem->setTitle(entry->fullName);
        contentItem->setSubtitle(entry->phoneNumber);
        contentItem->setPixmap(entry->thumbnail);

        contentItem->boundingRect();

        updateContentItemMode(index, contentItem);
    }

    void updateContentItemMode(const QModelIndex &index, DuiContentItem *contentItem) const {
        int row = index.row();

        bool thereIsNextRow;

        if (amountOfColumns == 1) {
            thereIsNextRow = index.sibling(row + 1, 0).isValid();

            if (row == 0) {
                contentItem->setItemMode(DuiContentItem::SingleColumnTop);
            } else if (thereIsNextRow) {
                contentItem->setItemMode(DuiContentItem::SingleColumnCenter);
            } else {
                contentItem->setItemMode(DuiContentItem::SingleColumnBottom);
            }
        } else {
            bool left = ((row % 2) == 0);
            thereIsNextRow = left ? index.sibling(row + 2, 0).isValid() : index.sibling(row + 1, 0).isValid();
            if (row == 0 || row == 1) {
                left ? contentItem->setItemMode(DuiContentItem::TopLeft) : contentItem->setItemMode(DuiContentItem::TopRight);
            } else if (thereIsNextRow) {
                left ? contentItem->setItemMode(DuiContentItem::Left) : contentItem->setItemMode(DuiContentItem::Right);
            } else {
                left ? contentItem->setItemMode(DuiContentItem::BottomLeft) : contentItem->setItemMode(DuiContentItem::BottomRight);
            }
        }
    }

    void setColumns(int columns) {
        Q_ASSERT(columns > 0 && columns < 3);
        amountOfColumns = columns;
    }

private:
    int amountOfColumns;
};

void DuiListPage::loadPicturesInVisibleItems()
{
    imageLoader->loadPictures(list->firstVisibleItem(), list->lastVisibleItem());
}

void DuiListPage::setPlainListModel()
{
    cellCreator = new DuiContentItemCreator();
    list->setCellCreator(cellCreator);

    model = new PhoneBookModel();

    proxyModel = new PhoneBookSortedModel();
    proxyModel->setSourceModel(model);
    list->setItemModel(proxyModel);

    imageLoader = new PhoneBookImageLoader;

    // when list is moving we shouldn't do any processing, which may happen
    connect(list, SIGNAL(panningStarted()), imageLoader, SLOT(stopLoadingPictures()));
    // when list is stopped, lets load pictures if needed
    connect(list, SIGNAL(panningStopped()), this, SLOT(loadPicturesInVisibleItems()));
    // trigger initial pictures loading
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));

    changeAmountOfItemInList(0);
}

DuiComboBox *DuiListPage::createComboBoxAction(const QString &title, const QStringList &itemsList)
{
    DuiWidgetAction *widgetAction = new DuiWidgetAction(centralWidget());
    widgetAction->setLocation(DuiAction::ApplicationMenuLocation);
    DuiComboBox *comboBox = new DuiComboBox;
    comboBox->setTitle(title);
    comboBox->setIconVisible(false);
    comboBox->addItems(itemsList);
    comboBox->setCurrentIndex(0);
    widgetAction->setWidget(comboBox);
    addAction(widgetAction);

    return comboBox;
}

void DuiListPage::createActions()
{
    DuiComboBox *combo;

    // Uncomment to see actions for showing scrollTo function
    /*
    DuiAction * action = new DuiAction("Top", centralWidget());
    connect(action, SIGNAL(triggered()), this, SLOT(scrollToTop()));
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);

    action = new DuiAction("Bottom", centralWidget());
    connect(action, SIGNAL(triggered()), this, SLOT(scrollToBottom()));
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    */

    QStringList amountOfItemsList;
    amountOfItemsList << "50 items" << "100 items" << "200 items" << "1000 items";
    combo = createComboBoxAction("Items in model", amountOfItemsList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAmountOfItemInList(int)));

    QStringList sortingOrderList;
    sortingOrderList << "None" << "Ascending" << "Descending";
    combo = createComboBoxAction("Sort", sortingOrderList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSortingOrder(int)));

    QStringList listModeList;
    listModeList << "Plain";
    // Not implemented yet
    // listModeList << "Grouped";
    combo = createComboBoxAction("List mode", listModeList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeListMode(int)));

    QStringList amountOfColumnList;
    amountOfColumnList << "1 column" << "2 columns";
    combo = createComboBoxAction("Amount of columns", amountOfColumnList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAmountOfColumns(int)));

    QStringList selectionModes;
    selectionModes << "None" << "Single" << "Multi";
    combo = createComboBoxAction("Selection mode", selectionModes);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSelectionMode(int)));
}

void DuiListPage::scrollToBottom()
{
    list->scrollTo(model->index(model->rowCount() - 1, 0), DuiList::EnsureVisibleHint);
}

void DuiListPage::scrollToTop()
{
    list->scrollTo(model->index(0, 0), DuiList::EnsureVisibleHint);
}

void DuiListPage::changeSortingOrder(int index)
{
    switch (index) {
    case None:
        break;
    case Ascending:
        proxyModel->sort(0, Qt::AscendingOrder);
        break;
    case Descending:
        proxyModel->sort(0, Qt::DescendingOrder);
        break;
    }

    currentSortingIndex = index;
    loadPicturesInVisibleItems();
}

void DuiListPage::changeAmountOfItemInList(int index)
{
    Q_ASSERT(index >= 0 && index < 4);

    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    int amountOfItems[4] = {50, 100, 200, 1000};
    model->insertRows(0, amountOfItems[index]);
    changeSortingOrder(currentSortingIndex);
}

void DuiListPage::changeListMode(int index)
{
    switch (index) {
    case Plain:
        list->setShowGroups(false);
        proxyModel->setShowGroups(false);
        break;

    case Grouped:
        list->setShowGroups(true);
        proxyModel->setShowGroups(true);
        break;
    }
    changeSortingOrder(currentSortingIndex);
    loadPicturesInVisibleItems();
}

void DuiListPage::changeAmountOfColumns(int index)
{
    cellCreator->setColumns(index + 1);
    list->setColumns(index + 1);
}

void DuiListPage::changeSelectionMode(int index)
{
    switch (index) {
    case 0:
        list->setSelectionMode(DuiList::NoSelection);
        break;

    case 1:
        list->setSelectionMode(DuiList::SingleSelection);
        break;

    case 2:
        list->setSelectionMode(DuiList::MultiSelection);
        break;
    }
}

void DuiListPage::itemClick(const QModelIndex &index)
{
    duiDebug("DuiListPage::itemClick") << "Row was clicked: " << index.row();
}

void DuiListPage::createContent()
{
    DuiApplicationPage::createContent();
    createActions();

    DuiTheme::addPixmapDirectory(QDir(CONTACTS_DIR).canonicalPath());
    DuiWidget *panel = centralWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(layout);

    list = new DuiList(panel);
    list->setObjectName("wgList");

    setPlainListModel();

    layout->addItem(list);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClick(QModelIndex)));
}

void DuiListPage::retranslateUi()
{
    //% "List"
    setTitle(qtTrId("xx_listpage_title"));
    if (!isContentCreated())
        return;
    // this file has no other calls to qtTrId() except for the title
    // at the moment. If more qtTrId() calls are needed they should
    // be added here.
}
