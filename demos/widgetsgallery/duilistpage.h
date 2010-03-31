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

#ifndef DUILISTPAGE_H
#define DUILISTPAGE_H

#include "templatepage.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QTimer>

#ifdef HAVE_N900
class ContactModel;
class ContactImageLoader;
#else
class PhoneBookModel;
class PhoneBookSortedModel;
class PhoneBookImageLoader;
#endif

class DuiList;
class DuiComboBox;
class DuiListContentItemCreator;

class DuiListPage: public TemplatePage
{
    Q_OBJECT

public:
    enum DuiListPageSorting {
        None,
        Ascending,
        Descending
    };

    enum DuiListPageListMode {
        Plain,
        Grouped
    };

    DuiListPage();
    virtual ~DuiListPage();
    virtual QString timedemoTitle();

    virtual void createContent();

    void createActions();

public slots:
    void loadPicturesInVisibleItems();
    void setPlainListModel();

    void changeSortingOrder(int index);
#ifndef HAVE_N900
    void changeAmountOfItemInList(int index);
#endif
    void changeListMode(int index);
    void changeAmountOfColumns(int index);
    void changeSelectionMode(int index);
    void changeSeparatorsMode(int index);

    void scrollToBottom();
    void scrollToTop();

    void itemClick(const QModelIndex &index);

protected:
    void retranslateUi();

private:
    DuiComboBox *createComboBoxAction(const QString &title, const QStringList &itemsList);

private:

#ifdef HAVE_N900
    ContactModel* model;
    ContactImageLoader* imageLoader;
#else
    PhoneBookModel *model;
    PhoneBookSortedModel *proxyModel;
    PhoneBookImageLoader *imageLoader;
#endif

    DuiList *list;
    DuiListContentItemCreator *cellCreator;

    int currentSortingIndex;
};


#endif
