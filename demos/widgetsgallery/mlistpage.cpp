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
#include <MDialog>
#include <MSceneManager>
#include <MObjectMenu>

#include <MComboBox>
#include <MDebug>

#include <MWidgetAction>

#include "phonebookcell.h"
#include "phonebookheader.h"

#include <MListFilter>
#include <MTextEdit>
#include <MPannableViewport>

#include "utils.h"

#include "phonebookmodel.h"

MListPage::MListPage()
  : TemplatePage(TemplatePage::ListsGridsAndPopups),
    model(NULL),
    proxyModel(NULL),
    imageLoader(NULL),
    comboListMode(NULL),
    objectMenu(NULL),
    actionAdvancedConfiguration(NULL),
    dialogAdvancedConfiguration(NULL),
    list(NULL),
    cellCreator(NULL),
    longTappedIndex(),
    currentSortingIndex(0),
    currentListModeIndex(0)
{
    liveFilteringTextChangeTimer.setInterval(500);
    liveFilteringTextChangeTimer.setSingleShot(true);
    liveFilteringTextChangeTimer.stop();

    connect(&liveFilteringTextChangeTimer, SIGNAL(timeout()), this, SLOT(liveFilteringTextChangeTimeout()));
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

class MListAdvancedCustomHeaderCreator : public MAbstractCellCreator<PhoneBookHeader>
{
public:
    void updateCell(const QModelIndex &index, MWidget *cell) const {
        PhoneBookHeader *header = qobject_cast<PhoneBookHeader*>(cell);
        QString title = index.data(Qt::DisplayRole).toString();

        header->setGroupTitle(title);
        header->setGroupCount(index.model()->rowCount(index));
    }

    QSizeF cellSize() const {
        return QSizeF(-1, 100);
    }
};

class MListCustomHeaderCreator : public MAbstractCellCreator<MButton>
{
public:
    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MButton *header = qobject_cast<MButton*>(cell);
        QString title = index.data(Qt::DisplayRole).toString();

        header->setText(title);
    }
};

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
        }
        updateCell(index, cell);
        return cell;
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        PhoneBookCell *listCell = qobject_cast<PhoneBookCell*>(cell);
        if (listCell == NULL) // TODO This is shouldn't happen, list must know what it doing, but with multiple columns it happens sometimes
            return;

        QVariant data = index.data(Qt::DisplayRole);
        PhoneBookEntry *entry = static_cast<PhoneBookEntry *>(data.value<void *>());

        if(highlightText.isEmpty()) {
            listCell->setTitle(entry->fullName);
        } else {
            QString highlightedTitle = entry->fullName;
            int matchingIndex = highlightedTitle.indexOf(highlightText, 0, Qt::CaseInsensitive);
            if(matchingIndex != -1) {
                highlightedTitle.insert(matchingIndex+highlightText.length(), "</b>");
                highlightedTitle.insert(matchingIndex, "<b>");
            }
            listCell->setTitle(highlightedTitle);
        }

        listCell->setSubtitle(entry->phoneNumber);
        listCell->setImage(entry->thumbnail);

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
        }
        else if (columns > 1) {
            if (rows > 1) {
                if (row == 0) {
                    if (column == 0)
                        contentItem->setLayoutPosition(M::TopLeftPosition);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setLayoutPosition(M::TopCenterPosition);
                    else {
                        if (rows == 2 && (totalItems % columns))	//widget on row 1/2
                            contentItem->setLayoutPosition(M::HorizontalRightPosition);
                        else
                            contentItem->setLayoutPosition(M::TopRightPosition);
                    }
                }	//end first row
                else if (row < rows - 1) {
                    if (column == 0)
                        contentItem->setLayoutPosition(M::CenterLeftPosition);
                    else if (column > 0 && column < columns - 1 && !last)
                        contentItem->setLayoutPosition(M::CenterPosition);
                    else if (flatRow + columns > totalItems)
                        contentItem->setLayoutPosition(M::BottomRightPosition);
                    else {
                        //correct decoration for last cell on next-to-last row
                        if (row == rows - 2 && (totalItems % columns))
                            contentItem->setLayoutPosition(M::BottomRightPosition);
                        else
                            contentItem->setLayoutPosition(M::CenterRightPosition);
                    }
                }	//end midsection
                else {
                    if (column == 0) {
                        if (last)	//special case where the cell is the last cell
                            contentItem->setLayoutPosition(M::VerticalBottomPosition);
                        else
                            contentItem->setLayoutPosition(M::BottomLeftPosition);
                    } //end last row, first cell
                    else {
                        if (column > 0 && column < columns - 1 && !last)
                            contentItem->setLayoutPosition(M::BottomCenterPosition);
                        else
                            contentItem->setLayoutPosition(M::BottomRightPosition);
                    }
                }	//end last row
            }	//end multiple rows
            else {
                if (column == 0) {
                    if (last)	//special case where the cell is the only cell
                        contentItem->setLayoutPosition(M::DefaultPosition);
                    else
                        contentItem->setLayoutPosition(M::HorizontalLeftPosition);
                }
                else {
                    if (column > 0 && column < columns - 1 && !last)
                        contentItem->setLayoutPosition(M::HorizontalCenterPosition);
                    else
                        contentItem->setLayoutPosition(M::HorizontalRightPosition);
                }
            }   //end single row
        }	//end multiple columns
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

    model = new PhoneBookModel();

    proxyModel = new MSortFilterProxyModel();
    proxyModel->setSortRole(PhoneBookModel::PhoneBookSortRole);
    proxyModel->setFilterRole(PhoneBookModel::PhoneBookFilterRole);
    proxyModel->setSourceModel(model);

    list->setItemModel(proxyModel);

    imageLoader = new PhoneBookImageLoader;

    // when list is moving we shouldn't do any processing, which may happen
    connect(list, SIGNAL(panningStarted()), imageLoader, SLOT(stopLoadingPictures()));
    // when list is stopped, lets load pictures if needed
    connect(list, SIGNAL(panningStopped()), this, SLOT(loadPicturesInVisibleItems()));
    // when list is moving hide empty live filtering editor
    connect(list, SIGNAL(panningStarted()), this, SLOT(hideEmptyTextEdit()));
    // trigger initial pictures loading
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));


    changeAmountOfItemInList(1);
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

MComboBox *MListPage::createComboBoxLabelButton(const QString &title, const QStringList &itemsList, QGraphicsWidget *parent)
{
    MComboBox *comboBox = new MComboBox(parent);
    comboBox->setStyleName("CommonComboBoxInverted");
    comboBox->setTitle(title);
    comboBox->setIconVisible(false);
    comboBox->addItems(itemsList);
    comboBox->setCurrentIndex(0);

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

    QStringList amountOfItemsList;
    amountOfItemsList << "50 items" << "100 items" << "200 items" << "1000 items";
    combo = createComboBoxAction("Items in model", amountOfItemsList);
    combo->setCurrentIndex(1); // by default create 100 items
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAmountOfItemInList(int)));

    QStringList sortingOrderList;
    sortingOrderList << "None" << "Ascending" << "Descending";
    combo = createComboBoxAction("Sort", sortingOrderList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSortingOrder(int)));

    QStringList listModeList;
    listModeList << "Plain" << "Grouped";
    comboListMode = createComboBoxAction("List mode", listModeList);
    connect(comboListMode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeListMode(int)));

    QStringList amountOfColumnList;
    amountOfColumnList << "1 column" << "2 columns" << "3 columns" << "4 columns";
    combo = createComboBoxAction("Amount of columns", amountOfColumnList);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAmountOfColumns(int)));

    QStringList selectionModes;
    selectionModes << "None" << "Single" << "Multi";
    combo = createComboBoxAction("Selection mode", selectionModes);
    connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSelectionMode(int)));

    actionAdvancedConfiguration = new MAction(centralWidget());
    actionAdvancedConfiguration->setLocation(MAction::ApplicationMenuLocation);
    actionAdvancedConfiguration->setText("Advanced Configuration");
    addAction(actionAdvancedConfiguration);
    connect(actionAdvancedConfiguration, SIGNAL(triggered()), SLOT(showAdvancedConfigurationDialog()));
}

void MListPage::createObjectMenuActions()
{
    objectMenu = new MObjectMenu(NULL);

    //% "Remove"
    MAction *action = new MAction(qtTrId("xx_listpage_list_remove"), this);
    objectMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(removeListItem()));

    //% "Edit"
    action = new MAction(qtTrId("xx_listpage_list_edit"), this);
    objectMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(editListItem()));
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
        proxyModel->sort(0, Qt::AscendingOrder);
        break;
    case Descending:
        proxyModel->sort(0, Qt::DescendingOrder);
        break;
    }
    currentSortingIndex = index;
    QTimer::singleShot(1500, this, SLOT(loadPicturesInVisibleItems()));
}

void MListPage::changeAmountOfItemInList(int index)
{
    Q_ASSERT(index >= 0 && index < 4);

    if(currentListModeIndex == Grouped) {
        list->setShowGroups(false);
    }

    if (model->rowCount() > 0)
        model->removeRows(0, model->rowCount());

    int amountOfItems[] = {50, 100, 200, 1000};
    model->clear();
    model->insertRows(0, amountOfItems[index]);
    changeSortingOrder(currentSortingIndex);
    changeListMode(currentListModeIndex);
}

void MListPage::changeListMode(int index)
{
    switch (index) {
    case Plain:
        list->setShowGroups(false);
        model->setGrouped(false);
        break;

    case Grouped:
        list->setShowGroups(true);
        model->setGrouped(true);
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
        list->setObjectName("listWithSeparators");
    else
        list->setObjectName("");
}

void MListPage::changeListIndexVisibility(int index)
{
    Q_ASSERT(index >= 0 && index <= 3);

    switch (index) {
    case 1:
        list->setIndexDisplayMode(MList::Show);
        break;
    case 2:
        list->setIndexDisplayMode(MList::Auto);
        break;
    case 3:
        list->setIndexDisplayMode(MList::Floating);
        break;
    default:
        list->setIndexDisplayMode(MList::Hide);
        break;
    }

    if (index > 0)
        comboListMode->setCurrentIndex(Grouped);

}

void MListPage::changeLiveFilteringMode(int index)
{
    Q_ASSERT(index >= 0 && index <= 1);
    bool enableLF = (index == 1);

    if(enableLF) {
        list->filtering()->setEnabled(true);
        list->filtering()->setFilterRole(PhoneBookModel::PhoneBookFilterRole);
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

void MListPage::changeGroupHeadersMode(int index)
{
    Q_ASSERT(index >=0 && index <= 2);

    switch (index) {
    case 1:
        list->setHeaderCreator(new MListCustomHeaderCreator);
        break;
    case 2:
        list->setHeaderCreator(new MListAdvancedCustomHeaderCreator);
        break;
    default:
        list->setHeaderCreator(NULL);
        break;
    };

    if (index > 0)
        comboListMode->setCurrentIndex(Grouped);
}

void MListPage::itemClick(const QModelIndex &index)
{
    mDebug("MListPage::itemClick") << "Row was clicked: " << index.row();
}

void MListPage::itemLongTapped(const QModelIndex &index, const QPointF &position)
{
    objectMenu->setCursorPosition(position);

    objectMenu->setTitle(index.data(PhoneBookModel::PhoneBookFilterRole).toString());

    if (index.parent().isValid()) {
        mDebug("MListPage::itemLongTapped") << "Row: " << index.row() << " Group: " << index.parent().row();
        sceneManager()->appearSceneWindow(objectMenu);
    }
    else if (model->hasChildren(index))
        mDebug("MListPage::itemLongTapped") << "Group: " << index.row();
    else {
        mDebug("MListPage::itemLongTapped") << "Row: " << index.row();
        sceneManager()->appearSceneWindow(objectMenu);
    }
    longTappedIndex = index;
}

void MListPage::removeListItem()
{
    if(longTappedIndex.isValid()) {
        mDebug("MListPage::removeListItem") << "Row about to be removed: " << longTappedIndex.row();
        list->itemModel()->removeRow(longTappedIndex.row(), longTappedIndex.parent());
        longTappedIndex = QModelIndex();
    }
}

void MListPage::editListItem()
{
    mDebug("MListPage::editListItem") << "Not implemented yet.";
    longTappedIndex = QModelIndex();
}

void MListPage::liveFilteringTextChanged()
{
    if(!list->filtering()->enabled())
        return;

    // With HWKB live filtering text edit is hidden when empty and shown when user enters text
    if(list->filtering()->editor()->text().isEmpty() && list->filtering()->editor()->isOnDisplay())
        QTimer::singleShot(1500, this, SLOT(hideEmptyTextEdit()));
    else if( !list->filtering()->editor()->text().isEmpty() && !list->filtering()->editor()->isOnDisplay())
        showTextEdit(true);

    // Notice that list view has not been updated yet to match the new filtering.
    // First and last visible item indexes are not up-to-date. This is why list page
    // starts a timer for loading pictures
    imageLoader->stopLoadingPictures();
    resetLiveFilteringTextChangeTimer();

    // Highlighting matching live filtering text can be done by
    // passing the text to cell creator
    cellCreator->highlightByText(list->filtering()->editor()->text());
}

void MListPage::resetLiveFilteringTextChangeTimer()
{
    if(liveFilteringTextChangeTimer.isActive()) {
        liveFilteringTextChangeTimer.stop();
    }
    liveFilteringTextChangeTimer.start();
}

void MListPage::liveFilteringTextChangeTimeout()
{
    static_cast<PhoneBookModel*>(model)->updateData(
        list->filtering()->proxy()->mapToSource(list->firstVisibleItem()),
        list->filtering()->proxy()->mapToSource(list->lastVisibleItem()) );

    loadPicturesInVisibleItems();
}

void MListPage::filteringVKB()
{
    // With VKB live filtering text edit is shown when user pans the list up starting from top position
    if (!list->filtering()->editor()->isOnDisplay()) {
        showTextEdit(true);
        list->filtering()->editor()->setFocus();
    }
}

void MListPage::hideEmptyTextEdit()
{
    if (list->filtering()->enabled() && list->filtering()->editor()->text().isEmpty()) {
        showTextEdit(false);
    }
}

void MListPage::showAdvancedConfigurationDialog()
{
    if (!dialogAdvancedConfiguration) {
        dialogAdvancedConfiguration = new MDialog("Advanced configuration", M::NoStandardButton);

        QGraphicsWidget *panel = dialogAdvancedConfiguration->centralWidget();

        MLayout *layout = new MLayout(panel);
        MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
        landscapePolicy->setContentsMargins(0, 0, 0, 0);
        landscapePolicy->setSpacing(0);
        // Use the same landscape policy for portrait mode.
        MLinearLayoutPolicy *portraitPolicy = landscapePolicy;

        QStringList separatorsModes;
        separatorsModes << "Off" << "On";
        MComboBox *combo = createComboBoxLabelButton("Separators", separatorsModes, panel);
        connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSeparatorsMode(int)));

        landscapePolicy->addItem(combo);

        QStringList listIndexModes;
        listIndexModes << "Off" << "On" << "Auto" << "Floating";
        combo = createComboBoxLabelButton("List Index", listIndexModes, panel);
        connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeListIndexVisibility(int)));

        landscapePolicy->addItem(combo);

        QStringList liveFilteringModes;
        liveFilteringModes << "Off" << "On";
        combo = createComboBoxLabelButton("Live Filtering", liveFilteringModes, panel);
        connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLiveFilteringMode(int)));

        landscapePolicy->addItem(combo);

        QStringList groupHeadersModes;
        groupHeadersModes << "Default - Labels" << "Custom - Buttons" << "Advanced - Custom Widget";
        combo = createComboBoxLabelButton("Group Headers", groupHeadersModes, panel);
        connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeGroupHeadersMode(int)));

        landscapePolicy->addItem(combo);

        layout->setLandscapePolicy(landscapePolicy);
        layout->setPortraitPolicy(portraitPolicy);

        dialogAdvancedConfiguration->centralWidget()->setLayout(layout);
    }

    dialogAdvancedConfiguration->appear(MSceneWindow::DestroyWhenDismissed);
}

void MListPage::showTextEdit(bool show)
{
    QGraphicsWidget* panel = centralWidget();
    QGraphicsLinearLayout* layout = (QGraphicsLinearLayout*) panel->layout();
    MTextEdit* textEdit = list->filtering()->editor();
    if (show && !textEdit->isOnDisplay()) {
        layout->insertItem(0, textEdit);
        textEdit->setVisible(true);
        pannableViewport()->setPosition(QPointF(0,0));
    } else if (textEdit->isOnDisplay()) {
        list->setFocus();
        textEdit->setVisible(false);
        layout->removeAt(0);
        textEdit->setText("");
    }
}

void MListPage::createContent()
{
    MApplicationPage::createContent();
    createActions();
    createObjectMenuActions();

    MTheme::addPixmapDirectory(QDir(CONTACTS_DIR).canonicalPath());
    QGraphicsWidget *panel = centralWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    list = new MList(panel);

    setPlainListModel();
    changeListMode(Grouped);
    comboListMode->setCurrentIndex(Grouped);

    layout->addItem(list);

    connect(list, SIGNAL(itemClicked(QModelIndex)), this, SLOT(itemClick(QModelIndex)));
    connect(list, SIGNAL(itemLongTapped(QModelIndex,QPointF)), this, SLOT(itemLongTapped(QModelIndex,QPointF)));

    connect(sceneManager(), SIGNAL(orientationChanged(M::Orientation)), this, SLOT(loadPicturesInVisibleItems()));

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
