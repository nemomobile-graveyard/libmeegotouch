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

#ifndef COMBOBOXPAGE_H
#define COMBOBOXPAGE_H

#include "templatepage.h"
#include <QAbstractListModel>

class MComboBox;
class MPopupList;
class MButton;
class QStringListModel;
class QSortFilterProxyModel;

class ComboBoxPage : public TemplatePage
{
    Q_OBJECT

public:
    ComboBoxPage();
    virtual ~ComboBoxPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    void retranslateUi();

private:
    MComboBox  *comboBox1;
    MComboBox  *comboBox2;
    MComboBox  *comboBox3;

    QStringListModel *model1;
    QSortFilterProxyModel *proxyModel;

};

#endif

