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

#ifndef DISPLAYMODESPAGE_H
#define DISPLAYMODESPAGE_H

#include "templatepage.h"

class MButton;
class MComboBox;
class MLabel;
class QGraphicsLinearLayout;

class LabeledCheckbox : public QGraphicsWidget
{
    Q_OBJECT
public:
    LabeledCheckbox(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~LabeledCheckbox();

    MButton *button;
    MLabel *label;
    QGraphicsLinearLayout *roundedCornersCheckboxLayout;
};

class DisplayModesPage : public TemplatePage
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

    DisplayModesPage();
    virtual ~DisplayModesPage();
    virtual QString timedemoTitle();

    virtual void createContent();

public slots:
    void changeNavigationBarDisplayMode(int index);
    void changeEscapeButtonDisplayMode(int index);
    void changeHomeButtonDisplayMode(int index);
    void changeFullScreenMode(bool fullScreen);
    void changeRoundedCorners(bool enable);
    void changeNavigationBarTransparency(bool transparent);

protected:
    virtual void retranslateUi();

private:
    void retranslateDisplayModeComboBox(MComboBox *combo);
    void addExampleActions();
    void createWindowStateWidgets();

    MComboBox *comboNavigationBarDisplayMode;
    MComboBox *comboEscapeButtonDisplayMode;
    MComboBox *comboHomeButtonDisplayMode;
    MLabel *lblDisplayMode;
    MLabel *lblWindowState;
    LabeledCheckbox *fullScreenCheckbox;
    LabeledCheckbox *navigationBarTransparencyCheckbox;
    LabeledCheckbox *roundedCornersCheckbox;
};

#endif // NAVIGATIONBARPAGE_H
