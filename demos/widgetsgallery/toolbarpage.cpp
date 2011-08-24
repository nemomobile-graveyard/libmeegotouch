/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include <MApplication>
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
        if (MApplication::instance()->objectName() == "widgetsgallery") {
            contentItem->setStyleName("CommonContentItem");
        } else {
            contentItem->setStyleName("CommonContentItemInverted");
        }

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
    iconTabsTopAction(0),
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
    resetButton->setObjectName("resetButton");
    resetButton->setStyleName(inv("CommonSingleButton"));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    containerPolicy->addItem(resetButton, Qt::AlignCenter);

    callList = new MList;
    callList->setObjectName("callList");
    cellCreator = new MContentItemCreator;
    callList->setCellCreator(cellCreator);
    callModel = new TestModel;
    callModel->setObjectName("callModel");
    callList->setItemModel(callModel);
    containerPolicy->addItem(callList);
    showCallDataAsGrid();

    toolsConfiguration();

    toolsAction = new MAction(this);
    toolsAction->setObjectName("toolsAction");
    toolsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(toolsAction, SIGNAL(triggered()), this, SLOT(toolsConfiguration()));
    addAction(toolsAction);

    textButtonsAction = new MAction(this);
    textButtonsAction->setObjectName("textButtonsAction");
    textButtonsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(textButtonsAction, SIGNAL(triggered()), this, SLOT(textButtonsConfiguration()));
    addAction(textButtonsAction);

    textFieldAction = new MAction(this);
    textFieldAction->setObjectName("textFieldAction");
    textFieldAction->setLocation(MAction::ApplicationMenuLocation);
    connect(textFieldAction, SIGNAL(triggered()), this, SLOT(textFieldConfiguration()));
    addAction(textFieldAction);

    iconTabsAction = new MAction(this);
    iconTabsAction->setObjectName("iconTabsAction");
    iconTabsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconTabsAction, SIGNAL(triggered()), this, SLOT(iconTabsBottomConfiguration()));
    addAction(iconTabsAction);

    iconAndLabelTabsAction = new MAction(this);
    iconAndLabelTabsAction->setObjectName("iconAndLabelTabsAction");
    iconAndLabelTabsAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconAndLabelTabsAction, SIGNAL(triggered()), this, SLOT(iconAndLabelTabsBottomConfiguration()));
    addAction(iconAndLabelTabsAction);

    iconTabsTopAction = new MAction(this);
    iconTabsTopAction->setObjectName("iconTabsTopAction");
    iconTabsTopAction->setLocation(MAction::ApplicationMenuLocation);
    connect(iconTabsTopAction, SIGNAL(triggered()), this, SLOT(iconTabsTopConfiguration()));
    addAction(iconTabsTopAction);

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
    //% "Label tabs (+icon in portrait)"
    iconAndLabelTabsAction->setText(qtTrId("xx_toolbar_page_icon_and_label_tabs"));
    //% "Tabs (with icons in portrait)"
    iconTabsAction->setText(qtTrId("xx_toolbar_page_icon_tabs"));
    //% "Icon tabs (top)"
    iconTabsTopAction->setText(qtTrId("xx_toolbar_page_icon_tabs_top"));

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
    layoutCheckboxes->setContentsMargins(0, 0, 0, 0);
    layoutCheckboxes->setSpacing(0);

    visibleBackButton = new MButton();
    visibleBackButton->setObjectName("visibleBackButton");
    visibleBackButton->setStyleName(inv("CommonRightCheckBox"));
    visibleBackButton->setViewType(MButton::checkboxType);
    visibleBackButton->setCheckable(true);
    visibleBackButton->setChecked(true);
    connect(visibleBackButton, SIGNAL(toggled(bool)), this, SLOT(setBackButtonVisible(bool)));
    visibleBackButtonLabel = new MLabel();
    visibleBackButtonLabel->setObjectName("visibleBackButtonLabel");
    visibleBackButtonLabel->setStyleName(inv("CommonSingleTitle"));
    visibleBackButtonLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    visibleBackButtonLabel->setWordWrap(false);
    visibleBackButtonLabel->setTextElide(true);

    visibleMenuButton = new MButton();
    visibleMenuButton->setObjectName("visibleMenuButton");
    visibleMenuButton->setStyleName(inv("CommonRightCheckBox"));
    visibleMenuButton->setViewType(MButton::checkboxType);
    visibleMenuButton->setCheckable(true);
    visibleMenuButton->setChecked(true);
    connect(visibleMenuButton, SIGNAL(toggled(bool)), this, SLOT(setMenuActionsVisible(bool)));
    visibleMenuButtonLabel = new MLabel();
    visibleMenuButtonLabel->setObjectName("visibleMenuButtonLabel");
    visibleMenuButtonLabel->setStyleName(inv("CommonSingleTitle"));
    visibleMenuButtonLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    visibleMenuButtonLabel->setWordWrap(false);
    visibleMenuButtonLabel->setTextElide(true);

    QGraphicsLinearLayout *layoutBackCheckbox = new QGraphicsLinearLayout(Qt::Horizontal);
    layoutBackCheckbox->setContentsMargins(0, 0, 0, 0);
    layoutBackCheckbox->setSpacing(0);
    layoutBackCheckbox->addItem(visibleBackButtonLabel);
    layoutBackCheckbox->addItem(visibleBackButton);
    layoutBackCheckbox->setAlignment(visibleBackButtonLabel, Qt::AlignCenter);
    layoutBackCheckbox->setAlignment(visibleBackButton, Qt::AlignCenter);
    layoutCheckboxes->addItem(layoutBackCheckbox);

    QGraphicsLinearLayout *layoutMenuCheckbox = new QGraphicsLinearLayout(Qt::Horizontal);
    layoutMenuCheckbox->setContentsMargins(0, 0, 0, 0);
    layoutMenuCheckbox->setSpacing(0);
    layoutMenuCheckbox->addItem(visibleMenuButtonLabel);
    layoutMenuCheckbox->addItem(visibleMenuButton);
    layoutMenuCheckbox->setAlignment(visibleMenuButtonLabel, Qt::AlignCenter);
    layoutMenuCheckbox->setAlignment(visibleMenuButton, Qt::AlignCenter);
    layoutCheckboxes->addItem(layoutMenuCheckbox);

    containerPolicy->addItem(layoutCheckboxes);
}

void ToolBarPage::setupSliders()
{
    QGraphicsLinearLayout *layoutSliders = new QGraphicsLinearLayout(Qt::Vertical);
    layoutSliders->setContentsMargins(0, 0, 0, 0);
    layoutSliders->setSpacing(0);

    maxToolsLabel = new MLabel;
    maxToolsLabel->setObjectName("maxToolsLabel");
    maxToolsLabel->setStyleName(inv("CommonFieldLabel"));
    maxToolsLabel->setTextElide(true);
    layoutSliders->addItem(maxToolsLabel);
    maxToolsSlider = new MSlider;
    maxToolsSlider->setObjectName("maxToolsSlider");
    maxToolsSlider->setStyleName(inv("CommonSlider"));
    maxToolsSlider->setRange(0, 5);
    maxToolsSlider->setValue(2);
    maxToolsSlider->setMinLabelVisible(false);
    maxToolsSlider->setMaxLabelVisible(false);
    maxToolsSlider->setHandleLabelVisible(true);
    maxToolsSlider->setMinLabel(QString::number(maxToolsSlider->minimum()));
    maxToolsSlider->setMaxLabel(QString::number(maxToolsSlider->maximum()));
    QObject::connect(maxToolsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTools(int)));
    layoutSliders->addItem(maxToolsSlider);

    maxTabsLabel = new MLabel;
    maxTabsLabel->setObjectName("maxTabsLabel");
    maxTabsLabel->setStyleName(inv("CommonFieldLabel"));
    maxTabsLabel->setTextElide(true);
    layoutSliders->addItem(maxTabsLabel);
    maxTabsSlider = new MSlider;
    maxTabsSlider->setObjectName("maxTabsSlider");
    maxTabsSlider->setStyleName(inv("CommonSlider"));
    maxTabsSlider->setRange(0, 4);
    maxTabsSlider->setValue(3);
    maxTabsSlider->setMinLabelVisible(false);
    maxTabsSlider->setMaxLabelVisible(false);
    maxTabsSlider->setHandleLabelVisible(true);
    maxTabsSlider->setMinLabel(QString::number(maxTabsSlider->minimum()));
    maxTabsSlider->setMaxLabel(QString::number(maxTabsSlider->maximum()));
    QObject::connect(maxTabsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTabs(int)));
    layoutSliders->addItem(maxTabsSlider);

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
        QString invertedIcons[5] = {"icon-m-toolbar-list-white",
                                    "icon-m-toolbar-grid-white",
                                    "icon-m-toolbar-reply-white",
                                    "icon-m-toolbar-stop-white",
                                    "icon-m-toolbar-forward-white"};

        for (int i=currentCount; i<qMin(5, count); i++) {
            MAction *action;
            if (MApplication::instance()->objectName() == "widgetsgallery") {
                action = new MAction(icons[i], "", this);
            } else {
                action = new MAction(invertedIcons[i], "", this);
            }
            action->setObjectName("configurationAction");
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
    action->setObjectName("saveAction");
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(disappear()));

    //% "Cancel"
    action = new MAction(qtTrId("xx_toolbar_page_tools_cancel"), this);
    action->setObjectName("cancelAction");
    action->setLocation(MAction::ToolBarLocation);
    addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(reset()));

    currentConfiguration = textButtons;
}

void ToolBarPage::textFieldConfiguration()
{
    initDefaultViewToolBar();

    MTextEdit *entry = new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    entry->setObjectName("entry");
    entry->setViewType("toolbar");
    MWidgetAction *action = new MWidgetAction(this);
    action->setObjectName("action");
    action->setLocation(MAction::ToolBarLocation);
    action->setWidget(entry);
    addAction(action);

    currentConfiguration = textField;
}

void ToolBarPage::iconAndLabelTabsBottomConfiguration()
{
    currentConfiguration = iconAndLabelTabsBottom;
    initTabViewToolBar(false, true);
}

void ToolBarPage::iconTabsBottomConfiguration()
{
    currentConfiguration = iconTabsBottom;
    initTabViewToolBar(false, false);
}

void ToolBarPage::iconTabsTopConfiguration()
{
    currentConfiguration = iconTabsTop;
    initTabViewToolBar(true, true);
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
    if (!currentStyleName.isEmpty())
        applicationWindow()->setStyleName(currentStyleName);
    applicationWindow()->setToolbarViewType(MToolBar::defaultType);
}

void ToolBarPage::initTabViewToolBar(bool onTop, bool labels)
{
    if (onTop) {
        currentStyleName = "CommonApplicationWindowTabBarOnTop";
        if (!MApplication::activeWindow()->isFullScreen())
            MApplication::activeWindow()->showFullScreen();
    } else {
        if (MApplication::activeWindow()->isFullScreen() && !wasFullScreen)
            MApplication::activeWindow()->showNormal();
        currentStyleName = labels ? "CommonApplicationWindow" : "CommonApplicationWindowTabBarLabelessIconTabs";
    }
    currentStyleName = inv(currentStyleName);
    applicationWindow()->setStyleName(currentStyleName);

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
            MAction *action = new MAction(icons ? "icon-m-telephony-callhistory-initiated" : "", qtTrId("xx_toolbar_page_tools_initiated"), this);
            action->setObjectName("callInitiatedAction");
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
            MAction *action = new MAction(icons ? "icon-m-telephony-callhistory-received" : "", qtTrId("xx_toolbar_page_tools_received"), this);
            action->setObjectName("callReceivedAction");
            action->setLocation(MAction::ToolBarLocation);
            action->setCheckable(true);
            connect(action, SIGNAL(triggered()), this, SLOT(populateCallReceived()));
            addAction(action);
            break;
        }
    case 3:
        {
            //% "Missed"
            MAction *action = new MAction(icons ? "icon-m-telephony-callhistory-missed" : "", qtTrId("xx_toolbar_page_tools_missed"), this);
            action->setObjectName("callMissedAction");
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
            action->setObjectName("callDivertedAction");
            action->setCheckable(true);
            action->setLocation(MAction::ToolBarLocation);
            addAction(action);
            break;
        }
    case 5:
        {
            //% "Call"
            MAction *action = new MAction(icons ? "icon-m-telephony-call" : "", qtTrId("xx_toolbar_page_tools_call"), this);
            action->setObjectName("telephonyCellularAction");
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
    return ((applicationWindow()->toolbarViewType() == MToolBar::tabType) &&
            (currentConfiguration == iconAndLabelTabsTop ||
             currentConfiguration == iconTabsTop ||
             currentConfiguration == textTabsTop));
}
