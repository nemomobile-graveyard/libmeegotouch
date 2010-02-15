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

#ifndef LANGUAGEPAGE_H
#define LANGUAGEPAGE_H

#include "templatepage.h"
#include <QAbstractListModel>

class DuiButton;
class DuiComboBox;
class QStandardItemModel;

class LanguagePage : public TemplatePage
{
    Q_OBJECT

public:
    LanguagePage();
    virtual ~LanguagePage();

    virtual void createContent();

public slots:
    void changeLanguage(int index);

protected:
    void retranslateUi();

private:
    DuiComboBox  *comboBox;
    QStandardItemModel *model;
};

#endif

