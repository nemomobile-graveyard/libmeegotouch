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

#ifndef TOOLBARPAGE_H
#define TOOLBARPAGE_H

#include "templatepage.h"
#include <DuiList>

class DuiToolBar;
class DuiGridLayoutPolicy;
class DuiList;

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

class DuiContentItemCreator;
class ToolBarPage : public TemplatePage
{
    Q_OBJECT

public:
    ToolBarPage();
    virtual ~ToolBarPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

private slots:
    void fourButtons();
    void textEntryWithTwoButtons();
    void selectToolbarDefaultView();
    void selectToolbarTabView();
    void populateCallMissed();
    void populateCallReceived();
    void populateCallInitiated();
    void showCallDataAsList();
    void showCallDataAsGrid();

private:
    void addTextEntry();
    void clearToolbarActions();
    void addButtonsToTabView();

    DuiList* callList;
    TestModel* callModel;
    DuiContentItemCreator * cellCreator;

    DuiAction * exampleAction1;
    DuiAction * exampleAction2;
    DuiAction * defaultViewAction;
    DuiAction * tabViewAction;

    bool isDefaultView;

};

#endif
