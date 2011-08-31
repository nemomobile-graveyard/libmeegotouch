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

#ifndef QUERYDIALOGSPAGE_H
#define QUERYDIALOGSPAGE_H

#include "templatepage.h"

#include <QModelIndex>
#include <QPointer>

#ifdef HAVE_LIBNGF
#   include <dbus/dbus.h>
#   include <libngf/client.h>
#endif

class MDialog;
class MList;

class QueryDialogsPage : public TemplatePage
{
    Q_OBJECT

public:
    enum MessageBoxOption {
        Default = 0,
        Icon = 1,
        LargeText = 2,
        NgfEffect = 4,
        WithoutDefaultButtons = 8,
        NoAsDefaultButton = 16
    };
    Q_DECLARE_FLAGS(MessageBoxOptions, MessageBoxOption)

    QueryDialogsPage();
    virtual ~QueryDialogsPage();

    QString timedemoTitle();

    void createContent();
    void populateLayout();

    void openMessageBox(MessageBoxOptions options = Default);

protected:
    void retranslateUi();

private Q_SLOTS:
    void itemClicked(const QModelIndex &index);

private:
    MLinearLayoutPolicy *policy;
    MList *list;

    QPointer<MDialog> dialog;

#ifdef HAVE_LIBNGF
    DBusConnection *connection;
    NgfClient *ngfClient;
#endif
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QueryDialogsPage::MessageBoxOptions)

#endif // QUERYDIALOGSPAGE_H
