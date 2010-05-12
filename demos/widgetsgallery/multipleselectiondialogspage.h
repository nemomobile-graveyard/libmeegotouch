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

#ifndef MULTIPLESELECTIONDIALOGSPAGE_H
#define MULTIPLESELECTIONDIALOGSPAGE_H

#include "templatepage.h"

#include <QModelIndex>
#include <QPointer>

class MDialog;
class MList;

class MultipleSelectionDialogsPage : public TemplatePage
{
    Q_OBJECT

public:
    MultipleSelectionDialogsPage();

    QString timedemoTitle();

    void createContent();
    void populateLayout();

    void openQuestionDialog();

private Q_SLOTS:
    void itemClicked(const QModelIndex &index);

private:
    MLinearLayoutPolicy *policy;
    MList *list;

    QPointer<MDialog> dialog;
};

#endif // MULTIPLESELECTIONDIALOGSPAGE_H
