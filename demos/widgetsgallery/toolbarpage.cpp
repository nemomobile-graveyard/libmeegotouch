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

#include "toolbarpage.h"
#include <DuiAction>
#include <DuiButton>
#include <DuiTextEdit>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiWidgetAction>
#include <DuiLayout>
#include <DuiSeparator>
#include <DuiToolBar>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLinearLayoutPolicy>

#include <DuiContentItem>
#include <QDebug>

TestModel::TestModel(QObject *parent)
  : QAbstractListModel(parent), mode(InitiatedCall) {
    maxRowCount = 4;
    listNames << "Lauri" << "Santeri" << "Kaarle" << "imran" << "Iqbal" << "Jakub" << "Mihaly" << "Ilias" << "Antti" <<"Jani" << "Tomas" << "Denis";
}

int TestModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return maxRowCount;
}

QVariant TestModel::data(const QModelIndex &index, int role) const
{
 if (role == Qt::DisplayRole) {
     QStringList rowData;
     rowData << listNames[(mode*rowCount())+index.row()];

     switch(mode) {
         case TestModel::MissedCall: {
            rowData << "Missed Call";
            break;
         }
         case TestModel::ReceivedCall: {
            rowData << "Received Call";
            break;
         }
         case TestModel::InitiatedCall: {
            rowData << "Initiated Call";
            break;
         }
     }
     return QVariant(rowData);
 }

 return QVariant();
}

void TestModel::reloadData()
{
    if (rowCount() > 0 ) {
        emit dataChanged(index(0,0), index(rowCount()-1,0));
    }
}

void TestModel::setMode(TestModel::CallMode mode) {
   this->mode = mode;
}

class DuiContentItemCreator : public DuiAbstractCellCreator<DuiContentItem>
{
public:
    DuiContentItemCreator() {
    }

    void updateCell(const QModelIndex &index, DuiWidget *cell) const {
        DuiContentItem *contentItem = qobject_cast<DuiContentItem *>(cell);

        QVariant data = index.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        contentItem->setTitle(rowData[0]);
        contentItem->setSubtitle(rowData[1]);

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

ToolBarPage::ToolBarPage()
{
    gid = TemplatePage::ViewsAndDialogs;
}

ToolBarPage::~ToolBarPage()
{
    delete callModel;
}

QString ToolBarPage::timedemoTitle()
{
    return "ToolBar";
}

void ToolBarPage::createContent()
{
    TemplatePage::createContent();

    callList = new DuiList;
    cellCreator = new DuiContentItemCreator;
    callList->setCellCreator(cellCreator);
    callModel = new TestModel;
    callList->setItemModel(callModel);
    containerPolicy->addItem(callList);
    showCallDataAsList();

    //% "Example 1"
    exampleAction1 = new DuiAction(qtTrId("xx_toolbar_page_example1"), this);
    exampleAction1->setLocation(DuiAction::ApplicationMenuLocation);
    connect(exampleAction1, SIGNAL(triggered()), this, SLOT(fourButtons()));
    addAction(exampleAction1);

    //% "Example 2"
    exampleAction2 = new DuiAction(qtTrId("xx_toolbar_page_example2"), this);
    exampleAction2->setLocation(DuiAction::ApplicationMenuLocation);
    connect(exampleAction2, SIGNAL(triggered()), this, SLOT(textEntryWithTwoButtons()));
    addAction(exampleAction2);

    //% "Default View"
    defaultViewAction = new DuiAction(qtTrId("xx_toolbar_page_defaultview"), this);
    defaultViewAction->setLocation(DuiAction::ApplicationMenuLocation);
    connect(defaultViewAction, SIGNAL(triggered()), this, SLOT(selectToolbarDefaultView()));
    addAction(defaultViewAction);

    //% "Tab View"
    tabViewAction = new DuiAction(qtTrId("xx_toolbar_page_tabview"), this);
    tabViewAction->setLocation(DuiAction::ApplicationMenuLocation);
    connect(tabViewAction, SIGNAL(triggered()), this, SLOT(selectToolbarTabView()));
    addAction(tabViewAction);

    isDefaultView = false;
    selectToolbarDefaultView();

    retranslateUi();
}


void ToolBarPage::createLayout()
{
    TemplatePage::createLayout();
}

void ToolBarPage::retranslateUi()
{
    TemplatePage::retranslateUi();

    //% "ToolBar"
    setTitle(qtTrId("xx_toolbar_page_title"));
    if (!isContentCreated())
        return;

    //% "<b>Toolbar</b> acts as a container for actions. Depending upon the view attached, "
    //% "it either acts as a tab bar or is a container for buttons and text input field. "
    //% "<ul><li>Click Menu to explore the example</li></ul>"
    infoLabel->setText("<a></a>" + qtTrId("xx_toolbar_page_info"));
}

void ToolBarPage::fourButtons()
{
    clearToolbarActions();

    DuiAction* action = new DuiAction("icon-m-list", "", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showCallDataAsList()));

    action = new DuiAction("icon-m-grid", "", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showCallDataAsGrid()));

}

void ToolBarPage::textEntryWithTwoButtons()
{
    clearToolbarActions();
    addTextEntry();
    DuiAction *action = new DuiAction("Icon-video", "Video", this);
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);
    DuiAction *actionSMS = new DuiAction("Icon-new-SMS", "SMS", this);
    actionSMS->setLocation(DuiAction::ToolBarLocation);
    insertAction(action, actionSMS);
}

void ToolBarPage::addTextEntry()
{
    DuiTextEdit *entry = new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());
    entry->setViewType("toolbar");
    DuiWidgetAction *action = new DuiWidgetAction(this);
    action->setLocation(DuiAction::ToolBarLocation);
    action->setWidget(entry);
    addAction(action);
}

void ToolBarPage::selectToolbarDefaultView()
{
    if (!isDefaultView) {
        ((DuiApplicationWindow*)DuiApplication::activeWindow())->setToolbarViewType(DuiToolBar::defaultType);
        exampleAction1->setVisible(true);
        exampleAction2->setVisible(true);
        fourButtons();
        isDefaultView = true;
    }
}

void ToolBarPage::selectToolbarTabView()
{
    if (isDefaultView) {
        ((DuiApplicationWindow*)DuiApplication::activeWindow())->setToolbarViewType(DuiToolBar::tabType);
        exampleAction1->setVisible(false);
        exampleAction2->setVisible(false);
        addButtonsToTabView();
        isDefaultView = false;
    }

}

void ToolBarPage::addButtonsToTabView()
{
    clearToolbarActions();

    DuiAction *action = new DuiAction("icon-m-telephony-call-initiated", NULL, this);
    action->setLocation(DuiAction::ToolBarLocation);
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, SIGNAL(triggered()), this, SLOT(populateCallInitiated()));
    addAction(action);

    action = new DuiAction("icon-m-telephony-call-received", NULL, this);
    action->setLocation(DuiAction::ToolBarLocation);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(populateCallReceived()));
    addAction(action);

    action = new DuiAction("icon-m-telephony-call-missed", NULL, this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(populateCallMissed()));
    action->setLocation(DuiAction::ToolBarLocation);
    addAction(action);

    populateCallInitiated();
}

void ToolBarPage::populateCallInitiated()
{
    callModel->setMode(TestModel::InitiatedCall);
    callModel->reloadData();
}

void ToolBarPage::populateCallReceived()
{
    callModel->setMode(TestModel::ReceivedCall);
    callModel->reloadData();
}

void ToolBarPage::populateCallMissed()
{
    callModel->setMode(TestModel::MissedCall);
    callModel->reloadData();
}

void ToolBarPage::showCallDataAsList()
{
    cellCreator->setColumns(1);
    callList->setColumns(1);
}

void ToolBarPage::showCallDataAsGrid()
{
    cellCreator->setColumns(2);
    callList->setColumns(2);
}

void ToolBarPage::clearToolbarActions()
{
    QList<QAction *> acts = actions();
    int actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        DuiAction *action = qobject_cast<DuiAction *>(acts[i]);
        if (action && action->location().testFlag(DuiAction::ToolBarLocation)) {
            removeAction(action);
        }
    }
    DuiApplicationWindow* window = ((DuiApplicationWindow*)DuiApplication::activeWindow());
    acts = window->actions();
    actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        window->removeAction(acts[i]);
    }
}
