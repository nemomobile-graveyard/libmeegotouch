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
#include <MSortFilterProxyModel>

#include <MComboBox>
#include <MDebug>

#include <MWidgetAction>

#include "phonebookcell.h"

#include <MListFilter>
#include <MTextEdit>
#include <MPannableViewport>

#include "utils.h"

#ifdef HAVE_N900
#include "contactmodel.h"
#else
#include "phonebookmodel.h"
#endif //HAVE_N900

MListPage::MListPage()
  : TemplatePage(TemplatePage::ListsGridsAndPopups),
    model(NULL),
#ifndef HAVE_N900
    proxyModel(NULL),
#endif //HAVE_N900
    imageLoader(NULL),
    list(NULL),
    currentSortingIndex(0),
    currentListModeIndex(0)
{
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

class MListContentItemCreator : public MAbstractCellCreator<PhoneBookCell>
{
public:
    MListContentItemCreator() : amountOfColumns(1), highlightText("") {
    }
    
    MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
    {
        // FIXME: It's a workaround against a bug, that if the layout is created and 
        // set in constructor then the pixmaps are properly loaded.
        PhoneBookCell *cell = dynamic_cast<PhoneBookCell *>(recycler.take(PhoneBookCell::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new PhoneBookCell;
            cell->initLayout();
        }
        updateCell(index, cell);
        return cell;
    }
    
    void updateCell(const QModelIndex &index, MWidget *cell) const {
        PhoneBookCell *listCell = qobject_cast<PhoneBookCell*>(cell);
        if (listCell == NULL) // TODO This is shouldn't happen, list must know what it doing, but with multiple columns it happens sometimes
            return;
        
        QVariant data = index.data(Qt::DisplayRole);
        
#ifdef HAVE_N900
        Contact *contact = data.value<Contact*>();
        listCell->setTitle(contact->getName());
        QStringList numbers = contact->getPhoneNumbers();
        if (numbers.size() > 0) {
            listCell->setSubtitle(numbers[0]);
        } else {
            QStringList addresses = contact->getEmailAddresses();
            if (addresses.size() > 0) {
                listCell->setSubtitle(addresses[0]);
            } else {
                listCell->setSubtitle(QString());
            }
        }

	listCell->setImage(contact->getAvatar().toImage());

#else
        PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(data.value<void *>());

        if(highlightText == "")
            listCell->setTitle(entry->fullName);
        else {
            QString highlightedTitle = entry->fullName;
            highlightedTitle.replace(highlightText, "<b>" + highlightText + "</b>");
            listCell->setTitle(highlightedTitle);
        }

        listCell->setSubtitle(entry->phoneNumber);
        listCell->setImage(entry->thumbnail);
#endif //HAVE_N900

        updateContentItemMode(index, listCell);
    }
       
    void updateContentItemMode(const QModelIndex &index, MListItem *contentItem) const {
        int flatRow = index.row();
        int row = flatRow / amountOfColumns;
        int column = flatRow % amountOfColumns;
        int totalItems = index.model()->rowCount(index.parent());

        int columns = amountOfColumns;
        int rows = totalItems / amountOfColumns;
        if (totalItems % amountOfColumns)
            rows += 1;

        bool last = (row == (rows - 1) && flatRow == (totalItems - 1));

        if (columns == 1) {
            if (rows > 1){
                if (row == 0)
                    contentItem->setLayoutPosition(M::VerticalTopPosition);
                else if (row < rows - 1)
                    contentItem->setLayoutPosition(M::VerticalCenterPosition);
                else
                    contentItem->setLayoutPosition(M::VerticalBottomPosition);
            } else {
                contentItem->setLayoutPosition(M::DefaultPosition);
            }
        } else if (columns > 1) {
            if (rows > 1) {
                if (row == 0) {
                    if (column == 0)
                        contentItem->setLayoutPosition(M::TopLeftPosition);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setLayoutPosition(M::TopCenterPosition);
                    else
                        contentItem->setLayoutPosition(M::TopRightPosition);
                } else if (row < rows - 1) {
                    if (column == 0)
                        contentItem->setLayoutPosition(M::CenterLeftPosition);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setLayoutPosition(M::CenterPosition);
                    else if (flatRow + columns > totalItems)
                        contentItem->setLayoutPosition(M::BottomRightPosition);
                    else
                        contentItem->setLayoutPosition(M::CenterRightPosition);
                }
                else {
                    if (column == 0)
                        contentItem->setLayoutPosition(M::BottomLeftPosition);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setLayoutPosition(M::BottomCenterPosition);
                    else
                        contentItem->setLayoutPosition(M::BottomRightPosition);
                }
            } else {
                if (column == 0)
                    contentItem->setLayoutPosition(M::HorizontalLeftPosition);
                else if (column > 0 && column < columns - 1 && !last)
                    contentItem->setLayoutPosition(M::HorizontalCenterPosition);
                else
                    contentItem->setLayoutPosition(M::HorizontalRightPosition);
            }
        }
    }

    void setColumns(int columns) {
        Q_ASSERT(columns > 0 && columns < 5);
        amountOfColumns = columns;
    }

    void highlightByText(QString text) {
        highlightText = text;
    }

private:
    int amountOfColumns;
    QString highlightText;
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
    
    proxyModel = new MSortFilterProxyModel();
    proxyModel->setSortRole(PhoneBookModel::PhoneBookSortRole);
    proxyModel->setFilterRole(PhoneBookModel::PhoneBookFilterRole);
    proxyModel->setSourceModel(model);
    
    
    list->setItemModel(proxyModel);

    imageLoader = new PhoneBookImageLoader;
#endif //HAVE_N900

    // when list is moving we shouldn't do any processing, which may happen
    connect(list, SIGNAL(panningStarted()), imageLoader, SLOT(stopLoadingPictures()));
    // when list is stopped, lets load pictures if needed
    connect(list, SIGNAL(panningStopped()), this, SLOT(loadPicturesInVisibleItems()));
    // trigger initial pictures loading
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));


#ifndef HAVE_N900
    changeAmountOfItemInList(0);
#endif //HAVE_N900
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
#endif //HAVE_N900

    QStringList sortingOrderList;
    sortingOrderList << "None" << "Ascending" << "Descending";
    combo = createComboBoxAction("Sort", sortingOrderList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSortingOrder(int)));

    QStringList listModeList;
    listModeList << "Plain" << "Grouped";
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

    QStringList listIndexModes;
    listIndexModes << "Hidden" << "Visible";
    combo = createComboBoxAction("List index mode", listIndexModes);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeListIndexVisibility(int)));

    QStringList liveFilteringModes;
    liveFilteringModes << "Off" << "On";
    combo = createComboBoxAction("Live Filtering", liveFilteringModes);
    combo->setCurrentIndex(1); // live filtering is enabled by default
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLiveFilteringMode(int)));
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
#ifndef HAVE_N900
        proxyModel->sort(0, Qt::AscendingOrder);
#else
         model->sort(0, Qt::AscendingOrder);
#endif //HAVE_N900
        break;
    case Descending:
#ifndef HAVE_N900
        proxyModel->sort(0, Qt::DescendingOrder);
#else
        model->sort(0, Qt::DescendingOrder);
#endif //HAVE_N900;
        break;
    }
    currentSortingIndex = index;
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));
}

#ifndef HAVE_N900
void MListPage::changeAmountOfItemInList(int index)
{
    Q_ASSERT(index >= 0 && index < 4);

    if(currentListModeIndex == Grouped) {
        list->setShowGroups(false);
    }


    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    int amountOfItems[4] = {50, 100, 200, 1000};
    model->insertRows(0, amountOfItems[index]);
    changeSortingOrder(currentSortingIndex);
    changeListMode(currentListModeIndex);
}
#endif //HAVE_N900

void MListPage::changeListMode(int index)
{
    switch (index) {
    case Plain:
        list->setShowGroups(false);
#ifndef HAVE_N900
        model->setGrouped(false);
#endif //HAVE_N900
        break;

    case Grouped:
        list->setShowGroups(true);
#ifndef HAVE_N900
        model->setGrouped(true);        
#endif //HAVE_N900
        break;
    }

    currentListModeIndex = index;

    changeSortingOrder(currentSortingIndex);
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));
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

    if (enableSeparators)
        list->setObjectName("wgListWithSeparators");
    else
        list->setObjectName("wgList");
}

void MListPage::changeListIndexVisibility(int index)
{
    Q_ASSERT(index >= 0 && index <= 1);
    bool indexVisible = (index == 1);

    list->setIndexVisible(indexVisible);
}

void MListPage::changeLiveFilteringMode(int index)
{
    Q_ASSERT(index >= 0 && index <= 1);
    bool enableLF = (index == 1);

    if(enableLF) {
        list->filtering()->setEnabled(true);
#ifndef HAVE_N900
        list->filtering()->setFilterRole(PhoneBookModel::PhoneBookFilterRole);
#endif //HAVE_N900
        list->filtering()->editor()->setVisible(false);
        connect(list->filtering(), SIGNAL(listPannedUpFromTop()), this, SLOT(filteringVKB())); 
        connect(list->filtering()->editor(), SIGNAL(textChanged()), this, SLOT(liveFilteringTextChanged())); 
    } else {
        disconnect(list->filtering(), SIGNAL(listPannedUpFromTop()), this, SLOT(filteringVKB())); 
        disconnect(list->filtering()->editor(), SIGNAL(textChanged()), this, SLOT(liveFilteringTextChanged())); 
        list->filtering()->setEnabled(false);
        showTextEdit(false);
    }
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
#ifndef HAVE_N900
        proxyModel->removeRow(longTappedIndex.row(), longTappedIndex.parent());
#endif //HAVE_N900
    }
}

void MListPage::editListItem()
{
    mDebug("MListPage::editListItem") << "Not implemented yet.";
}

void MListPage::liveFilteringTextChanged()
{
    // With HWKB live filtering text edit is shown when user enters text
    if(list->filtering()->enabled() && list->filtering()->editor()->text() != "" && !list->filtering()->editor()->isOnDisplay())
        showTextEdit(true);

    // Highlighting matching live filtering text can be done by
    // passing the text to cell creator and updating visible items
    cellCreator->highlightByText(list->filtering()->editor()->text());
#ifndef HAVE_N900
    static_cast<PhoneBookModel*>(model)->updateData(list->firstVisibleItem(), list->lastVisibleItem());
#endif //HAVE_N900
}

void MListPage::filteringVKB()
{
    // With VKB live filtering text edit is shown when user pans the list up starting from top position
    if(!list->filtering()->editor()->isOnDisplay()) {
        showTextEdit(true);
        list->filtering()->editor()->setFocus();
    }
}

void MListPage::showTextEdit(bool show) {
    QGraphicsWidget* panel = centralWidget();
    QGraphicsLinearLayout* layout = (QGraphicsLinearLayout*) panel->layout();
    MTextEdit* textEdit = list->filtering()->editor();
    if(show && !textEdit->isOnDisplay()) {
        layout->insertItem(0, textEdit);
        textEdit->setVisible(true);
        pannableViewport()->setPosition(QPointF(0,0));
    } else if(textEdit->isOnDisplay()) {
        textEdit->setVisible(false);
        layout->removeAt(0);
        textEdit->setText("");
    }
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

    //% "Remove"
    MAction *action = new MAction(qtTrId("xx_listpage_list_remove"), list);
    action->setLocation(MAction::ObjectMenuLocation);
    list->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(removeListItem()));

    //% "Edit"
    action = new MAction(qtTrId("xx_listpage_list_edit"), list);
    action->setLocation(MAction::ObjectMenuLocation);
    list->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(editListItem()));

    setPlainListModel();

    layout->addItem(list);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClick(QModelIndex)));
    connect(list, SIGNAL(itemLongTapped(QModelIndex)), this, SLOT(itemLongTapped(QModelIndex)));

    changeLiveFilteringMode(1); // live filtering is enabled by default
    
    retranslateUi();
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
