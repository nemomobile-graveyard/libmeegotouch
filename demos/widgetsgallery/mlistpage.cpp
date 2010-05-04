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

#include "mlistpage.h"

#include <QDir>
#include <QTime>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QTextCodec>


#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MAbstractCellCreator>
#include <MLocale>
#include <MList>
#include <MLabel>
#include <MImageWidget>
#include <MButton>
#include <MTheme>
#include <MAction>

#include <MComboBox>
#include <MDebug>

#include <MContentItem>
#include <MWidgetAction>


#include "utils.h"

#ifdef HAVE_N900
#include "contactmodel.h"
#else
#include "phonebookmodel.h"
#endif

MListPage::MListPage() :
    model(NULL), list(NULL), currentSortingIndex(0)
{
    gid = TemplatePage::ListsGridsAndMenus;
}

MListPage::~MListPage()
{
    delete model;
    delete imageLoader;
}

QString MListPage::timedemoTitle()
{
    return "List";
}

class MListContentItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
    MListContentItemCreator() : amountOfColumns(1) {

    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
        if (contentItem == NULL) // TODO This is shouldn't happen, list must know what it doing, but with multiple columns it happens sometimes
            return;

        QVariant data = index.data(Qt::DisplayRole);

#ifdef HAVE_N900
        Contact *contact = data.value<Contact*>();
        contentItem->setTitle(contact->getName());
        QStringList numbers = contact->getPhoneNumbers();
        if (numbers.size() > 0) {
            contentItem->setSubtitle(numbers[0]);
        } else {
            QStringList addresses = contact->getEmailAddresses();
            if (addresses.size() > 0) {
                contentItem->setSubtitle(addresses[0]);
            } else {
                contentItem->setSubtitle(QString());
            }
        }

        contentItem->setPixmap(contact->getAvatar());
#else
        PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(data.value<void *>());
        contentItem->setTitle(entry->fullName);
        contentItem->setSubtitle(entry->phoneNumber);
        contentItem->setImage(entry->thumbnail);
#endif

        contentItem->boundingRect();

        updateContentItemMode(index, contentItem);
    }

    void updateContentItemMode(const QModelIndex &index, MContentItem *contentItem) const {
        int flatRow = index.row();
        int row = flatRow / amountOfColumns;
        int column = flatRow % amountOfColumns;
        int totalItems = index.model()->rowCount();

        int columns = amountOfColumns;
        int rows = index.model()->rowCount() / amountOfColumns;
        if(index.model()->rowCount() % amountOfColumns)
            rows += 1;

        bool last = (row == (rows - 1) && flatRow == (totalItems - 1));

        if (columns == 1) {
            if (rows > 1){
                if (row == 0)
                    contentItem->setItemMode(MContentItem::SingleColumnTop);
                else if (row < rows - 1)
                    contentItem->setItemMode(MContentItem::SingleColumnCenter);
                else
                    contentItem->setItemMode(MContentItem::SingleColumnBottom);
            } else {
                contentItem->setItemMode(MContentItem::Single);
            }
        } else if (columns > 1) {
            if (rows > 1) {
                if (row == 0) {
                    if (column == 0)
                        contentItem->setItemMode(MContentItem::TopLeft);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setItemMode(MContentItem::Top);
                    else
                        contentItem->setItemMode(MContentItem::TopRight);
                } else if (row < rows - 1) {
                    if (column == 0)
                        contentItem->setItemMode(MContentItem::Left);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setItemMode(MContentItem::Center);
                    else if (flatRow + columns > totalItems)
                        contentItem->setItemMode(MContentItem::BottomRight);
                    else
                        contentItem->setItemMode(MContentItem::Right);
                }
                else {
                    if (column == 0)
                        contentItem->setItemMode(MContentItem::BottomLeft);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setItemMode(MContentItem::Bottom);
                    else
                        contentItem->setItemMode(MContentItem::BottomRight);
                }
            } else {
                if (column == 0)
                    contentItem->setItemMode(MContentItem::SingleRowLeft);
                else if (column > 0 && column < columns - 1 && !last)
                    contentItem->setItemMode(MContentItem::SingleRowCenter);
                else
                    contentItem->setItemMode(MContentItem::SingleRowRight);
            }
        }
    }

    void setColumns(int columns) {
        Q_ASSERT(columns > 0 && columns < 5);
        amountOfColumns = columns;
    }

private:
    int amountOfColumns;
};

void MListPage::loadPicturesInVisibleItems()
{
    imageLoader->loadPictures(list->firstVisibleItem(), list->lastVisibleItem());
}

void MListPage::setPlainListModel()
{
    cellCreator = new MListContentItemCreator();
    list->setCellCreator(cellCreator);

#ifdef HAVE_N900
    model = new ContactModel();
    list->setItemModel(model);

    imageLoader = new ContactImageLoader();
#else
    model = new PhoneBookModel();

    proxyModel = new PhoneBookSortedModel();
    proxyModel->setSourceModel(model);
    list->setItemModel(proxyModel);

    imageLoader = new PhoneBookImageLoader;
#endif

    // when list is moving we shouldn't do any processing, which may happen
    connect(list, SIGNAL(panningStarted()), imageLoader, SLOT(stopLoadingPictures()));
    // when list is stopped, lets load pictures if needed
    connect(list, SIGNAL(panningStopped()), this, SLOT(loadPicturesInVisibleItems()));
    // trigger initial pictures loading
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));


#ifdef HAVE_N900
    loadPicturesInVisibleItems();
#else
    changeAmountOfItemInList(0);
#endif
}

MComboBox *MListPage::createComboBoxAction(const QString &title, const QStringList &itemsList)
{
    MWidgetAction *widgetAction = new MWidgetAction(centralWidget());
    widgetAction->setLocation(MAction::ApplicationMenuLocation);
    MComboBox *comboBox = new MComboBox;
    comboBox->setTitle(title);
    comboBox->setIconVisible(false);
    comboBox->addItems(itemsList);
    comboBox->setCurrentIndex(0);
    widgetAction->setWidget(comboBox);
    addAction(widgetAction);

    return comboBox;
}

void MListPage::createActions()
{
    MComboBox *combo;

    // Uncomment to see actions for showing scrollTo function
    /*
    MAction * action = new MAction("Top", centralWidget());
    connect(action, SIGNAL(triggered()), this, SLOT(scrollToTop()));
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);

    action = new MAction("Bottom", centralWidget());
    connect(action, SIGNAL(triggered()), this, SLOT(scrollToBottom()));
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    */

#ifndef HAVE_N900
    QStringList amountOfItemsList;
    amountOfItemsList << "50 items" << "100 items" << "200 items" << "1000 items";
    combo = createComboBoxAction("Items in model", amountOfItemsList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAmountOfItemInList(int)));
#endif

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
    amountOfColumnList << "1 column" << "2 columns" << "3 columns" << "4 columns";
    combo = createComboBoxAction("Amount of columns", amountOfColumnList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAmountOfColumns(int)));

    QStringList selectionModes;
    selectionModes << "None" << "Single" << "Multi";
    combo = createComboBoxAction("Selection mode", selectionModes);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSelectionMode(int)));

    QStringList separatorsModes;
    separatorsModes << "Off" << "On";
    combo = createComboBoxAction("Separators", separatorsModes);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSeparatorsMode(int)));
}

void MListPage::scrollToBottom()
{
    list->scrollTo(model->index(model->rowCount() - 1, 0), MList::EnsureVisibleHint);
}

void MListPage::scrollToTop()
{
    list->scrollTo(model->index(0, 0), MList::EnsureVisibleHint);
}

void MListPage::changeSortingOrder(int index)
{
    switch (index) {
    case None:
        break;
    case Ascending:
#ifdef HAVE_N900
        model->sort(0, Qt::AscendingOrder);
#else
        proxyModel->sort(0, Qt::AscendingOrder);
#endif
        break;
    case Descending:
#ifdef HAVE_N900
        model->sort(0, Qt::DescendingOrder);
#else
        proxyModel->sort(0, Qt::DescendingOrder);
#endif
        break;
    }
    currentSortingIndex = index;
    loadPicturesInVisibleItems();
}

#ifndef HAVE_N900
void MListPage::changeAmountOfItemInList(int index)
{
    Q_ASSERT(index >= 0 && index < 4);

    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    int amountOfItems[4] = {50, 100, 200, 1000};
    model->insertRows(0, amountOfItems[index]);
    changeSortingOrder(currentSortingIndex);
}
#endif

void MListPage::changeListMode(int index)
{
    switch (index) {
    case Plain:
        list->setShowGroups(false);
#ifndef HAVE_N900
        proxyModel->setShowGroups(false);
#endif
        break;

    case Grouped:
        list->setShowGroups(true);
#ifndef HAVE_N900
        proxyModel->setShowGroups(true);
#endif
        break;
    }
    changeSortingOrder(currentSortingIndex);
    loadPicturesInVisibleItems();
}

void MListPage::changeAmountOfColumns(int index)
{
    cellCreator->setColumns(index + 1);
    list->setColumns(index + 1);
}

void MListPage::changeSelectionMode(int index)
{
    switch (index) {
    case 0:
        list->setSelectionMode(MList::NoSelection);
        break;

    case 1:
        list->setSelectionMode(MList::SingleSelection);
        break;

    case 2:
        list->setSelectionMode(MList::MultiSelection);
        break;
    }
}

void MListPage::changeSeparatorsMode(int index)
{
    Q_ASSERT(index >= 0 && index <= 1);
    bool enableSeparators = (index == 1);

    if(enableSeparators)
        list->setObjectName("wgListWithSeparators");
    else
        list->setObjectName("wgList");
}

void MListPage::itemClick(const QModelIndex &index)
{
    mDebug("MListPage::itemClick") << "Row was clicked: " << index.row();
}

void MListPage::itemLongTapped(const QModelIndex &index)
{
    mDebug("MListPage::itemLongTapped") << "Row was long tapped: " << index.row();
    longTappedIndex = index;
}

void MListPage::removeListItem()
{
    if(longTappedIndex.isValid()) {
        mDebug("MListPage::removeListItem") << "Row about to be removed: " << longTappedIndex.row();
        model->removeRow(longTappedIndex.row(), longTappedIndex.parent());
    }
}

void MListPage::editListItem()
{
    mDebug("MListPage::editListItem") << "Not implemented yet.";
}

void MListPage::createContent()
{
    MApplicationPage::createContent();
    createActions();

    MTheme::addPixmapDirectory(QDir(CONTACTS_DIR).canonicalPath());
    QGraphicsWidget *panel = centralWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    panel->setLayout(layout);

    list = new MList(panel);
    list->setObjectName("wgList");

    MAction *action = new MAction(qtTrId("xx_listpage_list_remove"), list);
    action->setLocation(MAction::ObjectMenuLocation);
    list->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(removeListItem()));

    action = new MAction(qtTrId("xx_listpage_list_edit"), list);
    action->setLocation(MAction::ObjectMenuLocation);
    list->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(editListItem()));

    setPlainListModel();

    layout->addItem(list);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClick(QModelIndex)));
    connect(list, SIGNAL(itemLongTapped(QModelIndex)), this, SLOT(itemLongTapped(QModelIndex)));
}

void MListPage::retranslateUi()
{
    //% "List"
    setTitle(qtTrId("xx_listpage_title"));
    if (!isContentCreated())
        return;
    // this file has no other calls to qtTrId() except for the title
    // at the moment. If more qtTrId() calls are needed they should
    // be added here.
}
