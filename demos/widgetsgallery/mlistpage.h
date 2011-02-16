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

#ifndef MLISTPAGE_H
#define MLISTPAGE_H

#include "templatepage.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QTimer>
#include <QPointer>

class PhoneBookModel;
class MSortFilterProxyModel;
class PhoneBookImageLoader;

class MDialog;
class MList;
class MComboBox;
class MListContentItemCreator;
class MAbstractItemModel;
class MObjectMenu;

class MListPage: public TemplatePage
{
    Q_OBJECT

public:
    enum MListPageSorting {
        None,
        Ascending,
        Descending
    };

    enum MListPageListMode {
        Plain,
        Grouped
    };

    MListPage();
    virtual ~MListPage();
    virtual QString timedemoTitle();

    virtual void createContent();

    void createActions();
    void createObjectMenuActions();

public slots:
    void loadPicturesInVisibleItems();
    void setPlainListModel();

    void changeSortingOrder(int index);
    void changeAmountOfItemInList(int index);
    void changeListMode(int index);
    void changeAmountOfColumns(int index);
    void changeSelectionMode(int index);
    void changeSeparatorsMode(int index);
    void changeListIndexVisibility(int index);
    void changeLiveFilteringMode(int index);
    void changeGroupHeadersMode(int index);

    void scrollToBottom();
    void scrollToTop();

    void itemClick(const QModelIndex &index);
    void itemLongTapped(const QModelIndex &index, const QPointF &position);

    void removeListItem();
    void editListItem();

    void liveFilteringTextChanged();
    void resetLiveFilteringTextChangeTimer();
    void liveFilteringTextChangeTimeout();
    void filteringVKB();
    void hideEmptyTextEdit();

    void showAdvancedConfigurationDialog();

protected:
    void retranslateUi();

private:
    MComboBox *createComboBoxAction(const QString &title, const QStringList &itemsList);
    MComboBox *createComboBoxLabelButton(const QString &title, const QStringList &itemsList, QGraphicsWidget *parent);
    void showTextEdit(bool show);

private:

    PhoneBookModel *model;
    MSortFilterProxyModel *proxyModel;
    PhoneBookImageLoader *imageLoader;

    MComboBox *comboListMode;

    MObjectMenu *objectMenu;

    MAction *actionAdvancedConfiguration;
    QPointer<MDialog> dialogAdvancedConfiguration;

    MList *list;
    MListContentItemCreator *cellCreator;

    QModelIndex longTappedIndex;
    QTimer liveFilteringTextChangeTimer;
    int currentSortingIndex;
    int currentListModeIndex;
};


#endif
