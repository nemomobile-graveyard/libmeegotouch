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

#ifndef TOOLBARPAGE_H
#define TOOLBARPAGE_H

#include "templatepage.h"
#include <MList>

class MToolBar;
class MGridLayoutPolicy;
class MList;
class MSlider;

class TestModel : public QAbstractListModel
{
   Q_OBJECT

public:
   enum CallMode {
       MissedCall,
       ReceivedCall,
       InitiatedCall
   };
   TestModel(QObject *parent = 0);

   int rowCount(const QModelIndex &parent = QModelIndex()) const;

   QVariant data(const QModelIndex &index, int role) const;

   void reloadData();

   void setMode(CallMode mode);

private:
   CallMode mode;
   int maxRowCount;
   QStringList listNames;

};

class MContentItemCreator;

class ToolBarPage : public TemplatePage
{
    Q_OBJECT

public:
    enum configuration {
        tools = 0,
        textButtons,
        textField,
        iconAndLabelTabsBottom,
        iconTabsBottom,
        textTabsBottom,
        iconAndLabelTabsTop,
        iconTabsTop,
        textTabsTop
    };

    ToolBarPage();
    virtual ~ToolBarPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

private slots:
    void populateCallMissed();
    void populateCallReceived();
    void populateCallInitiated();
    void showCallDataAsList();
    void showCallDataAsGrid();

    void onAppear();
    void onDisappear();
    void reset();

    void toolsConfiguration();
    void textButtonsConfiguration();
    void textFieldConfiguration();
    void iconAndLabelTabsBottomConfiguration();
    void iconTabsBottomConfiguration();
    void textTabsBottomConfiguration();
    void iconAndLabelTabsTopConfiguration();
    void iconTabsTopConfiguration();
    void textTabsTopConfiguration();

    void setBackButtonVisible(bool visible);
    void setMenuActionsVisible(bool visible);

    void updateTools(int count);
    void updateTabs(int count);

private:
    void clearToolbarActions();
    void setupCheckBoxes();
    void setupSliders();
    int buttonCount();
    void removeLast(int count = 1);
    void toolsConfiguration(int count);
    void initDefaultViewToolBar();
    void initTabViewToolBar(bool onTop, bool icons, bool labels);
    void addTab(int tabNumber);
    bool isTabBarOnTop();

    MList* callList;
    TestModel* callModel;
    MContentItemCreator * cellCreator;

    QString defaultStyleName;
    QString currentStyleName;
    configuration currentConfiguration;
    bool wasFullScreen;

    MAction *toolsAction;
    MAction *textButtonsAction;
    MAction *textFieldAction;
    MAction *iconAndLabelTabsAction;
    MAction *iconTabsAction;
    MAction *textTabsAction;
    MAction *iconAndLabelTabsTopAction;
    MAction *iconTabsTopAction;
    MAction *textTabsTopAction;

    MButton *visibleBackButton;
    MLabel  *visibleBackButtonLabel;
    MButton *visibleMenuButton;
    MLabel  *visibleMenuButtonLabel;

    MLabel  *maxToolsLabel;
    MSlider *maxToolsSlider;
    MLabel  *maxTabsLabel;
    MSlider *maxTabsSlider;

    MButton *resetButton;
};

#endif
