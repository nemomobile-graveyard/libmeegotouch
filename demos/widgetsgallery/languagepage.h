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

#ifndef LANGUAGEPAGE_H
#define LANGUAGEPAGE_H

#include "templatepage.h"
#include <QAbstractListModel>

class MButton;
class MComboBox;
class QStandardItemModel;

class LanguagePage : public TemplatePage
{
    Q_OBJECT

public:
    LanguagePage();
    virtual ~LanguagePage();
    virtual QString timedemoTitle();

    virtual void createContent();

public slots:
    void changeLanguage(int index);
    void changeLcTime(int index);
    void changeLcTimeFormat24h(int index);
    void changeLcCollate(int index);
    void changeLcNumeric(int index);
    void changeLcMonetary(int index);

protected:
    void retranslateUi();

private:
    MComboBox *comboBoxLanguage;
    QStandardItemModel *modelLanguage;
    MComboBox *comboBoxLcTime;
    QStandardItemModel *modelLcTime;
    MComboBox *comboBoxLcTimeFormat24h;
    QStandardItemModel *modelLcTimeFormat24h;
    MComboBox *comboBoxLcCollate;
    QStandardItemModel *modelLcCollate;
    MComboBox *comboBoxLcNumeric;
    QStandardItemModel *modelLcNumeric;
    MComboBox *comboBoxLcMonetary;
    QStandardItemModel *modelLcMonetary;
    MLabel *labelHaveGconf;
    MLabel *labelHaveIcu;
    MLabel *labelExampleNumber;
    MLabel *labelExampleDateTime;
    MLabel *labelExampleWeekNumber;
    MLabel *labelExampleCurrency;
    MLabel *labelExampleTranslation1;
    MLabel *labelExampleTranslation2;
    MLabel *labelExampleTranslation3;
    MLabel *labelLtrTest;
    MLabel *labelRtlTest;
    MLabel *labelLtrTestRich;
    MLabel *labelRtlTestRich;
    MLabel *labelFontTest;
};

#endif

