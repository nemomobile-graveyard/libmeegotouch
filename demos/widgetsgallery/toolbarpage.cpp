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
#include <MSlider>
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>
#include <QDebug>


TestModel::TestModel(QObject *parent)
  : QAbstractListModel(parent),
    mode(InitiatedCall),
    maxRowCount(4),
    listNames()
{
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
    if (rowCount() > 0) {
        emit dataChanged(index(0,0), index(rowCount()-1,0));
    }
}

void TestModel::setMode(TestModel::CallMode mode)
{
    this->mode = mode;
}

class MContentItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
    MContentItemCreator() : amountOfColumns( 1 ) {}

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
    currentConfiguration(tools),
    wasFullScreen(false),
    toolsAction(0),
    textButtonsAction(0),
    textFieldAction(0),
    iconAndLabelTabsAction(0),
    iconTabsAction(0),
    textTabsAction(0),
    iconAndLabelTabsTopAction(0),
    iconTabsTopAction(0),
    textTabsTopAction(0),
    visibleBackButton(0),
    visibleBackButtonLabel(0),
    visibleMenuButton(0),
    visibleMenuButtonLabel(0),
    maxToolsLabel(0),
    maxToolsSlider(0),
    maxTabsLabel(0),
    maxTabsSlider(0),
    resetButton(0)
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

    setupCheckBoxes();
    setupSliders();

    resetButton = new MButton();
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    containerPolicy->addItem(resetButton, Qt::AlignCenter);

    callList = new MList;
    cellCreator = new MContentItemCreator;
    callList->setCellCreator(cellCreator);
    callModel = new TestModel;
    callList->setItemModel(callModel);
    containerPolicy->setSpacing(20);
    containerPolicy->addItem(callList);
    showCallDataAsGrid();

    toolsConfiguration();

    toolsAction = new MAction(this);
    toolsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(toolsAction, SIGNAL(triggered()), this, SLOT(toolsConfiguration()));
    addAction(toolsAction);

    textButtonsAction = new MAction(this);
    textButtonsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(textButtonsAction, SIGNAL(triggered()), this, SLOT(textButtonsConfiguration()));
    addAction(textButtonsAction);

    textFieldAction = new MAction(this);
    textFieldAction->setLocation(MAction::ApplicationMenuLocation);
    connect(textFieldAction, SIGNAL(triggered()), this, SLOT(textFieldConfiguration()));
    addAction(textFieldAction);

    iconTabsAction = new MAction(this);
    iconTabsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconTabsAction, SIGNAL(triggered()), this, SLOT(iconTabsBottomConfiguration()));
    addAction(iconTabsAction);

    textTabsAction = new MAction(this);
    textTabsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(textTabsAction, SIGNAL(triggered()), this, SLOT(textTabsBottomConfiguration()));
    addAction(textTabsAction);

    iconAndLabelTabsAction = new MAction(this);
    iconAndLabelTabsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconAndLabelTabsAction, SIGNAL(triggered()), this, SLOT(iconAndLabelTabsBottomConfiguration()));
    addAction(iconAndLabelTabsAction);

    iconTabsTopAction = new MAction(this);
    iconTabsTopAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconTabsTopAction, SIGNAL(triggered()), this, SLOT(iconTabsTopConfiguration()));
    addAction(iconTabsTopAction);

    textTabsTopAction = new MAction(this);
    textTabsTopAction->setLocation(MAction::ApplicationMenuLocation);
    connect(textTabsTopAction, SIGNAL(triggered()), this, SLOT(textTabsTopConfiguration()));
    addAction(textTabsTopAction);

    iconAndLabelTabsTopAction = new MAction(this);
    iconAndLabelTabsTopAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconAndLabelTabsTopAction, SIGNAL(triggered()), this, SLOT(iconAndLabelTabsTopConfiguration()));
    addAction(iconAndLabelTabsTopAction);

    connect(this, SIGNAL(appearing()), this, SLOT(onAppear()));
    connect(this, SIGNAL(disappearing()), this, SLOT(onDisappear()));

    retranslateUi();
}


void ToolBarPage::createLayout()
{
    TemplatePage::createLayout();
}

void ToolBarPage::retranslateUi()
{
    //% "ToolBar"
    setTitle(qtTrId("xx_toolbar_page_title"));
    if (!isContentCreated())
        return;

    //% "<b>Toolbar</b> acts as a container for actions. Depending upon the view attached, "
    //% "it either acts as a tab bar or is a container for buttons and text input field. "
    //% "<ul><li>Click Menu to explore the example</li></ul>"
    infoLabel->setText("<a></a>" + qtTrId("xx_toolbar_page_info"));

    //% "Tools"
    toolsAction->setText(qtTrId("xx_toolbar_page_tools"));
    //% "Text buttons"
    textButtonsAction->setText(qtTrId("xx_toolbar_page_text_buttons"));
    //% "Text field"
    textFieldAction->setText(qtTrId("xx_toolbar_page_text_field"));
    //% "Icon + label tabs in portrait (default)"
    iconAndLabelTabsAction->setText(qtTrId("xx_toolbar_page_icon_and_label_tabs"));
    //% "Icon tabs"
    iconTabsAction->setText(qtTrId("xx_toolbar_page_icon_tabs"));
    //% "Text tabs"
    textTabsAction->setText(qtTrId("xx_toolbar_page_text_tabs"));
    //% "Icon + label tabs in portrait (top)"
    iconAndLabelTabsTopAction->setText(qtTrId("xx_toolbar_page_icon_and_label_tabs_top"));
    //% "Icon tabs (top)"
    iconTabsTopAction->setText(qtTrId("xx_toolbar_page_icon_tabs_top"));
    //% "Text tabs (top)"
    textTabsTopAction->setText(qtTrId("xx_toolbar_page_text_tabs_top"));

    //% "Back button visible"
    visibleBackButtonLabel->setText(qtTrId("xx_toolbar_page_back_button_visible"));
    //% "Menu button visible"
    visibleMenuButtonLabel->setText(qtTrId("xx_toolbar_page_menu_button_visible"));

    //% "Tools:"
    maxToolsLabel->setText(qtTrId("xx_toolbar_page_max_tools"));
    //% "Tabs:"
    maxTabsLabel->setText(qtTrId("xx_toolbar_page_max_tabs"));

    //% "Reset settings"
    resetButton->setText(qtTrId("xx_toolbar_page_reset_settings"));
}

void ToolBarPage::setupCheckBoxes()
{
    QGraphicsLinearLayout *layoutCheckboxes = new QGraphicsLinearLayout(Qt::Vertical);

    visibleBackButton = new MButton();
    visibleBackButton->setViewType(MButton::checkboxType);
    visibleBackButton->setCheckable(true);
    visibleBackButton->setChecked(true);
    connect(visibleBackButton, SIGNAL(toggled(bool)), this, SLOT(setBackButtonVisible(bool)));
    visibleBackButtonLabel = new MLabel();
    visibleBackButtonLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    visibleBackButtonLabel->setWordWrap(false);
    visibleBackButtonLabel->setTextElide(true);

    visibleMenuButton = new MButton();
    visibleMenuButton->setViewType(MButton::checkboxType);
    visibleMenuButton->setCheckable(true);
    visibleMenuButton->setChecked(true);
    connect(visibleMenuButton, SIGNAL(toggled(bool)), this, SLOT(setMenuActionsVisible(bool)));
    visibleMenuButtonLabel = new MLabel();
    visibleMenuButtonLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    visibleMenuButtonLabel->setWordWrap(false);
    visibleMenuButtonLabel->setTextElide(true);

    QGraphicsLinearLayout *layoutBackCheckbox = new QGraphicsLinearLayout(Qt::Horizontal);
    layoutBackCheckbox->addItem(visibleBackButtonLabel);
    layoutBackCheckbox->addItem(visibleBackButton);
    layoutBackCheckbox->setAlignment(visibleBackButtonLabel, Qt::AlignCenter);
    layoutBackCheckbox->setAlignment(visibleBackButton, Qt::AlignCenter);
    layoutCheckboxes->addItem(layoutBackCheckbox);

    QGraphicsLinearLayout *layoutMenuCheckbox = new QGraphicsLinearLayout(Qt::Horizontal);
    layoutMenuCheckbox->addItem(visibleMenuButtonLabel);
    layoutMenuCheckbox->addItem(visibleMenuButton);
    layoutMenuCheckbox->setAlignment(visibleMenuButtonLabel, Qt::AlignCenter);
    layoutMenuCheckbox->setAlignment(visibleMenuButton, Qt::AlignCenter);
    layoutCheckboxes->addItem(layoutMenuCheckbox);

    containerPolicy->addItem(layoutCheckboxes);
}

void ToolBarPage::setupSliders()
{
    QGraphicsGridLayout *layoutSliders = new QGraphicsGridLayout;
    layoutSliders->setSpacing(0);
    layoutSliders->setColumnFixedWidth(1, 250);

    maxToolsLabel = new MLabel;
    maxToolsLabel->setTextElide(true);
    layoutSliders->addItem(maxToolsLabel, 0, 0);
    maxToolsSlider = new MSlider;
    maxToolsSlider->setRange(0, 5);
    maxToolsSlider->setValue(2);
    maxToolsSlider->setMinLabelVisible(true);
    maxToolsSlider->setMaxLabelVisible(true);
    maxToolsSlider->setHandleLabelVisible(true);
    maxToolsSlider->setMinLabel(QString::number(maxToolsSlider->minimum()));
    maxToolsSlider->setMaxLabel(QString::number(maxToolsSlider->maximum()));
    QObject::connect(maxToolsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTools(int)));
    layoutSliders->addItem(maxToolsSlider, 0, 1);

    maxTabsLabel = new MLabel;
    maxTabsLabel->setTextElide(true);
    layoutSliders->addItem(maxTabsLabel, 1, 0);
    maxTabsSlider = new MSlider;
    maxTabsSlider->setRange(0, 4);
    maxTabsSlider->setValue(3);
    maxTabsSlider->setMinLabelVisible(true);
    maxTabsSlider->setMaxLabelVisible(true);
    maxTabsSlider->setHandleLabelVisible(true);
    maxTabsSlider->setMinLabel(QString::number(maxTabsSlider->minimum()));
    maxTabsSlider->setMaxLabel(QString::number(maxTabsSlider->maximum()));
    QObject::connect(maxTabsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTabs(int)));
    layoutSliders->addItem(maxTabsSlider, 1, 1);

    containerPolicy->addItem(layoutSliders);
}

void ToolBarPage::onAppear()
{
    wasFullScreen = MApplication::activeWindow()->isFullScreen();
    setBackButtonVisible(visibleBackButton->isChecked());
    setMenuActionsVisible(visibleMenuButton->isChecked());
    defaultStyleName = applicationWindow()->styleName();
    if (!currentStyleName.isEmpty())
        applicationWindow()->setStyleName(currentStyleName);
    if (isTabBarOnTop() && !MApplication::activeWindow()->isFullScreen())
        MApplication::activeWindow()->showFullScreen();
}

void ToolBarPage::onDisappear()
{
    setBackButtonVisible(true);
    setMenuActionsVisible(true);
    applicationWindow()->setStyleName(defaultStyleName);
    MWindow *window = MApplication::activeWindow();
    if (!wasFullScreen && window && window->isFullScreen())
        window->showNormal();
}

void ToolBarPage::reset()
{
    visibleBackButton->setChecked(true);
    visibleMenuButton->setChecked(true);
    maxToolsSlider->setValue(2);
    maxTabsSlider->setValue(3);
    toolsConfiguration();
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

void ToolBarPage::setBackButtonVisible(bool visible)
{
    setComponentsDisplayMode(EscapeButton, visible ? MApplicationPageModel::Show : MApplicationPageModel::Hide);
}

void ToolBarPage::setMenuActionsVisible(bool visible)
{
    QList<QAction *> acts = actions();
    int actsSize = acts.size();
    for (int i = 0; i < actsSize; ++i) {
        MAction *action = qobject_cast<MAction *>(acts[i]);
        if (action && action->location().testFlag(MAction::ApplicationMenuLocation)) {
            action->setVisible(visible);
        }
    }
}

void ToolBarPage::updateTools(int count)
{
    maxToolsSlider->setHandleLabel(QString::number(count));

    if (currentConfiguration == tools)
        toolsConfiguration(count);
}

void ToolBarPage::updateTabs(int count)
{
    maxTabsSlider->setHandleLabel(QString::number(count));

    if (((MApplicationWindow*)MApplication::activeWindow())->toolbarViewType() == MToolBar::tabType) {
        int currentCount = buttonCount();
        if (count > currentCount) {
            for (int i=currentCount+1; i<=count; i++)
                addTab(i);
        } else if (count < currentCount) {
            removeLast(currentCount-count);
        }
    }
}

void ToolBarPage::toolsConfiguration()
{
    initDefaultViewToolBar();
    toolsConfiguration(maxToolsSlider->value());
}

void ToolBarPage::toolsConfiguration(int count)
{
    currentConfiguration = tools;
    int currentCount = buttonCount();

    if (currentCount < count) {
        QString icons[5] = {"icon-m-toolbar-list",
                            "icon-m-toolbar-grid",
                            "icon-m-toolbar-reply",
                            "icon-m-toolbar-stop",
                            "icon-m-toolbar-forward"};

        for (int i=currentCount; i<qMin(5, count); i++) {
            MAction *action = new MAction(icons[i], "", this);
            action->setLocation(MAction::ToolBarLocation);
            addAction(action);
            switch (i) {
            case 0:
                connect(action, SIGNAL(triggered()), this, SLOT(showCallDataAsList()));
                break;
            case 1:
                connect(action, SIGNAL(triggered()), this, SLOT(showCallDataAsGrid()));
                break;
            default:
                break;
            }
        }
    } else if (currentCount > count){
        removeLast(currentCount - count);
    }
}

void ToolBarPage::textButtonsConfiguration()
{
    initDefaultViewToolBar();

    //% "Save"
    MAction* action = new MAction(qtTrId("xx_toolbar_page_tools_save"), this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(disappear()));

    //% "Cancel"
    action = new MAction(qtTrId("xx_toolbar_page_tools_cancel"), this);
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(reset()));

    currentConfiguration = textButtons;
}

void ToolBarPage::textFieldConfiguration()
{
    initDefaultViewToolBar();

    MTextEdit *entry = new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    entry->setViewType("toolbar");
    MWidgetAction *action = new MWidgetAction(this);
    action->setLocation(MAction::ToolBarLocation);
    action->setWidget(entry);
    addAction(action);

    currentConfiguration = textField;
}

void ToolBarPage::iconAndLabelTabsBottomConfiguration()
{
    currentConfiguration = iconAndLabelTabsBottom;
    initTabViewToolBar(false, true, true);
}

void ToolBarPage::iconTabsBottomConfiguration()
{
    currentConfiguration = iconTabsBottom;
    initTabViewToolBar(false, true, false);
}

void ToolBarPage::textTabsBottomConfiguration()
{
    currentConfiguration = textTabsBottom;
    initTabViewToolBar(false, false, true);
}

void ToolBarPage::iconAndLabelTabsTopConfiguration()
{
    currentConfiguration = iconAndLabelTabsTop;
    initTabViewToolBar(true, true, true);
}

void ToolBarPage::iconTabsTopConfiguration()
{
    currentConfiguration = iconTabsTop;
    initTabViewToolBar(true, true, false);
}

void ToolBarPage::textTabsTopConfiguration()
{
    currentConfiguration = textTabsTop;
    initTabViewToolBar(true, false, true);
}

int ToolBarPage::buttonCount()
{
    int count = 0;
    QList<QAction *> acts = actions();
    for (int i = 0; i < acts.size(); ++i) {
        MAction *action = qobject_cast<MAction *>(acts[i]);
        if (action && action->location().testFlag(MAction::ToolBarLocation)) {
            count++;
        }
    }
    return count;
}

void ToolBarPage::removeLast(int count)
{
    int removedCnt = 0;
    QList<QAction *> acts = actions();
    int actsSize = acts.size();
    for (int i = actsSize-1; i >= 0; i--) {
        MAction *action = qobject_cast<MAction *>(acts[i]);
        if (action && action->location().testFlag(MAction::ToolBarLocation)) {
            removeAction(action);
            removedCnt++;
            if (removedCnt >= count)
                break;
        }
    }
}

void ToolBarPage::initDefaultViewToolBar()
{
    clearToolbarActions();
    if (MApplication::activeWindow()->isFullScreen() && !wasFullScreen)
        MApplication::activeWindow()->showNormal();
    currentStyleName = defaultStyleName;
    applicationWindow()->setStyleName(currentStyleName);
    ((MApplicationWindow*)MApplication::activeWindow())->setToolbarViewType(MToolBar::defaultType);
}

void ToolBarPage::initTabViewToolBar(bool onTop, bool icons, bool labels)
{
    if (onTop) {
        if (!MApplication::activeWindow()->isFullScreen())
            MApplication::activeWindow()->showFullScreen();
        currentStyleName = (icons && labels) ? "CommonApplicationWindowTabBarOnTop" : (icons ? "IconTabsTop" : "LabelTabsTop");
        applicationWindow()->setStyleName(currentStyleName);
    } else {
        if (MApplication::activeWindow()->isFullScreen() && !wasFullScreen)
            MApplication::activeWindow()->showNormal();
        currentStyleName = (icons && labels) ? defaultStyleName : (icons ? "IconTabsBottom" : "LabelTabsBottom");
        applicationWindow()->setStyleName(currentStyleName);
    }

    clearToolbarActions();
    ((MApplicationWindow*)MApplication::activeWindow())->setToolbarViewType(MToolBar::tabType);

    for (int i=1; i<=maxTabsSlider->value(); i++)
        addTab(i);
}

void ToolBarPage::addTab(int tabNumber)
{
    bool icons = false;

    switch (currentConfiguration) {
    case iconAndLabelTabsBottom:
    case iconAndLabelTabsTop:
    case iconTabsBottom:
    case iconTabsTop:
        icons = true;
        break;
    case textTabsBottom:
    case textTabsTop:
        icons = false;
        break;
    default:
        return;
    }

    switch (tabNumber) {
    case 1:
        {
            //% "Initiated"
            MAction *action = new MAction(icons ? "icon-m-telephony-call-initiated" : "", qtTrId("xx_toolbar_page_tools_initiated"), this);
            action->setLocation(MAction::ToolBarLocation);
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, SIGNAL(triggered()), this, SLOT(populateCallInitiated()));
            addAction(action);
            populateCallInitiated();
            break;
        }
    case 2:
        {
            //% "Received"
            MAction *action = new MAction(icons ? "icon-m-telephony-call-received" : "", qtTrId("xx_toolbar_page_tools_received"), this);
            action->setLocation(MAction::ToolBarLocation);
            action->setCheckable(true);
            connect(action, SIGNAL(triggered()), this, SLOT(populateCallReceived()));
            addAction(action);
            break;
        }
    case 3:
        {
            //% "Missed"
            MAction *action = new MAction(icons ? "icon-m-telephony-call-missed" : "", qtTrId("xx_toolbar_page_tools_missed"), this);
            action->setCheckable(true);
            connect(action, SIGNAL(triggered()), this, SLOT(populateCallMissed()));
            action->setLocation(MAction::ToolBarLocation);
            addAction(action);
            break;
        }
    case 4:
        {
            //% "Diverted"
            MAction *action = new MAction(icons ? "icon-m-telephony-call-diverted" : "", qtTrId("xx_toolbar_page_tools_diverted"), this);
            action->setCheckable(true);
            action->setLocation(MAction::ToolBarLocation);
            addAction(action);
            break;
        }
    case 5:
        {
            //% "Call"
            MAction *action = new MAction(icons ? "icon-m-telephony-cellular" : "", qtTrId("xx_toolbar_page_tools_call"), this);
            action->setCheckable(true);
            action->setLocation(MAction::ToolBarLocation);
            addAction(action);
            break;
        }
    default:
        break;
    }
}

bool ToolBarPage::isTabBarOnTop()
{
    return ((((MApplicationWindow*)MApplication::activeWindow())->toolbarViewType() == MToolBar::tabType) &&
            (currentConfiguration == iconAndLabelTabsTop ||
             currentConfiguration == iconTabsTop ||
             currentConfiguration == textTabsTop));
}
