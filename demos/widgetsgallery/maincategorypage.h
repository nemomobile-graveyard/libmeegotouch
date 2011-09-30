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
#ifndef MAINCATEGORYPAGE_H
#define MAINCATEGORYPAGE_H

#include "templatepage.h"

#include <QModelIndex>

class MLinearLayoutPolicy;
class MList;

class QAbstractItemModel;

class MainCategoryPage : public TimedemoPage
{
    Q_OBJECT
public:
    MainCategoryPage(QAbstractItemModel* demosDataModel, const QModelIndex &parentIndex);

    QString timedemoTitle();

    void createContent();
    void populateLayout();

private Q_SLOTS:
    void galleryPageItemClicked(const QModelIndex &index);

private:
    QAbstractItemModel *dataModel;
    MList *list;

    MLinearLayoutPolicy *policy;
};

#endif // MAINCATEGORYPAGE_H
