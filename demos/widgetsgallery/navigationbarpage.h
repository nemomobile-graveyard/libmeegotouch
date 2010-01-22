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

#ifndef NAVIGATIONBARPAGE_H
#define NAVIGATIONBARPAGE_H

#include "templatepage.h"

class DuiButton;
class DuiComboBox;
class DuiLabel;

class NavigationBarPage : public TemplatePage
{
    Q_OBJECT
public:
    enum ButtonIndex {
        Auto = 0,
        ManualBack,
        CloseWindow
    };

    enum DisplayModeComboBoxIndex {
        ComboShow = 0,
        ComboAutoHide,
        ComboHide
    };

    NavigationBarPage();
    virtual ~NavigationBarPage();
    virtual QString timedemoTitle();

    virtual void createContent();

public slots:
    void setButtonsState();
    void changeEscapeMode(int index);
    void changeNavigationBarDisplayMode(int index);
    void changeEscapeButtonDisplayMode(int index);
    void changeHomeButtonDisplayMode(int index);

protected:
    virtual void retranslateUi();

private:
    void retranslateDisplayModeComboBox(DuiComboBox *combo);
    void addExampleActions();

    DuiComboBox *comboNavigationBarDisplayMode;
    DuiComboBox *comboEscapeButtonDisplayMode;
    DuiComboBox *comboHomeButtonDisplayMode;
    DuiComboBox *comboEscapeMode;
    DuiLabel *lblDisplayMode;
    DuiLabel *lblEscapeModeDesc;
};

#endif // NAVIGATIONBARPAGE_H
