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

#include "toolbarpage.h"
#include <MAction>
#include <MButton>
#include <MTextEdit>
#include <MLocale>
#include <MLabel>
#include <MWidgetAction>
#include <MLayout>
#include <MSeparator>
#include <MToolBar>
#include <MApplication>
#include <MApplicationWindow>
#include <MLinearLayoutPolicy>

#include <MContentItem>
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

class MContentItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
    MContentItemCreator() : amountOfColumns( 0 ) {
    }

    void updateCell(const QModelIndex &index, MWidget *cell) const {
        MContentItem *contentItem = qobject_cast<MContentItem *>(cell);

        QVariant data = index.data(Qt::DisplayRole);
        QStringList rowData = data.value<QStringList>();
        contentItem->setTitle(rowData[0]);
        contentItem->setSubtitle(rowData[1]);

        updateContentItemMode(index, contentItem);
    }

    void updateContentItemMode(const QModelIndex &index, MContentItem *contentItem) const {
        int row = index.row();

        bool thereIsNextRow;

        if (amountOfColumns == 1) {
            thereIsNextRow = index.sibling(row + 1, 0).isValid();

            if (row == 0) {
                contentItem->setItemMode(MContentItem::SingleColumnTop);
            } else if (thereIsNextRow) {
                contentItem->setItemMode(MContentItem::SingleColumnCenter);
            } else {
                contentItem->setItemMode(MContentItem::SingleColumnBottom);
            }
        } else {
            bool left = ((row % 2) == 0);
            thereIsNextRow = left ? index.sibling(row + 2, 0).isValid() : index.sibling(row + 1, 0).isValid();
            if (row == 0 || row == 1) {
                left ? contentItem->setItemMode(MContentItem::TopLeft) : contentItem->setItemMode(MContentItem::TopRight);
            } else if (thereIsNextRow) {
                left ? contentItem->setItemMode(MContentItem::Left) : contentItem->setItemMode(MContentItem::Right);
            } else {
                left ? contentItem->setItemMode(MContentItem::BottomLeft) : contentItem->setItemMode(MContentItem::BottomRight);
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

ToolBarPage::ToolBarPage() :
    TemplatePage(TemplatePage::ApplicationView),
    callList(0),
    callModel(0),
    cellCreator(0),
    exampleAction1(0),
    exampleAction2(0),
    defaultViewAction(0),
    tabViewAction(0),
    isDefaultView(true)
{
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

    callList = new MList;
    cellCreator = new MContentItemCreator;
    callList->setCellCreator(cellCreator);
    callModel = new TestModel;
    callList->setItemModel(callModel);
    containerPolicy->addItem(callList);
    showCallDataAsList();

    //% "Example 1"
    exampleAction1 = new MAction(qtTrId("xx_toolbar_page_example1"), this);
    exampleAction1->setLocation(MAction::ApplicationMenuLocation);
    connect(exampleAction1, SIGNAL(triggered()), this, SLOT(fourButtons()));
    addAction(exampleAction1);

    //% "Example 2"
    exampleAction2 = new MAction(qtTrId("xx_toolbar_page_example2"), this);
    exampleAction2->setLocation(MAction::ApplicationMenuLocation);
    connect(exampleAction2, SIGNAL(triggered()), this, SLOT(textEntryWithTwoButtons()));
    addAction(exampleAction2);

    //% "Default View"
    defaultViewAction = new MAction(qtTrId("xx_toolbar_page_defaultview"), this);
    defaultViewAction->setLocation(MAction::ApplicationMenuLocation);
    connect(defaultViewAction, SIGNAL(triggered()), this, SLOT(selectToolbarDefaultView()));
    addAction(defaultViewAction);

    //% "Tab View"
    tabViewAction = new MAction(qtTrId("xx_toolbar_page_tabview"), this);
    tabViewAction->setLocation(MAction::ApplicationMenuLocation);
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

    MAction* action = new MAction("icon-m-toolbar-list", "", this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showCallDataAsList()));

    action = new MAction("icon-m-toolbar-grid", "", this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(showCallDataAsGrid()));

}

void ToolBarPage::textEntryWithTwoButtons()
{
    clearToolbarActions();
    addTextEntry();
    MAction *action = new MAction("icon-m-toolbar-camera", "Camera", this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    MAction *actionGallery = new MAction("icon-m-toolbar-gallery", "Gallery", this);
    actionGallery->setLocation(MAction::ToolBarLocation);
    insertAction(action, actionGallery);
}

void ToolBarPage::addTextEntry()
{
    MTextEdit *entry = new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    entry->setViewType("toolbar");
    MWidgetAction *action = new MWidgetAction(this);
    action->setLocation(MAction::ToolBarLocation);
    action->setWidget(entry);
    addAction(action);
}

void ToolBarPage::selectToolbarDefaultView()
{
    if (!isDefaultView) {
        ((MApplicationWindow*)MApplication::activeWindow())->setToolbarViewType(MToolBar::defaultType);
        exampleAction1->setVisible(true);
        exampleAction2->setVisible(true);
        fourButtons();
        isDefaultView = true;
    }
}

void ToolBarPage::selectToolbarTabView()
{
    if (isDefaultView) {
        ((MApplicationWindow*)MApplication::activeWindow())->setToolbarViewType(MToolBar::tabType);
        exampleAction1->setVisible(false);
        exampleAction2->setVisible(false);
        addButtonsToTabView();
        isDefaultView = false;
    }

}

void ToolBarPage::addButtonsToTabView()
{
    clearToolbarActions();

    MAction *action = new MAction("icon-m-telephony-call-initiated", NULL, this);
    action->setLocation(MAction::ToolBarLocation);
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, SIGNAL(triggered()), this, SLOT(populateCallInitiated()));
    addAction(action);

    action = new MAction("icon-m-telephony-call-received", NULL, this);
    action->setLocation(MAction::ToolBarLocation);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(populateCallReceived()));
    addAction(action);

    action = new MAction("icon-m-telephony-call-missed", NULL, this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(populateCallMissed()));
    action->setLocation(MAction::ToolBarLocation);
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
        MAction *action = qobject_cast<MAction *>(acts[i]);
        if (action && action->location().testFlag(MAction::ToolBarLocation)) {
            removeAction(action);
        }
    }
    MApplicationWindow* window = ((MApplicationWindow*)MApplication::activeWindow());
    acts = window->actions();
    actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        window->removeAction(acts[i]);
    }
}
